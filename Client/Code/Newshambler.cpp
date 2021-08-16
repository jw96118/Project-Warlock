#include "stdafx.h"
#include "Newshambler.h"
#include "Convert.h"
#include "Export_Function.h"
#include "Projectile.h"
#include "Spell.h"
#include "BossGauge.h"
CNewshambler::CNewshambler(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos)
	: CMonster(pGraphicDev, pLayerTag, wstrObjName)
{
	m_iPtCnt = 0;
	m_vPos = vPos;
	m_fSpeed = 2.0f;
	m_fDetectRange = 18.0f;
	m_fAttackRange = 10.0f;
	m_tData.fCurEXE = 10.f;
	m_tData.fMaxHP = 1000.f;
	m_tData.fCurHP = m_tData.fMaxHP;
	m_tData.fDamage = 10.f;
	m_eCurPattern = NS_IDLE;
	m_ePrevPattern = m_eCurPattern;
	m_iMorphFlag = 0;
	m_fScale = 4.0f;
	m_iPatternNum = 1;

	m_tHitColl.vCenter = m_vPos;
	m_tHitColl.fRadius = m_fScale*0.23f;

	m_tCollider.vMax = { 0.5f,1.f,0.5f };
	m_tCollider.vMin = { -0.5f,0.f,-0.5f };

}


CNewshambler::~CNewshambler(void)
{
}

HRESULT CNewshambler::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_vPos;
	Change_Animation(m_eCurPattern);


	return S_OK;
}

_int CNewshambler::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pGuage == nullptr)
	{
		m_wstrProjectile = m_wstrObjName + L"Newshambler_Gauge_";
		CGameObject * pGo = m_pGuage = CBossGauge::Create(m_pGraphicDev, L"Texture_Boss_Gauge", m_pTransformCom, 1.0f, 0);
		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);
	}
	else
		m_pGuage->Set_HPRatio(m_tData.fCurHP / m_tData.fMaxHP);

	Update_Pattern(fTimeDelta);

	CMonster::Update_GameObject(fTimeDelta);//컴포넌트 연산및 빌보드연산은 여기서함

	Update_HitCollider();
	Update_BoxCollider();
	Animator_GameObject(fTimeDelta);

	return NO_EVENT;
}

void CNewshambler::Render_GameObject(void)
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

HRESULT CNewshambler::LateReady_GameObject(void)
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>
		(Engine::Get_Component(L"02.GameLogic", L"Obj_Player", L"Com_Transform", Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTargetTransformCom, -1);

	return S_OK;
}

HRESULT CNewshambler::Add_Component(void)
{
	CMonster::Add_Component();
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom[NS_IDLE]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_Idle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_Idle", pComponent);


	pComponent = m_pTextureCom[NS_MAGIC]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_Magic"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_Magic", pComponent);

	pComponent = m_pTextureCom[NS_SLASH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_Slash"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_Slash", pComponent);

	pComponent = m_pTextureCom[NS_BASH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_Bash"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_Bash", pComponent);

	pComponent = m_pTextureCom[NS_DEATH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_Death"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_Death", pComponent);

	pComponent = m_pTextureCom[NS_FIRST_CAST]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_FirstCast"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_FirstCast", pComponent);

	pComponent = m_pTextureCom[NS_FIRST_HIT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_FirstHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_FirstHit", pComponent);

	pComponent = m_pTextureCom[NS_FIRST_SLASH]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_FirstSlash"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_FirstSlash", pComponent);

	pComponent = m_pTextureCom[NS_FIRST_WALK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_FirstWalk"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_FirstWalk", pComponent);



	pComponent = m_pTextureCom[NS_SECOND_CHOMP]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_SecondChomp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_SecondChomp", pComponent);

	pComponent = m_pTextureCom[NS_SECOND_HIT]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_SecondHit"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_SecondHit", pComponent);

	pComponent = m_pTextureCom[NS_SECONDWALK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_SecondWalk"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_SecondWalk", pComponent);

	pComponent = m_pTextureCom[NS_WALK]
		= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Texture_Newshambler_Walk"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture_Newshambler_Walk", pComponent);


	return S_OK;
}

void CNewshambler::Change_Animation(_uint uiAnimationID)
{
	if (uiAnimationID != NS_SECOND_CHOMP)
		m_tFrame.fTimeScale = 1.f;
	else
		m_tFrame.fTimeScale = 0.5f;

	m_tFrame.uiAnimationID = uiAnimationID;
	m_tFrame.fFrameCnt = 0.f;
	m_tFrame.fMaxFrameCnt = (_float)m_pTextureCom[uiAnimationID]->Get_TextureFrameSize();
	m_tFrame.fWidth = (_float)m_pTextureCom[uiAnimationID]->Get_ImageInfo().Width;
	m_tFrame.fHeight = (_float)m_pTextureCom[uiAnimationID]->Get_ImageInfo().Height;

}

void CNewshambler::Animator_GameObject(const _float & fTimeDelta)
{
	switch (m_ePrevPattern)
	{
	case CNewshambler::NS_BASH:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bAttack = false;
			m_iPatternNum += 1;
			Off_SoundFlag(4);

		}
		break;
	case CNewshambler::NS_MAGIC:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bShoot = false;
			m_iPatternNum += 1;
			Off_SoundFlag(8);

		}
		break;
	case CNewshambler::NS_SLASH:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_iPatternNum += 1;
			m_bAttack = false;
			Off_SoundFlag(16);

		}
		break;
	case CNewshambler::NS_FIRST_HIT:
		if (!(m_iMorphFlag & 1))
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = m_tFrame.fFrameCnt - 1;
			m_bShoot = false;
			m_bAttack = false;
			m_iMorphFlag |= 1;
			m_iPatternNum = 0;
		}
		break;
	case CNewshambler::NS_FIRST_CAST:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bShoot = false;
			m_iPatternNum += 1;
			Off_SoundFlag(4);

		}
		break;
	case CNewshambler::NS_FIRST_SLASH:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bAttack = true;
			Off_SoundFlag(8);
			m_iPatternNum += 1;

		}
		break;
	case CNewshambler::NS_SECOND_CHOMP:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bAttack = false;
			m_iPatternNum += 1;
			Off_SoundFlag(4);

		}
		break;
	case CNewshambler::NS_SECOND_HIT:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bShoot = false;
			m_bAttack = false;
			m_iMorphFlag |= 2;
		}
		break;
	case CNewshambler::NS_FIRST_WALK:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale*1.25f;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
			m_bShoot = false;
			m_bAttack = false;
			m_iMorphFlag |= 2;
		}

		break;
	case CNewshambler::NS_DEATH:
		if (!(m_iMorphFlag & 4))
			m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = m_tFrame.fMaxFrameCnt - 1;
			m_iMorphFlag |= 4;

		}
		break;
	default:
		m_tFrame.fFrameCnt += m_tFrame.fMaxFrameCnt * fTimeDelta * m_tFrame.fTimeScale;

		if (m_tFrame.fMaxFrameCnt <= m_tFrame.fFrameCnt)
		{
			m_tFrame.fFrameCnt = 0.f;
		}
		break;
	}


}

void CNewshambler::Change_State(Animation::MONSTER State)
{
	m_eCurState = State;
	if (m_ePrevState != m_eCurState)
	{
		m_ePrevState = m_eCurState;
		Change_Animation(m_ePrevState);
	}

}

void CNewshambler::Change_Pattern(NS_Pattern ePattern)
{
	m_eCurPattern = ePattern;
	if (m_ePrevPattern != m_eCurPattern)
	{
		m_ePrevPattern = m_eCurPattern;
		Change_Animation(m_ePrevPattern);
	}
}

_int CNewshambler::Update_Pattern(_float fTimeDelta)
{
	m_pTargetTransformCom->Get_Info(Engine::INFO_POS, &m_vTargetPos);
	m_pTransformCom->Get_Info(Engine::INFO_POS, &m_vPos);
	m_vDir = m_vTargetPos - m_vPos;
	m_fTargetLength = D3DXVec3Length(&m_vDir);
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	float fHpRatio = m_tData.fCurHP / m_tData.fMaxHP;
	if (m_eCurPattern != NS_DEATH && m_fDetectRange >= m_fTargetLength)
	{
		On_SoundFlag(1, L"shambler_detect.wav");

		if (fHpRatio >= 0.8f)
			Phase1(fTimeDelta);
		else if (fHpRatio >= 0.6f)
		{
			if (!(m_iMorphFlag & 1))
				First_Hit(fTimeDelta);
			else
				Phase2(fTimeDelta);
		}
		else if (fHpRatio >= 0.3f)
		{
			if (!(m_iMorphFlag & 2))
				Second_Hit(fTimeDelta);
			else
				Phase3(fTimeDelta);
		}
	}
	else
	{
		Off_SoundFlag(1);
	}
	if (fHpRatio <= 0.f && !(m_iMorphFlag & 4))
		Change_Pattern(NS_DEATH);


	return NO_EVENT;

}

void CNewshambler::Update_HitCollider()
{
	float fHpRatio = m_tData.fCurHP / m_tData.fMaxHP;
	float fIntaval = 1.f;
	_vec3 temp;
	D3DXVec3TransformNormal(&temp, &_vec3{ 1.f, 0, 0 }, &m_pTransformCom->m_matWorld);
	D3DXVec3Normalize(&temp, &temp);
	if (fHpRatio >= 0.8f)
	{
		m_tHitColl.vCenter = { m_vPos.x + temp.x*fIntaval,m_fScale*0.37f,m_vPos.z + temp.z*fIntaval };
	}
	else if (fHpRatio >= 0.6f)
	{
		m_tHitColl.vCenter = { m_vPos.x - temp.x*fIntaval,m_fScale*0.37f,m_vPos.z - temp.z*fIntaval };
	}
	else if (fHpRatio >= 0.3f)
	{
		m_tHitColl.vCenter = { m_vPos.x ,m_fScale*0.44f,m_vPos.z };
		m_tHitColl.fRadius = m_fScale*0.27f;
	}
}

void CNewshambler::Update_BoxCollider()
{
	m_tCollider.vMax = { m_vPos.x + m_fScale*0.35f,m_vPos.y + m_fScale*0.1f ,m_vPos.z + m_fScale*0.35f };
	m_tCollider.vMin = { m_vPos.x - m_fScale*0.35f,m_vPos.y - m_fScale*0.5f ,m_vPos.z - m_fScale*0.35f };

}

_int CNewshambler::Bash(_float fTimeDelta)
{
	Change_Pattern(NS_BASH);
	if ((int)m_tFrame.fFrameCnt == 8 && !m_bAttack)
	{
		m_bAttack = true;
		On_SoundFlag(4, L"shambler_attack_1.wav");

	}
	return NO_EVENT;
}

_int CNewshambler::Magic(_float fTimeDelta)
{
	Change_Pattern(NS_MAGIC);
	if ((int)m_tFrame.fFrameCnt == 5 && !m_bShoot)
	{
		_vec3 vProjPos = m_vPos + (m_vDir*m_fTargetLength*0.8f);
		m_wstrProjectile = m_wstrObjName + L"Ice_Spike1_" + to_wstring(m_iPtCnt);
		CGameObject * pGo = CSpell::Create(m_pGraphicDev, L"Texture_Ice_Spike1", vProjPos, 3);
		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);
		On_SoundFlag(8, L"shambler_attack_2.wav");

		int iRange = 5;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 2; j < 4; j++)
			{
				if (i & 1)
					vProjPos = { m_vPos.x + (rand() % iRange),m_vPos.y ,m_vPos.z + (rand() % iRange) };
				else
					vProjPos = { m_vPos.x - (rand() % iRange),m_vPos.y ,m_vPos.z - (rand() % iRange) };

				m_wstrProjectile = m_wstrObjName + L"Ice_Spike" + to_wstring(j) + L"_" + to_wstring(m_iPtCnt);
				pGo = CSpell::Create(m_pGraphicDev, L"Texture_Ice_Spike" + to_wstring(j), vProjPos, 3);
				Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

			}
			m_iPtCnt++;


		}
		//vProjPos = { m_vPos.x + 2.f,m_vPos.y ,m_vPos.z };
		//m_wstrProjectile = m_wstrObjName + L"Ice_Spike2_" + to_wstring(m_iPtCnt);
		//pGo = CSpell::Create(m_pGraphicDev, L"Texture_Ice_Spike2", vProjPos, 3);
		//Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

		//vProjPos = { m_vPos.x - 3.f,m_vPos.y ,m_vPos.z };
		//m_wstrProjectile = m_wstrObjName + L"Ice_Spike3_" + to_wstring(m_iPtCnt);
		//pGo = CSpell::Create(m_pGraphicDev, L"Texture_Ice_Spike3", vProjPos, 4.5f);
		//Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

		//vProjPos = { m_vPos.x - 4.f,m_vPos.y ,m_vPos.z - 2 };
		//m_wstrProjectile = m_wstrObjName + L"Ice_Spike3_1_" + to_wstring(m_iPtCnt);
		//pGo = CSpell::Create(m_pGraphicDev, L"Texture_Ice_Spike1", vProjPos, 6);
		//Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);


		m_bShoot = true;
	}
	return NO_EVENT;
}

_int CNewshambler::Slash(_float fTimeDelta)
{
	Change_Pattern(NS_SLASH);
	if ((int)m_tFrame.fFrameCnt == 5 && !m_bAttack)
	{
		On_SoundFlag(16, L"shambler_attack_3.wav");

		m_bAttack = true;
		m_wstrProjectile = m_wstrObjName + L"Eater_" + to_wstring(m_iPtCnt);
		CGameObject * pGo = CProjectile::Create(m_pGraphicDev, L"Texture_Eater", _vec3{ m_vPos.x,m_vPos.y,m_vPos.z }, m_vDir, 5.0f, 1.0f, 20.f, CProjectile::PROJECTILE_STRAIGHT);
		m_iPtCnt++;

		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

	}
	return NO_EVENT;
}

_int CNewshambler::Walk(_float fTimeDelta)
{
	Change_Pattern(NS_WALK);
	m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed * fTimeDelta));

	return NO_EVENT;
}

_int CNewshambler::First_Cast(_float fTimeDelta)
{
	Change_Pattern(NS_FIRST_CAST);

	if ((int)m_tFrame.fFrameCnt == 5 && !m_bShoot)
	{
		On_SoundFlag(4, L"shambler_attack_1.wav");

		m_bShoot = true;
		int iRange = 5;
		_vec3 vProjPos = m_vPos + (m_vDir*m_fTargetLength*0.8f);
		m_wstrProjectile = m_wstrObjName + L"Ice_Spike1_" + to_wstring(m_iPtCnt);
		CGameObject * pGo = CSpell::Create(m_pGraphicDev, L"Texture_Ice_Spike1", vProjPos, 3);
		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

		for (int i = 0; i < 4; i++)
		{
			for (int j = 2; j < 4; j++)
			{
				if (i & 1)
					vProjPos = { m_vPos.x + (rand() % iRange),m_vPos.y ,m_vPos.z + (rand() % iRange) };
				else
					vProjPos = { m_vPos.x - (rand() % iRange),m_vPos.y ,m_vPos.z - (rand() % iRange) };

				m_wstrProjectile = m_wstrObjName + L"Ice_Spike" + to_wstring(j) + L"_" + to_wstring(m_iPtCnt);
				pGo = CSpell::Create(m_pGraphicDev, L"Texture_Ice_Spike" + to_wstring(j), vProjPos, 3);
				Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

			}
			m_iPtCnt++;


		}

	}
	return NO_EVENT;
}

_int CNewshambler::First_Hit(_float fTimeDelta)
{
	Change_Pattern(NS_FIRST_HIT);
	On_SoundFlag(32, L"shambler_pain_1.wav");

	return NO_EVENT;
}

_int CNewshambler::First_Slash(_float fTimeDelta)
{
	Change_Pattern(NS_FIRST_SLASH);
	if ((int)m_tFrame.fFrameCnt == 5 && !m_bAttack)
	{
		m_bAttack = true;
		_vec3 temp;
		float fIntaval = 1.f;
		D3DXVec3TransformNormal(&temp, &_vec3{ 1.f, 0, 0 }, &m_pTransformCom->m_matWorld);
		D3DXVec3Normalize(&temp, &temp);
		On_SoundFlag(8, L"shambler_attack_2.wav");

		_vec3 vProjectilePos = { m_vPos.x + temp.x*fIntaval,m_fScale*0.35f,m_vPos.z + temp.z*fIntaval };
		vProjectilePos = { m_vPos.x + temp.x*-fIntaval,m_fScale*0.35f,m_vPos.z + temp.z*-fIntaval };
		m_wstrProjectile = m_wstrObjName + L"Eater2_" + to_wstring(m_iPtCnt);
		CGameObject* pGo = CProjectile::Create(m_pGraphicDev, L"Texture_Eater", _vec3{ vProjectilePos.x,vProjectilePos.y,vProjectilePos.z }, m_vDir, 5.0f, 1.0f, 20.f, CProjectile::PROJECTILE_SINE, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), 1.5f);
		m_iPtCnt++;
		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);

		vProjectilePos = { m_vPos.x + temp.x*fIntaval,m_fScale*0.35f,m_vPos.z + temp.z*fIntaval };
		m_wstrProjectile = m_wstrObjName + L"Eater3_" + to_wstring(m_iPtCnt);
		pGo = CProjectile::Create(m_pGraphicDev, L"Texture_Eater", _vec3{ vProjectilePos.x,vProjectilePos.y,vProjectilePos.z }, m_vDir, 5.0f, 1.0f, 20.f, CProjectile::PROJECTILE_SINE, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), -1.5f);

		m_iPtCnt++;
		Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);


	}
	return NO_EVENT;
}

_int CNewshambler::First_Walk(_float fTimeDelta)
{
	m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed*1.25f * fTimeDelta));
	Change_Pattern(NS_FIRST_WALK);
	return NO_EVENT;
}

_int CNewshambler::Second_Chomp(_float fTimeDelta)
{
	Change_Pattern(NS_SECOND_CHOMP);

	if ((int)m_tFrame.fFrameCnt == 5 && !m_bAttack)
	{
		On_SoundFlag(4, L"shambler_attack_2.wav");

		m_bAttack = true;
		for (int i = -2; i < 3; i++)
		{
			m_wstrProjectile = m_wstrObjName + L"Eater_" + to_wstring(m_iPtCnt);
			CGameObject * pGo = CProjectile::Create(m_pGraphicDev, L"Texture_Eater", m_vPos, m_vDir, 5.0f, 1.0f, 20.f, CProjectile::PROJECTILE_SINE, D3DXCOLOR(1.f, 0.f, 0.f, 1.f), (float)i*0.5f);
			m_iPtCnt++;
			Engine::Get_Layer(m_pLayerTag)->Add_GameObject(m_wstrProjectile.c_str(), pGo);
		}
	}
	return NO_EVENT;
}

_int CNewshambler::Second_Hit(_float fTimeDelta)
{
	Change_Pattern(NS_SECOND_HIT);
	On_SoundFlag(64, L"shambler_pain_2.wav");

	return NO_EVENT;
}

_int CNewshambler::Second_Walk(_float fTimeDelta)
{
	m_pTransformCom->Chase_Target(&m_vTargetPos, (m_fSpeed*1.5f * fTimeDelta));

	Change_Pattern(NS_SECONDWALK);

	return NO_EVENT;
}



_int CNewshambler::Death(_float fTimeDelta)
{
	Change_Pattern(NS_DEATH);
	On_SoundFlag(2, L"shambler_death_bloody.wav");

	return NO_EVENT;

}

_int CNewshambler::Phase1(_float fTimeDelta)
{
	if (m_fTargetLength <= m_fDetectRange&&m_fTargetLength >= m_fAttackRange)
	{
		Walk(fTimeDelta);
	}
	if (m_fTargetLength <= m_fAttackRange)
	{
		switch (m_iPatternNum)
		{
		case 0:
			Bash(fTimeDelta);
			break;
		case 1:
			Magic(fTimeDelta);
			break;
		case 2:
			Slash(fTimeDelta);
			break;
		default:
			m_iPatternNum = 0;
			break;
		}
	}

	return NO_EVENT;
}

_int CNewshambler::Phase2(_float fTimeDelta)
{
	if (m_fTargetLength <= m_fDetectRange&&m_fTargetLength >= m_fAttackRange)
	{
		First_Walk(fTimeDelta);

	}
	if (m_fTargetLength <= m_fAttackRange)
	{
		switch (m_iPatternNum)
		{
		case 0:
			First_Slash(fTimeDelta);
			break;
		case 1:
			First_Cast(fTimeDelta);
			break;
		default:
			m_iPatternNum = 0;
			m_bShoot = false;
			m_bAttack = false;
			break;
		}
	}

	return NO_EVENT;
}

_int CNewshambler::Phase3(_float fTimeDelta)
{
	if (m_fTargetLength <= m_fDetectRange&&m_fTargetLength >= m_fAttackRange)
	{
		Second_Walk(fTimeDelta);
	}
	if (m_fTargetLength <= m_fAttackRange)
	{
		Second_Chomp(fTimeDelta);
	}
	return NO_EVENT;
}


CNewshambler * CNewshambler::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pLayerTag, wstring wstrObjName, _vec3 vPos)
{
	CNewshambler*	pInstance = new CNewshambler(pGraphicDev, pLayerTag, wstrObjName, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CNewshambler::Free(void)
{
	CMonster::Free();
}
