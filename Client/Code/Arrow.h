#ifndef Arrow_h__
#define Arrow_h__

#include "Defines.h"
#include "Effect.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CFirstPersonCamera;

class CArrow : public CEffect
{
protected:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArrow(void);

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

	virtual	HRESULT			Add_Component(void) override;
	virtual void			Update_Collider(void) override;

	void					OnEffect(const _vec3& vPos, const _vec3& vMoveDir, const _float& fDamage);
	void					Update_WorldMatrix(void);
	void					Move_Arrow(const _float& fTimeDelta);
	_bool					IsCollision_OtherGameObject();


private:
	CFirstPersonCamera*		m_pCamera;
	_float					m_fSpeed;

public:
	static CArrow*			Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pLayerTag, const _tchar * pObjectTag);

protected:
	virtual void			Free(void) override;
};

#endif // Arrow_h__
