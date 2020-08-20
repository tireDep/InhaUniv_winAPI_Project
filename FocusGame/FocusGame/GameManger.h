#pragma once
#include "stdafx.h"

enum BaicsWindowSize
{
	eWindowWidth = 816, eWindowHeight = 651
};

enum SceneNum
{
	eMainScene = 0, eGameScene = 50, eResultScene = 150, eExit = 9999,
};

class GameManager
{
private:
	RECT screenSize;

	int nowScene;
	bool isPause;

	bool isPlayerLive;

	GameManager();
	~GameManager();

public:
	static GameManager* GetInstance();

	void CalcScreenSize(HWND hWnd);

	void SetSceneNum(int num);
	void SetIsPause();
	void SetIsPlayerLive(bool live);

	int GetSceneNum();
	bool GetIsPause();
	bool GetIsPlayerLive();
	RECT GetScreenSize();
};

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

inline void GameManager::SetSceneNum(int num)
{
	nowScene = num;
}

inline void GameManager::SetIsPause()
{
	if (isPause == false)
		isPause = true;
	else
		isPause = false;
}

inline void GameManager::SetIsPlayerLive(bool live)
{
	isPlayerLive = live;
}

inline int GameManager::GetSceneNum()
{
	return nowScene;
}

inline bool GameManager::GetIsPause()
{
	return isPause;
}

inline bool GameManager::GetIsPlayerLive()
{
	return isPlayerLive;
}

RECT GameManager::GetScreenSize()
{
	return screenSize;
}
