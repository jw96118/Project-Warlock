#include "stdafx.h"
#include "Bat.h"
#include "Convert.h"
#include "Export_Function.h"
#include <iostream>
#include "Projectile.h"

CBat::CBat(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos)
	: CMonster(pGraphicDev, pLayerTag, wstrObjName)
{
	m_iPtCnt = 0;
	m_vPos = vPos;
	m_fSpeed = 2.0f;
	m_fDetectRange = 10.0f;
	m_fAttackRange = 7.0f;
	m_eCurState = Animation::MON_IDLE;
	m_ePrevState = m_eCurState;
	m_tData.fCurEXE = 10.f;
	m_tData.fMaxHP = 100.f;
	m_tData.fCurHP = m_tData.fMaxHP;
	m_tData.fDamage = 10.f;
	m_fScale = 1.0f;
	m_tHitColl.fRadius = m_fScale*0.15f;

}


CBat::~CBat(void)
{
}

HRESULT CBat::Ready_GameObject(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vPos;
	Change_Animation(m_eCurState);
	return S_OK;

}

_int CBat::Update_GameObject(const _float & fTimeDelta)
{
	if (m_tData.fCurHP <= 0.f)
		Death(fTimeDelta);
	else
		Update_Pattern(fTimeDelta);

	CMonster::Update_GameObject(fTimeDelta);//컴포넌트 연산및 빌보드연산은 여기서함
	Animator_GameObject(fTimeDelta);

	Update_HitCollider();
	Update_BoxCollider();


	return NO_EVENT;
}

void CBat::Render_GameObject(void)
{
#ifdef _DEBUG
	Render_HitCollider();
	Render_BoxCollider();
#endif

	m_pTransformCom->m_matWorld._42 = 0.6f;//Y 높이 보정
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom[m_tFrame.uiAnimationID]->Render_Texture((_uint)m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}

HRESULT CBat::LateReady_GameObject(void)
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransformCom, -1);


	return S_OK;
}

HRESULT CBat::Add_Component(void)
{
	CMonster::Add_Component();
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::MON_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Bat_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Bat_Idle", pComponent);

	pComponent = m_pTextureCom[Animation::MON_BACK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Bat_Back"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Bat_Back", pComponent);

	pComponent = m_pTextureCom[Animation::MON_HIT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Bat_Hit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Bat_Hit", pComponent);

	pComponent = m_pTextureCom[Animation::MON_SHOOT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Bat_Shoot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Bat_Shoot", pComponent);


	pComponent = m_pTextureCom[Animation::MON_DEATH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Bat_Death"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Bat_Death", pComponent);

	return S_OK;
}

void CBat::Change_Animation(_uint uiAnimationID)
{
	using namespace Animation;
	switch (uiAnimationID)
	{
	case Animation::MON_IDLE:
		m_tFrame.uiAnimationID = m_ePrevState;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MON_IDLE]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::MON_IDLE]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::MON_IDLE]->Get_ImageInfo().Height;
		break;
	case Animation::MON_BACK:
		m_tFrame.uiAnimationID = m_ePrevState;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MON_BACK]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::MON_BACK]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::MON_BACK]->Get_ImageInfo().Height;
		break;
	case Animation::MON_SHOOT:
		m_tFrame.uiAnimationID = m_ePrevState;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MON_SHOOT]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::MON_SHOOT]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::MON_SHOOT]->Get_ImageInfo().Height;
		break;
	case Animation::MON_HIT:
		m_tFrame.uiAnimationID = m_ePrevState;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MON_HIT]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::MON_HIT]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::MON_HIT]->Get_ImageInfo().Height;
		break;
	case Animation::MON_DEATH:
		m_tFrame.uiAnimationID = m_ePrevState;
		m_tFrame.fFrameCnt = 0.f;
		m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[Animation::MON_DEATH]->Get_TextureFrameSize();
		m_tFrame.fTimeScale = 1.f;
		m_tFrame.fWidth = (_float)m_pTextureCom[Animation::MON_DEATH]->Get_ImageInfo().Width;
		m_tFrame.fHeight = (_float)m_pTextureCom[Animation::MON_DEATH]->Get_ImageInfo().Height;
		break;
	default:
		break;
	}
}

void CBat::Animator_GameObject(const _float & fTimeDelta)
{
	if (!m_bDeath)
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

	using namespace Animation;
	switch (m_ePrevState)
	{
	case Animation::MON_SHOOT:
		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bShoot = false;
			Off_SoundFlag(4);
		}
		break;
	case Animation::MON_HIT:
		break;
	case Animation::MON_DEATH:
		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = m_tFrame.fMaxFrameCnt - 1;
			m_bDeath = true;
			if (m_pEventCount != nullptr)
				(*m_pEventCount)--;
		}
		break;
	default:
		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
		}
		break;

	}

}

void CBat::Change_State(Animation::MONSTER State)
{
	m_eCurState = State;
	if (m_ePrevState != m_eCurState)
	{
		m_ePrevState = m_eCurState;
		Change_Animation(m_ePrevState);
	}

}

_int CBat::Update_Pattern(_float fTimeDelta)
{
	m_pTargetTransformCom->Get_Info(Engine::INFO_POS, &m_vTargetPos);

	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vPos);
	m_vDir = m_vTargetPos - m_vPos;
	m_fTargetLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	if (m_eCurState != Animation::MON_DEATH&& m_fDetectRange >= m_fTargetLength)
	{
		On_SoundFlag(1, L"Bat_detect_01.wav");

		if (m_fAttackRange >= m_fTargetLength)
		{
			Shoot(fTimeDelta);
		}
		else if (!m_bShoot)
		{
			Idle(fTimeDelta);
		}
	}
	else
	{
		Off_SoundFlag(1);
		Back(fTimeDelta);
	}


	return NO_EVENT;

}

void CBat::Update_HitCollider()
{
	m_tHitColl.vCenter = { m_vPos.x,m_vPos.y*1.25f,m_vPos.z };
}

void CBat::Update_BoxCollider()
{
	m_tCollider.vMax = { m_vPos.x + m_fScale*0.35f,m_vPos.y + m_fScale*0.1f ,m_vPos.z + m_fScale*0.35f };
	m_tCollider.vMin = { m_vPos.x - m_fScale*0.35f,m_vPos.y - m_fScale*0.5f ,m_vPos.z - m_fScale*0.35f };
}

_int CBat::Idle(_float fTimeDelta)
{
	Change_State(Animation::MON_IDLE);

	if (m_fTargetLength <= m_fDetectRange)
		m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed * fTimeDelta));

	return NO_EVENT;
}

_int CBat::Back(_float fTimeDelta)
{
	Change_State(Animation::MON_BACK);
	return NO_EVENT;

}

_int CBat::Hit(_float fTimeDelta, _float fDamage)
{
	Change_State(Animation::MON_HIT);
	m_tData.fCurHP -= fDamage;
	return NO_EVENT;
}

_int CBat::Shoot(_float fTimeDelta)
{
	Change_State(Animation::MON_SHOOT);

	if ((int)m_tFrame.fFrameCnt == 3 && !m_bShoot)
	{
		m_bShoot = true;
		m_wstrProjectile = m_wstrObjName + L"Bat_spit" + to_wstring(m_iPtCnt);
		CGameObject * pGo = CProjectile::Create(m_pGraphicDev, L"Texture_Bat_Spit", m_vPos, m_vDir, 3.0f, 0.35f);

		m_iPtCnt++;
		On_SoundFlag(4, L"Bat_attack_01.wav");

		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);
	}

	return NO_EVENT;

}

_int CBat::Death(_float fTimeDelta)
{
	Change_State(Animation::MON_DEATH);
	On_SoundFlag(2, L"Bat_death_01.wav");
	return NO_EVENT;

}


CBat * CBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos, _int* pEventCount)
{
	CBat*	pInstance = new CBat(pGraphicDev, pLayerTag, wstrObjName, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	pInstance->m_pEventCount = pEventCount;
	return pInstance;

}

void CBat::Free(void)
{
	CMonster::Free();
}
