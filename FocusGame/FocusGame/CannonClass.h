#pragma once
#include "stdafx.h"
#include "GameManger.h"
#include "ObstacleClass.h"

#include <time.h>

#define hitRange 5
#define largeHitRange 10

class Cannon : public Obstacle
{
private:
	RECT hitRect;

	POINT centerPos;

	RECT testShot;
	
	time_t nowTime;
	struct tm *tmTime = localtime(&nowTime);

	int timer;
	int countDownSec;

public:
	Cannon();
	Cannon(POINT set);
	~Cannon();

	void Update();
	void DrawObject(HDC hdc);

	void CheckInPlayer();
};
