#pragma once
#include "UI.h"
class UIScrollBar :
    public UI
{
private:
	friend class Object;
private:
	bool m_bDrag = false;
	int m_iCurBlock = 0;
	int m_iNumBlock = 0;
	int m_iBlockHeight = 0;
	class UIButton* m_pScroller = nullptr;
	UIScrollBar();
	UIScrollBar(const UIScrollBar& ui);
	virtual ~UIScrollBar();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIScrollBar* Clone();
public:
	void SetNumBlock(int num)
	{
		m_iNumBlock = num;
	}
	int GetCurBlock() const
	{
		return m_iCurBlock;
	}
	class UIButton* GetScroller();
};
