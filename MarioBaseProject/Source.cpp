//Be grateful for humble beginnings, because the next level will always require so much more of you
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "constants.h"
#include <iostream>

//Globals
SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Texture* g_texture = nullptr;

// Function prototypes
bool InitSDL();
void CloseSDL();
bool Update();
void Render(int angle = 0, int direction = 0);
SDL_Texture* LoadTextureFromFile(std::string path);
void FreeTexture();

int imageAngle = 0;
int imageFlip = 0;

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
			Render(imageAngle, imageFlip);
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
		g_texture = LoadTextureFromFile("Images/test.bmp");

		if (g_texture == nullptr)
		{
			return false;
		}

		return true;
	}
}

void CloseSDL()
{
	// Clear the texture
	FreeTexture();

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

void Render(int angle, int direction)
{
	// Clear the screen
	SDL_SetRenderDrawColor(g_renderer, 1, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(g_renderer);

	// Set where to render the texture
	SDL_Rect renderLocation = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// Render to screen
	SDL_RenderCopyEx(g_renderer, g_texture, NULL, &renderLocation, angle, NULL, (SDL_RendererFlip)direction);


	// Update the screen
	SDL_RenderPresent(g_renderer);
}

void FreeTexture()
{
	// Check the texture exists before destroying it
	if (g_texture != nullptr)
	{
		SDL_DestroyTexture(g_texture);
		g_texture = nullptr;
	}
}

SDL_Texture* LoadTextureFromFile(std::string path)
{
	// Remove memory used for a previous texture
	FreeTexture();

	SDL_Texture* p_texture = nullptr;

	// Load the image
	SDL_Surface* p_surface = IMG_Load(path.c_str());

	if (p_surface != nullptr)
	{
		// Create the texture from the pixels on the surface
		p_texture = SDL_CreateTextureFromSurface(g_renderer, p_surface);

		if (p_texture == nullptr)
		{
			std::cout << "Unable to create texture from surface. Error: " << SDL_GetError();
		}

		// Remove the loaded surface now that we have a texture
		SDL_FreeSurface(p_surface);
	}
	else
	{
		std::cout << "Unable to create texture from surface. Error: " << IMG_GetError();
	}

	// Return the texture
	return p_texture;
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
				case SDLK_SPACE:
					imageAngle += 10;
					break;
				case SDLK_a:
					imageFlip = 1;
					break;
				case SDLK_d:
					imageFlip = 0;
					break;
			}
	}

	return false;
}