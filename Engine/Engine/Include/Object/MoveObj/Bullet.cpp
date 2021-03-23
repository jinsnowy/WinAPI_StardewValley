#include "Bullet.h"
#include "../../Object/Ref.h"
#include "../../Resources/Texture.h"
#include "../../Collider/ColliderRect.h"
#include "../../Collider/ColliderSphere.h"
#include "../../Core/Camera.h"
Bullet::Bullet()
    : m_Dist(0.f),
      m_LimitDist(500.f)

{
}

Bullet::Bullet(const Bullet& bullet)
    : MovableObject(bullet),
    m_LimitDist(bullet.m_LimitDist),
    m_Dist(bullet.m_Dist)
{

}

Bullet::~Bullet()
{
}

bool Bullet::Init()
{
    SetSpeed(500.0f);
    SetPivot(0.5f, 0.5f);
    SetSize(50.0f, 50.0f);
    SetTexture("Bullet", L"bullet.bmp");

    m_pTexture->SetColorKey(255, 0, 255);

    ColliderSphere* pSphere = AddCollider<ColliderSphere>("BulletBody");

    pSphere->SetSphere(Pos(0.f, 0.f), 25.f);
    SAFE_RELEASE(pSphere);
    return true;
}

void Bullet::Input(float dt)
{
    MovableObject::Input(dt);
}

int Bullet::Update(float dt)
{
    MovableObject::Update(dt);

    MoveAngle(dt);

    m_Dist += GetSpeed() * dt;

    if (m_Dist >= m_LimitDist)
        Die();

    return 0;
}

int Bullet::LateUpdate(float dt)
{
    MovableObject::LateUpdate(dt);
    return 0;
}

void Bullet::Collision(float dt)
{
    MovableObject::Update(dt);
}

void Bullet::Draw(HDC hDC, float dt)
{
    MovableObject::Draw(hDC, dt);
    // Ellipse(hDC, (int)m_tPos.x, (int)m_tPos.y, int(m_tPos.x + m_tSize.x), int(m_tPos.y + m_tSize.y));
}

Bullet* Bullet::Clone()
{
    return new Bullet(*this);
}

void Bullet::Hit(Collider* pSrc, Collider* pDst, float dt)
{
    if(pSrc->GetObj()->GetTag() == "PlayerBullet" && pDst->GetTag() == "MinionBody")
        Die();
    else if (pSrc->GetObj()->GetTag() == "MinionBullet" && pDst->GetTag() == "PlayerBody")
        Die();
}