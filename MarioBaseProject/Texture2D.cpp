#include "Texture2D.h"
#include <SDL_image.h>
#include <iostream>

Texture2D::Texture2D(SDL_Renderer* renderer, float _scale)
{
	m_renderer = renderer;
	scale = _scale;
}

Texture2D::~Texture2D()
{
	//Free up memory
	Free();

	m_renderer = nullptr;
}

bool Texture2D::LoadFromFile(std::string path)
{
	// Remove memory used for a previous texture
	Free();

	// Load the image
	SDL_Surface* p_surface = IMG_Load(path.c_str());

	if (p_surface != nullptr)
	{
		// Colour key the image to be transparent
		SDL_SetColorKey(p_surface, SDL_TRUE, SDL_MapRGB(p_surface->format, 0, 0xFF, 0xFF));

		// Create the texture from the pixels on the surface
		m_texture = SDL_CreateTextureFromSurface(m_renderer, p_surface);

		if (m_texture == nullptr)
		{
			std::cout << "Unable to create texture from surface. Error: " << SDL_GetError();
		}
		else
		{
			m_width = static_cast<int>((float)p_surface->w * scale + 0.5f);
			m_height = static_cast<int>((float)p_surface->h * scale + 0.5f);
		}

		// Remove the loaded surface now that we have a texture
		SDL_FreeSurface(p_surface);
	}
	else
	{
		std::cout << "Unable to create texture from surface. Error: " << IMG_GetError();
	}

	// Return whether the process was successful
	return m_texture != nullptr;
}

void Texture2D::Free()
{
	// Check the texture exists before destroying it
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_width = 0;
		m_height = 0;
		m_texture = nullptr;
	}
}

void Texture2D::Render(Vector2D new_position, SDL_RendererFlip flip, double angle)
{
	// Set where to render the texture
	SDL_Rect renderLocation = { static_cast<int>(new_position.x), static_cast<int>(new_position.y), m_width, m_height };

	// Render to screen
	SDL_RenderCopyEx(m_renderer, m_texture, NULL, &renderLocation, angle, NULL, flip);
}