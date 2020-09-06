#include "stdafx.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "GameManager.h"

#include <fstream>
#include <string>

#define dGameManager GameManager::GetInstance()
#define dPlayer Player::GetInstance()

using namespace std;

Map::Map()
{
	hMapBitmap = (HBITMAP)LoadImage(NULL, TEXT("../Image/tile.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hMapBitmap, sizeof(BITMAP), &mapBitmap);

	isNextStage = false;
	ReadMapData();
	// todo : 플레이어 기록에 따른 맵 파싱 필요
	// TileMap tileMap;

	// for (int i = 0; i < 816; i += 16)
	// {
	// 	tileMap.pos = { i, 0, 16 + i, 16 };
	// 	tileMap.type = eMapBlock;
	// 	mapPos.push_back(tileMap);
	// 
	// 	tileMap.pos = { i, 576, 16 + i, 576 + 16 };
	// 	tileMap.type = eMapBlock;
	// 	mapPos.push_back(tileMap);
	// 
	// 
	// 	tileMap.pos = { i, 304, 16 + i, 304 + 16 };
	// 	tileMap.type = eMapBlock;
	// 	mapPos.push_back(tileMap);
	// 
	// }
	// 
	// for (int i = 0; i < 592; i += 16)
	// {
	// 	tileMap.pos = { 0, i, 16, 16 + i };
	// 	tileMap.type = eMapBlock;
	// 	mapPos.push_back(tileMap);
	// 
	// 	tileMap.pos = { 784, i, 784 + 16, 16 + i };
	// 	tileMap.type = eMapBlock;
	// 	mapPos.push_back(tileMap);
	// }
	// 
	// tileMap.type = eMapBlock;
	// tileMap.pos = { 500, 484, 516, 500 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.pos = { 500, 500, 516, 516 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapSpike;
	// tileMap.pos = { 500, 548, 516, 564 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapBlock;
	// tileMap.pos = { 300, 500, 316, 516 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.pos = { 300, 516, 316, 532 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.pos = { 300, 532, 316, 548 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.pos = { 300, 548, 316, 564 };
	// mapPos.push_back(tileMap);
	// 
	// // >> cannon test
	// tileMap.type = eMapCannon_0;
	// tileMap.pos = { 112, 112, 128, 128};
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapCannon_1;
	// tileMap.pos = { 128, 112, 144, 128 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapCannon_2;
	// tileMap.pos = { 128, 128, 144, 144 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapCannon_3;
	// tileMap.pos = { 112, 128, 128, 144 };
	// mapPos.push_back(tileMap);
	// 
	// 
	// tileMap.type = eMapCannon_4;
	// tileMap.pos = { 704, 112, 720, 128 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapCannon_5;
	// tileMap.pos = { 720, 112, 736, 128 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapCannon_6;
	// tileMap.pos = { 720, 128, 736, 144 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapCannon_7;
	// tileMap.pos = { 704, 128, 720, 144 };
	// mapPos.push_back(tileMap);
	// // >> cannon test
	// 
	// // >> gate test
	// tileMap.type = eMapGate_0;
	// tileMap.pos = { 704, 528, 720, 544 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGate_1;
	// tileMap.pos = { 720, 528, 736, 544 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGate_2;
	// tileMap.pos = { 720, 544, 736, 560 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGate_3;
	// tileMap.pos = { 704, 544, 720, 560 };
	// mapPos.push_back(tileMap);
	// // >> gate test
	// 
	// tileMap.type = eMapBtn_0;
	// tileMap.pos = { 640, 544, 656, 560 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapBtn_2;
	// tileMap.pos = { 640, 560, 656, 576 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapBtn_2;
	// tileMap.pos = { 224, 288, 240, 304 };
	// mapPos.push_back(tileMap);
	// 
	// // >> btn
	// 
	// /*tileMap.type = eMapGateCloseVertical;
	// tileMap.pos = { 200,500,216,516 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGateCloseVertical;
	// tileMap.pos = { 200,516,216,532 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGateCloseVertical;
	// tileMap.pos = { 200,532,216,548 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGateCloseVertical;
	// tileMap.pos = { 200,548,216,564 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGateCloseVertical;
	// tileMap.pos = { 200,564,216,580 };
	// mapPos.push_back(tileMap); */
	// 
	// 
	// tileMap.type = eMapGateCloseHorizen;
	// tileMap.pos = { 224,560,240,576 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGateCloseHorizen;
	// tileMap.pos = { 240,560,256,576 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGateCloseHorizen;
	// tileMap.pos = { 256,560,272,576 };
	// mapPos.push_back(tileMap);
	// 
	// tileMap.type = eMapGateCloseHorizen;
	// tileMap.pos = { 272,560,288,576 };
	// mapPos.push_back(tileMap);
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
		RECT area;
		RECT playerPos = dPlayer->GetPlayerPos();

		for (int i = 0; i < mapPos.size(); i++)
		{
			// >> 가시 충돌 판정
			if (mapPos[i].type == eMapSpike && IntersectRect(&area, &mapPos[i].pos, &playerPos))
			{
				dPlayer->SetIsPlayerDead(true);
				break;
			}

			// >> 스위치 off 판정
			if (mapPos[i].type == eMapBtn_0 && IntersectRect(&area, &mapPos[i].pos, &playerPos))
			{
				mapPos[i].type = eMapBtn_1;
				for (int j = 0; j < mapPos.size(); j++)
				{
					if (mapPos[j].pos.top == mapPos[i].pos.bottom && mapPos[j].pos.left == mapPos[i].pos.left && mapPos[j].pos.right == mapPos[i].pos.right
						&& mapPos[j].type == eMapBtn_2)
						mapPos[j].type = eMapBtn_3;
				}
				// >> 바로 밑에 있는 버튼이 꺼지도록 판정 변경
			}

			// >> 다음 스테이지 불러오기
			if (mapPos[i].type == eMapGateOpen && IntersectRect(&area, &mapPos[i].pos, &playerPos))
				isNextStage = true;
		}

		if (CheckOffBtn())
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if (mapPos[i].type == eMapGate_0 || mapPos[i].type == eMapGate_1 || mapPos[i].type == eMapGate_2 || mapPos[i].type == eMapGate_3)
					mapPos[i].type = eMapGateOpen;
				// todo : 애니메이션 추가?
			}
		}
		// >> 스위치 off 판정
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
	bool isOff;
	for (int i = 0; i < mapPos.size(); i++)
	{
		isOff = true;
		if (IntersectRect(&area, &mapPos[i].pos, &hitPos) && mapPos[i].type == eMapBtn_2)
		{
			for (int j = 0; j < mapPos.size(); j++)
			{
				if (mapPos[i].pos.top == mapPos[j].pos.bottom && mapPos[i].pos.left == mapPos[j].pos.left && mapPos[i].pos.right == mapPos[j].pos.right)
					isOff = false;	// >> 위에 무언가가 있을 경우 꺼지면 x
			}
			
			if (isOff)
				mapPos[i].type = eMapBtn_3;
		}
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
			pos = { 64,64 };	// todo -> 애니메이션?

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

		else if (mapPos[i].type == eMapGateCloseVertical)
			pos = { 0,112 };
		else if (mapPos[i].type == eMapGateCloseHorizen)
			pos = { 16,112 };

		TransparentBlt(hdc, mapPos[i].pos.left, mapPos[i].pos.top, blength.x, blength.y, hMapDc, pos.x, pos.y, blength.x, blength.y, RGB(255, 0, 255));
	}

	SelectObject(hMapDc, hMapBit);
	DeleteDC(hMapDc);
}

void Map::SetNextStage()
{
	// todo : read Stage
	// todo : setPlayerResen

	dGameManager->SetNowStage(dGameManager->GetNowStage() + 1);
	mapPos.clear();
	resetPos.clear();
	
	ReadMapData();
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

void Map::ReadMapData()
{
	string fileName = "map_0";
	ifstream mapFile;

	fileName += to_string(dGameManager->GetNowStage()) + ".dat";
	fileName = "./Map/" + fileName;

	mapFile.open(fileName, ios::in | ios::binary);
	if (mapFile.is_open())
	{
		TileMap tileMap;
		tileMap.pos = { 0,0,0,0 };
		tileMap.type = 0;

		// >> 간단한 파싱
		mapFile >> resenSpot.x >> resenSpot.y; // 플레이어 리젠 위치 

		while (!mapFile.eof())
		{
			mapFile >> tileMap.type >> tileMap.pos.left >> tileMap.pos.top >> tileMap.pos.right >> tileMap.pos.bottom;
			mapPos.push_back(tileMap);
		}
		// >> 간단한 파싱

		// string temp;
		// mapFile.seekg(0, std::ios::end);
		// int size = mapFile.tellg();
		// 
		// temp.resize(size);
		// mapFile.seekg(0, std::ios::beg);
		// mapFile.read(&temp[0], size);


	}
	else
	{
		// >> 불러올 맵이 없다 => All Clear

		// todo : stageClear -> endScene -> MainScene
		// todo : 맨 처음 초기값으로 세팅해주어야 함

		// dGameManager->SetNowStage(0);
		// dGameManager->SetFocusLv(0);
		// 실제 값

		dGameManager->SetNowStage(0);
		dGameManager->SetFocusLv(250);
		dGameManager->SetNowScene(eResultScene);
		// 임시 값
		// todo : 수정 예정
	}

	mapFile.close();

	resetPos = mapPos;
	// >> resetValue
}

POINT Map::GetResenSpot()
{
	return resenSpot;
}

void Map::SetIsNextStage(bool set)
{
	isNextStage = set;
}

bool Map::GetIsNextStage()
{
	return isNextStage;
}
