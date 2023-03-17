#pragma once
#include "Tile.h"
#include "TileData.h"
#include <SDL.h>
#include <string>
#include <vector>

class GameScreen
{
	public:
		GameScreen(SDL_Renderer* renderer);
		~GameScreen();

		virtual void Render();
		virtual void Update(float deltaTime, SDL_Event e);

		float LoadFromTXT(std::string levelPath);
		std::string GetFileName(tileData tileInfo, int index, std::string levelFile, int rows, int columns);
		int LongestLine(std::string str);

		std::vector<Tile*> tileMap;
		std::vector<Tile*> GetOnScreenTiles();

		Vector2D cameraPosition;
		float levelWidth;
	protected:
		SDL_Renderer* m_renderer;

};