#include "stdafx.h"

#include "Weapon.h"
#include "WeaponMgr.h"
#include "Staff.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CWeaponMgr)

CWeaponMgr::CWeaponMgr()
	: m_pUsingWeapon(nullptr)
	, m_uiRevolverAmmoCurCnt(0)
	, m_uiRevolverAmmoMaxCnt(0)
	, m_uiSpellAmmoCurCnt(0)
	, m_uiSpellAmmoMaxCnt(0)
	, m_uiMinigunAmmoCurCnt(0)
	, m_uiMinigunAmmoMaxCnt(0)
	, m_uiArrowAmmoCurCnt(0)
	, m_uiArrowAmmoMaxCnt(0)
	, m_uiRocketAmmoCurCnt(0)
	, m_uiRocketAmmoMaxCnt(0)
{
	for (_int i = 0; i < WEAPONTYPE_END; ++i)
		for (_int j = 0; j < WEAPONID_END; ++j)
			m_pWeapon[i][j] = nullptr;

	for (_int i = 0; i < WEAPONTYPE_END; ++i)
		for (_int j = 0; j < WEAPONID_END; ++j)
			m_bIsUnLock[i][j] = true;
}


CWeaponMgr::~CWeaponMgr()
{
	Free();
}

_uint CWeaponMgr::Get_AmmoCurCnt(AMMOTYPE eAmmoType)
{
	_uint uiAmmoCurCnt = 0;

	switch (eAmmoType)
	{
	case AMMO_REVOLVER:
		uiAmmoCurCnt = m_uiRevolverAmmoCurCnt;
		break;
	case AMMO_SPELL:
		uiAmmoCurCnt = m_uiSpellAmmoCurCnt;
		break;
	case AMMO_ARROW:
		uiAmmoCurCnt = m_uiArrowAmmoCurCnt;
		break;
	case AMMO_ROCKETLAUNCHER:
		uiAmmoCurCnt = m_uiRocketAmmoCurCnt;
		break;
	case AMMO_MINIGUN:
		uiAmmoCurCnt = m_uiMinigunAmmoCurCnt;
		break;
	}

	return uiAmmoCurCnt;
}

_uint CWeaponMgr::Get_AmmoMaxCnt(AMMOTYPE eAmmoType)
{
	_uint uiAmmoMaxCnt = 0;

	switch (eAmmoType)
	{
	case AMMO_REVOLVER:
		uiAmmoMaxCnt = m_uiRevolverAmmoMaxCnt;
		break;
	case AMMO_SPELL:
		uiAmmoMaxCnt = m_uiSpellAmmoMaxCnt;
		break;
	case AMMO_ARROW:
		uiAmmoMaxCnt = m_uiArrowAmmoMaxCnt;
		break;
	case AMMO_ROCKETLAUNCHER:
		uiAmmoMaxCnt = m_uiRocketAmmoMaxCnt;
		break;
	case AMMO_MINIGUN:
		uiAmmoMaxCnt = m_uiMinigunAmmoMaxCnt;
		break;
	}

	return uiAmmoMaxCnt;
}

_bool CWeaponMgr::Get_IsUnLock(WEAPONTYPE eType, WEAPONID eID)
{
	return m_bIsUnLock[eType][eID];
}

void CWeaponMgr::Set_UnLock(WEAPONTYPE eType, WEAPONID eID, _bool bUnLock)
{
	m_bIsUnLock[eType][eID] = bUnLock;
}

HRESULT CWeaponMgr::Ready_WeaponMgr(void)
{
	// Test Code
	//m_bIsUnLock[WEAPONTYPE_DEFAULT][WEAPONID_02] = true;
	//m_bIsUnLock[WEAPONTYPE_STAFF][WEAPONID_01] = true;
	//m_bIsUnLock[WEAPONTYPE_GUN][WEAPONID_02] = true;

	m_bIsUnLock[WEAPONTYPE_STAFF][WEAPONID_02] = false;
	m_bIsUnLock[WEAPONTYPE_CROSSBOW][WEAPONID_02] = false;

	// Test Code
	m_uiMinigunAmmoMaxCnt = 500;
	m_uiMinigunAmmoCurCnt = m_uiMinigunAmmoMaxCnt;

	m_uiRevolverAmmoMaxCnt = 100;
	m_uiRevolverAmmoCurCnt = m_uiRevolverAmmoMaxCnt;

	m_uiArrowAmmoMaxCnt = 10;
	m_uiArrowAmmoCurCnt = m_uiArrowAmmoMaxCnt;

	m_uiRocketAmmoMaxCnt = 10;
	m_uiRocketAmmoCurCnt = m_uiRocketAmmoMaxCnt;

	m_uiSpellAmmoMaxCnt = 10;
	m_uiSpellAmmoCurCnt = m_uiSpellAmmoMaxCnt;

	return S_OK;
}

HRESULT CWeaponMgr::Setting_Weapon(CWeapon* pWeapon)
{
	NULL_CHECK_RETURN(pWeapon, E_FAIL);

	const WEAPONID& eWeaponID = pWeapon->Get_WeaponID();
	const WEAPONTYPE& eWeaponType = pWeapon->Get_WeaponType();

	m_pWeapon[eWeaponType][eWeaponID] = pWeapon;

	return S_OK;
}

void CWeaponMgr::Key_Input(void)
{
	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_NUM1))
	{
		if (m_pUsingWeapon != nullptr)
		{
			// 현재 장착한 무기와 같은 타입의 무기 교체
			if (m_pUsingWeapon->Get_WeaponType() == WEAPONTYPE_DEFAULT)
			{
				WEAPONID eChangeID = (m_pUsingWeapon->Get_WeaponID() == WEAPONID_01) ? WEAPONID_02 : WEAPONID_01;

				// 교체할 무기 잠김 해제 확인
				if (m_bIsUnLock[WEAPONTYPE_DEFAULT][eChangeID])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_DEFAULT][eChangeID];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
			// 현재 장착한 무기와 다른 타입의 무기 교체
			else
			{
				if (m_bIsUnLock[WEAPONTYPE_DEFAULT][WEAPONID_01])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_DEFAULT][WEAPONID_01];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
				if (m_bIsUnLock[WEAPONTYPE_DEFAULT][WEAPONID_02])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_DEFAULT][WEAPONID_02];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
		}
		else
		{
			if (m_bIsUnLock[WEAPONTYPE_DEFAULT][WEAPONID_01])
			{

				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_DEFAULT][WEAPONID_01];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}

			if (m_bIsUnLock[WEAPONTYPE_DEFAULT][WEAPONID_02])
			{
				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_DEFAULT][WEAPONID_02];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}
		}
	}
	else if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_NUM2))
	{
		if (m_pUsingWeapon != nullptr)
		{
			// 현재 장착한 무기와 같은 타입의 무기 교체
			if (m_pUsingWeapon->Get_WeaponType() == WEAPONTYPE_STAFF)
			{
				WEAPONID eChangeID = (m_pUsingWeapon->Get_WeaponID() == WEAPONID_01) ? WEAPONID_02 : WEAPONID_01;

				// 교체할 무기 잠김 해제 확인
				if (m_bIsUnLock[WEAPONTYPE_STAFF][eChangeID])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_STAFF][eChangeID];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
			// 현재 장착한 무기와 다른 타입의 무기 교체
			else
			{
				if (m_bIsUnLock[WEAPONTYPE_STAFF][WEAPONID_01])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_STAFF][WEAPONID_01];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}

				if (m_bIsUnLock[WEAPONTYPE_STAFF][WEAPONID_02])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_STAFF][WEAPONID_02];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
		}
		else
		{
			if (m_bIsUnLock[WEAPONTYPE_STAFF][WEAPONID_01])
			{
				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_STAFF][WEAPONID_01];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}

			if (m_bIsUnLock[WEAPONTYPE_STAFF][WEAPONID_02])
			{
				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_STAFF][WEAPONID_02];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}
		}
	}
	else if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_NUM3))
	{
		if (m_pUsingWeapon != nullptr)
		{
			// 현재 장착한 무기와 같은 타입의 무기 교체
			if (m_pUsingWeapon->Get_WeaponType() == WEAPONTYPE_CROSSBOW)
			{
				WEAPONID eChangeID = (m_pUsingWeapon->Get_WeaponID() == WEAPONID_01) ? WEAPONID_02 : WEAPONID_01;

				// 교체할 무기 잠김 해제 확인
				if (m_bIsUnLock[WEAPONTYPE_CROSSBOW][eChangeID])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_CROSSBOW][eChangeID];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
			// 현재 장착한 무기와 다른 타입의 무기 교체
			else
			{
				if (m_bIsUnLock[WEAPONTYPE_CROSSBOW][WEAPONID_01])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_CROSSBOW][WEAPONID_01];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}

				if (m_bIsUnLock[WEAPONTYPE_CROSSBOW][WEAPONID_02])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_CROSSBOW][WEAPONID_02];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
		}
		else
		{
			if (m_bIsUnLock[WEAPONTYPE_CROSSBOW][WEAPONID_01])
			{
				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_CROSSBOW][WEAPONID_01];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}

			if (m_bIsUnLock[WEAPONTYPE_CROSSBOW][WEAPONID_02])
			{
				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_CROSSBOW][WEAPONID_02];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}

		}
	}
	else if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_NUM4))
	{
		if (m_pUsingWeapon != nullptr)
		{
			// 현재 장착한 무기와 같은 타입의 무기 교체
			if (m_pUsingWeapon->Get_WeaponType() == WEAPONTYPE_GUN)
			{
				WEAPONID eChangeID = (m_pUsingWeapon->Get_WeaponID() == WEAPONID_01) ? WEAPONID_02 : WEAPONID_01;

				// 교체할 무기 잠김 해제 확인
				if (m_bIsUnLock[WEAPONTYPE_GUN][eChangeID])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_GUN][eChangeID];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
			// 현재 장착한 무기와 다른 타입의 무기 교체
			else
			{
				if (m_bIsUnLock[WEAPONTYPE_GUN][WEAPONID_01])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_GUN][WEAPONID_01];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}

				if (m_bIsUnLock[WEAPONTYPE_GUN][WEAPONID_02])
				{
					if (!m_pUsingWeapon->Get_IsIdle())
						return;

					m_pUsingWeapon->Set_Enable(false);
					m_pUsingWeapon = m_pWeapon[WEAPONTYPE_GUN][WEAPONID_02];
					m_pUsingWeapon->Set_Enable(true);

					return;
				}
			}
		}
		else
		{
			if (m_bIsUnLock[WEAPONTYPE_GUN][WEAPONID_01])
			{
				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_GUN][WEAPONID_01];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}

			if (m_bIsUnLock[WEAPONTYPE_GUN][WEAPONID_02])
			{
				m_pUsingWeapon = m_pWeapon[WEAPONTYPE_GUN][WEAPONID_02];
				m_pUsingWeapon->Set_Enable(true);

				return;
			}
		}
	}
}

void CWeaponMgr::Change_DefaultWeapon(void)
{
	m_pUsingWeapon->Set_Enable(false);
	m_pUsingWeapon = m_pWeapon[WEAPONTYPE_DEFAULT][WEAPONID_01];
	m_pUsingWeapon->Set_Enable(true);
}

void CWeaponMgr::UseAmmo(AMMOTYPE eAmmoType, _uint uiUsingCnt /*= 1*/)
{
	switch (eAmmoType)
	{
	case AMMO_REVOLVER:
		m_uiRevolverAmmoCurCnt -= uiUsingCnt;
		break;
	case AMMO_SPELL:
		m_uiSpellAmmoCurCnt -= uiUsingCnt;
		break;
	case AMMO_ARROW:
		m_uiArrowAmmoCurCnt -= uiUsingCnt;
		break;
	case AMMO_ROCKETLAUNCHER:
		m_uiRocketAmmoCurCnt -= uiUsingCnt;
		break;
	case AMMO_MINIGUN:
		m_uiMinigunAmmoCurCnt -= uiUsingCnt;
		break;
	}
}

void CWeaponMgr::AddAmmo(AMMOTYPE eAmmoType, _uint uiAddAmmoCnt)
{
	switch (eAmmoType)
	{
	case AMMO_REVOLVER:
		m_uiRevolverAmmoCurCnt += uiAddAmmoCnt;
		if (m_uiRevolverAmmoCurCnt > m_uiRevolverAmmoMaxCnt)
			m_uiRevolverAmmoCurCnt = m_uiRevolverAmmoMaxCnt;
		break;
	case AMMO_SPELL:
		m_uiSpellAmmoCurCnt += uiAddAmmoCnt;
		if (m_uiSpellAmmoCurCnt > 0)
			dynamic_cast<CStaff*>(m_pWeapon[WEAPONTYPE_STAFF][WEAPONID_01])->Set_Usable(true);
		if (m_uiSpellAmmoCurCnt > m_uiSpellAmmoMaxCnt)
			m_uiSpellAmmoCurCnt = m_uiSpellAmmoMaxCnt;
		break;
	case AMMO_ARROW:
		m_uiArrowAmmoCurCnt += uiAddAmmoCnt;
		if (m_uiArrowAmmoCurCnt > m_uiArrowAmmoMaxCnt)
			m_uiArrowAmmoCurCnt = m_uiArrowAmmoMaxCnt;
		break;
	case AMMO_ROCKETLAUNCHER:
		m_uiRocketAmmoCurCnt += uiAddAmmoCnt;
		if (m_uiRocketAmmoCurCnt > m_uiRocketAmmoMaxCnt)
			m_uiRocketAmmoCurCnt = m_uiRocketAmmoMaxCnt;
		break;
	case AMMO_MINIGUN:
		m_uiMinigunAmmoCurCnt += uiAddAmmoCnt;
		if (m_uiMinigunAmmoCurCnt > m_uiMinigunAmmoMaxCnt)
			m_uiMinigunAmmoCurCnt = m_uiMinigunAmmoMaxCnt;
		break;
	}
}

void CWeaponMgr::Reset_UsingWeapon(void)
{
	m_pUsingWeapon = nullptr;
}

CWeapon * CWeaponMgr::Get_CurWeapon()
{
	return m_pUsingWeapon;
}

void CWeaponMgr::Free(void)
{
}
