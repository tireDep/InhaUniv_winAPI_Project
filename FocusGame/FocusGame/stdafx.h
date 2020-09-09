// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _AFXDLL
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include <afx.h>
#include <afxwin.h>
// >> window보다 먼저 include 되어야 함
// >> mci함수 관련(사운드)

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <vector>
// TODO: reference additional headers your program requires here
