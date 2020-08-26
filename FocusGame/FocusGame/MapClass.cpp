#include "stdafx.h"
#include "MapClass.h"

using namespace std;

Map::Map()
{
	// todo : 플레이어 기록에 따른 맵 파싱 필요
	MapTile mapTile;

	for (int i = 0; i < 816; i += 16)
	{
		mapTile.pos = { i, 0, 16 + i, 16 };
		mapTile.type = eMapBlock;
		mapPos.push_back(mapTile);

		mapTile.pos = { i, 576, 16 + i, 576 + 16 };
		mapTile.type = eMapBlock;
		mapPos.push_back(mapTile);
	}
	
	for (int i = 0; i < 592; i += 16)
	{
		mapTile.pos = { 0, i, 16, 16 + i };
		mapTile.type = eMapBlock;
		mapPos.push_back(mapTile);

		mapTile.pos = { 784, i, 784 + 16, 16 + i };
		mapTile.type = eMapBlock;
		mapPos.push_back(mapTile);
	}
	
	mapTile.type = eMapBlock;
	mapTile.pos = { 500, 484, 516, 500 };
	mapPos.push_back(mapTile);

	mapTile.pos = { 500, 500, 516, 516 };
	mapPos.push_back(mapTile);

	//mapTile.pos = { 500, 516, 516, 532 };
	//mapPos.push_back(mapTile);

	//mapTile.pos = { 500, 532, 516, 548 };
	//mapPos.push_back(mapTile);

	//mapTile.type = eMapObstacle;
	//mapTile.pos = { 500, 548, 516, 564 };
	//mapPos.push_back(mapTile);

	mapTile.type = eMapBlock;
	mapTile.pos = { 300, 500, 316, 516 };
	mapPos.push_back(mapTile);

	mapTile.pos = { 300, 516, 316, 532 };
	mapPos.push_back(mapTile);

	mapTile.pos = { 300, 532, 316, 548 };
	mapPos.push_back(mapTile);

	mapTile.pos = { 300, 548, 316, 564 };
	mapPos.push_back(mapTile);
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
	for(int i=0;i<mapPos.size();i++)
		Rectangle(hdc, mapPos[i].pos.left, mapPos[i].pos.top, mapPos[i].pos.right, mapPos[i].pos.bottom);
}

vector<MapTile> Map::GetMapPos()
{
	return mapPos;
}
