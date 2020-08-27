#include "stdafx.h"
#include "PlayerClass.h"
#include "CannonClass.h"

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
		POINT playerCenter;
		playerCenter.x = (playerPos.left + playerPos.right) * 0.5;
		playerCenter.y = (playerPos.top + playerPos.bottom) * 0.5;

		float length = sqrt(pow(playerCenter.x - centerPos.x, 2) + pow(playerCenter.y - centerPos.y, 2));

		int addX = -1, addY = -1;
		RECT area;
		POINT checkSpot = centerPos;

		while (addX !=0)
		{
			if (playerCenter.x > checkSpot.x) addX = 1;
			else if (playerCenter.x < checkSpot.x) addX = -1;
			else addX = 0;

			checkSpot.x += addX;

			for (int i = 0; i < mapPos.size(); i++)
			{
				if (PtInRect(&mapPos[i].pos, checkSpot) && checkSpot.x != playerCenter.x && checkSpot.y != playerCenter.y)
				{
						printf("---------------------------------------------------------------%d\n", i);
				}
			}
		}

		checkSpot = centerPos;
		while (addY != 0)
		{
			if (playerCenter.y > checkSpot.y) addY = 1;
			else if (playerCenter.y < checkSpot.y) addY = -1;
			else addY = 0;

			checkSpot.y += addY;

			for (int i = 0; i < mapPos.size(); i++)
			{
				if (PtInRect(&mapPos[i].pos, checkSpot) && checkSpot.x != playerCenter.x && checkSpot.y != playerCenter.y)
				{
					printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++%d\n", i);
				}
			}
		}
	}
	return;
}
