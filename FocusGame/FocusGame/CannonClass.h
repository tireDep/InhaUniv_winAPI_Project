#pragma once
#include "stdafx.h"
#include "GameManger.h"
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
	// �׽�Ʈ ��(������)
	
	time_t nowTime;
	struct tm *tmTime = localtime(&nowTime);
	bool isCanShoot;

	float timer;
	int countDownSec;

public:
	Cannon();
	Cannon(parceCannon set);
	~Cannon();

	void Update();
	void DrawObject(HDC hdc);

	void CheckInPlayer();
	void MoveTestShot();
	void ResetTestShot();
};
