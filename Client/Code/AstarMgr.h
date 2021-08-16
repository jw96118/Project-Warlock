
#ifndef AtarMgr_h__
#define AtarMgr_h__

#include "Defines.h"
#include "Base.h"

class CAstarMgr : public CBase
{
	DECLARE_SINGLETON(CAstarMgr)

private:
	CAstarMgr();
	~CAstarMgr();

//public:
//	list<TILE_INFO*>& GetBestPath();
//
//public:
//	void StartAstar(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos);
//
//private:
//	bool PathFinding(int iStartIndex, int iGoalIndex);
//	void MakePath(int iStartIndex, int iGoalIndex);
//	bool IsInOpenlist(int iIndex);
//	bool IsInCloselist(int iIndex);
//
//private:
//	//list<int>			m_OpenList;
//	//list<int>			m_CloseList;
//	//list<TILE_INFO*>	m_BestPath; // 만들어진 경로	
//
//	//CObjectMgr*			m_pObjectMgr;
//	//CTerrain*			m_pTerrain;
//
//	//int					m_iStartIndex;	
private:
	virtual void Free() override;
};

#endif