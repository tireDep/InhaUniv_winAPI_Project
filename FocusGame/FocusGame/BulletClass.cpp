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
float accelerate = 20.0f; // 미사일의 속도

float distanceX = targetPos.x - missilePos.x;
float distanceY = targetPos.y - missilePos.y;
float distance = sqrtf(distanceX * distanceX + distanceY * distanceY);

float directionAngle = (float)(atan2(missileMove.y, missileMove.x)); // 미사일이 나아가는 방향의 각도
float targetAngle = (float)(atan2(targetPos.y - missilePos.y, targetPos.x - missilePos.x)); // 미사일과 목표점 사이의 각도

float angleDifference; // 미사일이 나아가는 방향의 각도, 미사일과 목표점 사이의 각도의 차
if (fabsf(targetAngle - directionAngle) < ANGLE(360) - fabsf(targetAngle - directionAngle))
angleDifference = targetAngle - directionAngle;
else
angleDifference = (targetAngle - directionAngle > 0 ? -ANGLE(360) : ANGLE(360)) + (targetAngle - directionAngle);

float homingAngle = directionAngle + (angleDifference * (1.0f / (distance * 0.045f))); // 최종적으로 결정되는 미사일의 각도
if (fabsf(homingAngle) > ANGLE(180))
homingAngle += homingAngle > 0 ? -ANGLE(360) : ANGLE(360);

missileMove.x = cos(homingAngle) * accelerate;
missileMove.y = sin(homingAngle) * accelerate; // 미사일의 운동량에 대입
*/
