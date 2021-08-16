#include "stdafx.h"
#include "Minigun.h"

#include "WeaponMgr.h"
#include "CollisionMgr.h"
#include "Monster.h"
#include "Convert.h"
#include "Player.h"
#include "Bullet.h"
#include "EffectMgr.h"
#include "Blood.h"

#include "Export_Function.h"

CMinigun::CMinigun(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
	, m_bFire(false)
	, m_bEmptyRotate(false)
	, m_bFireBullet(false)
	, m_bShot(false)
	, m_bRotateSound(false)
{

}

CMinigun::~CMinigun(void)
{

}

HRESULT CMinigun::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = { 5.5f, 5.5f, 0.f };

	Change_Animation(Animation::MINIGUN_ROTATE);
	Reset_BeltAnimation();

	m_vPos = { WINCX * 0.5f + 60.f, WINCY - m_tFrame.fHeight * m_vScale.y * 0.5f + 50.f, 0.01f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPos.x, m_vPos.y);

	m_vChangingPos = m_vPivotPos = m_vPos;

	m_vChangingPos.y -= 150.f;

	m_eWeaponID = WEAPONID_01;
	m_eWeaponType = WEAPONTYPE_GUN;

	//m_iMaxBulletCnt = 30;
	//m_iCurBulletCnt = m_iMaxBulletCnt;

	m_fDamage = 10.f;

	return S_OK;
}

_int CMinigun::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pShop->Get_Enable())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
		return NO_EVENT;
	}

	m_pPlayer->Set_Ammo(m_pWeaponMgr->Get_AmmoCurCnt(AMMO_MINIGUN), m_pWeaponMgr->Get_AmmoMaxCnt(AMMO_MINIGUN));

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
	IsPlayAmmoBelt(fTimeDelta);

	memcpy(&m_matView.m[3][0], &m_vPos, sizeof(_vec3));

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return NO_EVENT;
}

void CMinigun::Render_GameObject(void)
{
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);

	// Minigun
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	m_pTextureCom[m_tFrame.uiAnimationID]->Render_Texture((_uint)m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	// AmmoBullet

	m_pTextureCom[Animation::MINIGUN_BELT]->Render_Texture((_uint)m_tBeltFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT CMinigun::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::MINIGUN_BELT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Minigun_Belt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Minigun_Belt", pComponent);

	pComponent = m_pTextureCom[Animation::MINIGUN_FIRE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Minigun_Fire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Minigun_Fire", pComponent);

	pComponent = m_pTextureCom[Animation::MINIGUN_ROTATE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Minigun_Rotate"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Minigun_Rotate", pComponent);

	pComponent = m_pTextureCom[Animation::MINIGUN_TURRETMODE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Minigun_TurretMode"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Minigun_TurretMode", pComponent);

	pComponent = m_pTextureCom[Animation::MINIGUN_TURRETMODEFIRE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Minigun_TurretModeFire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Minigun_TurretModeFire", pComponent);

	pComponent = m_pTextureCom[Animation::MINIGUN_TURRETMODEIDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Minigun_TurretModeIdle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Minigun_TurretModeIdle", pComponent);


	return S_OK;
}

void CMinigun::Change_Animation(_uint uiAnimationID)
{
	USING(Animation)

		switch (uiAnimationID)
		{
		case MINIGUN_ROTATE:
			m_tFrame.uiAnimationID = Animation::MINIGUN_ROTATE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MINIGUN_ROTATE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 11.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::MINIGUN_ROTATE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::MINIGUN_ROTATE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			//m_vPos = m_vPivotPos;
			break;
		case MINIGUN_FIRE:
			m_tFrame.uiAnimationID = Animation::MINIGUN_FIRE;
			m_tFrame.fFrameCnt = 0.f;
			m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MINIGUN_FIRE]->Get_TextureFrameSize();
			m_tFrame.fTimeScale = 11.f;
			m_tFrame.fWidth = (_float)m_pTextureCom[Animation::MINIGUN_FIRE]->Get_ImageInfo().Width;
			m_tFrame.fHeight = (_float)m_pTextureCom[Animation::MINIGUN_FIRE]->Get_ImageInfo().Height;
			m_tFrame.vOffset = { 0.f, 0.f, 0.f };
			//m_vPos = m_vPivotPos;
			break;
		}

	D3DXMatrixScaling(&m_matView, m_tFrame.fWidth * m_vScale.x, m_tFrame.fHeight * m_vScale.y, 0.f);
}

void CMinigun::Animator_GameObject(const _float& fTimeDelta)
{
	USING(Animation)

		switch (m_tFrame.uiAnimationID)
		{
		case MINIGUN_ROTATE:
			if (m_bEmptyRotate)
			{
				if ((_uint)m_tFrame.fFrameCnt == 0 &&
					!m_bRotateSound)
				{
					_bool bSound = false;
					Engine::Play_Sound(L"minigun_rotate_end.wav",
						Engine::CHANNELID::EFFECT,
						bSound);
					m_bRotateSound = true;
				}
				m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;
			}
			break;
		case MINIGUN_FIRE:
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;
			if ((_int)m_tFrame.fFrameCnt == 0 &&
				!m_bFireBullet)
			{
				m_bFireBullet = true;
				m_pWeaponMgr->UseAmmo(AMMO_MINIGUN);
			}
			if ((_int)m_tFrame.fFrameCnt == 0 &&
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
		case MINIGUN_ROTATE:
			m_bRotateSound = false;
			break;
		case MINIGUN_FIRE:
			m_bFireBullet = false;
			m_bShot = false;
			if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_MINIGUN) <= 0)
				Change_Animation(Animation::MINIGUN_ROTATE);
			break;
		}

	}
}

void CMinigun::Key_Input(const _float & fTimeDelta)
{
	CWeapon::Key_Input(fTimeDelta);

	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::MINIGUN_ROTATE &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_MINIGUN) > 0)
		{
			m_bIsIdle = false;
			Change_Animation(Animation::MINIGUN_FIRE);
		}
		else if (m_tFrame.uiAnimationID == Animation::MINIGUN_ROTATE &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_MINIGUN) == 0)
		{
			m_bIsIdle = false;
			m_bEmptyRotate = true;
		}
	}

	if (Engine::Get_KeyMgr()->KeyUp(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::MINIGUN_ROTATE &&
			m_bEmptyRotate &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_MINIGUN) == 0)
		{
			m_bRotateSound = false;
			m_bIsIdle = true;
			m_tFrame.fFrameCnt = 0.f;
			m_bEmptyRotate = false;
			Reset_BeltAnimation();
		}
		else if (m_tFrame.uiAnimationID == Animation::MINIGUN_FIRE &&
			m_pWeaponMgr->Get_AmmoCurCnt(AMMO_MINIGUN) > 0)
		{
			m_bShot = false;
			m_bIsIdle = true;
			m_bFireBullet = false;
			Reset_BeltAnimation();
			Change_Animation(Animation::MINIGUN_ROTATE);
		}
	}
}

void CMinigun::Mouse_Move(const _float & fTimeDelta)
{
	if (m_bIsMovePlayer ||
		m_bEmptyRotate ||
		m_tFrame.uiAnimationID == Animation::MINIGUN_FIRE)
		return;

	_long	dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
		m_vPos.x -= dwMouseMove / 10.f;

	_vec3 vMoveDir = m_vPivotPos - m_vPos;
	m_vPos += vMoveDir * fTimeDelta * 3.f;
}

void CMinigun::OnEnable_GameObject(void)
{
	Change_Animation(Animation::MINIGUN_ROTATE);
	CWeapon::OnEnable_GameObject();
}

void CMinigun::Change_WeaponAction(const _float & fTimeDelta)
{
	if (m_tFrame.uiAnimationID == Animation::MINIGUN_ROTATE &&
		m_tFrame.fFrameCnt == 0)
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

void CMinigun::Reset_BeltAnimation(void)
{
	m_tBeltFrame.uiAnimationID = Animation::MINIGUN_BELT;
	m_tBeltFrame.fFrameCnt = 0.f;
	m_tBeltFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MINIGUN_BELT]->Get_TextureFrameSize();
	m_tBeltFrame.fTimeScale = 9.f;
	m_tBeltFrame.fWidth = (_float)m_pTextureCom[Animation::MINIGUN_BELT]->Get_ImageInfo().Width;
	m_tBeltFrame.fHeight = (_float)m_pTextureCom[Animation::MINIGUN_BELT]->Get_ImageInfo().Height;
	m_tBeltFrame.vOffset = { 0.f, 0.f, 0.f };
}

void CMinigun::IsPlayAmmoBelt(const _float & fTimeDelta)
{
	if (m_tFrame.uiAnimationID == Animation::MINIGUN_FIRE ||
		m_bEmptyRotate)
	{
		m_tBeltFrame.fFrameCnt += m_tBeltFrame.fMaxFrameCnt * fTimeDelta * m_tBeltFrame.fTimeScale;

		if (m_tBeltFrame.fMaxFrameCnt <= m_tBeltFrame.fFrameCnt)
		{
			Reset_BeltAnimation();
		}
	}
}

void CMinigun::Shot_Bullet(void)
{
	if (m_pWeaponMgr->Get_AmmoCurCnt(AMMO_MINIGUN) <= 0)
		return;

	m_pWeaponMgr->UseAmmo(AMMO_MINIGUN);

	//Engine::CLayer*	pLayer = Engine::Get_Layer(L"06.Monster");

	//// 레이 충돌 시 거리가 가장 짧은 몬스터 정보를 가져오기 위한 변수
	//CMonster* pHitObject = nullptr;
	//_float fMinDist = 0.f;
	//_vec3 vMinIntersection = { 0.f, 0.f, 0.f };

	_bool bSound = false;
	Engine::Play_Sound(L"minigun_upgrade_metallic_shot_1.wav",
		Engine::CHANNELID::EFFECT,
		bSound);

	//for (auto iter : pLayer->GetMapObj())
	//{
	//	CMonster* pMonster = dynamic_cast<CMonster*>(iter.second);
	//	NULL_CHECK(pMonster);

	//	if (pMonster->Dead_Mon())
	//		continue;

	//	_float fDist = 0.f;
	//	_vec3 vIntersection = { 0.f, 0.f, 0.f };

	//	_bool bIsCollision = CollisionMgr::IsCollisionRayToSphere(vIntersection,
	//		fDist,
	//		m_pGraphicDev,
	//		pMonster->Get_HitCollider());

	//	if (bIsCollision)
	//	{
	//		if (pHitObject == nullptr)
	//		{
	//			pHitObject = pMonster;
	//			fMinDist = fDist;
	//			vMinIntersection = vIntersection;
	//		}
	//		else
	//		{
	//			if (fMinDist > fDist)
	//			{
	//				pHitObject = pMonster;
	//				fMinDist = fDist;
	//				vMinIntersection = vIntersection;
	//			}
	//		}
	//	}
	//}

	//if (pHitObject == nullptr)
	//	return;

	//pHitObject->Damage();

	CBullet* pBullet = dynamic_cast<CBullet*>(m_pEffectMgr->PopEffect(EFFECT_BULLET));

	_vec3 vRayPos;
	_vec3 vRayDir;

	CollisionMgr::Ray(m_pGraphicDev, vRayPos, vRayDir);
	pBullet->OnEffect(vRayPos, vRayDir, m_fDamage);
}


CMinigun* CMinigun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMinigun*	pInstance = new CMinigun(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMinigun::Free(void)
{
	CWeapon::Free();
}