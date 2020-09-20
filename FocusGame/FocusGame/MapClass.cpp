#include "stdafx.h"
#include "MapClass.h"
#include "PlayerClass.h"
#include "GameManager.h"
#include "UIClass.h"
#include "SoundSystem.h"

#include <fstream>
#include <string>

#define dGameManager GameManager::GetInstance()
#define dPlayer Player::GetInstance()
#define dUI UI::GetInstance()
#define dSoundSys SoundSystem::GetInstance()

#define dKeyCode 'k'

using namespace std;

Map::Map()
{
	hMapBitmap = (HBITMAP)LoadImage(NULL, TEXT("Image/tile.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hMapBitmap, sizeof(BITMAP), &mapBitmap);
}

Map::~Map()
{
	DeleteObject(hMapBitmap);
}

Map* Map::GetInstance()
{
	static Map map;
	return &map;
}

void Map::Update()
{
	if (!dPlayer->GetIsPlayerDead())
	{
		RECT area;
		RECT playerPos = dPlayer->GetPlayerPos();

		for (int i = 0; i < mapPos.size(); i++)
		{
			// >> 아이템 충돌 판정
			if (PlayerGetItem())
				break;

			// >> 가시 충돌 판정
			if (mapPos[i].type == eMapSpike && IntersectRect(&area, &mapPos[i].pos, &playerPos))
			{
				dPlayer->SetIsPlayerDead(true);
				break;
			}

			// >> 스위치 off 판정
			if (mapPos[i].type == eMapBtn_0 && IntersectRect(&area, &mapPos[i].pos, &playerPos))
			{
				mapPos[i].type = eMapBtn_1;
				for (int j = 0; j < mapPos.size(); j++)
				{
					if (mapPos[j].pos.top == mapPos[i].pos.bottom && mapPos[j].pos.left == mapPos[i].pos.left && mapPos[j].pos.right == mapPos[i].pos.right
						&& mapPos[j].type == eMapBtn_2)
						mapPos[j].type = eMapBtn_3;
				}
				// >> 바로 밑에 있는 버튼이 꺼지도록 판정 변경

				dSoundSys->PlayBtnOffSound();
			}

			// >> 다음 스테이지 불러오기
			if (mapPos[i].type == eMapGateOpen && IntersectRect(&area, &mapPos[i].pos, &playerPos))
				isNextStage = true;
		}

		if (CheckOffBtn())
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if (mapPos[i].type == eMapGate_0 || mapPos[i].type == eMapGate_1 || mapPos[i].type == eMapGate_2 || mapPos[i].type == eMapGate_3)
				{
					if(mapPos[i].type==eMapGate_0) dSoundSys->PlayGateBreakSound();	// 사운드 겹침 방지
					mapPos[i].type = eMapGateOpen;
				}
				// todo : 애니메이션 추가?
			}
		}
		// >> 스위치 off 판정
	}
}

bool Map::CheckOffBtn()
{
	for (int i = 0; i < mapPos.size(); i++)
	{
		if (mapPos[i].type == eMapBtn_2)
			return false;
	}
	return true;
}

void Map::CheckShotOffBtn(const RECT &hitPos)
{
	RECT area;
	bool isOff;
	for (int i = 0; i < mapPos.size(); i++)
	{
		isOff = true;
		if (IntersectRect(&area, &mapPos[i].pos, &hitPos) && mapPos[i].type == eMapBtn_2)
		{
			for (int j = 0; j < mapPos.size(); j++)
			{
				if (mapPos[i].pos.top == mapPos[j].pos.bottom && mapPos[i].pos.left == mapPos[j].pos.left && mapPos[i].pos.right == mapPos[j].pos.right)
					isOff = false;	// >> 위에 무언가가 있을 경우 꺼지면 x
			}
			
			if (isOff)
				mapPos[i].type = eMapBtn_3;
		}
	}
}

void Map::DrawObject(HDC hdc)
{
	if (dGameManager->GetDrawRect())
	{
		for (int i = 0; i<mapPos.size(); i++)
			Rectangle(hdc, mapPos[i].pos.left, mapPos[i].pos.top, mapPos[i].pos.right, mapPos[i].pos.bottom);
	}
}

void Map::RenderObject(HWND hWnd, HDC hdc)
{
	HDC hMapDc;
	HBITMAP hMapBit;
	POINT blength = { 16,16 };
	POINT pos = { 0,0 };

	hMapDc = CreateCompatibleDC(hdc);
	hMapBit = (HBITMAP)SelectObject(hMapDc, hMapBitmap);

	for (int i = 0; i < mapPos.size(); i++)
	{
		if (mapPos[i].type == eMapBlock)
			pos = { 0,0 };

		else if (mapPos[i].type == eMapHalfBlock)
			pos = { 16,0 };

		else if (mapPos[i].type == eMapSpike)
			pos = { 0,16 };

		else if (mapPos[i].type == eMapBtn_0)
			pos = { 0,32 };
		else if (mapPos[i].type == eMapBtn_1)
			pos = { 16,32 };

		else if (mapPos[i].type == eMapBtn_2)
			pos = { 0,48 };
		else if (mapPos[i].type == eMapBtn_3)
			pos = { 16,48 };

		else if (mapPos[i].type == eMapGate_0)
			pos = { 0,64 };
		else if (mapPos[i].type == eMapGate_1)
			pos = { 16,64 };
		else if (mapPos[i].type == eMapGate_2)
			pos = { 32,64 };
		else if (mapPos[i].type == eMapGate_3)
			pos = { 48,64 };

		else if (mapPos[i].type == eMapGateOpen)
			pos = { 64,64 };	// todo -> 애니메이션?

		else if (mapPos[i].type == eMapCannon_0)
			pos = { 0,80 };
		else if (mapPos[i].type == eMapCannon_1)
			pos = { 16,80 };
		else if (mapPos[i].type == eMapCannon_2)
			pos = { 32,80 };
		else if (mapPos[i].type == eMapCannon_3)
			pos = { 48,80 };

		else if (mapPos[i].type == eMapCannon_4)
			pos = { 0,96 };
		else if (mapPos[i].type == eMapCannon_5)
			pos = { 16,96 };
		else if (mapPos[i].type == eMapCannon_6)
			pos = { 32,96 };
		else if (mapPos[i].type == eMapCannon_7)
			pos = { 48,96 };

		else if (mapPos[i].type == eMapGateCloseVertical)
			pos = { 0,112 };
		else if (mapPos[i].type == eMapGateCloseHorizen)
			pos = { 16,112 };

		else if (mapPos[i].type == eMapItem)
			pos = { 0,128 };

		TransparentBlt(hdc, mapPos[i].pos.left, mapPos[i].pos.top, blength.x, blength.y, hMapDc, pos.x, pos.y, blength.x, blength.y, RGB(255, 0, 255));
	}

	SelectObject(hMapDc, hMapBit);
	DeleteObject(hMapBit);
	DeleteDC(hMapDc);
}

void Map::SetNextStage()
{
	if (dGameManager->GetNowStage() == -1)
		dUI->SetIsGoMain(false);	// >> 엔딩 본 후 재 실행 시

	if (dUI->GetIsGoMain() == false)
		dGameManager->SetNowStage(dGameManager->GetNowStage() + 1);
	else
	{
		// >> 일시정지 후 메인화면에서 다시 넘어온 경우
		dGameManager->SetNowStage(dGameManager->GetNowStage());
		dUI->SetIsGoMain(false);
	}

	mapPos.clear();
	resetPos.clear();
	
	ReadMapData();
}

vector<TileMap> Map::GetMapPos()
{
	return mapPos;
}

vector<ParceCannonStruct> Map::CheckInCannon()
{
	ParceCannonStruct tempVal;
	vector<ParceCannonStruct> result;

	vector<TileMap>::iterator it;
	for (it = mapPos.begin(); it < mapPos.end(); it++)
	{
		if (it->type == eMapCannon_0)
		{
			tempVal.pos = { it->pos.right, it->pos.bottom };
			tempVal.type = dNormal;
			
			result.push_back(tempVal);
		}

		if (it->type == eMapCannon_4)
		{
			tempVal.pos = { it->pos.right, it->pos.bottom };
			tempVal.type = dHoming;

			result.push_back(tempVal);
		}
	}

	return result;
}

void Map::Reset()
{
	mapPos = resetPos;
}

void Map::ReadMapData()
{
	string fileName;
	ifstream mapFile;

	if (dGameManager->GetNowStage() < 10)
		fileName = "map_0";
	else
		fileName = "map_";

	fileName += to_string(dGameManager->GetNowStage()) + ".dat";
	fileName = "./Map/" + fileName;

	mapFile.open(fileName, ios::in | ios::binary);
	if (mapFile.is_open())
	{
		TileMap tileMap;
		tileMap.pos = { 0,0,0,0 };
		tileMap.type = 0;

		mapFile.read((char*)&resenSpot.x, sizeof(int));
		mapFile.read((char*)&resenSpot.y, sizeof(int));

		resenSpot.x = (resenSpot.x / dKeyCode) - dKeyCode;
		resenSpot.y = (resenSpot.y / dKeyCode) - dKeyCode;

		while (!mapFile.eof())
		{
			mapFile.read((char*)&tileMap.type, sizeof(int));
			mapFile.read((char*)&tileMap.pos.left, sizeof(int));
			mapFile.read((char*)&tileMap.pos.top, sizeof(int));
			mapFile.read((char*)&tileMap.pos.right, sizeof(int));
			mapFile.read((char*)&tileMap.pos.bottom, sizeof(int));

			tileMap.type = (tileMap.type / dKeyCode) - dKeyCode;
			tileMap.pos.left = (tileMap.pos.left / dKeyCode) - dKeyCode;
			tileMap.pos.top = (tileMap.pos.top / dKeyCode) - dKeyCode;
			tileMap.pos.right = (tileMap.pos.right / dKeyCode) - dKeyCode;
			tileMap.pos.bottom = (tileMap.pos.bottom / dKeyCode) - dKeyCode;

			if (tileMap.type <= 0)
				break;	// >> 무한루프 처리

			mapPos.push_back(tileMap);
		}
		dSoundSys->PlayChangeStageSound();
	}
	else
	{
		// >> 불러올 맵이 없다 => All Clear
		// >> 맨 처음 초기값 세팅
		// >> stageClear -> endScene -> MainScene

		dGameManager->SetNowStage(-1);	
		// >> 맵을 불러오면서 +1 이기 때문에 0 stage 시작을 위해 -1 초기화
		dGameManager->SetFocusLv(0);
		dGameManager->SetNowScene(eResultScene);
	}

	mapFile.close();

	resetPos = mapPos;
	// >> resetValue
	
	isNextStage = false;
}

POINT Map::GetResenSpot()
{
	return resenSpot;
}

void Map::SetIsNextStage(bool set)
{
	isNextStage = set;
}

bool Map::GetIsNextStage()
{
	return isNextStage;
}

bool Map::PlayerGetItem()
{
	RECT area;

	for (int i = 0; i < mapPos.size(); i++)
	{
		if (mapPos[i].type == eMapItem && IntersectRect(&area, &mapPos[i].pos, &dPlayer->GetPlayerPos()))
		{
			dSoundSys->PlayGetItemSound();
			dPlayer->SetFocusLv();
			mapPos.erase(mapPos.begin() + i);
			return true;
		}
	}

	return false;
}
