#ifndef Explosion_h__
#define Explosion_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CFirstPersonCamera;

class CExplosion : public CEffect
{
protected:
	explicit CExplosion(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CExplosion(void);

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

	virtual	HRESULT			Add_Component(void) override;
	virtual void			Update_Collider(void) override;
	void					OnEffect(const _vec3& vPos);
	void					Update_WorldMatrix(void);
	void					Animator_GameObject(const _float& fTimeDelta);

private:
	CFirstPersonCamera*		m_pCamera;

	FRAME					m_tFrame;

public:
	static CExplosion*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pLayerTag, const _tchar * pObjectTag);

protected:
	virtual void			Free(void) override;
};

#endif // Explosion_h__
