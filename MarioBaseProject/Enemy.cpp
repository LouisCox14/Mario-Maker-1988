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
	levelScreen->enemies.erase(remove(levelScreen->enemies.begin(), levelScreen->enemies.end(), this), levelScreen->enemies.end());
	m_renderer = nullptr;
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
