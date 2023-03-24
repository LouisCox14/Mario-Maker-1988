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
		Tile(SDL_Renderer* renderer, Vector2D startPosition, tileData _tileInfo, std::vector<Tile*> tileMap, float scale = 1.0f);
		~Tile();

		tileData tileInfo;
		bool SetUpTile(std::vector<Tile*> tileMap, float scale);

		void Render(Vector2D cameraPosition);
		void Update(float deltaTime);

		bool hasCollider;
		bool isAnimated;
		BoxCollider coll;
		Vector2D position;

		bool LoadCompositeSprite(std::vector<Tile*> tileMap);
		std::string CalculateCompositeSprite(std::vector<Tile*> tileMap);
		void CalculateCompositeCollider(std::vector<Tile*> tileMap);
		int GetTileIndexAtPixelPos(Vector2D gridPosition, std::vector<Tile*> tileMap);

	private:
		SDL_Renderer* m_renderer;
		Texture2D* m_tileTexture;
		Animator* m_animator;
};

