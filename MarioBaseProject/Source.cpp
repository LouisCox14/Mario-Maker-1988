//Be grateful for humble beginnings, because the next level will always require so much more of you
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "constants.h"
#include "Commons.h"
#include "Texture2D.h"
#include <iostream>

//Globals
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Texture2D* g_texture = nullptr;

// Function prototypes
bool InitSDL();
void CloseSDL();
bool Update();
void Render();

int main(int argc, char* args[])
{
	// Check SDL setup
	if (InitSDL())
	{
		// Flag variable to keep the window open
		bool quit = false;

		//Game loop
		while (!quit)
		{
			Render();
			quit = Update();
		}
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

		g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

		if (g_renderer != nullptr)
		{
			// Initialise PNG loading
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags) & imageFlags))
			{
				std::cout << "SDL_Image could not initialise. Error: " << IMG_GetError();
				return false;
			}
		}
		else
		{
			std::cout << "Renderer could not initialise. Error: " << SDL_GetError();
			return false;
		}

		// Load the background texture
		g_texture = new Texture2D(g_renderer);

		if (!g_texture->LoadFromFile("Images/test.bmp"))
		{
			return false;
		}

		if (g_texture == nullptr)
		{
			return false;
		}

		return true;
	}
}

void CloseSDL()
{
	// Release the texture
	delete g_texture;
	g_texture = nullptr;

	// Release the renderer
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;

	// Release the window
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Render()
{
	// Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 1, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);

	g_texture->Render(Vector2D(), SDL_FLIP_NONE);

	// Update the screen
	SDL_RenderPresent(g_renderer);
}

bool Update()
{
	// Event handler
	SDL_Event e;

	// Get events
	SDL_PollEvent(&e);

	// Handle the events
	switch (e.type)
	{
		case SDL_QUIT:
			return true;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					return true;
					break;
			}
	}

	return false;
}