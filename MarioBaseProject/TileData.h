#pragma once
#include <string>
#include <map>
#include <string>
#include <array>
#include "Commons.h"

enum COLLIDER_TYPE
{
	COMPOSITE,
	SINGLE
};

struct tileData
{
	std::string fileName;
	std::array<COLLISION_SIDES, 4> autoTileSides;

	COLLIDER_TYPE colliderType;
	std::array<COLLISION_SIDES, 4> collisionSides;

	bool isAnimated;
	int animationFrames;
	float animationDelay;

	tileData(std::string _fileName = "", std::array<COLLISION_SIDES, 4> _autoTileSides = { NONE, NONE, NONE, NONE })
	{
		fileName = _fileName;
		autoTileSides = _autoTileSides;

		colliderType = SINGLE;
		collisionSides = { TOP, RIGHT, BOTTOM, LEFT };

		isAnimated = false;
	}

	tileData(std::string _fileName, std::array<COLLISION_SIDES, 4> _autoTileSides, COLLIDER_TYPE _colliderType, std::array<COLLISION_SIDES, 4> _collisionSides = { TOP, RIGHT, BOTTOM, LEFT })
	{
		fileName = _fileName;
		autoTileSides = _autoTileSides;

		colliderType = _colliderType;
		collisionSides = _collisionSides;

		isAnimated = false;
	}

	tileData(std::string _fileName, int _animationFrames, float _animationDelay)
	{
		fileName = _fileName;
		autoTileSides = { NONE, NONE, NONE, NONE };

		colliderType = SINGLE;
		collisionSides = { TOP, RIGHT, BOTTOM, LEFT };

		isAnimated = true;
		animationFrames = _animationFrames;
		animationDelay = _animationDelay;
	}
};

static std::map<char, tileData> tileDict = 
{
	{'g', tileData(std::string("Ground"), {TOP, RIGHT, NONE, LEFT}, COMPOSITE)},
	{'b', tileData(std::string("Green Block"), {TOP, RIGHT, BOTTOM, LEFT}, COMPOSITE, {TOP, NONE, NONE, NONE})},
	{'w', tileData(std::string("Wood Block"))},
	{'p', tileData(std::string("Pipe"), {TOP, RIGHT, NONE, LEFT}, COMPOSITE)},
	{'s', tileData(std::string("Shiny Block"), 4, 0.15f)},
	{'q', tileData(std::string("Question Block"), 4, 0.15f)}
};