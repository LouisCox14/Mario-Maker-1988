#pragma once
#include "Commons.h"
#include "GameScreen.h"
#include <vector>
#include <string>

class Texture2D;
class Character;

class LevelScreen : GameScreen
{
	public:
		LevelScreen(SDL_Renderer* renderer, GameScreenManager* _screenManager, std::string _levelPath, bool multiplayer);
		~LevelScreen();

		void Render() override;
		void Update(float deltaTime, SDL_Event e) override;

		void GameOver();
		bool gameOver;

		void LoadFromJSON();
		std::string levelPath;

		std::vector<Tile*> tileMap;
		std::vector<Tile*> GetOnScreenTiles();

		int levelWidth;
		int levelHeight;

	private:
		std::vector<Character*> characters;

		bool SetUpLevel(bool multiplayer);
};