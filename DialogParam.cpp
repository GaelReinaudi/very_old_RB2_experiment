// DialogParam.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "Parametre.h"
#include ".\dialogparam.h"


// Boîte de dialogue CDialogParam

IMPLEMENT_DYNAMIC(CDialogParam, CDialog)
CDialogParam::CDialogParam(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogParam::IDD, pParent)
{
	m_bWasPaintedOnce = false;
}

CDialogParam::CDialogParam(int UnInt)
	: CDialog(CDialogParam::IDD, NULL)
{
	m_bWasPaintedOnce = false;
}

CDialogParam::~CDialogParam()
{
}

void CDialogParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARAM_COMBO_PARAM, m_ComboParam);
	DDX_Control(pDX, IDC_PARAM_LIST_PARAM, m_ListParam);
}

BEGIN_MESSAGE_MAP(CDialogParam, CDialog)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_PARAM_LIST_PARAM, OnLbnSelchangeParamListParam)
END_MESSAGE_MAP()

void CDialogParam::OnPaint()
{
	if(!m_bWasPaintedOnce)
	{
		m_bWasPaintedOnce = true;
		FOR_ALL_PARAMETERS_DEFINED_do
			m_ListParam.AddString(pLeParam->m_NomTexte);
		}
	}
}

void CDialogParam::OnLbnSelchangeParamListParam()
{
	
}
