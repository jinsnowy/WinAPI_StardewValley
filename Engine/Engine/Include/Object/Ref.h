#pragma once
#include "../framework.h"
class Ref
{
protected:
	Ref();
	virtual ~Ref() = 0;
protected:
	bool m_bEnable;
	bool m_bLife;
	int m_Ref;
	string m_strTag;
public:
	void AddRef() { ++m_Ref; }
	int Release();
	bool GetEnable() const { return m_bEnable; }
	bool GetLife() const { return m_bLife; }
	string GetTag() const { return m_strTag; }
	void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetTag(const string& tagName);
};
