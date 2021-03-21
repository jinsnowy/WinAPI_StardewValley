#pragma once
#include "Collider.h"

class ColliderPixel :
    public Collider
{
	friend class Object;
protected:
	ColliderPixel();
	ColliderPixel(const ColliderPixel& coll);
	virtual ~ColliderPixel();
private:
	int m_iWidth;
	int m_iHeight;
	vector<Pixel> m_vecPixel;
	string m_strFileName;
	string m_strPathKey;
public:
	const vector<Pixel>& GetPixel() const
	{
		return m_vecPixel;
	}
	int GetWidth() const
	{
		return m_iWidth;
	}
	int GetHeight() const
	{
		return m_iHeight;
	}
public:
	bool SetPixelInfo(const char* pFileName, const string& strPathKey = TEXTURE_PATH);
	Rect GetBounds() const;
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual bool CheckCollision(Collider* pDst);
	virtual void Draw(HDC hdc, float dt);
	virtual ColliderPixel* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};


