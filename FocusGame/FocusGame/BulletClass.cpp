#include "stdafx.h"
#include "BulletClass.h"

#define maxCnt 10

Bullet::Bullet()
{
	BulletSctruct nBullet;
	BulletSctruct hBullet;

	for (int i = 0; i < maxCnt; i++)
	{
		nBullet.centerPos.x = 0;
		nBullet.centerPos.y = 0;
		nBullet.isShot = false;
		nBullet.type = dNormal;
		nBulletList.push_back(nBullet);

		hBullet.centerPos.x = 0;
		hBullet.centerPos.y = 0;
		hBullet.isShot = false;
		hBullet.type = dNormal;
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

}

void Bullet::DrawObject(HDC hdc)
{
	RECT tempRect;

	for (int i = 0; i < maxCnt; i++)
	{
		if (nBulletList[i].isShot == true)
		{
			tempRect = ConversionRect(nBulletList[i].centerPos);
			Rectangle(hdc, tempRect.left, tempRect.top, tempRect.right, tempRect.bottom);
		}

		if (hBulletList[i].isShot == true)
		{
			tempRect = ConversionRect(hBulletList[i].centerPos);
			Rectangle(hdc, tempRect.left, tempRect.top, tempRect.right, tempRect.bottom);
		}
	}
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