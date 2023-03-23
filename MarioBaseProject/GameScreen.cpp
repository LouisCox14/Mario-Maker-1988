#include "GameScreen.h"
#include "Constants.h"
#include "Commons.h"
#include "Tile.h"
#include "TileData.h"
#include <iostream>
#include <fstream>
#include <string>

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	m_renderer = renderer;
	cameraPosition = Vector2D(0, 0);
}

GameScreen::~GameScreen()
{
	m_renderer = nullptr;
}

void GameScreen::Render() {}

void GameScreen::Update(float deltaTime, SDL_Event e) {}