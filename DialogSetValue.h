#pragma once
#include "resource.h"
#include "Parametre.h"


// Boîte de dialogue CDialogSetValue

class CDialogSetValue : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetValue)

public:
	CDialogSetValue(CWnd* pParent = NULL);   // constructeur standard
	CDialogSetValue(double value, CWnd* pParent = NULL);
	virtual ~CDialogSetValue();

// Données de boîte de dialogue
	enum { IDD = IDD_DIALOG_VALUE };
private:
	CNiNumEdit m_NiSetValueEdit1;
public:
	double m_Value;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	void ValueChangedSetValueEdit1(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
};
