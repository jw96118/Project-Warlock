#ifndef Roof_h__
#define Roof_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CLightTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CRoof : public Engine::CGameObject
{
private:
	explicit CRoof(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoof(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	void Set_Transform(Engine::CGameObject* pGameObjcet);
private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_Material(void);

private:
	_vec3					m_vDir;
	_float					m_fSpeed = 20.f;
	ID3DXLine*              m_pLine;
public:
	static CRoof*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // TempObject_h__
