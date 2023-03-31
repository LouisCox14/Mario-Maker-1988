#pragma once
#include "Commons.h"
#include "Animator.h"
#include "PhysicsObject.h"
#include <SDL.h>
#include <Vector>

class Character;
class LevelScreen;

class Enemy
{
	public:
		Enemy(SDL_Renderer* renderer, Vector2D startPosition, LevelScreen* _levelScreen, std::vector<Character*> _characters);
		~Enemy();

		void Render(Vector2D cameraPosition);

		virtual void Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap);
		virtual void TakeDamage();

		std::string spriteDirectory;

		SDL_Renderer* m_renderer;
		Texture2D* m_texture;
		Animator* m_animator;
		PhysicsObject* m_physics;

		Vector2D m_position;
		FACING m_facing_direction;

		std::vector<Character*> characters;
		LevelScreen* levelScreen;
};