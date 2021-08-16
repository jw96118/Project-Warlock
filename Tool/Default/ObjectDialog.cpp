// ObjectDialog.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "MainFrm.h"
#include "ObjectDialog.h"
#include "afxdialogex.h"
#include "ToolView.h"
#include "TestStage.h"
// CObjectDialog 대화 상자

IMPLEMENT_DYNAMIC(CObjectDialog, CDialogEx)

CObjectDialog::CObjectDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OBJ, pParent)
{

}

CObjectDialog::~CObjectDialog()
{
}

BOOL CObjectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString wstring;

	m_ObjCom.AddString(L"Terrain");
	m_ObjCom.AddString(L"Wall");
	m_ObjCom.AddString(L"Player");
	m_ObjCom.AddString(L"Monster");
	m_ObjCom.AddString(L"Item");
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CObjectDialog::Set_Scene(Engine::CScene * pScene)
{
	m_pScene = pScene;
}

void CObjectDialog::CreateObject(Engine::OBJECTTYPE objTag)
{
	CString str = L"";
	CString strMonster = L"";
	bool tempMonsterFlag = false;
	bool tempItemFlag = false;
	switch (objTag)
	{
	case Engine::OBJ_TERRAIN:
		str = L"Obj_Terrain";
		break;
	case Engine::OBJ_WALL:
		str = L"Obj_Wall";
		break;
	case Engine::OBJ_PLAYER:
		str = L"Obj_Player";
		break;
	case Engine::OBJ_MONSTER:
		str = L"Obj_Monster";
		break;
	case Engine::OBJ_ITEM:
		str = L"Obj_Item";
		break;
	case Engine::OBJ_END:
		break;

	}
	if (str == L"Obj_Monster")
	{
		switch (m_TypeCombo.GetCurSel())
		{
		case Engine::NO_MONSTER:
			tempMonsterFlag = true;
			break;
		case Engine::MONSTER_BAT:
			strMonster = L"_Bat";
			break;
		case Engine::MONSTER_SPIDER:
			strMonster = L"_Spider";
			break;
		case Engine::MONSTER_SHIELD:
			strMonster = L"_Shield";
			break;
		case Engine::MONSTER_ICE:
			strMonster = L"_Ice";
			break;
		case Engine::MONSTER_MIDDLEBOSS:
			strMonster = L"_Middle";
			break;
		case Engine::MONSTER_BOSS:
			strMonster = L"_Boss";
			break;
		default:
			tempMonsterFlag = true;
			break;
		}
	}
	else if (str == L"Obj_Item")
	{
		switch (m_TypeCombo.GetCurSel())
		{
		case Engine::NO_ITEM:
			tempItemFlag = true;
			break;
		case Engine::ITEM_HP:
			strMonster = L"_HP";
			break;
		case Engine::ITEM_MANA:
			strMonster = L"_MANA";
			break;
		case Engine::ITEM_MINIGAME:
			strMonster = L"_MINIGAME";
			break;
		default:
			tempItemFlag = true;
			break;
		}
	}


	CString strCount;
	strCount.Format(_T("%d"), m_iObjNameCount[objTag]);
	CString strName;
	str += strMonster;
	strName = str;
	if (objTag != Engine::OBJ_PLAYER)
		str += strCount;
	if (!(objTag == Engine::OBJ_MONSTER && tempMonsterFlag == true) || !(objTag == Engine::OBJ_ITEM && tempItemFlag == true))
	{
		if (!FAILED(dynamic_cast<CTestStage*>(m_pScene)->Add_GameObject(objTag,m_TypeCombo.GetCurSel(),str, strName)))
		{

			CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
			pMain->m_wndClassView.UpdateList(objTag, (Engine::MONSTERTYPE)m_TypeCombo.GetCurSel(),str);

			m_iObjCount[objTag]++;
			m_iObjNameCount[objTag]++;
			cout << "오브젝트 생성" << endl;
		}
	}
	else
	{
		AfxMessageBox(L"속성을 확인해주세요");
	}
}

void CObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_COMBO, m_ObjCom);
	DDX_Control(pDX, IDC_OBJ_COMBO2, m_TypeCombo);
}


BEGIN_MESSAGE_MAP(CObjectDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_OBJ_COMBO, &CObjectDialog::OnCbnSelchangeComCombo)
	ON_BN_CLICKED(ID_OBJ_OK, &CObjectDialog::OnBnClickedComOk)
	ON_BN_CLICKED(ID_OBJ_CANCEL, &CObjectDialog::OnBnClickedComCancel)
	ON_CBN_SELCHANGE(IDC_OBJ_COMBO2, &CObjectDialog::OnCbnSelchangeObjCombo2)
END_MESSAGE_MAP()


// CObjectDialog 메시지 처리기


void CObjectDialog::OnCbnSelchangeComCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch (m_ObjCom.GetCurSel())
	{
	case Engine::OBJ_TERRAIN:
	case Engine::OBJ_WALL:
	case Engine::OBJ_PLAYER:
		m_TypeCombo.ResetContent();
		break;
	case Engine::OBJ_MONSTER:
		m_TypeCombo.AddString(L"No");
		m_TypeCombo.AddString(L"Bat");
		m_TypeCombo.AddString(L"Spider");
		m_TypeCombo.AddString(L"Shield");
		m_TypeCombo.AddString(L"Ice");
		m_TypeCombo.AddString(L"Middle");
		m_TypeCombo.AddString(L"Boss");
		break;
	case Engine::OBJ_ITEM:
		m_TypeCombo.AddString(L"No");
		m_TypeCombo.AddString(L"HP");
		m_TypeCombo.AddString(L"Mana");
		m_TypeCombo.AddString(L"MiniGame");
		break;
	}
}


void CObjectDialog::OnBnClickedComOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CToolView* pToolView = nullptr;	
	Engine::CGameObject* Object = nullptr;
	switch (m_ObjCom.GetCurSel())
	{
	case Engine::OBJ_TERRAIN:

		break;
	case Engine::OBJ_WALL:
		break;
	case Engine::OBJ_PLAYER:
		 pToolView = (CToolView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();
		if (pToolView == nullptr)
		{
			return;
		}
		Object = pToolView->GetSceneObj(L"Obj_Player");
		if (Object == nullptr)
		{
			break;
		}
		else
		{
			AfxMessageBox(L"이미 있는 오브젝트입니다");
			return;
		}
		break;
	case Engine::OBJ_MONSTER:
		break;

	}
	CreateObject((Engine::OBJECTTYPE)m_ObjCom.GetCurSel());

}


void CObjectDialog::OnBnClickedComCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ObjCom.Clear();
	this->DestroyWindow();
}


void CObjectDialog::OnCbnSelchangeObjCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
