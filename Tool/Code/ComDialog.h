#pragma once
#include <afxwin.h>
class CComDialog :
	public CDialog
{
	DECLARE_DYNAMIC(CComDialog)

public:
	CComDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CComDialog();

	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_DIALOG_COM
};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
public:
	const int& GetTile() const;
	const float& GetAngle() const;
	const float& GetChangeX() const;
	const float& GetChangeY() const;
	const RECT* GetCol() const;
	afx_msg void OnLbnSelchangeTile();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
private:
	void TileRender();
public: // Control Variables



};

