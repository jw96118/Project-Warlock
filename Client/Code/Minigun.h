#ifndef Minigun_h__
#define Minigun_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CMinigun : public CWeapon
{
private:
	explicit CMinigun(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMinigun(void);

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

	void					Reset_BeltAnimation(void);
	void					IsPlayAmmoBelt(const _float & fTimeDelta);
	void					Shot_Bullet(void);

private:
	Engine::CTexture*		m_pTextureCom[Animation::MINIGUN_END];
	_bool					m_bFire;
	FRAME					m_tBeltFrame;

	_uint					m_iMaxBulletCnt;
	_uint					m_iCurBulletCnt;

	_bool					m_bEmptyRotate;
	_bool					m_bFireBullet;
	_bool					m_bShot;

	// Sound
	_bool					m_bRotateSound;

public:
	static CMinigun*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // Revolver_h__
