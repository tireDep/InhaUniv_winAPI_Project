#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "GameManger.h"

using namespace std;

enum state
{
	eIdle = 0, eMoveLeft = 10, eMoveRight = 15, eMoveDown = 20, eMoveUp = 25,
	eJump = 50, eFall = 100, eFocus = 150, eSmallFocus = 10,

	eLimitL = 16, eLimitR = 784, eLimitT = 16, eLimitB = 576
};

enum playerSet
{
	ePlayerSize = 8, efMoveSize = 8, eMoveSpeed = 10, eFouceGauge = 0,
	eFocusLv0 = 0, eFocusLv1 = 100, eFocusLv2 = 150, eFocusLv3 = 250,

	eGravity = 250, eJumpPower = 150
	// ※ : 수치는 조정 가능..
	// 정해지지 않은 값들
};

#define defTimeSec 0.1

class Player : public Object
{
private:
	GameManager *gameManger = GameManager::GetInstance();

	POINT playerPos[4];	// 플레이어 위치
	POINT focusPos[4];	// 포커스 위치
	POINT fMovePos[4];
	POINT centerPos;
	POINT fCenterPos;

	POINT lastPlayerPos[4];
	POINT lastMoveCenter;

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
	~Player();

public:
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
	void FocusMomentum();
	void CanMovePlayer();
	void MovePlayer(POINT pos[], int direction, int num, float mulNum, float addNum);
	// 이동 관련 함수

	void SetPos(POINT pos[], int xPos, int yPos, int addNum);

	void CalcCenterPos();
	void CalcFCenterPos();

	RECT ConversionRect(POINT pos[]);
};

