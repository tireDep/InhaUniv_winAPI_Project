#pragma once

#include "resource.h"

enum MapType
{
	eMapBlock = 50, eMapSpike = 100,
	eMapCannon_0 = 150, eMapCannon_1 = 151, eMapCannon_2 = 152, eMapCannon_3 = 153,	// 일반
	eMapCannon_4 = 154, eMapCannon_5 = 155, eMapCannon_6 = 156, eMapCannon_7 = 157,	// 유도

	eMapGate_0 = 200, eMapGate_1 = 201, eMapGate_2 = 202, eMapGate_3 = 203,	// 게이트close?
	eMapGateOpen = 204, eMapGateCloseVertical = 205, eMapGateCloseHorizen = 206,

	eMapBtn_0 = 250, eMapBtn_1 = 251,	// 버튼(스위치)
	eMapBtn_2 = 252, eMapBtn_3 = 253	// 버튼(블럭)
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

	HBITMAP hbitmap;
	BITMAP bitmap;

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