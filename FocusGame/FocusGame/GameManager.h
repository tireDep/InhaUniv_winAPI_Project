#pragma once
#include "stdafx.h"
#include "FocusGame.h"

using namespace std;

enum BaicsWindowSize
{
	eWindowWidth = 816, eWindowHeight = 631,
	eTrueWinWidth = 800, eTrueWinHeight = 592
};

enum SceneNum
{
	eMainScene = 0, eChangeScene = 25, eGameScene = 50, eResultScene = 150, eExit = 9999,
};

class GameManager
{
private:
	RECT screenSize;

	int nowScene;
	bool isPause;

	bool isPlayerLive;

	bool isDrawRect;

	int nowStage;
	int nowFocusLv;

	GameManager();

public:
	~GameManager();
	static GameManager* GetInstance();

	void CalcScreenSize(HWND hWnd);

	void ReadSaveData();
	void WriteSaveData();

	void SetIsPause();
	void SetIsPlayerLive(bool live);

	void SetNowScene(int scene);
	void SetDrawRect(bool set);
	void SetNowStage(int set);
	void SetFocusLv(int set);

	bool GetIsPause();
	bool GetIsPlayerLive();
	int GetNowScene();
	RECT GetScreenSize();
	bool GetDrawRect();
	int GetNowStage();
	int GetFocusLv();
};

