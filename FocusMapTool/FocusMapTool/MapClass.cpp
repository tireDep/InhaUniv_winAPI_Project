#include "stdafx.h"
#include "MapClass.h"

Map::Map()
{

}

Map::~Map()
{

}

Map * Map::GetInstance()
{
	static Map map;
	return &map;
}

void Map::DrawMap(HDC hdc)
{
	HBRUSH hBrush, oldBrush;

	hBrush = CreateSolidBrush(RGB(36, 36, 36));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	PatBlt(hdc, 0, 0, eWindowWidth, eWindowWidth, PATCOPY);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);

	for (int i = 0; i < eWindowWidth; i += 16)
	{
		MoveToEx(hdc, i, 0, NULL);
		LineTo(hdc, i, eWindowHeight);
	}

	for (int i = 0; i < eWindowHeight; i += 16)
	{
		MoveToEx(hdc, 0, i, NULL);
		LineTo(hdc, eWindowWidth, i);
	}

	hBrush = CreateSolidBrush(RGB(100, 100, 100));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, 800, 0, eWindowWidth, eWindowHeight);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);

	Rectangle(hdc, 840, 50, 856, 66);
	Rectangle(hdc, 870, 50, 886, 66);
	Rectangle(hdc, 900, 50, 916, 66);
	//Rectangle(hdc, 930, 50, 946, 66);

	Rectangle(hdc, 840, 100, 856, 116);
	Rectangle(hdc, 870, 100, 886, 116);
	Rectangle(hdc, 900, 100, 916, 116);
	//Rectangle(hdc, 930, 100, 946, 116);

	Rectangle(hdc, 840, 150, 856, 166);
	Rectangle(hdc, 840, 166, 856, 182);

	Rectangle(hdc, 870, 150, 886, 166);
	Rectangle(hdc, 870, 166, 886, 182);

	Rectangle(hdc, 900, 150, 916, 166);
	Rectangle(hdc, 916, 150, 932, 166);
	Rectangle(hdc, 900, 166, 916, 182);
	Rectangle(hdc, 916, 166, 932, 182);

	Rectangle(hdc, 900, 222, 916, 238);
	Rectangle(hdc, 916, 222, 932, 238);
	Rectangle(hdc, 900, 238, 916, 254);
	Rectangle(hdc, 916, 238, 932, 254);

	Rectangle(hdc, 840, 222, 856, 238);
	Rectangle(hdc, 856, 222, 872, 238);
	Rectangle(hdc, 840, 238, 856, 254);
	Rectangle(hdc, 856, 238, 872, 254);

	Rectangle(hdc, 840, 300, 940, 350);

	Rectangle(hdc, 840, 400, 940, 450);
}

void Map::RenderMap(HWND hWnd, HDC hdc)
{
	DrawMap(hdc);
}
