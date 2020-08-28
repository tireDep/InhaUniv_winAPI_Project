#pragma once

#include "resource.h"

enum MapType
{
	eMapBlock = 50, eMapObstacle = 100,
	eMapCannon_0 = 150, eMapCannon_1 = 151, eMapCannon_2 = 152, eMapCannon_3 = 153
};

struct TileMap
{
	RECT pos;
	int type;
};

struct BulletSctruct
{
	POINT centerPos;
	int type;
	bool isShot;
};