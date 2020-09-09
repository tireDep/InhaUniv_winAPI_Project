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

	Rectangle(hdc, 800, 0, eWindowWidth, eWindowHeight);
}

void Map::RenderMap(HWND hWnd, HDC hdc)
{
	DrawMap(hdc);
}
