#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "FocusGame.h"
#include "GameManager.h"

using namespace std;

class Map : public Object
{
private:
	GameManager *gameManger = GameManager::GetInstance();
	
	vector<TileMap> mapPos;
	vector<TileMap> resetPos;

	Map();

public:
	~Map();
	static Map* GetInstance();
	void Update();

	bool CheckOffBtn();
	void CheckShotOffBtn(const RECT &hitPos);

	void DrawObject(HDC hdc);

	vector<TileMap> GetMapPos();
	vector<parceCannon> CheckInCannon();

	void Reset();
};