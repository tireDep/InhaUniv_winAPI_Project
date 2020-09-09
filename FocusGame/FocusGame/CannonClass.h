#pragma once
#include "stdafx.h"
#include "GameManager.h"
#include "ObstacleClass.h"

#include <time.h>

class Cannon : public Obstacle
{
private:
	int type;

	RECT hitRect;
	POINT centerPos;

	RECT testShot;
	POINT tempCenter;
	POINT nextSpot;

	RECT shotCd;
	// 테스트 샷(벽판정)
	
	time_t nowTime;
	struct tm *tmTime = localtime(&nowTime);
	bool isCanShoot;

	float timer;
	int countDownSec;

	float shootedTimer;
	int shottedDownSec;
	bool isShooted;
	
	float checkTimer;
	int hitRangeSec;
	bool isPlayer; 

public:
	Cannon();
	Cannon(parceCannon set);
	~Cannon();

	void Update();
	void DrawObject(HDC hdc);
	void RenderObject(HWND hWnd, HDC hdc);

	void CheckInPlayer();
	void MoveTestShot();
	void CheckHit();
	bool CheckTileMap(TileMap mapTile);

	void Reset();
};
