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

	testShot = hitRect;
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
}

void Cannon::CheckInPlayer()
{
	RECT area;
	vector<TileMap> mapPos = dGameManager->GetNowMap();
	RECT playerPos = dGameManager->GetNowPlayerPos();

	if (IntersectRect(&area, &hitRect, &playerPos))
	{
		// todo : 벽 판정 필요 -> 테스트 총알 발사로 확인?
		
		// >> countdownTimer
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
		// >> countdownTimer
	}
	return;
}
