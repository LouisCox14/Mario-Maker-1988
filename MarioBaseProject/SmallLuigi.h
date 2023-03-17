#pragma once
#include "Character.h"

class SmallLuigi : Character
{
public:
	SmallLuigi(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, float scale, Vector2D& _cameraPosition);
};