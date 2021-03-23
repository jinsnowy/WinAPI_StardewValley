#pragma once
#include "../framework.h"
#include "../Collider/Collider.h"
#include "../Animation/Animation.h"
#include "../Resources/Texture.h"
#include "../Effect/EffectInterface.h"

class Object : public Ref
{
	friend class PrototypeManager;
protected:
	// 씬, 레이어, 텍스쳐, 콜라이더
	OBJ_TYPE m_eObjType = OBJ_NORMAL;
	bool m_bEnableAnimation = true;
	bool m_bUIObject = false;
	class Scene* m_pScene;
	class Layer* m_pLayer;
	class Texture* m_pTexture;
	EffectPtr m_pEffect;
	Animation* m_pAnimation;
	list<Collider*> m_ColliderList;
public:
	bool IsUIObject()const { return m_bUIObject; }
	bool HasEffect() const { return m_pEffect != nullptr; }
	void SetEffect(const EffectPtr& pEffect);
	OBJ_TYPE GetObjectType() const { return m_eObjType; }
	template<typename T>
	static T* CreateObject(const string& strTag)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);
		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return nullptr;
		}
		return pObj;
	}
	static Object* CreateObjectByType(OBJ_TYPE eType);

	void SetAnimationVisibility(bool enabled) { m_bEnableAnimation = enabled; }
	void SetClipColorKey(const string& strName, unsigned char r, unsigned char g, unsigned char b);
	Animation* CreateAnimation(const string& strTag)
	{
		SAFE_RELEASE(m_pAnimation);

		m_pAnimation = new Animation;

		m_pAnimation->SetTag(strTag);
		m_pAnimation->SetObj(this);

		if (!m_pAnimation->Init())
		{
			SAFE_RELEASE(m_pAnimation);
			return nullptr;
		}

		m_pAnimation->AddRef();

		return m_pAnimation;
	}
	bool AddAnimationClip(const string& strName,
		ANIMATION_TYPE eType, ANIMATION_OPTION eOption,
		float	fAnimationTime,
		float   fAnimationLimitTime,
		int		iFrameMaxX,
		int		iFrameMaxY,
		int		iStartX,
		int		iStartY,
		int		iLengthX,
		int		iLengthY,
		float	fOptionLimitTime,
		const string& strTexKey,
		const wchar_t* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	bool AddAnimationClip(const string& strName,
		ANIMATION_TYPE eType, ANIMATION_OPTION eOption,
		float	fAnimationTime,
		float   fAnimationLimitTime,
		int		iFrameMaxX,
		int		iFrameMaxY,
		int		iStartX,
		int		iStartY,
		int		iLengthX,
		int		iLengthY,
		float	fOptionLimitTime,
		const string& strTexKey,
		const vector<wstring>& vecFileName,
		const string& strPathKey = TEXTURE_PATH);
public:
	const list<Collider*>* GetColliderList() const
	{
		return &m_ColliderList;
	}
	void EraseAllColiders()
	{
		Safe_Release_VecList(m_ColliderList);
	}
	Collider* GetCollider(const string& strTag);
	template<typename T>
	void AddCollisionFunction(const string& strTag, COLLISION_STATE eState, T* pObj, void (T::* pFunc)(Collider*, Collider*, float))
	{
		list<Collider*>::iterator iter;
		list<Collider*>::iterator iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddCollisionFunction(eState, pObj, pFunc);
				break;
			}
		}
	}
	template<typename T>
	T* AddCollider(const string& strTag)
	{
		T* pCollider = new T;

		pCollider->SetObj(this);
		pCollider->SetTag(strTag);

		if (!pCollider->Init())
		{
			SAFE_RELEASE(pCollider);
			return nullptr;
		}

		pCollider->AddRef();
		m_ColliderList.push_back(pCollider);

		return pCollider;
	}
	bool CheckCollider() const
	{
		return !m_ColliderList.empty();
	}
protected:
	bool m_blsPhysics;
	float m_fGravityTime;
public:
	void ClearGravity()
	{
		m_fGravityTime = 0.f;
	}
	void SetPhysics(bool bPhysics)
	{
		m_blsPhysics = bPhysics;
	}
	bool GetPhysics() const
	{
		return m_blsPhysics;
	}
protected:
	bool m_bEnableTransparent = false;
	unsigned char m_iAlpha = 125;
	Pos   m_tPos;
	Pos   m_tPivot;
	Size  m_tImageOffset;
	Size  m_tSize;
public:
	void SetAlpha(unsigned char alpha) { m_iAlpha = alpha; }
	unsigned char GetAlpha() const { return m_iAlpha; }
	void EnableTransparentEffect() { m_bEnableTransparent = true; }
	void DisableTransparentEffect() { m_bEnableTransparent = false; }
public:
	Object();
	Object(const Object& obj);
	virtual ~Object();

	// 씬과 레이어 Get/Set
	void SetScene(class Scene* pScene) { m_pScene = pScene; }
	void SetLayer(class Layer* pLayer) { m_pLayer = pLayer; }
	class Scene* GetScene() const { return m_pScene; }
	class Layer* GetLayer() const { return m_pLayer; }

	// 객체 변수 Get/Set
	float GetLeft() const { return m_tPos.x - m_tSize.x * m_tPivot.x; }
	float GetTop() const { return m_tPos.y - m_tSize.y * m_tPivot.y; }
	float GetRight() const { return GetLeft() + m_tSize.x; }
	float GetBottom() const { return GetTop() + m_tSize.y; }
	Pos GetCenter() const
	{
		return GetTopLeft() + m_tSize * 0.5f;
	}
	Pos GetPos() const { return m_tPos; }
	Pos GetPivot() const { return m_tPivot; }
	Pos GetTopLeft() const { return m_tPos - m_tSize * m_tPivot; }
	Size GetSize() const { return m_tSize; }
	Size GetImageSize() const;
	Size GetImageOffset() const { return m_tImageOffset; }
	Texture* AccessTexture() const { return m_pTexture; }
	string GetTexTag() const;

	void SetAsTextureSize();
	void AddOffset(float x, float y) { m_tPos.x += x; m_tPos.y += y; }
	void AddOffset(const Pos& offset) { m_tPos += offset; }
	void SetPos(float x, float y) { m_tPos.x = x; m_tPos.y = y; }
	void SetSize(float x, float y) { m_tSize.x = x; m_tSize.y = y; }
	void SetPivot(float x, float y) { m_tPivot.x = x; m_tPivot.y = y; }
	void SetPos(const Pos& pos) { m_tPos = pos; }
	void SetSize(const Size& size) { m_tSize = size; }
	void SetPivot(const Pos& pivot) { m_tPivot = pivot; }
	void SetImageOffset(const Size& tOffset) { m_tImageOffset = tOffset; }
	void SetImageOffset(float x, float y) { m_tImageOffset = Size(x, y); }
public:
	void SetTexture(class Texture* pTexture);
	void SetTexture(const string& strKey, const wchar_t* pFileName = nullptr, const string& strPathKey = TEXTURE_PATH);
	void SetColorKey(unsigned char r, unsigned char g, unsigned char b);
	void SetAnimationClipColorKey(const string& strClip, unsigned char r, unsigned char g, unsigned char b);
	void SetClipNextState(const string& strName, int iState);
	inline void DrawImageAt(HDC hdc, const Pos& at, bool ignorePivot = false)
	{
		DrawImageAt(hdc, int(at.x), int(at.y), ignorePivot);
	}
	inline void DrawImageAt(HDC hdc, int px, int py, bool ignorePivot = false)
	{
		if (m_pTexture)
		{
			Size tSize = GetImageSize();
			if (!ignorePivot)
			{
				px = px - int(m_tPivot.x * tSize.x);
				py = py - int(m_tPivot.y * tSize.y);
			}
			m_pTexture->DrawImageAt(hdc, px, py);
		}
	}
	inline void DrawImageAtFixedSize(HDC hdc, const Pos& at, int size, bool ignorePivot = false)
	{
		DrawImageAtFixedSize(hdc, int(at.x), int(at.y), size, ignorePivot);
	}
	inline void DrawImageAtFixedSize(HDC hdc, int px, int py, int size, bool ignorePivot = false)
	{
		if (m_pTexture)
		{
			Size tSize = GetImageSize();
			if (!ignorePivot)
			{
				px = px - int(m_tPivot.x * tSize.x);
				py = py - int(m_tPivot.y * tSize.y);
			}
			m_pTexture->DrawImageAtFixedSize(hdc, px, py, size);
		}
	}
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Object* Clone() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void LateInit();
	virtual void StateTransit(int) {};
public:
	void SaveFromFile(FILE* pFile);
	void LoadFromFile(FILE* pFile);
	void SaveFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void SaveFromFullPath(const char* pFullPath);
	void LoadFromPath(const char* pFileNmae, const string& strPathKey = DATA_PATH);
	void LoadFromFullPath(const char* pFullPath);
};

