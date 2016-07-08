#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Boîte de dialogue CDialogParam

class CDialogParam : public CDialog
{
	DECLARE_DYNAMIC(CDialogParam)

public:
	CDialogParam(CWnd* pParent = NULL);   // constructeur standard
	CDialogParam(int UnInt);
	virtual ~CDialogParam();

// Données de boîte de dialogue
	enum { IDD = IDD_DIALOG_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	bool m_bWasPaintedOnce;
	CComboBoxEx m_ComboParam;
public:
	CListBox m_ListParam;
	afx_msg void OnPaint();
	afx_msg void OnLbnSelchangeParamListParam();
};
