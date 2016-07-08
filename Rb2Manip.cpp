// Rb2Manip.cpp : D�finit les comportements de classe pour l'application.
//

#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"

#include "Rb2Doc.h"
#include "Rb2View.h"
#include "SettingSave.h"
//#include "CBcamTopologyViewCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRb2ManipApp

BEGIN_MESSAGE_MAP(CRb2ManipApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Commandes de fichier standard
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// construction CRb2ManipApp

CRb2ManipApp::CRb2ManipApp()
: Rb2ManipWinSave()//"Windows position and size", "Rb2", false)
{
	m_pView = NULL;
	m_pView2 = NULL;
}


// Seul et unique objet CRb2ManipApp

CRb2ManipApp theApp;

/////////////////////////////////////////////////////////////////////////////
// initialisation CRb2ManipApp

BOOL CRb2ManipApp::InitInstance()
{
/*PourPictureBox*/// /////////// Juste pour utiliser GdiPlus 
/*PourPictureBox*/// GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL); //gdi+ ini

	// InitCommonControls() est requis sur Windows�XP si le manifeste de l'application
	// sp�cifie l'utilisation de ComCtl32.dll version�6 ou ult�rieure pour activer
	// les styles visuels.  Dans le cas contraire, la cr�ation de fen�tres �chouera.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialiser les biblioth�ques OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Initialisation standard
	// Si vous n'utilisez pas ces fonctionnalit�s et que vous souhaitez r�duire la taille
	// de votre ex�cutable final, vous devez supprimer ci-dessous
	// les routines d'initialisation sp�cifiques dont vous n'avez pas besoin.
	// Changez la cl� de Registre sous laquelle nos param�tres sont enregistr�s.
	// TODO : Modifiez cette cha�ne avec des informations appropri�es,
	// telles que le nom de votre soci�t� ou organisation
	SetRegistryKey(_T("Applications locales g�n�r�es par AppWizard"));
	LoadStdProfileSettings(13);  // Charge les options de fichier INI standard (y compris les derniers fichiers utilis�s)
	// Inscrire les mod�les de document de l'application. Ces mod�les
	//  lient les documents, fen�tres frame et vues entre eux
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRb2Doc),
		RUNTIME_CLASS(CMainFrame),       // fen�tre frame SDI principale
		RUNTIME_CLASS(CRb2View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// Activer les ouvertures d'ex�cution DDE
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	// Analyser la ligne de commande pour les commandes shell standard, DDE, ouverture de fichiers
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Distribue les commandes sp�cifi�es dans la ligne de commande. Retourne la valeur FALSE si
	// l'application a �t� d�marr�e avec /RegServer, /Register, /Unregserver ou /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// La seule fen�tre a �t� initialis�e et peut donc �tre affich�e et mise � jour

	CREATESTRUCT uneCs;
	CString NameDefault("Rb2WinSet.Rb2");
	int m_nCmdShow = Rb2ManipWinSave.SetWinSettings(uneCs, NameDefault);
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// appelle DragAcceptFiles uniquement s'il y a un suffixe
	//  Dans une application SDI, cet appel doit avoir lieu juste apr�s ProcessShellCommand
	// Activer les ouvertures via glisser-d�placer
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// bo�te de dialogue CAboutDlg utilis�e pour la bo�te de dialogue '� propos de' pour votre application

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

// Impl�mentation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Commande App pour ex�cuter la bo�te de dialogue
void CRb2ManipApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// gestionnaires de messages pour CRb2ManipApp

/////////////////////////// CF : Doc&Astuces  : "CView Access From Anywhere"
void CRb2ManipApp::StoreMyView(CRb2View *pView)
{
	if(m_pView == NULL)
		m_pView = pView;
	else if(m_pView2 == NULL)
		m_pView2 = pView;
}

CRb2View* CRb2ManipApp::GetMyView()
{
	return m_pView;
}
CRb2View* CRb2ManipApp::GetMyView2()
{
	return m_pView2;
}
/////////////////////////// End CF : Doc&Astuces  : "CView Access From Anywhere"

BOOL CRb2ManipApp::OnIdle(LONG lCount)
{
// 	m_pMainFrame->m_CamDialog.OnIdle(lCount);
// 	m_pMainFrame->m_TraitImDialog.OnIdle(lCount);
	m_pMainFrame->m_MainDialog.OnIdle(lCount);
//	m_pMainFrame->m_GeneticDialog.OnIdle(lCount);
	return CWinApp::OnIdle(lCount);
}

int CRb2ManipApp::ExitInstance()
{
/*PourPictureBox*/// /////////// Juste pour utiliser GdiPlus 
/*PourPictureBox*/// GdiplusShutdown(m_pGdiToken);

	return CWinApp::ExitInstance();
}
