#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "GameManager.h"

using namespace std;

enum state
{
	eIdle = 0, eMoveLeft = 1, eMoveUp = 2, eMoveRight = 3, eMoveDown = 4,
	eJump = 50, eFall = 100, eFocus = 150, eDead = 175, eSmallFocus = 10,

	eLimitL = 16, eLimitR = 784, eLimitT = 16, eLimitB = 576
};

enum playerSet
{
	ePlayerSize = 8, eMoveSpeed = 15,

	eGravity = 160, eJumpPower = 70
	// ※ : 수치는 조정 가능..
	// 정해지지 않은 값들
};

#define dTimeSec 0.1
#define dCorrection 0.1

class Player : public Object
{
private:
	POINT playerPos[4];	// 플레이어 위치
	POINT focusPos[4];	// 포커스 위치
	POINT fMovePos[4];
	POINT centerPos;
	POINT fCenterPos;

	POINT lastPlayerPos[4];
	POINT lastMoveCenter;

	POINT resenPos;

	int moveDirection;
	int moveSpeed;
	int gravity;

	bool isCharging;
	int playerState;	// 현재 상태 플래그

	int maxFocusGauge;
	int focusGauge;
	int focusLv;

	bool isJump;	// 점프 중 판별
	int jumpPower;	// 점프하는 힘

	bool isBtmGround;

	HBITMAP hPlayerBitmap;
	HBITMAP hFocusBitmap;
	HBITMAP hSpotBitmap;

	BITMAP playerBitmap;
	BITMAP focusBitmap;
	BITMAP spotBitmap;

	bool isRightSight;
	int nowFrame;
	bool isEndAni;

	time_t nowTime;
	struct tm *tmTime;

	int timer;
	int countDownSec;

	bool isGetItem;
	POINT aniPos;

	Player();

public:
	~Player();
	static Player* GetInstance();

	void Update();
	void Gravity();
	
	bool CheckBtmGround(int &lengthDiff);
	bool CollisionMap(int direction, int &lengthDiff);

	void CheckOut(POINT pos[], int direction);
	bool CheckOutMap(POINT pos[], int direction, int &lengthDiff);
	bool CheckBlockMap();
	// focus 관련 충돌 함수

	void DrawObject(HDC hdc);
	void RenderObject(HWND hWnd, HDC hdc);

	void CalcFocusMove();
	bool FocusMomentum();
	void CanMovePlayer();
	void MovePlayer(POINT pos[], int direction, int num, float mulNum, float addNum);

	int CheckFocusRange(int direction, int mulNum);
	bool CheckTileMap(TileMap tile);
	// 이동 관련 함수

	void SetPos(POINT pos[], int xPos, int yPos, int addNum);

	void CalcCenterPos();
	void CalcFCenterPos();

	RECT ConversionRect(POINT pos[]);

	void ReturnLastPos();
	void Reset();

	RECT GetPlayerPos();
	RECT GetFocusPos();
	bool GetIsFocusMode();
	bool GetIsEndAni();

	void SetIsPlayerDead(bool set);
	bool GetIsPlayerDead();

	void SetFocusLv();
	int GetFocusLv();

	void SetFocusGauge();
};

