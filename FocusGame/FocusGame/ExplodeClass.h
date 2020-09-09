#pragma once
#include "stdafx.h"
#include "FocusGame.h"
#include "ObstacleClass.h"

class Explode : public Obstacle
{
private:
	vector<explodStruct> explodeList;

	Explode();

public:
	~Explode();
	
	static Explode* GetInstance();

	void Update();
	void CheckHitPlayer(explodStruct &effect);

	void DrawObject(HDC hdc);
	void RenderObject(HWND hWnd, HDC hdc);
	void SetNextFrame();

	void StartExplode(POINT bulletPos);

	void Reset();
	void ResetExplode(explodStruct &effect); 
};
