#pragma once
#include "Commons.h"
#include "Character.h"
#include <SDL.h>

class LevelScreen;

enum EnemyType
{
	GOOMBA
};

class EnemySpawner
{
	public:
		EnemySpawner(SDL_Renderer* _renderer, LevelScreen* _levelScreen, Vector2D* _cameraPosition, EnemyType _enemyType, Vector2D _position);
		~EnemySpawner();

		void Update();

	private:
		bool isActive;
		LevelScreen* levelScreen;
		Vector2D* cameraPosition;
		SDL_Renderer* renderer;
		EnemyType enemyType;
		Vector2D position;
};