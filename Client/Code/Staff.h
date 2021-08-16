#ifndef Staff_h__
#define Staff_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CStaff : public CWeapon
{
private:
	explicit CStaff(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaff(void);

private:
	enum CHARELEVEL { CHARGE_LEVEL1, CHARGE_LEVEL2, CHARGE_LEVEL3, CHARGE_END };

public:
	void					Set_Usable(const _bool bUseable);

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

	void					Shot_SpellSphere(void);

private:
	Engine::CTexture*		m_pTextureCom[Animation::STAFF_END];

	_float					m_fFireKeyPressTime;
	_float					m_fChargeTable[CHARGE_END];
	_float					m_fChargeScale;

	_bool					m_bChargeLevel2;
	_bool					m_bChargeLevel3;

	_bool					m_bFire;
	_bool					m_bRelease;

	_bool					m_bUseable;

public:
	static CStaff*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // Dagger_h__
