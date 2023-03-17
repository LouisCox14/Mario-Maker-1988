#include "Tile.h"
#include "Texture2D.h"
#include "GameScreen.h"
#include "Commons.h"
#include <iostream>
#include <array>

Tile::Tile(SDL_Renderer* renderer, Vector2D startPosition, std::string imagePath, tileData tileInfo, float scale)
{
	m_renderer = renderer;
	position = startPosition;

	SetUpTile(imagePath, scale);

	Vector2D textureDimensions = Vector2D(m_tileTexture->GetHeight(), m_tileTexture->GetWidth());

	if (tileInfo.colliderType == COMPOSITE)
	{
		std::array<COLLISION_SIDES, 4> compositeColliderSides = { NONE, NONE, NONE, NONE };

		if (imagePath.find(" Top") != -1 && tileInfo.collisionSides[(int)TOP] == TOP)
		{
			compositeColliderSides[(int)TOP] = TOP;
		}
		if (imagePath.find(" Right") != -1 && tileInfo.collisionSides[(int)RIGHT] == RIGHT)
		{
			compositeColliderSides[(int)RIGHT] = RIGHT;
		}
		if (imagePath.find(" Bottom") != -1 && tileInfo.collisionSides[(int)BOTTOM] == BOTTOM)
		{
			compositeColliderSides[(int)BOTTOM] = BOTTOM;
		}
		if (imagePath.find(" Left") != -1 && tileInfo.collisionSides[(int)LEFT] == LEFT)
		{
			compositeColliderSides[(int)LEFT] = LEFT;
		}

		coll = BoxCollider(position + (textureDimensions / 2), textureDimensions, compositeColliderSides);
	}
	else
	{
		coll = BoxCollider(position + (textureDimensions / 2), textureDimensions, tileInfo.collisionSides);
	}

	if (coll.collisionSides == std::array<COLLISION_SIDES, 4>{NONE, NONE, NONE, NONE})
	{
		hasCollider = false;
	}
	else
	{
		hasCollider = true;
	}
}

Tile::~Tile()
{
	m_renderer = nullptr;

	delete m_tileTexture;
	m_tileTexture = nullptr;

	coll = BoxCollider(Vector2D(0, 0), Vector2D(0, 0));
}

void Tile::Render(Vector2D cameraPosition)
{
	m_tileTexture->Render(position - cameraPosition, SDL_FLIP_NONE);
}

bool Tile::SetUpTile(std::string imagePath, float scale)
{
	// Load texture
	m_tileTexture = new Texture2D(m_renderer, scale);


	if (!m_tileTexture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load tile texture! " << imagePath << " not found." << std::endl;
		return false;
	}

	return true;
}