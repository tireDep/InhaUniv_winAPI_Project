#include "stdafx.h"
#include "FocusGame.h"
#include "PlayerClass.h"

Player::Player()
{
	//if(gameManger->GetScreenSize().right != 0)
	//	SetPos(playerPos, gameManger->GetScreenSize().right / 2, gameManger->GetScreenSize().bottom / 2, ePlayerSize);
	//else
	// todo : 플레이어 정보가 파싱된 위치로 이동해야 함
	SetPos(playerPos, eTrueWinWidth / 2, eTrueWinHeight / 2, ePlayerSize);

	playerState = eIdle;
	isJump = false;
	jumpPower = 0;

	isBtmGround = false;

	moveDirection = eIdle;
	moveSpeed = eMoveSpeed;
	gravity = eGravity;

	focusGauge = eFocusLv3;
	focusLv = eFocusLv3;
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
	int diffNum = 0;
	if (playerState != eFocus || playerState != eJump)
	{
		for (int i = 0; i < 4; i++)
			playerPos[i].y += gravity * 0.1;

		if (CheckBtmGround(diffNum))
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y += diffNum;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y -= diffNum;
		}
	}
}

bool Player::CheckBtmGround(int &lengthDiff)
{
	//// todo : 바닥과의 충돌 판정이 필요함!
	//// 맵정보가 인자로 필요함
	//// 장애물인지 벽돌인지 판별 필요함!

	//RECT temp;
	//vector<MapTile> checkBtm = gameManger->GetMap();
	//RECT checkRect;

	//checkRect.left = playerPos[0].x;
	//checkRect.top = playerPos[0].y;
	//checkRect.right = playerPos[2].x;
	//checkRect.bottom = playerPos[2].y;

	//for (int i = 0; i < checkBtm.size(); i++)
	//{
	//	if (IntersectRect(&temp, &checkBtm[i], &checkRect))
	//	{
	//		isBtmGround = true;
	//		lengthDiff = checkRect.bottom - checkBtm[i].top;
	//		//printf("%d\n", lengthDiff);

	//		return false;
	//	}
	//	else
	//		isBtmGround = false;
	//}
	// return true;

	return true;
}

inline bool Player::CheckUpGround(int & lengthDiff)
{
	/*RECT temp;
	vector<MapTile> checkBtm = gameManger->GetMap();
	RECT checkRect;

	checkRect.left = playerPos[0].x;
	checkRect.top = playerPos[0].y;
	checkRect.right = playerPos[2].x;
	checkRect.bottom = playerPos[2].y;

	for (int i = 0; i < checkBtm.size(); i++)
	{
		if (IntersectRect(&temp, &checkBtm[i], &checkRect))
		{
			isBtmGround = true;
			lengthDiff = checkBtm[i].bottom - checkRect.top;
			printf("%d\n", lengthDiff);

			return false;
		}
		else
			isBtmGround = false;
	}

	return true;*/

	return true;
}

void Player::DrawObject(HDC hdc)
{
	//if(playerState == eFocus)
	Polygon(hdc, focusPos, 4);
	// todo : 반투명한 이미지로 대체

	Polygon(hdc, playerPos, 4);

	if (playerState == eFocus)
		Polygon(hdc, fMovePos, 4);
}

void Player::MovePlayer()
{
	static bool isCharing = false;
	// todo : 이동 가능 판별 (지형 판별)

	// 수평이동 포물선!
	// test
	// POINT speed;
	// float v;
	// 
	// static float time;
	// 
	// if (lastMove.x != 0)
	// {
	// 	speed.x = lastMove.x - fCenterPos.x;
	// 	speed.y = lastMove.y - fCenterPos.y;
	// 
	// 	v = sqrt((pow(speed.x, 2) + pow(speed.y, 2)));
	// 	//printf("tt : %d %d\n", lastMove.x, lastMove.y);
	// 	printf("speed : %d %d, v : %f\n", speed.x, speed.y, v);
	// 	// 속력 계산
	// 
	// 	POINT calcV;
	// 	calcV.x = v;
	// 	calcV.y = gravity * time;
	// 
	// 	float objV = sqrt(pow(calcV.x, 2) + pow(calcV.y, 2));
	// 
	// 	POINT pos;
	// 	pos.x = calcV.x * time;
	// 	pos.y = 0.5 * gravity * time * time;
	// 
	// 	if (speed.x > 0)
	// 	{
	// 		playerPos[0].x -= pos.x;
	// 		playerPos[0].y += pos.y;
	// 
	// 		playerPos[1].x -= pos.x;
	// 		playerPos[1].y += pos.y;
	// 
	// 		playerPos[2].x -= pos.x;
	// 		playerPos[2].y += pos.y;
	// 
	// 		playerPos[3].x -= pos.x;
	// 		playerPos[3].y += pos.y;
	// 	}
	// 	else if (speed.x < 0)
	// 	{
	// 		playerPos[0].x += pos.x;
	// 		playerPos[0].y += pos.y;
	// 
	// 		playerPos[1].x += pos.x;
	// 		playerPos[1].y += pos.y;
	// 
	// 		playerPos[2].x += pos.x;
	// 		playerPos[2].y += pos.y;
	// 
	// 		playerPos[3].x += pos.x;
	// 		playerPos[3].y += pos.y;
	// 	}
	// 	else if (speed.x == 0)
	// 	{
	// 		playerPos[0].y += pos.y;
	// 
	// 		playerPos[1].y += pos.y;
	// 
	// 		playerPos[2].y += pos.y;
	// 
	// 		playerPos[3].y += pos.y;
	// 	}
	// 
	// 	if (playerPos[2].y > 550)
	// 		lastMove.x = 0;
	// 
	// 	time = 0.1;
	// }
	// else
	// 	time = 0;
	// 
	// test

	// test2
	POINT speed;
	float v;

	int degree = 120;
	float time = 0.05;

	if (lastMove.x != 0 && playerState != eFocus)
	{
		speed.x = lastMove.x - fCenterPos.x;
		speed.y = lastMove.y - fCenterPos.y;

		v = sqrt((pow(speed.x, 2) + pow(speed.y, 2)));
		//printf("tt : %d %d\n", lastMove.x, lastMove.y);
		//printf("speed : %d %d, v : %f\n", speed.x, speed.y, v);
		// 속력 계산

		float halfG = gravity * 0.5;
		POINT calcV;
		calcV.x = v * cos(degree);
		calcV.y = v * sin(degree) - halfG * time;

		float finV = sqrt(pow(calcV.x, 2) + pow(calcV.y, 2));

		POINT pos;
		pos.x = v * cos(degree)*time;
		pos.y = v *sin(degree)*time - (0.5*halfG*pow(time, 2));

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

		int temp = 0;
		if (CheckBtmGround(temp))	// 충돌처리 판정 필요
		{
			lastMove.x = 0;
			playerState = eIdle;
		}
		else
			playerState = eFall;
	}

	// test2

	if (playerState != eFocus)
	{
		// 플레이어 이동
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y += eMoveSpeed;

			int diffNum;
			if (CheckBtmGround(diffNum))
			{
				for (int i = 0; i < 4; i++)
					playerPos[i].y += diffNum;
			}
			else
			{
				for (int i = 0; i < 4; i++)
					playerPos[i].y -= diffNum;
			}
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
		if (!isJump && ((GetAsyncKeyState(VK_SPACE) & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000))
			&& GetKeyState(0x41) >= 0)	// 포커스 풀리자마자 뛰는 것 방지
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

			int temp;
			if (!CheckUpGround(temp))
			{
				playerState = eFall;

				jumpPower = 0;
				for (int i = 0; i < 4; i++)
					playerPos[i].y += temp;
				printf("------------------------------------------------------e456");
			}
		}
		else
		{
			//// 점프 하는 동안에 점프 못하게 막아야 함
			//// 바닥에 닿았을 경우 리셋 & 바로 점프 x
			if (((GetKeyState(VK_SPACE) < 0) || (GetKeyState(VK_UP) < 0)))
				isJump = true;	// jump키를 누르고 있는 상황
			else if (isBtmGround)
			{
				// jump키를 누르고 있지 x, 지면에 닿음 -> 변수 초기화, n번 점프 방지
				isJump = false;
				playerState = eIdle;
				jumpPower = eJumpPower;
			}
		}	// else_jump
			// 점프

			// 포커스
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

			if (focusGauge <= eSmallFocus || GetKeyState(0x41) < 0)	// 계속 누르고 있으면 포커스 모드 실행 x
				isCharing = true;
			else
				isCharing = false;

			CalcCenterPos();
			SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
		}
		// 포커스
	}

	else
	{
		if (((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(0x61) & 0x8000)))
		{
			if (focusGauge > eSmallFocus)
			{
				focusGauge -= 0.5;
				SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
			}
			else
			{
				isCharing = true;
				playerState = eIdle;
				// 게이지가 다 달면 이동하지 x
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


		// 포커스 내부에서만 이동가능해야 함
		// todo : 이동좌표가 맵 밖인지 && 맵인지 판별 필요(이동 x)
		POINT checkCenter;
		checkCenter.x = (fMovePos[0].x + fMovePos[2].x) / 2;
		checkCenter.y = (fMovePos[0].y + fMovePos[2].y) / 2;

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			moveDirection = eMoveUp;
			if (checkCenter.y > focusPos[0].y)
				MoveFocusPos(moveDirection, -1);
			else
				MoveFocusPos(moveDirection, 1);
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			moveDirection = eMoveDown;
			if (checkCenter.y < focusPos[2].y)
				MoveFocusPos(moveDirection, 1);
			else
				MoveFocusPos(moveDirection, -1);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			moveDirection = eMoveLeft;
			if (checkCenter.x > focusPos[0].x)
				MoveFocusPos(moveDirection, -1);
			else
				MoveFocusPos(moveDirection, 1);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			moveDirection = eMoveRight;
			if (checkCenter.x < focusPos[2].x)
				MoveFocusPos(moveDirection, 1);
			else
				MoveFocusPos(moveDirection, -1);
		}

		if (GetKeyState(VK_UP) >= 0 && GetKeyState(VK_DOWN) >= 0 && GetKeyState(VK_LEFT) >= 0 && GetKeyState(VK_RIGHT) >= 0)
		{
			//  키가 눌리고 있지 않을 경우 플레이어 위치로 초기화
			SetPos(fMovePos, centerPos.x, centerPos.y, efMoveSize);
		}

		CalcFCenterPos();
	}
	// Player Control
}

void Player::MoveFocusPos(int direction, int moveVal)
{
	if ((direction == eMoveUp && moveVal == -1) || (direction == eMoveDown && moveVal == 1))
	{
		for (int i = 0; i < 4; i++)
			fMovePos[i].y += eMoveSpeed * moveVal;
	}
	else if (direction == eMoveUp || direction == eMoveDown)
	{
		for (int i = 0; i < 4; i++)
			fMovePos[i].y += moveVal;
	}

	if ((direction == eMoveLeft && moveVal == -1) || (direction == eMoveRight && moveVal == 1))
	{
		for (int i = 0; i < 4; i++)
			fMovePos[i].x += eMoveSpeed * moveVal;
	}
	else if (direction == eMoveLeft || direction == eMoveRight)
	{
		for (int i = 0; i < 4; i++)
			fMovePos[i].x += moveVal;
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

inline void Player::CalcFCenterPos()
{
	fCenterPos.x = (fMovePos[0].x + fMovePos[1].x) / 2;
	fCenterPos.y = (fMovePos[0].y + fMovePos[2].y) / 2;
}
