/*#pragma once
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

		float LoadFromTXT(std::string levelPath);
		std::string GetFileName(tileData tileInfo, int index, std::string levelFile, int rows, int columns);
		int LongestLine(std::string str);

		std::vector<Tile*> tileMap;
		std::vector<Tile*> GetOnScreenTiles();

	private:
		std::vector<Character*> characters;

		bool SetUpLevel(float levelScale, bool multiplayer);
};
*/