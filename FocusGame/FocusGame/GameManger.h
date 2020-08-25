#pragma once
#include "stdafx.h"
#include "FocusGame.h"

using namespace std;

enum BaicsWindowSize
{
	eWindowWidth = 816, eWindowHeight = 651,
	eTrueWinWidth = 800, eTrueWinHeight = 592
};

enum SceneNum
{
	eMainScene = 0, eGameScene = 50, eResultScene = 150, eExit = 9999,
};

class GameManager
{
private:
	RECT screenSize;
	vector<MapTile> nowMap;

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

	void SetNowMap(vector<MapTile> map);

	int GetSceneNum();
	bool GetIsPause();
	bool GetIsPlayerLive();
	RECT GetScreenSize();

	vector<MapTile> GetMap();
};

