// DialogCurve.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "DialogCurve.h"


// Boîte de dialogue CDialogCurve

IMPLEMENT_DYNAMIC(CDialogCurve, CDialog)
CDialogCurve::CDialogCurve(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCurve::IDD, pParent)
{
}

CDialogCurve::~CDialogCurve()
{
}

void CDialogCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURVE_GRAPH_01, m_NiGraph01);
}


BEGIN_MESSAGE_MAP(CDialogCurve, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogCurve
