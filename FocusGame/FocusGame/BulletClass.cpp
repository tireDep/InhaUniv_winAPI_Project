#include "stdafx.h"
#include "BulletClass.h"

Bullet::Bullet()
{

}

Bullet::~Bullet()
{

}

void Bullet::Update()
{

}

void Bullet::DrawObject(HDC hdc)
{

}

/*
float accelerate = 20.0f; // �̻����� �ӵ�

float distanceX = targetPos.x - missilePos.x;
float distanceY = targetPos.y - missilePos.y;
float distance = sqrtf(distanceX * distanceX + distanceY * distanceY);

float directionAngle = (float)(atan2(missileMove.y, missileMove.x)); // �̻����� ���ư��� ������ ����
float targetAngle = (float)(atan2(targetPos.y - missilePos.y, targetPos.x - missilePos.x)); // �̻��ϰ� ��ǥ�� ������ ����

float angleDifference; // �̻����� ���ư��� ������ ����, �̻��ϰ� ��ǥ�� ������ ������ ��
if (fabsf(targetAngle - directionAngle) < ANGLE(360) - fabsf(targetAngle - directionAngle))
angleDifference = targetAngle - directionAngle;
else
angleDifference = (targetAngle - directionAngle > 0 ? -ANGLE(360) : ANGLE(360)) + (targetAngle - directionAngle);

float homingAngle = directionAngle + (angleDifference * (1.0f / (distance * 0.045f))); // ���������� �����Ǵ� �̻����� ����
if (fabsf(homingAngle) > ANGLE(180))
homingAngle += homingAngle > 0 ? -ANGLE(360) : ANGLE(360);

missileMove.x = cos(homingAngle) * accelerate;
missileMove.y = sin(homingAngle) * accelerate; // �̻����� ����� ����
*/
