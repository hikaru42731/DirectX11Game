
#include "Main.h"
#include "Audio.h"

IXAudio2*				Audio::audioXAudio = NULL;
IXAudio2MasteringVoice*	Audio::masteringVoice = NULL;


void Audio::InitMaster()
{
	// COM������
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio����
	XAudio2Create(&audioXAudio, 0);

	// �}�X�^�����O�{�C�X����
	audioXAudio->CreateMasteringVoice(&masteringVoice);
}


void Audio::UninitMaster()
{
	masteringVoice->DestroyVoice();
	audioXAudio->Release();
	CoUninitialize();
}


void Audio::Load(const char *FileName)
{

	// �T�E���h�f�[�^�Ǎ�
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		soundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)soundData, buflen);


		audioLength = readlen;
		playLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}
	volume = 1.0f;
	// �T�E���h�\�[�X����
	audioXAudio->CreateSourceVoice(&sourceVoice, &wfx);
	assert(sourceVoice);
}


void Audio::Uninit()
{
	sourceVoice->Stop();
	sourceVoice->DestroyVoice();

	delete[] soundData;
}

void Audio::Play(bool Loop)
{
	sourceVoice->Stop();
	sourceVoice->FlushSourceBuffers();


	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = audioLength;
	bufinfo.pAudioData = soundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = playLength;

	// ���[�v�ݒ�
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = playLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	sourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	sourceVoice->SetVolume(volume, XAUDIO2_COMMIT_NOW);
	// �Đ�
	sourceVoice->Start();


}

void Audio::Stop()
{
	sourceVoice->Stop();
}



