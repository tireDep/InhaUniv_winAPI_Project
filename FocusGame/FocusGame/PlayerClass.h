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
};

Player::Player()
{
	if(gameManger->GetScreenSize().right != 0)
		SetPos(playerPos, gameManger->GetScreenSize().right / 2, gameManger->GetScreenSize().bottom / 2, ePlayerSize);
	else
		SetPos(playerPos, 800 / 2, 592 / 2, ePlayerSize);

	cout << gameManger->GetSceneNum() << endl;

	playerState = eIdle;
	isJump = false;
	jumpPower = 0;

	isBtmGround = false;

	moveSpeed = eMoveSpeed;
	gravity = eGravity;

	focusGauge = eFocusLv1;
	focusLv = eFocusLv1;
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
	Rectangle(hdc, 0, 0, 800, 800);
	// �׽�Ʈ��

	if(playerState == eFocus)
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
		if (!isJump && ((GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000)) )
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
			{
				// jumpŰ�� ������ �ִ� ��Ȳ
				//printf("push\n");
				isJump = true;
			}
			else if(isBtmGround)
			{
				// jumpŰ�� ������ ���� x, ���鿡 ���� -> ���� �ʱ�ȭ
				// n�� ���� ����
				// printf("didt push\n");
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
		}

		else
		{
			moveSpeed = eMoveSpeed;
			// jumpPower = eJumpPower;
			gravity = eGravity;

			if (focusGauge <= focusLv)
			{
				focusGauge += 1.5;

				{
					focusPos[0].x -= 1.5;
					focusPos[0].y -= 1.5;

					focusPos[1].x += 1.5;
					focusPos[1].y -= 1.5;

					focusPos[2].x += 1.5;
					focusPos[2].y += 1.5;

					focusPos[3].x -= 1.5;
					focusPos[3].y += 1.5;
				}
			}
			else
				focusGauge = focusLv;

			if (focusGauge <= eSmallFocus || GetKeyState(0x41) < 0)	// ��� ������ ������ ��Ŀ�� ��� ���� x
				isCharing = true;
			else
				isCharing = false;

			printf("-----fGauge : %d\n", focusGauge);
		}

		printf("fGauge : %d\n", focusGauge);
		// ��Ŀ��
	}

	else
	{
		if ( ((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(0x61) & 0x8000)) )
		{
			if (focusGauge >= 0)
			{
				focusGauge -= 1.5;
				
				focusPos[0].x += 1.5;
				focusPos[0].y += 1.5;

				focusPos[1].x -= 1.5;
				focusPos[1].y += 1.5;

				focusPos[2].x -= 1.5;
				focusPos[2].y -= 1.5;

				focusPos[3].x += 1.5;
				focusPos[3].y -= 1.5;
			}
			else
			{
				isCharing = true;
				playerState = eIdle;
			}
		}
		else
			playerState = eIdle;


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
