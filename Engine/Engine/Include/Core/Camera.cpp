#include "Camera.h"
#include <cassert>
#include "../Object/Object.h"
#include "../Collider/CollisionManager.h"

DEFINITION_SINGLE(Camera)

Camera::Camera()
	:
	m_tPos(0.f,0.f),
	m_tPrev(-1.f, -1.f),
	m_tClientRS(0, 0),
	m_tWorldRS(0, 0),
	m_tPivot(0, 0),
	m_pTarget(nullptr)
{
}

Camera::~Camera()
{
	SAFE_RELEASE(m_pTarget);
}

bool Camera::Init(const Pos& tPos, const RESOLUTION& tRS, const RESOLUTION& tWorldRS)
{
	if (tWorldRS.x < tRS.x || tWorldRS.y < tRS.y)
		return false;

	m_tPos = tPos;
	m_tClientRS = tRS;
	m_tWorldRS = tWorldRS;

	COLLISION_MANAGER->Init();
	return true;
}

void Camera::Input(float dt)
{
	if (m_pTarget)
	{
		m_tPos = m_pTarget->GetPos();
		Pos offset = m_tPivot * m_tClientRS;
		Pos cameraTopLeft = m_tPos - offset;
		Pos cameraBottomRight = cameraTopLeft + m_tClientRS;
		if (cameraTopLeft.x < 0)
		{
			m_tPos.x = offset.x;
		}
		else if (cameraBottomRight.x >= m_tWorldRS.x)
		{
			m_tPos.x = m_tWorldRS.x - (m_tClientRS.x - offset.x);
		}
		if (cameraTopLeft.y < 0)
		{
			m_tPos.y = offset.y;
		}
		else if (cameraBottomRight.y >= m_tWorldRS.y)
		{
			m_tPos.y = m_tWorldRS.y - (m_tClientRS.y - offset.y);
		}

		if (m_tPrev != m_tPos)
		{
			COLLISION_MANAGER->QuadTreeUpdate();
		}
		m_tPrev = m_tPos;
	}
}

void Camera::Update(float dt)
{
}

void Camera::Scroll(float x, float y)
{
	assert(m_tWorldRS.x >= m_tClientRS.x && m_tWorldRS.y >= m_tClientRS.y);
	
	m_tPos.x += x;
	m_tPos.y += y;

	if (m_tPos.x < 0)
		m_tPos.x = 0.f;
	else if (m_tPos.x > m_tWorldRS.x - m_tClientRS.x)
		m_tPos.x = float(m_tWorldRS.x - m_tClientRS.x);

	if (m_tPos.y < 0)
		m_tPos.y = 0.f;
	else if (m_tPos.y > m_tWorldRS.y - m_tClientRS.y)
		m_tPos.y =float(m_tWorldRS.y - m_tClientRS.y);
}

Rect Camera::GetWorldRect() const
{
	Pos tPos = GetTopLeft();
	return { tPos.x, tPos.y, tPos.x + m_tClientRS.x , tPos.y + m_tClientRS.y };
}

void Camera::ReleaseTarget()
{
	SAFE_RELEASE(m_pTarget);
}

void Camera::SetTarget(Object* pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget;

	if (m_pTarget)
		m_pTarget->AddRef();
}
