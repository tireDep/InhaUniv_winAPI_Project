#pragma once
#include "stdafx.h"
#include "GameManger.h"
#include "ObstacleClass.h"

#include <time.h>

class Cannon : public Obstacle
{
private:
	RECT hitRect;

	POINT centerPos;

	POINT tempSpot;
	RECT testShot;
	
	time_t nowTime;
	struct tm *tmTime = localtime(&nowTime);

	float timer;
	int countDownSec;

public:
	Cannon();
	Cannon(POINT set);
	~Cannon();

	void Update();
	void DrawObject(HDC hdc);

	void CheckInPlayer();
};
