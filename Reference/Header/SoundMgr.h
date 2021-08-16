#pragma once
#ifndef _SoundMgr_h_

#define _SoundMgr_h_

// FMOD: ����Ƽ, �𸮾�� ���� ��� �������� �η� ���̴� ���� ���� ����.
// ���忡 ���õ� �������̽��� ����.

#include "fmod.h"
#include "Engine_Defines.h"
#include "Base.h"

#pragma comment(lib, "fmodex_vc.lib")

#include <io.h>

BEGIN(Engine)
class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

public:

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Ready_SoundMgr(const char* path);
	void Update_Sound();
	void Play_Sound(const wstring& wstrSoundKey, Engine::CHANNELID eID, _bool flag = false);
	void Play_BGM(const wstring& wstrSoundKey);
	void Stop_Sound(Engine::CHANNELID eID);
	void Stop_All();
	void Set_Volume(Engine::CHANNELID eID, float fVol); // ������ ������ �ּ� 0.f ~ �ִ� 1.f����.
	void Release();

private:
	void LoadSoundFile(const char* path);

private:
	// FMOD_SOUND: ���� ���Ͽ� ���� ������ ���� ����ü.
	map<wstring, FMOD_SOUND*>	m_MapSound;

	// FMOD_CHANNEL: ���带 ����ϰ� �����ϴ� ����. ex) ���� ���� ��.
	FMOD_CHANNEL*	m_pChannelArr[Engine::CHANNELID::MAX_CHANNEL];

	// FMOD_SYSTEM: FMOD_SOUND�� FMOD_CHANNEL�� �Ѱ� �����ϴ� ��ü.
	FMOD_SYSTEM*	m_pSystem;

public:
	virtual void	Free(void);
};
END
#endif // !_SoundMgr_h_