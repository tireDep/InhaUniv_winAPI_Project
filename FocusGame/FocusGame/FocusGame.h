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
	POINT nextSpot;
	RECT shotBullet;

	int type;
	bool isShot;
	float speed;
};

struct parceCannon
{
	POINT pos;
	int type;
};

struct explodStruct
{
	POINT centerPos;
	RECT explodeRect;

	HBITMAP hAniImg;
	BITMAP bitAni;
	
	bool isStart;
	int maxFrame;
	int curFrame;
};

#define dNormal -1
#define dHoming 1