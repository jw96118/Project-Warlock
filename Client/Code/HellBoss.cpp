#include "stdafx.h"
#include "HellBoss.h"
#include "Convert.h"
#include "Export_Function.h"
#include <iostream>
#include "Spell.h"
#include "BossGauge.h"

CHellBoss::CHellBoss(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos)
	: CMonster(pGraphicDev, pLayerTag, wstrObjName)
{
	m_iPtCnt = 0;
	m_vPos = vPos;
	m_fSpeed = 2.0f;
	m_fDetectRange = 30.0f;
	m_fAttackRange = 25.0f;
	m_tData.fCurEXE = 10.f;
	m_tData.fMaxHP = 500.f;
	m_tData.fCurHP = m_tData.fMaxHP;
	m_tData.fDamage = 10.f;
	m_eCurPattern = DWARF_IDLE;
	m_ePrevPattern = m_eCurPattern;
	m_iMorphFlag = 0;
	m_fScale = 3.0f;
	m_iPatternNum = 0;
	m_iBigAttackNum = 5;


	m_tHitColl.fRadius = m_fScale*0.45;
	m_tHitColl.vCenter = m_vPos;
}


CHellBoss::~CHellBoss(void)
{
}

HRESULT CHellBoss::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vPos;
	Change_Animation(m_eCurPattern);
	return S_OK;
}

_int CHellBoss::Update_GameObject(const _float & fTimeDelta)
{
	//cout << m_tData.fCurHP << endl;
	if (m_pGuage == nullptr)
	{
		m_wstrProjectile = m_wstrObjName + L"HellBoss_Gauge_";
		CGameObject * pGo = m_pGuage = CBossGauge::Create(m_pGraphicDev, L"Texture_Boss_Gauge", m_pTransformCom, 1.0f, 1);
		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);
	}
	else
		m_pGuage->Set_HPRatio(m_tData.fCurHP / m_tData.fMaxHP);


	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	Update_Pattern(fTimeDelta);

	CMonster::Update_GameObject(fTimeDelta);//컴포넌트 연산및 빌보드연산은 여기서함
	m_pTransformCom->m_matWorld._42 = m_fScale*1.5f;

	Update_HitCollider();
	Update_BoxCollider();
	Animator_GameObject(fTimeDelta);

	return NO_EVENT;
}

void CHellBoss::Render_GameObject(void)
{
#ifdef _DEBUG
	Render_HitCollider();
	Render_BoxCollider();
#endif
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
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


}

HRESULT CHellBoss::LateReady_GameObject(void)
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransformCom, -1);
	return S_OK;
}

HRESULT CHellBoss::Add_Component(void)
{
	CMonster::Add_Component();
	Engine::CComponent*		pComponent = nullptr;

	//Dwarf
	pComponent = m_pTextureCom[DWARF_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Dwarf_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Dwarf_Idle", pComponent);

	pComponent = m_pTextureCom[DWARF_WALK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Dwarf_Walk"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Dwarf_Walk", pComponent);

	pComponent = m_pTextureCom[DWARF_SHOOT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Dwarf_Shoot"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Dwarf_Shoot", pComponent);

	pComponent = m_pTextureCom[DWARF_EYEBLAST]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Dwarf_EyeBlast"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Dwarf_EyeBlast", pComponent);

	pComponent = m_pTextureCom[DWARF_MORPH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Dwarf_Morph"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Dwarf_Morph", pComponent);

	//Big
	pComponent = m_pTextureCom[BIG_WALK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_Walk"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_Walk", pComponent);

	pComponent = m_pTextureCom[BIG_ATTACK_START]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_AttackStart"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_AttackStart", pComponent);

	pComponent = m_pTextureCom[BIG_ATTACK_SPIN]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_AttackSpin"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_AttackSpin", pComponent);

	pComponent = m_pTextureCom[BIG_ATTACK_FIRE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_AttackFire"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_AttackFire", pComponent);

	pComponent = m_pTextureCom[BIG_ATTACK_END]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_AttackEnd"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_AttackEnd", pComponent);

	//Big First
	pComponent = m_pTextureCom[BIG_FIRST_WALK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_FirstWalk"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_FirstWalk", pComponent);

	pComponent = m_pTextureCom[BIG_FIRST_HIT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_FirstHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_FirstHit", pComponent);

	pComponent = m_pTextureCom[BIG_FIRST_ATTACK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_FirstAttack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_FirstAttack", pComponent);

	pComponent = m_pTextureCom[BIG_MORPH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Big_Morph"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Big_Morph", pComponent);

	//Caco
	pComponent = m_pTextureCom[CACO_WALK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Caco_Float"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Caco_Float", pComponent);

	pComponent = m_pTextureCom[CACO_NOVA]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Caco_Nova"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Caco_Nova", pComponent);

	pComponent = m_pTextureCom[CACO_EYELASER]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Caco_EyeLaser"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Caco_EyeLaser", pComponent);

	pComponent = m_pTextureCom[CACO_ATTACK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Caco_Attack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Caco_Attack", pComponent);

	pComponent = m_pTextureCom[CACO_MORPH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Caco_Morph"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Caco_Morph", pComponent);

	//Final
	pComponent = m_pTextureCom[FINAL_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Final_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Final_Idle", pComponent);

	pComponent = m_pTextureCom[FINAL_DEATH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_HellBoss_Final_Death"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_HellBoss_Final_Death", pComponent);

	return S_OK;
}

void CHellBoss::Change_Animation(_uint uiAnimationID)
{
	m_tFrame.uiAnimationID = uiAnimationID;
	m_tFrame.fFrameCnt = 0.f;
	m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[uiAnimationID]->Get_TextureFrameSize();
	m_tFrame.fWidth = (_float)m_pTextureCom[uiAnimationID]->Get_ImageInfo().Width;
	m_tFrame.fHeight = (_float)m_pTextureCom[uiAnimationID]->Get_ImageInfo().Height;
	switch (uiAnimationID)
	{
	case CHellBoss::DWARF_MORPH:
		m_tFrame.fTimeScale = 0.3f;

		break;

	case CHellBoss::BIG_ATTACK_START:
		m_tFrame.fTimeScale = 2.f;
		break;
	case CHellBoss::BIG_ATTACK_SPIN:
		m_tFrame.fTimeScale = 6.f;
		break;
	case CHellBoss::BIG_ATTACK_FIRE:
		m_tFrame.fTimeScale = 9.f;
		break;
	case CHellBoss::BIG_ATTACK_END:
		m_tFrame.fTimeScale = 3.f;
		break;
	case CHellBoss::BIG_MORPH:
		m_tFrame.fTimeScale = 0.1f;
		break;
	case CHellBoss::CACO_MORPH:
		m_tFrame.fTimeScale = 0.3f;
		break;
	case CHellBoss::FINAL_DEATH:
		m_tFrame.fTimeScale = 0.5f;
		break;
	case CHellBoss::HB_END:
		break;
	default:
		m_tFrame.fTimeScale = 1.f;
		break;
	}


}

void CHellBoss::Animator_GameObject(const _float & fTimeDelta)
{
	switch (m_ePrevPattern)
	{
	case CHellBoss::DWARF_SHOOT:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::DWARF_EYEBLAST:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::DWARF_MORPH:
		if (!(m_iMorphFlag & 1))
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum = 0;
			m_iMorphFlag |= 1;
		}
		break;
	case CHellBoss::BIG_ATTACK_START:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::BIG_ATTACK_SPIN:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::BIG_ATTACK_FIRE:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::BIG_ATTACK_END:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum = 0;
		}
		break;

	case CHellBoss::BIG_FIRST_HIT:
		if (!(m_iMorphFlag & 2))
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum = 0;
			m_iMorphFlag |= 2;
		}
		break;
	case CHellBoss::BIG_FIRST_ATTACK:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::BIG_MORPH:
		if (!(m_iMorphFlag & 4))
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum = 0;
			m_iMorphFlag |= 4;
		}
		break;

	case CHellBoss::CACO_NOVA:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::CACO_EYELASER:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}
		break;
	case CHellBoss::CACO_ATTACK:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
		}

		break;
	case CHellBoss::CACO_MORPH:
		if (!(m_iMorphFlag & 8))
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum = 0;
			m_iMorphFlag |= 8;
		}

		break;
	case CHellBoss::FINAL_DEATH:
		if (!(m_iMorphFlag & 16))
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = m_tFrame.fMaxFrameCnt - 1;
			m_iPatternNum = 0;
			m_iMorphFlag |= 16;
		}
		break;

	case CHellBoss::HB_END:
		break;
	default:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
			m_tFrame.fFrameCnt = 0.f;
		break;
	}

}

void CHellBoss::Change_State(Animation::MONSTER State)
{
	m_eCurState = State;
	if (m_ePrevState != m_eCurState)
	{
		m_ePrevState = m_eCurState;
		Change_Animation(m_ePrevState);
	}

}

void CHellBoss::Change_Pattern(HB_Pattern ePattern)
{
	m_eCurPattern = ePattern;
	if (m_ePrevPattern != m_eCurPattern)
	{
		m_ePrevPattern = m_eCurPattern;
		Change_Animation(m_ePrevPattern);
	}
}

_int CHellBoss::Update_Pattern(_float fTimeDelta)
{
	m_pTargetTransformCom->Get_Info(Engine::INFO_POS, &m_vTargetPos);

	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vPos);
	m_vDir = m_vTargetPos - m_vPos;
	m_fTargetLength = D3DXVec3Length(&m_vDir);

	float fHpRatio = m_tData.fCurHP / m_tData.fMaxHP;
	if (m_eCurPattern != FINAL_DEATH && m_fDetectRange >= m_fTargetLength)
	{
		//if (fHpRatio >= 0.9f)
		//	Dwarf_Pattern(fTimeDelta);
		//else if (fHpRatio >= 0.7f)
		//{
		//	if (!(m_iMorphFlag & 1))
		//		Dwaf_Morph(fTimeDelta);
		//	else
		//		Big_Pattern(fTimeDelta);
		//}
		//else if (fHpRatio >= 0.5f)
		//{
		//	if (!(m_iMorphFlag & 2))
		//		Big_First_Hit(fTimeDelta);
		//	else
		//		Big_First_Pattern(fTimeDelta);
		//}
		//else if (fHpRatio >= 0.3f)
		//{
		//	if (!(m_iMorphFlag & 4))
		//		Big_Morph(fTimeDelta);
		//	else
		//		Caco_Pattern(fTimeDelta);
		//}
/*		else*/ if (fHpRatio >= 0.f)
		{
			if (!(m_iMorphFlag & 8))
				Caco_Morph(fTimeDelta);
			else
				Final_Pattern(fTimeDelta);
		}
		else if (fHpRatio < 0.f)
			Final_Death(fTimeDelta);
	}



	return NO_EVENT;

}

void CHellBoss::Update_HitCollider()
{
	float fHpRatio = m_tData.fCurHP / m_tData.fMaxHP;
	float fIntaval = 1.5f;
	_vec3 temp;


	D3DXVec3TransformNormal(&temp, &_vec3{ 1.f, 0, 0 }, &m_pTransformCom->m_matWorld);
	D3DXVec3Normalize(&temp, &temp);
	//if (fHpRatio >= 0.9f)
	//{
	//	m_tHitColl.vCenter = { m_vPos.x + temp.x*fIntaval,m_fScale,m_vPos.z + temp.z*fIntaval };
	//	m_tHitColl.fRadius = m_fScale*0.35f;

	//}
	//else if (fHpRatio >= 0.7f)
	//{
	//	m_tHitColl.vCenter = { m_vPos.x + temp.x*2.2f,m_fScale*1.8f,m_vPos.z - temp.z*fIntaval };
	//	m_tHitColl.fRadius = m_fScale*0.7f;
	//}
	//else if (fHpRatio >= 0.5f)
	//{
	//	m_tHitColl.vCenter = { m_vPos.x - temp.x*2.2f,m_fScale*1.8f,m_vPos.z - temp.z*fIntaval };
	//	m_tHitColl.fRadius = m_fScale*0.7f;
	//}
	//else if (fHpRatio >= 0.3f)
	//{
	//	m_tHitColl.vCenter = { m_vPos.x ,m_fScale*1.3f,m_vPos.z };
	//	m_tHitColl.fRadius = m_fScale*1.3f;
	//}
	//else
	
	{
		m_tHitColl.vCenter = { m_vPos.x + temp.x*0.35f,m_fScale*1.05f,m_vPos.z+ temp.z };
		m_tHitColl.fRadius = m_fScale*0.1f;

	}
}

void CHellBoss::Update_BoxCollider()
{
	m_tCollider.vMax = { m_vPos.x + m_fScale*0.35f,m_vPos.y + m_fScale*0.1f ,m_vPos.z + m_fScale*0.35f };
	m_tCollider.vMin = { m_vPos.x - m_fScale*0.35f,m_vPos.y - m_fScale*0.5f ,m_vPos.z - m_fScale*0.35f };

}

void CHellBoss::Dwarf_Pattern(_float fTimeDelta)
{
	if (m_fTargetLength > m_fDetectRange)
		Dwaf_Idle(fTimeDelta);

	if (m_fTargetLength <= m_fDetectRange&&m_fTargetLength >= m_fAttackRange)
	{
		Dwaf_Walk(fTimeDelta);
	}

	if (m_fTargetLength <= m_fAttackRange)
	{
		switch (m_iPatternNum)
		{
		case 0:
			Dwaf_Shoot(fTimeDelta);
			break;
		case 1:
			Dwaf_EyeBlast(fTimeDelta);
			break;
		default:
			m_iPatternNum = 0;
			break;
		}
	}
}

void CHellBoss::Dwaf_Idle(_float fTimeDelta)
{
	Change_Pattern(DWARF_IDLE);
}

void CHellBoss::Dwaf_Walk(_float fTimeDelta)
{
	Change_Pattern(DWARF_WALK);
	m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed * fTimeDelta));

}

void CHellBoss::Dwaf_Shoot(_float fTimeDelta)
{
	Change_Pattern(DWARF_SHOOT);

}

void CHellBoss::Dwaf_EyeBlast(_float fTimeDelta)
{
	Change_Pattern(DWARF_EYEBLAST);

}

void CHellBoss::Dwaf_Morph(_float fTimeDelta)
{
	Change_Pattern(DWARF_MORPH);

}

void CHellBoss::Big_Pattern(_float fTimeDelta)
{

	if (m_fTargetLength <= m_fAttackRange)
	{
		if (m_iPatternNum == 0)
			Big_Attack_Start(fTimeDelta);
		else if (m_iPatternNum <= 7)
			Big_Attack_Spin(fTimeDelta);
		else if (m_iPatternNum <= 16)
			Big_Attack_Fire(fTimeDelta);
		else if (m_iPatternNum <= 20)
			Big_Attack_End(fTimeDelta);
		//switch (m_iPatternNum)
		//{
		//case 0:
		//	Big_Attack_Start(fTimeDelta);
		//	break;
		//case 1:
		//	Big_Attack_Spin(fTimeDelta);
		//	break;
		//case 2:
		//case 3:
		//case 4:
		//case 5:
		//case 6:
		//	Big_Attack_Fire(fTimeDelta);
		//	break;
		//case 7:
		//	Big_Attack_End(fTimeDelta);
		//	break;
		//default:
		//	m_iPatternNum = 0;
		//	break;
		//}
	}
	else
	{
		Big_Walk(fTimeDelta);
	}
}

void CHellBoss::Big_Walk(_float fTimeDelta)
{
	Change_Pattern(BIG_WALK);
	m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed * fTimeDelta));
	m_iPatternNum = 0;

}

void CHellBoss::Big_Attack_Start(_float fTimeDelta)
{
	Change_Pattern(BIG_ATTACK_START);
}

void CHellBoss::Big_Attack_Spin(_float fTimeDelta)
{
	Change_Pattern(BIG_ATTACK_SPIN);
}

void CHellBoss::Big_Attack_Fire(_float fTimeDelta)
{
	Change_Pattern(BIG_ATTACK_FIRE);
}

void CHellBoss::Big_Attack_End(_float fTimeDelta)
{
	Change_Pattern(BIG_ATTACK_END);
}

void CHellBoss::Big_First_Pattern(_float fTimeDelta)
{
	if (m_fTargetLength <= m_fAttackRange)
	{
		Big_First_Attack(fTimeDelta);
	}
	else
	{
		Big_Walk(fTimeDelta);
	}
}

void CHellBoss::Big_First_Hit(_float fTimeDelta)
{
	Change_Pattern(BIG_FIRST_HIT);

}

void CHellBoss::Big_First_Walk(_float fTimeDelta)
{
	Change_Pattern(BIG_FIRST_WALK);
}

void CHellBoss::Big_First_Attack(_float fTimeDelta)
{
	Change_Pattern(BIG_FIRST_ATTACK);

}

void CHellBoss::Big_Morph(_float fTimeDelta)
{
	Change_Pattern(BIG_MORPH);
}

void CHellBoss::Caco_Pattern(_float fTimeDelta)
{

	if (m_fTargetLength <= m_fDetectRange&&m_fTargetLength >= m_fAttackRange)
	{
		Caco_Walk(fTimeDelta);
	}

	if (m_fTargetLength <= m_fAttackRange)
	{
		switch (m_iPatternNum)
		{
		case 0:
			Caco_Attack(fTimeDelta);
			break;
		case 1:
			Caco_Nova(fTimeDelta);
			break;
		case 2:
			Caco_EyeLaser(fTimeDelta);
			break;
		default:
			m_iPatternNum = 0;
			break;
		}
	}
}

void CHellBoss::Caco_Walk(_float fTimeDelta)
{
	Change_Pattern(CACO_WALK);
	m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed * fTimeDelta));
	m_iPatternNum = 0;


}

void CHellBoss::Caco_Nova(_float fTimeDelta)
{
	Change_Pattern(CACO_NOVA);

}

void CHellBoss::Caco_EyeLaser(_float fTimeDelta)
{
	Change_Pattern(CACO_EYELASER);

}

void CHellBoss::Caco_Attack(_float fTimeDelta)
{
	Change_Pattern(CACO_ATTACK);

}

void CHellBoss::Caco_Morph(_float fTimeDelta)
{
	Change_Pattern(CACO_MORPH);

}

void CHellBoss::Final_Pattern(_float fTimeDelta)
{
	Final_Idle(fTimeDelta);
}

void CHellBoss::Final_Idle(_float fTimeDelta)
{
	Change_Pattern(FINAL_IDLE);

	switch (m_iPatternNum)
	{
	case 0:
		Final_Spike1(fTimeDelta);
		break;
	case 1:
		Final_Spike2(fTimeDelta);
		break;
	case 2:
		Final_Tentacle(fTimeDelta);
		break;
	default:
		m_iPatternNum = 0;
		break;
	}

}

void CHellBoss::Final_Spike1(_float fTimeDelta)
{
}

void CHellBoss::Final_Spike2(_float fTimeDelta)
{
}

void CHellBoss::Final_Tentacle(_float fTimeDelta)
{
}

void CHellBoss::Final_Death(_float fTimeDelta)
{
	Change_Pattern(FINAL_DEATH);

}




CHellBoss * CHellBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos)
{
	CHellBoss*	pInstance = new CHellBoss(pGraphicDev, pLayerTag, wstrObjName, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CHellBoss::Free(void)
{
	CMonster::Free();
}
