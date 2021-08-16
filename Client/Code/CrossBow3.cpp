#include "stdafx.h"
#include "CrossBow3.h"

#include "WeaponMgr.h"
#include "Convert.h"
#include "Player.h"
#include "Arrow.h"
#include "EffectMgr.h"
#include "CollisionMgr.h"

#include "Export_Function.h"

CCrossBow3::CCrossBow3(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
	, m_bFrameAction(false)
{

}

CCrossBow3::~CCrossBow3(void)
{

}

HRESULT CCrossBow3::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = { 5.5f, 5.5f, 0.f };

	//m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW);
	//m_iArrowCnt = m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW);
	_uint uiArrowCnt = m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW);

	switch (uiArrowCnt % 3)
	{
	case 0:
		if (uiArrowCnt == 0)
			break;
		Change_Animation(Animation::CROSSBOW3_ARROWLEFT3);
		break;
	case 1:
		Change_Animation(Animation::CROSSBOW3_ARROWLEFT1);
		break;
	case 2:
		Change_Animation(Animation::CROSSBOW3_ARROWLEFT2);
		break;
	}

	m_vPos = { WINCX * 0.5f, WINCY - m_tFrame.fHeight * m_vScale.y * 0.5f + 100.f, 0.01f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPos.x, m_vPos.y);

	m_vChangingPos = m_vPivotPos = m_vPos;

	m_vChangingPos.y -= 150.f;

	m_eWeaponID = WEAPONID_02;
	m_eWeaponType = WEAPONTYPE_CROSSBOW;

	m_fDamage = 10.f;

	return S_OK;
}

_int CCrossBow3::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pShop->Get_Enable())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
		return NO_EVENT;
	}

	m_pPlayer->Set_Ammo(m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW), m_pWeaponMgr->Get_AmmoMaxCnt(AMMO_ARROW));

	if (m_bIsChanging)
	{
		Change_WeaponAction(fTimeDelta);

		memcpy(&m_matView.m[3][0], &m_vPos, sizeof(_vec3));

		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

		return NO_EVENT;
	}

	Key_Input(fTimeDelta);
	Mouse_Move(fTimeDelta);
	Animator_GameObject(fTimeDelta);

	memcpy(&m_matView.m[3][0], &m_vPos, sizeof(_vec3));

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return NO_EVENT;
}

void CCrossBow3::Render_GameObject(void)
{
	//if (!m_bFrameAction && m_iArrowCnt <= 0)
	//	return;

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);


	m_pTextureCom[m_tFrame.uiAnimationID]->Render_Texture((_uint)m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CCrossBow3::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT1]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_CrossBow3_1Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_CrossBow3_1Arrow", pComponent);

	pComponent = m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT2]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_CrossBow3_2Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_CrossBow3_2Arrow", pComponent);

	pComponent = m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT3]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_CrossBow3_3Arrow"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_CrossBow3_3Arrow", pComponent);

	pComponent = m_pTextureCom[Animation::CROSSBOW3_RELOAD]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_CrossBow3_Reload"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_CrossBow3_Reload", pComponent);

	return S_OK;
}

void CCrossBow3::Change_Animation(_uint uiAnimationID)
{
	USING(Animation)

		switch (uiAnimationID)
		{
		case CROSSBOW3_ARROWLEFT1:
			m_tFrame.uiAnimationID = Animation::CROSSBOW3_ARROWLEFT1;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT1]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 5.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT1]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT1]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = true;
			//m_vPos = m_vPivotPos;
			break;
		case CROSSBOW3_ARROWLEFT2:
			m_tFrame.uiAnimationID = Animation::CROSSBOW3_ARROWLEFT2;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT2]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 5.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT2]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT2]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = true;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		case CROSSBOW3_ARROWLEFT3:
			m_tFrame.uiAnimationID = Animation::CROSSBOW3_ARROWLEFT3;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT3]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 5.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT3]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::CROSSBOW3_ARROWLEFT3]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = true;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		case CROSSBOW3_RELOAD:
			m_tFrame.uiAnimationID = Animation::CROSSBOW3_RELOAD;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::CROSSBOW3_RELOAD]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 2.5f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::CROSSBOW3_RELOAD]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::CROSSBOW3_RELOAD]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		}

	D3DXMatrixScaling(&m_matView, m_tFrame.fWidth * m_vScale.x, m_tFrame.fHeight * m_vScale.y, 0.f);
}

void CCrossBow3::Animator_GameObject(const _float& fTimeDelta)
{
	USING(Animation)

		if (!m_bFrameAction)
			return;

	m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;

		switch (m_tFrame.uiAnimationID)
		{
		case CROSSBOW3_ARROWLEFT1:
		{

			m_bFrameAction = false;
			// TODO : 화살 없을 때 기본무기로 자동 교체
			if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) <= 0)
			{
				m_pWeaponMgr->Change_DefaultWeapon();
				break;
			}

			_bool bSound = false;
			Engine::Play_Sound(L"crossbow_reload.wav",
				Engine::CHANNELID::EFFECT,
				bSound);

			m_bFrameAction = true;
			Change_Animation(CROSSBOW3_RELOAD);
		}
		break;
		case CROSSBOW3_ARROWLEFT2:
			m_bFrameAction = false;
			Change_Animation(CROSSBOW3_ARROWLEFT1);
			break;
		case CROSSBOW3_ARROWLEFT3:
			m_bFrameAction = false;
			Change_Animation(CROSSBOW3_ARROWLEFT2);
			break;
		case CROSSBOW3_RELOAD:
			m_bFrameAction = false;
			if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) <= 3)
			{
				switch (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) % 3)
				{
				case 0:
					if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) == 0)
						break;
					Change_Animation(Animation::CROSSBOW3_ARROWLEFT3);
					break;
				case 1:
					Change_Animation(Animation::CROSSBOW3_ARROWLEFT1);
					break;
				case 2:
					Change_Animation(Animation::CROSSBOW3_ARROWLEFT2);
					break;
				}
			}
			else
			{
				Change_Animation(Animation::CROSSBOW3_ARROWLEFT3);
			}
			break;
		}

	}
}

void CCrossBow3::Key_Input(const _float & fTimeDelta)
{
	CWeapon::Key_Input(fTimeDelta);

	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID != Animation::CROSSBOW3_RELOAD &&
			!m_bFrameAction &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) > 0)
		{
			m_bIsIdle = false;

			m_bFrameAction = true;

			m_pWeaponMgr->UseAmmo(AMMO_ARROW);

			Shot_Arrow();
		}
	}
}

void CCrossBow3::Mouse_Move(const _float & fTimeDelta)
{
	if (m_bIsMovePlayer ||
		m_bFrameAction)
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
		m_vPos.x -= dwMouseMove / 10.f;

	_vec3 vMoveDir = m_vPivotPos - m_vPos;
	m_vPos += vMoveDir * fTimeDelta * 3.f;
}

void CCrossBow3::OnEnable_GameObject(void)
{
	_uint uiArrowCnt = m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW);

	switch (uiArrowCnt % 3)
	{
	case 0:
		if (uiArrowCnt == 0)
		{
			m_pWeaponMgr->Change_DefaultWeapon();
			break;
		}
		Change_Animation(Animation::CROSSBOW3_ARROWLEFT3);
		break;
	case 1:
		Change_Animation(Animation::CROSSBOW3_ARROWLEFT1);
		break;
	case 2:
		Change_Animation(Animation::CROSSBOW3_ARROWLEFT2);
		break;
	}
	CWeapon::OnEnable_GameObject();
}

void CCrossBow3::Change_WeaponAction(const _float & fTimeDelta)
{
	if (m_tFrame.uiAnimationID != Animation::CROSSBOW3_RELOAD)
	{
		_vec3 vMoveDir = m_vPivotPos - m_vPos;
		m_vPos += vMoveDir * fTimeDelta * 8.f;

		if (D3DXVec3Length(&(m_vPivotPos - m_vPos)) < 5.f)
		{
			m_vPos = m_vPivotPos;
			m_bIsChanging = false;
		}
	}
}

void CCrossBow3::Shot_Arrow(void)
{
	_bool bSound = false;
	Engine::Play_Sound(L"crossbow_shot.wav",
		Engine::CHANNELID::EFFECT,
		bSound);

	CArrow* pArrow = dynamic_cast<CArrow*>(m_pEffectMgr->PopEffect(EFFECT_ARROW));

	_vec3 vRayPos;
	_vec3 vRayDir;

	CollisionMgr::Ray(m_pGraphicDev, vRayPos, vRayDir);
	pArrow->OnEffect(vRayPos, vRayDir, m_fDamage);
}


CCrossBow3* CCrossBow3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossBow3*	pInstance = new CCrossBow3(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCrossBow3::Free(void)
{
	CWeapon::Free();
}