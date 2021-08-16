#include "stdafx.h"
#include "AstarMgr.h"


IMPLEMENT_SINGLETON(CAstarMgr)

CAstarMgr::CAstarMgr()
	//: 
	//m_pTerrain(nullptr),
	//m_iStartIndex(0)
{
}


CAstarMgr::~CAstarMgr()
{
}
//
//list<TILE_INFO*>& CAstarMgr::GetBestPath()
//{
//	// TODO: 여기에 반환 구문을 삽입합니다.
//	return m_BestPath;
//}
//
//void CAstarMgr::StartAstar(const D3DXVECTOR3& vStartPos, const D3DXVECTOR3& vGoalPos)
//{
//	m_pTerrain = dynamic_cast<CTerrain*>(m_pObjectMgr->GetTerrain());
//	NULL_CHECK(m_pTerrain);
//
//	m_OpenList.clear();
//	m_CloseList.clear();
//
//
//	for (auto pTile : m_pTerrain->GetVecTile())
//	{
//#ifdef _DEBUG
//		if(0 == pTile->byOption)
//			pTile->byDrawID = 2;
//#endif
//
//		pTile->iCost = 1;
//	}
//
//
//	m_BestPath.clear();
//
//	m_iStartIndex = m_pTerrain->GetTileIndex(vStartPos);
//	int iGoalIndex = m_pTerrain->GetTileIndex(vGoalPos);
//
//	if (-1 == m_iStartIndex || -1 == iGoalIndex)
//		return;
//
//	if (m_iStartIndex == iGoalIndex)
//		return;
//
//	if (1 == m_pTerrain->GetVecTile()[iGoalIndex]->byOption)
//		return;
//
//	if (PathFinding(m_iStartIndex, iGoalIndex))
//		MakePath(m_iStartIndex, iGoalIndex);
//}
//
//bool CAstarMgr::PathFinding(int iStartIndex, int iGoalIndex)
//{
//	if(!m_OpenList.empty())
//		m_OpenList.pop_front();
//
//	m_CloseList.push_back(iStartIndex);
//
//	for (auto pTile : m_pTerrain->GetVecGraph()[iStartIndex])
//	{
//		if (iGoalIndex == pTile->iMyIndex)
//		{
//			pTile->iParentIndex = iStartIndex;
//			return true;
//		}
//
//		if (!IsInOpenlist(pTile->iMyIndex) && !IsInCloselist(pTile->iMyIndex))
//		{
//			pTile->iParentIndex = iStartIndex;
//			pTile->iCost += m_pTerrain->GetVecTile()[iStartIndex]->iCost;
//			m_OpenList.push_back(pTile->iMyIndex);
//
//#ifdef _DEBUG
//			pTile->byDrawID = 3;
//#endif
//		}
//	}
//
//	if (m_OpenList.empty()) // 더이상 찾을 길이 없다.
//		return false;
//
//	// 휴리스틱 비용을 조사하고 난 후에 오픈 리스트를 비용이 적은 쪽으로 정렬.
//	m_OpenList.sort(
//		[&](int iPreIndex, int iNextIndex)
//	{
//		// 휴리스틱 = 최초 출발점과의 거리 + 최종 도착점과의 거리
//		D3DXVECTOR3 v1 = m_pTerrain->GetVecTile()[m_iStartIndex]->vPos 
//			- m_pTerrain->GetVecTile()[iPreIndex]->vPos;
//		D3DXVECTOR3 v2 = m_pTerrain->GetVecTile()[iGoalIndex]->vPos
//			- m_pTerrain->GetVecTile()[iPreIndex]->vPos;
//		float fHueristikA = (D3DXVec3Length(&v1) + D3DXVec3Length(&v2)) * m_pTerrain->GetVecTile()[iPreIndex]->iCost;
//
//		D3DXVECTOR3 v3 = m_pTerrain->GetVecTile()[m_iStartIndex]->vPos
//			- m_pTerrain->GetVecTile()[iNextIndex]->vPos;
//		D3DXVECTOR3 v4 = m_pTerrain->GetVecTile()[iGoalIndex]->vPos
//			- m_pTerrain->GetVecTile()[iNextIndex]->vPos;
//		float fHueristikB = (D3DXVec3Length(&v3) + D3DXVec3Length(&v4)) * m_pTerrain->GetVecTile()[iNextIndex]->iCost;
//
//		return fHueristikA < fHueristikB;
//	});
//
//	return PathFinding(m_OpenList.front(), iGoalIndex);
//}
//
//void CAstarMgr::MakePath(int iStartIndex, int iGoalIndex)
//{
//	m_BestPath.push_front(m_pTerrain->GetVecTile()[iGoalIndex]);
//	int iParentIndex = m_pTerrain->GetVecTile()[iGoalIndex]->iParentIndex;
//
//	while (true)
//	{
//		if (iParentIndex == iStartIndex)
//			break;
//
//		m_BestPath.push_front(m_pTerrain->GetVecTile()[iParentIndex]);
//		iParentIndex = m_pTerrain->GetVecTile()[iParentIndex]->iParentIndex;
//	}
//
//#ifdef _DEBUG
//	for (auto pTile : m_BestPath)
//		pTile->byDrawID = 36;
//#endif
//}
//
//bool CAstarMgr::IsInOpenlist(int iIndex)
//{
//	// find 함수: <algorithm>에서 제공.
//	// 현재 컨테이너를 순회하면서 세번째 인자로 전달 받은 값이 존재하는지 탐색하는 함수.
//	auto iter_find = find(m_OpenList.begin(), m_OpenList.end(), iIndex);
//
//	if(m_OpenList.end() == iter_find)
//		return false;
//
//	return true;
//}
//
//bool CAstarMgr::IsInCloselist(int iIndex)
//{
//	auto iter_find = find(m_CloseList.begin(), m_CloseList.end(), iIndex);
//
//	if (m_CloseList.end() == iter_find)
//		return false;
//
//	return true;
//}

void CAstarMgr::Free()
{
}
