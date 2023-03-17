#include "GameScreen.h"
#include "Constants.h"
#include "Commons.h"
#include "Tile.h"
#include "TileData.h"
#include <iostream>
#include <fstream>
#include <string>

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	m_renderer = renderer;
	cameraPosition = Vector2D(0, 0);
}

GameScreen::~GameScreen()
{
	m_renderer = nullptr;
}

void GameScreen::Render() {}

void GameScreen::Update(float deltaTime, SDL_Event e) {}

float GameScreen::LoadFromTXT(std::string levelPath)
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

	float tileScale = SCREEN_HEIGHT / TILE_RESOLUTION / rows;
	levelWidth = columns * TILE_RESOLUTION * tileScale;

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
			tilePos.x += TILE_RESOLUTION * tileScale;
			continue;
		}
		else if (levelString[i] == '\n')
		{
			tilePos.y += TILE_RESOLUTION * tileScale;
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

std::string GameScreen::GetFileName(tileData tileInfo, int index, std::string levelString, int rows, int columns)
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

int GameScreen::LongestLine(std::string str)
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

std::vector<Tile*> GameScreen::GetOnScreenTiles()
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
