#pragma once
#include "stdafx.h"
#include "Obstacle.h"

class Bullet : public Obstacle
{
private:

public:
	Bullet();
	~Bullet();

	void Update();
	void DrawObject(HDC hdc);
};
