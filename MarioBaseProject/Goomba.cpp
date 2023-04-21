#include "Goomba.h"
#include "Character.h"

Goomba::Goomba(SDL_Renderer* renderer, Vector2D startPosition, LevelScreen* _levelScreen, FACING facing) : Enemy(renderer, startPosition, _levelScreen, facing)
{
	spriteDirectory = "Assets/Sprites/Goomba/";
	if (!m_texture->LoadFromFile(spriteDirectory + "Goomba.png"))
	{
		std::cout << "Failed to load emeny texture! " << spriteDirectory << "Goomba.png not found." << std::endl;
	}

	m_animator = new Animator(*m_texture, std::map<std::string, Animation>{
		{ "Walk", Animation(std::vector<std::string>{"Goomba Walk 1.png", "Goomba Walk 2.png"}, 0.25f, true)},
		{ "Death", Animation(std::vector<std::string>{"Goomba Death.png"}, 0.5f, false) },
	}, std::string("Walk"), spriteDirectory);

	m_physics = new PhysicsObject(m_position, CAMERA_SCALE, Vector2D((float)m_texture->GetWidth(), (float)m_texture->GetHeight() - CAMERA_SCALE), Vector2D(2, 0), 1);
	moveSpeed = 30;

	deathAnimTime = 0.35f;
}

Goomba::~Goomba()
{

}

void Goomba::Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap)
{
	if (!isDead)
	{
		if (m_facing_direction == FACING_RIGHT)
		{
			m_physics->velocity.x = moveSpeed;
		}
		else
		{
			m_physics->velocity.x = -moveSpeed;
		}

		m_animator->Update(deltaTime);
		m_physics->UpdatePhysics(deltaTime, tileMap);

		if (m_physics->sidesColliding[(int)RIGHT] == RIGHT)
		{
			m_facing_direction = FACING_LEFT;
		}
		else if (m_physics->sidesColliding[(int)LEFT] == LEFT)
		{
			m_facing_direction = FACING_RIGHT;
		}

		m_position = m_physics->position;

		for (Character* player : levelScreen->characters)
		{
			if (m_physics->coll.OverlapCheck(player->m_physics.coll))
			{
				if (m_physics->coll.GetCollisionSide(player->m_physics.coll) != TOP)
				{
					player->TakeDamage();
				}
				else
				{
					player->EnemyJump();
					TakeDamage();
				}
			}
		}
	}
	else
	{
		deathAnimTime -= deltaTime;

		if (deathAnimTime <= 0.0f)
		{
			delete this;
		}
	}


}

void Goomba::TakeDamage()
{
	isDead = true;
	m_animator->SetAnimation("Death");
}
