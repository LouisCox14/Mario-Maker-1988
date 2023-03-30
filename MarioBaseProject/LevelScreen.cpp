#include "LevelScreen.h"
#include "Texture2D.h"
#include "SmallMario.h"
#include "SmallLuigi.h"
#include <iostream>
#include <fstream>
#include <string>
#include <json\json.h>
#include <json\value.h>

LevelScreen::LevelScreen(SDL_Renderer* renderer, GameScreenManager* _screenManager, std::string _levelPath, bool multiplayer) : GameScreen(renderer, _screenManager)
{
	levelPath = _levelPath;
	LoadFromJSON();
	SetUpLevel(multiplayer);

	audioPlayer = new AudioPlayer({ "NewLevel", "Death"}, {"LevelMusic1", "LevelMusic2"});

	if (multiplayer)
		audioPlayer->PlayMusic("LevelMusic2");
	else
		audioPlayer->PlayMusic("LevelMusic1");

	audioPlayer->PlayClip("NewLevel");
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
	Vector2D newCameraPosition = Vector2D(0, 0);

	for (Character* character : characters)
	{
		character->Update(deltaTime, e, GetOnScreenTiles());

		if (character->GetPosition().y > levelHeight + 5)
		{
			gameOver = true;
		}

		newCameraPosition.x += (character->GetPosition().x + (character->GetSize().x / 2)) / characters.size();
		newCameraPosition.y += (character->GetPosition().y + (character->GetSize().y / 2)) / characters.size();
	}

	newCameraPosition -= Vector2D(SCREEN_WIDTH, SCREEN_HEIGHT) / 2;

	newCameraPosition.x = std::max(newCameraPosition.x, 0.0f);
	newCameraPosition.x = std::min(newCameraPosition.x, (float)levelWidth - SCREEN_WIDTH);

	newCameraPosition.y = std::max(newCameraPosition.y, 0.0f);
	newCameraPosition.y = std::min(newCameraPosition.y, (float)levelHeight - SCREEN_HEIGHT);

	cameraPosition = newCameraPosition;

	for (Tile* tile : GetOnScreenTiles())
	{
		if (tile->isAnimated)
		{
			tile->Update(deltaTime);
		}
	}

	if (gameOver)
		GameOver();
}

void LevelScreen::GameOver()
{
	audioPlayer->StopMusic();
	audioPlayer->PlayClip("Death");
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

	int alpha = 0;
	while (alpha < 255)
	{
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, alpha);
		SDL_RenderFillRect(m_renderer, NULL);
		SDL_Delay(85);
		SDL_RenderPresent(m_renderer);
		alpha += 5;
	}

	GameScreen::screenManager->ChangeScreen(SCREEN_LEVEL, levelPath);
}

void LevelScreen::LoadFromJSON()
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
	int highestX = 0;
	int highestY = 0;

	for (Tile* tile : tileMap)
	{
		if (tile->position.x + TILE_RESOLUTION * CAMERA_SCALE > highestX)
		{
			highestX = tile->position.x + TILE_RESOLUTION * CAMERA_SCALE;
		}
		if (tile->position.y + TILE_RESOLUTION * CAMERA_SCALE > highestY)
		{
			highestY = tile->position.y + TILE_RESOLUTION * CAMERA_SCALE;
		}
	}

	levelWidth = highestX;
	levelHeight = highestY;

	// Set up player character
	characters.push_back((Character*)new SmallMario(m_renderer, this, "Assets/Sprites/Small Mario/Idle.png", Vector2D(64, 64), CAMERA_SCALE, cameraPosition));

	if (multiplayer)
	{
		characters.push_back((Character*)new SmallLuigi(m_renderer, this, "Assets/Sprites/Small Luigi/Idle.png", Vector2D(64, 64), CAMERA_SCALE, cameraPosition));
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
