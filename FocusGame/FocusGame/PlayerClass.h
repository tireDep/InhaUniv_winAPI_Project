#pragma once
#include "stdafx.h"
#include "GameManger.h"

using namespace std;

enum state
{
	eIdle = 0, eMoveL = 10, eMoveR = 15, eMoveDown = 20, 
	eJump = 50, eDrop = 100, eFocus = 150
};

enum playerSet
{
	ePlayerSize = 8, efMoveSize = 8, eMoveSpeed = 10, eFouceGauge = 0,
	eFocusLv0 = 0, eFocusLv1 = 100, eFocusLv2 = 150, eFocusLv3 = 250
	// ※ : 수치는 조정 가능..
};

class Player
{
private:
	GameManager *gameManger = GameManager::GetInstance();

	POINT playerPos[4];	// 플레이어 위치
	POINT focusPos[4];	// 포커스 위치
	POINT fMovePos[4];
	POINT centerPos;

	int state;	// 현재 상태 플래그
	int focusGauge;
	// 포커스 lv 필요?

	Player();
	~Player();

public:
	static Player* GetInstance();

	void Update();
	void DrawPlayer(HDC hdc);

	void MovePlayer();

	void SetPos(POINT pos[], int xPos, int yPos, int addNum);

	void CalcCenterPos();
};

Player::Player()
{
	// gameManger = GameManager::GetInstance();
	// // playerSet
	// playerPos[0].x = eWindowWidth / 2 - ePlayerSize;
	// playerPos[0].y = eWindowHeight/ 2 - ePlayerSize;
	// 
	// playerPos[1].x = eWindowWidth / 2 + ePlayerSize;
	// playerPos[1].y = eWindowHeight / 2 - ePlayerSize;
	// 
	// playerPos[2].x = eWindowWidth / 2 + ePlayerSize;
	// playerPos[2].y = eWindowHeight / 2 + ePlayerSize;
	// 
	// playerPos[3].x = eWindowWidth / 2 - ePlayerSize;
	// playerPos[3].y = eWindowHeight / 2 + ePlayerSize;
	// // playerSet

	if(gameManger->GetScreenSize().right != 0)
		SetPos(playerPos, gameManger->GetScreenSize().right / 2, gameManger->GetScreenSize().bottom / 2, ePlayerSize);
	else
		SetPos(playerPos, 800 / 2, 592 / 2, ePlayerSize);

	cout << gameManger->GetSceneNum() << endl;
	state = eIdle;
	focusGauge = eFocusLv1;
	// ※ : 첫 시작은 0으로 해야함 -> 추후 아이템 구현시 수정
	
	CalcCenterPos();

	// // focusSet
	// focusPos[0].x = centerPos.x - focusGauge;
	// focusPos[0].y = centerPos.y - focusGauge;
	// 
	// focusPos[1].x = centerPos.x + focusGauge;
	// focusPos[1].y = centerPos.y - focusGauge;
	// 
	// focusPos[2].x = centerPos.x + focusGauge;
	// focusPos[2].y = centerPos.y + focusGauge;
	// 
	// focusPos[3].x = centerPos.x - focusGauge;
	// focusPos[3].y = centerPos.y + focusGauge;
	// // focusSet

	SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);

	// // focusMoveSet
	// fMovePos[0].x = centerPos.x - efMoveSize;
	// fMovePos[0].y = centerPos.y - efMoveSize;
	// 
	// fMovePos[1].x = centerPos.x + efMoveSize;
	// fMovePos[1].y = centerPos.y - efMoveSize;
	// 
	// fMovePos[2].x = centerPos.x + efMoveSize;
	// fMovePos[2].y = centerPos.y + efMoveSize;
	// 
	// fMovePos[3].x = centerPos.x - efMoveSize;
	// fMovePos[3].y = centerPos.y + efMoveSize;
	// // focusMoveSet

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
	MovePlayer();
}

void Player::DrawPlayer(HDC hdc)
{
	if(state==eFocus)
		Polygon(hdc, focusPos, 4);
	// 반투명한 이미지로 대체

	Polygon(hdc, playerPos, 4);
	
	if (state == eFocus)
		Polygon(hdc, fMovePos, 4);
}

void Player::MovePlayer()
{
	// todo : 이동 가능 판별 (지형 판별)

	if (state != eFocus)
	{
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

		//// 점프 하는 동안에 점프 못하게 막아야 함
		//// 바닥에 닿았을 경우 리셋 & 바로 점프 x
		//// static int tempJumpPower = 115;
		//// static int tempGravity = 250;

		//static int tempJumpPower = 100;
		//static int tempGravity = 250;

		//static bool isJumping = false;

		//// if (tempJumpPower > 0 && !isJumping && ((GetAsyncKeyState(VK_SPACE) & 0x8000))) //  || (GetAsyncKeyState(VK_UP) & 0x0001))// && true)
		//if (tempJumpPower > 0 && !isJumping && ((GetAsyncKeyState(VK_SPACE)))) //  || (GetAsyncKeyState(VK_UP) & 0x0001))// && true)
		//{
		//	// for (int i = 0; i < 4; i++)
		//	// 	draw[i].y -= moveSpeed * jumpHeight;
		//	// 기본 점프
		//	isJumping = true;
		//}
		//else if (isJumping)
		//{
		//	if (tempJumpPower > 0)
		//	{
		//		tempJumpPower -= tempGravity * 0.1;
		//		for (int i = 0; i < 4; i++)
		//			//draw[i].y -= downSpeed;
		//			draw[i].y -= tempJumpPower;
		//	}
		//	else
		//	{
		//		isJumping = false;
		//		// tempJumpPower = 100;
		//	}

		//	cout << tempJumpPower << endl;
		//}
		//else
		//{
		//	if (draw[2].y < 575)	// 지면에 닿지 않았을 경우 => 수치 수정 예정 
		//	{
		//		// tempJumpPower += tempGravity * 0.1;
		//		for (int i = 0; i < 4; i++)
		//			draw[i].y += tempGravity * 0.1;
		//	}
		//	else
		//	{
		//		isJumping = false;
		//		tempJumpPower = 95;	// 지면에 닿을 경우 => 점프 가능
		//	}
		//	// todo : 땅에 닿지 않은 경우 닿을 때까지 처리해야 함
		//}
		//// 점프
	//}
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
	}
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
