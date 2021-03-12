#include "MovableObject.h"

MovableObject::MovableObject() 
	: m_Angle(0.f), m_Speed(100.f),
	m_bMove(false)
{
}

MovableObject::MovableObject(const MovableObject& obj) 
	: Object(obj),
	m_Angle(obj.m_Angle),
	m_Speed(obj.m_Speed)
{
}

MovableObject::~MovableObject()
{
}

bool MovableObject::Init()
{
	return true;
}

void MovableObject::Input(float dt)
{
	Object::Input(dt);
	m_bMove = false;
}

int MovableObject::Update(float dt)
{
	Object::Update(dt);
    return 0;
}

int MovableObject::LateUpdate(float dt)
{
	Object::LateUpdate(dt);
    return 0;
}

void MovableObject::Collision(float dt)
{
	Object::Collision(dt);
}

void MovableObject::Draw(HDC hDC, float dt)
{
	Object::Draw(hDC, dt);
}

void MovableObject::Save(FILE* pFile)
{
	Object::Save(pFile);
}

void MovableObject::Load(FILE* pFile)
{
	Object::Load(pFile);
}

void MovableObject::LateInit()
{
	Object::LateInit();
}

void MovableObject::Move(float x, float y)
{
	m_tPos.x += x;
	m_tPos.y += y;

	m_bMove = true;
}

void MovableObject::Move(float x, float y, float dt)
{
	m_tPos.x += x * dt;
	m_tPos.y += y * dt;

	m_bMove = true;
}

void MovableObject::Move(const Pos& tMove, float dt)
{
	m_tPos += tMove * dt;

	m_bMove = true;
}

void MovableObject::MoveXFromSpeed(float dt, MOVE_DIR dir)
{
	m_tPos.x += m_Speed * dt *  float(dir);

	m_bMove = true;
}

void MovableObject::MoveYFromSpeed(float dt, MOVE_DIR dir)
{
	m_tPos.y += m_Speed * dt * float(dir);

	m_bMove = true;
}

void MovableObject::MoveX(float x)
{
	m_tPos.x += x;

	m_bMove = true;
}

void MovableObject::MoveX(float x, float dt)
{
	m_tPos.x += x * dt;

	m_bMove = true;
}

void MovableObject::MoveY(float y)
{
	m_tPos.y += y;

	m_bMove = true;
}

void MovableObject::MoveY(float y, float dt)
{
	m_tPos.y += y * dt;

	m_bMove = true;
}

void MovableObject::MoveAngle()
{
	m_tPos.x += cosf(m_Angle) * m_Speed;
	m_tPos.y += sinf(m_Angle) * m_Speed;

	m_bMove = true;
}

void MovableObject::MoveAngle(float fTime)
{
	m_tPos.x += cosf(m_Angle) * m_Speed * fTime;
	m_tPos.y += sinf(m_Angle) * m_Speed * fTime;

	m_bMove = true;
}
