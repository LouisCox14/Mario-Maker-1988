#pragma once
#include <cmath>
#include <vector>

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

enum COLLISION_SIDES
{
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
};

struct Vector2D
{
	float x;
	float y;

	Vector2D(float initial_x = 0.0f, float initial_y = 0.0f)
	{
		x = initial_x;
		y = initial_y;
	}

	Vector2D operator*(const float& a) const
	{
		return Vector2D(a * x, a * y);
	}

	Vector2D& operator*=(const float& a)
	{
		x = x * a;
		y = y * a;
		return *this;
	}

	Vector2D operator*(const Vector2D& a) const
	{
		return Vector2D(a.x * x, a.y * y);
	}

	Vector2D& operator*=(const Vector2D& a)
	{
		x *= a.x;
		y *= a.y;
		return *this;
	}

	Vector2D operator+(const Vector2D& a) const
	{
		return Vector2D(a.x + x, a.y + y);
	}

	Vector2D& operator+=(const Vector2D& a)
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	Vector2D operator/(const float& a) const
	{
		return Vector2D(a / x, a / y);
	}

	Vector2D& operator/=(const float& a)
	{
		x = x / a;
		y = y / a;
		return *this;
	}

	Vector2D operator-(const Vector2D& a) const
	{
		return Vector2D(a.x - x, a.y - y);
	}

	Vector2D& operator-=(const Vector2D& a)
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}
};

struct BoxCollider
{
	Vector2D position;
	Vector2D size;

	BoxCollider(Vector2D _position = Vector2D(0, 0), Vector2D _size = Vector2D(0, 0))
	{
		position = _position;
		size = _size;
	}

	COLLISION_SIDES GetCollisionSide(BoxCollider other)
	{
		if (abs(position.x - other.position.x) < size.x / 2 + other.size.x / 2)
		{
			if (position.x > other.position.x)
			{
				return LEFT;
			}
			else
			{
				return RIGHT;
			}
		}

		if (abs(position.y - other.position.y) < size.y / 2 + other.size.y / 2)
		{
			if (position.y > other.position.y)
			{
				return TOP;
			}
			else
			{
				return BOTTOM;
			}
		}
	}

	bool OverlapCheck(BoxCollider other)
	{

		if (abs(position.x - other.position.x) > size.x / 2 + other.size.x / 2 || abs(position.y - other.position.y) > size.y / 2 + other.size.y / 2)
		{
			return false;
		}

		return true;
	}
};