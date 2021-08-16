#ifndef Shop_h__
#define Shop_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CShop : public Engine::CGameObject
{
protected:
	explicit CShop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShop(void);

private:
	enum UPGRADE { UPGRADE_COLDSTAFF, UPGRADE_CROSSBOW3, UPGRADE_END};

public:
	virtual void			Set_Enable(_bool bEnable) override;

public:
	void					ToString_UpgradePoint(_uint uiUpgradePoint);

protected:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	virtual void			OnEnable_GameObject(void) override;
	virtual	HRESULT			Add_Component(void);

private:
	void					Upgrade_Weapon(void);

protected:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CRenderer*		m_pRendererCom;

	Engine::CTexture*		m_pSlotSelectedCom;
	Engine::CTexture*		m_pUpgradeAvailableCom;
	Engine::CTexture*		m_pUpgradePressedCom;
	Engine::CTexture*		m_pColdStaffHudCom;
	Engine::CTexture*		m_pCrossBow3HudCom;
	Engine::CTexture*		m_pSlotUpgradeCom;

	_vec3					m_vPivotPos;
	_matrix					m_matShopView;
	_matrix					m_matSlotStaffView;
	_matrix					m_matSlotCrossBowView;
	_matrix					m_matColdStaffView;
	_matrix					m_matCrossBow3View;
	_matrix					m_matUpgradeAvailableView;
	_matrix					m_matUpgradePressedView;

	_bool					m_bStaffOnSlot;
	_bool					m_bCrossBowOnSlot;
	_bool					m_bUpgradePressed;

	UPGRADE					m_eUpgrade;

	_uint					m_uiColdStaffPoint;
	_uint					m_uiCrossBow3Point;

	_bool					m_bIsUpgradeColdStaff;
	_bool					m_bIsUpgradeCrossBow;

	_tchar					m_tszUpgradePoint[8];

public:
	static CShop*			Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void			Free(void) override;
};

#endif // Shop_h__
