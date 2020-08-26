#pragma once

#include "resource.h"

enum MapType
{
	eMapBlock = 50, eMapObstacle = 100
};

struct MapTile
{
	RECT pos;
	int type;
};