#include "SoundManager.h"
#include "../Core/PathManager.h"
#define SOUND_EXP(MSG) SoundException(__LINE__, __FILE__, MSG)

DEFINITION_SINGLE(SoundManager)

SoundManager::SoundManager()
	: m_pSystem(nullptr)
{
}

SoundManager::~SoundManager()
{
	unordered_map<string, PSOUNDINFO>::iterator iter;
	unordered_map<string, PSOUNDINFO>::iterator iterEnd = m_mapSound.end();
	
	for (iter = m_mapSound.begin(); iter != m_mapSound.end(); ++iter)
	{
		iter->second->pSound->release();
		SAFE_DELETE(iter->second);
	}
	m_mapSound.clear();
	m_pSystem->close();
	m_pSystem->release();
}

bool SoundManager::Init()
{
	System_Create(&m_pSystem);
	if (m_pSystem->init(SD_END, FMOD_INIT_NORMAL, 0) != FMOD_OK)
		return false;
	return true;
}

void SoundManager::Update(float dt)
{
	if (m_pSystem->update() != FMOD_OK)
		throw SOUND_EXP(L"Sound System Update Failed\n");
}

bool SoundManager::LoadSound(const string& strKey,
									bool bLoop,
									SOUND_CHANNEL eChannel,
									const char* pFileName,
									const string& strPathKey)
{
	if (FindSound(strKey))
		return true;

	const char* pPath = PATH_MANAGER->FindPathByMultiByte(strPathKey);

	string fullPath;
	if (pPath)
		fullPath = pPath;
	fullPath += pFileName;

	PSOUNDINFO sound = new SOUNDINFO;

	sound->bLoop = bLoop;
	sound->eChannel = eChannel;

	const auto LOOP_MODE = bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	if (m_pSystem->createSound(fullPath.c_str(), LOOP_MODE, 0, &sound->pSound) != FMOD_OK)
	{
		SAFE_DELETE(sound);
		throw SOUND_EXP(L"Sound File Creation Failed\n");
	}
	
	m_mapSound.insert(make_pair(strKey, sound));

	return true;
}

bool SoundManager::IsEnd(SOUND_CHANNEL eChannel)
{
	bool isPlaying;
	m_pChannel[eChannel]->isPlaying(&isPlaying);
	return !isPlaying;
}

bool SoundManager::IsEnd(const string& strKey)
{
	PSOUNDINFO sound = FindSound(strKey);
	if (sound == nullptr)
		throw EXCEPT(L"Sound not found");

	bool isPlaying;
	m_pChannel[sound->eChannel]->isPlaying(&isPlaying);
	return !isPlaying;
}

void SoundManager::PlaySound(const string& strKey)
{
	PSOUNDINFO sound = FindSound(strKey);

	if (sound == nullptr)
		throw SOUND_EXP(L"Sound Key Not Found\n");

	Update(0.f);
	if (m_pSystem->playSound(sound->pSound, 0, false, &m_pChannel[sound->eChannel]) != FMOD_OK)
	{
		wchar_t msg[100] = L"";
		wstring wstrKey(strKey.begin(), strKey.end());
		wsprintf(msg, L"[%s] Sound Play Failed", wstrKey.c_str());
		throw SOUND_EXP(msg);
	}
}

void SoundManager::PauseSound(const string& strKey)
{
	PSOUNDINFO sound = FindSound(strKey);

	if (sound == nullptr)
		throw SOUND_EXP(L"Sound Key Not Found\n");

	m_pChannel[sound->eChannel]->stop();
}

void SoundManager::PauseSound(SOUND_CHANNEL eChannel)
{
	m_pChannel[eChannel]->stop();
}

void SoundManager::AdjustVolume(SOUND_CHANNEL eChannel, float vol)
{
	if (vol > 1.0f) vol = 1.0f;
	else if (vol < 0.0f) vol = 0.0f;
	m_pChannel[eChannel]->setVolume(vol);
}

PSOUNDINFO SoundManager::FindSound(const string& strKey)
{
	auto found = m_mapSound.find(strKey);
	if (found == m_mapSound.end())
		return nullptr;
	return found->second;
}

SoundManager::SoundException::SoundException(int codeLine, const char* fileName, const wchar_t* message)
	: UserException(codeLine, fileName), msg(message)
{
}

const wchar_t* SoundManager::SoundException::GetType() const noexcept
{
	return L"SoundManager Excpetion";
}

wstring SoundManager::SoundException::GetErrorMessage() const noexcept
{
	return SoundManager::SoundException::UserException::GetErrorMessage() + L"\n" + msg;
}
