#pragma once
#include "stdafx.h"

enum TileType
{
	eMapBlock = 50, eMapHalfBlock = 75, eMapSpike = 100,
	eMapCannon_0 = 150, eMapCannon_1 = 151, eMapCannon_2 = 152, eMapCannon_3 = 153,	// �Ϲ�
	eMapCannon_4 = 154, eMapCannon_5 = 155, eMapCannon_6 = 156, eMapCannon_7 = 157,	// ����

	eMapGate_0 = 200, eMapGate_1 = 201, eMapGate_2 = 202, eMapGate_3 = 203,	// ����Ʈclose?
	eMapGateOpen = 204, eMapGateCloseVertical = 205, eMapGateCloseHorizen = 206,

	eMapBtn_0 = 250, eMapBtn_1 = 251,	// ��ư(����ġ)
	eMapBtn_2 = 252, eMapBtn_3 = 253,	// ��ư(��)

	eMapItem = 300
};

enum Value
{
	eWindowWidth = 1000, eWindowHeight = 651,
	eTrueWinWidth = 800, eTrueWinHeight = 592,

	eBlockSize = 16

};

struct TileMap
{
	RECT pos;
	int type;
};

class Map
{
private:
	Map();

public:
	~Map();

	static Map* GetInstance();

	void DrawMap(HDC hdc);
	void RenderMap(HWND hWnd, HDC hdc);

};