#pragma once

#include <xaudio2.h>
#include "gameObject.h"


class Audio : public GameObject
{
private:
	static IXAudio2*				audioXAudio;
	static IXAudio2MasteringVoice*	masteringVoice;

	IXAudio2SourceVoice*	sourceVoice;
	BYTE*					soundData;

	int						audioLength;
	int						playLength;

	float volume;
public:
	static void InitMaster();
	static void UninitMaster();

	void Init() {};
	void Uninit();
	void Update() {};
	void Draw() {};

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void Stop();
	void SetVolume(float Volume) { volume = Volume; }



};

