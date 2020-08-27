#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "FocusGame.h"
#include "GameManger.h"

using namespace std;

class Map : public Object
{
private:
	GameManager *gameManger = GameManager::GetInstance();
	vector<TileMap> mapPos;

	Map();

public:
	~Map();
	static Map* GetInstance();
	void Update();
	void DrawObject(HDC hdc);

	vector<TileMap> GetMapPos();
	vector<POINT> CheckInCannon();
};