#pragma once
#include "Commons.h"
#include "Constants.h"
#include "TileData.h"
#include "GameScreen.h"
#include <vector>
#include <string>

class LevelCreator : GameScreen
{
	public:
		LevelCreator(SDL_Renderer* renderer);
		~LevelCreator();

		virtual void Render() override;
		virtual void Update(float deltaTime, SDL_Event e) override;

		Vector2D camMovement;
		float camSpeed;
		float camScale;
		float scaleSpeed;

		int mouseX;
		int mouseY;

		std::vector<Tile*> tileMap;

	private:
		Vector2D PixelToGridPos(Vector2D pixelPosition);
		void PlaceTile(Vector2D gridPosition, tileData tileInfo);
		int GetTileIndexAtGridPos(Vector2D gridPosition);
};