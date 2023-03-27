#include "GameScreenManager.h"
#include "LevelScreen.h"
#include "LevelCreator.h"
#include "Titlescreen.h"

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
	if (!newScreen)
		m_current_screen->Update(deltaTime, e);
	else
		newScreen = false;
}

void GameScreenManager::ChangeScreen(SCREENS new_screen, std::string fileDirectory)
{
	newScreen = true;

	// Clear up the old screen
	if (m_current_screen != nullptr)
	{
		delete m_current_screen;
	}

	GameScreen* tempScreen;

	switch (new_screen)
	{
		case SCREEN_MENU:
			tempScreen = (GameScreen*)new Titlescreen(m_renderer, this);
			m_current_screen = tempScreen;
			tempScreen = nullptr;
			break;
		case SCREEN_LEVEL_CREATOR:
			tempScreen = (GameScreen*)new LevelCreator(m_renderer, fileDirectory);
			m_current_screen = tempScreen;
			tempScreen = nullptr;
			break;
		case SCREEN_LEVEL:
			tempScreen = (GameScreen*)new LevelScreen(m_renderer, fileDirectory, multiplayer);
			m_current_screen = tempScreen;
			tempScreen = nullptr;
			break;
		default:	;
	}
}
