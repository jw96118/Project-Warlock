#include "stdafx.h"
#include "Dagger.h"

#include "Player.h"
#include "Convert.h"
#include "Monster.h"
#include "CollisionMgr.h"
#include "EffectMgr.h"
#include "DaggerThrow.h"
#include "Shock.h"

#include "Export_Function.h"

CDagger::CDagger(LPDIRECT3DDEVICE9 pGraphicDev)
	: CWeapon(pGraphicDev)
	, m_fThrowKeyPressTime(0.f)
	, m_bThrowKeyPress(false)
	, m_bUsableSkill(false)
	, m_bPlayThrowAnimation(false)
	, m_bStab(false)
	, m_bThrow(false)
	, m_bRecallSound(false)
{
	ZeroMemory(&m_tCollider, sizeof(m_tCollider));
}

CDagger::~CDagger(void)
{

}

HRESULT CDagger::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::Ready_GameObject(), E_FAIL);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vScale = { 5.5f, 5.5f, 0.f };

	Change_Animation(Animation::DAGGER_IDLE);

	m_vPos = { WINCX * 0.5f + 130.f, WINCY - m_tFrame.fHeight * m_vScale.y * 0.5f + 50.f, 0.01f };

	Engine::CConvert::ClientToD3DCoordinate2D(WINCX, WINCY, m_vPos.x, m_vPos.y);

	m_vChangingPos = m_vPivotPos = m_vPos;

	m_vChangingPos.y -= 150.f;

	m_eWeaponID = WEAPONID_01;
	m_eWeaponType = WEAPONTYPE_DEFAULT;

	m_tCollider.fRadius = 0.3f;

	m_fDamage = 10.f;

	return S_OK;
}

HRESULT CDagger::LateReady_GameObject(void)
{
	FAILED_CHECK_RETURN(CWeapon::LateReady_GameObject(), E_FAIL);

	m_pPlayerTransform = dynamic_cast<Engine::CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC));
	FAILED_CHECK_RETURN(m_pPlayerTransform, E_FAIL);

	m_pCamera = dynamic_cast<CFirstPersonCamera*>(Engine::Get_GameObject(L"01.Environment", L"MainCamera"));

	m_tCollider.fRadius = 0.5f;

	return S_OK;
}

_int CDagger::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pShop->Get_Enable())
	{
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);
		return NO_EVENT;
	}

	if (m_bIsChanging)
	{
		Change_WeaponAction(fTimeDelta);

		memcpy(&m_matView.m[3][0], &m_vPos, sizeof(_vec3));

		m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

		return NO_EVENT;
	}

	Key_Input(fTimeDelta);
	Mouse_Move(fTimeDelta);
	Update_Collider();
	Animator_GameObject(fTimeDelta);

	memcpy(&m_matView.m[3][0], &m_vPos, sizeof(_vec3));

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);

	return NO_EVENT;
}

void CDagger::Render_GameObject(void)
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

HRESULT CDagger::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::DAGGER_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Dagger_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Dagger_Idle", pComponent);

	pComponent = m_pTextureCom[Animation::DAGGER_STAB]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Dagger_Stab"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Dagger_Stab", pComponent);

	pComponent = m_pTextureCom[Animation::DAGGER_THROW]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Dagger_Throw"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Dagger_Throw", pComponent);

	return S_OK;
}

void CDagger::Change_Animation(_uint uiAnimationID)
{
	USING(Animation)

	switch (uiAnimationID)
	{
	case DAGGER_IDLE:
		m_tFrame.uiAnimationID = Animation::DAGGER_IDLE;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::DAGGER_IDLE]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::DAGGER_IDLE]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::DAGGER_IDLE]->Get_ImageInfo().Height;
		m_tFrame.vOffset = { 0.f, 0.f, 0.f };
		m_vPos = m_vPivotPos;
		m_bIsIdle = true;
		break;
	case DAGGER_STAB:
		m_tFrame.uiAnimationID = Animation::DAGGER_STAB;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::DAGGER_STAB]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 6.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::DAGGER_STAB]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::DAGGER_STAB]->Get_ImageInfo().Height;
		m_tFrame.vOffset = { -130.f, -140.f, 0.f };
		m_vPos = m_vPivotPos + m_tFrame.vOffset;
		m_bIsIdle = false;
		break;
	case DAGGER_THROW:
		m_tFrame.uiAnimationID = Animation::DAGGER_THROW;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::DAGGER_THROW]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.2f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::DAGGER_THROW]->Get_ImageInfo(1).Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::DAGGER_THROW]->Get_ImageInfo(1).Height;
		m_tFrame.vOffset = { 250.f, 0.f, 0.f };
		m_vPos = m_vPivotPos + m_tFrame.vOffset;
		m_bIsIdle = false;
		break;
	}

	D3DXMatrixScaling(&m_matView, m_tFrame.fWidth * m_vScale.x, m_tFrame.fHeight * m_vScale.y, 0.f);
}

void CDagger::Animator_GameObject(const _float& fTimeDelta)
{
	USING(Animation)

		switch (m_tFrame.uiAnimationID)
		{
		case DAGGER_STAB:
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;
			if ((_uint)m_tFrame.fFrameCnt == 1 &&
				!m_bStab)
			{
				_bool bSound = false;
				Engine::Play_Sound(L"dagger_metal_swing_1.wav",
					Engine::CHANNELID::EFFECT,
					bSound);

				m_bStab = true;
				Stab_Dagger();
			}
			break;
		case DAGGER_THROW:
			if (m_bPlayThrowAnimation)
			{
				m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;
				if ((_uint)m_tFrame.fFrameCnt == 3 &&
					!m_bThrow)
				{
					_bool bSound = false;
					Engine::Play_Sound(L"magic_dagger_throw_1.wav",
						Engine::CHANNELID::EFFECT,
						bSound);

					m_bThrow = true;
					Throw_Dagger();
				}
				if ((_uint)m_tFrame.fFrameCnt == 5 &&
					!m_bRecallSound)
				{
					_bool bSound = false;
					Engine::Play_Sound(L"magic_dagger_recall.wav",
						Engine::CHANNELID::EFFECT,
						bSound);

					m_bRecallSound = true;
				}
			}
			break;
		}


	if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
	{
		m_tFrame.fFrameCnt = 0.f;

		switch (m_tFrame.uiAnimationID)
		{
		case DAGGER_STAB:
			m_bStab = false;
			Change_Animation(DAGGER_IDLE);
			break;
		case DAGGER_THROW:
			m_fThrowKeyPressTime = 0.f;
			m_bThrowKeyPress = false;
			m_bUsableSkill = false;
			m_bPlayThrowAnimation = false;
			m_bThrow = false;
			m_bRecallSound = false;
			Change_Animation(DAGGER_IDLE);
			break;
		}

	}
}

void CDagger::Key_Input(const _float & fTimeDelta)
{
	CWeapon::Key_Input(fTimeDelta);


	if (Engine::Get_KeyMgr()->KeyUp(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID != Animation::DAGGER_STAB &&
			m_fThrowKeyPressTime <= 0.2f)
		{
			m_fThrowKeyPressTime = 0.f;
			m_bThrowKeyPress = false;
			Change_Animation(Animation::DAGGER_STAB);
		}

		else if (m_tFrame.uiAnimationID == Animation::DAGGER_THROW &&
			m_fThrowKeyPressTime > 0.2f &&
			!m_bUsableSkill)
		{
			m_bThrowKeyPress = false;
			m_fThrowKeyPressTime = 0.f;
			Change_Animation(Animation::DAGGER_IDLE);
		}

		else if (m_tFrame.uiAnimationID == Animation::DAGGER_THROW &&
			m_bUsableSkill &&
			!m_bPlayThrowAnimation)
		{
			m_tFrame.fFrameCnt = 1.f;
			m_bPlayThrowAnimation = true;
		}
	}

	if (Engine::Get_KeyMgr()->KeyPressing(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::DAGGER_THROW &&
			m_bThrowKeyPress &&
			!m_bUsableSkill)
		{
			m_fThrowKeyPressTime += fTimeDelta;

			if (m_fThrowKeyPressTime >= 0.6f)
				m_bUsableSkill = true;
		}
	}

	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_LBUTTON))
	{
		if (m_tFrame.uiAnimationID == Animation::DAGGER_IDLE &&
			!m_bThrowKeyPress)
		{
			m_bThrowKeyPress = true;
			Change_Animation(Animation::DAGGER_THROW);
		}
	}
}

void CDagger::OnEnable_GameObject(void)
{
	m_pPlayer->Set_Ammo(0.f, 0.f);
	Change_Animation(Animation::DAGGER_IDLE);
	CWeapon::OnEnable_GameObject();
}

void CDagger::Update_Collider(void)
{
	_vec3 vLookDir = m_pCamera->Get_LookDir();
	D3DXVec3Normalize(&vLookDir, &vLookDir);

	m_tCollider.vCenter = m_pCamera->Get_Eye() + vLookDir;
	m_tCollider.vCenter.y -= 0.3f;
}

void CDagger::Stab_Dagger(void)
{
	Engine::CLayer*	pLayer = Engine::Get_Layer(L"06.Monster");

	// 레이 충돌 시 거리가 가장 짧은 몬스터 정보를 가져오기 위한 변수
	CMonster* pHitObject = nullptr;
	_float fMinDist = 0.f;

	for (auto iter : pLayer->GetMapObj())
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(iter.second);
		NULL_CHECK(pMonster);

		if (pMonster->Dead_Mon())
			continue;

		_float fDist = 0.f;

		_bool bIsCollision = CollisionMgr::IsCollisionSphereToSphere(pMonster->Get_HitCollider(), m_tCollider);

		if (bIsCollision)
		{
			_vec3 vLength = pMonster->Get_HitCollider().vCenter - m_tCollider.vCenter;
			fDist = D3DXVec3Length(&vLength);

			if (pHitObject == nullptr)
			{
				pHitObject = pMonster;
				fMinDist = fDist;
			}
			else
			{
				if (fMinDist > fDist)
				{
					pHitObject = pMonster;
					fMinDist = fDist;
				}
			}
		}
	}

	if (pHitObject == nullptr)
		return;

	pHitObject->Get_Damage(20.f);
}

void CDagger::Throw_Dagger(void)
{
	CDaggerThrow* pDagger = dynamic_cast<CDaggerThrow*>(m_pEffectMgr->PopEffect(EFFECT_DAGGER));

	_vec3 vRayPos;
	_vec3 vRayDir;

	CollisionMgr::Ray(m_pGraphicDev, vRayPos, vRayDir);
	pDagger->OnEffect(vRayPos, vRayDir, m_fDamage);
}


CDagger* CDagger::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDagger*	pInstance = new CDagger(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDagger::Free(void)
{
	CWeapon::Free();
}