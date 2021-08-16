#ifndef Revolver_h__
#define Revolver_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CRevolver : public CWeapon
{
private:
	explicit CRevolver(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRevolver(void);

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

	void					Shot_Bullet(void);

private:
	Engine::CTexture*		m_pTextureCom[Animation::REVOLVER_END];

	_bool					m_bShot;

public:
	static CRevolver*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // Revolver_h__
