#pragma once

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CScrollTex;
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CCalculator;

END


class CTitle : public Engine::CGameObject
{
public:
	static enum SCROLLDIR{LEFT,RIGHT};
	explicit CTitle(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexint, SCROLLDIR eScrollDIr, _vec3 vPos,_float fScale, _float fSpd);
	virtual ~CTitle(void);
public:
	void		Set_Camera(Engine::CGameObject* pCamera){m_pCamera = pCamera;}

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		CalculateTransform();
	void		SetScale();
	void		SetPos();

private:
	Engine::CScrollTex*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CGameObject*	m_pCamera = nullptr;
	wstring					m_wstrTexture=L"";
	_matrix					m_MatWorld;
	_matrix					m_MatView;
	_vec3					m_vPos;
	_float					m_fScale;
	_float					m_fScroll;
	_float					m_fSpd;
	SCROLLDIR				m_eSCroll;
	//_tchar					m_szScrollVal[128];//Degug

public:
	static CTitle*		Create(LPDIRECT3DDEVICE9 pGraphicDev , wstring wstrTex, SCROLLDIR eScrollDIr, _vec3 vPos,_float fScale=1.0f, _float fSpd=1.0f);

private:
	virtual void Free(void) override;
};

