#include "stdafx.h"
#include "PlayerClass.h"
#include "BulletClass.h"
#include "CannonClass.h"

#define hitRange 15
#define countDown 3
#define shootedDown 2
#define hitRangeDown 1
#define tShotSpeed 10

#define degree 90

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

	shotCd = { centerPos.x - 16, centerPos.y - 24, centerPos.x + 16, centerPos.y - 16 };

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
	CheckInPlayer();
}

void Cannon::DrawObject(HDC hdc)
{

	if (isShooted)
	{
		SelectObject(hdc, GetStockObject(GRAY_BRUSH));
		Rectangle(hdc, shotCd.left, shotCd.top, shotCd.right, shotCd.bottom);	// test_shootdGage
	}

	if (isPlayer)
	{
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, shotCd.left, shotCd.top, shotCd.right, shotCd.bottom);	// test_shootdGage
	}
	
	// SelectObject(hdc, GetStockObject(NULL_BRUSH));
	// Rectangle(hdc, hitRect.left, hitRect.top, hitRect.right, hitRect.bottom);	// test
	// Rectangle(hdc, testShot.left, testShot.top, testShot.right, testShot.bottom);	// test_testShot
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

		// POINT lastVec;
		// lastVec.x = nextSpot.x;
		// lastVec.y = nextSpot.y;
		// 탄의 원래 속도 벡터를 저장

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

		// float rad = M_PI / 180 * degree;
		// POINT nextSpot2;
		// nextSpot2.x = cos(rad)*lastVec.x - sin(rad)*lastVec.y;
		// nextSpot2.y = sin(rad)*lastVec.x + cos(rad)*lastVec.y;
		// // >> 시계방향으로 선회할 때의 상한 각도에 해당하는 속도 벡터 계산
		// 
		// // >> 캐릭터 방향 선회인지 제한각도만큼만 선회인지 계산
		// if (lastVec.x * nextSpot.x + lastVec.y * nextSpot.y >= lastVec.x * nextSpot2.x + lastVec.y * nextSpot2.y)
		// {
		// 	 // >> 캐릭터가 선회 가능한 범위 일 경우 캐릭터 방향으로 선회
		// 	 nextSpot.x = nextSpot.x;
		// 	 nextSpot.y = nextSpot.y;
		// }
		// else
		// {
		// 	 // >> 캐릭터가 선회 가능한 범위 밖에 있을 경우
		// 
		// 	 POINT nextSpot3;
		// 	 nextSpot3.x = cos(rad) * lastVec.x + sin(rad) * lastVec.y;
		// 	 nextSpot3.y = -sin(rad) * lastVec.x + cos(rad) * lastVec.y;
		// 	 // >> 시계 반대방향으로 선회할 때의 상한 각도에 해당하는 속도벡터 계산
		// 
		// 	 POINT posVec;
		// 	 posVec.x = playerCenter.x - centerPos.x;
		// 	 posVec.y = playerCenter.y - centerPos.y;
		// 	 // >> 총알에서 캐릭터까지의 상대위치벡터 계산
		// 
		// 	 // >> 시계방향 선회인지 반시계방향 선회인지 계산
		// 	 if (posVec.x * nextSpot2.x + posVec.y * nextSpot2.y >= posVec.x * nextSpot3.x + posVec.y * nextSpot3.y)
		// 	 {
		// 		 // >> 시계방향 선회
		// 		 nextSpot.x = nextSpot2.x;
		// 		 nextSpot.x = nextSpot2.y;
		// 	 }
		// 	 else
		// 	 {
		// 		 // 반시계방향 선회
		// 		 nextSpot.x = nextSpot3.x;
		// 		 nextSpot.x = nextSpot3.y;
		// 	 }
		// }

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
			// 발사 경고 후 발사
			isShooted = false;
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
		if (IntersectRect(&area, &dMapPos[i].pos, &testShot) && dMapPos[i].type == eMapBlock)
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

void Cannon::Reset()
{
	nextSpot.x = 0;
	nextSpot.y = 0;

	testShot.left = centerPos.x - eBlockSize * 0.5;
	testShot.top = centerPos.y - eBlockSize * 0.5;
	testShot.right = centerPos.x + eBlockSize * 0.5;
	testShot.bottom = centerPos.y + eBlockSize * 0.5;

	shotCd = { centerPos.x - 16, centerPos.y - 24, centerPos.x + 16, centerPos.y - 16 };

	tempCenter.x = -1;
	tempCenter.y = -1;

	isPlayer = false;
}
