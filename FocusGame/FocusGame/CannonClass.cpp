#include "stdafx.h"
#include "PlayerClass.h"
#include "BulletClass.h"
#include "CannonClass.h"

#define hitRange 5
#define countDown 3
#define shootedDown 2
#define hitRangeDown 1
#define tShotSpeed 10
// todo : savedata?

#define dPlayer Player::GetInstance()
#define dGameManager GameManager::GetInstance()
#define dBulletList Bullet::GetInstance()

#define dMapPos dGameManager->GetNowMap()
#define dPlayerPos dGameManager->GetNowPlayerPos()

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
	shottedDownSec = shootedDown;

	isPlayer = false;
	checkTimer = 0;
	hitRangeSec = hitRangeDown;
}

Cannon::Cannon(parceCannon set)
{
	hitRect.left = set.pos.x - eBlockSize * hitRange;
	hitRect.top = 0;
	hitRect.right = set.pos.x + eBlockSize * hitRange;
	hitRect.bottom = eTrueWinHeight;

	centerPos.x = set.pos.x;
	centerPos.y = set.pos.y;

	shotCd = { centerPos.x - 16, centerPos.y - 30, centerPos.x + 16, centerPos.y - 20 };

	timer = tmTime->tm_sec;
	countDownSec = countDown;
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
	shottedDownSec = shootedDown;
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
	playerCenter.x = (dPlayerPos.left + dPlayerPos.right) * 0.5;
	playerCenter.y = (dPlayerPos.top + dPlayerPos.bottom) * 0.5;

	if (IntersectRect(&area, &hitRect, &dPlayerPos) && isCanShoot)
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
			hitRangeSec = hitRangeDown;
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
			shottedDownSec = shootedDown;
			dBulletList->Shoot(centerPos, playerCenter, type);
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
			countDownSec = countDown;
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
	for (int i = 0; i < dMapPos.size(); i++)
	{
		if (IntersectRect(&area, &dMapPos[i].pos, &testShot) && CheckTileMap(dMapPos[i]))
		{
			// >> 맵에 부딪힘
			isCanShoot = false;
			break;
		}

		if (IntersectRect(&area, &dPlayerPos, &testShot))
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
	if (mapTile.type == eMapBlock || mapTile.type == eMapSpike || 
		mapTile.type == eMapGate_0 || mapTile.type == eMapGate_1 || mapTile.type == eMapGate_2 || mapTile.type == eMapGate_3 ||
		mapTile.type == eMapBtn_0 || mapTile.type == eMapBtn_1 || mapTile.type == eMapBtn_2 || mapTile.type == eMapBtn_3 || 
		mapTile.type == eMapGateCloseVertical || mapTile.type == eMapGateCloseHorizen )
		return true;
	else
		return false;
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
}

