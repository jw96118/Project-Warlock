#include "stdafx.h"

#include "EffectMgr.h"
#include "Effect.h"
#include "DaggerThrow.h"
#include "Arrow.h"
#include "SpellSphere.h"
#include "Explosion.h"
#include "Shock.h"
#include "Bullet.h"
#include "Blood.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CEffectMgr)

CEffectMgr::CEffectMgr()
	: m_pGraphicDev(nullptr)
{
}


CEffectMgr::~CEffectMgr()
{
	Free();
}

HRESULT CEffectMgr::Ready_EffectMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	m_wstrEffectName = L"Effect_";
	m_uiNameCnt = 0;

	return S_OK;
}

void CEffectMgr::Reset_Effect(void)
{
	for (_uint i = 0; i < EFFECT_END; ++i)
	{
		_uint iCnt = m_EffectPoolList[i].size();

		for (_uint j = 0; j < iCnt; ++j)
		{
			m_EffectPoolList[i].pop_back();
		}

		m_EffectPoolList[i].clear();
	}
}

CEffect * CEffectMgr::PopEffect(EFFECT eEffect)
{
	CEffect* pEffect = nullptr;

	for (auto effect : m_EffectPoolList[eEffect])
	{
		if (!effect->Get_Enable())
		{
			pEffect = effect;
			break;
		}
	}

	if (pEffect == nullptr)
	{
		_tchar tszName[16] = {};

		swprintf_s(tszName, L"%d", m_uiNameCnt++);
		m_uiNameCnt;

		wstring wstrObjName = m_wstrEffectName + tszName;
		switch (eEffect)
		{
		case EFFECT_DAGGER:
			pEffect = CDaggerThrow::Create(m_pGraphicDev, L"09.Effect", wstrObjName.c_str());
			break;
		case EFFECT_ARROW: 
			pEffect = CArrow::Create(m_pGraphicDev, L"09.Effect", wstrObjName.c_str());
			break;
		case EFFECT_SPELLSPHERE:
			pEffect = CSpellSphere::Create(m_pGraphicDev, L"09.Effect", wstrObjName.c_str());
			break;
		case EFFECT_EXPLOSION:
			pEffect = CExplosion::Create(m_pGraphicDev, L"09.Effect", wstrObjName.c_str());
			break;
		case EFFECT_SHOCK:
			pEffect = CShock::Create(m_pGraphicDev, L"09.Effect", wstrObjName.c_str());
			break;
		case EFFECT_BULLET:
			pEffect = CBullet::Create(m_pGraphicDev, L"09.Effect", wstrObjName.c_str());
			break;
		case EFFECT_BLOOD:
			pEffect = CBlood::Create(m_pGraphicDev, L"09.Effect", wstrObjName.c_str());
			break;
		}
		m_EffectPoolList[eEffect].push_back(pEffect);
	}

	return pEffect;
}

void CEffectMgr::PushEffect(EFFECT eEffect, CEffect * pEffect)
{
}

void CEffectMgr::Free(void)
{
	m_pGraphicDev->Release();
}
