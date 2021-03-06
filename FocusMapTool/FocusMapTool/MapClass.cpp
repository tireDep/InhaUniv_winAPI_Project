#include "stdafx.h"
#include "MapClass.h"

#include <fstream>
#include <string>
#include <cstring>
#include <commdlg.h>

#pragma comment (lib, "msimg32.lib")

#define dKeyCode 'k'

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

	nowTypeRect = { 820, 12, 966, 32 };

	resetBtn = { 840, 560, 946, 580};

	nowType = -1;

	hMapBitmap = (HBITMAP)LoadImage((NULL), TEXT("Image/tile.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hMapBitmap, sizeof(BITMAP), &mapBitmap);
}

Map::~Map()
{
	DeleteObject(hMapBitmap);
}

Map * Map::GetInstance()
{
	static Map map;
	return &map;
}

void Map::DrawMap(HDC hdc)
{
	HBRUSH hBrush, oldBrush;

	// hBrush = CreateSolidBrush(RGB(36, 36, 36));
	// oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	// 
	// PatBlt(hdc, 0, 0, eWindowWidth, eWindowWidth, WHITENESS);
	// 
	// SelectObject(hdc, oldBrush);
	// DeleteObject(hBrush);

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

	DrawBtn(hdc, nowTypeRect);
	SetShowType(nowType);
	DrawText(hdc, typeName, _tcslen(typeName), &nowTypeRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	DrawBtn(hdc,resetBtn);
	DrawText(hdc, TEXT("Reset"), _tcslen(TEXT("Reset")), &resetBtn, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	// for (int i = 0; i < tileMap.size(); i++)
	// {
	// 	Rectangle(hdc, tileMap[i].pos.left, tileMap[i].pos.top, tileMap[i].pos.right, tileMap[i].pos.bottom);
	// }

	// >> 중심 표시선
	HPEN hPen, oldPen;
	hPen = CreatePen(BS_SOLID, 1, RGB(110, 160, 207));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	MoveToEx(hdc, 0, eTrueWinHeight * 0.5, NULL);
	LineTo(hdc, eTrueWinWidth, eTrueWinHeight * 0.5);

	MoveToEx(hdc, eTrueWinWidth*0.5, 0, NULL);
	LineTo(hdc, eTrueWinWidth * 0.5, eTrueWinHeight);

	MoveToEx(hdc, 0, 0, NULL);
	LineTo(hdc, eTrueWinWidth, eTrueWinHeight);

	MoveToEx(hdc, eTrueWinWidth, 0, NULL);
	LineTo(hdc, 0, eTrueWinHeight);

	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	// << 중심 표시선
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

	for (int i = 0; i < tileMap.size(); i++)
		TransparentBlt(hdc, tileMap[i].pos.left, tileMap[i].pos.top, 16, 16, hmemDc, tileMap[i].showPos.x, tileMap[i].showPos.y, 16, 16, RGB(255, 0, 255));

	SelectObject(hmemDc, hOldBitmap);
	DeleteObject(hOldBitmap);
	DeleteDC(hmemDc);
}

void Map::DrawBtn(HDC hdc, RECT rect)
{
	Rectangle(hdc, rect.left - 5, rect.top - 5, rect.right + 5, rect.bottom + 5);
}

void Map::AddTile(POINT pos)
{
	RECT tempRect = CalcTileRange(pos);

	if (nowType == ePlayerResen)
	{
		vector<TileMap>::iterator it;
		for (it = tileMap.begin(); it < tileMap.end();)
		{
			if (it->type == ePlayerResen)
				it = tileMap.erase(it);
			else
				it++;
		}
	}

	if (nowType != -1 && tempRect.left != -1 && tempRect.top != -1 && tempRect.right&&tempRect.bottom)
		PushBack(tempRect);
}

void Map::AddTile(POINT startPos, POINT lastPos)
{
	if (nowType == ePlayerResen)
	{
		AddTile(startPos);
		return;
	}

	RECT tempRect1 = CalcTileRange(startPos);
	RECT tempRect2 = CalcTileRange(lastPos);

	if (nowType != -1 && tempRect1.left != -1 && tempRect1.top != -1 && tempRect1.right != -1 && tempRect1.bottom != -1
		&& tempRect2.left != -1 && tempRect2.top != -1 && tempRect2.right != -1 && tempRect2.bottom != -1)
	{
		TileMap temp;

		int horizontalMove = tempRect2.left - tempRect1.left;
		int verticalMove = tempRect2.top - tempRect1.top;

		int mulHorizontalNum = 1;
		int mulVerticalNum = 1;
		CalcMulNum(horizontalMove, verticalMove, mulHorizontalNum, mulVerticalNum);

		PushBack(tempRect1);
		PushBack(tempRect2);

		RECT addRect = tempRect1;
		while (1)
		{
			addRect.left += 16 * mulHorizontalNum;
			addRect.top += 16 * mulVerticalNum;
			addRect.right += 16 * mulHorizontalNum;
			addRect.bottom += 16 * mulVerticalNum;

			if (addRect.left == tempRect2.left && addRect.top == tempRect2.top&&addRect.right == tempRect2.right&&addRect.bottom == tempRect2.bottom)
				break;

			PushBack(addRect);

			horizontalMove = tempRect2.left - addRect.left;
			verticalMove = tempRect2.top - addRect.top;
			CalcMulNum(horizontalMove, verticalMove, mulHorizontalNum, mulVerticalNum);
		}
	}

}

void Map::PushBack(RECT addRect)
{
	TileMap temp;

	temp.pos = addRect;
	temp.type = nowType;
	temp.showPos = SetShowType(temp.type);

	CheckIsDoublePos(temp.pos);
	tileMap.push_back(temp);
}

void Map::DeleteTile(POINT pos)
{
	RECT tempRect = CalcTileRange(pos);

	for (int i = 0; i < tileMap.size(); i++)
	{
		if (tileMap[i].pos.left == tempRect.left 
			&& tileMap[i].pos.top == tempRect.top 
			&& tileMap[i].pos.right == tempRect.right 
			&& tileMap[i].pos.bottom == tempRect.bottom)

			tileMap.erase(tileMap.begin() + i);
	}
}

void Map::DeleteTile(POINT startPos, POINT lastPos)
{
	RECT tempRect1 = CalcTileRange(startPos);
	RECT tempRect2 = CalcTileRange(lastPos);

	if (nowType != -1 && tempRect1.left != -1 && tempRect1.top != -1 && tempRect1.right != -1 && tempRect1.bottom != -1
		&& tempRect2.left != -1 && tempRect2.top != -1 && tempRect2.right != -1 && tempRect2.bottom != -1)
	{
		int horizontalMove = tempRect2.left - tempRect1.left;
		int verticalMove = tempRect2.top - tempRect1.top;

		int mulHorizontalNum = 1;
		int mulVerticalNum = 1;
		CalcMulNum(horizontalMove, verticalMove, mulHorizontalNum, mulVerticalNum);

		int index = CheckIsSameRect(tempRect1);
		if (index != -1)
			tileMap.erase(tileMap.begin() + index);
		else
			return;

		RECT addRect = tempRect1;
		while (1)
		{
			addRect.left += 16 * mulHorizontalNum;
			addRect.top += 16 * mulVerticalNum;
			addRect.right += 16 * mulHorizontalNum;
			addRect.bottom += 16 * mulVerticalNum;

			index = CheckIsSameRect(addRect);

			if (index == -1 || (tempRect2.left == addRect.left && tempRect2.top == addRect.top && tempRect2.right == addRect.right && tempRect2.bottom == addRect.bottom))
				break;

			tileMap.erase(tileMap.begin() + index);

			horizontalMove = tempRect2.left - addRect.left;
			verticalMove = tempRect2.top - addRect.top;
			CalcMulNum(horizontalMove, verticalMove, mulHorizontalNum, mulVerticalNum);
		}

	}
}

RECT Map::CalcBtn(RECT rect)
{
	RECT result;
	result.left = rect.left - 5;
	result.top = rect.top - 5;
	result.right = rect.right + 5;
	result.bottom = rect.bottom + 5;

	return result;
}

RECT Map::CalcTileRange(POINT pos)
{
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

	return tempRect;
}

void Map::CalcMulNum(int moveX, int moveY, int &mulX, int &mulY)
{
	if (moveX > 0)
		mulX = 1;
	else if (moveX < 0)
		mulX = -1;
	else
		mulX = 0;

	if (moveY > 0)
		mulY = 1;
	else if (moveY < 0)
		mulY = -1;
	else
		mulY = 0;
}

int Map::CheckIsSameRect(RECT rect)
{
	for (int i = 0; i < tileMap.size(); i++)
	{
		if (tileMap[i].pos.left == rect.left && tileMap[i].pos.top == rect.top 
			&& tileMap[i].pos.right == rect.right && tileMap[i].pos.bottom == rect.bottom)
			return i;
	}
	return -1;
}

void Map::CheckIsDoublePos(RECT rect)
{
	for (int i = 0; i < tileMap.size(); i++)
	{
		if (tileMap[i].pos.left == rect.left && tileMap[i].pos.top == rect.top
			&& tileMap[i].pos.right == rect.right && tileMap[i].pos.bottom == rect.bottom)
			tileMap.erase(tileMap.begin() + i);
	}
}

void Map::SetNowType(HWND hWnd, POINT pos)
{
	if (PtInRect(&CalcBtn(basicBlock), pos))
		nowType = eMapBlock;
	else if (PtInRect(&CalcBtn(halfBlock), pos))
		nowType = eMapHalfBlock;
	else if (PtInRect(&CalcBtn(spike), pos))
		nowType = eMapSpike;
	else if (PtInRect(&CalcBtn(item), pos))
		nowType = eMapItem;

	else if (PtInRect(&CalcBtn(blockVerticalRazer), pos))
		nowType = eMapGateCloseVertical;
	else if (PtInRect(&CalcBtn(blockHorizenRazer), pos))
		nowType = eMapGateCloseHorizen;
	else if (PtInRect(&CalcBtn(switchOn), pos))
		nowType = eMapBtn_0;
	else if (PtInRect(&CalcBtn(btnOn), pos))
		nowType = eMapBtn_2;

	else if (PtInRect(&CalcBtn(gate1), pos))
		nowType = eMapGate_0;
	else if (PtInRect(&CalcBtn(gate2), pos))
		nowType = eMapGate_1;
	else if (PtInRect(&CalcBtn(gate3), pos))
		nowType = eMapGate_2;
	else if (PtInRect(&CalcBtn(gate4), pos))
		nowType = eMapGate_3;

	else if (PtInRect(&CalcBtn(basicCannon1), pos))
		nowType = eMapCannon_0;
	else if (PtInRect(&CalcBtn(basicCannon2), pos))
		nowType = eMapCannon_1;
	else if (PtInRect(&CalcBtn(basicCannon3), pos))
		nowType = eMapCannon_2;
	else if (PtInRect(&CalcBtn(basicCannon4), pos))
		nowType = eMapCannon_3;

	else if (PtInRect(&CalcBtn(homingCannon1), pos))
		nowType = eMapCannon_4;
	else if (PtInRect(&CalcBtn(homingCannon2), pos))
		nowType = eMapCannon_5;
	else if (PtInRect(&CalcBtn(homingCannon3), pos))
		nowType = eMapCannon_6;
	else if (PtInRect(&CalcBtn(homingCannon4), pos))
		nowType = eMapCannon_7;

	else if (PtInRect(&CalcBtn(playerResenBtn), pos))
		nowType = ePlayerResen;

	else if (PtInRect(&CalcBtn(openFileBtn), pos))
		FileOpen(hWnd);

	else if (PtInRect(&CalcBtn(saveFileBtn), pos))
		FileSave(hWnd);

	else if (PtInRect(&CalcBtn(resetBtn), pos))
		Reset();

	else
		nowType = -1;

}

POINT Map::SetShowType(int type)
{
	if (type == eMapBlock)
	{
		_tcscpy(typeName, L"type : Block");
		return { 0,0 };
	}
	else if (type == eMapHalfBlock)
	{
		_tcscpy(typeName, L"type : HalfBlock");
		return { 16,0 };
	}
	else if (type == eMapSpike)
	{
		_tcscpy(typeName, L"type : Spike");
		return { 0,16 };
	}
	else if (type == eMapItem)
	{
		_tcscpy(typeName, L"type : Item");
		return { 0, 16 * 8 };
	}

	else if (type == eMapGateCloseVertical)
	{
		_tcscpy(typeName, L"type : Vert_CloseDoor");
		return { 0, 16 * 7 };
	}
	else if (type == eMapGateCloseHorizen)
	{
		_tcscpy(typeName, L"type : Hori_CloseDoor");
		return { 16, 16 * 7 };
	}
	else if (type == eMapBtn_0)
	{
		_tcscpy(typeName, L"type : SwitchOn");
		return { 0, 16 * 2 };
	}
	else if (type == eMapBtn_2)
	{
		_tcscpy(typeName, L"type : BtnOn");
		return { 0, 16 * 3 };
	}

	else if (type == eMapGate_0)
	{
		_tcscpy(typeName, L"type : Gate_1");
		return { 0, 16 * 4 };
	}
	else if (type == eMapGate_1)
	{
		_tcscpy(typeName, L"type : Gate_2");
		return { 16, 16 * 4 };
	}
	else if (type == eMapGate_2)
	{
		_tcscpy(typeName, L"type : Gate_3");
		return { 32, 16 * 4 };
	}
	else if (type == eMapGate_3)
	{
		_tcscpy(typeName, L"type : Gate_4");
		return { 48, 16 * 4 };
	}

	else if (type == eMapCannon_0)
	{
		_tcscpy(typeName, L"type : Basic_C_1");
		return { 0, 16 * 5 };
	}
	else if (type == eMapCannon_1)
	{
		_tcscpy(typeName, L"type : Basic_C_2");
		return{ 16, 16 * 5 };
	}
	else if (type == eMapCannon_2)
	{
		_tcscpy(typeName, L"type : Basic_C_3");
		return{ 32, 16 * 5 };
	}
	else if (type == eMapCannon_3)
	{
		_tcscpy(typeName, L"type : Basic_C_4");
		return{ 48, 16 * 5 };
	}

	else if (type == eMapCannon_4)
	{
		_tcscpy(typeName, L"type : Homing_C_1");
		return{ 0, 16 * 6 };
	}
	else if (type == eMapCannon_5)
	{
		_tcscpy(typeName, L"type : Homing_C_2");
		return{ 16, 16 * 6 };
	}
	else if (type == eMapCannon_6)
	{
		_tcscpy(typeName, L"type : Homing_C_3");
		return{ 32, 16 * 6 };
	}
	else if (type == eMapCannon_7)
	{
		_tcscpy(typeName, L"type : Homing_C_4");
		return{ 48, 16 * 6 };
	}

	else if (type == ePlayerResen)
	{
		_tcscpy(typeName, L"type : PlayerResen");
		return{ 16, 16 * 8 };
	}

	else _tcscpy(typeName, L"type : ");
}

void Map::FileOpen(HWND hWnd)
{
	OPENFILENAME ofn;
	TCHAR strBuffer[128];	// 현재 위치 경로 버퍼
	TCHAR fileName[128] = L"\0";	// 경로 및 파일 이름 버퍼
	TCHAR sFilter[] = L"모든 파일\0*.*\0맵 파일\0*.dat\0텍스트 파일\0*.txt\0";

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = sFilter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = 128;
	ofn.lpstrInitialDir = _tgetcwd(strBuffer, 128);	// 현재 경로 불러오기
	if (GetOpenFileName(&ofn) != 0)
	{
		wsprintf(fileName, L"%s", ofn.lpstrFile);
		MessageBox(hWnd, fileName, L"불러오기 선택", MB_OKCANCEL);

		ReadData(hWnd, fileName);
	}
	else
	{
		MessageBox(hWnd, L"불러오기를 취소하였습니다.", L"불러오기 취소", MB_OKCANCEL);
	}
}

void Map::ReadData(HWND hWnd, TCHAR *OpenFileName)
{
	string readDataFile;
	ifstream mapFile;

	std::wstring wStr = OpenFileName;
	readDataFile = string(wStr.begin(), wStr.end());
	// >> TCHAR -> string

	char *changeFile;
	changeFile = strtok(&readDataFile[0], "\\");

	while (1)
	{
		changeFile= strtok(NULL, "\\");
		if (changeFile == NULL)
		{
			MessageBox(hWnd, L"해당 파일을 지원하지 않습니다.", L"파일 열기 실패", MB_OKCANCEL);
			return;
		}
		if (strstr(changeFile, ".dat"))
			break;
	}

	mapFile.open(changeFile, ios::in | ios::binary);
	if (mapFile.is_open())
	{
		tileMap.clear();	
		// >> 새로 읽어오기 전 초기화

		POINT pos = { 0,0 };
		TileMap temp;

		temp.pos = { 0,0,0,0 };
		temp.type = 0;

		// >> 플레이어
		mapFile.read((char*)&pos.x , sizeof(int));
		mapFile.read((char*)&pos.y, sizeof(int));

		pos.x = (pos.x / dKeyCode) - dKeyCode;
		pos.y = (pos.y / dKeyCode) - dKeyCode;

		temp.pos.left = pos.x - 8;
		temp.pos.top = pos.y - 8;
		temp.pos.right = pos.x + 8;
		temp.pos.bottom = pos.y + 8;
		temp.type = ePlayerResen;
		temp.showPos = SetShowType(temp.type);

		tileMap.push_back(temp);

		// >> 맵 정보
		while (!mapFile.eof())
		{
			mapFile.read((char*)&temp.type, sizeof(int));
			mapFile.read((char*)&temp.pos.left, sizeof(int));
			mapFile.read((char*)&temp.pos.top, sizeof(int));
			mapFile.read((char*)&temp.pos.right, sizeof(int));
			mapFile.read((char*)&temp.pos.bottom, sizeof(int));

			temp.type = (temp.type / dKeyCode) - dKeyCode;
			temp.pos.left = (temp.pos.left / dKeyCode) - dKeyCode;
			temp.pos.top = (temp.pos.top / dKeyCode) - dKeyCode;
			temp.pos.right = (temp.pos.right / dKeyCode) - dKeyCode;
			temp.pos.bottom = (temp.pos.bottom / dKeyCode) - dKeyCode;
			
			if (temp.type < 0)
				break;	// >> 무한루프 처리

			temp.showPos = SetShowType(temp.type);
			tileMap.push_back(temp);
		}
	}

	mapFile.close();
}

void Map::FileSave(HWND hWnd)
{
	OPENFILENAME ofn;
	TCHAR strBuffer[128];	// 현재 위치 경로 버퍼
	TCHAR fileName[128] = L"\0";	// 경로 및 파일 이름 버퍼
	TCHAR sFilter[] = L"모든 파일\0*.*\0맵 파일\0*.dat\0텍스트 파일\0*.txt\0";

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = sFilter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = 128;
	ofn.lpstrInitialDir = _tgetcwd(strBuffer, 128);	// 현재 경로 불러오기
	if (GetSaveFileName(&ofn) != 0)
	{
		wsprintf(fileName, L"%s", ofn.lpstrFile);
		MessageBox(hWnd, fileName, L"파일 저장", MB_OKCANCEL);
		
		SaveData(hWnd, fileName);
	}
	else
	{
		MessageBox(hWnd, L"파일 저장을 취소하였습니다.", L"파일 저장 취소", MB_OKCANCEL);
	}
}

void Map::SaveData(HWND hWnd, TCHAR *SaveFileName)
{
	string writeDataFile;
	ofstream mapFile;

	std::wstring wStr = SaveFileName;
	writeDataFile = string(wStr.begin(), wStr.end());
	// >> TCHAR -> string

	char *changeFile;
	changeFile = strtok(&writeDataFile[0], "\\");

	while (1)
	{
		changeFile = strtok(NULL, "\\");
		if (changeFile == NULL)
		{
			MessageBox(hWnd, L"해당 파일을 지원하지 않습니다.", L"파일 저장 실패", MB_OKCANCEL);
			return;
		}
		if (strstr(changeFile, ".dat"))
			break;
	}

	POINT playerPos;
	bool isPlayerIn = false;
	for (int i = 0; i < tileMap.size(); i++)
	{
		if (tileMap[i].type == ePlayerResen)
		{
			isPlayerIn = true;

			playerPos.x = (tileMap[i].pos.left + tileMap[i].pos.right) * 0.5;
			playerPos.y = (tileMap[i].pos.top + tileMap[i].pos.bottom) * 0.5;
		}
	}

	if (!isPlayerIn)
	{
		MessageBox(hWnd, L"플레이어 위치가 존재하지 않습니다.", L"파일 저장 실패", MB_OKCANCEL);
		return;
	}

	mapFile.open(changeFile);

	// mapFile << playerPos.x << "\t" << playerPos.y << endl;
	playerPos.x = (playerPos.x + dKeyCode) * dKeyCode;
	playerPos.y = (playerPos.y + dKeyCode) * dKeyCode;

	mapFile.write((char*)&playerPos.x, sizeof(int));
	mapFile.write((char*)&playerPos.y, sizeof(int));

	TileMap tempMap;
	for (int i = 0; i < tileMap.size(); i++)
	{
		if (tileMap[i].type != ePlayerResen)
		{
			tempMap.type = (tileMap[i].type + dKeyCode) * dKeyCode;
			tempMap.pos.left = (tileMap[i].pos.left + dKeyCode) * dKeyCode;
			tempMap.pos.top = (tileMap[i].pos.top + dKeyCode) * dKeyCode;
			tempMap.pos.right = (tileMap[i].pos.right + dKeyCode) * dKeyCode;
			tempMap.pos.bottom = (tileMap[i].pos.bottom + dKeyCode) * dKeyCode;

			mapFile.write((char*)&tempMap.type, sizeof(int));
			mapFile.write((char*)&tempMap.pos.left, sizeof(int));
			mapFile.write((char*)&tempMap.pos.top, sizeof(int));
			mapFile.write((char*)&tempMap.pos.right, sizeof(int));
			mapFile.write((char*)&tempMap.pos.bottom, sizeof(int));
		}
			// mapFile << tileMap[i].type << "\t" << tileMap[i].pos.left << "\t" << tileMap[i].pos.top << "\t" << tileMap[i].pos.right << "\t" << tileMap[i].pos.bottom << endl;
	}

	mapFile.close();
}

void Map::Reset()
{
	tileMap.clear();
}

