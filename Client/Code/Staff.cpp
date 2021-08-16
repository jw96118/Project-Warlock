#include "stdafx.h"
#include "Staff.h"

#include "WeaponMgr.h" 
#include "Player.h"
#include "Convert.h"
#include "SpellSphere.h"
#include "EffectMgr.h"
#include "CollisionMgr.h"

#include "Export_Function.h"

CStaff::CStaff(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
	, m_fFireKeyPressTime(0.f)
	, m_fChargeScale(0.f)
	, m_bChargeLevel2(false)
	, m_bChargeLevel3(false)
	, m_bFire(false)
	, m_bRelease(false)
	, m_bUseable(true)
{

}

CStaff::~CStaff(void)
{

}

void CStaff::Set_Usable(const _bool bUseable)
{
	m_bUseable = bUseable;
}

HRESULT CStaff::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = { 5.5f, 5.5f, 0.f };

	Change_Animation(Animation::STAFF_IDLE);

	m_vPos = { WINCX * 0.5f, WINCY - m_tFrame.fHeight * m_vScale.y * 0.5f + 150.f, 0.01f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPos.x, m_vPos.y);

	m_vChangingPos = m_vPivotPos = m_vPos;

	m_vChangingPos.y -= 150.f;

	m_fChargeTable[CHARGE_LEVEL1] = 1.f;
	m_fChargeTable[CHARGE_LEVEL2] = 1.7f;
	m_fChargeTable[CHARGE_LEVEL3] = 2.f;

	m_fChargeScale = m_fChargeTable[CHARGE_LEVEL1];

	m_eWeaponID = WEAPONID_01;
	m_eWeaponType = WEAPONTYPE_STAFF;

	m_fDamage = 10.f;

	return S_OK;
}

_int CStaff::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pShop->Get_Enable())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
		return NO_EVENT;
	}

	m_pPlayer->Set_MP(m_pWeaponMgr->Get_AmmoCurCnt(AMMO_SPELL), m_pWeaponMgr->Get_AmmoMaxCnt(AMMO_SPELL));

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

void CStaff::Render_GameObject(void)
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

HRESULT CStaff::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::STAFF_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Staff_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Staff_Idle", pComponent);

	pComponent = m_pTextureCom[Animation::STAFF_FIRE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Staff_Fire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Staff_Fire", pComponent);

	pComponent = m_pTextureCom[Animation::STAFF_CHARGE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Staff_Charge"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Staff_Charge", pComponent);

	pComponent = m_pTextureCom[Animation::STAFF_CHARGELOOP]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Staff_ChargeLoop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Texture_Staff_ChargeLoop", pComponent);

	pComponent = m_pTextureCom[Animation::STAFF_CHARGERELEASE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Staff_ChargeRelease"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Texture_Staff_ChargeRelease", pComponent);

	return S_OK;
}

void CStaff::Change_Animation(_uint uiAnimationID)
{
	USING(Animation)

		switch (uiAnimationID)
		{
		case STAFF_IDLE:
			m_tFrame.uiAnimationID = Animation::STAFF_IDLE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::STAFF_IDLE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 1.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::STAFF_IDLE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::STAFF_IDLE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = true;
			//m_vPos = m_vPivotPos;
			break;
		case STAFF_FIRE:
			m_tFrame.uiAnimationID = Animation::STAFF_FIRE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::STAFF_FIRE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 6.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::STAFF_FIRE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::STAFF_FIRE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		case STAFF_CHARGE:
			m_tFrame.uiAnimationID = Animation::STAFF_CHARGE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::STAFF_CHARGE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 1.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::STAFF_CHARGE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::STAFF_CHARGE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		case STAFF_CHARGELOOP:
			m_tFrame.uiAnimationID = Animation::STAFF_CHARGELOOP;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::STAFF_CHARGELOOP]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 1.7f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::STAFF_CHARGELOOP]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::STAFF_CHARGELOOP]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			m_pWeaponMgr->UseAmmo(AMMO_SPELL);
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		case STAFF_CHARGERELEASE:
			m_tFrame.uiAnimationID = Animation::STAFF_CHARGERELEASE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::STAFF_CHARGERELEASE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 6.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::STAFF_CHARGERELEASE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::STAFF_CHARGERELEASE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			m_bIsIdle = false;
			//m_vPos = m_vPivotPos + m_tFrame.vOffset;
			break;
		}

	D3DXMatrixScaling(&m_matView, m_tFrame.fWidth * m_vScale.x, m_tFrame.fHeight * m_vScale.y, 0.f);
}

void CStaff::Animator_GameObject(const _float& fTimeDelta)
{
	USING(Animation)

		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;


	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;

		switch (m_tFrame.uiAnimationID)
		{
		case STAFF_FIRE:
			m_fFireKeyPressTime = 0.f;
			Change_Animation(STAFF_IDLE);
			break;
		case STAFF_CHARGE:
		{
			Engine::Stop_Sound(Engine::CHANNELID::EFFECT);

			_bool bSound = false;
			Engine::Play_Sound(L"staff_charge_loopable_sound_loop.wav",
				Engine::CHANNELID::EFFECT,
				bSound);
			Change_Animation(STAFF_CHARGELOOP);
		}
		break;
		case STAFF_CHARGELOOP:
		{
			Engine::Stop_Sound(Engine::CHANNELID::EFFECT);

			_bool bSound = false;
			Engine::Play_Sound(L"staff_charge_loopable_sound_loop.wav",
				Engine::CHANNELID::EFFECT,
				bSound);
		}
		break;
		case STAFF_CHARGERELEASE:
			m_fFireKeyPressTime = 0.f;
			m_bChargeLevel2 = false;
			m_bChargeLevel3 = false;
			m_fChargeScale = m_fChargeTable[CHARGE_LEVEL1];
			Change_Animation(STAFF_IDLE);
			break;
		}
	}
}

void CStaff::Key_Input(const _float & fTimeDelta)
{
	CWeapon::Key_Input(fTimeDelta);

	if (Engine::Get_KeyMgr()->KeyUp(Engine::KEY_LBUTTON))
	{
		if ((m_tFrame.uiAnimationID == Animation::STAFF_IDLE ||
			m_tFrame.uiAnimationID == Animation::STAFF_CHARGE) &&
			m_bUseable)
		{
			Engine::Stop_Sound(Engine::CHANNELID::EFFECT);

			_bool bSound = false;
			Engine::Play_Sound(L"staff_basic_shot.wav",
				Engine::CHANNELID::EFFECT,
				bSound);

			m_pWeaponMgr->UseAmmo(AMMO_SPELL);
			Shot_SpellSphere();
			Change_Animation(Animation::STAFF_FIRE);

			if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_SPELL) == 0)
				m_bUseable = false;
		}

		else if (m_tFrame.uiAnimationID == Animation::STAFF_CHARGELOOP &&
			m_bUseable)
		{
			Engine::Stop_Sound(Engine::CHANNELID::EFFECT);

			_bool bSound = false;
			Engine::Play_Sound(L"staff_huge_shot.wav",
				Engine::CHANNELID::EFFECT,
				bSound);

			Shot_SpellSphere();
			Change_Animation(Animation::STAFF_CHARGERELEASE);

			if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_SPELL) == 0)
				m_bUseable = false;
		}
	}

	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::STAFF_CHARGELOOP)
		{
			m_fFireKeyPressTime += fTimeDelta;

			if (m_fFireKeyPressTime >= 2.f &&
				m_pWeaponMgr->Get_AmmoCurCnt(AMMO_SPELL) > 0 &&
				!m_bChargeLevel3)
			{
				m_bUseable = true;
				m_bChargeLevel3 = true;
				m_pWeaponMgr->UseAmmo(AMMO_SPELL);
				m_fChargeScale = m_fChargeTable[CHARGE_LEVEL3];
			}
			else if (m_fFireKeyPressTime >= 1.f &&
				m_pWeaponMgr->Get_AmmoCurCnt(AMMO_SPELL) > 0 &&
				!m_bChargeLevel2)
			{
				m_bUseable = true;
				m_bChargeLevel2 = true;
				m_pWeaponMgr->UseAmmo(AMMO_SPELL);
				m_fChargeScale = m_fChargeTable[CHARGE_LEVEL2];
			}

		}
	}

	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::STAFF_IDLE &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_SPELL) > 0)
		{
			_bool bSound = false;
			Engine::Play_Sound(L"staff_charging_full.wav",
				Engine::CHANNELID::EFFECT,
				bSound);

			//m_pWeaponMgr->UseAmmo(AMMO_SPELL);
			m_fChargeScale = m_fChargeTable[CHARGE_LEVEL1];
			Change_Animation(Animation::STAFF_CHARGE);
		}
	}
}

void CStaff::OnEnable_GameObject(void)
{
	Change_Animation(Animation::STAFF_IDLE);
	CWeapon::OnEnable_GameObject();
}

void CStaff::Shot_SpellSphere(void)
{
	CSpellSphere* pSpellSphere = dynamic_cast<CSpellSphere*>(m_pEffectMgr->PopEffect(EFFECT_SPELLSPHERE));

	_vec3 vRayPos;
	_vec3 vRayDir;

	CollisionMgr::Ray(m_pGraphicDev, vRayPos, vRayDir);
	pSpellSphere->OnEffect(vRayPos, vRayDir, m_fDamage);
}

CStaff* CStaff::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStaff*	pInstance = new CStaff(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStaff::Free(void)
{
	CWeapon::Free();
}