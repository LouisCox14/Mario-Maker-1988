#include "Enemy.h"
#include "LevelScreen.h"

Enemy::Enemy(SDL_Renderer* renderer, Vector2D startPosition, LevelScreen* _levelScreen, FACING facing)
{
	levelScreen = _levelScreen;
	m_renderer = renderer;
	m_position = startPosition;

	m_facing_direction = facing;
	m_texture = new Texture2D(m_renderer, CAMERA_SCALE);
}

Enemy::~Enemy() 
{
	levelScreen->enemies[std::distance(levelScreen->enemies.begin(), std::find(levelScreen->enemies.begin(), levelScreen->enemies.end(), this))] = nullptr;
	m_renderer = nullptr;
	delete m_animator;
	m_animator = nullptr;
	delete m_physics;
	m_physics = nullptr;
	delete m_texture;
	m_texture = nullptr;
	levelScreen = nullptr;
}

void Enemy::Render(Vector2D cameraPosition)
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

void Enemy::Update(float deltaTime, SDL_Event e, const std::vector<Tile*>& tileMap) {}

void Enemy::TakeDamage() {}
