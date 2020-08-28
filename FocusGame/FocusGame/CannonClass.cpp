#include "stdafx.h"
#include "PlayerClass.h"
#include "BulletClass.h"
#include "CannonClass.h"

#define hitRange 15
#define countDown 3
#define shootedDown 2
#define tShotSpeed 10

#define degree 90

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
	shottedDownSec = shootedDown;
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
	isCanShoot = true;

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
	SelectObject(hdc, GetStockObject(NULL_BRUSH));

	if (isShooted)
		Rectangle(hdc, shotCd.left, shotCd.top, shotCd.right, shotCd.bottom);	// test_shootdGage

	Rectangle(hdc, hitRect.left, hitRect.top, hitRect.right, hitRect.bottom);	// test

	Rectangle(hdc, testShot.left, testShot.top, testShot.right, testShot.bottom);	// test_testShot
}

void Cannon::CheckInPlayer()
{
	RECT area;
	vector<TileMap> mapPos = dGameManager->GetNowMap();
	RECT playerPos = dGameManager->GetNowPlayerPos();

	POINT playerCenter;
	playerCenter.x = (playerPos.left + playerPos.right) * 0.5;
	playerCenter.y = (playerPos.top + playerPos.bottom) * 0.5;

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
			// �߻� ��� �� �߻�
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

	if (IntersectRect(&area, &hitRect, &playerPos) && isCanShoot)
	{
		POINT tempPlayerCenter;
		if (tempCenter.x == -1 && tempCenter.y == -1)
		{
			tempCenter = centerPos;
			tempPlayerCenter = playerCenter;
		}

		 // float calc = sqrt(float(tempPlayerCenter.x - tempCenter.x) * float(tempPlayerCenter.x - tempCenter.x) + float(tempPlayerCenter.y - tempCenter.y)*float(tempPlayerCenter.y - tempCenter.y));
		 // �Ϲ�
		 float calc = sqrt(float(playerCenter.x - tempCenter.x) * float(playerCenter.x - tempCenter.x) + float(playerCenter.y - tempCenter.y)*float(playerCenter.y - tempCenter.y));
		 // ����
		 // �� �� ������ �Ÿ�
		 
		 POINT lastVec;
		 lastVec.x = nextSpot.x;
		 lastVec.y = nextSpot.y;
		 // ź�� ���� �ӵ� ���͸� ����

		 if (calc)
		 {
		 	 // nextSpot.x = (tempPlayerCenter.x - tempCenter.x) / calc * tShotSpeed;
		 	 // nextSpot.y = (tempPlayerCenter.y - tempCenter.y) / calc * tShotSpeed;
			 // �Ϲ�
			 nextSpot.x = (playerCenter.x - tempCenter.x) / calc * tShotSpeed;
			 nextSpot.y = (playerCenter.y - tempCenter.y) / calc * tShotSpeed;
			 // ����
		 }
		 else
		 {
		 	nextSpot.x = 0;
		 	nextSpot.y = tShotSpeed;
		 }
		 // >> ĳ���� �������� �ӵ� ���� ���

		 float rad = M_PI / 180 * degree;
		 POINT nextSpot2;
		 nextSpot2.x = cos(rad)*lastVec.x - sin(rad)*lastVec.y;
		 nextSpot2.y = sin(rad)*lastVec.x + cos(rad)*lastVec.y;
		 // >> �ð�������� ��ȸ�� ���� ���� ������ �ش��ϴ� �ӵ� ���� ���

		 // >> ĳ���� ���� ��ȸ���� ���Ѱ�����ŭ�� ��ȸ���� ���
		 if (lastVec.x * nextSpot.x + lastVec.y * nextSpot.y >= lastVec.x * nextSpot2.x + lastVec.y * nextSpot2.y)
		 {
			 // >> ĳ���Ͱ� ��ȸ ������ ���� �� ��� ĳ���� �������� ��ȸ
			 nextSpot.x = nextSpot.x;
			 nextSpot.y = nextSpot.y;
		 }
		 else
		 {
			 // >> ĳ���Ͱ� ��ȸ ������ ���� �ۿ� ���� ���

			 POINT nextSpot3;
			 nextSpot3.x = cos(rad) * lastVec.x + sin(rad) * lastVec.y;
			 nextSpot3.y = -sin(rad) * lastVec.x + cos(rad) * lastVec.y;
			 // >> �ð� �ݴ�������� ��ȸ�� ���� ���� ������ �ش��ϴ� �ӵ����� ���

			 POINT posVec;
			 posVec.x = playerCenter.x - centerPos.x;
			 posVec.y = playerCenter.y - centerPos.y;
			 // >> �Ѿ˿��� ĳ���ͱ����� �����ġ���� ���

			 // >> �ð���� ��ȸ���� �ݽð���� ��ȸ���� ���
			 if (posVec.x * nextSpot2.x + posVec.y * nextSpot2.y >= posVec.x * nextSpot3.x + posVec.y * nextSpot3.y)
			 {
				 // >> �ð���� ��ȸ
				 nextSpot.x = nextSpot2.x;
				 nextSpot.x = nextSpot2.y;
			 }
			 else
			 {
				 // �ݽð���� ��ȸ
				 nextSpot.x = nextSpot3.x;
				 nextSpot.x = nextSpot3.y;
			 }
		 }
		 // >>

		 MoveTestShot();

		for (int i = 0; i < mapPos.size(); i++)
		{
			if (IntersectRect(&area, &mapPos[i].pos, &testShot) && mapPos[i].type == eMapBlock)	
			{
				// >> �ʿ� �ε���
				isCanShoot = false;
				break;
			}

			if (IntersectRect(&area, &playerPos, &testShot))
			{
				// >> �÷��̾ �ε���
				isCanShoot = false;
				isShooted = true;
				break;
			}
			
			// if()
		}
	}

	else
		ResetTestShot();

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
}

void Cannon::ResetTestShot()
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
}
