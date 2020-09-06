#include "stdafx.h"
#include "GameManager.h"

#include <fstream>

#define dKeyCode 'k'

using namespace std;

GameManager::GameManager()
{
	// nowScene = eMainScene;
	nowScene = eGameScene;

	isPause = false;
	isPlayerLive = true;

	isDrawRect = false;

	ReadSaveData();
}

GameManager::~GameManager()
{
	WriteSaveData();
}

GameManager* GameManager::GetInstance()
{
	static GameManager gameManager;
	return &gameManager;
}

void GameManager::CalcScreenSize(HWND hWnd)
{
	GetClientRect(hWnd, &screenSize);
}

void GameManager::ReadSaveData()
{
	ifstream readFile;
	readFile.open("saveData.dat", ios::in | ios::binary);
	if (readFile.is_open())
	{
		// 자료 읽어오기
		readFile.read((char*)&nowStage, sizeof(int));
		readFile.read((char*)&nowFocusLv, sizeof(int));

		// decode
		nowStage = (nowStage / dKeyCode) - dKeyCode;
		nowFocusLv = (nowFocusLv / dKeyCode) - dKeyCode;
	}
	else
	{
		// nowStage = 0;
		// nowFocusLv = 0;
		// 실제 데이터

		nowStage = 0;
		nowFocusLv = 50;
		// 임시 숫자
	}
	readFile.close();
}

void GameManager::WriteSaveData()
{
	ofstream saveFile;
	saveFile.open("saveData.dat", ios::out | ios::binary);

	// encode
	nowStage = (nowStage + dKeyCode) * dKeyCode;
	nowFocusLv = (nowFocusLv + dKeyCode) * dKeyCode;

	saveFile.write((char*)&nowStage, sizeof(int));
	saveFile.write((char*)&nowFocusLv, sizeof(int));

	saveFile.close();
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

void GameManager::SetDrawRect(bool set)
{
	if (set == true)
		isDrawRect = true;
	else
		isDrawRect = false;
}

void GameManager::SetNowStage(int set)
{
	nowStage = set;
}

void GameManager::SetFocusLv(int set)
{
	nowFocusLv = set;
}

bool GameManager::GetDrawRect()
{
	return isDrawRect;
}

int GameManager::GetNowStage()
{
	return nowStage;
}

int GameManager::GetFocusLv()
{
	return nowFocusLv;
}
