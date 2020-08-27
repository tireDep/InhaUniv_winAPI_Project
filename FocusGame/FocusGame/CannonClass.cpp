#include "stdafx.h"
#include "PlayerClass.h"
#include "CannonClass.h"

#define dCountdown 2

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

	bool isCanShoot = false;
	if (IntersectRect(&area, &hitRect, &playerPos))
	{
		// todo : 벽 판정 필요 -> 테스트 총알 발사로 확인?
		POINT addSpot;
		POINT playerCenter;
		playerCenter.x = (playerPos.left + playerPos.right) * 0.5;
		playerCenter.y = (playerPos.top + playerPos.bottom) * 0.5;

		// if (tempSpot.x == -1 && tempSpot.y == -1)
		// 	tempSpot = centerPos;
		// 
		// float calc = sqrt(float(playerCenter.x - tempSpot.x) * float(playerCenter.x - tempSpot.x) + float(playerCenter.y - tempSpot.y)*float(playerCenter.y - tempSpot.y));
		// 
		// if (calc)
		// {
		// 	addSpot.x = (playerCenter.x - tempSpot.x) / calc * 5;
		// 	addSpot.y = (playerCenter.y - tempSpot.y) / calc * 5;
		// }
		// else
		// {
		// 	addSpot.x = 0;
		// 	addSpot.y = 5;
		// }

		// tempSpot.x += addSpot.x;
		// tempSpot.y += addSpot.y;

		static POINT tempPlayer;	// 임시
		if (tempSpot.x == -1 && tempSpot.y == -1)
		{
			tempSpot = centerPos;
			tempPlayer = playerCenter;
		}

		if (playerCenter.x > tempSpot.x) // (tempPlayer.x > tempSpot.x)
			tempSpot.x += 5;
		else if (playerCenter.x < tempSpot.x) // (tempPlayer.x < tempSpot.x)
			tempSpot.x -= 5;
		//else
		//	tempSpot.x = 0;

		if (playerCenter.y > tempSpot.y) // (tempPlayer.y > tempSpot.y)
			tempSpot.y += 5;
		else if (playerCenter.y < tempSpot.y) // (tempPlayer.y < tempSpot.y)
			tempSpot.y -= 5;
		//else
		//	tempSpot.y = 0;

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
				countDownSec = dCountdown;
				printf("카운트 타임 오버\n");
			}
		}
		// >> countdownTimer

		// >>testShot
	
		// printf("%d %d\n", playerCenter.x, playerCenter.y);
		// int arc = (int) (-atan2(abs(centerPos.y - playerCenter.y), abs(centerPos.y - playerCenter.y)) * 180) / 3.14 + 90;
		// int largex = 1, largey = -1;
		// 
		// // if (playerCenter.y - centerPos.y > 0)
		// // {
		// // 	largey = 1;
		// // 	if (playerCenter.x - centerPos.y > 0)
		// // 	{
		// // 		largex = -1;
		// // 		arc = arc + 90;
		// // 	}
		// // 	else
		// // 		arc = 270 - arc;
		// // }
		// // else
		// // {
		// // 	if (playerCenter.x - centerPos.x > 0)
		// // 	{
		// // 		largey = -1;
		// // 		arc = 90 - arc;
		// // 	}
		// // 	else
		// // 		arc = 270 + arc;
		// // }
		// 
		// // POINT homingSpot;
		// // homingSpot.x
		// // centerPos.x += sin()
		// double sinArr[360];
		// double cosArr[360];
		// 
		// for (int i = 0; i < 360; i++)
		// {
		// 	sinArr[i] = sin(i*0.0175);
		// 	cosArr[i] = cos(i*0.0175);
		// }
		// 
		// centerPos.x += sinArr[arc] * 5;
		// centerPos.y -= cosArr[arc] * 5;

		// >>testShot
	}
	return;
}
