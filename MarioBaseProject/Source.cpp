//Be grateful for humble beginnings, because the next level will always require so much more of you
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "constants.h"
#include <iostream>

//Globals
SDL_Window* g_window = nullptr;

// Function prototypes
bool InitSDL();
void CloseSDL();

int main(int argc, char* args[])
{
	// Check SDL setup
	if (InitSDL())
	{
		SDL_Delay(5000);
	}

	CloseSDL();

	return 0;
}

bool InitSDL()
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		// Setup passed, so create a window
		g_window = SDL_CreateWindow("Games Engine Creation",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
			);

		// Check the window was created successfully
		if (g_window == nullptr)
		{
			// Window failed
			std::cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}

		return true;
	}
}

void CloseSDL()
{
	// Release the window
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}