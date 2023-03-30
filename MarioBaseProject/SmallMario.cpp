#include "SmallMario.h"

SmallMario::SmallMario(SDL_Renderer* renderer, LevelScreen* _levelScreen, std::string imagePath, Vector2D startPosition, float scale, Vector2D& _cameraPosition) : Character(renderer, _levelScreen, imagePath, startPosition, scale, _cameraPosition)
{
	grounded = false;

	controls = CharacterControls(SDL_SCANCODE_D, SDL_SCANCODE_A, SDLK_w);

	xInput = 0;
	groundMoveSpeed = 275;
	airMoveSpeed = 175;

	jumpForce = 275;
	jumpCounterFactor = 0.55f;
	jumpInputFudge = 0.2f;
	coyoteTime = 0.1f;

	movingDrag = Vector2D(2, 0);
	staticDrag = Vector2D(7, 0);
	fallingDrag = Vector2D(2, 0.01);
	jumpingDrag = Vector2D(1.5, 1);

	m_physics = PhysicsObject(m_position, scale, Vector2D((float)m_texture->GetWidth(), (float)m_texture->GetHeight()), movingDrag, 1);

	animator = new Animator(*m_texture, std::map<std::string, Animation>{
		{"Idle", Animation(std::vector<std::string>{"Idle.png"}, 0.25f, true)},
		{ "Run", Animation(std::vector<std::string>{"Idle.png", "Run.png"}, 0.1f, true) },
		{ "Turn", Animation(std::vector<std::string>{"Turn.png"}, 0.25f, true) },
		{ "Jump", Animation(std::vector<std::string>{"Jump.png"}, 0.25f, true) }
	}, std::string("Idle"), std::string("Assets/Sprites/Small Mario/"));
}

void SmallMario::Animate()
{
	if (m_physics.sidesColliding[(int)BOTTOM] == BOTTOM)
	{
		if (xInput != 0 && m_physics.velocity.x != 0)
		{
			if ((xInput == 1 && m_physics.velocity.x > 0) || (xInput == -1 && m_physics.velocity.x < 0))
			{
				animator->SetAnimation("Run");
			}
			else
			{
				animator->SetAnimation("Turn");
			}
		}
		else
		{
			animator->SetAnimation("Idle");
		}
	}
	else
	{
		animator->SetAnimation("Jump");
	}
}

void SmallMario::TakeDamage()
{
	levelScreen->gameOver = true;
}
