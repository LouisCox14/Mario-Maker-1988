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
	groundMoveSpeed = 550;
	airMoveSpeed = 350;

	jumpForce = 550;
	jumpCounterFactor = 0.55f;
	jumpInputFudge = 0.2f;
	coyoteTime = 0.1f;

	movingDrag = Vector2D(2, 0);
	staticDrag = Vector2D(7, 0);
	fallingDrag = Vector2D(2, 0.01);
	jumpingDrag = Vector2D(1.5, 1);

	// Load texture
	m_texture = new Texture2D(m_renderer, scale);

	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load character texture! " << imagePath << " not found." << std::endl;
	}

	m_physics = PhysicsObject(m_position, Vector2D((float)m_texture->GetWidth(), (float)m_texture->GetHeight()), movingDrag, 1);

	animator = new Animator(*m_texture, std::map<std::string, Animation>{
		{"Idle", Animation(std::vector<std::string>{"Idle.png"}, 0.25f, true)},
		{ "Run", Animation(std::vector<std::string>{"Idle.png", "Run.png"}, 0.1f, true)},
		{ "Turn", Animation(std::vector<std::string>{"Turn.png"}, 0.25f, true)},
		{ "Jump", Animation(std::vector<std::string>{"Jump.png"}, 0.25f, true)}
	}, std::string("Running"), std::string("Sprites/Small Mario/"));
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
						jumpKeyDown = true;
						timeSinceJumpInput = 0;
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
						jumpKeyDown = false;
						EndJump();
						break;
				}
				break;
		}
	}

	if (xInput != 0)
	{
		animator->SetAnimation("Run");

		if (xInput > 0)
		{
			m_facing_direction = FACING_RIGHT;

			if (m_physics.velocity.x < 0)
			{
				animator->SetAnimation("Turn");
			}
		}
		else if (xInput < 0)
		{
			m_facing_direction = FACING_LEFT;

			if (m_physics.velocity.x > 0)
			{
				animator->SetAnimation("Turn");
			}
		}
	}
	else
	{
		animator->SetAnimation("Idle");
	}

	Move(deltaTime);
	Jump(deltaTime);
	SetMovementValues();
	m_physics.UpdatePhysics(deltaTime, tileMap);
	SetPosition(m_physics.position);

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
		animator->SetAnimation("Jump");

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