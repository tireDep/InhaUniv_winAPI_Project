#include "stdafx.h"
#include "UIClass.h"
#include "SoundSystem.h"
#include "PlayerClass.h"

#define dCountDown 2
#define dEndDown 1

#define dChangeFrameMax eTrueWinWidth * 4
#define dEndFrameMax eTrueWinWidth * 4

#define dForTimer 75000000

#define dMap Map::GetInstance()
#define dGameManager GameManager::GetInstance()
#define dSoundSys SoundSystem::GetInstance()
#define dPlayer Player::GetInstance()

UI::UI()
{
	mainBtn = { 295, 305, 395, 370 };
	playBtn = { 405, 305, 505, 370 };

	timer = 0;
	countDownSec = dCountDown;

	nowFrame = { 0,0 };

	isGoMain = true;
	isEndScene = false;

	hUIBitmap = (HBITMAP)LoadImage(NULL, TEXT("Image/ui.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
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

	uiDc = CreateCompatibleDC(hdc);
	hUIBit = (HBITMAP)SelectObject(uiDc, hUIBitmap);

	if (dGameManager->GetNowScene() == eMainScene)
	{
		// >> timer
		time(&nowTime);
		tmTime = localtime(&nowTime);

		if (timer != tmTime->tm_sec)
		{
			timer = tmTime->tm_sec;
			countDownSec--;
		}
		// >> timer

		countDownSec = countDownSec == 0 ? dCountDown : countDownSec;

		if (countDownSec <= dCountDown * 0.5)
			nowFrame = { 0,0 };
		else
			nowFrame = { eTrueWinWidth,0 };

		TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, nowFrame.x, nowFrame.y, pos.x, pos.y, RGB(255, 0, 255));
	}
	else if (dGameManager->GetNowScene() == eChangeScene)
	{
		countDownSec = dCountDown;

		// >> tiemr
		for (double i = 0; i < dForTimer; i++) {} // >> 창 전환 카운트, 타이머로 원하는 속도가 나오지 않음
		nowFrame.x += eTrueWinWidth;
		nowFrame.y = eTrueWinHeight * 1;

		TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, nowFrame.x, nowFrame.y, pos.x, pos.y, RGB(255, 0, 255));

		if (nowFrame.x >= dChangeFrameMax)
		{
			dSoundSys->PlayChangeStageSound();
			dMap->SetIsNextStage(true);
			dGameManager->SetNowScene(eGameScene);
		}
	}
	else if (dGameManager->GetNowScene() == eGameScene)
	{
		if (dGameManager->GetNowStage() == 0)
		{
			nowFrame = { eTrueWinWidth, eTrueWinHeight * 2 };
			TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, nowFrame.x, nowFrame.y, pos.x, pos.y, RGB(255, 0, 255));
		}

		if (dPlayer->GetFocusLv() == 1 && dGameManager->GetFocusLv() == 0)
		{
			// >> 처음 아이템 먹을 때 UI
			nowFrame = { eTrueWinWidth * 2, eTrueWinHeight * 2 };
			TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, nowFrame.x, nowFrame.y, pos.x, pos.y, RGB(255, 0, 255));
		}

		if (!dGameManager->GetIsPlayerLive())
		{
			nowFrame = { 0,eTrueWinHeight * 2 };
			TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, nowFrame.x, nowFrame.y, pos.x, pos.y, RGB(255, 0, 255));
		}

		if (dGameManager->GetIsPause())
		{
			nowFrame = { 0,eTrueWinHeight * 3 };
			TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, nowFrame.x, nowFrame.y, pos.x, pos.y, RGB(255, 0, 255));
		}
	}
	else if (dGameManager->GetNowScene() == eResultScene)
	{
		if (!isEndScene)
		{
			dSoundSys->SetIsStop(true);
			isEndScene = true;
			nowFrame = { 0,eTrueWinHeight * 4 }; // >> 맨 처음 들어왔을 때
		}
		
		// >> timer
		if (nowFrame.y == eTrueWinHeight * 4)
		{
			time(&nowTime);
			tmTime = localtime(&nowTime);

			if (timer != tmTime->tm_sec)
			{
				timer = tmTime->tm_sec;
				countDownSec--;
			}

			if (countDownSec <= 0)
			{
				countDownSec = dEndDown;
				nowFrame.x += eTrueWinWidth;
			}
		}
		else if (nowFrame.y == eTrueWinHeight * 5)
		{
			for (double i = 0; i < dForTimer * 2; i++) {} // >> 창 전환 카운트, 타이머로 원하는 속도가 나오지 않음
			nowFrame.x += eTrueWinWidth;
		}
		// >> timer

		TransparentBlt(hdc, 0, 0, pos.x, pos.y, uiDc, nowFrame.x, nowFrame.y, pos.x, pos.y, RGB(255, 0, 255));

		if (dEndFrameMax <= nowFrame.x && nowFrame.y == eTrueWinHeight * 4)
			nowFrame = { 0,eTrueWinHeight * 5 };
		else if (dEndFrameMax < nowFrame.x && nowFrame.y == eTrueWinHeight * 5)
		{
			isEndScene = false;
			countDownSec = dCountDown;
			nowFrame = { 0,0 };
			dMap->SetIsNextStage(true);
			dSoundSys->SetIsStop(false);
			dGameManager->SetFocusLv(0);
			dGameManager->SetNowScene(eMainScene);
		}
	}

	SelectObject(uiDc, hUIBit);
	DeleteObject(hUIBit);
	DeleteDC(uiDc);
}

void UI::CheckPushBtn(POINT pos)
{
	if (PtInRect(&mainBtn, pos))
	{
		isGoMain = true;
		dGameManager->SetIsPause();
		dGameManager->SetNowScene(eMainScene);
		dSoundSys->SetIsStop(false);
	}

	else if (PtInRect(&playBtn, pos))
		dGameManager->SetIsPause();
}

void UI::Reset()
{

}

void UI::SetIsGoMain(bool set)
{
	isGoMain = set;
}

bool UI::GetIsGoMain()
{
	return isGoMain;
}

