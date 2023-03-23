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
		Tile(SDL_Renderer* renderer, Vector2D startPosition, std::string imagePath, tileData tileInfo, float scale = 1.0f);
		~Tile();

		bool SetUpTile(std::string imagePath, float scale);
		void Render(Vector2D cameraPosition);
		void Update(float deltaTime);

		bool hasCollider;
		bool isAnimated;
		BoxCollider coll;
		Vector2D position;

	private:
		SDL_Renderer* m_renderer;
		Texture2D* m_tileTexture;
		Animator* m_animator;
};

