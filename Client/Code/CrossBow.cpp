#include "stdafx.h"
#include "CrossBow.h"

#include "WeaponMgr.h"
#include "Convert.h"
#include "Player.h"
#include "Arrow.h"
#include "CollisionMgr.h"
#include "EffectMgr.h"
#include "Export_Function.h"

CCrossBow::CCrossBow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
	, m_bShot(false)
{

}

CCrossBow::~CCrossBow(void)
{

}

HRESULT CCrossBow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = { 5.5f, 5.5f, 0.f };

	Change_Animation(Animation::CROSSBOW_IDLE);

	m_vPos = { WINCX * 0.5f, WINCY - m_tFrame.fHeight * m_vScale.y * 0.5f + 100.f, 0.01f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPos.x, m_vPos.y);

	m_vChangingPos = m_vPivotPos = m_vPos;

	m_vChangingPos.y -= 150.f;

	m_eWeaponID = WEAPONID_01;
	m_eWeaponType = WEAPONTYPE_CROSSBOW;

	m_fDamage = 10.f;

	return S_OK;
}

_int CCrossBow::Update_GameObject(const _float& fTimeDelta)
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

void CCrossBow::Render_GameObject(void)
{
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

HRESULT CCrossBow::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::CROSSBOW_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_CrossBow_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_CrossBow_Idle", pComponent);

	pComponent = m_pTextureCom[Animation::CROSSBOW_FIRE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_CrossBow_Fire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_CrossBow_Fire", pComponent);

	pComponent = m_pTextureCom[Animation::CROSSBOW_RELOAD]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_CrossBow_Reload"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_CrossBow_Reload", pComponent);

	return S_OK;
}

void CCrossBow::Change_Animation(_uint uiAnimationID)
{
	USING(Animation)

		switch (uiAnimationID)
		{
		case CROSSBOW_IDLE:
			m_tFrame.uiAnimationID = Animation::CROSSBOW_IDLE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::CROSSBOW_IDLE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 1.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::CROSSBOW_IDLE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::CROSSBOW_IDLE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = true;
			//m_vPos = m_vPivotPos;
			break;
		case CROSSBOW_FIRE:
			m_tFrame.uiAnimationID = Animation::CROSSBOW_FIRE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::CROSSBOW_FIRE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 4.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::CROSSBOW_FIRE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::CROSSBOW_FIRE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		case CROSSBOW_RELOAD:
			m_tFrame.uiAnimationID = Animation::CROSSBOW_RELOAD;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::CROSSBOW_RELOAD]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 2.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::CROSSBOW_RELOAD]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::CROSSBOW_RELOAD]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		}

	D3DXMatrixScaling(&m_matView, m_tFrame.fWidth * m_vScale.x, m_tFrame.fHeight * m_vScale.y, 0.f);
}

void CCrossBow::Animator_GameObject(const _float& fTimeDelta)
{
	USING(Animation)

		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	switch (m_tFrame.uiAnimationID)
	{
	case CROSSBOW_FIRE:
		if ((_uint)m_tFrame.fFrameCnt == 2 &&
			!m_bShot)
		{
			Shot_Arrow();
			m_bShot = true;
		}
		break;
	}


	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;

		switch (m_tFrame.uiAnimationID)
		{
		case CROSSBOW_FIRE:
		{
			m_bShot = false;
			if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) <= 0)
			{
				m_pWeaponMgr->Change_DefaultWeapon();
				break;
			}

			_bool bSound = false;
			Engine::Play_Sound(L"crossbow_reload.wav",
				Engine::CHANNELID::EFFECT,
				bSound);

			Change_Animation(CROSSBOW_RELOAD);
		}
		break;
		case CROSSBOW_RELOAD:
			Change_Animation(CROSSBOW_IDLE);
			break;
		}

	}
}

void CCrossBow::Key_Input(const _float & fTimeDelta)
{
	CWeapon::Key_Input(fTimeDelta);

	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::CROSSBOW_IDLE &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) > 0)
		{
			Change_Animation(Animation::CROSSBOW_FIRE);
		}
	}
}

void CCrossBow::OnEnable_GameObject(void)
{
	if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ARROW) <= 0)
	{
		m_pWeaponMgr->Change_DefaultWeapon();
		return;
	}

	Change_Animation(Animation::CROSSBOW_IDLE);
	CWeapon::OnEnable_GameObject();
}

void CCrossBow::Shot_Arrow(void)
{
	m_pWeaponMgr->UseAmmo(AMMO_ARROW);

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


CCrossBow* CCrossBow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrossBow*	pInstance = new CCrossBow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCrossBow::Free(void)
{
	CWeapon::Free();
}