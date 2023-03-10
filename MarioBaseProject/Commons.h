#pragma once

struct Vector2D
{
	float x;
	float y;

	Vector2D(float initial_x = 0.0f, float initial_y = 0.0f)
	{
		x = initial_x;
		y = initial_y;
	}
};

enum SCREENS
{
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum FACING
{
	FACING_LEFT,
	FACING_RIGHT
};