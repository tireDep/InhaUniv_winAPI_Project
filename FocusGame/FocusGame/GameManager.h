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

enum BasicNum
{
	eBlockSize = 16
};

class GameManager
{
private:
	RECT screenSize;
	vector<TileMap> nowMap;

	RECT nowPlayerPos;
	int nowScene;
	bool isPause;

	bool isPlayerLive;

	bool isDrawRect;

	int nowStage;
	int nowFocusLv;
	// todo : saveData

	GameManager();

public:
	~GameManager();
	static GameManager* GetInstance();

	void CalcScreenSize(HWND hWnd);

	void ReadSaveData();
	void WriteSaveData();

	void SetSceneNum(int num);
	void SetIsPause();
	void SetIsPlayerLive(bool live);

	void SetNowPlayerPos(RECT set);
	void SetNowMap(vector<TileMap> map);
	void SetNowScene(int scene);

	int GetSceneNum();
	bool GetIsPause();
	bool GetIsPlayerLive();
	int GetNowScene();

	RECT GetScreenSize();
	vector<TileMap> GetNowMap();
	RECT GetNowPlayerPos();

	void SetDrawRect(bool set);
	void SetNowStage(int set);
	void SetFocusLv(int set);

	bool GetDrawRect();
	int GetNowStage();
	int GetFocusLv();
};

