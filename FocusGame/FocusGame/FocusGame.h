#pragma once

#include "resource.h"

enum MapType
{
	eMapBlock = 50, eMapObstacle = 100,
	eMapCannon_0 = 150, eMapCannon_1 = 151, eMapCannon_2 = 152, eMapCannon_3 = 153,
	eMapCannon_4 = 154, eMapCannon_5 = 155, eMapCannon_6 = 156, eMapCannon_7 = 157
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

struct parceCannon
{
	POINT pos;
	int type;
};

#define dNormal -1
#define dHoming 1