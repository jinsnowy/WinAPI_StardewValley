#pragma once
#include "MovableObject.h"
class Bullet :
    public MovableObject
{
private:
    friend class Object;
    friend class Scene;
private:
    Bullet();
    Bullet(const Bullet& bullet);
    ~Bullet();
private:
    float   m_Dist;
    float   m_LimitDist;
public:
    void SetBulletDistance(float fDist)
    {
        m_LimitDist = fDist;
    }
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual int Update(float dt);
    virtual int LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hDC, float dt);
    Bullet* Clone();
public:
    void Hit(class Collider* pSrc, class Collider* pDst, float dt);
};

