#pragma once
#include "stdafx.h"
#pragma comment(lib, "winmm.lib")

#include <mmsystem.h>
#include <Digitalv.h>

// >> sound
// >> mci_xx ���� ���� ����

class SoundSystem
{
private:
	MCI_OPEN_PARMS mOpenParms;
	MCI_PLAY_PARMS mPlayParms;
	DWORD dDeviceID;
	MCI_OPEN_PARMS mciOpen;
	MCI_PLAY_PARMS mciPlay;

	int dwID;

	SoundSystem();

public:
	~SoundSystem();

	static SoundSystem* GetInstance();
};
