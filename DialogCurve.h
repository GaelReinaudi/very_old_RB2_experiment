#pragma once


// Bo�te de dialogue CDialogCurve

class CDialogCurve : public CDialog
{
	DECLARE_DYNAMIC(CDialogCurve)

public:
	CDialogCurve(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogCurve();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_DIALOG_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	// le NiGraph de la fenetre
	CNiGraph m_NiGraph01;
};
