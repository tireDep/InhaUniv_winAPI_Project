#include "stdafx.h"
#include "MapClass.h"
#include<commdlg.h>

#pragma comment (lib, "msimg32.lib")

Map::Map()
{
	basicBlock = { 840, 50, 856, 66 };
	halfBlock = { 870, 50, 886, 66 };
	spike = { 900, 50, 916, 66 };
	item = { 930, 50, 946, 66 };

	blockVerticalRazer = { 840, 100, 856, 116 };
	blockHorizenRazer = { 870, 100, 886, 116 };
	switchOn = { 900, 100, 916, 116 };
	btnOn = { 930, 100, 946, 116 };

	gate1 = { 840, 150, 856, 166 };
	gate2 = { 870, 150, 886, 166 };
	gate3 = { 930, 150, 946, 166 };
	gate4 = { 900, 150, 916, 166 }; 

	basicCannon1 = { 840, 200, 856, 216 };
	basicCannon2 = { 870, 200, 886, 216 };
	basicCannon3 = { 930, 200, 946, 216 };
	basicCannon4 = { 900, 200, 916, 216 };

	homingCannon1 = { 840, 250, 856, 266 };
	homingCannon2 = { 870, 250, 886, 266 };
	homingCannon3 = { 930, 250, 946, 266 };
	homingCannon4 = { 900, 250, 916, 266 };

	playerResenBtn = { 840, 300, 946, 340 };

	openFileBtn = { 840, 400, 946, 450 };
	saveFileBtn = { 840, 480, 946, 530 };

	hMapBitmap = (HBITMAP)LoadImage((NULL), TEXT("Image/tile.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hMapBitmap, sizeof(BITMAP), &mapBitmap);
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

	hBrush = CreateSolidBrush(RGB(150, 150, 150));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, 800, 0, eWindowWidth, eWindowHeight);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);

	DrawBtn(hdc, basicBlock);
	DrawBtn(hdc, halfBlock);
	DrawBtn(hdc, spike);
	DrawBtn(hdc, item);

	DrawBtn(hdc, blockVerticalRazer);
	DrawBtn(hdc, blockHorizenRazer);
	DrawBtn(hdc, switchOn);
	DrawBtn(hdc, btnOn);

	DrawBtn(hdc, gate1);
	DrawBtn(hdc, gate2);
	DrawBtn(hdc, gate3);
	DrawBtn(hdc, gate4);

	DrawBtn(hdc, basicCannon1);
	DrawBtn(hdc, basicCannon2);
	DrawBtn(hdc, basicCannon3);
	DrawBtn(hdc, basicCannon4);

	DrawBtn(hdc, homingCannon1);
	DrawBtn(hdc, homingCannon2);
	DrawBtn(hdc, homingCannon3);
	DrawBtn(hdc, homingCannon4);

	DrawBtn(hdc, playerResenBtn);
	DrawText(hdc, TEXT("Player Resen"), _tcslen(TEXT("Player Resen")), &playerResenBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DrawBtn(hdc, openFileBtn);
	DrawText(hdc, TEXT("Open File"), _tcslen(TEXT("Open File")), &openFileBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DrawBtn(hdc, saveFileBtn);
	DrawText(hdc, TEXT("Save File"), _tcslen(TEXT("Save File")), &saveFileBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	for (int i = 0; i < tileMap.size(); i++)
	{
		Rectangle(hdc, tileMap[i].pos.left, tileMap[i].pos.top, tileMap[i].pos.right, tileMap[i].pos.bottom);
	}
}

void Map::RenderMap(HWND hWnd, HDC hdc)
{
	DrawMap(hdc);

	HDC hmemDc;
	HBITMAP hOldBitmap;
	POINT spot = { 0,0 };

	hmemDc = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hmemDc, hMapBitmap);

	// >> btn Image
	TransparentBlt(hdc, basicBlock.left, basicBlock.top, 16, 16, hmemDc, 0, 0, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, halfBlock.left, halfBlock.top, 16, 16, hmemDc, 16, 0, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, spike.left, spike.top, 16, 16, hmemDc, 0, 16, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, item.left, item.top, 16, 16, hmemDc, 0, 16 * 8, 16, 16, RGB(255, 0, 255));
	
	TransparentBlt(hdc, blockVerticalRazer.left, blockVerticalRazer.top, 16, 16, hmemDc, 0, 16 * 7, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, blockHorizenRazer.left, blockHorizenRazer.top, 16, 16, hmemDc, 16, 16 * 7, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, switchOn.left, switchOn.top, 16, 16, hmemDc, 0, 32, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, btnOn.left, btnOn.top, 16, 16, hmemDc, 0, 48, 16, 16, RGB(255, 0, 255));

	TransparentBlt(hdc, gate1.left, gate1.top, 16, 16, hmemDc, 0, 16 * 4, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, gate2.left, gate2.top, 16, 16, hmemDc, 16, 16 * 4, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, gate3.left, gate3.top, 16, 16, hmemDc, 32, 16 * 4, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, gate4.left, gate4.top, 16, 16, hmemDc, 48, 16 * 4, 16, 16, RGB(255, 0, 255));

	TransparentBlt(hdc, basicCannon1.left, basicCannon1.top, 16, 16, hmemDc, 0, 16 * 5, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, basicCannon2.left, basicCannon2.top, 16, 16, hmemDc, 16, 16 * 5, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, basicCannon3.left, basicCannon3.top, 16, 16, hmemDc, 32, 16 * 5, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, basicCannon4.left, basicCannon4.top, 16, 16, hmemDc, 48, 16 * 5, 16, 16, RGB(255, 0, 255));

	TransparentBlt(hdc, homingCannon1.left, homingCannon1.top, 16, 16, hmemDc, 0, 16 * 6, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, homingCannon2.left, homingCannon2.top, 16, 16, hmemDc, 16, 16 * 6, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, homingCannon3.left, homingCannon3.top, 16, 16, hmemDc, 32, 16 * 6, 16, 16, RGB(255, 0, 255));
	TransparentBlt(hdc, homingCannon4.left, homingCannon4.top, 16, 16, hmemDc, 48, 16 * 6, 16, 16, RGB(255, 0, 255));
	// << btn Image

	SelectObject(hmemDc, hOldBitmap);
	DeleteDC(hmemDc);
}

void Map::DrawBtn(HDC hdc, RECT rect)
{
	Rectangle(hdc, rect.left - 5, rect.top - 5, rect.right + 5, rect.bottom + 5);
}

void Map::AddTile(POINT pos)
{
	SetNowType(pos);

	RECT tempRect = { -1,-1,-1,-1 };

	for (int i = 0; i < eTrueWinWidth; i += 16)
	{
		if (i <= pos.x && i + 16 >= pos.x)
		{
			tempRect.left = i;
			tempRect.right = i + 16;
			break;
		}
	}

	for (int i = 0; i < eTrueWinHeight; i += 16)
	{
		if (i <= pos.y && i + 16 >= pos.y)
		{
			tempRect.top = i;
			tempRect.bottom = i + 16;
			break;
		}
	}

	//if (nowType != -1 && tempRect.left != -1 && tempRect.top != -1 && tempRect.right&&tempRect.bottom)
	{
		TileMap temp;
		temp.pos = tempRect;
		temp.type = nowType;

		tileMap.push_back(temp);
	}
}

void Map::SetNowType(POINT pos)
{
	if (PtInRect(&basicBlock, pos))
		nowType = eMapBlock;
	else if (PtInRect(&halfBlock, pos))
		nowType = eMapHalfBlock;
	else if (PtInRect(&spike, pos))
		nowType = eMapSpike;
	else if (PtInRect(&item, pos))
		nowType = eMapItem;

	else if (PtInRect(&blockVerticalRazer, pos))
		nowType = eMapGateCloseVertical;
	else if (PtInRect(&blockHorizenRazer, pos))
		nowType = eMapGateCloseHorizen;
	else if (PtInRect(&switchOn, pos))
		nowType = eMapBtn_0;
	else if (PtInRect(&btnOn, pos))
		nowType = eMapBtn_2;

	else if (PtInRect(&gate1, pos))
		nowType = eMapGate_0;
	else if (PtInRect(&gate2, pos))
		nowType = eMapGate_1;
	else if (PtInRect(&gate3, pos))
		nowType = eMapGate_2;
	else if (PtInRect(&gate4, pos))
		nowType = eMapGate_3;

	else if (PtInRect(&basicCannon1, pos))
		nowType = eMapCannon_0;
	else if (PtInRect(&basicCannon2, pos))
		nowType = eMapCannon_1;
	else if (PtInRect(&basicCannon3, pos))
		nowType = eMapCannon_2;
	else if (PtInRect(&basicCannon4, pos))
		nowType = eMapCannon_3;

	else if (PtInRect(&homingCannon1, pos))
		nowType = eMapCannon_4;
	else if (PtInRect(&homingCannon2, pos))
		nowType = eMapCannon_5;
	else if (PtInRect(&homingCannon3, pos))
		nowType = eMapCannon_6;
	else if (PtInRect(&homingCannon4, pos))
		nowType = eMapCannon_7;

	else if (PtInRect(&playerResenBtn, pos))
		nowType = ePlayerResen;

	else if (PtInRect(&openFileBtn, pos))
		int a; // todo : fileOpen

	else if (PtInRect(&saveFileBtn, pos))
		int a;// todo : saveOpen

	else
		nowType = -1;

}
