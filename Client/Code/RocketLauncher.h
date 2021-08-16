#ifndef RocketLauncher_h__
#define RocketLauncher_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END
class CFirstPersonCamera;

class CRocketLauncher : public CWeapon
{
private:
	explicit CRocketLauncher(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRocketLauncher(void);

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual HRESULT			LateReady_GameObject(void) override;
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
	Engine::CTexture*		m_pTextureCom[Animation::ROCKETLAUNCHER_END];

	CFirstPersonCamera*		m_pCamera;

	_bool					m_bShot;
	_float					m_fLimitDist;
	SPHERECOLLIDER			m_tCollider;

public:
	static CRocketLauncher*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // RocketLauncher_h__
