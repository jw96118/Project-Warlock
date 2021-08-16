
#pragma once

#include "ViewTree.h"
#include "PropertiesWnd.h"
class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CClassView : public CDockablePane
{
public:
	CClassView() noexcept;
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void ClearGameObject();
	void ClearColObject();
protected:
	CClassToolBar m_wndToolBar;

	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillClassView();
public:
	CViewTree m_wndClassView;
private:
	Engine::CGameObject* preSelectObj = nullptr;
public:
	void UpdateList(Engine::OBJECTTYPE objTag, int testTag,CString str);
	void DeleteList(CString str);

	void UpdateColList(CString str);
	void DeleteColList(CString str);

private:
	HTREEITEM hRootTop;
	HTREEITEM m_hTerrainClass;
	HTREEITEM m_hWallClass;
	HTREEITEM m_hPlayerClass;
	HTREEITEM m_hMonsterClass;
	HTREEITEM m_hItemClass;
	HTREEITEM m_hMonster_Bat_Class;
	HTREEITEM m_hMonster_Spider_Class;
	HTREEITEM m_hMonster_Shield_Class;
	HTREEITEM m_hMonster_Ice_Class;
	HTREEITEM m_hMonster_Middle_Class;
	HTREEITEM m_hMonster_Boss_Class;
	HTREEITEM hColTop;
	HTREEITEM hCol;

// 재정의입니다.
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);
	afx_msg void OnDblClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

