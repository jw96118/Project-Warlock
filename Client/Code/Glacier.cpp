#include "stdafx.h"
#include "Glacier.h"
#include "Convert.h"
#include "Export_Function.h"
#include <iostream>
#include "Projectile.h"

CGlacier::CGlacier(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos)
	: CMonster(pGraphicDev, pLayerTag, wstrObjName)
{
	m_iPtCnt = 0;
	m_vPos = vPos;
	m_fSpeed = 2.0f;
	m_fDetectRange = 18.0f;
	m_fAttackRange = 8.0f;
	m_eCurState = Animation::MON_IDLE;
	m_ePrevState = m_eCurState;
	m_tData.fCurEXE = 10.f;
	m_tData.fMaxHP = 300.f;
	m_tData.fCurHP = m_tData.fMaxHP;
	m_tData.fDamage = 10.f;
	m_fScale = 2.0f;

	m_tHitColl.fRadius = m_fScale*0.45;
	m_tHitColl.vCenter = m_vPos;
}


CGlacier::~CGlacier(void)
{
}

HRESULT CGlacier::Ready_GameObject(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vPos;
	Change_Animation(m_eCurState);
	return S_OK;
}

_int CGlacier::Update_GameObject(const _float & fTimeDelta)
{
	if (m_tData.fCurHP <= 0.f)
		Death(fTimeDelta);
	else
		Update_Pattern(fTimeDelta);

	if (GetAsyncKeyState('R') & 0x8001)
		m_tData.fCurHP -= 10;

	CMonster::Update_GameObject(fTimeDelta);//컴포넌트 연산및 빌보드연산은 여기서함
	Update_HitCollider();
	Update_BoxCollider();
	Animator_GameObject(fTimeDelta);

	return NO_EVENT;
}

void CGlacier::Render_GameObject(void)
{
#ifdef _DEBUG
	Render_HitCollider();
	Render_BoxCollider();
#endif

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Set Alpha Test
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Render UI
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom[m_tFrame.uiAnimationID]->Render_Texture((_uint)m_tFrame.fFrameCnt);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

}

HRESULT CGlacier::Add_Component(void)
{
	CMonster::Add_Component();
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[Animation::MON_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Glacier_Move"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Glacier_Idle", pComponent);

	pComponent = m_pTextureCom[Animation::MON_SHOOT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Glacier_Shoot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Glacier_Shoot", pComponent);

	pComponent = m_pTextureCom[Animation::MON_DEATH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Glacier_Death"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Glacier_Death", pComponent);

	pComponent = m_pTextureCom[Animation::MON_HIT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Glacier_HitMove"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Glacier_Hit", pComponent);

	pComponent = m_pTextureCom[Animation::MON_BACK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Glacier_Back"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Glacier_Back", pComponent);



	return S_OK;
}

void CGlacier::Change_Animation(_uint uiAnimationID)
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

	case Animation::MON_END:
		break;
	default:
		break;
	}
}

void CGlacier::Animator_GameObject(const _float & fTimeDelta)
{
	if (!m_bDeath)
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;



	using namespace Animation;
	switch (m_ePrevState)
	{
	case Animation::MON_SHOOT:
		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			Off_SoundFlag(4);

			m_tFrame.fFrameCnt = 0.f;
			m_bShoot = false;
		}
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

void CGlacier::Change_State(Animation::MONSTER State)
{
	m_eCurState = State;
	if (m_ePrevState != m_eCurState)
	{
		m_ePrevState = m_eCurState;
		Change_Animation(m_ePrevState);
	}

}

_int CGlacier::Update_Pattern(_float fTimeDelta)
{
	Engine::CTransform*		pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));

	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	//_vec3 vPlayerPos, vMonPos,vLength;
	pPlayerTransformCom->Get_Info(Engine::INFO_POS, &m_vTargetPos);
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vPos);
	m_vDir = m_vTargetPos - m_vPos;
	m_fTargetLength = D3DXVec3Length(&m_vDir);



	float fHpRatio = m_tData.fCurHP / m_tData.fMaxHP;

	if (m_eCurState != Animation::MON_DEATH&& m_fDetectRange >= m_fTargetLength)
	{
		On_SoundFlag(1, L"Glacier_Detect_01.wav");

		if (m_fTargetLength <= m_fDetectRange&&m_fTargetLength >= m_fAttackRange)
			m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed * fTimeDelta));

		if (fHpRatio >= 1.0f)
			Phase1(fTimeDelta);
		else if (fHpRatio >= 0.8f)
			Phase2(fTimeDelta);
		else if (fHpRatio >= 0.f)
			Phase3(fTimeDelta);
		else if (fHpRatio <= 0.f)
			Death(fTimeDelta);

	}
	else
		Off_SoundFlag(1);

	return NO_EVENT;

}

void CGlacier::Update_HitCollider()
{
	m_tHitColl.vCenter = { m_vPos.x,m_fScale*0.5f,m_vPos.z };

}

void CGlacier::Update_BoxCollider()
{
	m_tCollider.vMax = { m_vPos.x + m_fScale*0.38f,m_vPos.y + m_fScale*0.1f ,m_vPos.z + m_fScale*0.38f };
	m_tCollider.vMin = { m_vPos.x - m_fScale*0.38f,m_vPos.y - m_fScale*0.5f ,m_vPos.z - m_fScale*0.38f };
}

_int CGlacier::Phase1(_float fTimeDelta)
{
	Change_State(Animation::MON_IDLE);
	return NO_EVENT;
}

_int CGlacier::Phase2(_float fTimeDelta)
{
	Change_State(Animation::MON_HIT);
	if ((int)m_tFrame.fFrameCnt == 3)
	{
		On_SoundFlag(8, L"Glacier_Hit_01.wav");
	}
	return NO_EVENT;
}

_int CGlacier::Phase3(_float fTimeDelta)
{
	Change_State(Animation::MON_SHOOT);

	if ((int)m_tFrame.fFrameCnt == 8 && !m_bShoot)
	{
		m_bShoot = true;
		m_wstrProjectile = m_wstrObjName + L"Texture_Glacier_Bullet" + to_wstring(m_iPtCnt);
		CGameObject * pGo = CProjectile::Create(m_pGraphicDev, L"Texture_Glacier_Bullet", m_vPos, m_vDir, 3.0f, 0.25f);

		m_iPtCnt++;
		On_SoundFlag(4, L"Glacier_Attack_01.wav");
		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

	}
	return NO_EVENT;
}


_int CGlacier::Death(_float fTimeDelta)
{
	Change_State(Animation::MON_DEATH);
	On_SoundFlag(2, L"Glacier_Death_01.wav");

	return NO_EVENT;

}


CGlacier * CGlacier::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos, _int* pEventCount)
{
	CGlacier*	pInstance = new CGlacier(pGraphicDev, pLayerTag, wstrObjName, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	pInstance->m_pEventCount = pEventCount;
	return pInstance;

}
void CGlacier::Free(void)
{
	CMonster::Free();
}
