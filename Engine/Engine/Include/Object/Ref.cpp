#include "Ref.h"
#include <cassert>
#include "../UnReleaseTest.h"

extern unordered_set<size_t> _undeleted_list;

Ref::Ref() : m_Ref(1), m_bEnable(true), m_bLife(true)
{
#ifdef _DEBUG
	_undeleted_list.insert(reinterpret_cast<size_t>(this));
#endif
}

Ref::~Ref()
{
#ifdef _DEBUG
	size_t thisObjAddress = reinterpret_cast<size_t>(this);
	if (_undeleted_list.find(thisObjAddress) != _undeleted_list.end())
	{
		_undeleted_list.erase(thisObjAddress);
	}
#endif
}


int Ref::Release()
{
	--m_Ref;
	if (m_Ref == 0)
	{
		delete this;
		return 0;
	}
	return m_Ref;
}


void Ref::SetTag(const string& tagName)
{
	m_strTag = tagName;
}