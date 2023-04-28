#include "GameScreen.h"
#include "Constants.h"
#include "Commons.h"
#include "Tile.h"
#include "TileData.h"
#include "ButtonUI.h"
#include <iostream>
#include <fstream>
#include <string>

GameScreen::GameScreen(SDL_Renderer* renderer, GameScreenManager* _screenManager)
{
	m_renderer = renderer;
	screenManager = _screenManager;

	cameraPosition = Vector2D(0, 0);
}

GameScreen::~GameScreen()
{
	m_renderer = nullptr;
	audioPlayer = nullptr;
	screenManager = nullptr;
}

void GameScreen::Render() {}

void GameScreen::Update(float deltaTime, SDL_Event e) {}

void GameScreen::ButtonClicked(ButtonUI* button, bool leftClick) {}