#include "GameScreen.h"
#include "Constants.h"
#include "Commons.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
#include <string>

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	m_renderer = renderer;
}

GameScreen::~GameScreen()
{
	m_renderer = nullptr;
}

void GameScreen::Render(){}

void GameScreen::Update(float deltaTime, SDL_Event e){}

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

	float tileScale = SCREEN_HEIGHT / rows / TILE_RESOLUTION;

	Vector2D tilePos = Vector2D(0, 0);

	for (int i = 0; i < levelString.size() - 1; i++)
	{
		if (levelString[i] == ' ')
		{
			tilePos.x += TILE_RESOLUTION * tileScale;
			continue;
		}
		else if (levelString[i] == '\n' )
		{
			tilePos.y += TILE_RESOLUTION * tileScale;
			tilePos.x = 0;

			continue;
		}

		Tile* currentTile = new Tile(m_renderer, tilePos, std::string("Tiles/") + levelString[i] + ".png", tileScale);
		tileMap.push_back(currentTile);

		tilePos.x += TILE_RESOLUTION * tileScale;
	}

	return tileScale;
}
