#pragma once

#include "ProDialog.h"
// Boîte de dialogue CVariationDialog

class CVariationDialog : public CProDialog
{
	DECLARE_DYNAMIC(CVariationDialog)

public:
	CVariationDialog(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CVariationDialog();

// Données de boîte de dialogue
	enum { IDD = IDD_SEQ_VIEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
};
