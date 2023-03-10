#include "Tile.h"
#include "Texture2D.h"
#include <iostream>

Tile::Tile(SDL_Renderer* renderer, Vector2D startPosition, std::string imagePath, float scale)
{
	m_renderer = renderer;
	position = startPosition;

	SetUpTile(imagePath, scale);

	coll = BoxCollider(position, Vector2D(m_tileTexture->GetHeight(), m_tileTexture->GetWidth()));
}

Tile::~Tile()
{
	m_renderer = nullptr;

	delete m_tileTexture;
	m_tileTexture = nullptr;

	coll = BoxCollider(Vector2D(0, 0), Vector2D(0, 0));
}

void Tile::Render()
{
	m_tileTexture->Render(position, SDL_FLIP_NONE);
}

bool Tile::SetUpTile(std::string imagePath, float scale)
{
	// Load texture
	m_tileTexture = new Texture2D(m_renderer);


	if (!m_tileTexture->LoadFromFile(imagePath, scale))
	{
		std::cout << "Failed to load tile texture! " << imagePath << " not found." << std::endl;
		return false;
	}

	return true;
}