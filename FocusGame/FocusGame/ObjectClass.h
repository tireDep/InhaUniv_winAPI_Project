#pragma once
#include "stdafx.h"

class Object
{
private:

public:
	Object();
	virtual ~Object() = 0;

	virtual void Update() = 0;
	virtual void DrawObject(HDC hdc) = 0;
	virtual void RenderObject(HWND hWnd, HDC hdc) = 0;

	virtual void Reset() = 0;
};

