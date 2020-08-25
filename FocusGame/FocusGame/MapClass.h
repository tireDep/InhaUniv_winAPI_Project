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
	vector<MapTile> mapPos;

	Map();
	~Map();

public:
	static Map* GetInstance();
	void Update();
	void DrawObject(HDC hdc);
};