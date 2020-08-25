#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
//#include "GameManger.h"

using namespace std;

struct MapTile
{
	RECT pos;
	int type;
};

class Map : public Object
{
private:
	//GameManager *gameManger = GameManager::GetInstance();
	vector<MapTile> mapPos;

	Map();
	~Map();

public:
	static Map* GetInstance();
	void Update();
	void DrawObject(HDC hdc);
};