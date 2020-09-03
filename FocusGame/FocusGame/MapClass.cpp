#include "stdafx.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "GameManager.h"

#define dGameManager GameManager::GetInstance()
#define dPlayer Player::GetInstance()

using namespace std;

Map::Map()
{
	hMapBitmap = (HBITMAP)LoadImage(NULL, TEXT("../Image/tile.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hMapBitmap, sizeof(BITMAP), &mapBitmap);

	// todo : �÷��̾� ��Ͽ� ���� �� �Ľ� �ʿ�
	TileMap tileMap;

	for (int i = 0; i < 816; i += 16)
	{
		tileMap.pos = { i, 0, 16 + i, 16 };
		tileMap.type = eMapBlock;
		mapPos.push_back(tileMap);

		tileMap.pos = { i, 576, 16 + i, 576 + 16 };
		tileMap.type = eMapBlock;
		mapPos.push_back(tileMap);


		tileMap.pos = { i, 304, 16 + i, 304 + 16 };
		tileMap.type = eMapBlock;
		mapPos.push_back(tileMap);

	}
	
	for (int i = 0; i < 592; i += 16)
	{
		tileMap.pos = { 0, i, 16, 16 + i };
		tileMap.type = eMapBlock;
		mapPos.push_back(tileMap);

		tileMap.pos = { 784, i, 784 + 16, 16 + i };
		tileMap.type = eMapBlock;
		mapPos.push_back(tileMap);
	}
	
	tileMap.type = eMapBlock;
	tileMap.pos = { 500, 484, 516, 500 };
	mapPos.push_back(tileMap);
	
	tileMap.pos = { 500, 500, 516, 516 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapSpike;
	tileMap.pos = { 500, 548, 516, 564 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapBlock;
	tileMap.pos = { 300, 500, 316, 516 };
	mapPos.push_back(tileMap);

	tileMap.pos = { 300, 516, 316, 532 };
	mapPos.push_back(tileMap);

	tileMap.pos = { 300, 532, 316, 548 };
	mapPos.push_back(tileMap);

	tileMap.pos = { 300, 548, 316, 564 };
	mapPos.push_back(tileMap);

	// >> cannon test
	tileMap.type = eMapCannon_0;
	tileMap.pos = { 112, 112, 128, 128};
	mapPos.push_back(tileMap);
	
	tileMap.type = eMapCannon_1;
	tileMap.pos = { 128, 112, 144, 128 };
	mapPos.push_back(tileMap);
	
	tileMap.type = eMapCannon_2;
	tileMap.pos = { 128, 128, 144, 144 };
	mapPos.push_back(tileMap);
	
	tileMap.type = eMapCannon_3;
	tileMap.pos = { 112, 128, 128, 144 };
	mapPos.push_back(tileMap);


	tileMap.type = eMapCannon_4;
	tileMap.pos = { 704, 112, 720, 128 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapCannon_5;
	tileMap.pos = { 720, 112, 736, 128 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapCannon_6;
	tileMap.pos = { 720, 128, 736, 144 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapCannon_7;
	tileMap.pos = { 704, 128, 720, 144 };
	mapPos.push_back(tileMap);
	// >> cannon test

	// >> gate test
	tileMap.type = eMapGate_0;
	tileMap.pos = { 704, 528, 720, 544 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapGate_1;
	tileMap.pos = { 720, 528, 736, 544 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapGate_2;
	tileMap.pos = { 720, 544, 736, 560 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapGate_3;
	tileMap.pos = { 704, 544, 720, 560 };
	mapPos.push_back(tileMap);
	// >> gate test

	tileMap.type = eMapBtn_0;
	tileMap.pos = { 640, 544, 656, 560 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapBtn_2;
	tileMap.pos = { 640, 560, 656, 576 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapBtn_2;
	tileMap.pos = { 224, 288, 240, 304 };
	mapPos.push_back(tileMap);

	// >> btn

	resetPos = mapPos;
	// >> resetValue
}

Map::~Map()
{
	DeleteObject(hMapBitmap);
}

Map* Map::GetInstance()
{
	static Map map;
	return &map;
}

void Map::Update()
{
	if (!dPlayer->GetIsPlayerDead())
	{
		// >> ���� �浹 ����
		RECT area;
		RECT playerPos = dPlayer->GetPlayerPos();

		for (int i = 0; i < mapPos.size(); i++)
		{
			if (mapPos[i].type == eMapSpike && IntersectRect(&area, &mapPos[i].pos, &playerPos))
			{
				dPlayer->SetIsPlayerDead(true);
				break;
			}
		}
		// >> ���� �浹 ����

		// >> ����ġ off ����
		for (int i = 0; i < mapPos.size(); i++)
		{
			if (mapPos[i].type == eMapBtn_0 && IntersectRect(&area, &mapPos[i].pos, &playerPos))
			{
				mapPos[i].type = eMapBtn_1;
				mapPos[i + 1].type = eMapBtn_3;	// ���ÿ� �ٲ��� ��
			}	// ����ġ ��ư�� ���

		}

		if (CheckOffBtn())
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if (mapPos[i].type == eMapGate_0 || mapPos[i].type == eMapGate_1 || mapPos[i].type == eMapGate_2 || mapPos[i].type == eMapGate_3)
					mapPos[i].type = eMapGateOpen;
				// todo : �ִϸ��̼� �߰�?
			}
		}
		// >> ����ġ off ����
	}
}

bool Map::CheckOffBtn()
{
	for (int i = 0; i < mapPos.size(); i++)
	{
		if (mapPos[i].type == eMapBtn_2)
			return false;
	}
	return true;
}

void Map::CheckShotOffBtn(const RECT &hitPos)
{
	RECT area;
	for (int i = 0; i < mapPos.size(); i++)
	{
		if (IntersectRect(&area, &mapPos[i].pos, &hitPos) && mapPos[i].type == eMapBtn_2)
			mapPos[i].type = eMapBtn_3;
	}
}

void Map::DrawObject(HDC hdc)
{
	if (dGameManager->GetDrawRect())
	{
		for (int i = 0; i<mapPos.size(); i++)
			Rectangle(hdc, mapPos[i].pos.left, mapPos[i].pos.top, mapPos[i].pos.right, mapPos[i].pos.bottom);
	}
}

void Map::RenderObject(HWND hWnd, HDC hdc)
{
	HDC hMapDc;
	HBITMAP hMapBit;
	POINT blength = { 16,16 };
	POINT pos = { 0,0 };

	hMapDc = CreateCompatibleDC(hdc);
	hMapBit = (HBITMAP)SelectObject(hMapDc, hMapBitmap);

	for (int i = 0; i < mapPos.size(); i++)
	{
		if (mapPos[i].type == eMapBlock)
			pos = { 0,0 };

		else if (mapPos[i].type == eMapSpike)
			pos = { 0,16 };

		else if (mapPos[i].type == eMapBtn_0)
			pos = { 0,32 };
		else if (mapPos[i].type == eMapBtn_1)
			pos = { 16,32 };

		else if (mapPos[i].type == eMapBtn_2)
			pos = { 0,48 };
		else if (mapPos[i].type == eMapBtn_3)
			pos = { 16,48 };

		else if (mapPos[i].type == eMapGate_0)
			pos = { 0,64 };
		else if (mapPos[i].type == eMapGate_1)
			pos = { 16,64 };
		else if (mapPos[i].type == eMapGate_2)
			pos = { 32,64 };
		else if (mapPos[i].type == eMapGate_3)
			pos = { 48,64 };

		else if (mapPos[i].type == eMapGateOpen)
			pos = { 64,64 };	// todo -> �ִϸ��̼�?

		else if (mapPos[i].type == eMapCannon_0)
			pos = { 0,80 };
		else if (mapPos[i].type == eMapCannon_1)
			pos = { 16,80 };
		else if (mapPos[i].type == eMapCannon_2)
			pos = { 32,80 };
		else if (mapPos[i].type == eMapCannon_3)
			pos = { 48,80 };

		else if (mapPos[i].type == eMapCannon_4)
			pos = { 0,96 };
		else if (mapPos[i].type == eMapCannon_5)
			pos = { 16,96 };
		else if (mapPos[i].type == eMapCannon_6)
			pos = { 32,96 };
		else if (mapPos[i].type == eMapCannon_7)
			pos = { 48,96 };

		TransparentBlt(hdc, mapPos[i].pos.left, mapPos[i].pos.top, blength.x, blength.y, hMapDc, pos.x, pos.y, blength.x, blength.y, RGB(255, 0, 255));
	}

	SelectObject(hMapDc, hMapBit);
	DeleteDC(hMapDc);
}

vector<TileMap> Map::GetMapPos()
{
	return mapPos;
}

vector<parceCannon> Map::CheckInCannon()
{
	parceCannon tempVal;
	vector<parceCannon> result;

	vector<TileMap>::iterator it;
	for (it = mapPos.begin(); it < mapPos.end(); it++)
	{
		if (it->type == eMapCannon_0)
		{
			tempVal.pos = { it->pos.right, it->pos.bottom };
			tempVal.type = dNormal;
			
			result.push_back(tempVal);
		}

		if (it->type == eMapCannon_4)
		{
			tempVal.pos = { it->pos.right, it->pos.bottom };
			tempVal.type = dHoming;

			result.push_back(tempVal);
		}
	}

	return result;
}

void Map::Reset()
{
	mapPos = resetPos;
}
