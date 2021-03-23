#pragma once
#include "../framework.h"
#include "../Other/UserException.h"
#include "fmod.hpp"

using namespace FMOD;

typedef struct _tagSoundInfo
{
	Sound* pSound;
	bool bLoop;
	SOUND_CHANNEL eChannel;
}SOUNDINFO, *PSOUNDINFO;

class SoundManager
{
	DECLARE_SINGLE(SoundManager)
private:
	class SoundException : public UserException
	{
	private:
		wstring msg;
	public:
		SoundException(int codeLine, const char* fileName, const wchar_t* message);
		virtual const wchar_t* GetType() const noexcept;
		virtual wstring GetErrorMessage() const noexcept;
	};
private:
	System* m_pSystem;
	Channel* m_pChannel[SD_END];
	unordered_map<string, PSOUNDINFO> m_mapSound;
public:
	bool Init();
	void Update(float dt);
	bool LoadSound(const string& strKey, bool bLoop, SOUND_CHANNEL eChannel,
					const char* pFileName, const string& strPathKey = SOUND_PATH);
	bool IsEnd(SOUND_CHANNEL eChannel);
	bool IsEnd(const string& strKey);
	void PlaySound(const string& strKey);
	void PauseSound(const string& strKey);
	void PauseSound(SOUND_CHANNEL eChannel);
	void AdjustVolume(SOUND_CHANNEL eChannel, float vol);
private:
	PSOUNDINFO FindSound(const string& strKey);
};