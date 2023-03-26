#pragma once
#include <vector>
#include <string>
#include <array>
#include <map>
#include "Commons.h"

enum TILE_TYPE
{
	COMPOSITE,
	SINGLE
};

struct tileData
{
	std::string fileName;

	TILE_TYPE spriteType;
	std::array<COLLISION_SIDES, 4> autoTileSides;
	std::string defaultSuffix;

	TILE_TYPE colliderType;
	std::array<COLLISION_SIDES, 4> collisionSides;

	bool isAnimated;
	int animationFrames;
	float animationDelay;

	tileData(std::string _fileName = "")
	{
		fileName = _fileName;

		spriteType = SINGLE;
		colliderType = SINGLE;

		isAnimated = false;
	}

	tileData(std::string _fileName, TILE_TYPE _spriteType, std::array<COLLISION_SIDES, 4> _autoTileSides, std::string _defaultSuffix)
	{
		fileName = _fileName;
		defaultSuffix = _defaultSuffix;

		spriteType = _spriteType;
		autoTileSides = _autoTileSides;

		colliderType = SINGLE;
		isAnimated = false;
	}

	tileData(std::string _fileName, TILE_TYPE _spriteType, TILE_TYPE _colliderType, std::array<COLLISION_SIDES, 4> _autoTileSides = {TOP, BOTTOM, RIGHT, LEFT}, std::array<COLLISION_SIDES, 4> _collisionSides = { TOP, BOTTOM, RIGHT, LEFT }, std::string _defaultSuffix = "")
	{
		fileName = _fileName;
		defaultSuffix = _defaultSuffix;

		spriteType = _spriteType;
		autoTileSides = _autoTileSides;

		colliderType = _colliderType;
		collisionSides = _collisionSides;

		isAnimated = false;
	}

	tileData(std::string _fileName, int _animationFrames, float _animationDelay)
	{
		fileName = _fileName;

		spriteType = SINGLE;
		colliderType = SINGLE;
		collisionSides = { TOP, BOTTOM, RIGHT, LEFT };

		isAnimated = true;
		animationFrames = _animationFrames;
		animationDelay = _animationDelay;
	}
};

const std::vector<tileData> terrainTiles = {
	tileData("Ground", COMPOSITE, COMPOSITE, {TOP, NONE, RIGHT, LEFT}, {TOP, NONE, RIGHT, LEFT}, " Top"),
	tileData("Green Block", COMPOSITE, COMPOSITE, {TOP, BOTTOM, RIGHT, LEFT}, {TOP, NONE, NONE, NONE}, " Top Right")
};

const std::vector<tileData> specialTiles = {
	tileData("Shiny Block", 4, 0.15f),
	tileData("Question Block", 4, 0.15f),
	tileData("Pipe", COMPOSITE, COMPOSITE, {TOP, NONE, RIGHT, LEFT}, {TOP, NONE, RIGHT, LEFT}, " Top Right")
};

const std::vector<tileData> decorationTiles = {
	tileData("Shrubbery", COMPOSITE, SINGLE, {NONE, NONE, RIGHT, LEFT}, {NONE, NONE, NONE, NONE})
};

const std::map<std::string, tileData> allTiles = {
	{"Ground", terrainTiles[0]},
	{"Green Block", terrainTiles[1]},
	{"Shiny Block", specialTiles[0]},
	{"Question Block", specialTiles[1]},
	{"Pipe", specialTiles[2]},
	{"Shrubbery", decorationTiles[0]}
};