#include "Character.h"
#include "Constants.h"
#include "Texture2D.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition)
{
	m_renderer = renderer;
	m_position = startPosition;
	m_facing_direction = FACING_RIGHT;

	grounded = false;

	xInput = 0;
	moveSpeed = 80;

	jumpInput = false;

	// Load texture
	m_texture = new Texture2D(m_renderer);

	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load character texture! " << imagePath << " not found." << std::endl;
	}
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
						m_facing_direction = FACING_LEFT;
						break;
					case SDLK_d:
						xInput += 1;
						m_facing_direction = FACING_RIGHT;
						break;
					case SDLK_SPACE:
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

	Jump(deltaTime);
	Move(deltaTime);
	AddGravity(deltaTime);
}

void Character::SetPosition(Vector2D new_position)
{
	m_position = new_position;
}

Vector2D Character::GetPosition()
{
	return Vector2D(m_position);
}

void Character::AddGravity(float deltaTime)
{
	if (m_position.y + m_texture->GetHeight() <= SCREEN_HEIGHT)
	{
		m_position.y += deltaTime * GRAVITY;
	}
}

void Character::Move(float deltaTime)
{
	m_position.x += xInput * deltaTime * moveSpeed;
}

void Character::Jump(float deltaTime)
{
	if (jumpInput && grounded)
	{

	}
}