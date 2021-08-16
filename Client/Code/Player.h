#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"
#include "FirstPersonCamera.h"
#include "Shop.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CTexture;
class CLightTex;
class CCamera;

END
class CGauge;
class CWeaponMgr;

class CPlayer : public Engine::CGameObject
{
	static enum Item { HP, MP, BLUE_KEY, RED_KEY, AMMO, EXE, ITEM_END };

private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

public:
	BOXCOLLIDER*			Get_BoxCollider(void);
	SPHERECOLLIDER          Get_HitCollider(void);
	const OBJ_DATA&			Get_Data(void);
	const bool&             Get_MiniDead(void) const;
	const bool&             Get_MiniEvnet(void) const;
	const bool&             Get_MiniGame(void) const;
	void					Set_Ammo(_float fCurAmmo,_float fMaxAmmo);
	void					Set_MP(_float fCurMP, _float fMaxMP);
	void					Set_UpgradePoint(_uint uiUpgradePoint);
	void                    Set_MiniDead(bool flag);
	void                    Set_MiniEvent(bool flag);
	void					Add_UpgradePoint();
	void					Add_CurHP(_float fHP);
	void					Add_CurMP(_float fMP);
	void					Add_CurAmmo(_float fMP);
	void					BeAttack(_float fDamage);
	OBJ_DATA*				Get_DataInfo(void);
	_uint*					Get_UpgradePoint(void);

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual HRESULT			LateReady_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	void					Update_Collider(void);
	void					Update_Gauge(void);

private:
	HRESULT					Add_Component(void);
	void					Level_UP(void);

private:
	CWeaponMgr*				m_pWeaponMgr;
	CGauge*					m_pGauge[ITEM_END];


	BOXCOLLIDER				m_tBoxCollider;
	SPHERECOLLIDER			m_tHitCollider;

	OBJ_DATA				m_tData;
	_float					m_fCurMP;
	_float					m_fMaxMP;
	_float					m_fCurAmmo;
	_float					m_fMaxAmmo;
	_uint					m_uiUpgradePoint;

	_bool                   m_bMiniDead = false;
	_bool                   m_bMiniGame = false;

	_bool                   m_bMiniEvent = false;

	CShop*					m_pShop;

public:
	static CPlayer*			Create(LPDIRECT3DDEVICE9 pGraphicDev, bool bMiniGame = false);

private:
	virtual void			Free(void) override;
};

#endif // Player_h__
