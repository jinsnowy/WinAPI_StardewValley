#pragma once
#include "../Object/Ref.h"
#include "../framework.h"

class AnimationClip
{
public:
	ANIMATION_TYPE	   eType;
	ANIMATION_OPTION   eOption;
	int	    iNextState;
	std::vector<class Texture*> vecTexture;
	float	fAnimationTime;
	float   fAnimationLimitTime;
	float	fAnimationFrameTime;
	int		iFrameX;
	int		iFrameY;
	int		iFrameMaxX;
	int		iFrameMaxY;
	int		iStartX;
	int		iStartY;
	int		iLengthX;
	int		iLengthY;
	float	fOptionTime;
	float	fOptionLimitTime;
	Vec2F	tFrameSize;
};

class Animation : public Ref
{
private:
	friend class Object;
private:
	Animation();
	Animation(const Animation& anim);
	~Animation();
private:
	unordered_map<string, AnimationClip*> m_mapClip;
	AnimationClip* m_pCurClip;
	string		   m_strCurClip;
	string		   m_strDefaultClip;
	class Object*  m_pObj;
public:
	AnimationClip* GetCurrentClip() const { return m_pCurClip; }
	void SetObj(class Object* pObj)
	{
		m_pObj = pObj;
	}
	bool AddClip(const string& strName,
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

	bool AddClip(const string& strName,
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
	void SetClipNextState(const string& strName, int iState);
	void SetClipColorKey(const string& strName, unsigned char r, unsigned char g, unsigned char b);
	void SetCurrentClip(const string& strCurClip);
	void SetDefaultClip(const string& strDefaultClip);
	void ChangeClip(const string& strClip);
private:
	AnimationClip* FindClip(const string& strName);
public:
	bool Init();
	void Update(float fTime);
	Animation* Clone();
public:
	void SaveFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void Save(FILE* pFile);
	void LoadFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void Load(FILE* pFile);
};
