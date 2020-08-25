#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "GameManger.h"

using namespace std;

enum state
{
	eIdle = 0, eMoveLeft = 10, eMoveRight = 15, eMoveDown = 20, eMoveUp = 25,
	eJump = 50, eFall = 100, eFocus = 150, eSmallFocus = 10
};

enum playerSet
{
	ePlayerSize = 8, efMoveSize = 8, eMoveSpeed = 10, eFouceGauge = 0,
	eFocusLv0 = 0, eFocusLv1 = 100, eFocusLv2 = 150, eFocusLv3 = 250,

	eGravity = 200, eJumpPower = 85
	// �� : ��ġ�� ���� ����..
	// �������� ���� ����
};

class Player : public Object
{
private:
	GameManager *gameManger = GameManager::GetInstance();

	POINT playerPos[4];	// �÷��̾� ��ġ
	POINT focusPos[4];	// ��Ŀ�� ��ġ
	POINT fMovePos[4];
	POINT centerPos;
	POINT fCenterPos;

	POINT lastMove;

	int moveDirection;
	int moveSpeed;
	int gravity;

	int playerState;	// ���� ���� �÷���
	int focusGauge;
	int focusLv;

	bool isJump;	// ���� �� �Ǻ�
	int jumpPower;	// �����ϴ� ��

	bool isBtmGround;

	Player();
	~Player();

public:
	static Player* GetInstance();

	void Update();
	void Gravity();
	
	bool CheckBtmGround(int &lengthDiff);
	bool CheckUpGround(int &lengthDiff);

	void DrawObject(HDC hdc);

	void MovePlayer();

	void MoveFocusPos(int direction, int moveVal);

	void SetPos(POINT pos[], int xPos, int yPos, int addNum);

	void CalcCenterPos();
	void CalcFCenterPos();
};

