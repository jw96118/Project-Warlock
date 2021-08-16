#include "stdafx.h"
#include "MainApp.h"

#include "Export_Function.h"
#include "Logo.h"
#include "Stage.h"
#include "Stage2.h"
#include "Convert.h"
#include "WeaponMgr.h"
#include "EffectMgr.h"
#include "DataMgr.h"

_bool g_bCursorFix = false;

CMainApp::CMainApp(void)
	: m_pDeviceClass(nullptr)
	, m_pManagement(nullptr)
	, m_pGraphicDev(nullptr)
	, m_iEvent(NO_EVENT)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(489);
}

CMainApp::~CMainApp(void)
{

}

HRESULT CMainApp::Ready_MainApp(void)
{
	Engine::Ready_SoundMgr("../../Sound/");

	FAILED_CHECK_RETURN(SetUp_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagement), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	//Engine::Stop_All();//TODO:�뷡��� ������!!! 
	m_iEvent = NO_EVENT;


	if (Engine::CKeyMgr::GetInstance()->KeyDown(Engine::KEY_Q))
		g_bCursorFix = !g_bCursorFix;

	if(g_bCursorFix)
		SetCursorPos(WINCX*0.5f,WINCY*0.5f);

	Engine::Set_InputDev();
	Engine::Get_KeyMgr()->Update();

	if (nullptr != m_pManagement)
		m_iEvent = m_pManagement->Update_Scene(fTimeDelta);



	return m_iEvent;
}

_int CMainApp::LateUpdate_MainApp(const _float & fTimeDelta)
{
	if (CHANGE_SCENE == m_iEvent)
		return m_iEvent;

	if (nullptr != m_pManagement)
		m_pManagement->LateUpdate_Scene(fTimeDelta);

	return m_iEvent;
}

void CMainApp::Render_MainApp(void)
{
	if (CHANGE_SCENE == m_iEvent)
		return;

	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);			// Z���ۿ� ������ �ϵ�, �ڵ� ������ ���� ���� ������ �Ǵ��ϴ� �ɼ�
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z���ۿ� �ȼ����� ���� ���� ���� ���� ������ �Ǵ��ϴ� �ɼ�

	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));
	
	if (nullptr != m_pManagement)
		m_pManagement->Render_Scene();

	Engine::Render_End();
}

HRESULT CMainApp::SetUp_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd,
		Engine::MODE_FULL,
		WINCX,
		WINCY,
		&m_pDeviceClass),
		E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_Device();
	(*ppGraphicDev)->AddRef();

	// FONT
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"�ü�", 20, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Second", L"����", 10, 10, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Third", L"����ǹ��� ����", 15, 35, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Mini", L"����ǹ��� ����", 20, 40, FW_THIN), E_FAIL);

	// INPUTDEV
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	// DebugMesh
	FAILED_CHECK_RETURN(Engine::Ready_DebugMesh(m_pGraphicDev), E_FAIL);

	// Manager
	FAILED_CHECK_RETURN(CWeaponMgr::GetInstance()->Ready_WeaponMgr(), E_FAIL);
	FAILED_CHECK_RETURN(CEffectMgr::GetInstance()->Ready_EffectMgr(m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(CDataMgr::GetInstance()->Ready_DataMgr(), E_FAIL);

	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9& pGraphicDev, 
								Engine::CManagement ** ppManagement)
{
	Engine::CScene*		pScene = nullptr;

	FAILED_CHECK_RETURN(Engine::Create_Management(ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN((*ppManagement)->SetUp_Scene(pScene), E_FAIL);

	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free(void)
{
	CDataMgr::GetInstance()->DestroyInstance();
	CWeaponMgr::GetInstance()->DestroyInstance();
	CEffectMgr::GetInstance()->DestroyInstance();

	Engine::Safe_Release(m_pGraphicDev);
	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagement);

	Engine::Release_Resources();
	Engine::Release_Utility();
	Engine::Release_System();
}

