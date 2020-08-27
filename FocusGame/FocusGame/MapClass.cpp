#include "stdafx.h"
#include "MapClass.h"

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
	
	tileMap.type = eMapBlock;
	tileMap.pos = { 500, 484, 516, 500 };
	mapPos.push_back(tileMap);

	tileMap.pos = { 500, 500, 516, 516 };
	mapPos.push_back(tileMap);

	//tileMap.pos = { 500, 516, 516, 532 };
	//mapPos.push_back(tileMap);

	//tileMap.pos = { 500, 532, 516, 548 };
	//mapPos.push_back(tileMap);

	//tileMap.type = eMapObstacle;
	//tileMap.pos = { 500, 548, 516, 564 };
	//mapPos.push_back(tileMap);

	tileMap.type = eMapBlock;
	tileMap.pos = { 300, 500, 316, 516 };
	mapPos.push_back(tileMap);

	tileMap.pos = { 300, 516, 316, 532 };
	mapPos.push_back(tileMap);

	tileMap.pos = { 300, 532, 316, 548 };
	mapPos.push_back(tileMap);

	tileMap.pos = { 300, 548, 316, 564 };
	mapPos.push_back(tileMap);


	tileMap.type = eMapCannon_0;
	tileMap.pos = { 100, 100, 116, 116};
	mapPos.push_back(tileMap);

	tileMap.type = eMapCannon_1;
	tileMap.pos = { 116, 100, 132, 116 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapCannon_2;
	tileMap.pos = { 116, 116, 132, 132 };
	mapPos.push_back(tileMap);

	tileMap.type = eMapCannon_3;
	tileMap.pos = { 100, 116, 116, 132 };
	mapPos.push_back(tileMap);



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

}

void Map::DrawObject(HDC hdc)
{
	Rectangle(hdc, 116 - 16 * 5, 0, 116 + 16 * 5, eTrueWinHeight); //test

	for(int i=0;i<mapPos.size();i++)
		Rectangle(hdc, mapPos[i].pos.left, mapPos[i].pos.top, mapPos[i].pos.right, mapPos[i].pos.bottom);
}

vector<TileMap> Map::GetMapPos()
{
	return mapPos;
}
