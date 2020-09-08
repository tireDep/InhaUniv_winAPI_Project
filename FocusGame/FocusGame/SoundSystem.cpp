#include "stdafx.h"
#include "SoundSystem.h"

SoundSystem::SoundSystem()
{
	char change[] = "test.wav";
	wchar_t wText[128];
	mbstowcs(wText, change, strlen(change) + 1);
	LPWSTR ptr = wText;
	mciOpen.lpstrElementName = ptr;

	char change2[] = "mpegvideo";
	wchar_t wText2[128];
	mbstowcs(wText2, change2, strlen(change2) + 1);
	LPWSTR ptr2 = wText2;
	mciOpen.lpstrDeviceType = ptr2;

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);

	dwID = mciOpen.wDeviceID;

	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mPlayParms);
	// play & repeat

	// mciSendCommandW(dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mPlayParms);
	// // >> Pause
	// 
	// mciSendCommandW(dwID, MCI_RESUME, 0, NULL);
	// // >> Resume
	// 
	// mciSendCommandW(dwID, MCI_CLOSE, 0, NULL);
	// // >> stop

}

SoundSystem::~SoundSystem()
{

}

SoundSystem * SoundSystem::GetInstance()
{
	static SoundSystem soundSys;
	return &soundSys;
}
