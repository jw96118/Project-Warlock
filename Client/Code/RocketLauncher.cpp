#include "stdafx.h"
#include "RocketLauncher.h"

#include "WeaponMgr.h"
#include "Convert.h"
#include "Player.h"
#include "CollisionMgr.h"
#include "Monster.h"
#include "FirstPersonCamera.h"
#include "Explosion.h"
#include "EffectMgr.h"
#include "Shock.h"

#include "Export_Function.h"

CRocketLauncher::CRocketLauncher(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
	, m_bShot(false)
	, m_fLimitDist(0.f)
{

}

CRocketLauncher::~CRocketLauncher(void)
{

}

HRESULT CRocketLauncher::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = { 5.5f, 5.5f, 0.f };

	Change_Animation(Animation::ROCKETLAUNCHER_IDLE);

	m_vPos = { WINCX * 0.5f + 300.f, WINCY - m_tFrame.fHeight * m_vScale.y * 0.5f + 250.f, 0.01f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPos.x, m_vPos.y);

	m_vChangingPos = m_vPivotPos = m_vPos;

	m_vChangingPos.y -= 150.f;

	m_eWeaponID = WEAPONID_02;
	m_eWeaponType = WEAPONTYPE_GUN;

	m_tCollider.fRadius = 2.2f;

	m_fLimitDist = 4.f;

	m_fDamage = 10.f;

	return S_OK;
}

HRESULT CRocketLauncher::LateReady_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::LateReady_GameObject(), E_FAIL);
	m_pCamera = dynamic_cast<CFirstPersonCamera*>(Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
	FAILED_CHECK_RETURN(m_pCamera, E_FAIL);

	return S_OK;
}

_int CRocketLauncher::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pShop->Get_Enable())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
		return NO_EVENT;
	}

	m_pPlayer->Set_Ammo(m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ROCKETLAUNCHER), m_pWeaponMgr->Get_AmmoMaxCnt(AMMO_ROCKETLAUNCHER));

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

void CRocketLauncher::Render_GameObject(void)
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

HRESULT CRocketLauncher::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::ROCKETLAUNCHER_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_RocketLauncher_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_RocketLauncher_Idle", pComponent);

	pComponent = m_pTextureCom[Animation::ROCKETLAUNCHER_FIRE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_RocketLauncher_Fire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_RocketLauncher_Fire", pComponent);

	return S_OK;
}

void CRocketLauncher::Change_Animation(_uint uiAnimationID)
{
	USING(Animation)

		switch (uiAnimationID)
		{
		case ROCKETLAUNCHER_IDLE:
			m_tFrame.uiAnimationID = Animation::ROCKETLAUNCHER_IDLE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::ROCKETLAUNCHER_IDLE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 1.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::ROCKETLAUNCHER_IDLE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::ROCKETLAUNCHER_IDLE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = true;
			//m_vPos = m_vPivotPos;
			break;
		case ROCKETLAUNCHER_FIRE:
			m_tFrame.uiAnimationID = Animation::ROCKETLAUNCHER_FIRE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::ROCKETLAUNCHER_FIRE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 4.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::ROCKETLAUNCHER_FIRE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::ROCKETLAUNCHER_FIRE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		}

	D3DXMatrixScaling(&m_matView, m_tFrame.fWidth * m_vScale.x, m_tFrame.fHeight * m_vScale.y, 0.f);
}

void CRocketLauncher::Animator_GameObject(const _float& fTimeDelta)
{
	USING(Animation)

		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	switch (m_tFrame.uiAnimationID)
	{
	case ROCKETLAUNCHER_FIRE:
		if ((_uint)m_tFrame.fFrameCnt == 0 &&
			!m_bShot)
		{
			Shot_Bullet();
			m_bShot = true;
		}
		break;
	}

	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;

		switch (m_tFrame.uiAnimationID)
		{
		case ROCKETLAUNCHER_FIRE:
			m_bShot = false;
			Change_Animation(ROCKETLAUNCHER_IDLE);
			break;
		}
	}
}

void CRocketLauncher::Key_Input(const _float & fTimeDelta)
{
	CWeapon::Key_Input(fTimeDelta);

	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::ROCKETLAUNCHER_IDLE &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_ROCKETLAUNCHER) > 0)
		{
			Change_Animation(Animation::ROCKETLAUNCHER_FIRE);
		}
	}
}

void CRocketLauncher::OnEnable_GameObject(void)
{
	Change_Animation(Animation::ROCKETLAUNCHER_IDLE);
	CWeapon::OnEnable_GameObject();
}

void CRocketLauncher::Shot_Bullet(void)
{
	m_pWeaponMgr->UseAmmo(AMMO_ROCKETLAUNCHER);

	_bool bSound = false;
	Engine::Play_Sound(L"bomberman_sound.wav",
		Engine::CHANNELID::EFFECT,
		bSound);

	_vec3 vPos = m_pCamera->Get_Eye();
	vPos.y -= 1.f;

	const _vec3 vMoveDir = m_pCamera->Get_MoveDir();

	m_tCollider.vCenter = vPos + vMoveDir * m_fLimitDist;

	Engine::CLayer*	pLayer = Engine::Get_Layer(L"06.Monster");

	for (auto iter : pLayer->GetMapObj())
	{
		if (!iter.second->Get_Enable())
			continue;

		CMonster* pMonster = dynamic_cast<CMonster*>(iter.second);
		if (pMonster == nullptr ||
			pMonster->Dead_Mon())
			continue;

		const SPHERECOLLIDER& tHitCollider = pMonster->Get_HitCollider();

		_bool bIsCollision = CollisionMgr::IsCollisionSphereToSphere(m_tCollider, tHitCollider);

		if (bIsCollision)
		{
			pMonster->Damage();
		}
	}

	CExplosion* pExplosion = dynamic_cast<CExplosion*>(m_pEffectMgr->PopEffect(EFFECT_EXPLOSION));

	pExplosion->OnEffect(m_tCollider.vCenter);
}


CRocketLauncher* CRocketLauncher::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRocketLauncher*	pInstance = new CRocketLauncher(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRocketLauncher::Free(void)
{
	CWeapon::Free();
}