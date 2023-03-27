#include "LevelCreator.h"
#include "Commons.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <json\json.h>

LevelCreator::LevelCreator(SDL_Renderer* renderer, std::string importPath) : GameScreen(renderer)
{
	camSpeed = 100.0f;
	camScale = 2.0f;
	scaleSpeed = 0.15f;

	UI.insert({ TERRAIN, new ButtonUI(renderer, this, Vector2D(0, 16), std::string("UI/Terrain Tab.png")) });
	UI.insert({ SPECIAL, new ButtonUI(renderer, this, Vector2D(0, 64), std::string("UI/Special Tab.png")) });
	UI.insert({ ENEMIES, new ButtonUI(renderer, this, Vector2D(0, 112), std::string("UI/Enemies Tab.png")) });
	UI.insert({ GAME_LOGIC, new ButtonUI(renderer, this, Vector2D(0, 160), std::string("UI/Game Logic Tab.png")) });
	UI.insert({ DECORATIONS, new ButtonUI(renderer, this, Vector2D(0, 208), std::string("UI/Decorations Tab.png")) });
	UI.insert({ SIDEBAR, new ButtonUI(renderer, this, Vector2D(0, 0), std::string("UI/Sidebar.png")) });

	renderOrderUI = { TERRAIN, SPECIAL, ENEMIES, GAME_LOGIC, DECORATIONS, SIDEBAR };

	selectedTile = terrainTiles[0];

	if (importPath != "")
	{
		ImportFile(importPath);
	}
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

	for (UI_TABS tab : renderOrderUI)
	{
		UI.at(tab)->Render();
	}

	for (ButtonUI* tileOption : optionsDisplayed)
	{
		tileOption->Render();
	}
}

void LevelCreator::Update(float deltaTime, SDL_Event e)
{
	buttonClicked = false;

	if (activeTab == SIDEBAR)
	{
		optionsDisplayed.clear();
		UI.at(SIDEBAR)->position.x = SCREEN_WIDTH;
	}
	else
	{
		UI.at(SIDEBAR)->position.x = SCREEN_WIDTH - 80;
	}

	float sideBarPosition = UI.at(SIDEBAR)->position.x;

	for (UI_TABS button : renderOrderUI)
	{
		UI.at(button)->position.x = sideBarPosition - 13;
	}

	UI.at(activeTab)->position.x -= 20;
	UI.at(SIDEBAR)->position.x = sideBarPosition;

	for (std::pair<UI_TABS, ButtonUI*> button : UI)
	{
		button.second->Update(deltaTime, e);
	}

	for (ButtonUI* tileOption : optionsDisplayed)
	{
		tileOption->Update(deltaTime, e);
	}


	SDL_GetMouseState(&mouseX, &mouseY);
	Vector2D mouseGridPos = PixelToGridPos(Vector2D((float)mouseX, (float)mouseY));

	if (e.key.repeat == 0)
	{
		switch (e.type)
		{
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_a:
						camMovement.x -= 1;
						break;
					case SDLK_d:
						camMovement.x += 1;
						break;
					case SDLK_w:
						camMovement.y -= 1;
						break;
					case SDLK_s:
						camMovement.y += 1;
						break;
					case SDLK_e:
						ExportFile();
						break;
				}

				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
					case SDLK_a:
						camMovement.x += 1;
						break;
					case SDLK_d:
						camMovement.x -= 1;
						break;
					case SDLK_w:
						camMovement.y += 1;
						break;
					case SDLK_s:
						camMovement.y -= 1;
						break;
				}

				break;
			case SDL_MOUSEBUTTONDOWN:

				if (!buttonClicked)
				{
					activeTab = SIDEBAR;
				}

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
			if (GetTileIndexAtGridPos(mouseGridPos) == -1)
			{
				PlaceTile(mouseGridPos, selectedTile);
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
		}
	}

	GameScreen::cameraPosition += camMovement * camSpeed * deltaTime;
	GameScreen::cameraPosition.x = std::max(GameScreen::cameraPosition.x, 0.0f);
	GameScreen::cameraPosition.y = std::max(GameScreen::cameraPosition.y, 0.0f);
}

void LevelCreator::ButtonClicked(ButtonUI* clickedButton, bool leftClick)
{
	buttonClicked = true;
	if (leftClick)
	{
		for (std::pair<UI_TABS, ButtonUI*> button : UI)
		{
			if (button.second == clickedButton && button.first != SIDEBAR)
			{
				activeTab = button.first;

				switch (activeTab)
				{
					case TERRAIN:
						LoadTileOptions(terrainTiles);
						break;
					case SPECIAL:
						LoadTileOptions(specialTiles);
						break;
					case ENEMIES:
						LoadTileOptions({});
						break;
					case GAME_LOGIC:
						LoadTileOptions({});
						break;
					case DECORATIONS:
						LoadTileOptions(decorationTiles);
						break;
				}
			}
		}

		if (GetButtonIndex(optionsDisplayed, clickedButton) != -1)
		{
			selectedTile = tileOptions[GetButtonIndex(optionsDisplayed, clickedButton)];
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

void LevelCreator::LoadTileOptions(std::vector<tileData> tileVector)
{
	tileOptions = tileVector;
	optionsDisplayed.clear();

	int yOffset = 16;

	for (tileData tileInfo : tileVector)
	{
		std::string tempFileName = "Tiles/" + tileInfo.fileName;

		if (tileInfo.isAnimated)
		{
			tempFileName += " 1";
		}

		if (tileInfo.defaultSuffix != "")
		{
			tempFileName += tileInfo.defaultSuffix;
		}

		tempFileName += ".png";

		optionsDisplayed.push_back(new ButtonUI(m_renderer, this, Vector2D(SCREEN_WIDTH - 64, yOffset), tempFileName, 3.0f));

		yOffset += 64;
	}
}

Vector2D LevelCreator::PixelToGridPos(Vector2D pixelPosition)
{
	pixelPosition += GameScreen::cameraPosition;
	Vector2D gridPosition = pixelPosition / (TILE_RESOLUTION * camScale);
	return Vector2D((int)(gridPosition.x), (int)(gridPosition.y));
}

void LevelCreator::PlaceTile(Vector2D gridPosition, tileData tileInfo)
{
	tileMap.push_back(new Tile(m_renderer, gridPosition * TILE_RESOLUTION * camScale, tileInfo, tileMap, camScale));

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

void LevelCreator::ExportFile()
{
	std::ofstream fout("Levels/Test.json", std::ios::out);

	Json::Value levelObject;

	int i = 0;
	Json::Value tiles(Json::arrayValue);
	for (Tile* tile : tileMap)
	{
		Json::Value currentTile;
		currentTile["x"] = tile->position.x;
		currentTile["y"] = tile->position.y;
		currentTile["ImageDirectory"] = tile->tilePath;

		Json::Value collisionDirections(Json::arrayValue);
		for (COLLISION_SIDES collSide : tile->coll.collisionSides)
		{
			collisionDirections.append(collSide);
		}
		currentTile["CollisionDirections"] = collisionDirections;

		currentTile["TileDataReference"] = tile->tileInfo.fileName;

		tiles.append(currentTile);
		i++;
	}

	levelObject["Tiles"] = tiles;

	Json::StyledWriter styledWriter;
	fout << styledWriter.write(levelObject);

	fout.close();
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

	fin.close();
}