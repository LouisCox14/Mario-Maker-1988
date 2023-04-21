#pragma once
#include "Commons.h"
#include "GameScreen.h"
#include "Enemy.h"
#include <vector>
#include <string>

class Texture2D;
class Character;
class EnemySpawner;

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

		std::vector<Enemy*> enemies;
		std::vector<EnemySpawner*> enemySpawners;

		int levelWidth;
		int levelHeight;

		Vector2D startPos;
		std::vector<Character*> characters;

	private:
		bool SetUpLevel(bool multiplayer);
};