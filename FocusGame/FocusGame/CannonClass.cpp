#include "stdafx.h"
#include "PlayerClass.h"
#include "CannonClass.h"

#define hitRange 15
#define largeHitRange 10
#define dCountdown 2
#define hitCountdown 5

#define dGameManager GameManager::GetInstance()

Cannon::Cannon()
{
	hitRect.left = 0;
	hitRect.top = 0;
	hitRect.right = 0;
	hitRect.bottom = 0;

	centerPos.x = 0;
	centerPos.y = 0;
}

Cannon::Cannon(POINT set)
{
	hitRect.left = set.x - eBlockSize * hitRange;
	hitRect.top = 0;
	hitRect.right = set.x + eBlockSize * hitRange;
	hitRect.bottom = eTrueWinHeight;

	centerPos.x = set.x;
	centerPos.y = set.y;

	timer = tmTime->tm_sec;
	countDownSec = dCountdown;

	testShot.left = set.x - eBlockSize * 0.5;
	testShot.top = set.y - eBlockSize * 0.5;
	testShot.right = set.x + eBlockSize * 0.5;
	testShot.bottom = set.y + eBlockSize * 0.5;

	tempSpot.x = -1;
	tempSpot.y = -1;
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
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, hitRect.left, hitRect.top, hitRect.right, hitRect.bottom);	// test

	Rectangle(hdc, testShot.left, testShot.top, testShot.right, testShot.bottom);	// test_testShot
}

void Cannon::CheckInPlayer()
{
	RECT area;
	vector<TileMap> mapPos = dGameManager->GetNowMap();
	RECT playerPos = dGameManager->GetNowPlayerPos();

	static bool isCanShoot = false;
	if (IntersectRect(&area, &hitRect, &playerPos))// && !isCanShoot)
	{
		// todo : 벽 판정 필요 -> 테스트 총알 발사로 확인?
		POINT addSpot;
		addSpot.x = 0;
		addSpot.y = 0;
		POINT playerCenter;
		playerCenter.x = (playerPos.left + playerPos.right) * 0.5;
		playerCenter.y = (playerPos.top + playerPos.bottom) * 0.5;

		static POINT tempPlayer;	// 임시
		if (tempSpot.x == -1 && tempSpot.y == -1)
		{
			tempSpot = centerPos;
			tempPlayer = playerCenter;
		}

		 // if (tempSpot.x == -1 && tempSpot.y == -1)
		 // 	tempSpot = centerPos;
		 
		 // float calc = sqrt(float(tempPlayer.x - tempSpot.x) * float(tempPlayer.x - tempSpot.x) + float(tempPlayer.y - tempSpot.y)*float(tempPlayer.y - tempSpot.y));
		 float calc = sqrt(float(playerCenter.x - tempSpot.x) * float(playerCenter.x - tempSpot.x) + float(playerCenter.y - tempSpot.y)*float(playerCenter.y - tempSpot.y));
		 // 두 점 사이의 거리
		 
		 // >> 
		 POINT lastVec;
		 lastVec.x = addSpot.x;
		 lastVec.y = addSpot.y;
		 // >>

		 if (calc)
		 {
		 	// addSpot.x = (tempPlayer.x - tempSpot.x) / calc * 25;	// 10
		 	// addSpot.y = (tempPlayer.y - tempSpot.y) / calc * 25;	// 10
			 addSpot.x = (playerCenter.x - tempSpot.x) / calc * 8;	// 10
			 addSpot.y = (playerCenter.y - tempSpot.y) / calc * 8;	// 10
		 }
		 else
		 {
		 	addSpot.x = 0;
		 	addSpot.y = 10;
		 }
		 // 속도 벡터

		 // >>
		 float rad = 3.14 / 180 * 45;
		 float vx2 = cos(rad)*lastVec.x - sin(rad)*lastVec.y;
		 float vy2 = sin(rad)*lastVec.x + cos(rad)*lastVec.y;

		 if (lastVec.x * addSpot.x + lastVec.y * addSpot.y >= lastVec.x * vx2 + lastVec.y * vy2)
		 {
			 addSpot.x = addSpot.x;
			 addSpot.y = addSpot.y;
		 }
		 else
		 {
			 float vx3 = cos(rad)*lastVec.x + sin(rad)*lastVec.y;
			 float vy3 = -sin(rad)*lastVec.x + cos(rad)*lastVec.y;

			 float px = playerCenter.x - centerPos.x;
			 float py = playerCenter.y - centerPos.y;

			 if (px*vx2 + py*vy2 >= px*vx3 + py*vy3)
			 {
				 addSpot.x = vx2;
				 addSpot.x = vy2;
			 }
			 else
			 {
				 addSpot.x = vx3;
				 addSpot.x = vy3;
			 }
		 }
		 // >>

		tempSpot.x += addSpot.x;
		tempSpot.y += addSpot.y;

		//static POINT tempPlayer;	// 임시
		//if (tempSpot.x == -1 && tempSpot.y == -1)
		//{
		//	tempSpot = centerPos;
		//	tempPlayer = playerCenter;
		//}

		//if (playerCenter.x > tempSpot.x) // (tempPlayer.x > tempSpot.x)
		//	tempSpot.x += 8;
		//else if (playerCenter.x < tempSpot.x) // (tempPlayer.x < tempSpot.x)
		//	tempSpot.x -= 8;
		////else
		////	tempSpot.x = 0;

		//if (playerCenter.y > tempSpot.y) // (tempPlayer.y > tempSpot.y)
		//	tempSpot.y += 8;
		//else if (playerCenter.y < tempSpot.y) // (tempPlayer.y < tempSpot.y)
		//	tempSpot.y -= 8;
		////else
		////	tempSpot.y = 0;

		testShot.left = tempSpot.x - eBlockSize * 0.5;
		testShot.top = tempSpot.y - eBlockSize * 0.5;
		testShot.right = tempSpot.x + eBlockSize * 0.5;
		testShot.bottom = tempSpot.y + eBlockSize * 0.5;

		for (int i = 0; i < mapPos.size(); i++)
		{
			if (IntersectRect(&area, &mapPos[i].pos, &testShot) && mapPos[i].type == eMapBlock)
			{
				printf("hit");
				testShot.left = centerPos.x - eBlockSize * 0.5;
				testShot.top = centerPos.y - eBlockSize * 0.5;
				testShot.right = centerPos.x + eBlockSize * 0.5;
				testShot.bottom = centerPos.y + eBlockSize * 0.5;

				tempSpot.x = -1;
				tempSpot.y = -1;


				break;
			}

			if (IntersectRect(&area, &playerPos, &testShot))
			{
				printf("playerHit");
				isCanShoot = true;

				testShot.left = centerPos.x - eBlockSize * 0.5;
				testShot.top = centerPos.y - eBlockSize * 0.5;
				testShot.right = centerPos.x + eBlockSize * 0.5;
				testShot.bottom = centerPos.y + eBlockSize * 0.5;

				tempSpot.x = -1;
				tempSpot.y = -1;

				break;
			}
			
			// if()
		}

		// >> countdownTimer
		if (isCanShoot)
		{
			time(&nowTime);
			tmTime = localtime(&nowTime);

			if (timer != tmTime->tm_sec)
			{
				timer = tmTime->tm_sec;
				countDownSec--;
				printf("testTime\n");
			}

			if (countDownSec == 0)
			{
				// countDownSec = dCountdown;
				countDownSec = hitCountdown;
				isCanShoot = false;
				printf("카운트 타임 오버\n");
			}
		}
		// >> countdownTimer
	}

	else
	{
		testShot.left = centerPos.x - eBlockSize * 0.5;
		testShot.top = centerPos.y - eBlockSize * 0.5;
		testShot.right = centerPos.x + eBlockSize * 0.5;
		testShot.bottom = centerPos.y + eBlockSize * 0.5;

		tempSpot.x = -1;
		tempSpot.y = -1;
	}

	return;
}
