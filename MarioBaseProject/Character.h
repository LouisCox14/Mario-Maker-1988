#pragma once
#include "Commons.h"
#include "PhysicsObject.h"
#include <iostream>
#include <SDL.h>

class Texture2D;
class PhysicsObject;

class Character
{
	public:
		Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
		~Character();

		virtual void Render();
		virtual void Update(float deltaTime, SDL_Event e);
		void SetPosition(Vector2D new_position);
		Vector2D GetPosition();

	private:
		FACING m_facing_direction;
		bool grounded;

		int xInput;
		float moveSpeed;

		bool jumpInput;
		float jumpForce;
		float currentJumpForce;
		float jumpDrag;

		void Move(float deltaTime);
		void Jump(float deltaTime);

	protected:
		SDL_Renderer* m_renderer;
		Vector2D m_position;
		Texture2D* m_texture;
		PhysicsObject m_physics;
};

