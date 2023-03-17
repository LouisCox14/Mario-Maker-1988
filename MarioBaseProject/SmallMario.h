#pragma once
#include "Character.h"

class SmallMario : Character
{
	public:
		SmallMario(SDL_Renderer* renderer, std::string imagePath, Vector2D startPosition, float scale, Vector2D& _cameraPosition);
		void Animate() override;
};