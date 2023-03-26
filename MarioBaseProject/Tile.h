#pragma once
#include "Commons.h"
#include "TileData.h"
#include "Animator.h"
#include <SDL.h>
#include <string>

class Texture2D;

class Tile
{
	public:
		Tile(SDL_Renderer* renderer = NULL, Vector2D startPosition = Vector2D(0, 0), tileData _tileInfo = tileData(""), std::vector<Tile*> tileMap = {}, float scale = 1.0f);
		Tile(SDL_Renderer* renderer, Vector2D startPosition, tileData _tileInfo, std::string _filePath, std::array<COLLISION_SIDES, 4> collSides, float scale = 1.0f);
		~Tile();

		tileData tileInfo;
		std::string tilePath;

		bool SetUpTile(std::vector<Tile*> tileMap, float scale);

		void Render(Vector2D cameraPosition);
		void Update(float deltaTime);

		bool hasCollider;
		bool isAnimated;
		BoxCollider coll;
		Vector2D position;

		void SetupAnimator();

		bool LoadCompositeSprite(std::vector<Tile*> tileMap);
		std::string CalculateCompositeSprite(std::vector<Tile*> tileMap);
		void CalculateCompositeCollider(std::vector<Tile*> tileMap);
		int GetTileIndexAtPixelPos(Vector2D gridPosition, std::vector<Tile*> tileMap);

	private:
		SDL_Renderer* m_renderer;
		Texture2D* m_tileTexture;
		Animator* m_animator;
};

