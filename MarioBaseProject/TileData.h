#pragma once
#include <string>
#include <map>
#include <string>

struct tileData
{
	std::string fileName;

	bool autoTileTop;
	bool autoTileRight;
	bool autoTileBottom;
	bool autoTileLeft;

	tileData(std::string _fileName = "", bool _autoTileTop = false, bool _autoTileRight = false, bool _autoTileBottom = false, bool _autoTileLeft = false)
	{
		fileName = _fileName;
		autoTileTop = _autoTileTop;
		autoTileRight = _autoTileRight;
		autoTileBottom = _autoTileBottom;
		autoTileLeft = _autoTileLeft;
	}
};

std::map<char, tileData> tileDict = {
	{'g', tileData(std::string("Ground"), true, true, false, true)},
	{'b', tileData(std::string("Green Block"), true, true, true, true)},
};