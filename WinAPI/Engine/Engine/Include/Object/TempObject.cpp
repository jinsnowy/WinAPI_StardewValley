#include "TempObject.h"

TempObject::~TempObject()
{
}

TempObject::TempObject(TempObject& obj)
	:Object(obj)
{
	m_fLifeDuration = obj.m_fLifeDuration;
}

bool TempObject::Init()
{
	return true;
}

void TempObject::Input(float dt)
{
	Object::Input(dt);
}

int TempObject::Update(float dt)
{
	Object::Update(dt);
	m_fLifeDuration -= dt;
	if (m_fLifeDuration <= 0.f)
	{
		Die();
	}
	return 0;
}

int TempObject::LateUpdate(float dt)
{
	Object::LateUpdate(dt);
	return 0;
}

void TempObject::Draw(HDC hdc, float dt)
{
	Object::Draw(hdc, dt);
}

TempObject* TempObject::Clone()
{
	return new TempObject(*this);
}
