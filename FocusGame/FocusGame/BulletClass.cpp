#include "stdafx.h"
#include "PlayerClass.h"
#include "BulletClass.h"
#include "ExplodeClass.h"

#define dShotSpeed 10
#define dDegree 90
#define dMaxCnt 100

#define dGameManager GameManager::GetInstance()
#define dPlayer Player::GetInstance()

#define dMapPos dGameManager->GetNowMap()
#define dPlayerPos dGameManager->GetNowPlayerPos()

#define dExplode Explode::GetInstance()

Bullet::Bullet()
{
	BulletSctruct nBullet;
	BulletSctruct hBullet;

	for (int i = 0; i < dMaxCnt; i++)
	{
		nBullet.centerPos = { -1, -1 };
		nBullet.nextSpot = { 0, 0 };
		nBullet.shotBullet = { 0, 0 };
		nBullet.isShot = false;
		nBullet.type = dNormal;
		nBullet.speed = dShotSpeed;
		nBulletList.push_back(nBullet);

		hBullet.centerPos = { -1, -1 };
		hBullet.nextSpot = { 0, 0 };
		hBullet.shotBullet = { 0, 0 };
		hBullet.isShot = false;
		hBullet.type = dHoming;
		hBullet.speed = dShotSpeed;
		hBulletList.push_back(hBullet);
	}
}

Bullet::~Bullet()
{

}

Bullet* Bullet::GetInstance()
{
	static Bullet bullet;
	return &bullet;
}

void Bullet::Update()
{
	CheckShot();
}

void Bullet::DrawObject(HDC hdc)
{
	SelectObject(hdc, GetStockObject(BLACK_BRUSH));

	for (int i = 0; i < dMaxCnt; i++)
	{
		if (nBulletList[i].isShot == true)
			Rectangle(hdc, nBulletList[i].shotBullet.left, nBulletList[i].shotBullet.top, nBulletList[i].shotBullet.right, nBulletList[i].shotBullet.bottom);

		if (hBulletList[i].isShot == true)
			Rectangle(hdc, hBulletList[i].shotBullet.left, hBulletList[i].shotBullet.top, hBulletList[i].shotBullet.right, hBulletList[i].shotBullet.bottom);
	}
}

void Bullet::Shoot(const POINT &cannonCenter, const POINT &playerCenter, int bulletType)
{
	//>> 타입확인 및 off인 총알 탐색
	for (int i = 0; i < dMaxCnt; i++)
	{
		if (nBulletList[i].isShot == false && bulletType == nBulletList[i].type)
		{
			nBulletList[i].isShot = true;
			CalcBullet(nBulletList[i], cannonCenter, playerCenter, bulletType);
			break;
		}

		if (hBulletList[i].isShot == false && bulletType == hBulletList[i].type)
		{
			hBulletList[i].isShot = true;
			CalcBullet(hBulletList[i], cannonCenter, playerCenter, bulletType);
			break;
		}
	}
}

void Bullet::CalcBullet(BulletSctruct &bullet, const POINT &cannonCenter, const POINT &playerCenter, int bulletType)
{
	if (bulletType == dHoming || (bullet.centerPos.x == -1 && bullet.centerPos.y == -1))
	{
		POINT tempPlayerCenter;
		if (bullet.centerPos.x == -1 && bullet.centerPos.y == -1)
		{
			bullet.centerPos = cannonCenter;
			tempPlayerCenter = playerCenter;
		}

		float calc = sqrt(float(playerCenter.x - bullet.centerPos.x) * float(playerCenter.x - bullet.centerPos.x) + float(playerCenter.y - bullet.centerPos.y)*float(playerCenter.y - bullet.centerPos.y));
		// 두 점 사이의 거리

		if (calc)
		{
			bullet.nextSpot.x = (playerCenter.x - bullet.centerPos.x) / calc * bullet.speed;
			bullet.nextSpot.y = (playerCenter.y - bullet.centerPos.y) / calc * bullet.speed;
		}
		else
		{
			bullet.nextSpot.x = 0;
			bullet.nextSpot.y = bullet.speed;
		}
		// >> 캐릭터 방향으로 속도 벡터 계산
	}

	MoveShot(bullet);
}

void Bullet::CheckShot()
{
	POINT playerCenter;
	playerCenter.x = (dPlayerPos.left + dPlayerPos.right) * 0.5;
	playerCenter.y = (dPlayerPos.top + dPlayerPos.bottom) * 0.5;

	for (int i = 0; i < dMaxCnt; i++)
	{
		if (nBulletList[i].isShot == true)
			CalcBullet(nBulletList[i], nBulletList[i].centerPos, playerCenter, nBulletList[i].type); // MoveShot(nBulletList[i]);

		if (hBulletList[i].isShot == true)
		{
			CalcBullet(hBulletList[i], hBulletList[i].centerPos, playerCenter, hBulletList[i].type);
		}
	}
}

void Bullet::MoveShot(BulletSctruct &bullet)
{
	// >> 포커스 내부 판정
	float mulNum = 0;
	RECT area;
	RECT focusPos = dPlayer->GetFocusPos();
	bool isFocus = dPlayer->GetIsFocusMode();
	if (isFocus && IntersectRect(&area, &focusPos, &bullet.shotBullet))
		mulNum = 0.25;// bullet.speed = dShotSpeed * 0.5 * 0.5;
	else
		mulNum = 1; // bullet.speed = dShotSpeed;
	// >> 포커스 내부 판정

	bullet.centerPos.x += bullet.nextSpot.x * mulNum;
	bullet.centerPos.y += bullet.nextSpot.y * mulNum;

	bullet.shotBullet.left = bullet.centerPos.x - eBlockSize * 0.5;
	bullet.shotBullet.top = bullet.centerPos.y - eBlockSize * 0.5;
	bullet.shotBullet.right = bullet.centerPos.x + eBlockSize * 0.5;
	bullet.shotBullet.bottom = bullet.centerPos.y + eBlockSize * 0.5;

	CheckHit(bullet);
}

void Bullet::CheckHit(BulletSctruct &bullet)
{
	RECT area;

	for (int i = 0; i < dMapPos.size(); i++)
	{
		if (IntersectRect(&area, &dMapPos[i].pos, &bullet.shotBullet) && dMapPos[i].type == eMapBlock)
		{
			// >> 맵에 부딪힘
			dExplode->StartExplode(bullet.centerPos);
			ResetBullet(bullet);
			break;
		}

		if (IntersectRect(&area, &dPlayerPos, &bullet.shotBullet))
		{
			// >> 플레이어에 부딪힘
			dExplode->StartExplode(bullet.centerPos);
			ResetBullet(bullet);

			dGameManger->SetIsPlayerLive(false);
			// todo : 폭발 이펙트 & hit 판정
			// todo : 플레이어 사망 판정, 스테이지 리셋
			break;
		}
	}
}

void Bullet::Reset()
{
	for (int i = 0; i < dMaxCnt; i++)
	{
		ResetBullet(nBulletList[i]);
		ResetBullet(hBulletList[i]);
	}
}

void Bullet::ResetBullet(BulletSctruct & bullet)
{
	bullet.centerPos = { -1, -1 };
	bullet.nextSpot = { 0, 0 };
	bullet.shotBullet = { 0, 0 };
	bullet.isShot = false;
	bullet.speed = dShotSpeed;
}

RECT Bullet::ConversionRect(POINT pos)
{
	RECT conversion;

	conversion.left = pos.x - eBlockSize * 0.5;
	conversion.top = pos.y - eBlockSize * 0.5;
	conversion.right = pos.x + eBlockSize * 0.5;
	conversion.bottom = pos.y + eBlockSize * 0.5;

	return conversion;
}
