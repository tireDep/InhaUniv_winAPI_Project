#pragma once
#include "stdafx.h"
#include "GameManager.h"

class UI : public Object
{
private:
	HBITMAP hUIBitmap;
	BITMAP uiBitmap;

	RECT mainBtn;
	RECT playBtn;

	UI();

public:
	static UI* GetInstance();
	~UI();

	void Update();

	void DrawObject(HDC hdc);
	void RenderObject(HWND hWnd, HDC hdc);

	void CheckPushBtn(POINT pos);

	void Reset();

};

UI::UI()
{
	mainBtn = { 295, 305, 395, 370 };
	playBtn = { 405, 305, 505, 370 };

	hUIBitmap = (HBITMAP)LoadImage(NULL, TEXT("../Image/ui.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hUIBitmap, sizeof(BITMAP), &uiBitmap);
}

UI * UI::GetInstance()
{
	static UI ui;
	return &ui;
}

UI::~UI()
{
	DeleteObject(hUIBitmap);
}

void UI::Update()
{

}

void UI::DrawObject(HDC hdc)
{

}

void UI::RenderObject(HWND hWnd, HDC hdc)
{
	HDC uiDc;
	HBITMAP hUIBit;
	POINT pos = { eTrueWinWidth, eTrueWinHeight };
	POINT aniPos = { 0,0 };

	POINT showPos = { 0,0 };

	uiDc = CreateCompatibleDC(hdc);
	hUIBit = (HBITMAP)SelectObject(uiDc, hUIBitmap);

	if (dgameManager->GetNowScene() == eMainScene)
	{
		aniPos.x = eTrueWinWidth * 1;
		aniPos.y = eTrueWinHeight * 0;

		TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, aniPos.x, aniPos.y, pos.x, pos.y, RGB(255, 0, 255));
		// todo : 애니메이션 적용
	}
	else if (dgameManager->GetNowScene() == eChangeScene)
	{
		// todo : 애니메이션 후 화면 전환
		aniPos.x = eTrueWinWidth * 2;
		aniPos.y = eTrueWinHeight * 1;

		TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, aniPos.x, aniPos.y, pos.x, pos.y, RGB(255, 0, 255));

		dgameManager->SetNowScene(eGameScene);
	}
	else if (dgameManager->GetNowScene() == eGameScene)
	{
		if (!dgameManager->GetIsPlayerLive())
		{
			showPos.y = eTrueWinHeight * 2;
			TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, showPos.x, showPos.y, pos.x, pos.y, RGB(255, 0, 255));
		}

		if (dgameManager->GetIsPause())
		{
			showPos.y = eTrueWinHeight * 3;
			TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, showPos.x, showPos.y, pos.x, pos.y, RGB(255, 0, 255));
		}
	}
	else if (dgameManager->GetNowScene() == eResultScene)
	{
		// todo : 애니메이션 후 화면 전환
		aniPos.x = eTrueWinWidth * 2;
		aniPos.y = eTrueWinHeight * 4;

		TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, aniPos.x, aniPos.y, pos.x, pos.y, RGB(255, 0, 255));

		dgameManager->SetSceneNum(eMainScene);
	}

	SelectObject(uiDc, hUIBit);
	DeleteObject(uiDc);
}

void UI::CheckPushBtn(POINT pos)
{
	if (PtInRect(&mainBtn, pos))
		dgameManager->SetNowScene(eMainScene);

	else if (PtInRect(&playBtn, pos))
		dgameManager->SetIsPause();
}

void UI::Reset()
{

}
