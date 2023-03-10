#pragma once
#include "Tile.h"
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
		std::vector<Tile*> tileMap;

	protected:
		SDL_Renderer* m_renderer;

};