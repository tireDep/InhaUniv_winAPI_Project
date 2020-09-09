#pragma once
#include "stdafx.h"

class Object
{
private:

public:
	Object();
	virtual ~Object();

	virtual void Update();
	virtual void DrawObject(HDC hdc);
	virtual void RenderObject(HWND hWnd, HDC hdc);

	virtual void Reset();
};

