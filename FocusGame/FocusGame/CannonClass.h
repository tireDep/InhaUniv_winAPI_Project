#pragma once
#include "stdafx.h"
#include "GameManger.h"
#include "ObstacleClass.h"

#define hitRange 5
#define largeHitRange 10

class Cannon : public Obstacle
{
private:
	RECT hitRect;

	POINT centerPos;

public:
	Cannon();
	Cannon(POINT set);
	~Cannon();

	void Update();
	void DrawObject(HDC hdc);

	void CheckInPlayer();
};
