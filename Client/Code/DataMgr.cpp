#include "stdafx.h"

#include "DataMgr.h"
#include "Player.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CDataMgr)

CDataMgr::CDataMgr()
	: m_uiUpgradePoint(0)
{
}


CDataMgr::~CDataMgr()
{
	Free();
}

const OBJ_DATA & CDataMgr::Get_PlayerData(void) const
{
	return m_tData;
}

HRESULT CDataMgr::Ready_DataMgr()
{
	m_tData.uiLev = 1;
	m_tData.fMaxHP = 150.f;
	m_tData.fCurHP = m_tData.fMaxHP;
	m_tData.fDamage = 15.f;
	m_tData.fMaxEXE = 100.f;
	m_tData.fCurEXE = 0.f;

	return S_OK;
}

void CDataMgr::Copy_DataToPlayer(CPlayer * pPlayer)
{
	*pPlayer->Get_DataInfo() = m_tData;
	*pPlayer->Get_UpgradePoint() = m_uiUpgradePoint;
}

void CDataMgr::Copy_DataToMgr(CPlayer * pPlayer)
{
	m_tData = *pPlayer->Get_DataInfo();
	m_uiUpgradePoint = *pPlayer->Get_UpgradePoint();
}

void CDataMgr::Free(void)
{
}
