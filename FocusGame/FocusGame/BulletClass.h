#pragma once
#include "stdafx.h"
#include "GameManager.h"
#include "FocusGame.h"
#include "ObstacleClass.h"

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
	void RenderObject(HWND hWnd, HDC hdc);

	void Shoot(const POINT &cannonCenter, const POINT &playerCenter, int bulletType);
	void CalcBullet(BulletSctruct &bullet, const POINT &cannonCenter, const POINT &playerCenter, int bulletType);
	void CheckShot();
	void MoveShot(BulletSctruct &bullet);
	void CheckHit(BulletSctruct &bullet);
	
	void Reset();
	void ResetBullet(BulletSctruct &bullet);

	RECT ConversionRect(POINT pos);
};
