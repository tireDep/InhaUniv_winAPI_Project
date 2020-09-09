#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "FocusGame.h"

using namespace std;

class Obstacle : public Object
{
private:

public:
	Obstacle();
	virtual ~Obstacle();

	virtual void Update();
	virtual void DrawObject(HDC hdc);
	virtual void RenderObject(HWND hWnd, HDC hdc);

	static void AddWeapon(vector<Obstacle *> &obstacle, vector<parceCannon> cannonSet);

	virtual void Reset();

	static void DeleteAllData(vector<Obstacle *> &vec);
};
