#include "stdafx.h"
#include "ObstacleClass.h"
#include "CannonClass.h"

Obstacle::Obstacle()
{

}

Obstacle::~Obstacle()
{

}

void Obstacle::Update()
{

}

void Obstacle::DrawObject(HDC hdc)
{

}

void Obstacle::RenderObject(HWND hWnd, HDC hdc)
{

}

void Obstacle::AddWeapon(vector<Obstacle *> &obstacle, vector<ParceCannonStruct> cannonSet)
{
	DeleteAllData(obstacle);

	// >> 맵에 대포가 존재하는지 판단
	if (cannonSet.size() > 0)
	{
		for (int i = 0; i < cannonSet.size(); i++)
		{
			Cannon *addCannon = new Cannon(cannonSet[i]);
			obstacle.push_back(addCannon);
		}
	}
}

void Obstacle::DeleteAllData(vector<Obstacle *> &vec)
{
	// >> 동적할당 해제
	vector<Obstacle *>::iterator it;
	for (it = vec.begin(); it < vec.end();)
	{
		if (*it != NULL)
		{
			delete *it;
			it = vec.erase(it);
		}
		else
			it++;
	}
	// >> 동적할당 해제
}

void Obstacle::Reset()
{

}