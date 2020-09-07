// FocusGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "FocusGame.h"

// >> --------------------------
#include "MapClass.h"
#include "PlayerClass.h"
#include "GameManager.h"
#include "CannonClass.h"
#include "BulletClass.h"
#include "ExplodeClass.h"
#include "UIClass.h"

using namespace std;
// << --------------------------

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FOCUSGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FOCUSGAME));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOCUSGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FOCUSGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, eWindowWidth, eWindowHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	GameManager *gameManager = GameManager::GetInstance();
	Map *map = Map::GetInstance();
	Player *player = Player::GetInstance();
	Bullet *bulletList = Bullet::GetInstance();
	Explode *explodeList = Explode::GetInstance();
	UI *ui = UI::GetInstance();
	
	static vector<Obstacle *>obstacle;
	static vector<Object *> object;

    switch (message)
    {
	case WM_CREATE:
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);

		SetTimer(hWnd, 0, 25, NULL);	// playeGame
		SetTimer(hWnd, 50, 50, NULL);	// animationFrame 
		SetTimer(hWnd, 100, 100, NULL);	// resetGame

		gameManager->CalcScreenSize(hWnd);
		gameManager->SetIsPlayerLive(true);

		object.push_back(map);
		object.push_back(player);

		Obstacle::AddWeapon(obstacle, map->CheckInCannon());

		break;

	case WM_TIMER:
		if (gameManager->GetSceneNum() == eGameScene)
		{
			bool isPlayerLive = gameManager->GetIsPlayerLive();

			if (!gameManager->GetIsPause() && map->GetIsNextStage())
			{
				bulletList->Reset();
				explodeList->Reset();

				map->SetNextStage();
				player->Reset();

				Obstacle::AddWeapon(obstacle, map->CheckInCannon());

				map->SetIsNextStage(false);
			}

			if (!gameManager->GetIsPause() && wParam == 100 && !isPlayerLive)
			{
				for (int i = 0; i < object.size(); i++)
					object[i]->Reset();

				for (int i = 0; i < obstacle.size(); i++)
					obstacle[i]->Reset();

				bulletList->Reset();
				explodeList->Reset();
				// >> Reset
				
				// >> 화면 전환 타이머
				time_t nowTime;
				struct tm *tmTime = localtime(&nowTime);
				float checkTime = 0;
				float countDown = 2;

				while (countDown > 0)
				{
					time(&nowTime);
					tmTime = localtime(&nowTime); 
					if (tmTime->tm_sec != checkTime)
					{
						checkTime = tmTime->tm_sec;
						countDown--;
					}

					if (countDown <= 0)
					{
						gameManager->SetIsPlayerLive(true);
						player->SetIsPlayerDead(false);
					}
				}
				// >> 화면 전환 타이머
			}
			
			if (!gameManager->GetIsPause() && wParam == 0 && isPlayerLive)
			{
				gameManager->SetNowMap(map->GetMapPos());
				gameManager->SetNowPlayerPos(player->GetPlayerPos());

				for (int i = 0; i<object.size(); i++)
					object[i]->Update();

				for (int i = 0; i < obstacle.size(); i++)
					obstacle[i]->Update();

				bulletList->Update();
				explodeList->Update();
				explodeList->SetNextFrame();
			}
		}

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_LBUTTONDOWN:
		if (gameManager->GetIsPause() && gameManager->GetNowScene() == eGameScene)
		{
			POINT input;
			input.x = LOWORD(lParam);
			input.y = HIWORD(lParam);
			ui->CheckPushBtn(input);
		}
		break;

	case WM_KEYDOWN:
		if (gameManager->GetSceneNum() == eMainScene)
		{
			gameManager->SetSceneNum(eChangeScene);
			// todo : 화면전환 애니메이션 후 씬 넘버 변경
			// todo : 키입력이 게임화면까지 넘어감
		}
		break;

	case WM_CHAR:
		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001 && gameManager->GetSceneNum() == eGameScene)
			gameManager->SetIsPause();

		if (GetAsyncKeyState(VK_TAB) & 0x0001 && gameManager->GetDrawRect() == false)
			gameManager->SetDrawRect(true);
		else
			gameManager->SetDrawRect(false);

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HDC memDc;
		HBITMAP hBit, oldBit;

		RECT rectView = gameManager->GetScreenSize();

		memDc = CreateCompatibleDC(hdc);
		hBit = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
		oldBit = (HBITMAP)SelectObject(memDc, hBit);

		HBRUSH hBrush, oldBrush;
		hBrush = CreateSolidBrush(RGB(36, 36, 36));
		oldBrush = (HBRUSH)SelectObject(memDc, hBrush);

		PatBlt(memDc, rectView.left, rectView.top, rectView.right, rectView.bottom, PATCOPY);
		// PATCOPY : dc에 있는 브러시 색상 그대로 출력
		if (gameManager->GetNowScene() == eMainScene || gameManager->GetNowScene() == eChangeScene)
			ui->RenderObject(hWnd, memDc);

		else if (gameManager->GetNowScene() == eGameScene)
		{
			SelectObject(memDc, oldBrush);
			DeleteObject(hBrush);

			if (gameManager->GetIsPlayerLive())
			{
				for (int i = 0; i<object.size(); i++)
					object[i]->RenderObject(hWnd, memDc);

				bulletList->RenderObject(hWnd, memDc);
				explodeList->RenderObject(hWnd, memDc);

				// >> drawObject
				player->DrawObject(memDc);
				map->DrawObject(memDc);

				for (int i = 0; i < obstacle.size(); i++)
					obstacle[i]->DrawObject(memDc);

				bulletList->DrawObject(memDc);
				explodeList->DrawObject(memDc);
				// >> drawObject
			}

			ui->RenderObject(hWnd, memDc);
		}

		else if(gameManager->GetSceneNum() == eResultScene)
			ui->RenderObject(hWnd, memDc);

		BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, memDc, 0, 0, SRCCOPY);

		SelectObject(memDc, oldBit);
		DeleteObject(hBit);
		DeleteDC(memDc);

		EndPaint(hWnd, &ps);
	}
	break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
   
    case WM_DESTROY:
		FreeConsole();

		Obstacle::DeleteAllData(obstacle);
		// todo : 수정 가능하면 수정해 볼 것

		KillTimer(hWnd, 0);
		KillTimer(hWnd, 50);
		KillTimer(hWnd, 100);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
