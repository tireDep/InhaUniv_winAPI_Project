#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "FocusGame.h"
#include "GameManager.h"

using namespace std;

class Map : public Object
{
private:
	GameManager *gameManager = GameManager::GetInstance();
	
	vector<TileMap> mapPos;
	vector<TileMap> resetPos;

	HBITMAP hMapBitmap;
	BITMAP mapBitmap;

	Map();

public:
	~Map();
	static Map* GetInstance();
	void Update();

	bool CheckOffBtn();
	void CheckShotOffBtn(const RECT &hitPos);

	void DrawObject(HDC hdc);
	void RenderObject(HWND hWnd, HDC hdc);

	void SetNextStage();

	vector<TileMap> GetMapPos();
	vector<parceCannon> CheckInCannon();

	void Reset();

	void ReadMapData();
};