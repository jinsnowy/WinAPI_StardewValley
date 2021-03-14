#include "Ref.h"
#include <cassert>


Ref::Ref() : m_Ref(1), m_bEnable(true), m_bLife(true)
{
}

Ref::~Ref()
{
}


void Ref::SetTag(const string& tagName)
{
	m_strTag = tagName;
}