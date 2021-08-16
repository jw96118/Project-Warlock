#include "stdafx.h"
#include "StageMini.h"
#include "Dagger.h"
#include "Revolver.h"
#include "Staff.h"
#include "ColdStaff.h"
#include "CrossBow.h"
#include "CrossBow3.h"
#include "Minigun.h"
#include "RocketLauncher.h"
#include "WeaponMgr.h"
#include "Spider.h"
#include "ColObject.h"
#include "Export_Function.h"
#include "Crusader.h"
#include "Glacier.h"
#include "Newshambler.h"
#include "HellBoss.h"
#include "CollisionMgr.h"
#include "Roof.h"
#include "ITem.h"
#include "EventDoor.h"
#include "MiniMonster.h"
#include "MiniMap.h"
#include "MiniObject.h"
#include "MiniMonsterEvent.h"
#include "EffectMgr.h"
#include "Stage3.h"
CStageMini::CStageMini(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_fTime(0.f)
	, m_dwRenderCnt(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CStageMini::~CStageMini(void)
{

}

HRESULT CStageMini::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"01.Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"02.GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"03.UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Weapon_Layer(L"04.Weapon"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Col_Layer(L"05.Col"), E_FAIL);

	return S_OK;
}

HRESULT CStageMini::LateReady_Scene(void)
{
	Engine::Play_BGM(L"027 Antarctic - That Thing.wav");

	FAILED_CHECK_RETURN(Engine::CScene::LateReady_Scene(), E_FAIL);

	// MainCamera 세팅
	Engine::CCamera* pCamera = dynamic_cast<Engine::CCamera*>(Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
	NULL_CHECK_RETURN(pCamera, E_FAIL);

	if (pCamera->Get_IsMainCamera())
		Engine::Get_Renderer()->Set_Perspective(*pCamera->Get_Projection());

	//// 무기 리스트 세팅
	//CWeapon* pWeapon = nullptr;

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Dagger"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Staff"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"CrossBow"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"CrossBow3"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"ColdStaff"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Revolver"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Minigun"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	//pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"RocketLauncher"));
	//NULL_CHECK_RETURN(pWeapon, E_FAIL);
	//CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	return S_OK;
}

_int CStageMini::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;


	_int iEvent = Engine::CScene::Update_Scene(fTimeDelta);

	if (m_bSceneChange)
	{
		Engine::CScene*		pScene = CStage3::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, E_FAIL);
		Engine::Stop_All();
		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), E_FAIL);
		CWeaponMgr::GetInstance()->Reset_UsingWeapon();
		CEffectMgr::GetInstance()->Reset_Effect();
		return CHANGE_SCENE;
	}
	if (GetAsyncKeyState('J')&0x8000)//TODO: TEST 지우기
		Open();

	if (m_iItemCount <= 0)
	{
		Open();
	}

	return iEvent;
}

_int CStageMini::LateUpdate_Scene(const _float & fTimeDelta)
{
	Engine::CLayer* pLayerWall = Engine::Get_Layer(L"05.Col");
	Engine::CLayer* pLayerDoor = Engine::Get_Layer(L"05.DoorCol");
	Engine::CLayer* pMonsterWall = Engine::Get_Layer(L"06.Monster");
	Engine::CGameObject* pObject = Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player");

	CollisionMgr::CollisionWallToPlayer(pLayerWall, pObject);

	CollisionMgr::CollisionWallToMiniMonster(pLayerWall, pMonsterWall, fTimeDelta);

	CollisionMgr::CollisionDoorToPlayer(pLayerDoor, pObject);

	//CollisionMgr::CollisionWallToMiniMonster(pLayerDoor, pMonsterWall);
	//CollisionMgr::CollisionWallToMonster(pLayerDoor, pMonsterWall);

	_int iEvent = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return NO_EVENT;
}

void CStageMini::Render_Scene(void)
{
	POINT ptMousePos;
	GetCursorPos(&ptMousePos);
	ScreenToClient(g_hWnd, &ptMousePos);
	swprintf_s(m_szScrollPos, L"ScrollPos X=: %d  , Y= %d ", ptMousePos.x, ptMousePos.y);
	Engine::Render_Font(L"Font_Default", m_szScrollPos, &_vec2(10.f, 10.f), D3DXCOLOR(0.f, 1.f, 0.f, 1.f));

	++m_dwRenderCnt;

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}


	Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(10.f, 40.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	swprintf_s(m_szItem, L"남은 아이템 : %d  ", m_iItemCount);
	Engine::Render_Font(L"Font_Mini", m_szItem, &_vec2(WINCX*0.4f, 40.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));


}

void CStageMini::Set_ItemCount()
{
	m_iItemCount--;
}

CStageMini* CStageMini::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageMini*   pInstance = new CStageMini(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStageMini::Free(void)
{
	Engine::CScene::Free();
}

HRESULT CStageMini::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CLayer*      pDoorLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pDoorLayer, E_FAIL);

	Engine::CGameObject*      pGameObject = nullptr;

	Load_Map(pLayer, pLayerTag, L"../../Save/MiniStage.dat");

	wstring temp = L"Portal";
	pGameObject = CItem::Create(m_pGraphicDev, this);
	dynamic_cast<CItem*>(pGameObject)->Set_Pos(_vec3(20.f, 1.f, 20.f));
	dynamic_cast<CItem*>(pGameObject)->Set_Type(Engine::ITEM_PORTAL);
	dynamic_cast<CItem*>(pGameObject)->Set_Texture();
	pLayer->Add_GameObject(temp.c_str(), pGameObject);

	temp = L"EventDoor1";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(20.f, 1.f, 19.f), _vec3(0.f, 0.f, 0.f));
	dynamic_cast<CEventDoor*>(pGameObject)->Set_ColSize(_vec3(1.f, 1.f, 0.1f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);


	temp = L"EventDoor2";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(20.f, 1.f, 21.f), _vec3(0.f, 180.f, 0.f));
	dynamic_cast<CEventDoor*>(pGameObject)->Set_ColSize(_vec3(1.f, 1.f, 0.1f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);

	temp = L"EventDoor3";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(19.f, 1.f, 20.f), _vec3(0.f, 90.f, 0.f));
	dynamic_cast<CEventDoor*>(pGameObject)->Set_ColSize(_vec3(0.1f, 1.f, 1.f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);

	temp = L"EventDoor4";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(21.f, 1.f, 20.f), _vec3(0.f, -90.f, 0.f));
	dynamic_cast<CEventDoor*>(pGameObject)->Set_ColSize(_vec3(0.1f, 1.f, 1.f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);

	temp = L"EventDoor5";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(20.f, 1.f, 19.1f), _vec3(0.f, 180.f, 0.f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);


	temp = L"EventDoor6";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(20.f, 1.f, 20.9f), _vec3(0.f, 0.f, 0.f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);

	temp = L"EventDoor7";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(19.1f, 1.f, 20.f), _vec3(0.f, -90.f, 0.f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);

	temp = L"EventDoor8";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(20.9f, 1.f, 20.f), _vec3(0.f, 90.f, 0.f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);




	m_mapLayer.emplace(L"05.DoorCol", pDoorLayer);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT   CStageMini::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*      pGameObject = nullptr;



	// FirstPersonCamera (Player Camera)
	pGameObject = CFirstPersonCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 1.2f, -5.f),
		&_vec3(0.f, 1.2f, 0.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(45.f),
		_float(WINCX) / WINCY,
		0.3f,
		1000.f,
		true);

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainCamera", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);


	m_pMiniMapLayer = Engine::CLayer::Create();
	//UI
	float fUIScale = 2.5f;

	pGameObject = CMiniMap::Create(m_pGraphicDev, L"Texture_MiniMapBack", _vec3(WINCX*0.4f, WINCY*0.3f, 0.1f), 3.f, 5);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pMiniMapLayer->Add_GameObject(L"MiniBack", pGameObject), E_FAIL);



	return S_OK;
}

HRESULT CStageMini::Ready_UI_Layer(const _tchar* pLayerTag)
{

#pragma region //UI
	//Engine::CLayer*      pMiniMapLayer = Engine::CLayer::Create();
	//Engine::CGameObject*      pGameObject = nullptr;
	////UI
	//float fUIScale = 2.5f;

	//pGameObject = CMiniMap::Create(m_pGraphicDev, L"Texture_MiniMapBack", _vec3(-WINCX * 0.5f, WINCY*0.675f, 0.1f), fUIScale*2.07f, 3);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pMiniMapLayer->Add_GameObject(L"THUD", pGameObject), E_FAIL);
	Engine::CGameObject*      pGameObject = nullptr;
	pGameObject = CMiniMonsterEvent::Create(m_pGraphicDev, L"Texture_MiniMonster", _vec3(0.f, -WINCY * 1.4f, 0.08f), 20.f, 5);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pMiniMapLayer->Add_GameObject(L"Monster_Event", pGameObject), E_FAIL);


	m_mapLayer.emplace(pLayerTag, m_pMiniMapLayer);

	return S_OK;
#pragma endregion

}

HRESULT CStageMini::Ready_Weapon_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*      pGameObject = nullptr;

	//// Dagger
	//pGameObject = CDagger::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dagger", pGameObject), E_FAIL);

	//// Staff
	//pGameObject = CStaff::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Staff", pGameObject), E_FAIL);

	//// Revolver
	//pGameObject = CRevolver::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Revolver", pGameObject), E_FAIL);

	//// CrossBow
	//pGameObject = CCrossBow::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrossBow", pGameObject), E_FAIL);

	//// CrossBow3
	//pGameObject = CCrossBow3::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrossBow3", pGameObject), E_FAIL);

	//// ColdStaff
	//pGameObject = CColdStaff::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColdStaff", pGameObject), E_FAIL);

	//// Minigun
	//pGameObject = CMinigun::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minigun", pGameObject), E_FAIL);

	//// RocketLauncher
	//pGameObject = CRocketLauncher::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RocketLauncher", pGameObject), E_FAIL);

	//m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStageMini::Ready_Col_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CLayer*      pMonsterLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pMonsterLayer, E_FAIL);

	Load_Col(pLayer, pLayerTag, L"../../Save/MiniStage_Col.dat");

	m_mapLayer.emplace(pLayerTag, pLayer);


	wstring temp = L"Monster0";
	Engine::CGameObject* pGameObject = CMiniMonster::Create(m_pGraphicDev);
	dynamic_cast<CMiniMonster*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 2.f), _vec3(20.f, 1.f, 14.f), _vec3(0.f, 0.f, 0.f));

	pMonsterLayer->Add_GameObject(temp.c_str(), pGameObject);
	m_mapLayer.emplace(L"06.Monster", pMonsterLayer);


	return S_OK;
}

void CStageMini::Load_Map(Engine::CLayer* pLayer, const _tchar* pLayerTag, const TCHAR * pFilePath)
{
	int count = 0;

	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwBytes = 0;
	int tempInt[Engine::OBJ_END] = {};
	int tempInt2[Engine::OBJ_END] = {};

	for (int i = 0; i < Engine::OBJ_END; i++)
	{
		ReadFile(hFile, &tempInt[i], sizeof(int), &dwBytes, nullptr);
		ReadFile(hFile, &tempInt2[i], sizeof(int), &dwBytes, nullptr);
	}

	Engine::CGameObject*      pGameObject = nullptr;

	Engine::CLayer*           pRoofLayer = Engine::CLayer::Create();
	Engine::CLayer*           pItemLayer = Engine::CLayer::Create();
	Engine::CLayer*           pMonsterLayer = Engine::CLayer::Create();
	wstring tempRoofName = L"";
	wstring tempMiniObj = L"";
	_int    RoofCount = 0;
	_int    MiniCount = 0;
	pLayer->Load_Object(hFile, dwBytes, pFilePath, count);

	auto iter_Layer = pLayer->GetMapObj().begin();
	for (int i = 0; i < count; i++)
	{
		wstring temp = iter_Layer->first;//TODO:  Monster 총알 때문에 수정!!!
		if (temp.find(L"Obj_Mon", 0) != wstring::npos)
		{
			wstring tempMonster = temp.substr(12, 3);
			if (tempMonster == L"Bat")
				pGameObject = CBat::Create(m_pGraphicDev, pLayerTag, temp);
			if (tempMonster == L"Spi")
				pGameObject = CSpider::Create(m_pGraphicDev, pLayerTag, temp);
			if (tempMonster == L"Shi")
				pGameObject = CCrusader::Create(m_pGraphicDev, pLayerTag, temp);
			if (tempMonster == L"Ice")
				pGameObject = CGlacier::Create(m_pGraphicDev, pLayerTag, temp);
			if (tempMonster == L"Mid")
				pGameObject = CNewshambler::Create(m_pGraphicDev, pLayerTag, temp);
			if (tempMonster == L"Bos")
			{
				//pGameObject = CBat::Create(m_pGraphicDev, pLayerTag, temp);
			}
			pMonsterLayer->Add_GameObject(iter_Layer->first.c_str(), pGameObject->Load_Component(dwBytes, hFile, true));
			iter_Layer = pLayer->GetMapObj().erase(iter_Layer);
		}
		else if (temp.find(L"Obj_Ite", 0) != wstring::npos)
		{
			pGameObject = CItem::Create(m_pGraphicDev, this);
			pGameObject->Load_Component(dwBytes, hFile, true);
			dynamic_cast<CItem*>(pGameObject)->Set_Texture();
			pItemLayer->Add_GameObject(iter_Layer->first.c_str(), pGameObject);
			iter_Layer = pLayer->GetMapObj().erase(iter_Layer);
			m_iItemCount++;

			tempMiniObj = L"MiniObj";
			tempMiniObj += MiniCount;

			Engine::CGameObject* ptempObject = CMiniObject::Create(m_pGraphicDev, L"Texture_MiniItem", pGameObject->GetTransformCom(), _vec3(WINCX * 0.33f, WINCY* 0.18f, 0.09f), (pGameObject->Get_EnableP()));

			FAILED(m_pMiniMapLayer->Add_GameObject(tempMiniObj.c_str(), ptempObject), E_FAIL);
			MiniCount++;

		}
		else
		{
			if (temp.find(L"Obj_Pla", 0) != wstring::npos)
			{
				pGameObject = CPlayer::Create(m_pGraphicDev, true);
			}
			else if (temp.find(L"Obj_Wal", 0) != wstring::npos || temp.find(L"Obj_Ter", 0) != wstring::npos)
			{
				pGameObject = CTempObject::Create(m_pGraphicDev);

			}
			NULL(pGameObject, E_FAIL);
			iter_Layer->second = pGameObject->Load_Component(dwBytes, hFile, true);

			if (temp.find(L"Obj_Ter", 0) != wstring::npos)
			{
				tempRoofName = L"Obj_Roof";
				Engine::CGameObject*      pRoofObject = nullptr;
				pRoofObject = CRoof::Create(m_pGraphicDev);
				tempRoofName += RoofCount;

				dynamic_cast<CRoof*>(pRoofObject)->Set_Transform(pGameObject);
				pRoofLayer->Add_GameObject(tempRoofName.c_str(), pRoofObject);

				RoofCount++;
			}
			if (temp.find(L"Obj_Wal", 0) != wstring::npos)
			{
				tempMiniObj = L"MiniObj";
				tempMiniObj += MiniCount;
				Engine::CGameObject* ptempObject = CMiniObject::Create(m_pGraphicDev, L"Texture_MiniWall", pGameObject->GetTransformCom(), _vec3(WINCX * 0.33f, WINCY* 0.18f, 0.09f));

				FAILED(m_pMiniMapLayer->Add_GameObject(tempMiniObj.c_str(), ptempObject), E_FAIL);
				MiniCount++;
			}
			if (temp.find(L"Obj_Pla", 0) != wstring::npos)
			{
				tempMiniObj = L"MiniObj";
				tempMiniObj += MiniCount;
				Engine::CGameObject* ptempObject = CMiniObject::Create(m_pGraphicDev, L"Texture_MiniPlayer", pGameObject->GetTransformCom(), _vec3(WINCX * 0.33f, WINCY* 0.18f, 0.09f));

				FAILED(m_pMiniMapLayer->Add_GameObject(tempMiniObj.c_str(), ptempObject), E_FAIL);
				MiniCount++;
			}

			iter_Layer++;
		}
	}
	CloseHandle(hFile);
	m_mapLayer.emplace(L"06.Monster2", pMonsterLayer);
	m_mapLayer.emplace(L"07.Roof", pRoofLayer);

	m_mapLayer.emplace(L"08.Item", pItemLayer);

}

void CStageMini::Load_Col(Engine::CLayer * pLayer, const _tchar * pLayerTag, const TCHAR * pFilePath)
{
	int count = 0;

	HANDLE hFile = ::CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;
	DWORD dwBytes = 0;

	Engine::CGameObject*      pGameObject = nullptr;

	pLayer->Load_Object(hFile, dwBytes, pFilePath, count);

	auto iter_Layer = pLayer->GetMapObj().begin();
	for (int i = 0; i < count; i++)
	{

		pGameObject = CColObject::Create(m_pGraphicDev);

		NULL(pGameObject, E_FAIL);
		iter_Layer->second = pGameObject->Load_Component(dwBytes, hFile, false);

		iter_Layer++;
	}
	CloseHandle(hFile);

}

void CStageMini::Open()
{
	auto iter = m_mapLayer.begin();
	for (; iter != m_mapLayer.end(); iter++)
	{
		if (iter->first == L"05.DoorCol")
		{
			auto iter_Layer = iter->second->GetMapObj().begin();
			for (; iter_Layer != iter->second->GetMapObj().end(); iter_Layer++)
			{
				dynamic_cast<CEventDoor*>(iter_Layer->second)->Set_Up(true);
			}
		}
	}
}
