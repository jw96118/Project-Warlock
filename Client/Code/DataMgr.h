#ifndef DataMgr_h__
#define DataMgr_h__

#include "Defines.h"
#include "Base.h"

class CPlayer;

class CDataMgr : public CBase
{
	DECLARE_SINGLETON(CDataMgr)

private:
	explicit CDataMgr(void);
	virtual ~CDataMgr(void);

public:
	const OBJ_DATA&				Get_PlayerData(void) const;

public:
	HRESULT						Ready_DataMgr();

	void						Copy_DataToPlayer(CPlayer* pPlayer);
	void						Copy_DataToMgr(CPlayer* pPlayer);

private:
	OBJ_DATA					m_tData;
	_uint						m_uiUpgradePoint;

public:
	virtual	void				Free(void);
};


#endif // DataMgr_h__