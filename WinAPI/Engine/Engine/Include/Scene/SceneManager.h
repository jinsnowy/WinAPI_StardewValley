#pragma once
#include "../framework.h"

class Object;
class Scene;
class SceneManager
{
	friend class App;
	DECLARE_SINGLE(SceneManager)
private:
	int m_iShowMode = 1;
	int m_iSignal = 0;
	static constexpr int m_iChangeSignal = 0xabcdef;
	SceneState m_tNextState = {};

	class Player* m_pPlayer = nullptr;
	Scene* m_pScene = nullptr;
	vector<Scene*> m_vecScene;
	void ChangeScene();
	void ChangeShowMode();
private:
	static constexpr float m_fSceneDelay = 0.5f;
	static constexpr float m_fSceneDrawPeriod = 0.04f;
	float m_fDelay = 0.f;
	void FadeOut();
	void FadeIn();
public:
	int GetShowMode()const { return m_iShowMode; }
	bool CheckShowMode(STAGE_SHOW eShow)
	{
		return m_iShowMode & (1 << eShow);
	}
	void ToggleShowMode(STAGE_SHOW eShow)
	{
		m_iShowMode ^= (1 << eShow);
	}
	bool NeedChangeScene(int result) { return result == m_iChangeSignal; }
	void SignalizeSceneChange(SceneState state);
	class Scene* GetScene() const { return m_pScene; }
public:
	bool Init();
	void Input(float dt);
	int Update(float dt);
	int LateUpdate(float dt);
	void Collision(float dt);
	void Draw(HDC hdc, float dt);
	template<typename T>
	T* CreateScene(SCENE_CREATE sc)
	{
		T* pScene = new T;

		if (m_vecScene[sc])
		{
			SAFE_DELETE(m_vecScene[sc]);
		}

		pScene->SetSceneType(sc);

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return nullptr;
		}

		m_vecScene[sc] = pScene;
		return pScene;
	}
};

