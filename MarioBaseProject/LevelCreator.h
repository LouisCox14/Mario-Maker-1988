#pragma once
#include "GameScreen.h"
#include "Commons.h"
#include "Constants.h"
#include "TileData.h"
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

class LevelCreator : GameScreen
{
	public:
		LevelCreator(SDL_Renderer* renderer);
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

		std::map<UI_TABS, ButtonUI*> UI;
		std::vector<UI_TABS> renderOrderUI;
		UI_TABS activeTab = SIDEBAR;
		bool buttonClicked;
		std::vector<ButtonUI*> optionsDisplayed;
		std::vector<tileData> tileOptions;

		tileData selectedTile;

		std::vector<Tile*> tileMap;

	private:
		void LoadTileOptions(std::vector<tileData> tileVector);
		void ReloadNeighbouringComposites(Vector2D gridPosition, tileData tileInfo);
		std::vector<Tile*> GetNeighbouringComposites(Vector2D gridPosition, tileData tileInfo);
		Vector2D PixelToGridPos(Vector2D pixelPosition);
		void PlaceTile(Vector2D gridPosition, tileData tileInfo);
		int GetTileIndexAtGridPos(Vector2D gridPosition);
		int GetButtonIndex(std::vector<ButtonUI*> buttonVector, ButtonUI* targetButton);
		void ExportFile();
};