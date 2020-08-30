#include "stdafx.h"
#include "ExplodeClass.h"
#include "PlayerClass.h"
#include "GameManger.h"
#include <string>

#include<commdlg.h>

#pragma comment(lib, "msimg32.lib")

#define dMaxCnt 200

#define dGameManager GameManager::GetInstance()
#define dPlayer Player::GetInstance()

using namespace std;

Explode::Explode()
{
	explodStruct temp;
	for (int i = 0; i < dMaxCnt; i++)
	{
		temp.centerPos = { 0,0 };
		temp.explodeRect = { 0,0,0,0 };
		temp.isStart = false;

		temp.curFrame = 1;
		temp.maxFrame = 7;

		temp.hAniImg = (HBITMAP)LoadImage(NULL, TEXT("../Image/explode.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(temp.hAniImg, sizeof(BITMAP), &temp.bitAni);

		explodeList.push_back(temp);
	}
}

Explode::~Explode()
{
	for (int i = 0; i < dMaxCnt; i++)
		DeleteObject(explodeList[i].hAniImg);
}

Explode* Explode::GetInstance()
{
	static Explode explode;
	return &explode;
}

void Explode::Update()
{
	for (int i = 0; i < dMaxCnt; i++)
	{
		if (explodeList[i].isStart == false)
			CheckHitPlayer(explodeList[i]);
	}
}

void Explode::CheckHitPlayer(explodStruct effect)
{
	RECT area;
	RECT playerPos = dPlayer->GetPlayerPos();
	
	if (IntersectRect(&area, &playerPos, &effect.explodeRect) && effect.isStart == true)
	{
		effect.isStart = false;
		effect.curFrame = 1;
		printf("playerHit!\n");
		// todo : player 사망 판정 & 스테이지 리셋
	}
}

void Explode::DrawObject(HDC hdc)
{
	for (int i = 0; i < dMaxCnt; i++)
	{
		if (explodeList[i].isStart == true)
			Rectangle(hdc, explodeList[i].explodeRect.left, explodeList[i].explodeRect.top, 
				explodeList[i].explodeRect.right, explodeList[i].explodeRect.bottom);
	}
}

void Explode::DrawBitMap(HWND hWnd, HDC hdc)
{
	for (int i = 0; i < dMaxCnt; i++)
	{
		if (explodeList[i].isStart == true)
		{
			HDC hMemDC;
			HBITMAP hOldBitmap;

			hMemDC = CreateCompatibleDC(hdc);
			hOldBitmap = (HBITMAP)SelectObject(hMemDC, explodeList[i].hAniImg);

			int bx = explodeList[i].bitAni.bmWidth / 8;
			int by = explodeList[i].bitAni.bmHeight;

			int xStart = explodeList[i].curFrame * bx;
			int yStart = 0;

			TransparentBlt(hdc, explodeList[i].explodeRect.left, explodeList[i].explodeRect.top, bx, by, hMemDC, xStart, yStart, bx, by, RGB(255, 0, 255));

			SelectObject(hMemDC, hOldBitmap);
			DeleteDC(hMemDC);
		}
	}
}

void Explode::SetNextFrame()
{
	RECT area;
	for (int i = 0; i < dMaxCnt; i++)
	{
		if (explodeList[i].isStart == true)
		{
			if (IntersectRect(&area, &explodeList[i].explodeRect, &dPlayer->GetFocusPos()) && dPlayer->GetIsFocusMode())
				continue;	// 포커스 모드 & 영역 포함일 시 폭발 일시 정지
			else
				explodeList[i].curFrame++;

			if (explodeList[i].curFrame++ > explodeList[i].maxFrame)
			{
				explodeList[i].isStart = false;
				explodeList[i].curFrame = 1;
			}
		}
	}	// for
}

void Explode::StartExplode(POINT bulletPos)
{
	for (int i = 0; i < dMaxCnt; i++)
	{
		if (explodeList[i].isStart == false)
		{
			explodeList[i].isStart = true;
			explodeList[i].centerPos = bulletPos;

			explodeList[i].explodeRect.left = explodeList[i].centerPos.x - 32;
			explodeList[i].explodeRect.top = explodeList[i].centerPos.y - 32;
			explodeList[i].explodeRect.right = explodeList[i].centerPos.x + 32;
			explodeList[i].explodeRect.bottom = explodeList[i].centerPos.y + 32;
		}
	}
}
