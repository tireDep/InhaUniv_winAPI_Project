#pragma once
#include "stdafx.h"
#include "ObjectClass.h"

class Obstacle : public Object
{
private:

public:
	Obstacle();
	virtual ~Obstacle();

	virtual void Update();
	virtual void DrawObject(HDC hdc);
};
