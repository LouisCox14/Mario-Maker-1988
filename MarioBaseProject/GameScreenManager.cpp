#include "GameScreenManager.h"

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	m_renderer = renderer;
	m_current_screen = nullptr;

	ChangeScreen(startScreen);
}

GameScreenManager::~GameScreenManager()
{
	m_renderer = nullptr;

	delete m_current_screen;
	m_current_screen = nullptr;
}

void GameScreenManager::Render()
{
	m_current_screen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	m_current_screen->Update(deltaTime, e);
}

void GameScreenManager::ChangeScreen(SCREENS new_screen)
{
}
