/*#include "LevelScreen.h"
#include "Texture2D.h"
#include "SmallMario.h"
#include "SmallLuigi.h"
#include <iostream>
#include <fstream>

LevelScreen::LevelScreen(SDL_Renderer* renderer, std::string levelPath, bool multiplayer) : GameScreen(renderer)
{
	SetUpLevel(LoadFromTXT(levelPath), multiplayer);
}

LevelScreen::~LevelScreen()
{
	for (Character* character : characters)
	{
		delete character;
		character = nullptr;
	}
}

void LevelScreen::Render()
{

	// Draw the tilemap
	for (Tile* tile : GetOnScreenTiles())
	{
		tile->Render(GameScreen::cameraPosition);
	}

	//Draw the character
	for (Character* character : characters)
	{
		character->Render(GameScreen::cameraPosition);
	}
}

void LevelScreen::Update(float deltaTime, SDL_Event e)
{
	for (Character * character : characters)
	{
		character->Update(deltaTime, e, GetOnScreenTiles());
	}

	for (Tile* tile : GetOnScreenTiles())
	{
		if (tile->isAnimated)
		{
			tile->Update(deltaTime);
		}
	}

	float cameraX = 0;

	for (Character* character : characters)
	{
		cameraX += (character->GetPosition().x + (character->GetSize().x / 2)) / characters.size();
	}

	cameraX -= SCREEN_WIDTH / 2;

	cameraX = std::max(cameraX, 0.0f);
	cameraX = std::min(cameraX, (float)GameScreen::levelWidth - SCREEN_WIDTH);
	GameScreen::cameraPosition.x = cameraX;
}

bool LevelScreen::SetUpLevel(float levelScale, bool multiplayer)
{
	// Set up player character
	characters.push_back((Character*)new SmallMario(m_renderer, "Sprites/Small Mario/Idle.png", Vector2D(64, 64), levelScale, cameraPosition));

	if (multiplayer)
	{
		characters.push_back((Character*)new SmallLuigi(m_renderer, "Sprites/Small Luigi/Idle.png", Vector2D(64, 64), levelScale, cameraPosition));
	}

	return true;
}

float LevelScreen::LoadFromTXT(std::string levelPath)
{
	std::ifstream levelFile(levelPath);
	std::string levelString;

	if (levelFile.is_open()) {
		while (levelFile) {
			levelString += levelFile.get();
		}
	}

	int rows = std::count(levelString.begin(), levelString.end(), '\n');
	rows += (!levelString.empty() && levelString.back() != '\n');

	int columns = LongestLine(levelString);

	float tileScale = (float)SCREEN_HEIGHT / (float)TILE_RESOLUTION / (float)rows;
	levelWidth = (float)columns * (float)TILE_RESOLUTION * tileScale;

	Vector2D tilePos = Vector2D(0, 0);

	for (int row = 0; row < rows; row++)
	{
		int index = row * (columns + 1);
		int spaceRequired = -1;

		while (spaceRequired == -1)
		{
			if (index == levelString.size() - 1)
			{
				spaceRequired = 0;
				break;
			}
			else if (levelString[index] == '\n')
			{
				spaceRequired = columns - (index % (columns + 1));
				break;
			}

			index++;
		}

		if (spaceRequired > 0)
		{
			std::string whiteSpace(spaceRequired, ' ');
			levelString = levelString.insert(index, whiteSpace);
		}
	}

	for (int i = 0; i < levelString.size() - 1; i++)
	{
		if (levelString[i] == ' ')
		{
			tilePos.x += (float)TILE_RESOLUTION * tileScale;
			continue;
		}
		else if (levelString[i] == '\n')
		{
			tilePos.y += (float)TILE_RESOLUTION * tileScale;
			tilePos.x = 0;

			continue;
		}

		tileData tileInfo = tileDict.at(levelString[i]);

		Tile* currentTile = new Tile(m_renderer, tilePos, GetFileName(tileInfo, i, levelString, rows, columns), tileInfo, tileScale);
		tileMap.push_back(currentTile);

		tilePos.x += TILE_RESOLUTION * tileScale;
	}

	return tileScale;
}

std::string LevelScreen::GetFileName(tileData tileInfo, int index, std::string levelString, int rows, int columns)
{
	std::string tileName = tileInfo.fileName;

	if (tileInfo.autoTileSides[(int)TOP] == TOP)
	{
		if (index > columns)
		{
			if (levelString[index] != levelString[index - (columns + 1)])
			{
				tileName += " Top";
			}
		}
	}
	if (tileInfo.autoTileSides[(int)BOTTOM] == BOTTOM)
	{
		if (index < (columns + 1) * (rows - 1))
		{
			if (levelString[index] != levelString[index + (columns + 1)])
			{
				tileName += " Bottom";
			}
		}
	}
	if (tileInfo.autoTileSides[(int)RIGHT] == RIGHT && index != 0)
	{
		if ((index + 2) % (columns + 1) != 0)
		{
			if (levelString[index] != levelString[index + 1])
			{
				tileName += " Right";
			}
		}
	}
	if (tileInfo.autoTileSides[(int)LEFT] == LEFT && index != 0)
	{
		if (index % (columns + 1) != 0)
		{
			if (levelString[index] != levelString[index - 1])
			{
				tileName += " Left";
			}
		}
	}

	return std::string("Tiles/") + tileName + ".png";
}

int LevelScreen::LongestLine(std::string str)
{
	int largest = 0;
	int current = 0;

	for (int i = 0; i < str.size() - 1; i++)
	{
		if (str[i] == '\n')
		{
			if (current > largest)
			{
				largest = current;
			}

			current = 0;
		}
		else
		{
			current++;
		}
	}

	if (current > largest)
	{
		largest = current;
	}

	return largest;
}

std::vector<Tile*> LevelScreen::GetOnScreenTiles()
{
	std::vector<Tile*> onscreenTiles;

	for (Tile* tile : tileMap)
	{
		if (tile->position.x < cameraPosition.x + SCREEN_WIDTH && tile->position.x + tile->coll.size.x > cameraPosition.x)
		{
			onscreenTiles.push_back(tile);
		}
	}

	return onscreenTiles;
}
*/