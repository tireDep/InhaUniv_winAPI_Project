#include "stdafx.h"
#include "PlayerClass.h"
#include "BulletClass.h"
#include "CannonClass.h"
#include "MapClass.h"

#define dHitRange 5
#define dShootedDown 2
#define dHitRangeDown 1
#define dTShotSpeed 15

#define dCalcLv 10

#define dMap Map::GetInstance()
#define dPlayer Player::GetInstance()
#define dGameManager GameManager::GetInstance()
#define dBulletList Bullet::GetInstance()

Cannon::Cannon()
{
	hitRect.left = 0;
	hitRect.top = 0;
	hitRect.right = 0;
	hitRect.bottom = 0;

	centerPos.x = 0;
	centerPos.y = 0;

	shotCd = { 0,0,0,0 };

	tmTime = localtime(&nowTime);

	type = 0;
	isShooted = false;
	shootedTimer = 0;
	shottedDownSec = dShootedDown;

	isPlayer = false;
	checkTimer = 0;
	hitRangeSec = CalcSetValue(dHitRangeDown);
}

Cannon::Cannon(ParceCannonStruct set)
{
	hitRange = CalcSetValue(dHitRange);
	hitRect.left = set.pos.x - eBlockSize * hitRange;
	hitRect.top = 0;
	hitRect.right = set.pos.x + eBlockSize * hitRange;
	hitRect.bottom = eTrueWinHeight;

	centerPos.x = set.pos.x;
	centerPos.y = set.pos.y;

	shotCd = { centerPos.x - 16, centerPos.y - 30, centerPos.x + 16, centerPos.y - 20 };

	timer = tmTime->tm_sec;
	countDownSec = dShootedDown;
	// countDownSec = CalcSetValue(dShootedDown);
	isCanShoot = false;

	testShot.left = set.pos.x - eBlockSize * 0.5;
	testShot.top = set.pos.y - eBlockSize * 0.5;
	testShot.right = set.pos.x + eBlockSize * 0.5;
	testShot.bottom = set.pos.y + eBlockSize * 0.5;

	tmTime = localtime(&nowTime);

	if (set.type == dNormal)
		type = dNormal;
	else
		type = dHoming;

	isShooted = false;
	shootedTimer = 0;
	shottedDownSec = dShootedDown;
	// shottedDownSec = CalcSetValue(dShootedDown);

	tShotSpeed = CalcSetValue(dTShotSpeed);
}

Cannon::~Cannon()
{

}

void Cannon::Update()
{
	if (!dPlayer->GetIsPlayerDead() && dGameManager->GetIsPlayerLive())
		CheckInPlayer();
	else
		Reset();
}

void Cannon::DrawObject(HDC hdc)
{
	if (dGameManager->GetDrawRect())
	{
		SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Rectangle(hdc, hitRect.left, hitRect.top, hitRect.right, hitRect.bottom);	// test
		Rectangle(hdc, testShot.left, testShot.top, testShot.right, testShot.bottom);	// test_testShot
	}

	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	hPen = CreatePen(PS_SOLID, 1, RGB(36, 36, 36));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	if (isShooted)
	{
		hBrush = CreateSolidBrush(RGB(128, 42, 42));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, shotCd.left, shotCd.top, shotCd.right, shotCd.bottom);

		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}

	if (isPlayer)
	{
		hBrush = CreateSolidBrush(RGB(16, 16, 16));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		Rectangle(hdc, shotCd.left, shotCd.top, shotCd.right, shotCd.bottom);

		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}
	
	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
}

void Cannon::RenderObject(HWND hWnd, HDC hdc)
{
	
}

void Cannon::CheckInPlayer()
{
	RECT area;
	POINT playerCenter;
	playerCenter.x = (dPlayer->GetPlayerPos().left + dPlayer->GetPlayerPos().right) * 0.5;
	playerCenter.y = (dPlayer->GetPlayerPos().top + dPlayer->GetPlayerPos().bottom) * 0.5;

	if (IntersectRect(&area, &hitRect, &dPlayer->GetPlayerPos()) && isCanShoot)
	{
		POINT tempPlayerCenter;
		if (tempCenter.x == -1 && tempCenter.y == -1)
		{
			isPlayer = true;
			tempCenter = centerPos;
			tempPlayerCenter = playerCenter;
		}

		float calc = sqrt(float(playerCenter.x - tempCenter.x) * float(playerCenter.x - tempCenter.x) + float(playerCenter.y - tempCenter.y)*float(playerCenter.y - tempCenter.y));
		// 두 점 사이의 거리

		if (calc)
		{
			nextSpot.x = (playerCenter.x - tempCenter.x) / calc * tShotSpeed;
			nextSpot.y = (playerCenter.y - tempCenter.y) / calc * tShotSpeed;
		}
		else
		{
			nextSpot.x = 0;
			nextSpot.y = tShotSpeed;
		}
		// >> 캐릭터 방향으로 속도 벡터 계산

		MoveTestShot();
	}

	else
		Reset();

	if (isPlayer)
	{
		time(&nowTime);
		tmTime = localtime(&nowTime);

		if (checkTimer!= tmTime->tm_sec)
		{
			checkTimer = tmTime->tm_sec;
			hitRangeSec--;
		}

		if (hitRangeSec == 0)
		{
			hitRangeSec = CalcSetValue(dHitRangeDown);
			isPlayer = false;
		}
	}


	if (isShooted)
	{
		time(&nowTime);
		tmTime = localtime(&nowTime);

		if (shootedTimer != tmTime->tm_sec)
		{
			shootedTimer = tmTime->tm_sec;
			shottedDownSec--;
		}

		if (shottedDownSec == 0)
		{
			shottedDownSec = dShootedDown;
			dBulletList->Shoot(hitRect, centerPos, playerCenter, type);
			isShooted = false;
			// 발사 경고 후 발사
		}
	}


	if (!isCanShoot)
	{
		time(&nowTime);
		tmTime = localtime(&nowTime);

		if (timer != tmTime->tm_sec)
		{
			timer = tmTime->tm_sec;
			countDownSec--;
		}

		if (countDownSec == 0)
		{
			countDownSec = dShootedDown;
			isCanShoot = true;
		}
	}
	// >> countdownTimer

	return;
}

void Cannon::MoveTestShot()
{
	tempCenter.x += nextSpot.x;
	tempCenter.y += nextSpot.y;

	testShot.left = tempCenter.x - eBlockSize * 0.5;
	testShot.top = tempCenter.y - eBlockSize * 0.5;
	testShot.right = tempCenter.x + eBlockSize * 0.5;
	testShot.bottom = tempCenter.y + eBlockSize * 0.5;

	CheckHit();
}

void Cannon::CheckHit()
{
	RECT area;
	for (int i = 0; i < dMap->GetMapPos().size(); i++)
	{
		if (IntersectRect(&area, &dMap->GetMapPos()[i].pos, &testShot) && CheckTileMap(dMap->GetMapPos()[i]))
		{
			// >> 맵에 부딪힘
			isCanShoot = false;
			break;
		}

		if (IntersectRect(&area, &dPlayer->GetPlayerPos(), &testShot))
		{
			// >> 플레이어에 부딪힘
			isCanShoot = false;
			isShooted = true;
			break;
		}
	}
}

bool Cannon::CheckTileMap(TileMap mapTile)
{
	if (mapTile.type == eMapBlock || mapTile.type == eMapHalfBlock || mapTile.type == eMapSpike || 
		mapTile.type == eMapGate_0 || mapTile.type == eMapGate_1 || mapTile.type == eMapGate_2 || mapTile.type == eMapGate_3 ||
		mapTile.type == eMapBtn_0 || mapTile.type == eMapBtn_1 || mapTile.type == eMapBtn_2 || mapTile.type == eMapBtn_3 || 
		mapTile.type == eMapGateCloseVertical || mapTile.type == eMapGateCloseHorizen )
		return true;
	else
		return false;
}

RECT Cannon::GetHitRect()
{
	return hitRect;
}

void Cannon::Reset()
{
	nextSpot.x = 0;
	nextSpot.y = 0;

	testShot.left = centerPos.x - eBlockSize * 0.5;
	testShot.top = centerPos.y - eBlockSize * 0.5;
	testShot.right = centerPos.x + eBlockSize * 0.5;
	testShot.bottom = centerPos.y + eBlockSize * 0.5;

	shotCd = { centerPos.x - 16, centerPos.y - 30, centerPos.x + 16, centerPos.y - 20 };

	tempCenter.x = -1;
	tempCenter.y = -1;

	isPlayer = false;

	if (dPlayer->GetIsPlayerDead() && !dGameManager->GetIsPlayerLive())
		isShooted = false;

	countDownTime = dShootedDown;
	shootedDownTime = dShootedDown;
	hitRange = CalcSetValue(dHitRange);
	hitRangeDownTime = CalcSetValue(dHitRangeDown);
	tShotSpeed = CalcSetValue(dTShotSpeed);
}

int Cannon::CalcSetValue(int set)
{
	int calcNum = dGameManager->GetNowStage();

	int result = 0;
	bool isMinus = false;
	switch (set)
	{
	case dTShotSpeed:
		result = dTShotSpeed;
		break;

	case dHitRange:
		result = dHitRange;
		break;
		
	case dHitRangeDown:
		result = dHitRangeDown;
		isMinus = true;
		break;
	}

	while (calcNum / dCalcLv > 0)
	{
		if (calcNum % dCalcLv > 0 && !isMinus)
			result += 5;
		else if (calcNum % dCalcLv > 0 && isMinus && result > 0)
			result -= 1;

		calcNum = calcNum / dCalcLv;
	}

	return result;
}

