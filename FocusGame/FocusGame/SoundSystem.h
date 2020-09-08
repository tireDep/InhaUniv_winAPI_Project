#pragma once
#include "stdafx.h"
#include "ObjectClass.h"
#pragma comment(lib, "winmm.lib")

#include <mmsystem.h>
#include <Digitalv.h>

// >> sound
// >> mci_xx 변수 밑줄 제거

class SoundSystem : public Object
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

	void Update();

	void SetIsPause(bool set);
	void SetIsStop(bool set);
};
