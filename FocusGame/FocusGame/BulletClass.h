#pragma once
#include "stdafx.h"
#include "GameManger.h"
#include "FocusGame.h"
#include "ObstacleClass.h"

#define dNormal -1
#define dHoming 1

using namespace std;

class Bullet : public Obstacle
{
private:
	vector<BulletSctruct> nBulletList;
	vector<BulletSctruct> hBulletList;

	Bullet();

public:
	~Bullet();
	static Bullet* GetInstance();

	void Update();
	void DrawObject(HDC hdc);

	void Shoot(const POINT &cannonPos, const POINT &playerPos, int bulletType);

	RECT ConversionRect(POINT pos);
};
