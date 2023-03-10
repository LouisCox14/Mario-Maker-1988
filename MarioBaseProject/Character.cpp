#include "Character.h"
#include "Constants.h"
#include "Texture2D.h"
#include "PhysicsObject.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
{
	m_renderer = renderer;
	m_position = startPosition;
	m_facing_direction = FACING_RIGHT;

	grounded = false;

	xInput = 0;
	moveSpeed = 750;

	jumpInput = false;
	jumpForce = 300;
	jumpDrag = 300;

	// Load texture
	m_texture = new Texture2D(m_renderer);

	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load character texture! " << imagePath << " not found." << std::endl;
	}

	m_physics = PhysicsObject(m_position, Vector2D(m_texture->GetWidth(), m_texture->GetHeight()), Vector2D(5, 0.5), 1);
}

Character::~Character()
{
	m_renderer = nullptr;
}

void Character::Render()
{
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(m_position, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(m_position, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
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
						m_physics.velocity.y -= jumpForce;
						jumpInput = true;
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
						jumpInput = false;
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
	m_physics.UpdatePhysics(deltaTime);
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

void Character::Move(float deltaTime)
{
	m_physics.velocity.x += xInput * deltaTime * moveSpeed;
}

void Character::Jump(float deltaTime)
{
	if (jumpInput && grounded)
	{
		// Jump
	}
}