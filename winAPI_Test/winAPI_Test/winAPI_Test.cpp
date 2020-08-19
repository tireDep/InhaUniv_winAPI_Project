// winAPI_Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "winAPI_Test.h"

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
    LoadStringW(hInstance, IDC_WINAPI_TEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI_TEST));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI_TEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPI_TEST);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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

#include <iostream>

using namespace std;

HBITMAP hBackImage;
BITMAP bitBack;

static POINT draw[4] =
{
	{ 100,100 },{ 200,100 },{ 200,200 },{ 100,200 }
};

void CreateBitmap();
void DrawBitmap(HWND hWnd, HDC hdc);
void DeleteBitmap();

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool isPause = false;
	
	static int moveSpeed = 10;
	static int jumpHeight = 50;

	static int downSpeed = 20;

	static bool isFocus = false;
	static float focusPower = 0;
	static POINT focusPos[4] =
	{
		{150, 150}, {160, 150}, {160, 160}, {150, 160}
	};

	// static int i = 0;

    switch (message)
    {
	case WM_CREATE:
		AllocConsole();
		freopen("CONOUT$", "wt", stdout);

		SetTimer(hWnd, 0, 25, NULL);	// 플레이어 이동 및 제어

		CreateBitmap();
		break;

	case WM_TIMER:

		// if(wParam == 1)
		// {
		// 	while (draw[2].y < 500)
		// 	{
		// 		draw[i].y += moveSpeed;
		// 		if (i < 5) i++;
		// 		else i = 0;
		// 	}
		// }

		// Player Control

		// [0x8000] : 작동시 누르고 있다.
		// [0x0001] : 그 전에 눌렀었다.
		// [0x8001] : 두 경우 다 발생 함.
		// [0x0000] : 두 경우 다 발생 안함.

		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
		{
			if (isPause == false)
				isPause = true;
			else
				isPause = false;
		}

		if (!isPause)	// 일시정지가 아니고, 갈 수 있는 방향일 경우 && 갈 수 있는 상황인 경우
		{
			// todo : 이동 가능 판별 (지형 판별)

			if (!isFocus)
			{
				if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					for (int i = 0; i < 4; i++)
						draw[i].y += moveSpeed;
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					for (int i = 0; i < 4; i++)
						draw[i].x += moveSpeed;
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					for (int i = 0; i < 4; i++)
						draw[i].x -= moveSpeed;
				}
				// 플레이어 이동

				// 점프 하는 동안에 점프 못하게 막아야 함
				// 바닥에 닿았을 경우 리셋 & 바로 점프 x
				static int tempJumpPower = 100;

				static bool isJumping = false;

				if (tempJumpPower > 0 && !isJumping && ((GetAsyncKeyState(VK_SPACE) & 0x8000))) //  || (GetAsyncKeyState(VK_UP) & 0x0001))// && true)
				{
					// for (int i = 0; i < 4; i++)
					// 	draw[i].y -= moveSpeed * jumpHeight;
					// 기본 점프
					isJumping = true;
				}
				else if (isJumping)
				{
					if (tempJumpPower > 0)
					{
						tempJumpPower -= 10;
						for (int i = 0; i < 4; i++)
							draw[i].y -= downSpeed;
						// draw[i].y -= tempJumpPower;	// 땅바닥 근처에서 천천히 떨어짐
					}
					else
					{
						isJumping = false;
						// tempJumpPower = 100;
					}

					cout << tempJumpPower << endl;
				}
				else
				{
					if (draw[2].y < 650)	// 지면에 닿지 않았을 경우 => 수치 수정 예정 
					{
						for (int i = 0; i < 4; i++)
							draw[i].y += downSpeed;
					}
					else
					{
						isJumping = false;
						tempJumpPower = 100;	// 지면에 닿을 경우 => 점프 가능
					}
					// todo : 땅에 닿지 않은 경우 닿을 때까지 처리해야 함
				}
				// 점프
			}
			else
			{
				if (GetAsyncKeyState(VK_UP) & 0x8000)
				{
					for (int i = 0; i < 4; i++)
						focusPos[i].y -= 10;
				}
				if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				{
					for (int i = 0; i < 4; i++)
						focusPos[i].y += 10;
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000)
				{
					for (int i = 0; i < 4; i++)
						focusPos[i].x -= 10;
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
				{
					for (int i = 0; i < 4; i++)
						focusPos[i].x += 10;
				}
			}
			
			static bool isCharing = false;
			if (((GetAsyncKeyState(0x41) & 0x8000) || (GetAsyncKeyState(0x61) & 0x8000)) && !isCharing)
			{
				moveSpeed = 0;
				downSpeed = 0;
				// 포커스 모드시 이동 x
				// 1안 : 속도 제거
				// 2안 : boolean 변수로 제어

				if (focusPower <= 150)
				{
					isFocus = true;
					focusPower += 2.5;
				}
				else
				{
					isFocus = false;
					isCharing = true;
				}
				// todo : 플레이어보다 작아지면 강제로 풀려야 함
			}
			else
			{
				moveSpeed = 10;
				downSpeed = 20;
				// 포커스 모드시 이동 x
				// 1안 : 속도 제거
				// 2안 : boolean 변수로 제어

				isFocus = false;
				if (focusPower < 100)
					isCharing = false;
				else
					isCharing = true;

				if (focusPower > 0)
					focusPower -= 1.5;

				else if (focusPower < 0)
					focusPower = 0;
			}
			// focus
		}

		// Player Control

		InvalidateRect(hWnd, NULL, true);
		break;


	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		int tempSize = 150;
		POINT tempCenter;
		tempCenter.x = (draw[0].x + draw[1].x) / 2;
		tempCenter.y = (draw[0].y + draw[2].y) / 2;
		
		tempSize -= focusPower;

		// cout << focusPower << endl;
		Ellipse(hdc, tempCenter.x - tempSize, tempCenter.y - tempSize, tempCenter.x + tempSize, tempCenter.y + tempSize);
		// Ellipse(hdc, draw[0].x - 100, draw[0].y - 100, draw[2].x + 100, draw[2].y + 100);

		Polygon(hdc, draw, 4);

		if (isFocus == true)
			Polygon(hdc, focusPos, 4);
			// Ellipse(hdc, tempCenter.x - 10 - focusPos.x, tempCenter.y - 10 - focusPos.y, tempCenter.x + 10 + focusPos.x, tempCenter.y + 10 + focusPos.y);

		DrawBitmap(hWnd, hdc);

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
		
		KillTimer(hWnd,0);
		DeleteBitmap();

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

void CreateBitmap()
{
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("../Image/tt32.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	GetObject(hBackImage, sizeof(BITMAP), &bitBack);
}

void DrawBitmap(HWND hWnd, HDC hdc)
{
	HDC hMemDC;	// back Buffer
	HBITMAP hOldBitmap;
	int bx, by;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
	bx = bitBack.bmWidth;
	by = bitBack.bmHeight;

	for (int i = 0; i < 500; i+=100)
	{
		BitBlt(hdc, i,0, bx, by, hMemDC, 0, 0, SRCCOPY);
	}
	// 맵 테스트 출력

	BitBlt(hdc, draw[0].x, draw[0].y, bx, by, hMemDC, 0, 0, SRCCOPY);
	// 플레이어 테스트 출력

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

void DeleteBitmap()
{
	DeleteObject(hBackImage);
}