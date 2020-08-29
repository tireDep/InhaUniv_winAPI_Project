#include "stdafx.h"
#include "FocusGame.h"
#include "PlayerClass.h"

#define dAddFocusP 1.5
#define dMinusFocusP 1

#define dgameManger GameManager::GetInstance()

Player::Player()
{
	// todo : 플레이어 정보가 파싱된 위치로 이동해야 함
	SetPos(playerPos, eTrueWinWidth / 2, eTrueWinHeight / 2, ePlayerSize);

	playerState = eIdle;
	isJump = false;
	jumpPower = 0;

	isBtmGround = false;

	moveDirection = eIdle;
	moveSpeed = eMoveSpeed;
	gravity = eGravity;

	isCharging = false;
	focusGauge = eFocusLv3;
	focusLv = eFocusLv3;
	// todo : 첫 시작은 0으로 해야함 -> 추후 아이템 구현시 수정

	CalcCenterPos();

	SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
	SetPos(fMovePos, centerPos.x, centerPos.y, efMoveSize);
	CalcFCenterPos();

	SetPos(lastPlayerPos, centerPos.x, centerPos.y, efMoveSize);
	lastMoveCenter.x = 0;
	lastMoveCenter.y = 0;
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
	CanMovePlayer();
}

void Player::Gravity()
{
	int diffNum = 0;
	if (playerState != eFocus || playerState != eJump)
	{
		for (int i = 0; i < 4; i++)
			playerPos[i].y += gravity * dTimeSec;

		if (CheckBtmGround(diffNum))
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y += diffNum + dTimeSec;
		}
		else
		{
			for (int i = 0; i < 4; i++)
				playerPos[i].y -= diffNum + dTimeSec;
		}
	}
}

bool Player::CheckBtmGround(int &lengthDiff)
{
	RECT temp;
	vector<TileMap> checkBtm = dGameManger->GetNowMap();
	RECT checkRect = ConversionRect(playerPos);

	for (int i = 0; i < checkBtm.size(); i++)
	{
		if (IntersectRect(&temp, &checkBtm[i].pos, &checkRect))
		{
			if (checkBtm[i].type == eMapBlock)	// 블록과 충돌할 경우 정지
			{
				isBtmGround = true;
				lengthDiff = checkRect.bottom - checkBtm[i].pos.top;
				return false;
			}
		}
		else
			isBtmGround = false;
	}
	 return true;
}

bool Player::CollisionMap(POINT pos[], int direction, int & lengthDiff)
{
	RECT areaRect;
	vector<TileMap> checkBtm = dGameManger->GetNowMap();
	RECT checkRect = ConversionRect(pos);

	if (direction == eMoveRight)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapBlock)
			{
				lengthDiff = checkBtm[i].pos.left - checkRect.right;
				return false;
			}
			else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapObstacle)
			{
				// todo : 게임오버 추가
				printf("hitobstacle\n");
				return true;
			}
		}
			return true;
	}

	if (direction == eMoveLeft)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapBlock)
			{
				lengthDiff = checkBtm[i].pos.right - checkRect.left;
				return false;
			}
			// else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapObstacle)
			// {
			// 	// todo : 게임오버 추가
			// 	printf("hitobstacle\n");
			// 	return true;
			// }
		}
		return true;
	}

	if (direction == eMoveUp || direction == eJump)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapBlock)
			{
				lengthDiff = checkBtm[i].pos.bottom - checkRect.top;
				return false;
			}
			// else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapObstacle)
			// {
			// 	// todo : 게임오버 추가
			// 	printf("hitobstacle\n");
			// 	return true;
			// }

		}
		return true;
	}

	if (direction == eMoveDown)
	{
		for (int i = 0; i < checkBtm.size(); i++)
		{
			if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapBlock)
			{
				lengthDiff = checkBtm[i].pos.top - checkRect.bottom;
				return false;
			}
			// else if (IntersectRect(&areaRect, &checkBtm[i].pos, &checkRect) && checkBtm[i].type == eMapObstacle)
			// {
			// 	// todo : 게임오버 추가
			// 	printf("hitobstacle\n");
			// 	return true;
			// }
		}
		return true;
	}

}

void Player::CheckOut(POINT pos[], int direction)
{
	int diffNum = 0;
	bool isCheckMap = CheckOutMap(pos, direction, diffNum);
	float mulNum = isCheckMap == true ? -1 : 1;
	float addNum = 0;

	if (direction == eMoveRight || direction == eMoveDown)
		addNum = -dCorrection;

	MovePlayer(pos, direction, diffNum, mulNum, addNum);
}

bool Player::CheckOutMap(POINT pos[], int direction, int &lengthDiff)
{
	RECT area;
	RECT checkRect = ConversionRect(pos);
	vector<TileMap> mapPos = dgameManger->GetNowMap();

	// todo : 해당 위치가 벽이 아니면 지나갈 수 있어야 함!!(스테이지 클리어 후)
	if (direction == eMoveLeft)
	{
		if (checkRect.left <= eLimitL)
		{
			for (int i = 0; i < mapPos.size(); i++)
			{
				if (IntersectRect(&area, &checkRect, &mapPos[i].pos) && mapPos[i].type != eMapBlock)
				{
					printf("Can Escape\n");
					return true;
				}
			}

			lengthDiff = eLimitL - checkRect.left;
			return false;
		}
		else
			return true;
	}

	if (direction == eMoveRight)
	{
		if (checkRect.right >= eLimitR)
		{
			lengthDiff = eLimitR - checkRect.right;
			return false;
		}
		else
			return true;
	}

	if (direction == eMoveDown)
	{
		if (checkRect.bottom >= eLimitB)
		{
			lengthDiff = eLimitB - checkRect.bottom;
			return false;
		}
		else
			return true;
	}

	if (direction == eMoveUp)
	{ 
		if (checkRect.top <= eLimitT)
		{
			lengthDiff = eLimitT - checkRect.top;
			return false;
		}
		else
			return true;
	}
}

bool Player::CheckBlockMap()
{
	RECT area;
	vector<TileMap> tempMap = dGameManger->GetNowMap();
	RECT conRect = ConversionRect(fMovePos);
	RECT conRect2 = ConversionRect(playerPos);
	// ※ : 포커스 좌표로 잡아야 맨 위 블럭도 판정 가능

	for (int i = 0; i < tempMap.size(); i++)
	{
		if (IntersectRect(&area, &tempMap[i].pos, &conRect) && tempMap[i].type == eMapBlock)	// 블럭인 경우에만 지나갈 수 x
		{
			playerPos[0] = lastPlayerPos[0];
			playerPos[1] = lastPlayerPos[1];
			playerPos[2] = lastPlayerPos[2];
			playerPos[3] = lastPlayerPos[3];

			lastMoveCenter.x = 0;
			lastMoveCenter.y = 0;

			playerState = eIdle;

			return false;
		}
	}

	for (int i = 0; i < tempMap.size(); i++)
	{
		if (IntersectRect(&area, &tempMap[i].pos, &conRect2) && tempMap[i].type == eMapBlock)	// 블럭인 경우에만 지나갈 수 x
		{
			playerPos[0] = lastPlayerPos[0];
			playerPos[1] = lastPlayerPos[1];
			playerPos[2] = lastPlayerPos[2];
			playerPos[3] = lastPlayerPos[3];

			lastMoveCenter.x = 0;
			lastMoveCenter.y = 0;

			playerState = eIdle;

			return false;
		}
	}

	lastPlayerPos[0] = playerPos[0];
	lastPlayerPos[1] = playerPos[1];
	lastPlayerPos[2] = playerPos[2];
	lastPlayerPos[3] = playerPos[3];

	return true;
}

void Player::DrawObject(HDC hdc)
{
	Polygon(hdc, focusPos, 4);
	//RECT temp = ConversionRect(focusPos);
	//Ellipse(hdc, temp.left, temp.top, temp.right, temp.bottom);
	// todo : 반투명한 이미지로 대체

	Polygon(hdc, playerPos, 4);

	if (playerState == eFocus)
		Polygon(hdc, fMovePos, 4);
}

void Player::CalcFocusMove()
{
	if (lastMoveCenter.x != 0 && lastMoveCenter.y != 0 && playerState != eFocus)
	{
		bool isCanMove = CheckBlockMap();	// 이동위치가 블록이 아닐 경우에만 판단함

		if (isCanMove)
		{
			bool isMomentum = FocusMomentum();

			int diffNum = 0;
			if (CheckBtmGround(diffNum) && isMomentum)
			{
				lastMoveCenter.x--;
				lastMoveCenter.y--;
			}
			else
			{
				playerState = eIdle;
				lastMoveCenter = { 0,0 };

				playerPos[0] = lastPlayerPos[0];
				playerPos[1] = lastPlayerPos[1];
				playerPos[2] = lastPlayerPos[2];
				playerPos[3] = lastPlayerPos[3];
				// 맵에 끼이는 것 방지
			}

		}
	}
}

bool Player::FocusMomentum()
{
	// 포물선 계산
	POINT speed;
	float v;

	int degree = 120;
	float time = 0.1;

	speed.x = lastMoveCenter.x - fCenterPos.x;
	speed.y = lastMoveCenter.y - fCenterPos.y;

	if (speed.x == 0 || speed.y == 0)
		return false;	// x축 직선 이동 시 바닥에서 미끄러지는 것 방지

	v = sqrt((pow(speed.x, 2) + pow(speed.y, 2)));
	// 속력 계산

	float halfG = gravity * 0.5;
	POINT calcV;
	if (speed.x > 0)
		degree = 210;

	calcV.x = v * cos(degree);
	calcV.y = v * sin(degree) - halfG * time;

	float finV = sqrt(pow(calcV.x, 2) + pow(calcV.y, 2));

	POINT pos;
	pos.x = speed.x * cos(degree)*time;
	pos.y = speed.y *sin(degree)*time - (0.5*halfG*pow(time, 2));

	if (speed.x == 0)
		pos.x = 0;
	else if (speed.x > 0)
		pos.x *= 1;
	else
		pos.x *= -1;

	if (speed.y == 0)
		pos.y = 0;
	else if (speed.y > 0)
		pos.y *= 1;
	else
		pos.y *= -1;

	if (pos.y == 1 || pos.y == -1)
		return false;	// 작은 짧은 위치 이동시 바닥에서 미끄러지는 것 방지

	playerPos[0].x += pos.x;
	playerPos[0].y += pos.y;

	playerPos[1].x += pos.x;
	playerPos[1].y += pos.y;

	playerPos[2].x += pos.x;
	playerPos[2].y += pos.y;

	playerPos[3].x += pos.x;
	playerPos[3].y += pos.y;

	return true;
}

void Player::CanMovePlayer()
{
	CalcFocusMove();

	if (playerState != eFocus)
	{
		// 플레이어 이동
		if (!isJump && (GetAsyncKeyState(VK_DOWN) & 0x8000))
		{
			moveDirection = eMoveDown;
			MovePlayer(playerPos, moveDirection, eMoveSpeed, 1, 0);

			int diffNum = 0;
			if (CheckBtmGround(diffNum))
				MovePlayer(playerPos, moveDirection, diffNum, 1, dCorrection);
			else
				MovePlayer(playerPos, moveDirection, diffNum, -1, dCorrection);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			moveDirection = eMoveRight;
			MovePlayer(playerPos, moveDirection, eMoveSpeed, 1, 0);

			int diffNum = 0;
			if (CollisionMap(playerPos, eMoveRight, diffNum))
				MovePlayer(playerPos, moveDirection, diffNum, -1, -dCorrection);
			else
				MovePlayer(playerPos, moveDirection, diffNum, 1, -dCorrection);
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			moveDirection = eMoveLeft;
			MovePlayer(playerPos, moveDirection, eMoveSpeed, -1, 0);

			int diffNum = 0;
			if (CollisionMap(playerPos, eMoveLeft, diffNum))
				MovePlayer(playerPos, moveDirection, diffNum, -1, 0);
			else
				MovePlayer(playerPos, moveDirection, diffNum, 1, 0);

		}
		// 플레이어 이동

		// 점프
		if (!isJump && ((GetAsyncKeyState(VK_SPACE) & 0x8000) )	// 점프 시작
			&& GetKeyState(0x41) >= 0)	// 포커스 풀리자마자 뛰는 것 방지
		{
			isJump = true;
			playerState = eJump;
			jumpPower = eJumpPower;
		}
		else if (isJump && playerState == eJump)	// 점프 중
		{
			// 약 7칸 정도 뛰어짐 (gravity = 185, jumpPower = 85 기준)
			jumpPower -= eGravity * dTimeSec;

			// >> 충돌 판정
			POINT checkRect[4];
			checkRect[0] = playerPos[0];
			checkRect[1] = playerPos[1];
			checkRect[2] = playerPos[2];
			checkRect[3] = playerPos[3];

			int diffNum = 0;
			int underLineNum = 0;
			for (int i = 1; i < jumpPower; i++)
			{
				MovePlayer(checkRect, playerState, 1, -1, 0);

				if (CollisionMap(checkRect, playerState, diffNum))
					underLineNum++;
				else
					underLineNum--;
			}
			// << 충돌 판정

			printf("%d\n", underLineNum);
			if (underLineNum > 0)
			{
				MovePlayer(playerPos, playerState, underLineNum, -1, 0);
			}
			else
			{
				playerState = eFall;
				MovePlayer(playerPos, playerState, 0, 1, 1);
			}
		}
		else // 점프 후 바닥에 닿음
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
		if (((GetAsyncKeyState(0x41) & 0x8000)) && !isCharging)
		{
			moveSpeed = 0;
			jumpPower = 0;
			gravity = 0;

			playerState = eFocus;
			CalcCenterPos();
			lastMoveCenter.x = centerPos.x;
			lastMoveCenter.y = centerPos.y;

			lastPlayerPos[0] = playerPos[0];
			lastPlayerPos[1] = playerPos[1];
			lastPlayerPos[2] = playerPos[2];
			lastPlayerPos[3] = playerPos[3];
			// 운동량 계산을 위한 변수 값 저장


			SetPos(fMovePos, centerPos.x, centerPos.y, efMoveSize);
		}

		else
		{
			moveSpeed = eMoveSpeed;
			gravity = eGravity;

			if (focusGauge < focusLv)
				focusGauge += dAddFocusP;

			if (focusGauge <= eSmallFocus || GetKeyState(0x41) < 0)	// 계속 누르고 있으면 포커스 모드 실행 x
				isCharging = true;
			else
				isCharging = false;

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
				focusGauge -= dMinusFocusP;
				SetPos(focusPos, centerPos.x, centerPos.y, focusGauge);
			}
			else
			{
				isCharging = true;
				playerState = eIdle;
				// 게이지가 다 달면 이동하지 x
			}
		}
		else // 게이지가 다 감소하기 전, 이동한 경우
		{
			CalcFCenterPos();
			SetPos(playerPos, fCenterPos.x, fCenterPos.y, ePlayerSize);
			playerState = eIdle;
		}

		RECT area;
		RECT rcMovepos = ConversionRect(fMovePos);
		RECT rcFPos = ConversionRect(focusPos);

		POINT checkCenter;
		checkCenter.x = (fMovePos[0].x + fMovePos[2].x) / 2;
		checkCenter.y = (fMovePos[0].y + fMovePos[2].y) / 2;

		if ( (GetKeyState(VK_UP) >= 0 && GetKeyState(VK_DOWN) >= 0 && GetKeyState(VK_LEFT) >= 0 && GetKeyState(VK_RIGHT) >= 0)
			|| !IntersectRect(&area, &rcMovepos, &rcFPos) )	// ※ : 영역 밖으로 나가게 되면 리셋
		{
			//  키가 눌리고 있지 않을 경우 플레이어 위치로 초기화
			SetPos(fMovePos, centerPos.x, centerPos.y, efMoveSize);
		}
		
		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			moveDirection = eMoveUp;
			int underLineNum = CheckFocusRange(moveDirection, -1);
			// >> 충돌 판정

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, -1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, 1);

			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			moveDirection = eMoveDown;
			int underLineNum = CheckFocusRange(moveDirection, 1);
			// >> 충돌 판정

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, 1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, -1);

			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}
	

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			moveDirection = eMoveLeft;
			int underLineNum = CheckFocusRange(moveDirection, -1);
			// >> 충돌 판정

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, -1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, 1);

			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			moveDirection = eMoveRight;
			int underLineNum = CheckFocusRange(moveDirection, 1);
			// >> 충돌 판정

			if (underLineNum > 0)
				MovePlayer(fMovePos, moveDirection, underLineNum, 1, 0);
			else
				MovePlayer(fMovePos, moveDirection, 0, 1, -1);

			CheckOut(fMovePos, moveDirection);
			CalcFCenterPos();
		}
	}
	// Player Control
}

void Player::MovePlayer(POINT pos[], int direction, int num, float mulNum, float addNum)
{
	if (direction == eMoveLeft || direction == eMoveRight)
	{
		for (int i = 0; i < 4; i++)
			pos[i].x += num * mulNum + addNum;
	}
	else if (direction == eMoveUp || direction == eMoveDown || playerState == eJump || playerState == eFall)
	{
		for (int i = 0; i < 4; i++)
			pos[i].y += num * mulNum + addNum;
	}
}

int Player::CheckFocusRange(int direction, int mulNum)
{
	RECT calcRect = ConversionRect(fMovePos);
	int num = 0;

	if (direction == eMoveUp || direction == eMoveDown)
	{
		for (int i = 1; i < eMoveSpeed; i++)
		{
			calcRect.top += 1 * mulNum;
			calcRect.bottom += 1 * mulNum;

			if ((calcRect.top + calcRect.bottom) * 0.5 >= focusPos[0].y && direction == eMoveUp)
				num++;
			else if ((calcRect.top + calcRect.bottom) * 0.5 <= focusPos[2].y && direction == eMoveDown)
				num++;
			else
				break;
		}
	}

	else if (direction == eMoveLeft || direction == eMoveRight)
	{
		for (int i = 1; i < eMoveSpeed; i++)
		{
			calcRect.left += 1 * mulNum;
			calcRect.right += 1 * mulNum;

			if ((calcRect.left + calcRect.right) * 0.5 >= focusPos[0].x && direction == eMoveLeft)
				num++;
			else if ((calcRect.left + calcRect.right) * 0.5 <= focusPos[2].x && direction == eMoveRight)
				num++;
			else
				break;
		}
	}

	return num;
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

RECT Player::ConversionRect(POINT pos[])
{
	RECT conversion;

	conversion.left = pos[0].x;
	conversion.top = pos[0].y;
	conversion.right = pos[2].x;
	conversion.bottom = pos[2].y;

	return conversion;
}


RECT Player::GetPlayerPos()
{
	return ConversionRect(playerPos);
}

RECT Player::GetFocusPos()
{
	return ConversionRect(focusPos);
}

bool Player::GetIsFocusMode()
{
	if (playerState == eFocus)
		return true;
	else
		return false;
}
