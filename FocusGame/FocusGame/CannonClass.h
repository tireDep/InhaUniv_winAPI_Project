#pragma once
#include "stdafx.h"
#include "GameManger.h"
#include "ObstacleClass.h"

#define hitRange 5.5

class Cannon : public Obstacle
{
private:
	RECT hitRect;
	int test;

public:
	Cannon();
	Cannon(int set);
	~Cannon();

	void Update();
	void DrawObject(HDC hdc);
};
