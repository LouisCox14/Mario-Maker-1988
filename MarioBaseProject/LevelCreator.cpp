#include "LevelCreator.h"

LevelCreator::LevelCreator(SDL_Renderer* renderer) : GameScreen(renderer) 
{
	camSpeed = 100.0f;
	camScale = 1.0f;
	scaleSpeed = 0.15f;
}

LevelCreator::~LevelCreator()
{
}

void LevelCreator::Render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	Vector2D gridOffset = GameScreen::cameraPosition % (TILE_RESOLUTION * camScale);

	for (int x = 0; x < SCREEN_WIDTH + TILE_RESOLUTION * camScale; x += TILE_RESOLUTION * camScale)
	{
		SDL_RenderDrawLine(m_renderer, x - gridOffset.x, 0, x - gridOffset.x, SCREEN_HEIGHT);
	}

	for (int y = 0; y < SCREEN_HEIGHT + TILE_RESOLUTION * camScale; y += TILE_RESOLUTION * camScale)
	{
		SDL_RenderDrawLine(m_renderer, 0, y - gridOffset.y, SCREEN_WIDTH, y - gridOffset.y);
	}

	for (Tile* tile : tileMap)
	{
		tile->Render(GameScreen::cameraPosition);
	}
}

void LevelCreator::Update(float deltaTime, SDL_Event e)
{
	if (e.key.repeat == 0)
	{
		switch (e.type)
		{
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_a:
						camMovement.x -= 1;
						break;
					case SDLK_d:
						camMovement.x += 1;
						break;
					case SDLK_w:
						camMovement.y -= 1;
						break;
					case SDLK_s:
						camMovement.y += 1;
						break;
				}

				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
					case SDLK_a:
						camMovement.x += 1;
						break;
					case SDLK_d:
						camMovement.x -= 1;
						break;
					case SDLK_w:
						camMovement.y += 1;
						break;
					case SDLK_s:
						camMovement.y -= 1;
						break;
				}

				break;
			case SDL_MOUSEWHEEL:
				if (camScale + e.wheel.y * scaleSpeed > 0.3f)
				{
					camScale += e.wheel.y * scaleSpeed;
					GameScreen::cameraPosition += Vector2D(SCREEN_WIDTH, SCREEN_HEIGHT) * e.wheel.y * scaleSpeed;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouseX, &mouseY);
				Vector2D mouseGridPos = PixelToGridPos(Vector2D((float)mouseX, (float)mouseY));
				PlaceTile(mouseGridPos, tileDict.at('s'));
				break;
		}
	}

	GameScreen::cameraPosition += camMovement * camSpeed * deltaTime;
	GameScreen::cameraPosition.x = std::max(GameScreen::cameraPosition.x, 0.0f);
	GameScreen::cameraPosition.y = std::max(GameScreen::cameraPosition.y, 0.0f);
}

Vector2D LevelCreator::PixelToGridPos(Vector2D pixelPosition)
{
	pixelPosition += GameScreen::cameraPosition;
	Vector2D gridPosition = pixelPosition / (TILE_RESOLUTION * camScale);
	return Vector2D((int)(gridPosition.x), (int)(gridPosition.y));
}

void LevelCreator::PlaceTile(Vector2D gridPosition, tileData tileInfo)
{
	tileMap.push_back(new Tile(m_renderer, gridPosition * TILE_RESOLUTION * camScale, tileInfo.fileName, tileInfo, camScale));
}
