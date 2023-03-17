#pragma once
#include <SDL.h>

struct CharacterControls
{
	SDL_KeyCode rightMove;
	SDL_KeyCode leftMove;
	SDL_KeyCode jump;

	CharacterControls(SDL_KeyCode _rightMove = SDLK_d, SDL_KeyCode _leftMove = SDLK_a, SDL_KeyCode _jump = SDLK_SPACE)
	{
		rightMove = _rightMove;
		leftMove = _leftMove;
		jump = _jump;
	}
};