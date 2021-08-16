#ifndef Dagger_h__
#define Dagger_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;
class CTransform;

END

class CFirstPersonCamera;

class CDagger : public CWeapon
{
private:
	explicit CDagger(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDagger(void);

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

	void					Update_Collider(void);
	void					Stab_Dagger(void);
	void					Throw_Dagger(void);

private:
	Engine::CTexture*		m_pTextureCom[Animation::DAGGER_END];
	_float					m_fThrowKeyPressTime;
	_bool					m_bThrowKeyPress;
	_bool					m_bUsableSkill;
	_bool					m_bPlayThrowAnimation;

	CFirstPersonCamera*		m_pCamera;
	Engine::CTransform*		m_pPlayerTransform;
	SPHERECOLLIDER			m_tCollider;

	_bool					m_bStab;
	_bool					m_bThrow;

	// Sound
	_bool					m_bRecallSound;

public:
	static CDagger*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // Dagger_h__
