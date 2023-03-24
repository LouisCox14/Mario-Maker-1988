#include "Tile.h"
#include "Texture2D.h"
#include "GameScreen.h"
#include "Commons.h"
#include <iostream>
#include <array>

Tile::Tile(SDL_Renderer* renderer, Vector2D startPosition, tileData _tileInfo, std::vector<Tile*> tileMap, float scale)
{
	m_renderer = renderer;
	position = startPosition;
	tileInfo = _tileInfo;

	SetUpTile(tileMap, scale);

	Vector2D textureDimensions = Vector2D(m_tileTexture->GetHeight(), m_tileTexture->GetWidth());
	coll = BoxCollider(position + (textureDimensions / 2), textureDimensions);

	if (tileInfo.colliderType == COMPOSITE)
	{
		CalculateCompositeCollider(tileMap);
	}

	if (tileInfo.spriteType == COMPOSITE)
	{
		LoadCompositeSprite(tileMap);
	}

	if (tileInfo.isAnimated)
	{
		Animation tempAnim = Animation(std::vector<std::string>{}, tileInfo.animationDelay, true);

		for (int i = 1; i < tileInfo.animationFrames + 1; i++)
		{
			tempAnim.framePaths.push_back(tileInfo.fileName + " " + std::to_string(i) + ".png");
		}

		m_animator = new Animator(*m_tileTexture, std::map<std::string, Animation>{{"Idle", tempAnim}}, std::string("Idle"), std::string("Tiles/"));
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

void Tile::Update(float deltaTime)
{
	if (isAnimated)
	{
		m_animator->Update(deltaTime);
	};
}

std::string Tile::CalculateCompositeSprite(std::vector<Tile*> tileMap)
{
	std::string imagePath = tileInfo.fileName;

	for (COLLISION_SIDES direction : tileInfo.autoTileSides)
	{
		int tempIndex = -1;
		switch (direction)
		{
			case TOP:
				tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(0, -1) * m_tileTexture->GetHeight()), tileMap);
				if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
				{
					imagePath += " Top";
				}
				break;
			case RIGHT:
				tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(1, 0) * m_tileTexture->GetWidth()), tileMap);
				if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
				{
					imagePath += " Right";
				}
				break;
			case BOTTOM:
				tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(0, 1) * m_tileTexture->GetHeight()), tileMap);
				if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
				{
					imagePath += " Bottom";
				}
				break;
			case LEFT:
				tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(-1, 0) * m_tileTexture->GetWidth()), tileMap);
				if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
				{
					imagePath += " Left";
				}
				break;
		}
	}

	return imagePath;
}

void Tile::CalculateCompositeCollider(std::vector<Tile*> tileMap)
{
	std::array<COLLISION_SIDES, 4> tempCollisionSides = { TOP, RIGHT, LEFT, BOTTOM };

	for (COLLISION_SIDES direction : tileInfo.collisionSides)
	{
		int tempIndex = -1;
		switch (direction)
		{
		case TOP:
			tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(0, -1) * m_tileTexture->GetHeight()), tileMap);
			if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
			{
				tempCollisionSides[TOP] = NONE;
			}
			break;
		case RIGHT:
			tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(1, 0) * m_tileTexture->GetWidth()), tileMap);
			if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
			{
				tempCollisionSides[RIGHT] = NONE;
			}
			break;
		case BOTTOM:
			tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(0, 1) * m_tileTexture->GetHeight()), tileMap);
			if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
			{
				tempCollisionSides[BOTTOM] = NONE;
			}
			break;
		case LEFT:
			tempIndex = GetTileIndexAtPixelPos(position + (Vector2D(-1, 0) * m_tileTexture->GetWidth()), tileMap);
			if (tempIndex == -1 || tileMap[tempIndex]->tileInfo.fileName != tileInfo.fileName)
			{
				tempCollisionSides[LEFT] = NONE;
			}
			break;
		}
	}

	coll.collisionSides = tempCollisionSides;

	if (tempCollisionSides == std::array<COLLISION_SIDES, 4>{ NONE, NONE, NONE, NONE })
	{
		hasCollider = false;
	}
}

bool Tile::SetUpTile(std::vector<Tile*> tileMap, float scale)
{
	// Load texture
	m_tileTexture = new Texture2D(m_renderer, scale);

	std::string imagePath = "Tiles/" + tileInfo.fileName;

	if (tileInfo.isAnimated)
	{
		imagePath += " 1";
	}

	if (tileInfo.colliderType == COMPOSITE)
	{
		imagePath += tileInfo.defaultSuffix;
	}

	imagePath += ".png";

	if (!m_tileTexture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load tile texture! " << imagePath << " not found." << std::endl;
		return false;
	}

	return true;
}

bool Tile::LoadCompositeSprite(std::vector<Tile*> tileMap)
{
	std::string imagePath = "Tiles/" + CalculateCompositeSprite(tileMap) + ".png";

	if (!m_tileTexture->LoadFromFile(imagePath))
	{
		imagePath = "Tiles/" + tileInfo.fileName + tileInfo.defaultSuffix + ".png";

		if (!m_tileTexture->LoadFromFile(imagePath))
		{
			std::cout << "Failed to load tile texture! " << imagePath << " not found." << std::endl;
			return false;
		}
	}

	return true;
}

int Tile::GetTileIndexAtPixelPos(Vector2D pixelPos, std::vector<Tile*> tileMap)
{
	for (int i = 0; i < tileMap.size(); i++)
	{
		if (tileMap[i]->position == pixelPos)
		{
			return i;
		}
	}

	return -1;
}