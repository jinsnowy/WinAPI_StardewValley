#include "StaticObject.h"

StaticObject::StaticObject()
{
}

StaticObject::StaticObject(const StaticObject& obj)
    : Object(obj)
{
}

StaticObject::~StaticObject()
{
}

bool StaticObject::Init()
{
    return true;
}

void StaticObject::Input(float dt)
{
    Object::Input(dt);
}

int StaticObject::Update(float dt)
{
    Object::Update(dt);
    return 0;
}

int StaticObject::LateUpdate(float dt)
{
    Object::LateUpdate(dt);
    return 0;
}

void StaticObject::Collision(float dt)
{
    Object::Collision(dt);
}

void StaticObject::Draw(HDC hDC, float dt)
{
    Object::Draw(hDC, dt);
}

void StaticObject::Save(FILE* pFile)
{
    Object::Save(pFile);
}

void StaticObject::Load(FILE* pFile)
{
    Object::Load(pFile);
}

void StaticObject::LateInit()
{
    Object::LateInit();
}
