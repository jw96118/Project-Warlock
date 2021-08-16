#pragma once
#include "Export_Utility.h"

// CObjectDialog 대화 상자

class CObjectDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectDialog)

public:
	CObjectDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CObjectDialog();
	virtual BOOL OnInitDialog();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OBJ };
#endif
public:
	int m_iObjNameCount[Engine::OBJ_END] = {};
	int m_iObjCount[Engine::OBJ_END] = {};
private:
	Engine::CScene*		    m_pScene = nullptr;
public:
	void Set_Scene(Engine::CScene* pScene);
private:
	void CreateObject(Engine::OBJECTTYPE objTag);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComCombo();
	afx_msg void OnBnClickedComOk();
	afx_msg void OnBnClickedComCancel();
	CComboBox m_ObjCom;
	CComboBox m_TypeCombo;
	afx_msg void OnCbnSelchangeObjCombo2();
};
