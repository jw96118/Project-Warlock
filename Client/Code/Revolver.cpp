#include "stdafx.h"
#include "Revolver.h"

#include "WeaponMgr.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "Convert.h"
#include "Player.h"
#include "Shock.h"
#include "EffectMgr.h"
#include "Bullet.h"

#include "Export_Function.h"

CRevolver::CRevolver(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
	, m_bShot(false)
{

}

CRevolver::~CRevolver(void)
{
}

HRESULT CRevolver::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = { 5.5f, 5.5f, 0.f };

	Change_Animation(Animation::REVOLVER_IDLE);

	m_vPos = { WINCX * 0.5f, WINCY - m_tFrame.fHeight * m_vScale.y * 0.5f + 150.f, 0.01f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPos.x, m_vPos.y);

	m_vChangingPos = m_vPivotPos = m_vPos;

	m_vChangingPos.y -= 150.f;

	m_eWeaponID = WEAPONID_02;
	m_eWeaponType = WEAPONTYPE_DEFAULT;

	m_fDamage = 10.f;

	return S_OK;
}

_int CRevolver::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pShop->Get_Enable())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
		return NO_EVENT;
	}

	m_pPlayer->Set_Ammo(m_pWeaponMgr->Get_AmmoCurCnt(AMMO_REVOLVER), m_pWeaponMgr->Get_AmmoMaxCnt(AMMO_REVOLVER));


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

void CRevolver::Render_GameObject(void)
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

HRESULT CRevolver::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::REVOLVER_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Revolver_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Revolver_Idle", pComponent);

	pComponent = m_pTextureCom[Animation::REVOLVER_SHOT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Revolver_Shot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Revolver_Shot", pComponent);

	return S_OK;
}

void CRevolver::Change_Animation(_uint uiAnimationID)
{
	USING(Animation)

	switch (uiAnimationID)
	{
	case REVOLVER_IDLE:
		m_tFrame.uiAnimationID = Animation::REVOLVER_IDLE;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::REVOLVER_IDLE]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::REVOLVER_IDLE]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::REVOLVER_IDLE]->Get_ImageInfo().Height;
		m_tFrame.vOffset = { 0.f, 0.f, 0.f };
		m_bIsIdle = true;
		//m_vPos = m_vPivotPos;
		break;
	case REVOLVER_SHOT:
		m_tFrame.uiAnimationID = Animation::STAFF_FIRE;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::REVOLVER_SHOT]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 4.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::REVOLVER_SHOT]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::REVOLVER_SHOT]->Get_ImageInfo().Height;
		m_tFrame.vOffset = { 0.f, 0.f, 0.f };
		m_bIsIdle = false;
		//m_vPos = m_vPivotPos + m_tFrame.vOffset;
		break;
	}

	D3DXMatrixScaling(&m_matView, m_tFrame.fWidth * m_vScale.x, m_tFrame.fHeight * m_vScale.y, 0.f);
}

void CRevolver::Animator_GameObject(const _float& fTimeDelta)
{
	USING(Animation)

	m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;

		switch (m_tFrame.uiAnimationID)
		{
		case REVOLVER_SHOT:
			m_bShot = false;
			Change_Animation(REVOLVER_IDLE);
			break;
		}
	}

	switch (m_tFrame.uiAnimationID)
	{
	case REVOLVER_SHOT:
		if ((_uint)m_tFrame.fFrameCnt == 0 &&
			!m_bShot)
		{
			Shot_Bullet();
			m_bShot = true;
		}
		break;
	}
}

void CRevolver::Key_Input(const _float & fTimeDelta)
{
	CWeapon::Key_Input(fTimeDelta);

	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::REVOLVER_IDLE &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_REVOLVER) > 0)
		{
			Change_Animation(Animation::REVOLVER_SHOT);
		}
	}
}

void CRevolver::OnEnable_GameObject(void)
{
	Change_Animation(Animation::REVOLVER_IDLE);
	CWeapon::OnEnable_GameObject();
}

void CRevolver::Shot_Bullet(void)
{
	m_pWeaponMgr->UseAmmo(AMMO_REVOLVER);

	_bool bSound = false;
	Engine::Play_Sound(L"pistol_shot.wav",
		Engine::CHANNELID::EFFECT,
		bSound);

	CBullet* pBullet = dynamic_cast<CBullet*>(m_pEffectMgr->PopEffect(EFFECT_BULLET));

	_vec3 vRayPos;
	_vec3 vRayDir;

	CollisionMgr::Ray(m_pGraphicDev, vRayPos, vRayDir);
	pBullet->OnEffect(vRayPos, vRayDir, m_fDamage);
}


CRevolver* CRevolver::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRevolver*	pInstance = new CRevolver(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRevolver::Free(void)
{
	CWeapon::Free();
}