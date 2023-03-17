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

	tileData(std::string _fileName = "", std::array<COLLISION_SIDES, 4> _autoTileSides = { NONE, NONE, NONE, NONE })
	{
		fileName = _fileName;
		autoTileSides = _autoTileSides;
		colliderType = SINGLE;
		collisionSides = { TOP, RIGHT, BOTTOM, LEFT };
	}

	tileData(std::string _fileName, std::array<COLLISION_SIDES, 4> _autoTileSides, COLLIDER_TYPE _colliderType, std::array<COLLISION_SIDES, 4> _collisionSides = { TOP, RIGHT, BOTTOM, LEFT })
	{
		fileName = _fileName;
		autoTileSides = _autoTileSides;
		colliderType = _colliderType;
		collisionSides = _collisionSides;
	}
};

static std::map<char, tileData> tileDict = {
	{'g', tileData(std::string("Ground"), {TOP, RIGHT, NONE, LEFT})},
	{'b', tileData(std::string("Green Block"), {TOP, RIGHT, BOTTOM, LEFT}, COMPOSITE, {TOP, NONE, NONE, NONE})},
};