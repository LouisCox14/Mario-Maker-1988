#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include "SmallMario.h"
#include "SmallLuigi.h"
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel(LoadFromTXT("Levels/Test.txt"));
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete mario;
	mario = nullptr;
	delete luigi;
	luigi = nullptr;
}

void GameScreenLevel1::Render()
{

	// Draw the tilemap
	for (Tile* tile : GetOnScreenTiles())
	{
		tile->Render(GameScreen::cameraPosition);
	}

	//Draw the character
	mario->Render(GameScreen::cameraPosition);
	luigi->Render(GameScreen::cameraPosition);
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	mario->Update(deltaTime, e, GetOnScreenTiles());
	luigi->Update(deltaTime, e, GetOnScreenTiles());

	float cameraX = (mario->GetPosition().x + luigi->GetPosition().x) / 2 - SCREEN_WIDTH / 2;
	cameraX = std::max(cameraX, 0.0f);
	cameraX = std::min(cameraX, (float)GameScreen::levelWidth - SCREEN_WIDTH);
	GameScreen::cameraPosition.x = cameraX;
}

bool GameScreenLevel1::SetUpLevel(float levelScale)
{
	// Set up player character
	SmallMario* tempMario = new SmallMario(m_renderer, "Sprites/Small Mario/Idle.png", Vector2D(64, 64), levelScale, cameraPosition);
	mario = (Character*)tempMario;

	SmallLuigi* tempLuigi = new SmallLuigi(m_renderer, "Sprites/Small Mario/Idle.png", Vector2D(64, 64), levelScale, cameraPosition);
	luigi = (Character*)tempLuigi;

	return true;
}
