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

class CMiniObject;
class CMiniMap : public Engine::CGameObject

{
public:
	explicit CMiniMap(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexint, _vec3 vPos,_float fScale,_int iPivot );
	virtual ~CMiniMap(void);
public:
public:
	void		Set_Camera(Engine::CGameObject* pCamera){m_pCamera = pCamera;}

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	HRESULT     Add_WallObject(Engine::CGameObject* pGameObject, wstring TexName);
private:
	HRESULT		Add_Component(void);
	void		CalculateTransform();
	void		SetPivot();
	void		SetScale();
	void		SetPos();


private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CGameObject*	m_pCamera = nullptr;
	wstring					m_wstrTexture = L"";
	_matrix					m_MatWorld;
	_matrix					m_MatView;
	_vec3					m_vPos;
	_int					m_iPivot;
	_float					m_fScale;

	vector<CMiniObject*>      m_vecWall;
public:
	static CMiniMap*		Create(LPDIRECT3DDEVICE9 pGraphicDev , wstring wstrTex, _vec3 vPos,_float fScale=1.0f,_int iPivot = 3);

private:
	virtual void Free(void) override;
};

