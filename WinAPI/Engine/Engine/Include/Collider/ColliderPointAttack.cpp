#include "ColliderPointAttack.h"

ColliderPointAttack::ColliderPointAttack()
    : m_fPower(0.f)
{
}

ColliderPointAttack::ColliderPointAttack(const ColliderPointAttack& coll)
    : ColliderPoint(coll)
{
    m_fPower = coll.m_fPower;
}

ColliderPointAttack::~ColliderPointAttack()
{
}

bool ColliderPointAttack::Init()
{
    return true;
}

void ColliderPointAttack::Input(float dt)
{
    ColliderPoint::Input(dt);
}

int ColliderPointAttack::Update(float dt)
{
    ColliderPoint::Update(dt);
    return 0;
}

int ColliderPointAttack::LateUpdate(float dt)
{
    ColliderPoint::LateUpdate(dt);
    return 0;
}

bool ColliderPointAttack::CheckCollision(Collider* pDst)
{
    return ColliderPoint::CheckCollision(pDst);
}

void ColliderPointAttack::Draw(HDC hdc, float dt)
{
    ColliderPoint::Draw(hdc, dt);
}

ColliderPointAttack* ColliderPointAttack::Clone()
{
    return new ColliderPointAttack(*this);
}

void ColliderPointAttack::Save(FILE* pFile)
{
}

void ColliderPointAttack::Load(FILE* pFile)
{
}
