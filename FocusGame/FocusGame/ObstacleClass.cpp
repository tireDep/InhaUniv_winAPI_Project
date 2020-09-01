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

void Obstacle::AddCannon(vector<int> set)
{

}

void Obstacle::Reset()
{

}

void Obstacle::DeleteAllData(vector<Obstacle *> vec)
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
