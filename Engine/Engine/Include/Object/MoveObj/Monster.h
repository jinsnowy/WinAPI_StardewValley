#pragma once
#include "MovableObject.h"
class Monster :
    public MovableObject
{
    friend class Object;
protected:
    Monster();
    Monster(const Monster& obj);
    virtual ~Monster();
private:
    enum class MonsterState
    {
        MOVE_LEFT = 0,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN
    };
    float m_HP = 0.f;
    static constexpr float m_fMonsterSpeed = 150.f;
    static constexpr float m_HPMax = 200.f;
    MonsterState m_eState = MonsterState::MOVE_DOWN;
public:
    inline Pos GetCenterPos() const
    {
        return Pos(GetPos().x + GetSize().x / 2, GetPos().y);
    }
    void Hit(Collider* pSrc, Collider* pDst, float dt);
    void CheckBlock(const Pos& prev);
    virtual void StateTransit(int iNext);
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual int Update(float dt);
    virtual int LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hDC, float dt);
    virtual Monster* Clone();
public:
    virtual void Save(FILE* pFile);
    virtual void Load(FILE* pFile);
    virtual void LateInit();
};

