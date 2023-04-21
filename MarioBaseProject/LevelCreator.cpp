#include "LevelCreator.h"
#include "Commons.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <json\json.h>
#include <iomanip>
#include <ctime>
#include <sstream>

LevelCreator::LevelCreator(SDL_Renderer* renderer, GameScreenManager* _screenManager, std::string importPath) : GameScreen(renderer, _screenManager)
{
	exportPath = importPath;

	camSpeed = 75.0f * CAMERA_SCALE;
	camScale = 2.0f;
	scaleSpeed = 0.15f;

	exportButton = new ButtonUI(renderer, this, Vector2D(0, 0), std::string("Assets/UI/Level Creator/Export Button.png"));

	sidebarUI.insert({ TERRAIN, new ButtonUI(renderer, this, Vector2D(0, 16), std::string("Assets/UI/Level Creator/Terrain Tab.png")) });
	sidebarUI.insert({ SPECIAL, new ButtonUI(renderer, this, Vector2D(0, 64), std::string("Assets/UI/Level Creator/Special Tab.png")) });
	sidebarUI.insert({ ENEMIES, new ButtonUI(renderer, this, Vector2D(0, 112), std::string("Assets/UI/Level Creator/Enemies Tab.png")) });
	sidebarUI.insert({ GAME_LOGIC, new ButtonUI(renderer, this, Vector2D(0, 160), std::string("Assets/UI/Level Creator/Game Logic Tab.png")) });
	sidebarUI.insert({ DECORATIONS, new ButtonUI(renderer, this, Vector2D(0, 208), std::string("Assets/UI/Level Creator/Decorations Tab.png")) });
	sidebarUI.insert({ SIDEBAR, new ButtonUI(renderer, this, Vector2D(0, 0), std::string("Assets/UI/Level Creator/Sidebar.png")) });

	renderOrderUI = { TERRAIN, SPECIAL, ENEMIES, GAME_LOGIC, DECORATIONS, SIDEBAR };

	enemyIcons.insert({GOOMBA, new Texture2D(renderer, CAMERA_SCALE)});
	enemyIcons.at(GOOMBA)->LoadFromFile("Assets/Sprites/Goomba/Goomba.png");

	gameLogicIcons.insert({ START_POS, new Texture2D(renderer, CAMERA_SCALE) });
	gameLogicIcons.at(START_POS)->LoadFromFile("Assets/Sprites/Small Mario/Idle.png");

	selectedTile = terrainTiles[0];
	selectedEnemy = GOOMBA;
	selectedGameOption = START_POS;

	startPos = GridToPixelPos(Vector2D(2, 10));
	if (importPath != "")
	{
		ImportFile(importPath);
	}

	audioPlayer = new AudioPlayer({ "NewLevel", "ButtonClicked"}, {"LevelCreatorMusic"});
	audioPlayer->PlayMusic("LevelCreatorMusic");
	audioPlayer->PlayClip("NewLevel");
}

LevelCreator::~LevelCreator()
{
	m_renderer = nullptr;
}

void LevelCreator::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	Vector2D gridOffset = GameScreen::cameraPosition % (TILE_RESOLUTION * camScale) + 1;

	for (float x = 0; x < SCREEN_WIDTH + TILE_RESOLUTION * camScale; x += TILE_RESOLUTION * camScale)
	{
		SDL_RenderDrawLine(m_renderer, x - gridOffset.x, 0, x - gridOffset.x, SCREEN_HEIGHT);
	}

	for (float y = 0; y < SCREEN_HEIGHT + TILE_RESOLUTION * camScale; y += TILE_RESOLUTION * camScale)
	{
		SDL_RenderDrawLine(m_renderer, 0, y - gridOffset.y, SCREEN_WIDTH, y - gridOffset.y);
	}

	for (Tile* tile : tileMap)
	{
		tile->Render(GameScreen::cameraPosition);
	}

	for (std::pair<EnemyType, Vector2D> enemy : enemies)
	{
		enemyIcons.at(enemy.first)->Render(enemy.second - GameScreen::cameraPosition, SDL_FLIP_NONE);
	}

	gameLogicIcons.at(START_POS)->Render(startPos - GameScreen::cameraPosition, SDL_FLIP_NONE);

	for (UI_TABS tab : renderOrderUI)
	{
		sidebarUI.at(tab)->Render();
	}

	for (ButtonUI* tileOption : optionsDisplayed)
	{
		tileOption->Render();
	}

	exportButton->Render();
}

void LevelCreator::Update(float deltaTime, SDL_Event e)
{
	buttonClicked = false;

	if (activeTab == SIDEBAR)
	{
		optionsDisplayed.clear();
		sidebarUI.at(SIDEBAR)->position.x = SCREEN_WIDTH;
	}
	else
	{
		sidebarUI.at(SIDEBAR)->position.x = SCREEN_WIDTH - 80;
	}

	float sideBarPosition = sidebarUI.at(SIDEBAR)->position.x;

	for (UI_TABS button : renderOrderUI)
	{
		sidebarUI.at(button)->position.x = sideBarPosition - 13;
	}

	sidebarUI.at(activeTab)->position.x -= 20;
	sidebarUI.at(SIDEBAR)->position.x = sideBarPosition;

	for (std::pair<UI_TABS, ButtonUI*> button : sidebarUI)
	{
		button.second->Update(deltaTime, e);
	}

	for (ButtonUI* tileOption : optionsDisplayed)
	{
		tileOption->Update(deltaTime, e);
	}


	exportButton->Update(deltaTime, e);


	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2D mouseGridPos = PixelToGridPos(Vector2D((float)mouseX, (float)mouseY));

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	camMovement = Vector2D(0, 0);

	if (keystate[SDL_SCANCODE_A])
	{
		camMovement.x -= 1;
	}
	if (keystate[SDL_SCANCODE_D])
	{
		camMovement.x += 1;
	}
	if (keystate[SDL_SCANCODE_W])
	{
		camMovement.y -= 1;
	}
	if (keystate[SDL_SCANCODE_S])
	{
		camMovement.y += 1;
	}

	if (e.key.repeat == 0)
	{
		switch (e.type)
		{
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_e:
						ExportFile();
						break;
				}

				break;
			case SDL_MOUSEBUTTONDOWN:

				if (!buttonClicked)
				{
					activeTab = SIDEBAR;

					switch (e.button.button)
					{
						case SDL_BUTTON_LEFT:
							leftMouseDown = true;
							break;
						case SDL_BUTTON_RIGHT:
							rightMouseDown = true;
							break;
					}
					break;
				}
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT:
							leftMouseDown = false;
							break;
						case SDL_BUTTON_RIGHT:
							rightMouseDown = false;
							break;
				}
				break;
		}
	}

	if (activeTab == SIDEBAR)
	{
		if (leftMouseDown)
		{
			if (GetTileIndexAtGridPos(mouseGridPos) == -1 && GetEnemyIndexAtGridPos(mouseGridPos) == -1 && PixelToGridPos(startPos) != mouseGridPos)
			{
				switch (selectedType)
				{
				case ENEMIES:
					enemies.push_back(std::pair<EnemyType, Vector2D> {selectedEnemy, GridToPixelPos(mouseGridPos)});
					break;
				case GAME_LOGIC:
					switch (selectedGameOption)
					{
						case START_POS:
							startPos = GridToPixelPos(mouseGridPos);
							break;
					}
					break;
				default:
					PlaceTile(mouseGridPos, selectedTile);
					break;
				}
			}
		}
		else if (rightMouseDown)
		{
			int targetTile = GetTileIndexAtGridPos(mouseGridPos);
			if (targetTile != -1)
			{
				tileData tempTileInfo = tileMap[targetTile]->tileInfo;

				delete tileMap[targetTile];
				tileMap.erase(tileMap.begin() + targetTile);

				ReloadNeighbouringComposites(mouseGridPos, tempTileInfo);
			}
			else
			{
				int targetEnemy = GetEnemyIndexAtGridPos(mouseGridPos);
				if (targetEnemy != -1)
				{
					enemies.erase(enemies.begin() + targetEnemy);
				}
			}
		}
	}

	GameScreen::cameraPosition += camMovement * camSpeed * deltaTime;
	GameScreen::cameraPosition.x = std::max(GameScreen::cameraPosition.x, 0.0f);
	GameScreen::cameraPosition.y = std::max(GameScreen::cameraPosition.y, 0.0f);
}

void LevelCreator::ButtonClicked(ButtonUI* clickedButton, bool leftClick)
{
	audioPlayer->PlayClip("ButtonClicked");
	buttonClicked = true;
	if (leftClick)
	{
		for (std::pair<UI_TABS, ButtonUI*> button : sidebarUI)
		{
			if (button.second == clickedButton && button.first != SIDEBAR)
			{
				activeTab = button.first;

				switch (activeTab)
				{
					case TERRAIN:
						LoadTabOptions(GetTilePaths(terrainTiles));
						break;
					case SPECIAL:
						LoadTabOptions(GetTilePaths(specialTiles));
						break;
					case ENEMIES:
						LoadTabOptions(GetEnemyPaths());
						break;
					case GAME_LOGIC:
						LoadTabOptions(GetGameLogicPaths());
						break;
					case DECORATIONS:
						LoadTabOptions(GetTilePaths(decorationTiles));
						break;
				}
			}
		}

		if (clickedButton == exportButton)
		{
			ExportFile();
		}
		else if (GetButtonIndex(optionsDisplayed, clickedButton) != -1)
		{
			selectedType = activeTab;

			switch (activeTab)
			{
			case TERRAIN:
				selectedTile = terrainTiles[GetButtonIndex(optionsDisplayed, clickedButton)];
				break;
			case SPECIAL:
				selectedTile = specialTiles[GetButtonIndex(optionsDisplayed, clickedButton)];
				break;
			case DECORATIONS:
				selectedTile = decorationTiles[GetButtonIndex(optionsDisplayed, clickedButton)];
				break;
			case ENEMIES:
				selectedEnemy = static_cast<EnemyType>(GetButtonIndex(optionsDisplayed, clickedButton));
				break;
			case GAME_LOGIC:
				selectedGameOption = static_cast<GAME_LOGIC_OPTIONS>(GetButtonIndex(optionsDisplayed, clickedButton));
				break;
			}
		}
	}
}

int LevelCreator::GetButtonIndex(std::vector<ButtonUI*> buttonVector, ButtonUI* targetButton)
{
	auto it = std::find(buttonVector.cbegin(), buttonVector.cend(), targetButton);

	if (it != buttonVector.end())
	{
		return it - buttonVector.begin();
	}

	return -1;
}

std::vector<std::string> LevelCreator::GetTilePaths(std::vector<tileData> tileVector)
{
	std::vector<std::string> tilePaths = {};

	for (tileData tileInfo : tileVector)
	{
		std::string tempFileName = "Assets/Tiles/" + tileInfo.fileName;

		if (tileInfo.isAnimated)
		{
			tempFileName += " 1";
		}

		if (tileInfo.defaultSuffix != "")
		{
			tempFileName += tileInfo.defaultSuffix;
		}

		tempFileName += ".png";

		tilePaths.push_back(tempFileName);
	}

	return tilePaths;
}

std::vector<std::string> LevelCreator::GetEnemyPaths()
{
	std::vector<std::string> enemyPaths = {};

	for (std::pair<EnemyType, Texture2D*> enemy : enemyIcons)
	{
		enemyPaths.push_back(enemy.second->GetFilePath());
	}

	return enemyPaths;
}

std::vector<std::string> LevelCreator::GetGameLogicPaths()
{
	std::vector<std::string> gameLogicPaths = {};

	for (std::pair<GAME_LOGIC_OPTIONS, Texture2D*> gameLogicIcon : gameLogicIcons)
	{
		gameLogicPaths.push_back(gameLogicIcon.second->GetFilePath());
	}

	return gameLogicPaths;
}

void LevelCreator::LoadTabOptions(std::vector<std::string> filePaths)
{
	optionsDisplayed.clear();

	int yOffset = 16;

	for (std::string filePath : filePaths)
	{
		optionsDisplayed.push_back(new ButtonUI(m_renderer, this, Vector2D(SCREEN_WIDTH - 64, yOffset), filePath, 3.0f));

		yOffset += 64;
	}
}

Vector2D LevelCreator::PixelToGridPos(Vector2D pixelPosition)
{
	pixelPosition += GameScreen::cameraPosition;
	Vector2D gridPosition = pixelPosition / (TILE_RESOLUTION * camScale);
	return Vector2D((int)(gridPosition.x), (int)(gridPosition.y));
}

Vector2D LevelCreator::GridToPixelPos(Vector2D pixelPosition)
{
	return pixelPosition * TILE_RESOLUTION * camScale;
}


void LevelCreator::PlaceTile(Vector2D gridPosition, tileData tileInfo)
{
	tileMap.push_back(new Tile(m_renderer, GridToPixelPos(gridPosition), tileInfo, tileMap, camScale));

	ReloadNeighbouringComposites(gridPosition, tileInfo);
}

void LevelCreator::ReloadNeighbouringComposites(Vector2D gridPosition, tileData tileInfo)
{
	if (tileInfo.spriteType == COMPOSITE)
	{
		for (Tile* tile : GetNeighbouringComposites(gridPosition, tileInfo))
		{
			tile->LoadCompositeSprite(tileMap);
		}
	}

	if (tileInfo.colliderType == COMPOSITE)
	{
		for (Tile* tile : GetNeighbouringComposites(gridPosition, tileInfo))
		{
			tile->CalculateCompositeCollider(tileMap);
		}
	}
}

std::vector<Tile*> LevelCreator::GetNeighbouringComposites(Vector2D gridPosition, tileData tileInfo)
{
	std::vector<Tile*> neighbouringComposites;
	int tempIndex = 0;

	for (int i = -1; i < 2; i += 2)
	{
		tempIndex = GetTileIndexAtGridPos(gridPosition + Vector2D(0, 1) * i);
		if (tempIndex != -1 && tileMap[tempIndex]->tileInfo.fileName == tileInfo.fileName)
		{
			neighbouringComposites.push_back(tileMap[tempIndex]);
		}
	}
	for (int i = -1; i < 2; i += 2)
	{
		tempIndex = GetTileIndexAtGridPos(gridPosition + Vector2D(1, 0) * i);
		if (tempIndex != -1 && tileMap[tempIndex]->tileInfo.fileName == tileInfo.fileName)
		{
			neighbouringComposites.push_back(tileMap[tempIndex]);
		}
	}

	return neighbouringComposites;
}

int LevelCreator::GetTileIndexAtGridPos(Vector2D gridPosition)
{
	Vector2D pixelPosition = gridPosition * TILE_RESOLUTION * camScale;

	for (int i = 0; i < tileMap.size(); i++)
	{
		if (tileMap[i]->position == pixelPosition)
		{
			return i;
		}
	}

	return -1;
}

int LevelCreator::GetEnemyIndexAtGridPos(Vector2D gridPosition)
{
	Vector2D pixelPosition = gridPosition * TILE_RESOLUTION * camScale;

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i].second == pixelPosition)
		{
			return i;
		}
	}

	return -1;
}

void LevelCreator::ExportFile()
{
	if (exportPath == "")
	{
		const time_t t = std::time(nullptr);
		std::tm tm = {};

		localtime_s(&tm, &t);

		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		auto str = oss.str();

		exportPath = "../../MM88 Level (" + str + ").json";
	}

	std::ofstream fout(exportPath, std::ios::out);

	Json::Value levelObject;

	Json::Value startPosJSON;
	startPosJSON["x"] = startPos.x;
	startPosJSON["y"] = startPos.y;
	levelObject["Start Position"] = startPosJSON;

	Json::Value tilesJSON(Json::arrayValue);
	for (Tile* tile : tileMap)
	{
		Json::Value currentTile;
		currentTile["x"] = tile->position.x;
		currentTile["y"] = tile->position.y;
		currentTile["ImageDirectory"] = tile->tilePath;

		Json::Value collisionDirections(Json::arrayValue);

		for (COLLISION_SIDES collSide : tile->coll.collisionSides)
		{
			collisionDirections.append((int)collSide);
		}

		currentTile["CollisionDirections"] = collisionDirections;

		currentTile["TileDataReference"] = tile->tileInfo.fileName;

		tilesJSON.append(currentTile);
	}

	levelObject["Tiles"] = tilesJSON;

	Json::Value enemiesJSON(Json::arrayValue);
	for (std::pair<EnemyType, Vector2D> enemy : enemies)
	{
		Json::Value currentEnemy;
		currentEnemy["x"] = enemy.second.x;
		currentEnemy["y"] = enemy.second.y;
		currentEnemy["Type"] = static_cast<int>(enemy.first);

		enemiesJSON.append(currentEnemy);
	}

	levelObject["Enemies"] = enemiesJSON;

	Json::StyledWriter styledWriter;
	fout << styledWriter.write(levelObject);

	fout.close();

	GameScreen::screenManager->ChangeScreen(SCREEN_LEVEL, exportPath);
}

void LevelCreator::ImportFile(std::string importPath)
{
	std::ifstream fin(importPath, std::ios::in | std::ios::binary);

	if (!fin)
	{
		std::cout << "Could not access level file: " << importPath << std::endl;
	}

	Json::Value json;
	Json::Reader reader;

	reader.parse(fin, json);

	startPos = Vector2D(json["Start Position"]["x"].asFloat(), json["Start Position"]["y"].asFloat());

	for (Json::Value tile : json["Tiles"])
	{
		Vector2D tilePosition = Vector2D(tile["x"].asFloat(), tile["y"].asFloat());
		tileData tileInfo = allTiles.at(tile["TileDataReference"].asString());
		std::string filePath = tile["ImageDirectory"].asString();

		std::array<COLLISION_SIDES, 4> collSides = { NONE, NONE, NONE, NONE };
		for (int i = 0; i < 4; i++)
		{
			collSides[i] = static_cast<COLLISION_SIDES>(tile["CollisionDirections"][i].asInt());
		}

		tileMap.push_back(new Tile(m_renderer, tilePosition, tileInfo, filePath, collSides, CAMERA_SCALE));
	}

	for (Json::Value enemy : json["Enemies"])
	{
		Vector2D enemyPosition = Vector2D(enemy["x"].asFloat(), enemy["y"].asFloat());
		EnemyType enemyType = static_cast<EnemyType>(enemy["Type"].asInt());

		enemies.push_back(std::pair<EnemyType, Vector2D> {enemyType, enemyPosition});
	}

	fin.close();
}