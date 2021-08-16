#ifndef ColdStaff_h__
#define ColdStaff_h__

#include "Defines.h"
#include "Weapon.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CColdStaff : public CWeapon
{
private:
	explicit CColdStaff(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CColdStaff(void);

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

private:
	Engine::CTexture*		m_pTextureCom[Animation::COLDSTAFF_END];

	_uint					m_uiUsingMP;

public:
	static CColdStaff*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void			Free(void) override;
};

#endif // ColdStaff_h__
