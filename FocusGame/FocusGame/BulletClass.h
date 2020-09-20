 #pragma once
#include "stdafx.h"
#include "GameManager.h"
#include "FocusGame.h"
#include "ObstacleClass.h"

using namespace std;

class Bullet : public Obstacle
{
private:
	vector<BulletStruct> nBulletList;
	vector<BulletStruct> hBulletList;

	Bullet();

public:
	~Bullet();
	static Bullet* GetInstance();

	void Update();
	void DrawObject(HDC hdc);
	void RenderObject(HWND hWnd, HDC hdc);

	void Shoot(const RECT &hitRect, const POINT &cannonCenter, const POINT &playerCenter, int bulletType);
	void CalcBullet(BulletStruct &bullet, const POINT &cannonCenter, const POINT &playerCenter, int bulletType);
	void CheckShot();
	void MoveShot(BulletStruct &bullet);
	void CheckHit(BulletStruct &bullet);
	bool CheckTileMap(TileMap mapTile);
	
	void Reset();
	void ResetBullet(BulletStruct &bullet);

	RECT ConversionRect(POINT pos);
	int CalcSpeed();
};
