#pragma once
#include "Commons.h"
#include <iostream>
#include <SDL.h>

class Texture2D;

class Character
{
	public:
		Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition);
		~Character();

		virtual void Render();
		virtual void Update(float deltaTime, SDL_Event e);
		void SetPosition(Vector2D new_position);
		Vector2D GetPosition();

		virtual void AddGravity(float deltaTime);

	private:
		FACING m_facing_direction;
		bool grounded;

		int xInput;
		float moveSpeed;

		bool jumpInput;

		void Move(float deltaTime);
		void Jump(float deltaTime);

	protected:
		SDL_Renderer* m_renderer;
		Vector2D m_position;
		Texture2D* m_texture;
};

