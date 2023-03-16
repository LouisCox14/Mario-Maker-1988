#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel(LoadFromTXT("Levels/Test.txt"));
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete my_character;
	my_character = nullptr;
}

void GameScreenLevel1::Render()
{

	// Draw the tilemap
	for (Tile* tile : GetOnScreenTiles())
	{
		tile->Render(GameScreen::cameraPosition);
	}

	//Draw the character
	my_character->Render(GameScreen::cameraPosition);
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	my_character->Update(deltaTime, e, GetOnScreenTiles());

	float cameraX = my_character->GetPosition().x - SCREEN_WIDTH / 2;
	cameraX = std::max(cameraX, 0.0f);
	cameraX = std::min(cameraX, (float)GameScreen::levelWidth - SCREEN_WIDTH);
	GameScreen::cameraPosition.x = cameraX;
}

bool GameScreenLevel1::SetUpLevel(float levelScale)
{
	// Set up player character
	my_character = new Character(m_renderer, "Sprites/Small Mario/Idle.png", Vector2D(64, 64), levelScale);

	return true;
}
