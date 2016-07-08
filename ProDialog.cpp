// 
/////////////////////////// CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
//
////////////////////////////////////////////////////////////////////////////
//
// ProDialog.cpp: implementation of the CProDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProDialog.h"
#include ".\prodialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CProDialog, CDialogBar)

BEGIN_EVENTSINK_MAP(CProDialog, CDialogBar)
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CProDialog, CDialogBar)
//{{AFX_MSG_MAP(CProDialog)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
ON_WM_PAINT()
ON_WM_DROPFILES()
END_MESSAGE_MAP()

CProDialog::CProDialog()
:m_bWasPaintedOnce(false)
{
	// In derived classes set intial
	// state of control(s) here
}

CProDialog::~CProDialog()
{

}

BOOL CProDialog::Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName,
	UINT nStyle, UINT nID)
{
	// Let MFC Create the control
	if(!CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID))
		return FALSE;

	// Since there is no WM_INITDIALOG message we have to call
	// our own InitDialog function ourselves after m_hWnd is valid
	if(!OnInitDialogBar())
		return FALSE;

	return TRUE;
}

BOOL CProDialog::Create(CWnd * pParentWnd, UINT nIDTemplate,
	UINT nStyle, UINT nID)
{
	if(!Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, nID))
		return FALSE;

	// Since there is no WM_INITDIALOG message we have to call
	// our own InitDialog function ourselves after m_hWnd is valid
	if(!OnInitDialogBar())
		return FALSE;
	return TRUE;
}

BOOL CProDialog::OnInitDialogBar()
{
	// Support for the MFC DDX model 
	// If you do not want this do not call the base class
	// from derived classes
	UpdateData(FALSE);

	return TRUE;
}

void CProDialog::DoDataExchange(CDataExchange* pDX)
{
	//Derived Classes Overide this function
	ASSERT(pDX);

	CDialogBar::DoDataExchange(pDX);

	// In derived class call the DDX_??? functions to set/retrieve values
	// of your controls. See example derived class for how to do this.
}
BOOL CProDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd = GetFocus();
    if(wParam == IDOK)
    {
		reinterpret_cast<CDialog *>(this)->NextDlgCtrl();
		reinterpret_cast<CDialog *>(this)->PrevDlgCtrl();
		return FALSE;
	}
	return CDialogBar::OnCommand(wParam, lParam);
}

void CProDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO : ajoutez ici le code de votre gestionnaire de messages
	// N'appelez pas CDialogBar::OnPaint() pour la peinture des messages
}

void CProDialog::OnDropFiles(HDROP hDropInfo)
{
	// TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels

	CDialogBar::OnDropFiles(hDropInfo);
}




