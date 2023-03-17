#include "Character.h"
#include "Constants.h"
#include "Texture2D.h"
#include "PhysicsObject.h"
#include "GameScreenManager.h"
#include "GameScreen.h"
#include <vector>


Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, float scale, Vector2D& _cameraPosition) : cameraPosition(_cameraPosition)
{
	m_renderer = renderer;
	m_position = startPosition;
	m_facing_direction = FACING_RIGHT;

	// Load texture
	m_texture = new Texture2D(m_renderer, scale);

	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load character texture! " << imagePath << " not found." << std::endl;
	}
}

Character::~Character()
{
	m_renderer = nullptr;
}

void Character::Render(Vector2D cameraPosition)
{
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(m_position - cameraPosition, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(m_position - cameraPosition, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Animate() {}

void Character::Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap)
{
	if (e.key.repeat == 0)
	{
		switch (e.type)
		{
			case SDL_KEYDOWN:

				if (e.key.keysym.sym == controls.leftMove)
				{
					xInput -= 1;
				}
				else if (e.key.keysym.sym == controls.rightMove)
				{
					xInput += 1;
				}
				else if (e.key.keysym.sym == controls.jump)
				{
					jumpKeyDown = true;
					timeSinceJumpInput = 0;
				}
				break;
			case SDL_KEYUP:
				if (e.key.keysym.sym == controls.leftMove)
				{
					xInput += 1;
				}
				else if (e.key.keysym.sym == controls.rightMove)
				{
					xInput -= 1;
				}
				else if (e.key.keysym.sym == controls.jump)
				{
					jumpKeyDown = false;
					EndJump();
				}
				break;
		}
	}

	if (xInput > 0)
	{
		m_facing_direction = FACING_RIGHT;
	}
	else if (xInput < 0)
	{
		m_facing_direction = FACING_LEFT;
	}

	Move(deltaTime);
	Jump(deltaTime);
	SetMovementValues();
	LockToScreen();
	m_physics.UpdatePhysics(deltaTime, tileMap);
	SetPosition(m_physics.position);

	Animate();
	animator->Update(deltaTime);
}

void Character::SetPosition(Vector2D new_position)
{
	m_position = new_position;
}


Vector2D Character::GetPosition()
{
	return Vector2D(m_position);
}

Vector2D Character::GetSize()
{
	return Vector2D(m_texture->GetWidth(), m_texture->GetHeight());
}

void Character::LockToScreen()
{
	if (m_position.x + m_texture->GetWidth() > cameraPosition.x + SCREEN_WIDTH)
	{
		m_physics.velocity.x = std::min(m_physics.velocity.x, -100.0f);
	}
	else if (m_position.x < cameraPosition.x)
	{
		m_physics.velocity.x = std::max(m_physics.velocity.x, 100.0f);
	}
}

void Character::SetMovementValues()
{
	if (m_physics.sidesColliding[(int)BOTTOM] == BOTTOM)
	{
		moveSpeed = groundMoveSpeed;

		if (xInput != 0)
		{
			m_physics.drag = movingDrag;
		}
		else
		{
			m_physics.drag = staticDrag;
		}
	}
	else
	{
		moveSpeed = airMoveSpeed;
		if (m_physics.velocity.y < 0.0f)
		{
			m_physics.drag = jumpingDrag;
		}
		else
		{
			m_physics.drag = fallingDrag;
		}
	}
}

void Character::Move(float deltaTime)
{
	m_physics.velocity.x += xInput * deltaTime * moveSpeed;
}

void Character::Jump(float deltaTime)
{
	if (m_physics.sidesColliding[(int)BOTTOM] == BOTTOM)
	{
		timeSinceGrounded = 0;
	}
	else
	{
		timeSinceGrounded += deltaTime;
	}

	if (timeSinceJumpInput < jumpInputFudge && timeSinceGrounded < coyoteTime && !isJumping)
	{
		if (jumpKeyDown)
		{
			m_physics.velocity.y -= jumpForce;
		}
		else
		{
			m_physics.velocity.y -= jumpForce * (1 - jumpCounterFactor);
		}

		isJumping = true;
	}

	timeSinceJumpInput += deltaTime;

	if (m_physics.velocity.y > 0 && isJumping)
	{
		isJumping = false;
	}
}

void Character::EndJump()
{
	if (m_physics.velocity.y < 0 && isJumping)
	{
		m_physics.velocity.y *= 1 - jumpCounterFactor;
	}
}