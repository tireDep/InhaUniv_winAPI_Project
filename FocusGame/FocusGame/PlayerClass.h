#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "GameManager.h"

#define dGameManger GameManager::GetInstance()

using namespace std;

enum state
{
	eIdle = 0, eMoveLeft = 0, eMoveUp = 1, eMoveRight = 2, eMoveDown =3,
	eJump = 50, eFall = 100, eFocus = 150, eSmallFocus = 10,

	eLimitL = 16, eLimitR = 784, eLimitT = 16, eLimitB = 576
};

enum playerSet
{
	ePlayerSize = 8, efMoveSize = 8, eMoveSpeed = 15, eFouceGauge = 0,
	eFocusLv0 = 0, eFocusLv1 = 100, eFocusLv2 = 150, eFocusLv3 = 250,

	eGravity = 185, eJumpPower = 85
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

	POINT resetPlayerPos[4];

	int moveDirection;
	int moveSpeed;
	int gravity;

	bool isCharging;
	int playerState;	// 현재 상태 플래그
	int focusGauge;
	int focusLv;

	bool isJump;	// 점프 중 판별
	int jumpPower;	// 점프하는 힘

	bool isBtmGround;

	Player();

public:
	~Player();
	static Player* GetInstance();

	void Update();
	void Gravity();
	
	bool CheckBtmGround(int &lengthDiff);
	bool CollisionMap(POINT pos[], int direction, int &lengthDiff);

	void CheckOut(POINT pos[], int direction);
	bool CheckOutMap(POINT pos[], int direction, int &lengthDiff);
	bool CheckBlockMap();
	// focus 관련 충돌 함수

	void DrawObject(HDC hdc);

	void CalcFocusMove();
	bool FocusMomentum();
	void CanMovePlayer();
	void MovePlayer(POINT pos[], int direction, int num, float mulNum, float addNum);

	int CheckFocusRange(int direction, int mulNum);
	// 이동 관련 함수

	void SetPos(POINT pos[], int xPos, int yPos, int addNum);

	void CalcCenterPos();
	void CalcFCenterPos();

	RECT ConversionRect(POINT pos[]);

	void Reset();

	RECT GetPlayerPos();
	RECT GetFocusPos();
	bool GetIsFocusMode();
};

