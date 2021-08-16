#include "stdafx.h"
#include "Logo.h"
#include "Stage.h"
#include "Stage2.h"
#include "Stage3.h"
#include "StageMini.h"
#include "Loadingbar.h"
#include "LoadingScene.h"
#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{

}

CLogo::~CLogo(void)
{

}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(m_pGraphicDev, RESOURCE_END), E_FAIL);

	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"01.GameLogic"), E_FAIL);

	//m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE, &m_fLoadingCount);
	//NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	Engine::Play_BGM(L"001 Jerry and Luke's Final Theme.wav");
	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	//dynamic_cast<CLoadingBar*>(m_mapLayer.begin()->second->Get_GameObject(L"Loading_Bar"))->Set_Load(m_fLoadingCount  * 0.01f);

	/*if (true == m_pLoading->Get_Finish())
	{*/
	if (m_eLogo == START_GAME)
	{
		if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
		{
			//Engine::CScene*		pScene = CStage::Create(m_pGraphicDev);
			//Engine::CScene*		pScene = CStage2::Create(m_pGraphicDev);
			Engine::CScene*		pScene = CLoadIngScene::Create(m_pGraphicDev);
			//Engine::CScene*		pScene = CStageMini::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);
			Engine::Stop_All();
			FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), E_FAIL);

			return CHANGE_SCENE;
		}
	}
	else
	{
		if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
		{
			PostQuitMessage(0);
		}
	}
	if (Engine::Get_KeyMgr()->KeyDown(Engine::KEY_DOWN) || Engine::Get_KeyMgr()->KeyDown(Engine::KEY_UP))
	{
		if (m_eLogo == START_GAME)
		{
			m_eLogo = EXIT_GAME;
			m_mapLayer.begin()->second->Get_GameObject(L"TitleMenuNew")->Set_Enable(false);
			m_mapLayer.begin()->second->Get_GameObject(L"TitleMenuExit")->Set_Enable(true);
		}
		else
		{
			m_eLogo = START_GAME;
			m_mapLayer.begin()->second->Get_GameObject(L"TitleMenuNew")->Set_Enable(true);
			m_mapLayer.begin()->second->Get_GameObject(L"TitleMenuExit")->Set_Enable(false);
		}
	}

	//}

	return iExit;
}

void CLogo::Render_Scene(void)
{
	// DEBUG 용
	//Engine::Render_Font(L"Font_Default", m_pLoading->Get_LoadString(), &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo*	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	//	Engine::Safe_Release(m_pLoading);
	Engine::CScene::Free();
}


HRESULT	CLogo::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CLogo::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	//// BackGround
	//pGameObject = CBackGround::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	//m_mapLayer.emplace(pLayerTag, pLayer);
	pGameObject = CTitle::Create(m_pGraphicDev, L"Texture_MenuBG", CTitle::LEFT, _vec3(0, 0, 0.4f), 2.15f, 0.01f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);


	pGameObject = CTitle::Create(m_pGraphicDev, L"Texture_MenuBG1", CTitle::LEFT, _vec3(0, WINCY*-0.3f, 0.3f), 1.f, 0.01f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround1", pGameObject), E_FAIL);


	pGameObject = CTitle::Create(m_pGraphicDev, L"Texture_MenuBG2", CTitle::LEFT, _vec3(-WINCX * 0.1, WINCY*-0.2f, 0.02f), 1.f, 0.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround2", pGameObject), E_FAIL);


	pGameObject = CTitle::Create(m_pGraphicDev, L"Texture_MenuBG3", CTitle::RIGHT, _vec3(-WINCX * 0.1, WINCY*-0.5f, 0.02f), 1.25f, 0.02f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround3", pGameObject), E_FAIL);


	//pGameObject = CTitle::Create(m_pGraphicDev, L"Texture_Menu", CTitle::RIGHT, _vec3(WINCX*0.01f, 0.f, 0.02f), 1.f, 0);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TitleMenu", pGameObject), E_FAIL);

	pGameObject = CTitle::Create(m_pGraphicDev, L"Texture_Menu_New", CTitle::RIGHT, _vec3(WINCX*0.01f, 0.f, 0.02f), 1.f, 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TitleMenuNew", pGameObject), E_FAIL);
	pGameObject = CTitle::Create(m_pGraphicDev, L"Texture_Menu_Exit", CTitle::RIGHT, _vec3(WINCX*0.01f, 0.f, 0.02f), 1.f, 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TitleMenuExit", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT	CLogo::Ready_UI_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CLogo::Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax)
{
	// 메모리 예약
	Engine::Reserve_ContainerSize(eMax);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TriCol",
		Engine::BUFFER_TRICOL),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcCol",
		Engine::BUFFER_RCCOL),
		E_FAIL);

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
		L"Buffer_ScrollTex",
		Engine::BUFFER_SCROLLTEX),
		E_FAIL);

	// 텍스쳐
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_MenuBG",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Title/MenuBG_00.png"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_MenuBG1",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Title/MenuBG_01.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_MenuBG2",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Title/MenuBG_02.png"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_MenuBG3",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Title/MenuBG_03.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_Menu",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Title/TitleMenu.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_Menu_Exit",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Title/TitleMenu_SelectExit.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(pGraphicDev,
		RESOURCE_LOGO,
		L"Texture_Menu_New",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Title/TitleMenu_SelectNew.png"),
		E_FAIL);

	return S_OK;
}
