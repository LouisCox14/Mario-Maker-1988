#include "LevelScreen.h"
#include "Texture2D.h"
#include "SmallMario.h"
#include "SmallLuigi.h"
#include <iostream>
#include <fstream>
#include <string>
#include <json\json.h>
#include <json\value.h>

LevelScreen::LevelScreen(SDL_Renderer* renderer, std::string levelPath, bool multiplayer) : GameScreen(renderer)
{
	LoadFromJSON(levelPath);
	SetUpLevel(multiplayer);
}

LevelScreen::~LevelScreen()
{
	for (Character* character : characters)
	{
		delete character;
		character = nullptr;
	}
}

void LevelScreen::Render()
{

	// Draw the tilemap
	for (Tile* tile : GetOnScreenTiles())
	{
		tile->Render(GameScreen::cameraPosition);
	}

	//Draw the character
	for (Character* character : characters)
	{
		character->Render(GameScreen::cameraPosition);
	}
}

void LevelScreen::Update(float deltaTime, SDL_Event e)
{
	for (Character * character : characters)
	{
		character->Update(deltaTime, e, GetOnScreenTiles());
	}

	for (Tile* tile : GetOnScreenTiles())
	{
		if (tile->isAnimated)
		{
			tile->Update(deltaTime);
		}
	}

	float cameraX = 0;

	for (Character* character : characters)
	{
		cameraX += (character->GetPosition().x + (character->GetSize().x / 2)) / characters.size();
	}

	cameraX -= SCREEN_WIDTH / 2;

	cameraX = std::max(cameraX, 0.0f);
	cameraX = std::min(cameraX, (float)GameScreen::levelWidth - SCREEN_WIDTH);
	GameScreen::cameraPosition.x = cameraX;
}

void LevelScreen::LoadFromJSON(std::string levelPath)
{
	std::ifstream fin(levelPath, std::ios::in | std::ios::binary);

	if (!fin)
	{
		std::cout << "Could not access level file: " << levelPath << std::endl;
	}

	Json::Value json;
	Json::Reader reader;

	reader.parse(fin, json);

	for (Json::Value tile : json["Tiles"])
	{
		Vector2D tilePosition = Vector2D(tile["x"].asFloat(), tile["y"].asFloat());
		tileData tileInfo = allTiles.at(tile["TileDataReference"].asString());
		std::string filePath = tile["ImageDirectory"].asString();

		std::array<COLLISION_SIDES, 4> collSides = { NONE, NONE, NONE, NONE };
		for (int i = 0; i < 4; i++)
		{
			collSides[i] = static_cast<COLLISION_SIDES>(tile["CollisionDirections"][i].asInt());
		}

		tileMap.push_back(new Tile(m_renderer, tilePosition, tileInfo, filePath, collSides, CAMERA_SCALE));
	}

	fin.close();
}

bool LevelScreen::SetUpLevel(bool multiplayer)
{
	int furthestPoint = 0;

	for (Tile* tile : tileMap)
	{
		if (tile->position.x + TILE_RESOLUTION * CAMERA_SCALE > furthestPoint)
		{
			furthestPoint = tile->position.x + TILE_RESOLUTION * CAMERA_SCALE;
		}
	}

	GameScreen::levelWidth = furthestPoint;

	// Set up player character
	characters.push_back((Character*)new SmallMario(m_renderer, "Sprites/Small Mario/Idle.png", Vector2D(64, 64), CAMERA_SCALE, cameraPosition));

	if (multiplayer)
	{
		characters.push_back((Character*)new SmallLuigi(m_renderer, "Sprites/Small Luigi/Idle.png", Vector2D(64, 64), CAMERA_SCALE, cameraPosition));
	}

	return true;
}

std::vector<Tile*> LevelScreen::GetOnScreenTiles()
{
	std::vector<Tile*> onscreenTiles;

	for (Tile* tile : tileMap)
	{
		if (tile->position.x < cameraPosition.x + SCREEN_WIDTH && tile->position.x + tile->coll.size.x > cameraPosition.x)
		{
			onscreenTiles.push_back(tile);
		}
	}

	return onscreenTiles;
}
