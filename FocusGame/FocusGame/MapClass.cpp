#include "stdafx.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "GameManager.h"

#define dGameManager GameManager::GetInstance()
#define dPlayer Player::GetInstance()

using namespace std;

Map::Map()
{
	// todo : 플레이어 기록에 따른 맵 파싱 필요
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
	
	//tileMap.type = eMapBlock;
	//tileMap.pos = { 500, 484, 516, 500 };
	//mapPos.push_back(tileMap);
	//
	//tileMap.pos = { 500, 500, 516, 516 };
	//mapPos.push_back(tileMap);

	tileMap.type = eMapObstacle;
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

	resetPos = mapPos;
	// >> resetValue
}

Map::~Map()
{

}

Map* Map::GetInstance()
{
	static Map map;
	return &map;
}

void Map::Update()
{
	// >> 가시 충돌 판정
	RECT area;
	RECT playerPos = dPlayer->GetPlayerPos();

	for (int i = 0; i < mapPos.size(); i++)
	{
		if (mapPos[i].type == eMapObstacle && IntersectRect(&area, &mapPos[i].pos, &playerPos))
			dGameManager->SetIsPlayerLive(false);
	}
	// >> 가시 충돌 판정
}

void Map::DrawObject(HDC hdc)
{
	for(int i=0;i<mapPos.size();i++)
		Rectangle(hdc, mapPos[i].pos.left, mapPos[i].pos.top, mapPos[i].pos.right, mapPos[i].pos.bottom);
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
