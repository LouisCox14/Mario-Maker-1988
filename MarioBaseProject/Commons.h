#pragma once
#include <cmath>
#include <vector>
#include <iostream>
#include <array>

enum SCREENS
{
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES,
	SCREEN_LEVEL_CREATOR
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
	LEFT,
	NONE
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
		return Vector2D(x / a, y / a);
	}

	Vector2D& operator/=(const float& a)
	{
		x = x / a;
		y = y / a;
		return *this;
	}

	Vector2D operator%(const float& a) const
	{
		return Vector2D((int)x % (int)a, (int)y % (int)a);
	}

	Vector2D& operator%=(const float& a)
	{
		x = (int)x % (int)a;
		y = (int)y % (int)a;
		return *this;
	}

	Vector2D operator-(const Vector2D& a) const
	{
		return Vector2D(x - a.x, y - a.y);
	}

	Vector2D& operator-=(const Vector2D& a)
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}

	bool operator==(const Vector2D& a) const
	{
		return x == a.x && y == a.y;
	}
};

struct BoxCollider
{
	Vector2D position;
	Vector2D size;
	std::array<COLLISION_SIDES, 4> collisionSides;

	BoxCollider(Vector2D _position = Vector2D(0, 0), Vector2D _size = Vector2D(0, 0), std::array<COLLISION_SIDES, 4> _collisionSides = { TOP, RIGHT, BOTTOM, LEFT })
	{
		position = _position;
		size = _size;
		collisionSides = _collisionSides;
	}

	COLLISION_SIDES GetCollisionSide(BoxCollider other)
	{
		if (abs(position.y - other.position.y) < size.y / 2 + other.size.y / 2 && abs(position.x - other.position.x) < size.x / 2 + other.size.x / 2 - 1)
		{
			if (collisionSides[(int)TOP] == TOP && other.collisionSides[(int)BOTTOM] == BOTTOM && position.y - size.y / 2 > other.position.y + other.size.y / 2 - 2)
			{
				return TOP;
			}
			else if (collisionSides[(int)BOTTOM] == BOTTOM && other.collisionSides[(int)TOP] == TOP && position.y + size.y / 2 < other.position.y - other.size.y / 2 + 2)
			{
				return BOTTOM;
			}
		}

		if (abs(position.x - other.position.x) < size.x / 2 + other.size.x / 2 && abs(position.y - other.position.y) < size.y / 2 + other.size.y / 2 - 1)
		{
			if (collisionSides[(int)LEFT] == LEFT && other.collisionSides[(int)RIGHT] == RIGHT && position.x - size.x / 2 > other.position.x + other.size.x / 2 - 2)
			{
				return LEFT;
			}
			else if (collisionSides[(int)RIGHT] == RIGHT && other.collisionSides[(int)LEFT] == LEFT && position.x + size.x / 2 < other.position.x - other.size.x / 2 + 2)
			{
				return RIGHT;
			}
		}

		return NONE;
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