#pragma once
#ifndef _TEXTURE2D_H
#define _TEXTURE2D_H

#include <SDL.h>
#include <string>
#include "Commons.h"

class Texture2D
{
	public:
		Texture2D(SDL_Renderer* renderer, float _scale);
		~Texture2D();

		float scale = 1.0f;

		bool LoadFromFile(std::string path);
		void Free();
		void Render(Vector2D new_position, SDL_RendererFlip flip, double angle = 0.0f);

		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }
		std::string GetFilePath() { return filePath; }

	private:
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;

		int m_width;
		int m_height;

		std::string filePath;

		Vector2D imageRect;
};

#endif //_TEXTURE2D_H