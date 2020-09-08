#pragma once
#include "stdafx.h"
#include "GameManager.h"
#include "MapClass.h"
#include <time.h>

class UI : public Object
{
private:
	HBITMAP hUIBitmap;
	BITMAP uiBitmap;

	RECT mainBtn;
	RECT playBtn;

	time_t nowTime;
	struct tm *tmTime;

	int timer;
	int countDownSec;

	POINT nowFrame;

	bool isGoMain;
	bool isEndScene;

	UI();

public:
	static UI* GetInstance();
	~UI();

	void Update();

	void DrawObject(HDC hdc);
	void RenderObject(HWND hWnd, HDC hdc);

	void CheckPushBtn(POINT pos);

	void Reset();

	void SetIsGoMain(bool set);
	bool GetIsGoMain();
};