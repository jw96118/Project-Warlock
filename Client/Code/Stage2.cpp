#include "stdafx.h"
#include "Stage2.h"
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
#include "PlusWall.h"
#include "StageMini.h"
#include "DataMgr.h"
#include "EventDoor.h"
#include "EffectMgr.h"
#include "Shop.h"

CStage2::CStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
	, m_fTime(0.f)
	, m_dwRenderCnt(0)
{
	ZeroMemory(m_szFPS, sizeof(m_szFPS));
}

CStage2::~CStage2(void)
{

}

HRESULT CStage2::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"01.Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"02.GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"03.UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Weapon_Layer(L"04.Weapon"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Col_Layer(L"05.Col"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Effect_Layer(L"09.Effect"), E_FAIL);

	return S_OK;
}

HRESULT CStage2::LateReady_Scene(void)
{
	Engine::Play_BGM(L"019 Antarctic - Calm Before The Storm.wav");

	FAILED_CHECK_RETURN(Engine::CScene::LateReady_Scene(), E_FAIL);

	// MainCamera 세팅
	Engine::CCamera* pCamera = dynamic_cast<Engine::CCamera*>(Engine::Get_GameObject(L"01.Environment", L"MainCamera"));
	NULL_CHECK_RETURN(pCamera, E_FAIL);

	if (pCamera->Get_IsMainCamera())
		Engine::Get_Renderer()->Set_Perspective(*pCamera->Get_Projection());

	// 무기 리스트 세팅
	CWeapon* pWeapon = nullptr;

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Dagger"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Staff"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"CrossBow"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"CrossBow3"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"ColdStaff"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Revolver"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"Minigun"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	pWeapon = dynamic_cast<CWeapon*>(Engine::Get_GameObject(L"04.Weapon", L"RocketLauncher"));
	NULL_CHECK_RETURN(pWeapon, E_FAIL);
	CWeaponMgr::GetInstance()->Setting_Weapon(pWeapon);

	// 플레이어 데이터 복사
	Engine::CGameObject* pObject = Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player");

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObject);
	CDataMgr::GetInstance()->Copy_DataToPlayer(pPlayer);


	// Shop UI에 업그레이드 포인트 개수 전달
	CShop* pShop = dynamic_cast<CShop*>(Engine::Get_GameObject(L"02.GameLogic", L"Shop"));
	pShop->ToString_UpgradePoint(*pPlayer->Get_UpgradePoint());

	return S_OK;
}

_int CStage2::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;


	_int iEvent = Engine::CScene::Update_Scene(fTimeDelta);

	//if (CHANGE_SCENE == iEvent)
	//	return CHANGE_SCENE;
	if (m_bSceneChange)
	{
		Engine::CScene*		pScene = CStageMini::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, E_FAIL);
		Engine::Stop_All();
		CWeaponMgr::GetInstance()->Reset_UsingWeapon();
		CEffectMgr::GetInstance()->Reset_Effect();


		// 플레이어 데이터 매니저로 복사
		Engine::CGameObject* pObject = Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player");

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObject);
		CDataMgr::GetInstance()->Copy_DataToMgr(pPlayer);

		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), E_FAIL);

		return CHANGE_SCENE;
	}
	if (m_MonsterCount <= 0)
	{
		Open();
		m_bMiddleBossMove = true;
	}
	return iEvent;
}

_int CStage2::LateUpdate_Scene(const _float & fTimeDelta)
{
	Engine::CLayer* pLayerWall = Engine::Get_Layer(L"05.Col");
	Engine::CLayer* pLayerDoor = Engine::Get_Layer(L"05.DoorCol");
	Engine::CLayer* pMonsterWall = Engine::Get_Layer(L"06.Monster");
	Engine::CGameObject* pObject = Engine::Get_GameObject(L"02.GameLogic", L"Obj_Player");

	CollisionMgr::CollisionWallToPlayer(pLayerWall, pObject);

	CollisionMgr::CollisionWallToMonster(pLayerWall, pMonsterWall);

	CollisionMgr::CollisionDoorToPlayer(pLayerDoor, pObject);
	_int iEvent = Engine::CScene::LateUpdate_Scene(fTimeDelta);

	return NO_EVENT;
}

void CStage2::Render_Scene(void)
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
}

CStage2* CStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage2*   pInstance = new CStage2(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStage2::Free(void)
{
	Engine::CScene::Free();
}

HRESULT CStage2::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CLayer*      pBossLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pBossLayer, E_FAIL);




	Engine::CGameObject*      pGameObject = nullptr;

	Load_Map(pLayer, pLayerTag, L"06.Monster", L"06.Item", L"../../Save/Stage2Monster.dat", true);

	m_mapLayer.emplace(pLayerTag, pLayer);

	Load_Map(pBossLayer, pLayerTag, L"06.Monster", L"06.Item", L"../../Save/Stage2_Boss.dat", false);

	// Shop
	pGameObject = CShop::Create(m_pGraphicDev);
	pGameObject->Set_Enable(false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shop", pGameObject), E_FAIL);


	wstring temp = L"Portal";
	pGameObject = CItem::Create(m_pGraphicDev, this);
	dynamic_cast<CItem*>(pGameObject)->Set_Pos(_vec3(-26.f, 1.f, 0.f));
	dynamic_cast<CItem*>(pGameObject)->Set_Type(Engine::ITEM_PORTAL);
	dynamic_cast<CItem*>(pGameObject)->Set_Texture();
	pLayer->Add_GameObject(temp.c_str(), pGameObject);

	temp = L"Upgrade1";
	pGameObject = CItem::Create(m_pGraphicDev, this);
	dynamic_cast<CItem*>(pGameObject)->Set_Pos(_vec3(-8.f, 0.5f, 11.f));
	dynamic_cast<CItem*>(pGameObject)->Set_Type(Engine::ITEM_UPGRADE);
	dynamic_cast<CItem*>(pGameObject)->Set_Texture();
	pLayer->Add_GameObject(temp.c_str(), pGameObject);
	temp = L"Upgrade2";
	pGameObject = CItem::Create(m_pGraphicDev, this);
	dynamic_cast<CItem*>(pGameObject)->Set_Pos(_vec3(10.f, 0.5f, 7.f));
	dynamic_cast<CItem*>(pGameObject)->Set_Type(Engine::ITEM_UPGRADE);
	dynamic_cast<CItem*>(pGameObject)->Set_Texture();
	pLayer->Add_GameObject(temp.c_str(), pGameObject);
	temp = L"Upgrade3";
	pGameObject = CItem::Create(m_pGraphicDev, this);
	dynamic_cast<CItem*>(pGameObject)->Set_Pos(_vec3(10.f, 0.5f, 3.f));
	dynamic_cast<CItem*>(pGameObject)->Set_Type(Engine::ITEM_UPGRADE);
	dynamic_cast<CItem*>(pGameObject)->Set_Texture();
	pLayer->Add_GameObject(temp.c_str(), pGameObject);



	m_mapLayer.emplace(L"02.BossStage", pBossLayer);
	Engine::CLayer*      pDoorLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pDoorLayer, E_FAIL);
	temp = L"EventDoor1";
	pGameObject = CEventDoor::Create(m_pGraphicDev);
	dynamic_cast<CEventDoor*>(pGameObject)->Set_Transform(_vec3(2.f, 2.f, 1.f), _vec3(-13.5f, 1.f, 0.f), _vec3(0.f, -90.f, 0.f));
	dynamic_cast<CEventDoor*>(pGameObject)->Set_ColSize(_vec3(0.1f, 1.f, 1.f));
	pDoorLayer->Add_GameObject(temp.c_str(), pGameObject);

	m_mapLayer.emplace(L"05.DoorCol", pDoorLayer);

	return S_OK;
}

HRESULT   CStage2::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*      pGameObject = nullptr;

	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev,1);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

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

	return S_OK;
}

HRESULT CStage2::Ready_UI_Layer(const _tchar* pLayerTag)
{

#pragma region //UI
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	Engine::CGameObject*      pGameObject = nullptr;
	//UI
	float fUIScale = 2.5f;

	pGameObject = CHUD::Create(m_pGraphicDev, L"Texture_THUD", _vec3(-WINCX * 0.5f, WINCY*0.675f, 0.1f), fUIScale*2.07f, 3);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"THUD", pGameObject), E_FAIL);


	pGameObject = CGauge::Create(m_pGraphicDev, L"Texture_EXE", CGauge::EXE, _vec3(-WINCX * 0.4925f, WINCY*0.48f, 0.0f), fUIScale*2.07f, 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EXE_Bar", pGameObject), E_FAIL);

	pGameObject = CHUD::Create(m_pGraphicDev, L"Texture_LHUD", _vec3(WINCX*-0.5f, WINCY*0.48f, 0.1f), fUIScale, 3);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LHUD", pGameObject), E_FAIL);
	_vec2 mpPos = dynamic_cast<CHUD*>(pGameObject)->GetItem_Pos(CHUD::MP);
	_vec2 hpPos = dynamic_cast<CHUD*>(pGameObject)->GetItem_Pos(CHUD::HP);

	pGameObject = CGauge::Create(m_pGraphicDev, L"Texture_MP", CGauge::MP, _vec3(mpPos.x, mpPos.y, 0.f), fUIScale);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MP_Bar", pGameObject), E_FAIL);

	pGameObject = CGauge::Create(m_pGraphicDev, L"Texture_HP", CGauge::HP, _vec3(hpPos.x, hpPos.y, 0.f), fUIScale);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HP_Bar", pGameObject), E_FAIL);

	pGameObject = CHUD::Create(m_pGraphicDev, L"Texture_RHUD", _vec3(WINCX*0.5f, WINCY*0.48f, 0.1f), fUIScale, 2);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RHUD", pGameObject), E_FAIL);
	_vec2 ammoPos = dynamic_cast<CHUD*>(pGameObject)->GetItem_Pos(CHUD::AMMO);

	pGameObject = CGauge::Create(m_pGraphicDev, L"Texture_Ammo", CGauge::AMMO, _vec3(ammoPos.x, ammoPos.y, 0), fUIScale);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Ammo_Bar", pGameObject), E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);


#pragma endregion

}

HRESULT CStage2::Ready_Weapon_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*      pGameObject = nullptr;

	// Dagger
	pGameObject = CDagger::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dagger", pGameObject), E_FAIL);

	// Staff
	pGameObject = CStaff::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Staff", pGameObject), E_FAIL);

	// Revolver
	pGameObject = CRevolver::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Revolver", pGameObject), E_FAIL);

	// CrossBow
	pGameObject = CCrossBow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrossBow", pGameObject), E_FAIL);

	// CrossBow3
	pGameObject = CCrossBow3::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CrossBow3", pGameObject), E_FAIL);

	// ColdStaff
	pGameObject = CColdStaff::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ColdStaff", pGameObject), E_FAIL);

	// Minigun
	pGameObject = CMinigun::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Minigun", pGameObject), E_FAIL);

	// RocketLauncher
	pGameObject = CRocketLauncher::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RocketLauncher", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage2::Ready_Col_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Load_Col(pLayer, pLayerTag, L"../../Save/Stage2_Col.dat");

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage2::Ready_Effect_Layer(const _tchar * pLayerTag)
{
	Engine::CLayer*      pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

void CStage2::Load_Map(Engine::CLayer* pLayer, const _tchar* pLayerTag, const _tchar* pMonsterTag, const _tchar* pItemTag, const TCHAR * pFilePath, bool bRoofFlag)
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

	Engine::CLayer*           pRoofLayer = nullptr;
	Engine::CLayer*           pPlusWallLayer = nullptr;
	if (bRoofFlag)
		pRoofLayer = Engine::CLayer::Create();
	else
		pPlusWallLayer = Engine::CLayer::Create();
	Engine::CLayer*           pItemLayer = Engine::CLayer::Create();
	Engine::CLayer*           pMonsterLayer = Engine::CLayer::Create();
	wstring tempRoofName = L"";
	wstring tempUnder = L"_";
	_int    RoofCount = 0;
	_int    WallCount = 0;
	pLayer->Load_Object(hFile, dwBytes, pFilePath, count);

	auto iter_Layer = pLayer->GetMapObj().begin();
	for (int i = 0; i < count; i++)
	{
		wstring temp = iter_Layer->first;//TODO:  Monster 총알 때문에 수정!!!
		if (temp.find(L"Obj_Mon", 0) != wstring::npos)
		{
			wstring tempMonster = temp.substr(12, 3);
			if (tempMonster == L"Bat")
				pGameObject = CBat::Create(m_pGraphicDev, pLayerTag, temp,_vec3(0.f,0.f,0.f),&m_MonsterCount);
			if (tempMonster == L"Spi")
				pGameObject = CSpider::Create(m_pGraphicDev, pLayerTag, temp, _vec3(0.f, 0.f, 0.f),&m_MonsterCount);
			if (tempMonster == L"Shi")
				pGameObject = CCrusader::Create(m_pGraphicDev, pLayerTag, temp, _vec3(0.f, 0.f, 0.f),&m_MonsterCount);
			if (tempMonster == L"Ice")
				pGameObject = CGlacier::Create(m_pGraphicDev, pLayerTag, temp, _vec3(0.f, 0.f, 0.f),&m_MonsterCount);
			if (tempMonster == L"Mid")
			{
				pGameObject = CNewshambler::Create(m_pGraphicDev, pLayerTag, temp);
				m_MonsterCount--;
			}
			if (tempMonster == L"Bos")
			{
				//pGameObject = CBat::Create(m_pGraphicDev, pLayerTag, temp);
			}
			pMonsterLayer->Add_GameObject(iter_Layer->first.c_str(), pGameObject->Load_Component(dwBytes, hFile, true));
			m_MonsterCount++;
			iter_Layer = pLayer->GetMapObj().erase(iter_Layer);
		}
		else if (temp.find(L"Obj_Ite", 0) != wstring::npos)
		{
			pGameObject = CItem::Create(m_pGraphicDev,this);
			pGameObject->Load_Component(dwBytes, hFile, true);
			dynamic_cast<CItem*>(pGameObject)->Set_Texture();
			pItemLayer->Add_GameObject(iter_Layer->first.c_str(), pGameObject);
			iter_Layer = pLayer->GetMapObj().erase(iter_Layer);
		}
		else
		{
			if (temp.find(L"Obj_Pla", 0) != wstring::npos)
			{
				pGameObject = CPlayer::Create(m_pGraphicDev);
			}
			else if (temp.find(L"Obj_Wal", 0) != wstring::npos || temp.find(L"Obj_Ter", 0) != wstring::npos)
			{
				pGameObject = CTempObject::Create(m_pGraphicDev);

			}
			NULL(pGameObject, E_FAIL);
			iter_Layer->second = pGameObject->Load_Component(dwBytes, hFile, true);


			if (temp.find(L"Obj_Ter", 0) != wstring::npos)
			{
				if (pRoofLayer != nullptr)
				{
					tempRoofName = L"Obj_Roof";
					Engine::CGameObject*      pRoofObject = nullptr;
					pRoofObject = CRoof::Create(m_pGraphicDev);
					tempRoofName += RoofCount;

					dynamic_cast<CRoof*>(pRoofObject)->Set_Transform(pGameObject);
					pRoofLayer->Add_GameObject(tempRoofName.c_str(), pRoofObject);

					RoofCount++;
				}
			}
			if (temp.find(L"Obj_Wal", 0) != wstring::npos)
			{
				if (pPlusWallLayer != nullptr)
				{
					if (pGameObject->GetTransformCom()->m_vInfo[Engine::INFO_POS].y <= 1)
					{
						for (int j = 0; j < 3; j++)
						{
							tempRoofName = L"Obj_PlusWall";

							Engine::CGameObject*      pPlusWallObject = nullptr;
							pPlusWallObject = CPlusWall::Create(m_pGraphicDev);

							tempRoofName += RoofCount;

							dynamic_cast<CPlusWall*>(pPlusWallObject)->Set_Transform(pGameObject, (2.f + (j * 2)));
							pPlusWallLayer->Add_GameObject(tempRoofName.c_str(), pPlusWallObject);

							RoofCount++;
						}
					}
				}
			}

			iter_Layer++;
		}
	}
	CloseHandle(hFile);
	m_mapLayer.emplace(pMonsterTag, pMonsterLayer);
	if (bRoofFlag)
		m_mapLayer.emplace(L"07.Roof", pRoofLayer);
	else
		m_mapLayer.emplace(L"07.PlusWall", pPlusWallLayer);

	m_mapLayer.emplace(pItemTag, pItemLayer);

}

void CStage2::Load_Col(Engine::CLayer * pLayer, const _tchar * pLayerTag, const TCHAR * pFilePath)
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
void CStage2::Open()
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
