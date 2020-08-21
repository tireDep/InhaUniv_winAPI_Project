#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#include "GameManger.h"

using namespace std;

enum state
{
	eIdle = 0, eMoveL = 10, eMoveR = 15, eMoveDown = 20, 
	eJump = 50, eFall = 100, eFocus = 150
};

enum playerSet
{
	ePlayerSize = 8, efMoveSize = 8, eMoveSpeed = 10, eFouceGauge = 0,
	eFocusLv0 = 0, eFocusLv1 = 100, eFocusLv2 = 150, eFocusLv3 = 250,

	eGravity = 300, eJumpPower = 100
	// ※ : 수치는 조정 가능..
	// 정해지지 않은 값들
};

class Player : public Object
{
private:
	GameManager *gameManger = GameManager::GetInstance();

	POINT playerPos[4];	// 플레이어 위치
	POINT focusPos[4];	// 포커스 위치
	POINT fMovePos[4];
	POINT centerPos;

	int playerState;	// 현재 상태 플래그
	int focusGauge;
	// 포커스 lv 필요?

	bool isJump;	// 점프 중 판별
	int jumpPower;	// 점프하는 힘

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
	focusGauge = eFocusLv1;
	isJump = false;
	jumpPower = 0;

	isBtmGround = false;
	// todo : 첫 시작은 0으로 해야함 -> 추후 아이템 구현시 수정
	
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
	// 충돌판정(벽, 장애물 등)
}

void Player::Gravity()
{
	// 플레이어는 포커스 상태가 아니거나, 점프중이 아닌경우 계속 중력의 영향을 받음
	if (playerState != eFocus || playerState != eJump)
	{
		if(CheckBtmGround())
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y += eGravity * 0.1;
		}
	}
}

bool Player::CheckBtmGround()
{
	// todo : 바닥과의 충돌 판정이 필요함!
	// 맵정보가 인자로 필요함
	if (playerPos[2].y < 550)	// 임시 판정
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
	// 테스트용

	if(playerState == eFocus)
		Polygon(hdc, focusPos, 4);
	// todo : 반투명한 이미지로 대체

	Polygon(hdc, playerPos, 4);
	
	if (playerState == eFocus)
		Polygon(hdc, fMovePos, 4);
}

void Player::MovePlayer()
{
	// todo : 이동 가능 판별 (지형 판별)
	if (playerState != eFocus)
	{
		// 플레이어 이동
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
		// 플레이어 이동
		
		// 점프
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
				// todo : 윗부분 충돌 판정 필요할 듯?
				playerState = eFall;
			}
		}
		else
		{
			//// 점프 하는 동안에 점프 못하게 막아야 함
			//// 바닥에 닿았을 경우 리셋 & 바로 점프 x
			if ( ((GetKeyState(VK_SPACE) < 0) || (GetKeyState(VK_UP) < 0)) )
			{
				// jump키를 누르고 있는 상황
				//printf("push\n");
				isJump = true;
			}
			else if(isBtmGround)
			{
				// jump키를 누르고 있지 x, 지면에 닿음 -> 변수 초기화
				// n번 점프 방지
				// printf("didt push\n");
				isJump = false;
				playerState = eIdle;
				jumpPower = eJumpPower;	
			}
		}	// else_jump
		// 점프
	}
	//	else
	//	{
	//		if (GetAsyncKeyState(VK_UP) & 0x8000)
	//		{
	//			for (int i = 0; i < 4; i++)
	//				focusPos[i].y -= 10;
	//		}
	//		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//		{
	//			for (int i = 0; i < 4; i++)
	//				focusPos[i].y += 10;
	//		}
	//		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	//		{
	//			for (int i = 0; i < 4; i++)
	//				focusPos[i].x -= 10;
	//		}
	//		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	//		{
	//			for (int i = 0; i < 4; i++)
	//				focusPos[i].x += 10;
	//		}
	//	}

		//static bool isCharing = false;
		//if (((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(0x61) & 0x8000)) && !isCharing)
		//{
		//	moveSpeed = 0;
		//	downSpeed = 0;
		//	// 포커스 모드시 이동 x
		//	// 1안 : 속도 제거
		//	// 2안 : boolean 변수로 제어

		//	if (focusPower <= 150)
		//	{
		//		isFocus = true;
		//		focusPower += 2.5;
		//	}
		//	else
		//	{
		//		isFocus = false;
		//		isCharing = true;
		//	}
		//	// todo : 플레이어보다 작아지면 강제로 풀려야 함
		//}
		//else
		//{
		//	moveSpeed = 10;
		//	downSpeed = 20;
		//	// 포커스 모드시 이동 x
		//	// 1안 : 속도 제거
		//	// 2안 : boolean 변수로 제어

		//	isFocus = false;
		//	if (focusPower < 100)
		//		isCharing = false;
		//	else
		//		isCharing = true;

		//	if (focusPower > 0)
		//		focusPower -= 1.5;

		//	else if (focusPower < 0)
		//		focusPower = 0;
		//}
		//// focus
		// Player Control
	//}
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
