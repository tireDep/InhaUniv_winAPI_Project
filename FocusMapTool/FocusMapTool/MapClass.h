#pragma once
#include "stdafx.h"
#include <vector>

using namespace std;

enum TileType
{
	eMapBlock = 50, eMapHalfBlock = 75, eMapSpike = 100,
	eMapCannon_0 = 150, eMapCannon_1 = 151, eMapCannon_2 = 152, eMapCannon_3 = 153,	// 일반
	eMapCannon_4 = 154, eMapCannon_5 = 155, eMapCannon_6 = 156, eMapCannon_7 = 157,	// 유도

	eMapGate_0 = 200, eMapGate_1 = 201, eMapGate_2 = 202, eMapGate_3 = 203,	// 게이트close?
	eMapGateOpen = 204, eMapGateCloseVertical = 205, eMapGateCloseHorizen = 206,

	eMapBtn_0 = 250, eMapBtn_1 = 251,	// 버튼(스위치)
	eMapBtn_2 = 252, eMapBtn_3 = 253,	// 버튼(블럭)

	eMapItem = 300, ePlayerResen = 500
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

	POINT showPos;
};

class Map
{
private:
	RECT basicBlock;
	RECT halfBlock;
	RECT spike;

	RECT item;
	RECT blockVerticalRazer;
	RECT blockHorizenRazer;

	RECT switchOn;
	RECT btnOn;

	RECT gate1;
	RECT gate2;
	RECT gate3;
	RECT gate4;

	RECT basicCannon1;
	RECT basicCannon2;
	RECT basicCannon3;
	RECT basicCannon4;

	RECT homingCannon1;
	RECT homingCannon2;
	RECT homingCannon3;
	RECT homingCannon4;

	RECT playerResenBtn;

	RECT openFileBtn;
	RECT saveFileBtn;

	RECT nowTypeRect;

	HBITMAP hMapBitmap;
	BITMAP mapBitmap;

	vector<TileMap> tileMap;
	int nowType;

	TCHAR typeName[128];
	POINT typePos;

	Map();

public:
	~Map();

	static Map* GetInstance();

	void DrawMap(HDC hdc);
	void RenderMap(HWND hWnd, HDC hdc);

	void DrawBtn(HDC hdc, RECT rect);

	void AddTile(POINT pos);
	void AddTile(POINT startPos, POINT lastPos);
	void PushBack(RECT addRect);

	void DeleteTile(POINT pos);
	void DeleteTile(POINT startPos, POINT lastPos);

	RECT CalcBtn(RECT rect);
	RECT CalcTileRange(POINT pos);
	void CalcMulNum(int moveX, int moveY, int &mulX, int &mulY);
	int CheckIsSameRect(RECT rect);

	void CheckIsDoublePos(RECT rect);

	void SetNowType(POINT pos);

	POINT SetShowType(int type);
};