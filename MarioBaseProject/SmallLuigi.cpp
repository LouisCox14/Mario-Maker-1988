#include "SmallLuigi.h"

SmallLuigi::SmallLuigi(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, float scale, Vector2D& _cameraPosition) : Character(renderer, imagePath, startPosition, scale, _cameraPosition)
{
	grounded = false;

	controls = CharacterControls(SDLK_RIGHT, SDLK_LEFT, SDLK_UP);

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

	m_physics = PhysicsObject(m_position, Vector2D((float)m_texture->GetWidth(), (float)m_texture->GetHeight()), movingDrag, 1);

	animator = new Animator(*m_texture, std::map<std::string, Animation>{
		{"Idle", Animation(std::vector<std::string>{"Idle.png"}, 0.25f, true)},
		{ "Run", Animation(std::vector<std::string>{"Idle.png", "Run.png"}, 0.1f, true) },
		{ "Turn", Animation(std::vector<std::string>{"Turn.png"}, 0.25f, true) },
		{ "Jump", Animation(std::vector<std::string>{"Jump.png"}, 0.25f, true) }
	}, std::string("Running"), std::string("Sprites/Small Luigi/"));
}