#ifndef Weapon_h__
#define Weapon_h__

#include "Defines.h"
#include "GameObject.h"
#include "Struct.h"

BEGIN(Engine)

class CRenderer;
class CTexture;
class CRcTex;

END

class CEffectMgr;
class CWeaponMgr;
class CPlayer;
class CShop;

class CWeapon : public Engine::CGameObject
{
protected:
	explicit CWeapon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWeapon(void);

protected:
	enum MOVE_DIR { MOVE_RIGHT, MOVE_LEFT, MOVE_END };

public:
	const WEAPONID&			Get_WeaponID() const;
	const WEAPONTYPE&		Get_WeaponType() const;
	const _bool&			Get_IsIdle() const;
	const _float&			Get_Damage() const;

public:
	virtual HRESULT			Ready_GameObject(void) override;
	virtual HRESULT			LateReady_GameObject(void) override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	virtual void			OnEnable_GameObject(void) override;

protected:
	virtual HRESULT			Add_Component(void);
	virtual void			Change_Animation(_uint uiAnimationID) PURE;
	virtual void			Animator_GameObject(const _float& fTimeDelta) PURE;
	virtual void			Key_Input(const _float& fTimeDelta);
	virtual void			Mouse_Move(const _float& fTimeDelta);
	virtual void			Change_WeaponAction(const _float & fTimeDelta);

protected:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CRenderer*		m_pRendererCom;

	CWeaponMgr*				m_pWeaponMgr;
	CEffectMgr*				m_pEffectMgr;

	CPlayer*				m_pPlayer;

	_matrix					m_matView;
	FRAME					m_tFrame;
	_vec3					m_vScale;
	_vec3					m_vPos;
	_vec3					m_vPivotPos;
	_vec3					m_vTextureOffset;
	_float					m_fDamage;

	/* 플레이어 이동 시 무기 흔들림 관련 변수 */
	_bool					m_bIsMovePlayer;
	_int					m_iMoveOrder;
	_float					m_fDegree;
	_float					m_fMoveSpeedX;
	_float					m_fMoveSpeedY;

	/* 무기 아이디, 무기 타입*/
	WEAPONID				m_eWeaponID;
	WEAPONTYPE				m_eWeaponType;

	/* 무기 교체 알림 */
	_bool					m_bIsIdle;
	_bool					m_bIsChanging;
	_vec3					m_vChangingPos;

	CShop*					m_pShop;

protected:
	virtual void			Free(void) override;
};

#endif // Weapon_h__
