#pragma once
#include "Commons.h"
#include "PhysicsObject.h"
#include <iostream>
#include <SDL.h>
#include <vector>

class Texture2D;
class PhysicsObject;

class Character
{
	public:
		Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, float scale);
		~Character();

		virtual void Render(Vector2D cameraPosition);
		virtual void Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap);
		void SetPosition(Vector2D new_position);
		Vector2D GetPosition();

	private:
		FACING m_facing_direction;
		bool grounded;

		int xInput;
		float moveSpeed;
		float airMoveSpeed;
		float groundMoveSpeed;

		float jumpForce;
		float jumpCounterFactor;

		Vector2D movingDrag;
		Vector2D staticDrag;
		Vector2D fallingDrag;
		Vector2D jumpingDrag;
		Vector2D currentDrag;

		void SetMovementValues();
		void Move(float deltaTime);
		void StartJump();
		void EndJump();

	protected:
		SDL_Renderer* m_renderer;
		Vector2D m_position;
		Texture2D* m_texture;
		PhysicsObject m_physics;
};

