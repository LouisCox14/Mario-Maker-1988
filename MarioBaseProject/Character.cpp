#include "Character.h"
#include "Constants.h"
#include "Texture2D.h"
#include "PhysicsObject.h"
#include "GameScreenManager.h"
#include <vector>


Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, float scale)
{
	m_renderer = renderer;
	m_position = startPosition;
	m_facing_direction = FACING_RIGHT;

	grounded = false;

	xInput = 0;
	groundMoveSpeed = 750;
	airMoveSpeed = 350;

	jumpForce = 450;
	jumpCounterFactor = 0.55f;

	movingDrag = Vector2D(3.5, 0);
	staticDrag = Vector2D(7, 0);
	fallingDrag = Vector2D(2, 0.05);
	jumpingDrag = Vector2D(1.5, 0.5);

	// Load texture
	m_texture = new Texture2D(m_renderer);

	if (!m_texture->LoadFromFile(imagePath, scale))
	{
		std::cout << "Failed to load character texture! " << imagePath << " not found." << std::endl;
	}

	m_physics = PhysicsObject(m_position, Vector2D((float)m_texture->GetWidth(), (float)m_texture->GetHeight()), movingDrag, 1);
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

void Character::Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap)
{
	if (e.key.repeat == 0)
	{
		switch (e.type)
		{
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_a:
						xInput -= 1;
						break;
					case SDLK_d:
						xInput += 1;
						break;
					case SDLK_SPACE:
						StartJump();
						break;
				}
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
					case SDLK_a:
						xInput += 1;
						break;
					case SDLK_d:
						xInput -= 1;
						break;
					case SDLK_SPACE:
						EndJump();
						break;
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
	SetMovementValues();
	m_physics.UpdatePhysics(deltaTime, tileMap);
	SetPosition(m_physics.position);
}

void Character::SetPosition(Vector2D new_position)
{
	m_position = new_position;
}

Vector2D Character::GetPosition()
{
	return Vector2D(m_position);
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

void Character::StartJump()
{
	if (m_physics.sidesColliding[(int)BOTTOM] == BOTTOM)
	{
		m_physics.velocity.y -= jumpForce;
	}
}

void Character::EndJump()
{
	if (m_physics.velocity.y < 0)
	{
		m_physics.velocity.y *= 1 - jumpCounterFactor;
	}
}