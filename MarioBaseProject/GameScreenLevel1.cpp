#include "GameScreenLevel1.h"
#include "Texture2D.h"
#include <iostream>

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpLevel(LoadFromTXT("Levels/Test.txt"));
}

GameScreenLevel1::~GameScreenLevel1()
{
	delete m_background_texture;
	m_background_texture = nullptr;

	delete my_character;
	my_character = nullptr;
}

void GameScreenLevel1::Render()
{
	// Draw the background
	m_background_texture->Render(Vector2D(), SDL_FLIP_NONE);
	my_character->Render();

	for (Tile* tile : tileMap)
	{
		tile->Render();
	}
}

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
{
	my_character->Update(deltaTime, e);
}

bool GameScreenLevel1::SetUpLevel(float levelScale)
{
	// Load texture
	m_background_texture = new Texture2D(m_renderer);

	// Set up player character
	my_character = new Character(m_renderer, "Images/Mario.png", Vector2D(64, 330), levelScale);

	if (!m_background_texture->LoadFromFile("Images/test.bmp"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}

	return true;
}
