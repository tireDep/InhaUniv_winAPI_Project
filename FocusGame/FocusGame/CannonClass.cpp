#include "stdafx.h"
#include "CannonClass.h"

Cannon::Cannon()
{
	hitRect.left = 0;
	hitRect.top = 0;
	hitRect.right = 0;
	hitRect.bottom = 0;
}

Cannon::Cannon(int set)
{
	hitRect.left = set - eBlockSize * hitRange;
	hitRect.top = 0;
	hitRect.right = set + eBlockSize * hitRange;
	hitRect.bottom = eTrueWinHeight;
}

Cannon::~Cannon()
{

}

void Cannon::Update()
{

}

void Cannon::DrawObject(HDC hdc)
{
	Rectangle(hdc, hitRect.left, hitRect.top, hitRect.right, hitRect.bottom);	// test
}