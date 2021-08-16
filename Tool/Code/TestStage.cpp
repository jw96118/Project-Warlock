#include "pch.h"
#include "TestStage.h"
#include "TempObject.h"
#include "ColObject.h"
#include "MainFrm.h"
#include "Export_Function.h"

CTestStage::CTestStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CTestStage::~CTestStage(void)
{

}

HRESULT CTestStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	m_pKeyMgr = Engine::Get_KeyMgr();

	return S_OK;
}

HRESULT CTestStage::LateReady_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::LateReady_Scene(), E_FAIL);

	Engine::CCamera* pCamera = dynamic_cast<Engine::CCamera*>(Engine::Get_GameObject(L"GameLogic", L"DynamicCamera"));
	NULL_CHECK_RETURN(pCamera, E_FAIL);

	if (pCamera->Get_IsMainCamera())
		Engine::Get_Renderer()->Set_Perspective(*pCamera->Get_Projection());

	return S_OK;
}

_int CTestStage::Update_Scene(const _float& fTimeDelta)
{
	m_pKeyMgr->Update();
	Engine::CScene::Update_Scene(fTimeDelta);
	float u, v;
	float dist;
	return S_OK;
}

void CTestStage::Render_Scene(void)
{

}

CTestStage* CTestStage::Create(LPDIRECT3DDEVICE9 pGraphicDev, CToolView* m_pView)
{
	CTestStage*	pInstance = new CTestStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);
	pInstance->m_pView = m_pView;
	return pInstance;
}

void CTestStage::Free(void)
{
	Engine::CScene::Free();
}

HRESULT CTestStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pLayer, E_FAIL);

	m_pColLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(m_pColLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// Terrain
	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	// Player
	//pGameObject = CPlayer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	//
	//pGameObject = CToolCamera::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(L"ToolCamera", pGameObject), E_FAIL);


	pGameObject = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 5.f, -5.f),
		&_vec3(0.f, 0.f, 0.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f),
		_float(WINCX) / WINCY,
		1.f,
		1000.f,
		true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);




	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CTestStage::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	// 메모리 예약
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_LightTex",
		Engine::BUFFER_LIGHTTEX),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Tool_Buffer",
		Engine::BUFFER_TOOLTEX),
		E_FAIL);
	//	
	//FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
	//										RESOURCE_STATIC,
	//										L"Buffer_TerrainTex",
	//										Engine::BUFFER_TERRAINTEX,
	//										VTXCNTX,
	//										VTXCNTZ,
	//										VTXITV),
	//										E_FAIL);

	// DebugMesh
	FAILED_CHECK_RETURN(Engine::Ready_DebugMesh(m_pGraphicDev), E_FAIL);
	// 텍스쳐


	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Test",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Default.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_STAGE,
		L"Test_Monster",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/ToolMonster.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_STAGE,
		L"Test_Player",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/ToolPlayer.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/ProjectWarlock/Texture/Terrain/Terrain%d.png", 21),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Wall",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/ProjectWarlock/Texture/Wall/Wall%d.png", 19),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Crusader_Idle",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Monster/Crusader/Walk/Crusader_Walk%d.png", 1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Glacier_Move",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Monster/Glacier/Move/Glacier_Move%d.png", 1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Spider_Idle",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Monster/Spider/Idle/Spider_Idle%d.png", 1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Bat_Idle",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Monster/BatGray/Fly/batGreyFly000%d.png", 1),
		E_FAIL);



	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Newshambler_Idle",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/Texture/Monster/Newshambler/Idle/Newshambler_Idle%d.png", 1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Item_Hp",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Hp/TEXTURE_HEALTH_BIG_0000.png", 1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Item_Mana",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/Mana/city_mana_big0.png", 1),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Texture_Item_Mini",
		Engine::TEX_NORMAL,
		L"../../Client/Bin/Resource/ProjectWarlock/Texture/Item/MiniGame/upgrade_pickup0000.png", 1),
		E_FAIL);



	return S_OK;
}

HRESULT CTestStage::Add_GameObject(Engine::OBJECTTYPE objType, int monsterType, const _tchar * pObjName, const _tchar* pObjTypeName)
{
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CTempObject::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_ObjType(objType);
	pGameObject->Set_ObjName(pObjTypeName);
	if (objType == Engine::OBJ_MONSTER)
		pGameObject->Set_MonsterType((Engine::MONSTERTYPE)monsterType);
	else if (objType == Engine::OBJ_ITEM)
		pGameObject->Set_ItemType((Engine::ITEMTYPE)monsterType);
	dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(objType);
	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);
	m_mapLayer.emplace(L"GameObject", m_pLayer);
	return S_OK;
}



HRESULT CTestStage::Copy_GameObject(Engine::CGameObject * object, const _tchar * pObjName)
{
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CTempObject::Create(m_pGraphicDev, object);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(pGameObject->Get_ObjType());
	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);

	m_mapLayer.emplace(L"GameObject", m_pLayer);

	return S_OK;
}

HRESULT CTestStage::Copy_GameObject(Engine::CGameObject * object, const _tchar * pObjName, _vec3 tempPos)
{
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CTempObject::Create(m_pGraphicDev, object);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(pGameObject->Get_ObjType());
	pGameObject->GetTransformCom()->m_vInfo[Engine::INFO_POS] += tempPos;
	FAILED_CHECK_RETURN(m_pLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);

	m_mapLayer.emplace(L"GameObject", m_pLayer);

	return S_OK;
}



HRESULT CTestStage::Add_ColObject(const _tchar * pObjName)
{
	Engine::CGameObject*		pGameObject = nullptr;

	pGameObject = CColObject::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	FAILED_CHECK_RETURN(m_pColLayer->Add_GameObject(pObjName, pGameObject), E_FAIL);
	m_mapLayer.emplace(L"ColObject", m_pColLayer);
}

void CTestStage::Save(const TCHAR * pFilePath)
{
	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwBytes = 0;
	for (int i = 0; i < Engine::OBJ_END; i++)
	{
		WriteFile(hFile, &pToolView->m_DObj.m_iObjCount[i], sizeof(int), &dwBytes, nullptr);
		WriteFile(hFile, &pToolView->m_DObj.m_iObjNameCount[i], sizeof(int), &dwBytes, nullptr);
	}

	auto iter = m_mapLayer.begin();
	for (; iter != m_mapLayer.end(); iter++)
	{
		if (wcscmp(iter->first.c_str(), L"GameObject") == 0)
		{
			iter->second->Save_Object(hFile, dwBytes, pFilePath, true);
			break;
		}
	}
}

void CTestStage::Load(const TCHAR * pFilePath, int &count)
{
	bool flag = false;
	auto iter = m_mapLayer.begin();
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_wndClassView.ClearGameObject();
	for (; iter != m_mapLayer.end();)
	{

		if (wcscmp(iter->first.c_str(), L"GameObject") == 0)
		{
			Engine::Safe_Release(iter->second);
			Engine::Safe_Release(m_pLayer);
			iter = m_mapLayer.erase(iter);

			m_pLayer = Engine::CLayer::Create();
			flag = true;
			break;
		}
		else
		{
			iter++;
		}
	}
	if (!flag)
	{
		if (m_pLayer == nullptr)
		{
			m_pLayer = Engine::CLayer::Create();
		}
	}
	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwBytes = 0;
	int tempCount = 0;
	for (int i = 0; i < Engine::OBJ_END; i++)
	{
		ReadFile(hFile, &pToolView->m_DObj.m_iObjCount[i], sizeof(int), &dwBytes, nullptr);
		ReadFile(hFile, &pToolView->m_DObj.m_iObjNameCount[i], sizeof(int), &dwBytes, nullptr);
		tempCount += pToolView->m_DObj.m_iObjCount[i];
	}

	m_pLayer->Load_Object(hFile, dwBytes, pFilePath, count);

	auto iter_Layer = m_pLayer->GetMapObj().begin();
	for (int i = 0; i < tempCount; i++)
	{
		Engine::CGameObject*		pGameObject = nullptr;


		pGameObject = CTempObject::Create(m_pGraphicDev);
		NULL(pGameObject, E_FAIL);
		iter_Layer->second = pGameObject->Load_Component(dwBytes, hFile, true);
		pMain->m_wndClassView.UpdateList(iter_Layer->second->Get_ObjType(), iter_Layer->second->Get_MonsterType(), iter_Layer->first.c_str());
		dynamic_cast<CTempObject*>(pGameObject)->SetTestTexure(iter_Layer->second->Get_ObjType());
		iter_Layer++;

	}
	CloseHandle(hFile);
	m_mapLayer.emplace(L"GameObject", m_pLayer);
}

void CTestStage::Col_Save(const TCHAR * pFilePath)
{
	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_WRITE, 0, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwBytes = 0;

	auto iter = m_mapLayer.begin();
	for (; iter != m_mapLayer.end(); iter++)
	{
		if (wcscmp(iter->first.c_str(), L"ColObject") == 0)
		{
			iter->second->Save_Object(hFile, dwBytes, pFilePath, false);
			break;
		}
	}
}

void CTestStage::Col_Load(const TCHAR * pFilePath, int & count)
{
	bool flag = false;
	auto iter = m_mapLayer.begin();
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->m_wndClassView.ClearColObject();
	for (; iter != m_mapLayer.end();)
	{
		if (wcscmp(iter->first.c_str(), L"ColObject") == 0)
		{
			Engine::Safe_Release(iter->second);
			Engine::Safe_Release(m_pColLayer);
			iter = m_mapLayer.erase(iter);

			m_pColLayer = Engine::CLayer::Create();
			flag = true;
			break;
		}
		else
		{
			iter++;
		}
	}
	if (!flag)
	{
		if (m_pColLayer == nullptr)
		{
			m_pColLayer = Engine::CLayer::Create();
		}
	}
	CToolView* pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwBytes = 0;

	m_pColLayer->Load_Object(hFile, dwBytes, pFilePath, count);

	auto iter_Layer = m_pColLayer->GetMapObj().begin();
	for (int i = 0; i < m_pColLayer->GetMapObj().size(); i++)
	{
		Engine::CGameObject*		pGameObject = nullptr;

		pGameObject = CColObject::Create(m_pGraphicDev);
		NULL(pGameObject, E_FAIL);
		iter_Layer->second = pGameObject->Load_Component(dwBytes, hFile, false);
		pMain->m_wndClassView.UpdateColList(iter_Layer->first.c_str());
		iter_Layer++;

	}
	CloseHandle(hFile);
	m_mapLayer.emplace(L"ColObject", m_pColLayer);
}

HRESULT CTestStage::Delete_Object(const _tchar * pObjectName, int &count)
{
	Engine::CGameObject* tempObject = Get_GameObject(L"GameObject", pObjectName);

	auto iter = m_mapLayer.begin();
	for (; iter != m_mapLayer.end(); iter++)
	{
		if (wcscmp(iter->first.c_str(), L"GameObject") == 0)
		{
			auto iter_Layer = iter->second->GetMapObj().begin();
			for (; iter_Layer != iter->second->GetMapObj().end(); iter_Layer++)
			{
				if (iter_Layer->second == tempObject)
				{
					CMainFrame * pMain = (CMainFrame*)AfxGetMainWnd();
					pMain->m_wndClassView.DeleteList(iter_Layer->first.c_str());
					Engine::Safe_Release(iter_Layer->second);
					iter->second->GetMapObj().erase(iter_Layer->first);

					count--;
					return S_OK;
				}
			}
		}
	}


	return E_FAIL;
}

HRESULT CTestStage::Delete_Col_Object(const _tchar * pObjectName, int & count)
{
	Engine::CGameObject* tempObject = Get_GameObject(L"ColObject", pObjectName);

	auto iter = m_mapLayer.begin();
	for (; iter != m_mapLayer.end(); iter++)
	{
		if (wcscmp(iter->first.c_str(), L"ColObject") == 0)
		{
			auto iter_Layer = iter->second->GetMapObj().begin();
			for (; iter_Layer != iter->second->GetMapObj().end(); iter_Layer++)
			{
				if (iter_Layer->second == tempObject)
				{
					CMainFrame * pMain = (CMainFrame*)AfxGetMainWnd();
					pMain->m_wndClassView.DeleteList(iter_Layer->first.c_str());
					Engine::Safe_Release(iter_Layer->second);
					iter->second->GetMapObj().erase(iter_Layer->first);

					count--;
					return S_OK;
				}
			}
		}
	}

}
