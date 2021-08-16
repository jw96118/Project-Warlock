#ifndef EffectMgr_h__
#define EffectMgr_h__

#include "Defines.h"
#include "Base.h"

class CEffect;

class CEffectMgr : public CBase
{
	DECLARE_SINGLETON(CEffectMgr)

private:
	explicit CEffectMgr(void);
	virtual ~CEffectMgr(void);

public:
	HRESULT Ready_EffectMgr(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Reset_Effect(void);

public:
	CEffect* PopEffect(EFFECT eEffect);
	void PushEffect(EFFECT eEffect, CEffect* pEffect);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	list<CEffect*>		m_EffectPoolList[EFFECT_END];

	wstring				m_wstrEffectName;
	_uint				m_uiNameCnt;

public:
	virtual	void				Free(void);
};


#endif // EffectMgr_h__