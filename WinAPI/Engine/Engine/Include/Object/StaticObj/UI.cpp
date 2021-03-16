#include <cassert>
#include "UI.h"
#include "../../Scene/Layer.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Scene.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../../Collider/Collider.h"
#include "../../Animation/Animation.h"

UI::UI()
{
}

UI::UI(const UI& ui)
    :
    Object(ui)
{
}

UI::~UI()
{

}
bool UI::Init()
{
    return true;
}

void UI::Input(float dt)
{
    Object::Input(dt);
}

int UI::Update(float dt)
{
    Object::Update(dt);
    return 0;
}

int UI::LateUpdate(float dt)
{
    Object::LateUpdate(dt);
    return 0;
}

void UI::Collision(float dt)
{
    Object::Collision(dt);
}

void UI::Draw(HDC hdc, float dt)
{
    if (m_pTexture)
    {
        Pos tPos = m_tPos - m_tSize * m_tPivot;
        Pos tImagePos = m_tImageOffset;
        if (m_pAnimation && m_bEnableAnimation)
        {
            AnimationClip* pClip = m_pAnimation->GetCurrentClip();

            switch (pClip->eType)
            {
            case AT_FRAME:
                SetTexture(pClip->vecTexture[pClip->iFrameX]);
                break;
            case AT_ATLAS:
                tImagePos.x = pClip->iFrameX * m_tSize.x + m_tImageOffset.x;
                tImagePos.y = pClip->iFrameY * m_tSize.y + m_tImageOffset.y;
                break;
            }
        }

        if (m_pTexture->GetColorKeyEnable())
        {
            TransparentBlt(hdc, int(tPos.x), int(tPos.y), int(m_tSize.x), int(m_tSize.y),
                m_pTexture->GetDC(), int(tImagePos.x), int(tImagePos.y),
                int(m_tSize.x), int(m_tSize.y),
                m_pTexture->GetColorKey());
        }
        else
        {
            BitBlt(hdc, int(tPos.x), int(tPos.y),
                int(m_tSize.x), int(m_tSize.y),
                m_pTexture->GetDC(), 0, 0, SRCCOPY);
        }
    }

    if (SHOWCHECK(SHOW_COLL))
    {
        list<Collider*>::iterator iter;
        list<Collider*>::iterator iterEnd = m_ColliderList.end();

        for (iter = m_ColliderList.begin(); iter != iterEnd;)
        {
            if (!(*iter)->GetEnable())
            {
                ++iter;
                continue;
            }

            (*iter)->Draw(hdc, dt);

            if (!(*iter)->GetLife())
            {
                SAFE_RELEASE((*iter));
                iter = m_ColliderList.erase(iter);
                iterEnd = m_ColliderList.end();
            }
            else ++iter;
        }
    }
}
