#pragma once
#include "stdafx.h"
#include "ObjectClass.h"

using namespace std;

class Obstacle : public Object
{
private:

public:
	Obstacle();
	virtual ~Obstacle();

	virtual void Update();
	virtual void DrawObject(HDC hdc);

	void AddCannon(vector<int> set);

	static void DeleteAllData(vector<Obstacle *> vec);
};
