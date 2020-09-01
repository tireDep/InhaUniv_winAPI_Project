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
	// �� : ��ġ�� ���� ����..
	// �������� ���� ����
};

#define dTimeSec 0.1
#define dCorrection 0.1

class Player : public Object
{
private:
	POINT playerPos[4];	// �÷��̾� ��ġ
	POINT focusPos[4];	// ��Ŀ�� ��ġ
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
	int playerState;	// ���� ���� �÷���
	int focusGauge;
	int focusLv;

	bool isJump;	// ���� �� �Ǻ�
	int jumpPower;	// �����ϴ� ��

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
	// focus ���� �浹 �Լ�

	void DrawObject(HDC hdc);

	void CalcFocusMove();
	bool FocusMomentum();
	void CanMovePlayer();
	void MovePlayer(POINT pos[], int direction, int num, float mulNum, float addNum);

	int CheckFocusRange(int direction, int mulNum);
	// �̵� ���� �Լ�

	void SetPos(POINT pos[], int xPos, int yPos, int addNum);

	void CalcCenterPos();
	void CalcFCenterPos();

	RECT ConversionRect(POINT pos[]);

	void Reset();

	RECT GetPlayerPos();
	RECT GetFocusPos();
	bool GetIsFocusMode();
};

