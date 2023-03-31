#pragma once
#include "Enemy.h"
class Goomba : Enemy
{
	public:
		Goomba(SDL_Renderer* renderer, Vector2D startPosition, LevelScreen* _levelScreen, std::vector<Character*> _characters);
		~Goomba();

		void Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap) override;
		void TakeDamage() override;

		bool isDead;
		float deathAnimTime;
	private:
		float moveSpeed;
};

