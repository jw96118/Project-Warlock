#ifndef Col_h__
#define Col_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CColObject : public Engine::CGameObject
{
private:
	explicit CColObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CColObject(void);

public:
	const BOXCOLLIDER*		Get_Collider(void) const;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT LateReady_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);


private:
	BOXCOLLIDER			m_tCollider;

public:
	static CColObject*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // Col_h__
