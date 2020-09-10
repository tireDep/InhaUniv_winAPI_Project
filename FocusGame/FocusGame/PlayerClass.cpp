#include "stdafx.h"
#include "FocusGame.h"
#include "PlayerClass.h"
#include "MapClass.h"
#include "SoundSystem.h"

#include <time.h>

#define dAddFocusP 1.5
#define dMinusFocusP 1

#define dWalkMax 2
#define dDeadMax 4

#define dCountDown 1

#define dGameManager GameManager::GetInstance()
#define dMap Map::GetInstance()
#define dSoundSys SoundSystem::GetInstance()

Player::Player()
{
	hFocusBitmap = (HBITMAP)LoadImage(NULL, TEXT("Image/focus.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hFocusBitmap, sizeof(BITMAP), &focusBitmap);

	hSpotBitmap = (HBITMAP)LoadImage(NULL, TEXT("Image/focusSpot.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hSpotBitmap, sizeof(BITMAP), &spotBitmap);

	hPlayerBitmap = (HBITMAP)LoadImage(NULL, TEXT("Image/player.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hPlayerBitmap, sizeof(BITMAP), &playerBitmap);
}

Player::~Player()
{

}

Player* Player::GetInstance()
{
	static Player player;
	return &player;
}

void Player::Update()
{
	Gravity();
	// >> �÷��̾� ������ ��� ���� �߷��� ������ �޾ƾ� ��

	if (playerState != eDead)
		CanMovePlayer();
	else
	{
		dSoundSys->SetIsPause(true);
		if (isEndAni)
			dGameManager->SetIsPlayerLive(false);
	}
}

void Player::Gravity()
{
	int diffNum = 0;

	if (playerState != eFocus && playerState != eDead) // >> ��Ŀ�� ����� �� ������ ��õ(?) �ϴ°� ����
		CheckOut(playerPos, eMoveDown);	// >> �� ������ �������� �ʰ� ���� 

	if (playerState != eFocus || playerState != eJump)
	{
		for (int i = 0; i < 4; i++)
			playerPos[i].y += gravity * dTimeSec;

		if (CheckBtmGround(diffNum))
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y += diffNum + dTimeSec;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y -= diffNum + dTimeSec;
		}
	}
}

bool Player::CheckBtmGround(int &lengthDiff)
{
	RECT temp;
	vector<TileMap> checkBtm = dMap->GetMapPos();
	RECT checkRect = ConversionRect(playerPos);

	for (int i = 0; i < checkBtm.size(); i++)
	{
		if (IntersectRect(&temp, &checkBtm[i].pos, &checkRect))
		{
			if (CheckTileMap(checkBtm[i]))
			{
				isBtmGround = true;
				lengthDiff = checkRect.bottom - checkBtm[i].pos.top;
				return false;
			}
		}
		else
			isBtmGround = false;
	}
	 return true;
}

bool Player::CollisionMap(POINT pos[], int direction, int & lengthDiff)
{
	RECT areaRect;
	vector<TileMap> checkBtm = dMap->GetMapPos();
	RECT checkRect = ConversionRect(pos);

	if (direction == eMoveRight)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && CheckTileMap(checkBtm[i]))
			{
				lengthDiff = checkBtm[i].pos.left - checkRect.right;
				return false;
			}
			else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapSpike)
			{
				SetIsPlayerDead(true);
				return true;
			}
		}
		return true;
	}

	if (direction == eMoveLeft)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && CheckTileMap(checkBtm[i]))
			{
				lengthDiff = checkBtm[i].pos.right - checkRect.left;
				return false;
			}
			else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapSpike)
			{
				SetIsPlayerDead(true);
				return true;
			}
		}
		return true;
	}

	if (direction == eMoveUp || direction == eJump)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && CheckTileMap(checkBtm[i]))
			{
				lengthDiff = checkBtm[i].pos.bottom - checkRect.top;
				return false;
			}
			else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapSpike)
			{
				SetIsPlayerDead(true);
				return true;
			}

		}
		return true;
	}

	if (direction == eMoveDown)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && CheckTileMap(checkBtm[i]))
			{
				lengthDiff = checkRect.bottom - checkBtm[i].pos.top;
				return false;
			}
			else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapSpike)
			{
				SetIsPlayerDead(true);
				return true;
			}
		}
		return true;
	}

}

void Player::CheckOut(POINT pos[], int direction)
{
	int diffNum = 0;
	bool isCheckMap = CheckOutMap(pos, direction, diffNum);
	float mulNum = isCheckMap == true ? -1 : 1;
	float addNum = 0;

	if (direction == eMoveRight || direction == eMoveDown)
		addNum = -dCorrection;

	MovePlayer(pos, direction, diffNum, mulNum, addNum);
}

bool Player::CheckOutMap(POINT pos[], int direction, int &lengthDiff)
{
	RECT area;
	RECT checkRect = ConversionRect(pos);
	vector<TileMap> mapPos = dMap->GetMapPos();

	if (direction == eMoveLeft)
	{
		if (checkRect.left <= eLimitL)
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if ((mapPos[i].pos.left == checkRect.left && mapPos[i].pos.right == checkRect.right && mapPos[i].pos.top == checkRect.top && mapPos[i].pos.bottom == checkRect.bottom)
					&& mapPos[i].type == eMapGateOpen)
					return true;
			}

			lengthDiff = eLimitL - checkRect.left;
			return false;
		}
		else
			return true;
	}

	if (direction == eMoveRight)
	{
		if (checkRect.right >= eLimitR)
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if ((mapPos[i].pos.left == checkRect.left && mapPos[i].pos.right == checkRect.right && mapPos[i].pos.top == checkRect.top && mapPos[i].pos.bottom == checkRect.bottom)
					&& mapPos[i].type == eMapGateOpen)
					return true;
			}

			lengthDiff = eLimitR - checkRect.right;
			return false;
		}
		else
			return true;
	}

	if (direction == eMoveDown)
	{
		if (checkRect.bottom >= eLimitB)
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if ((mapPos[i].pos.left == checkRect.left && mapPos[i].pos.right == checkRect.right && mapPos[i].pos.top == checkRect.top && mapPos[i].pos.bottom == checkRect.bottom)
					&& mapPos[i].type == eMapGateOpen)
					return true;
			}

			lengthDiff = eLimitB - checkRect.bottom;
			return false;
		}
		else
			return true;
	}

	if (direction == eMoveUp)
	{ 
		if (checkRect.top <= eLimitT)
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if ((mapPos[i].pos.left == checkRect.left && mapPos[i].pos.right == checkRect.right && mapPos[i].pos.top == checkRect.top && mapPos[i].pos.bottom == checkRect.bottom) && 
					mapPos[i].type == eMapGateOpen)
					return true;
			}

			lengthDiff = eLimitT - checkRect.top;
			return false;
		}
		else
			return true;
	}
}

bool Player::CheckBlockMap()
{
	RECT area;
	vector<TileMap> tempMap = dMap->GetMapPos();
	RECT conRect = ConversionRect(fMovePos);
	RECT conRect2 = ConversionRect(playerPos);
	// �� : ��Ŀ�� ��ǥ�� ��ƾ� �� �� ���� ���� ����
	
	for (int i = 0; i < tempMap.size(); i++)
	{
		if (IntersectRect(&area, &tempMap[i].pos, &conRect) && CheckTileMap(tempMap[i]))
		{
			ReturnLastPos();	// ��Ŀ�� ��ǥ�� ���� ��ġ�� ��
			return false;
		}

		if (IntersectRect(&area, &tempMap[i].pos, &conRect2) && CheckTileMap(tempMap[i]))
		{
			ReturnLastPos();	// �÷��̾� ��ǥ�� ���� ��ġ�� ��
			return false;
		}
	}

	RECT playerRect = ConversionRect(playerPos);
	if (playerRect.left < 0 || playerRect.right>eTrueWinWidth || playerRect.top<0 || playerRect.bottom>eTrueWinHeight)
	{
		ReturnLastPos();	// �̵��� ��ǥ�� ���� ��� ��
		return false;
	}

	lastPlayerPos[0] = playerPos[0];
	lastPlayerPos[1] = playerPos[1];
	lastPlayerPos[2] = playerPos[2];
	lastPlayerPos[3] = playerPos[3];

	return true;
}

void Player::DrawObject(HDC hdc)
{
	if (dGameManager->GetDrawRect())
	{
		Polygon(hdc, focusPos, 4);
		Polygon(hdc, playerPos, 4);

		if (playerState == eFocus)
			Polygon(hdc, fMovePos, 4);
	}
}

void Player::RenderObject(HWND hWnd, HDC hdc)
{
	RECT focusRect = ConversionRect(focusPos);
	RECT spotRect = ConversionRect(fMovePos);
	
	HDC focusStartDc;
	HBITMAP hFocusStartBit;
	HDC focusFinDc;
	HBITMAP hFocusBit, hFocusBit2;
	int bx, by;

	HDC spotDc;
	HBITMAP hSpotBit;
	int spotX, spotY;

	if (!GetIsFocusMode())
	{
		// �Ϲ� ����
		focusFinDc = CreateCompatibleDC(hdc);
		hFocusBit = CreateCompatibleBitmap(hdc, 500, 500);	// bitmap size
		hFocusBit2 = (HBITMAP)SelectObject(focusFinDc, hFocusBit);

		focusStartDc = CreateCompatibleDC(hdc);
		hFocusStartBit = (HBITMAP)SelectObject(focusStartDc, hFocusBitmap);

		bx = focusBitmap.bmWidth;
		by = 500;

		BLENDFUNCTION bf;
		bf.AlphaFormat = 0;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = 10;	// ����

		StretchBlt(focusFinDc, 0, 0, 500, 500, focusStartDc, 0, 0, bx, by, SRCCOPY);	// ������ ����

		AlphaBlend(hdc, focusRect.left, focusRect.top, focusRect.right - focusRect.left, focusRect.bottom - focusRect.top,
			focusFinDc, 0, 0, 500, 500, bf);	// ����

		SelectObject(focusFinDc, hFocusBit2);
		DeleteDC(focusFinDc);
		DeleteObject(hFocusBit);
		DeleteObject(hFocusBit2);

		SelectObject(focusStartDc, hFocusStartBit);
		DeleteDC(focusStartDc);
	}
	else
	{
		// ��Ŀ�� ����
		focusFinDc = CreateCompatibleDC(hdc);
		hFocusBit = CreateCompatibleBitmap(hdc, 500,500);
		hFocusBit2 = (HBITMAP)SelectObject(focusFinDc, hFocusBit);

		focusStartDc = CreateCompatibleDC(hdc);
		hFocusStartBit = (HBITMAP)SelectObject(focusStartDc, hFocusBitmap);
		
		bx = focusBitmap.bmWidth;
		by = 500;

		spotDc = CreateCompatibleDC(hdc);
		hSpotBit = (HBITMAP)SelectObject(spotDc, hSpotBitmap);
		spotX = spotBitmap.bmWidth;
		spotY = spotBitmap.bmHeight;

		BLENDFUNCTION bf;
		bf.AlphaFormat = 0;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = 100;	// ����

		StretchBlt(focusFinDc, 0, 0, 500, 500, focusStartDc, 0, by, bx, by, SRCCOPY); // ������ ����

		AlphaBlend(hdc, focusRect.left, focusRect.top, focusRect.right - focusRect.left, focusRect.bottom - focusRect.top,
			focusFinDc, 0, 0, 500, 500, bf);	// ����

		// >> ��Ŀ�� ��ǥ
		HPEN hPen, oldPen;
		hPen = CreatePen(PS_DOT, 1, RGB(36, 36, 36));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		MoveToEx(hdc, centerPos.x, centerPos.y, NULL);
		LineTo(hdc, fCenterPos.x, fCenterPos.y);

		TransparentBlt(hdc, spotRect.left, spotRect.top, spotX, spotY, spotDc, 0, 0, spotX, spotY, RGB(255, 0, 255));

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		// >> ��Ŀ�� ��ǥ

		SelectObject(focusFinDc, hFocusBit2);
		DeleteDC(focusFinDc);
		DeleteObject(hFocusBit);
		DeleteObject(hFocusBit2);

		SelectObject(focusStartDc, hFocusStartBit);
		DeleteDC(focusStartDc);

		SelectObject(spotDc, hSpotBit);
		DeleteDC(spotDc);
	}

	RECT playerRect = ConversionRect(playerPos);
	HDC playerDc;
	HBITMAP hPlayerBit;
	int posX, posY;
	POINT aniPos = { 0,0 };

	playerDc = CreateCompatibleDC(hdc);
	hPlayerBit = (HBITMAP)SelectObject(playerDc, hPlayerBitmap);

	posX = 16;
	posY = 16;

	if(playerState==eDead)
	{
		aniPos = { nowFrame, 96 };
		
		time(&nowTime);
		tmTime = localtime(&nowTime);

		if (timer != tmTime->tm_sec)
		{
			timer = tmTime->tm_sec;
			countDownSec--;
		}

		if (countDownSec <= 0)
		{
			countDownSec = dCountDown;
			nowFrame += 16;
		}

		if (nowFrame == dDeadMax * 16)
		{
			nowFrame = 0;
			isEndAni = true;
		}
	}

	if (!dGameManager->GetIsPause())
	{
		if (playerState == eIdle)
			aniPos = { 0,0 };
		else if (playerState == eMoveLeft || playerState == eMoveRight)
		{
			aniPos = { nowFrame, 32 };
			nowFrame += 16;
			if (nowFrame == dWalkMax * 16)
				nowFrame = 0;
		}
		else if (playerState == eJump)
			aniPos = { 0,64 };
		else if (playerState == eFall)
			aniPos = { 16,64 };
		else if (playerState == eFocus)
			aniPos = { 0,128 };
	}
	else
		aniPos = { 0,0 };

	if (isRightSight == false)
		aniPos.y += 16;	// �¿���� Ȯ��

	TransparentBlt(hdc, playerRect.left, playerRect.top, posX, posY, playerDc, aniPos.x, aniPos.y, posX, posY, RGB(255, 0, 255));

	SelectObject(playerDc, hPlayerBit);
	DeleteDC(playerDc);
}

void Player::CalcFocusMove()
{
	if (lastMoveCenter.x != 0 && lastMoveCenter.y != 0 && playerState != eFocus)
	{
		bool isCanMoveCheck = CheckBlockMap();	// �̵���ġ�� ����� �ƴ� ��쿡�� �Ǵ���

		if (isCanMoveCheck)
		{
			bool isMomentum = FocusMomentum();

			int diffNum = 0;
			if (CheckBtmGround(diffNum) && isMomentum)
			{
				lastMoveCenter.x--;
				lastMoveCenter.y--;
			}
			else
				ReturnLastPos(); // �ʿ� ���̴� �� ����
		}
	}
}

bool Player::FocusMomentum()
{
	// ������ ���
	POINT speed;
	float v;

	int degree = 120;
	float time = 0.1;

	speed.x = lastMoveCenter.x - fCenterPos.x;
	speed.y = lastMoveCenter.y - fCenterPos.y;

	if (speed.x == 0 || speed.y == 0)
		return false;	// x�� ���� �̵� �� �ٴڿ��� �̲������� �� ����

	v = sqrt((pow(speed.x, 2) + pow(speed.y, 2)));
	// �ӷ� ���

	float halfG = gravity * 0.5;
	POINT calcV;
	if (speed.x > 0)
		degree = 210;

	calcV.x = v * cos(degree);
	calcV.y = v * sin(degree) - halfG * time;

	float finV = sqrt(pow(calcV.x, 2) + pow(calcV.y, 2));

	POINT pos;
	pos.x = speed.x * cos(degree)*time;
	pos.y = speed.y *sin(degree)*time - (0.5*halfG*pow(time, 2));

	if (speed.x == 0)
		pos.x = 0;
	else if (speed.x > 0)
		pos.x *= 1;
	else
		pos.x *= -1;

	if (speed.y == 0)
		pos.y = 0;
	else if (speed.y > 0)
		pos.y *= 1;
	else
		pos.y *= -1;

	if (pos.y == 1 || pos.y == -1)
		return false;	// ���� ª�� ��ġ �̵��� �ٴڿ��� �̲������� �� ����

	playerPos[0].x += pos.x;
	playerPos[0].y += pos.y;

	playerPos[1].x += pos.x;
	playerPos[1].y += pos.y;

	playerPos[2].x += pos.x;
	playerPos[2].y += pos.y;

	playerPos[3].x += pos.x;
	playerPos[3].y += pos.y;

	return true;
}

void Player::CanMovePlayer()
{
	CalcFocusMove();

	if (GetKeyState(0x41) >= 0 && GetKeyState(VK_DOWN) >= 0 && GetKeyState(VK_UP) >= 0
		&& GetKeyState(VK_RIGHT) >= 0 && GetKeyState(VK_LEFT) >= 0  && GetKeyState(VK_SPACE) >= 0 )
	{
		// Ű�� �� ������ ���� ��� IDLE ����(�ִϸ��̼� ���� X)
		playerState = eIdle;
		nowFrame = 0;
	}

	if (playerState != eFocus)
	{
		// �÷��̾� �̵�
		if (!isJump && (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8001))
		{
			moveDirection = eMoveDown;
			playerState = eFall;
			MovePlayer(playerPos, moveDirection, eMoveSpeed, 1, 0);

			int diffNum = 0;
			if (CheckBtmGround(diffNum))
				MovePlayer(playerPos, moveDirection, diffNum, 1, dCorrection);
			else
				MovePlayer(playerPos, moveDirection, diffNum, -1, dCorrection);

			CheckOut(playerPos, moveDirection);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8001) 
		{
			moveDirection = eMoveRight;
			playerState = eMoveRight;
			isRightSight = true;
			MovePlayer(playerPos, moveDirection, eMoveSpeed, 1, 0);

			int diffNum = 0;
			if (CollisionMap(playerPos, eMoveRight, diffNum))
				MovePlayer(playerPos, moveDirection, diffNum, -1, -dCorrection);
			else
				MovePlayer(playerPos, moveDirection, diffNum, 1, -dCorrection);

			CheckOut(playerPos, moveDirection);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8001)
		{
			moveDirection = eMoveLeft;
			playerState = eMoveLeft;
			isRightSight = false;
			MovePlayer(playerPos, moveDirection, eMoveSpeed, -1, 0);

			int diffNum = 0;
			if (CollisionMap(playerPos, eMoveLeft, diffNum))
				MovePlayer(playerPos, moveDirection, diffNum, -1, 0);
			else
				MovePlayer(playerPos, moveDirection, diffNum, 1, 0);

			CheckOut(playerPos, moveDirection);

		}
		// �÷��̾� �̵�

		// ����
		if (!isJump && (GetAsyncKeyState(VK_SPACE) & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8001) // || GetAsyncKeyState(VK_UP) & 0x8000)	// ���� ����
			&& GetKeyState(0x41) >= 0)	// ��Ŀ�� Ǯ���ڸ��� �ٴ� �� ����
		{
			isJump = true;
			playerState = eJump;
			jumpPower = eJumpPower;
		}
		else if (isJump && playerState == eJump)	// ���� ��
		{
			// �� 7ĭ ���� �پ��� (gravity = 185, jumpPower = 85 ����)
			jumpPower -= eGravity * dTimeSec;

			// >> �浹 ����
			POINT checkRect[4];
			checkRect[0] = playerPos[0];
			checkRect[1] = playerPos[1];
			checkRect[2] = playerPos[2];
			checkRect[3] = playerPos[3];

			int diffNum = 0;
			int underLineNum = 0;
			for (int i = 1; i < jumpPower; i++)
			{
				MovePlayer(checkRect, playerState, 1, -1, 0);

				if (CollisionMap(checkRect, playerState, diffNum))
					underLineNum++;
				else
					underLineNum--;

				CheckOut(playerPos, eMoveUp);	// >> �� ������ �������� �ʰ� ���� 
			}
			// << �浹 ����

			if (underLineNum > 0)
			{
				MovePlayer(playerPos, playerState, underLineNum + 8, -1, 0); 
				// ���� ���� ���� ����
			}
			else
			{
				playerState = eFall;
				MovePlayer(playerPos, playerState, 0, 1, 1);
			}
		}
		else // ���� �� �ٴڿ� ����
		{
			// ���� �ϴ� ���ȿ� ���� ���ϰ� ���ƾ� ��
			// �ٴڿ� ����� ��� ���� & �ٷ� ���� x
			if (GetKeyState(VK_SPACE) < 0) //  || (GetKeyState(VK_UP) < 0)))
				isJump = true;	// jumpŰ�� ������ �ִ� ��Ȳ

			else if (isBtmGround)
			{
				// jumpŰ�� ������ ���� x, ���鿡 ���� -> ���� �ʱ�ȭ, n�� ���� ����
				isJump = false;
				jumpPower = eJumpPower;
			}
		}	// else_jump
		// ����

		// ��Ŀ��
		if (((GetAsyncKeyState(0x41) & 0x8000 || GetAsyncKeyState(0x41) & 0x8001)) && !isCharging)
		{
			moveSpeed = 0;
			jumpPower = 0;
			gravity = 0;

			playerState = eFocus;
			CalcCenterPos();
			lastMoveCenter.x = centerPos.x;
			lastMoveCenter.y = centerPos.y;

			lastPlayerPos[0] = playerPos[0];
			lastPlayerPos[1] = playerPos[1];
			lastPlayerPos[2] = playerPos[2];
			lastPlayerPos[3] = playerPos[3];
			// ��� ����� ���� ���� �� ����

			SetPos(fMovePos, centerPos.x, centerPos.y, ePlayerSize);
			dSoundSys->PlayFocusSound();
		}

		else
		{
			moveSpeed = eMoveSpeed;
			gravity = eGravity;

			if (focusGauge < maxFocusGauge)
				focusGauge += dAddFocusP;

			if (focusGauge <= eSmallFocus || GetKeyState(0x41) < 0)	// ��� ������ ������ ��Ŀ�� ��� ���� x
				isCharging = true;
			else
				isCharging = false;

			CalcCenterPos();
			SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);

			fCenterPos.x = centerPos.x;
			fCenterPos.y = centerPos.y;
		}
		// ��Ŀ��
	}

	else
	{
		if (((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(0x61) & 0x8000)))
		{
			if (focusGauge > eSmallFocus)
			{
				focusGauge -= dMinusFocusP;
				SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
			}
			else
			{
				isCharging = true;
				playerState = eIdle;
				// �������� �� �޸� �̵����� x
			}
		}
		else // �������� �� �����ϱ� ��, �̵��� ���
		{
			CalcFCenterPos();
			SetPos(playerPos, fCenterPos.x, fCenterPos.y, ePlayerSize);
			CheckBlockMap(); // >> �̵���ġ�� ����� �ƴ� ��쿡�� �̵�
			playerState = eIdle;
		}

		RECT area;
		RECT rcMovepos = ConversionRect(fMovePos);
		RECT rcFPos = ConversionRect(focusPos);

		POINT checkCenter;
		checkCenter.x = (fMovePos[0].x + fMovePos[2].x) / 2;
		checkCenter.y = (fMovePos[0].y + fMovePos[2].y) / 2;

		if ( (GetKeyState(VK_UP) >= 0 && GetKeyState(VK_DOWN) >= 0 && GetKeyState(VK_LEFT) >= 0 && GetKeyState(VK_RIGHT) >= 0)
			|| !IntersectRect(&area, &rcMovepos, &rcFPos) )	// �� : ���� ������ ������ �Ǹ� ����
		{
			//  Ű�� ������ ���� ���� ��� �÷��̾� ��ġ�� �ʱ�ȭ
			SetPos(fMovePos, centerPos.x, centerPos.y, ePlayerSize);
			fCenterPos.x = centerPos.x;
			fCenterPos.y = centerPos.y;
		}
		
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			moveDirection = eMoveUp;
			int underLineNum = CheckFocusRange(moveDirection, -1);
			// >> �浹 ����

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, -1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, 1);

			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			moveDirection = eMoveDown;
			int underLineNum = CheckFocusRange(moveDirection, 1);
			// >> �浹 ����

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, 1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, -1);

			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}
	

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			moveDirection = eMoveLeft;
			isRightSight = false;
			int underLineNum = CheckFocusRange(moveDirection, -1);
			// >> �浹 ����

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, -1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, 1);

			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			moveDirection = eMoveRight;
			isRightSight = true;

			int underLineNum = CheckFocusRange(moveDirection, 1);
			// >> �浹 ����

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, 1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, -1);
			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}
	}
	// Player Control
}

void Player::MovePlayer(POINT pos[], int direction, int num, float mulNum, float addNum)
{
	if (direction == eMoveLeft || direction == eMoveRight)
	{
		for (int i = 0; i < 4; i++)
			pos[i].x += num * mulNum + addNum;
	}
	else if (direction == eMoveUp || direction == eMoveDown || playerState == eJump || playerState == eFall)
	{
		for (int i = 0; i < 4; i++)
			pos[i].y += num * mulNum + addNum;
	}
}

int Player::CheckFocusRange(int direction, int mulNum)
{
	RECT calcRect = ConversionRect(fMovePos);
	int num = 0;

	if (direction == eMoveUp || direction == eMoveDown)
	{
		for (int i = 1; i < eMoveSpeed; i++)
		{
			calcRect.top += 1 * mulNum;
			calcRect.bottom += 1 * mulNum;

			if ((calcRect.top + calcRect.bottom) * 0.5 >= focusPos[0].y && direction == eMoveUp)
				num++;
			else if ((calcRect.top + calcRect.bottom) * 0.5 <= focusPos[2].y && direction == eMoveDown)
				num++;
			else
				break;
		}
	}

	else if (direction == eMoveLeft || direction == eMoveRight)
	{
		for (int i = 1; i < eMoveSpeed; i++)
		{
			calcRect.left += 1 * mulNum;
			calcRect.right += 1 * mulNum;

			if ((calcRect.left + calcRect.right) * 0.5 >= focusPos[0].x && direction == eMoveLeft)
				num++;
			else if ((calcRect.left + calcRect.right) * 0.5 <= focusPos[2].x && direction == eMoveRight)
				num++;
			else
				break;
		}
	}

	return num;
}

bool Player::CheckTileMap(TileMap tile)
{
	if (moveDirection == eMoveDown && tile.type == eMapHalfBlock)
		return false;

	if (tile.type == eMapBlock || tile.type == eMapHalfBlock
		|| tile.type == eMapGate_0 || tile.type == eMapGate_1 || tile.type == eMapGate_2 || tile.type == eMapGate_3 || tile.type == eMapBtn_2 || tile.type == eMapBtn_3
		|| tile.type == eMapGateCloseHorizen || tile.type == eMapGateCloseVertical)	// ��ϰ� �浹�� ��� ����
		return true;
	else
		return false;
}

void Player::SetPos(POINT pos[], int xPos, int yPos, int addNum)
{
	POINT temp;
	for (int i = 0; i < 4; i++)
	{
		temp.x = addNum;
		temp.y = addNum;
		if (i == 0)
		{
			temp.x *= -1;
			temp.y *= -1;
		}
		else if (i == 1)
		{
			temp.x *= 1;
			temp.y *= -1;
		}
		else if (i == 2)
		{
			temp.x *= 1;
			temp.y *= 1;
		}
		else if (i == 3)
		{
			temp.x *= -1;
			temp.y *= 1;
		}

		pos[i].x = xPos + temp.x;
		pos[i].y = yPos + temp.y;

	}
}

inline void Player::CalcCenterPos()
{
	centerPos.x = (playerPos[0].x + playerPos[1].x) / 2;
	centerPos.y = (playerPos[0].y + playerPos[2].y) / 2;
}

inline void Player::CalcFCenterPos()
{
	fCenterPos.x = (fMovePos[0].x + fMovePos[1].x) / 2;
	fCenterPos.y = (fMovePos[0].y + fMovePos[2].y) / 2;
}

RECT Player::ConversionRect(POINT pos[])
{
	RECT conversion;

	conversion.left = pos[0].x;
	conversion.top = pos[0].y;
	conversion.right = pos[2].x;
	conversion.bottom = pos[2].y;

	return conversion;
}

void Player::ReturnLastPos()
{
	playerPos[0] = lastPlayerPos[0];
	playerPos[1] = lastPlayerPos[1];
	playerPos[2] = lastPlayerPos[2];
	playerPos[3] = lastPlayerPos[3];

	lastMoveCenter.x = 0;
	lastMoveCenter.y = 0;

	playerState = eIdle;
}

void Player::Reset()
{
	SetPos(playerPos, dMap->GetResenSpot().x, dMap->GetResenSpot().y, ePlayerSize);

	playerState = eIdle;
	isJump = false;
	jumpPower = 0;
	
	isBtmGround = false;
	
	moveDirection = eIdle;
	moveSpeed = eMoveSpeed;
	gravity = eGravity;
	
	isCharging = false;
	focusLv = dGameManager->GetFocusLv();
	SetFocusGauge();
	
	CalcCenterPos();
	
	SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
	SetPos(fMovePos, centerPos.x, centerPos.y, ePlayerSize);
	CalcFCenterPos();
	
	SetPos(lastPlayerPos, centerPos.x, centerPos.y, ePlayerSize);
	lastMoveCenter.x = 0;
	lastMoveCenter.y = 0;

	isRightSight = false;
	nowFrame = 0;
	isEndAni = false;

	timer = 0;
	countDownSec = dCountDown;
}


RECT Player::GetPlayerPos()
{
	return ConversionRect(playerPos);
}

RECT Player::GetFocusPos()
{
	return ConversionRect(focusPos);
}

bool Player::GetIsFocusMode()
{
	if (playerState == eFocus)
		return true;
	else
		return false;
}

bool Player::GetIsEndAni()
{
	return isEndAni;
}

void Player::SetIsPlayerDead(bool set)
{
	if (set == true)
	{
		gravity = eGravity;	
		// >> focus ��忡�� ������ �߷� x�̹Ƿ� ����

		for (int i = 0; i < 4; i++)
			focusPos[i] = { 0,0 };
		// >> ������ ���� �Ⱥ��̰�

		dSoundSys->PlayDeadSound();
		playerState = eDead;
	}
	else 
		playerState = eIdle;

	dSoundSys->SetIsPause(set);
}

bool Player::GetIsPlayerDead()
{
	if (playerState == eDead)
		return true;
	else
		return false;
}

void Player::SetFocusLv()
{
	focusLv++;
	SetFocusGauge();
}

int Player::GetFocusLv()
{
	return focusLv;
}

void Player::SetFocusGauge()
{
	focusGauge = 0;

	if (focusLv == 0)
		focusGauge = 0;
	else
	{
		for (int i = 0; i < focusLv; i++)
			focusGauge += 75;
	}
	maxFocusGauge = focusGauge;
}
