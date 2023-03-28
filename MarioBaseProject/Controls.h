#pragma once
#include <SDL.h>

struct CharacterControls
{
	SDL_Scancode rightMove;
	SDL_Scancode leftMove;
	SDL_KeyCode jump;

	CharacterControls(SDL_Scancode _rightMove = SDL_SCANCODE_A, SDL_Scancode _leftMove = SDL_SCANCODE_D, SDL_KeyCode _jump = SDLK_SPACE)
	{
		rightMove = _rightMove;
		leftMove = _leftMove;
		jump = _jump;
	}
};