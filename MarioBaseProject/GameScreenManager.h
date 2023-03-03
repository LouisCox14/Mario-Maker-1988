#pragma once
#include "GameScreen.h"
#include "Commons.h"
#include <SDL.h>

class GameScreen;

class GameScreenManager
{
	public:
		GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
		~GameScreenManager();

		void Render();
		void Update(float deltaTime, SDL_Event e);

		void ChangeScreen(SCREENS new_screen);

	private:
		SDL_Renderer* m_renderer;
		GameScreen* m_current_screen;
};