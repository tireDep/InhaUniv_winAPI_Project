#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "GameManger.h"

using namespace std;

enum state
{
	eIdle = 0, eMoveL = 10, eMoveR = 15, eMoveDown = 20, 
	eJump = 50, eFall = 100, eFocus = 150, eSmallFocus = 10
};

enum playerSet
{
	ePlayerSize = 8, efMoveSize = 8, eMoveSpeed = 10, eFouceGauge = 0,
	eFocusLv0 = 0, eFocusLv1 = 100, eFocusLv2 = 150, eFocusLv3 = 250,

	eGravity = 300, eJumpPower = 150
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
	bool CheckBtmGround();

	void DrawObject(HDC hdc);

	void MovePlayer();

	void SetPos(POINT pos[], int xPos, int yPos, int addNum);

	void CalcCenterPos();
	void CalcFCenterPos();
};

Player::Player()
{
	//if(gameManger->GetScreenSize().right != 0)
	//	SetPos(playerPos, gameManger->GetScreenSize().right / 2, gameManger->GetScreenSize().bottom / 2, ePlayerSize);
	//else
	SetPos(playerPos, 800 / 2, 592 / 2, ePlayerSize);

	cout << gameManger->GetSceneNum() << endl;

	playerState = eIdle;
	isJump = false;
	jumpPower = 0;

	isBtmGround = false;

	moveSpeed = eMoveSpeed;
	gravity = eGravity;

	focusGauge = eFocusLv3;
	focusLv = eFocusLv3;
	// todo : ù ������ 0���� �ؾ��� -> ���� ������ ������ ����
	
	CalcCenterPos();

	SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
	SetPos(fMovePos, centerPos.x, centerPos.y, efMoveSize);
}

Player::~Player()
{

}

Player* Player::GetInstance()
{
	static Player player;
	return &player;
}

void Player::Update()
{
	Gravity();
	MovePlayer();
	// �浹����(��, ��ֹ� ��)
}

void Player::Gravity()
{
	// �÷��̾�� ��Ŀ�� ���°� �ƴϰų�, �������� �ƴѰ�� ��� �߷��� ������ ����
	if (playerState != eFocus || playerState != eJump)
	{
		if(CheckBtmGround())
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y += gravity * 0.1;
		}
	}
}

bool Player::CheckBtmGround()
{
	// todo : �ٴڰ��� �浹 ������ �ʿ���!
	// �������� ���ڷ� �ʿ���
	if (playerPos[2].y < 550)	// �ӽ� ����
	{
		isBtmGround = false;
		return true;
	}
	else
	{
		isBtmGround = true;
		return false;
	}
}

void Player::DrawObject(HDC hdc)
{
	//if(playerState == eFocus)
		Polygon(hdc, focusPos, 4);
	// todo : �������� �̹����� ��ü

	Polygon(hdc, playerPos, 4);
	
	if (playerState == eFocus)
		Polygon(hdc, fMovePos, 4);
}

void Player::MovePlayer()
{
	static bool isCharing = false;
	// todo : �̵� ���� �Ǻ� (���� �Ǻ�)

	// �����̵� ������!
	// test
	POINT speed;
	float v;

	static float time;

	if (lastMove.x != 0)
	{
		speed.x = lastMove.x - fCenterPos.x;
		speed.y = lastMove.y - fCenterPos.y;

		v = sqrt((pow(speed.x, 2) + pow(speed.y, 2)));
		//printf("tt : %d %d\n", lastMove.x, lastMove.y);
		printf("speed : %d %d, v : %f\n", speed.x, speed.y, v);
		// �ӷ� ���

		POINT calcV;
		calcV.x = v;
		calcV.y = gravity * time;

		float objV = sqrt(pow(calcV.x, 2) + pow(calcV.y, 2));

		POINT pos;
		pos.x = calcV.x * time;
		pos.y = 0.5 * gravity * time * time;

		if (speed.x > 0)
		{
			playerPos[0].x -= pos.x;
			playerPos[0].y += pos.y;

			playerPos[1].x -= pos.x;
			playerPos[1].y += pos.y;

			playerPos[2].x -= pos.x;
			playerPos[2].y += pos.y;

			playerPos[3].x -= pos.x;
			playerPos[3].y += pos.y;
		}
		else if (speed.x < 0)
		{
			playerPos[0].x += pos.x;
			playerPos[0].y += pos.y;

			playerPos[1].x += pos.x;
			playerPos[1].y += pos.y;

			playerPos[2].x += pos.x;
			playerPos[2].y += pos.y;

			playerPos[3].x += pos.x;
			playerPos[3].y += pos.y;
		}
		else if (speed.x == 0)
		{
			playerPos[0].y += pos.y;

			playerPos[1].y += pos.y;

			playerPos[2].y += pos.y;

			playerPos[3].y += pos.y;
		}

		if (playerPos[2].y > 550)
			lastMove.x = 0;

		time += 0.1;
	}
	else
		time = 0;

	// test
	

	if (playerState != eFocus)
	{
		// �÷��̾� �̵�
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y += eMoveSpeed;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].x += eMoveSpeed;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].x -= eMoveSpeed;
		}
		// �÷��̾� �̵�
		
		// ����
		if (!isJump && ((GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) 
			&& GetKeyState(0x41) >= 0)	// ��Ŀ�� Ǯ���ڸ��� �ٴ� �� ����
		{
			isJump = true;
			playerState = eJump;
			jumpPower = eJumpPower;
		}
		else if (isJump && playerState == eJump)
		{
			if (jumpPower > 0)
			{
				jumpPower -= eGravity * 0.1;
				for (int i = 0; i < 4; i++)
					playerPos[i].y -= jumpPower;
			}
			else
			{
				// todo : ���κ� �浹 ���� �ʿ��� ��?
				playerState = eFall;
			}
		}
		else
		{
			//// ���� �ϴ� ���ȿ� ���� ���ϰ� ���ƾ� ��
			//// �ٴڿ� ����� ��� ���� & �ٷ� ���� x
			if ( ((GetKeyState(VK_SPACE) < 0) || (GetKeyState(VK_UP) < 0)) )
				isJump = true;	// jumpŰ�� ������ �ִ� ��Ȳ
			else if(isBtmGround)
			{
				// jumpŰ�� ������ ���� x, ���鿡 ���� -> ���� �ʱ�ȭ, n�� ���� ����
				isJump = false;
				playerState = eIdle;
				jumpPower = eJumpPower;	
			}
		}	// else_jump
		// ����

		// ��Ŀ��
		if (((GetAsyncKeyState(0x41) & 0x8000)) && !isCharing)
		{
			moveSpeed = 0;
			jumpPower = 0;
			gravity = 0;

			playerState = eFocus;
			CalcCenterPos();
			SetPos(fMovePos, centerPos.x, centerPos.y, efMoveSize);
		}

		else
		{
			moveSpeed = eMoveSpeed;
			gravity = eGravity;

			if (focusGauge < focusLv)
			{
				focusGauge += 1.5;
			}
			//else
			//	focusGauge = focusLv;

			if (focusGauge <= eSmallFocus || GetKeyState(0x41) < 0)	// ��� ������ ������ ��Ŀ�� ��� ���� x
				isCharing = true;
			else
				isCharing = false;

			CalcCenterPos();
			SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
		}
		// ��Ŀ��
	}

	else
	{
		if ( ((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(0x61) & 0x8000)) )
		{
			if (focusGauge > eSmallFocus)
			{
				focusGauge -= 1.5;
				SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
			}
			else
			{
				isCharing = true;

				// lastMove.x = centerPos.x; 
				// lastMove.y = centerPos.y;
				// 
				// SetPos(playerPos, fCenterPos.x, fCenterPos.y, ePlayerSize);
				// �������� �� �޸� �̵����� x
				playerState = eIdle;
			}
		}
		else
		{
			CalcCenterPos();
			lastMove.x = centerPos.x;
			lastMove.y = centerPos.y;

			CalcFCenterPos();
			SetPos(playerPos, fCenterPos.x, fCenterPos.y, ePlayerSize);
			playerState = eIdle;
		}


		// ��Ŀ�� ���ο����� �̵������ؾ� ��
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				fMovePos[i].y -= 10;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				fMovePos[i].y += 10;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				fMovePos[i].x -= 10;
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				fMovePos[i].x += 10;
		}
		CalcFCenterPos();
	}
		// Player Control
}

void Player::SetPos(POINT pos[], int xPos, int yPos, int addNum)
{
	POINT temp;
	for (int i = 0; i < 4; i++)
	{
		temp.x = addNum;
		temp.y = addNum;
		if (i == 0)
		{
			temp.x *= -1;
			temp.y *= -1;
		}
		else if (i == 1)
		{
			temp.x *= 1;
			temp.y *= -1;
		}
		else if (i == 2)
		{
			temp.x *= 1;
			temp.y *= 1;
		}
		else if (i == 3)
		{
			temp.x *= -1;
			temp.y *= 1;
		}

		pos[i].x = xPos + temp.x;
		pos[i].y = yPos + temp.y;

	}
}

inline void Player::CalcCenterPos()
{
	centerPos.x = (playerPos[0].x + playerPos[1].x) / 2;
	centerPos.y = (playerPos[0].y + playerPos[2].y) / 2;
}

inline void Player::CalcFCenterPos()
{
	fCenterPos.x = (fMovePos[0].x + fMovePos[1].x) / 2;
	fCenterPos.y = (fMovePos[0].y + fMovePos[2].y) / 2;
}
