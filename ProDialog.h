// 
/////////////////////////// CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
//
//////////////////////////////////////////////////////////////////////
//
// ProDialog.h: interface for the CProDialog class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////
//
// CProDialog window
//
////////////////////////////////////////////////////////////////////////////

class CProDialog : public CDialogBar
{
	DECLARE_DYNAMIC(CProDialog)

	// Construction / Destruction
	public:
	CProDialog();
	virtual ~CProDialog();

	// Attributes
	public:
	bool m_bWasPaintedOnce;

	// Operations
	public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

	// Implementation
	public:
	BOOL Create(CWnd * pParentWnd, UINT nIDTemplate, UINT nStyle, UINT
	nID);
	BOOL Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName, UINT
	nStyle, UINT nID);

	protected:
	virtual BOOL OnInitDialogBar();

	// Generated message map functions
	protected:
	//{{AFX_MSG(CProDialog)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDropFiles(HDROP hDropInfo);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
