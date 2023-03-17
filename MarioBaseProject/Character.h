#pragma once
#include "Commons.h"
#include "Controls.h"
#include "PhysicsObject.h"
#include "Animator.h"
#include <iostream>
#include <SDL.h>
#include <vector>

class Texture2D;
class PhysicsObject;

class Character
{
	public:
		Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, float scale, Vector2D& _cameraPosition);
		~Character();

		virtual void Render(Vector2D cameraPosition);
		virtual void Animate();
		virtual void Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap);
		void SetPosition(Vector2D new_position);
		Vector2D GetPosition();
		Vector2D GetSize();
		void LockToScreen();

		Vector2D& cameraPosition;

		FACING m_facing_direction;
		bool grounded;

		CharacterControls controls;

		int xInput;
		float moveSpeed;
		float airMoveSpeed;
		float groundMoveSpeed;

		bool jumpKeyDown;
		float jumpForce;
		float jumpCounterFactor;
		float jumpInputFudge;
		float timeSinceJumpInput = jumpInputFudge + 1;
		float coyoteTime;
		float timeSinceGrounded = coyoteTime + 1;
		bool isJumping;

		Vector2D movingDrag;
		Vector2D staticDrag;
		Vector2D fallingDrag;
		Vector2D jumpingDrag;
		Vector2D currentDrag;

		void SetMovementValues();
		void Move(float deltaTime);
		void Jump(float deltaTime);
		void EndJump();

	protected:
		SDL_Renderer* m_renderer;
		Vector2D m_position;
		Texture2D* m_texture;
		PhysicsObject m_physics;
		Animator* animator;
};