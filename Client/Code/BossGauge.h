#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END

class CBossGauge : public Engine::CGameObject
{
public:
	CBossGauge(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexint, Engine::CTransform* pTransform, _float fScale ,_uint uiBossType);
	~CBossGauge();

public:
	void			SetScale(_float fScale);
	void			SetPos(_vec3 vPos);
	void			Set_HPRatio(_float fHPRatio);
public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(void) override;
private:
	HRESULT			Add_Component(void);



private:
	Engine::CTransform*		m_pTargetTransform = nullptr;
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CGameObject*	m_pCamera = nullptr;
	_uint					m_uiBossType;
	wstring					m_wstrTexture = L"";
	_matrix					m_MatWorld;
	_matrix					m_MatView;
	_vec3					m_vPos;
	_float					m_fScale;
	_float					m_fHPRatio=1.f;
public:
	static CBossGauge*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTex, Engine::CTransform* pTransform, _float fScale = 1.0f, _uint uiBossType=0);

private:
	virtual void Free(void) override;
};


