#pragma once
#include "stdafx.h"

class Object
{
private:
	// Object();
	// virtual ~Object();

public:
	// static Object* GetInstance();

	Object();
	virtual ~Object();

	virtual void Update();
	virtual void DrawObject(HDC hdc);

	virtual void Reset();

	// virtual void RenderObject();
};

