#pragma once
#include "GameScreen.h"
#include "GameScreenManager.h"

class Titlescreen : GameScreen
{
	public:
		Titlescreen(SDL_Renderer* renderer, GameScreenManager* _screenManager);
		~Titlescreen();

		void Render() override;
		void Update(float deltaTime, SDL_Event e) override;

	private:
		Texture2D* background;
		Animator* backgroundAnimator;
		GameScreenManager* screenManager;

		std::string GetFileInput();
};