#ifndef WeaponMgr_h__
#define WeaponMgr_h__

#include "Defines.h"
#include "Base.h"

class CWeapon;

class CWeaponMgr : public CBase
{
	DECLARE_SINGLETON(CWeaponMgr)

private:
	explicit CWeaponMgr(void);
	virtual ~CWeaponMgr(void);

public:
	_uint					Get_AmmoCurCnt(AMMOTYPE eAmmoType);
	_uint					Get_AmmoMaxCnt(AMMOTYPE eAmmoType);
	_bool					Get_IsUnLock(WEAPONTYPE eType, WEAPONID eID);

public:
	void					Set_UnLock(WEAPONTYPE eType, WEAPONID eID, _bool bUnLock);

public:
	HRESULT					Ready_WeaponMgr(void);
	HRESULT					Setting_Weapon(CWeapon* pWeapon);
	void					Key_Input(void);
	void					Change_DefaultWeapon(void);
	void					UseAmmo(AMMOTYPE eAmmoTypem, _uint uiUsingCnt = 1);
	void					AddAmmo(AMMOTYPE eAmmoType, _uint uiAddAmmoCnt);
	void					Reset_UsingWeapon(void);
	CWeapon*				Get_CurWeapon();

private:
	CWeapon*				m_pWeapon[WEAPONTYPE_END][WEAPONID_END];
	_bool					m_bIsUnLock[WEAPONTYPE_END][WEAPONID_END];

	CWeapon*				m_pUsingWeapon;

	// ���� ź�� ��
	_uint					m_uiRevolverAmmoCurCnt;
	_uint					m_uiRevolverAmmoMaxCnt;


	// ������ �ֹ��� ��
	_uint					m_uiSpellAmmoCurCnt;
	_uint					m_uiSpellAmmoMaxCnt;

	// �̴ϰ� ź�� ��
	_uint					m_uiMinigunAmmoCurCnt;
	_uint					m_uiMinigunAmmoMaxCnt;

	// ���� ȭ�� ��
	_uint					m_uiArrowAmmoCurCnt;
	_uint					m_uiArrowAmmoMaxCnt;

	// ���Ϸ�ó ź�� ��
	_uint					m_uiRocketAmmoCurCnt;
	_uint					m_uiRocketAmmoMaxCnt;


public:
	virtual					void	Free(void);
};


#endif // WeaponMgr_h__