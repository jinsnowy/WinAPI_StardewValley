#pragma once
#include <string>
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
	size_t m_id;
	std::string m_strTag;
public:
	void AddRef() { ++m_Ref; }
	int Release()
	{
		--m_Ref;
		if (m_Ref == 0)
		{
			delete this;
			return 0;
		}
		return m_Ref;
	}
	bool GetEnable() const { return m_bEnable; }
	bool GetLife() const { return m_bLife; }
	size_t GetId() const { return m_id; }
	virtual std::string GetTag() const { return m_strTag; }
	virtual void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetTag(const std::string& tagName);
};
