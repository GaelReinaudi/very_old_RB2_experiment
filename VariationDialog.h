#pragma once

#include "ProDialog.h"
// Bo�te de dialogue CVariationDialog

class CVariationDialog : public CProDialog
{
	DECLARE_DYNAMIC(CVariationDialog)

public:
	CVariationDialog(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CVariationDialog();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_SEQ_VIEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
};
