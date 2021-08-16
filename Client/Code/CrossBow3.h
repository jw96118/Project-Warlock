#ifndef CrossBow3_h__
#define CrossBow3_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CCrossBow3 : public CWeapon
{
private:
	explicit CCrossBow3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrossBow3(void);

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

private:
	virtual HRESULT			Add_Component(void) override;
	virtual void			Change_Animation(_uint uiAnimationID) override;
	virtual void			Animator_GameObject(const _float& fTimeDelta) override;
	virtual void			Key_Input(const _float& fTimeDelta) override;
	virtual void			Mouse_Move(const _float& fTimeDelta) override;
	virtual void			OnEnable_GameObject(void) override;
	virtual void			Change_WeaponAction(const _float & fTimeDelta) override;

	void					Shot_Arrow(void);

private:
	Engine::CTexture*		m_pTextureCom[Animation::CROSSBOW3_END];
	_bool					m_bFrameAction;

	// Test Code
	//_uint					m_iArrowCnt;

public:
	static CCrossBow3*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // CrossBow3_h__
