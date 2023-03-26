#pragma once
#include "Commons.h"
#include "GameScreen.h"
#include "Character.h"
#include <vector>
#include <string>

class Texture2D;
class Character;

class LevelScreen : GameScreen
{
	public:
		LevelScreen(SDL_Renderer* renderer, std::string levelPath, bool multiplayer);
		~LevelScreen();

		void Render() override;
		void Update(float deltaTime, SDL_Event e) override;

		void LoadFromJSON(std::string levelPath);

		std::vector<Tile*> tileMap;
		std::vector<Tile*> GetOnScreenTiles();

	private:
		std::vector<Character*> characters;

		bool SetUpLevel(bool multiplayer);
};