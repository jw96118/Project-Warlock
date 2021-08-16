#ifndef CrossBow_h__
#define CrossBow_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CCrossBow : public CWeapon
{
private:
	explicit CCrossBow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrossBow(void);

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

private:
	virtual HRESULT			Add_Component(void) override;
	virtual void			Change_Animation(_uint uiAnimationID) override;
	virtual void			Animator_GameObject(const _float& fTimeDelta) override;
	virtual void			Key_Input(const _float& fTimeDelta) override;
	virtual void			OnEnable_GameObject(void) override;

	void					Shot_Arrow(void);

private:
	Engine::CTexture*		m_pTextureCom[Animation::CROSSBOW_END];

	_bool					m_bShot;

public:
	static CCrossBow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // CrossBow_h__
