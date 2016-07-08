// AppliDialogXPSDlg.h : header file
//

#pragma once
#include "..\DialogTranslationNewPort.h"

// CAppliDialogXPSDlg dialog
class CAppliDialogXPSDlg : public CDialogTranslationNewPort
{
// Construction
public:
	CAppliDialogXPSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DIALOG_TRANSLATION };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
