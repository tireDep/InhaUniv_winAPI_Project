#include "stdafx.h"
#include "GameManager.h"

GameManager::GameManager()
{
	nowScene = eMainScene;

	isPause = false;
	isPlayerLive = true;
	
	nowScene = eGameScene;
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

void GameManager::SetNowPlayerPos(RECT set)
{
	nowPlayerPos = set;
}

void GameManager::SetNowMap(vector<TileMap> map)
{
	nowMap = map;
}

void GameManager::SetNowScene(int scene)
{
	nowScene = scene;
}

int GameManager::GetSceneNum()
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

int GameManager::GetNowScene()
{
	return nowScene;
}

RECT GameManager::GetScreenSize()
{
	return screenSize;
}

vector<TileMap> GameManager::GetNowMap()
{
	return nowMap;
}

RECT GameManager::GetNowPlayerPos()
{
	return nowPlayerPos;
}
