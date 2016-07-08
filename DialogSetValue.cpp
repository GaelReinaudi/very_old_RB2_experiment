// DialogSetValue.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include ".\dialogsetvalue.h"


// Boîte de dialogue CDialogSetValue

IMPLEMENT_DYNAMIC(CDialogSetValue, CDialog)
CDialogSetValue::CDialogSetValue(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetValue::IDD, pParent)
{
}

CDialogSetValue::CDialogSetValue(double value, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetValue::IDD, pParent)
{
	m_Value = value;
}

CDialogSetValue::~CDialogSetValue()
{
}

BOOL CDialogSetValue::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_NiSetValueEdit1.SetValue(m_Value);

	this->GetDlgItem(IDC_SETVALUE_EDIT_01)->SetFocus();
	reinterpret_cast<CDialog *>(this)->NextDlgCtrl();
	reinterpret_cast<CDialog *>(this)->PrevDlgCtrl();


	return FALSE;// return TRUE unless you set the focus to a control
}
void CDialogSetValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETVALUE_EDIT_01, m_NiSetValueEdit1);
}


BEGIN_MESSAGE_MAP(CDialogSetValue, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogSetValue

BEGIN_EVENTSINK_MAP(CDialogSetValue, CDialog)
	ON_EVENT(CDialogSetValue, IDC_SETVALUE_EDIT_01, 1, ValueChangedSetValueEdit1, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
END_EVENTSINK_MAP()

void CDialogSetValue::ValueChangedSetValueEdit1(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_Value = double( Value->dblVal );
}
