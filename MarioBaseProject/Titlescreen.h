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

		void ButtonClicked(ButtonUI* button, bool leftClick) override;

	private:
		Texture2D* background;
		Texture2D* curtain;
		Texture2D* title;
		Texture2D* version;

		float curtainDrawTime;
		float curtainPosition;
		float titleFloatOffset;
		float titleFloatSpeed;

		ButtonUI* playButton;
		ButtonUI* multiplayerButton;
		ButtonUI* editButton;
		ButtonUI* createButton;

		GameScreenManager* screenManager;
		bool buttonClicked;

		std::string GetFileInput();
};