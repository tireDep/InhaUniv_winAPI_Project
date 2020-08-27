#include "stdafx.h"
#include "GameManger.h"

GameManager::GameManager()
{
	nowScene = eMainScene;
	isPause = false;
	isPlayerLive = true;
}

GameManager::~GameManager()
{

}

GameManager* GameManager::GetInstance()
{
	static GameManager gameManger;
	return &gameManger;
}

void GameManager::CalcScreenSize(HWND hWnd)
{
	GetClientRect(hWnd, &screenSize);
}

void GameManager::SetSceneNum(int num)
{
	nowScene = num;
}

void GameManager::SetIsPause()
{
	if (isPause == false)
		isPause = true;
	else
		isPause = false;
}

void GameManager::SetIsPlayerLive(bool live)
{
	isPlayerLive = live;
}

void GameManager::SetNowMap(vector<TileMap> map)
{
	nowMap = map;
}

inline int GameManager::GetSceneNum()
{
	return nowScene;
}

bool GameManager::GetIsPause()
{
	return isPause;
}

bool GameManager::GetIsPlayerLive()
{
	return isPlayerLive;
}

RECT GameManager::GetScreenSize()
{
	return screenSize;
}

vector<TileMap> GameManager::GetMap()
{
	return nowMap;
}
