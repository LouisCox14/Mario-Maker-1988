#pragma once
#include "Tile.h"
#include "TileData.h"
#include "ButtonUI.h"
#include <SDL.h>
#include <string>
#include <vector>

class ButtonUI;

class GameScreen
{
	public:
		GameScreen(SDL_Renderer* renderer);
		~GameScreen();

		virtual void Render();
		virtual void Update(float deltaTime, SDL_Event e);
		virtual void ButtonClicked(ButtonUI* button, bool leftClick);

		Vector2D cameraPosition;
		float levelWidth;
	protected:
		SDL_Renderer* m_renderer;
};