#pragma once
#include "Commons.h"
#include "Constants.h"
#include "GameScreen.h"
#include <SDL.h>
#include <string>

class Texture2D;
class GameScreen;

class ButtonUI
{
	public:
		ButtonUI(SDL_Renderer* renderer, GameScreen* attatchedScreen, Vector2D startPosition, std::string imagePath, float scale = 1.0f);
		~ButtonUI();

		void Update(float deltaTime, SDL_Event e);
		void Render();

		Vector2D position;
		bool ClickInBounds(Vector2D clickPos);

	private:
		SDL_Renderer* m_renderer;
		Texture2D* m_texture;
		GameScreen* m_attatchedScreen;
};