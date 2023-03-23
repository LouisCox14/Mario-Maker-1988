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

		Vector2D cameraPosition;
		float levelWidth;
	protected:
		SDL_Renderer* m_renderer;
};