#pragma once
#include "GameScreen.h"
#include "Commons.h"
#include "Constants.h"
#include "TileData.h"
#include "EnemySpawner.h"
#include "ButtonUI.h"
#include <vector>
#include <string>
#include <map>

enum UI_TABS
{
	TERRAIN,
	SPECIAL,
	ENEMIES,
	GAME_LOGIC,
	DECORATIONS,
	SIDEBAR
};

enum GAME_LOGIC_OPTIONS
{
	START_POS
};

class LevelCreator : GameScreen
{
	public:
		LevelCreator(SDL_Renderer* renderer, GameScreenManager* _screenManager, std::string importPath = "");
		~LevelCreator();

		void Render() override;
		void Update(float deltaTime, SDL_Event e) override;
		void ButtonClicked(ButtonUI* button, bool leftClick) override;

		Vector2D camMovement;
		float camSpeed;
		float camScale;
		float scaleSpeed;

		int mouseX;
		int mouseY;
		bool leftMouseDown;
		bool rightMouseDown;

		std::map<UI_TABS, ButtonUI*> sidebarUI;
		std::vector<UI_TABS> renderOrderUI;
		UI_TABS activeTab = SIDEBAR;
		bool buttonClicked;
		std::vector<ButtonUI*> optionsDisplayed;

		UI_TABS selectedType;

		tileData selectedTile;
		EnemyType selectedEnemy;
		GAME_LOGIC_OPTIONS selectedGameOption;

		std::map<GAME_LOGIC_OPTIONS, Texture2D*> gameLogicIcons;
		Vector2D startPos;

		std::map<EnemyType, Texture2D*> enemyIcons;
		std::vector<std::pair<EnemyType, Vector2D>> enemies;

		std::vector<Tile*> tileMap;

		ButtonUI* exportButton;
		std::string exportPath;

	private:
		std::vector<std::string> GetTilePaths(std::vector<tileData> tileVector);
		std::vector<std::string> GetEnemyPaths();
		std::vector<std::string> GetGameLogicPaths();

		void LoadTabOptions(std::vector<std::string> filePaths);

		void ReloadNeighbouringComposites(Vector2D gridPosition, tileData tileInfo);
		std::vector<Tile*> GetNeighbouringComposites(Vector2D gridPosition, tileData tileInfo);
		Vector2D PixelToGridPos(Vector2D pixelPosition);
		Vector2D GridToPixelPos(Vector2D gridPosition);
		void PlaceTile(Vector2D gridPosition, tileData tileInfo);

		int GetTileIndexAtGridPos(Vector2D gridPosition);
		int GetEnemyIndexAtGridPos(Vector2D gridPosition);

		int GetButtonIndex(std::vector<ButtonUI*> buttonVector, ButtonUI* targetButton);
		void ExportFile();
		void ImportFile(std::string importPath);
};