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


class CGauge : public Engine::CGameObject

{
public:
	static enum Item { HP, MP, BLUE_KEY, RED_KEY, AMMO, EXE, ITEM_END };
public:
	explicit CGauge(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexint,Item eType, _vec3 vPos,_float fScale,_int iPivot );
	virtual ~CGauge(void);
public:
	void		Set_Camera(Engine::CGameObject* pCamera){m_pCamera = pCamera;}
	_vec2		GetItem_Pos(Item eType) { return m_ItemPos[eType];}
	void		Set_Item(_uint uiItem, _float fVal);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		CalculateTransform();
	void		SetPivot();
	void		SetScale();
	void		SetPos();

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CGameObject*	m_pCamera = nullptr;
	wstring					m_wstrTexture=L"";
	_matrix					m_MatWorld;
	_matrix					m_MatView;
	_vec3					m_vPos;
	_int					m_iPivot;
	_float					m_fScale;
	_vec2					m_ItemPos[ITEM_END];
	_float					m_fItemVal[ITEM_END];
	Item					m_eItemType;
public:
	static CGauge*		Create(LPDIRECT3DDEVICE9 pGraphicDev , wstring wstrTex,Item eType, _vec3 vPos,_float fScale=1.0f,_int iPivot = 3);

private:
	virtual void Free(void) override;
};

