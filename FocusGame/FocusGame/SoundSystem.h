#pragma once
#include "stdafx.h"
// >> stdafx.hø° include ¿÷¿Ω

#include "ObjectClass.h"
#pragma comment(lib, "winmm.lib")

#include <mmsystem.h>
#include <Digitalv.h>

using namespace std;

class SoundSystem : public Object
{
private:
	MCI_OPEN_PARMS mciOpen;
	MCI_OPEN_PARMS mciGame;
	MCI_OPEN_PARMS mciEnd;

	MCI_PLAY_PARMS mciPlay;
	DWORD dDeviceID;

	vector<int> dwID;
	int nowID;

	SoundSystem();

public:
	~SoundSystem();

	static SoundSystem* GetInstance();

	void Update();

	void PlaySoundEffect();
	void PlayDeadSound();
	void PlayShotSound();
	void PlayExplodeSound();
	void PlayFocusSound();
	void PlayGateBreak();
	void PlayBtnOff();

	void PlayResultBgm();

	void SetIsPause(bool set);
	void SetIsStop(bool set);

	void SetFirstPos();
};
