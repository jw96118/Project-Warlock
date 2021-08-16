#include "ResourcesMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CResourcesMgr)


Engine::CResourcesMgr::CResourcesMgr(void)
{

}

Engine::CResourcesMgr::~CResourcesMgr(void)
{
	Free();
}

HRESULT Engine::CResourcesMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if (nullptr != m_pMapResources)
		return E_FAIL;

	m_pMapResources = new map<wstring, CResources*>[wSize];

	m_wSize = wSize;

	return S_OK;
}

HRESULT Engine::CResourcesMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, 
											const _ushort& wContainerIdx,
											const _tchar* pBufferTag,
											BUFFERID eID,
											const _ulong& dwCntX,
											const _ulong& dwCntZ,
											const _ulong& dwVtxItv)
{	
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources*	pResources = Find_Resources(wContainerIdx, pBufferTag);

	if (nullptr != pResources)
		return E_FAIL;


	switch (eID)
	{
	case BUFFER_TRICOL:
		pResources = CTriCol::Create(pGraphicDev);
		break;

	case BUFFER_RCCOL:
		pResources = CRcCol::Create(pGraphicDev);
		break;

	case BUFFER_RCTEX:
		pResources = CRcTex::Create(pGraphicDev);
		break;

	case BUFFER_TERRAINTEX:
		pResources = CTerrainTex::Create(pGraphicDev, dwCntX, dwCntZ, dwVtxItv);
		break;

	case BUFFER_CUBETEX:
		pResources = CCubeTex::Create(pGraphicDev);
		break;
	case BUFFER_SCROLLTEX:
		pResources = CScrollTex::Create(pGraphicDev);
		break;
	case BUFFER_TOOLTEX:
		pResources = CToolTex::Create(pGraphicDev);	
		break;
	case BUFFER_LIGHTTEX:
		pResources = CLightTex::Create(pGraphicDev);
		break;
	}
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(pBufferTag, pResources);

	return S_OK;
}

void Engine::CResourcesMgr::Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResources*	pResources = Find_Resources(wContainerIdx, pBufferTag);
	NULL_CHECK(pResources);

	dynamic_cast<CVIBuffer*>(pResources)->Render_Buffer();
}

Engine::CResources* Engine::CResourcesMgr::Find_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	//auto	iter = find_if(m_pMapResources[wContainerIdx].begin(),
	//	m_pMapResources[wContainerIdx].end(), CTag_Finder(pResourceTag));

	//if (iter == m_pMapResources[wContainerIdx].end())
	//	return nullptr;

	//return iter->second;
	auto iter = m_pMapResources[wContainerIdx].begin();
	for (; iter != m_pMapResources[wContainerIdx].end(); iter++)
	{
		if (iter->first == pResourceTag)
			return  iter->second;
	}
	return nullptr;
}

void Engine::CResourcesMgr::Free(void)
{
	for (_ushort i = 0; i < m_wSize; ++i)
	{
		for_each(m_pMapResources[i].begin(), m_pMapResources[i].end(), CDeleteMap());
		m_pMapResources[i].clear();
	}

	Safe_Delete_Array(m_pMapResources);
}

Engine::CComponent* Engine::CResourcesMgr::Clone(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	//auto	iter = find_if(m_pMapResources[wContainerIdx].begin(),
	//	m_pMapResources[wContainerIdx].end(), CTag_Finder(pResourceTag));

	//if (iter == m_pMapResources[wContainerIdx].end())
	//	return nullptr;

	//return iter->second->Clone();
	auto iter = m_pMapResources[wContainerIdx].begin();
	for (; iter != m_pMapResources[wContainerIdx].end(); iter++)
	{
		if (iter->first == pResourceTag)
			return  iter->second->Clone();
	}
	return nullptr;

}

CComponent * CResourcesMgr::Clone(CComponent * Com)
{
	return Com->Clone();
}

void CResourcesMgr::SendToClient(const _ushort & wContainerIdx, const _tchar* pBufferTag, void * pVertex)
{
	//auto	iter = find_if(m_pMapResources[wContainerIdx].begin(),
	//	m_pMapResources[wContainerIdx].end(), CTag_Finder(pBufferTag));

	//if (iter == m_pMapResources[wContainerIdx].end())
	//	return;

	//dynamic_cast<CVIBuffer*>(iter->second)->SendToClient(pVertex);


	auto iter = m_pMapResources[wContainerIdx].begin();
	for (; iter != m_pMapResources[wContainerIdx].end(); iter++)
	{
		if (iter->first == pBufferTag)
			dynamic_cast<CVIBuffer*>(iter->second)->SendToClient(pVertex);
	}
	return;


}

void CResourcesMgr::ReceiveToEngine(const _ushort & wContainerIdx, const _tchar* pBufferTag, void * pVertex)
{
	//auto	iter = find_if(m_pMapResources[wContainerIdx].begin(),
	//	m_pMapResources[wContainerIdx].end(), CTag_Finder(pBufferTag));

	//if (iter == m_pMapResources[wContainerIdx].end())
	//	return;

	//dynamic_cast<CVIBuffer*>(iter->second)->ReceiveToEngine(pVertex);
	auto iter = m_pMapResources[wContainerIdx].begin();
	for (; iter != m_pMapResources[wContainerIdx].end(); iter++)
	{
		if (iter->first == pBufferTag)
			dynamic_cast<CVIBuffer*>(iter->second)->ReceiveToEngine(pVertex);
	}
	return;

}

HRESULT Engine::CResourcesMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt /*= 1*/)
{
	NULL_CHECK_RETURN(m_pMapResources, E_FAIL);

	CResources*	pResources = Find_Resources(wContainerIdx, pTextureTag);

	if (nullptr != pResources)
		return E_FAIL;

	pResources = CTexture::Create(pGraphicDev, pPath, eType, iCnt);
	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_pMapResources[wContainerIdx].emplace(pTextureTag, pResources);

	return S_OK;
}

