#include "EnemySpawner.h"
#include "Goomba.h"
#include "LevelScreen.h"

EnemySpawner::EnemySpawner(SDL_Renderer* _renderer, LevelScreen* _levelScreen, Vector2D* _cameraPosition, EnemyType _enemyType, Vector2D _position)
{
	renderer = _renderer;
	levelScreen = _levelScreen;
	cameraPosition = _cameraPosition;
	enemyType = _enemyType;
	position = _position;

	isActive = true;
}

EnemySpawner::~EnemySpawner()
{
	renderer = nullptr;
	levelScreen = nullptr;
	cameraPosition = nullptr;
}

void EnemySpawner::Update()
{
	if (position.x > cameraPosition->x - TILE_RESOLUTION * CAMERA_SCALE && position.x < cameraPosition->x + SCREEN_WIDTH)
	{
		if (isActive)
		{
			isActive = false;

			FACING startDirection;
			if (position.x < cameraPosition->x + SCREEN_WIDTH / 2)
			{
				startDirection = FACING_RIGHT;
			}
			else
			{
				startDirection = FACING_LEFT;
			}

			switch (enemyType)
			{
				case GOOMBA:
					levelScreen->enemies.push_back((Enemy*)new Goomba(renderer, position, levelScreen, startDirection));
			}
		}
	}
	else
	{
		isActive = true;
	}
}
