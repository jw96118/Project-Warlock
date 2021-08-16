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


class CMiniObject : public Engine::CGameObject

{
public:
	explicit CMiniObject(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexint );
	virtual ~CMiniObject(void);
public:
	void		Set_Camera(Engine::CGameObject* pCamera){m_pCamera = pCamera;}

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		CalculateTransform();
	void		SetPivot();
	void        Set_Transform2D(Engine::CTransform* pTransform);
	void		SetScale();
	void		SetPos();
private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CGameObject*	m_pCamera = nullptr;
	wstring					m_wstrTexture = L"";
	_matrix					m_MatWorld;
	_matrix					m_MatView;
	_matrix					m_MatRot;
	_matrix					m_MatScale;
	_matrix					m_MatPos;

	_vec3					m_vPos;
	_int					m_iPivot;
	_vec3					m_vScale;
	
	bool*                   m_pObjectEnable;
	_vec3                   m_vParentPos;
public:
	static CMiniObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev , wstring wstrTex, Engine::CTransform* pTransform, _vec3 vParentPos, bool* m_pObjectEnabel = nullptr);

private:
	virtual void Free(void) override;
};

