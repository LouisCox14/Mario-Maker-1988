#pragma once
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"

class Texture2D;
class Character;

class GameScreenLevel1 : GameScreen
{
	public:
		GameScreenLevel1(SDL_Renderer* renderer);
		~GameScreenLevel1();

		void Render() override;
		void Update(float deltaTime, SDL_Event e) override;

	private:
		Character* my_character;

		bool SetUpLevel(float levelScale);
};

