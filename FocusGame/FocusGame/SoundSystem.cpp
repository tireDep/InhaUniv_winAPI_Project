#include "stdafx.h"
#include "SoundSystem.h"

#include "PlayerClass.h"
#include "GameManager.h"

#define dGameManager GameManager::GetInstance()
#define dPlayer Player::GetInstance()

SoundSystem::SoundSystem()
{
	// >> Main
	char change[] = "Sound/BGM/Main_groove.wav";
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

	dwID.push_back(mciOpen.wDeviceID);
	// >> Main

	// >> Game
	char change3[] = "Sound/BGM/Game_bgm.wav";
	//char change3[] = "Sound/BGM/Game_Alone.mp3";
	// char change3[] = "Sound/BGM/test.wav";
	wchar_t wText3[128];
	mbstowcs(wText3, change3, strlen(change3) + 1);
	LPWSTR ptr3 = wText3;
	mciGame.lpstrElementName = ptr3;

	char change4[] = "mpegvideo";
	wchar_t wText4[128];
	mbstowcs(wText4, change4, strlen(change4) + 1);
	LPWSTR ptr4 = wText4;
	mciGame.lpstrDeviceType = ptr4;

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciGame);

	dwID.push_back(mciGame.wDeviceID);
	// >> Game

	// >> End
	char change5[] = "Sound/BGM/Result_end.wav";
	wchar_t wText5[128];
	mbstowcs(wText5, change5, strlen(change5) + 1);
	LPWSTR ptr5 = wText5;
	mciEnd.lpstrElementName = ptr5;

	char change6[] = "mpegvideo";
	wchar_t wText6[128];
	mbstowcs(wText6, change6, strlen(change6) + 1);
	LPWSTR ptr6 = wText6;
	mciEnd.lpstrDeviceType = ptr6;

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciEnd);
	// >> End

	dwID.push_back(mciEnd.wDeviceID);

	nowID = dwID[0];
	mciSendCommand(nowID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay);
	// play & repeat
}

SoundSystem::~SoundSystem()
{
	if (dwID.size() > 0)
	{
		for(int i=0;i<dwID.size();i++)
			mciSendCommand(dwID[i], MCI_CLOSE, 0, NULL);
		// >> 메모리 삭제 
	}
}

SoundSystem * SoundSystem::GetInstance()
{
	static SoundSystem soundSys;
	return &soundSys;
}

void SoundSystem::Update()
{
	if (dGameManager->GetNowScene() != eMainScene)
	{
		mciSendCommandW(nowID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
		nowID = dwID[1];
		mciSendCommand(nowID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay);
	}
}

void SoundSystem::PlaySoundEffect()
{
	sndPlaySoundA(".\\sound\\SEF\\tone.wav", SND_ASYNC | SND_NODEFAULT);
}

void SoundSystem::PlayDeadSound()
{
	sndPlaySoundA(".\\sound\\SEF\\dead.wav", SND_ASYNC | SND_NODEFAULT);
}

void SoundSystem::PlayShotSound()
{
	sndPlaySoundA(".\\sound\\SEF\\shot.wav", SND_ASYNC | SND_NODEFAULT);
}

void SoundSystem::PlayExplodeSound()
{
	sndPlaySoundA(".\\sound\\SEF\\explosion.wav", SND_ASYNC | SND_NODEFAULT);
}

void SoundSystem::PlayFocusSound()
{
	sndPlaySoundA(".\\sound\\SEF\\focus.wav", SND_ASYNC | SND_NODEFAULT);
}

void SoundSystem::PlayGateBreak()
{
	sndPlaySoundA(".\\sound\\SEF\\gateBreak.wav", SND_ASYNC | SND_NODEFAULT);
}

void SoundSystem::PlayBtnOff()
{
	sndPlaySoundA(".\\sound\\SEF\\btnOff.wav", SND_ASYNC | SND_NODEFAULT);
}

void SoundSystem::PlayResultBgm()
{
	mciSendCommandW(nowID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
	nowID = dwID[2];

	SetFirstPos();
	mciSendCommand(nowID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay);
}

void SoundSystem::SetIsPause(bool set)
{
	if (dPlayer->GetIsPlayerDead())
		SetFirstPos();

	if (set == true)
		mciSendCommandW(nowID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
	else
		mciSendCommandW(nowID, MCI_RESUME, 0, NULL);
}

void SoundSystem::SetIsStop(bool set)
{
	SetFirstPos();
	mciSendCommandW(nowID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
	nowID = dwID[0];

	if (set == true)
		PlayResultBgm();

	else if (set == false)
	{
		SetFirstPos();
		mciSendCommand(nowID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&mciPlay);
	}
}

void SoundSystem::SetFirstPos()
{
	mciSendCommand(nowID, MCI_SEEK, MCI_SEEK_TO_END, (DWORD)(LPVOID)NULL);	// >> 맨 처음 위치로 이동
}
