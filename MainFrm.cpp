// MainFrm.cpp : implémentation de la classe CMainFrame
//
#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include "Sequence.h"
#include "MainDialog.h"
#include "Parametre.h"
#include "PrntScreen.h"
#include ".\mainfrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
CMainFrame* m_pMainFrame;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)

	ON_COMMAND(ID_BUTTON_GRAPHIC, OnSwitchView)
	ON_COMMAND(ID_BUTTON_CAM, OnCamMode)
	ON_COMMAND(ID_BUTTON_TRAITIMAGE,OnTraitImageMode)
	ON_COMMAND(ID_BUTTON_INPUT, OnInputMode)
	ON_COMMAND(ID_BUTTON_LASER, OnLaserMode)
	ON_COMMAND(ID_BUTTON_SEQ_VIEW, OnSeqView)
	ON_COMMAND(ID_BUTTON_GENETIC, OnGeneticView)
	ON_COMMAND(ID_BUTTON_MAIN_PORT, OnMainPort)
	ON_COMMAND(ID_BUTTON_PORT, OnSeqView)

	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MAIN_BUTTON_PORT, OnBnClickedButtonSwitchModeManip)
	ON_BN_CLICKED(IDC_PORT_BUTTON_SEQ , OnBnClickedButtonSwitchModeManip)
	ON_WM_CLOSE()
	ON_COMMAND(ID_EXITWITHOUTSAVING, OnExitwithoutsaving)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSaveRb2)
	ON_COMMAND(ID_FILE_SAVE_AS_RB2, OnFileSaveAsRb2)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_SEQUENCE_FULLSEQUENCE, OnSequenceFullsequence)
	ON_COMMAND(ID_SEQUENCE_MOLASSES, OnSequenceMolasses)
	ON_COMMAND(ID_STATICPORT_REFERENCEMOT, OnStaticportReferencemot)
	ON_COMMAND(ID_STATICPORT_REFERENCEMOLASSES, OnStaticportReferencemolasses)
	ON_BN_CLICKED(IDC_MAIN_BUTTON_PRINT_MAIN_DIALOG, OnPrintMainDialog)
	ON_BN_CLICKED(IDC_MAIN_BUTTON_PRINT_PORT_DIALOG, OnPrintPortDialog)
	ON_BN_CLICKED(IDC_MAIN_BUTTON_PRINT_BOTH_DIALOG, OnPrintBothDialog)
	ON_BN_CLICKED(IDC_MAIN_BUTTON_PRINT_SEQUENCE, OnPrintSequence)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // indicateur de la ligne d'état
	ID_INDICATOR_01,
	ID_INDICATOR_02,
	ID_INDICATOR_03,
	ID_INDICATOR_04,
	ID_INDICATOR_05,
	ID_INDICATOR_06,
	ID_INDICATOR_07,           
	ID_INDICATOR_08,
	ID_INDICATOR_09,
	ID_INDICATOR_10
};

// construction ou destruction de CMainFrame

CMainFrame::CMainFrame()
: m_Affichage(MAIN), m_AffichagePrevious(MAIN), m_AffichagePreviousPrevious(MAIN)
, m_Save("Options Générales de Rb2Manip", "Rb2")
, m_splitterPosition(400)
{
	m_CarteDAQmxSemblePouvoirMarcher = false;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_pMainFrame = this;

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Impossible de créer toolbar\n");
		return -1;      // échec de la création
	}
//    ShowControlBar(&m_wndToolBar, 0 ,FALSE);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Impossible de créer la barre d'état\n");
		return -1;      // échec de la création
	}
	m_wndStatusBar.SetPaneInfo(0 , ID_INDICATOR_01, SBPS_STRETCH,100);
	m_wndStatusBar.SetPaneInfo(1 , ID_INDICATOR_01, SBPS_NORMAL	,50);
	m_wndStatusBar.SetPaneInfo(2 , ID_INDICATOR_02, SBPS_NORMAL	,50);
	m_wndStatusBar.SetPaneInfo(3 , ID_INDICATOR_03, SBPS_NORMAL	,50);
	m_wndStatusBar.SetPaneInfo(4 , ID_INDICATOR_04, SBPS_NORMAL	,80);
	m_wndStatusBar.SetPaneInfo(5 , ID_INDICATOR_05, SBPS_NORMAL	,110);
	m_wndStatusBar.SetPaneInfo(6 , ID_INDICATOR_06, SBPS_NORMAL	,100);
	m_wndStatusBar.SetPaneInfo(7 , ID_INDICATOR_07, SBPS_NORMAL	,130);
	m_wndStatusBar.SetPaneInfo(8 , ID_INDICATOR_08, SBPS_NORMAL	,50);
	m_wndStatusBar.SetPaneInfo(9 , ID_INDICATOR_09, SBPS_NORMAL	,100);
	m_wndStatusBar.SetPaneInfo(10, ID_INDICATOR_10, SBPS_NORMAL	,100);
	
// 	if (!m_CamDialog.Create(this,IDD_CAM_DIALOG,CBRS_TOP|CBRS_TOOLTIPS,IDD_CAM_DIALOG))
// 	{
//         TRACE0("Failed to create IDD_CAM_DIALOG\n");
//         return -1; // fail to create
// 	}
// 
// 	if (!m_TraitImDialog.Create(this,IDD_TRAIT_IMAGE_DIALOG,CBRS_LEFT|CBRS_TOOLTIPS,IDD_TRAIT_IMAGE_DIALOG))
// 	{
//         TRACE0("Failed to create IDD_TRAIT_IMAGE_DIALOG\n");
//         return -1; // fail to create
// 	}
// 
 	if (!m_MainDialog.Create(this,IDD_MAIN_DIALOG,CBRS_LEFT|CBRS_TOOLTIPS,IDD_MAIN_DIALOG))
 	{
         TRACE0("Failed to create IDD_MAIN_DIALOG\n");
         return -1; // fail to create
 	}

	if (!m_PortDialog.Create(this,IDD_PORT_DIALOG,CBRS_LEFT|CBRS_TOOLTIPS,IDD_PORT_DIALOG))
	{
        TRACE0("Failed to create IDD_PORT_DIALOG\n");
        return -1; // fail to create
	}

// 	if (!m_FitDialog.Create(this,IDD_FIT_DIALOG,CBRS_RIGHT|CBRS_TOOLTIPS,IDD_FIT_DIALOG))
// 	{
//         TRACE0("Failed to create IDD_FIT_DIALOG\n");
//         return -1; // fail to create
// 	}
	
	if (!m_InputDialog.Create(this,IDD_INPUT_DIALOG,CBRS_LEFT|CBRS_TOOLTIPS,IDD_INPUT_DIALOG))
	{
        TRACE0("Failed to create IDD_INPUT_DIALOG\n");
        return -1; // fail to create
	}

// 	if (!m_GeneticDialog.Create(this,IDD_GENETIC_DIALOG,CBRS_LEFT|CBRS_TOOLTIPS,IDD_GENETIC_DIALOG))
// 	{
//         TRACE0("Failed to create IDD_GENETIC_DIALOG\n");
//         return -1; // fail to create
// 	}
// 
// 	if (!m_LaserDialog.Create(this,IDD_LASER_DIALOG,CBRS_LEFT|CBRS_TOOLTIPS,IDD_LASER_DIALOG))
// 	{
//         TRACE0("Failed to create IDD_LASER_DIALOG\n");
//         return -1; // fail to create
// 	}

	if (!m_SeqViewDialog.Create(this,IDD_SEQ_VIEW_DIALOG,CBRS_TOP|CBRS_TOOLTIPS,IDD_SEQ_VIEW_DIALOG))
	{
        TRACE0("Failed to create IDD_SEQ_VIEW_DIALOG\n");
        return -1; // fail to create
	}

	ModeDialogBar();

	// Voila un truc qui envoye un WM_TIMER event toute les xxxx milisecondes
	SetTimer(ID_CLOCK_TIMER, 100, NULL);	
	
	// on definit le repertoire de sauvegarde comme étant celui du .exe, 
	// ie : le repertoire courant au tout début de l'allumage du programme
	CSettingSave::DefineCurrentDirectory_SaveDirectory();

	DefineAllParameters();

	CString NameDefault(DEFAULT_CONFIG_FILE_NAME);
	m_Save.SetAllSettings( NameDefault );
	m_MainDialog.LaWfMainDialog.LoadFromFileWf("LaWfDeMainDialog.Wf");

	m_wndSplitter.SetRowInfo(0, m_splitterPosition, 0);
	m_wndSplitter.RecalcLayout();

	ModeDialogBar();

//////////////////////////////////////////////////////////////////////////////////////////
//	if(!m_CarteDAQmxSemblePouvoirMarcher)
//		ModeDialogBar(TRAITIMAGE);
//////////////////////////////////////////////////////////////////////////////////////////

return 0;
}

// appeller lors de OnCreate pour fabriquer la CView, ou  la CScrollView
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
	
	// Tous ce qui concerne la CSplitterWnd
	/////////////////////////// CF : Doc&Astuces  : "CODEPROJECT_SPLITTER.aspx.htm"
	CRect cr(0,0,600,600); 
	if( !m_wndSplitter.CreateStatic( this, 2, 1 ) ) 
	{ 
		MessageBox( "Error setting up splitter frames!", 
			"Init Error!", MB_OK | MB_ICONERROR ); 
		return FALSE; 
	}
	if( !m_wndSplitter.CreateView( 0, 0, 
		RUNTIME_CLASS(CRb2View), 
		CSize(cr.Width(), m_splitterPosition), pContext ) ) 
	{ 
		MessageBox( "Error setting up splitter frames!", 
			"Init Error!", MB_OK | MB_ICONERROR );
		return FALSE; 
	}
	if( !m_wndSplitter.CreateView( 1, 0, 
		RUNTIME_CLASS(CRb2View), 
		CSize(cr.Width(), cr.Height()/2 - m_splitterPosition), pContext ) ) 
	{ 
		MessageBox( "Error setting up splitter frames!", 
			"Init Error!", MB_OK | MB_ICONERROR );
		return FALSE; 
	}

return 1;//CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO : Changez ici la classe ou les styles Window en modifiant
	//  CREATESTRUCT cs
  
	CString NameDefault(DEFAULT_CONFIG_FILE_NAME);
	((CRb2ManipApp*)AfxGetApp())->Rb2ManipWinSave.SetWinSettings(cs, CString("Rb2WinSet.Rb2"));
	return TRUE;
}


// diagnostics pour CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

void CMainFrame::OnClose()
{
	OnFileSaveRb2();
	if(m_CarteDAQmxSemblePouvoirMarcher)
	{
		m_SeqViewDialog.ValueChangedSeqviewStart(0);
		m_MainDialog.ValueChangedButtonStartSequence(0);
		m_PortDialog.UpdateControls();
		m_PortDialog.ValueChangedPortCheckUpdateNow(true);
	}
	if(0 && m_CarteDAQmxSemblePouvoirMarcher)
	{
		m_PortDialog.m_bDigOut[12] = 0;
		m_PortDialog.m_Ana_2_Out[2] = 0;
		m_PortDialog.UpdateControls();
		m_PortDialog.ValueChangedPortCheckUpdateNow(true);
	}

	// un peu de ménage !
	// on vide la liste parametre avant d'éteindre
	while (!CParametre::Liste_Static_AllParametres.IsEmpty())
		delete CParametre::Liste_Static_AllParametres.GetHead();
	CFrameWnd::OnClose();
}

void CMainFrame::OnExitwithoutsaving()
{
	m_MainDialog.ValueChangedButtonStartSequence(0);

	m_PortDialog.m_bDigOut[13] = 0;
	m_PortDialog.UpdateControls();
	m_PortDialog.ValueChangedPortCheckUpdateNow(true);

	CString NameDate("");
	m_Save.SaveSettingsDate(NameDate);

	if(0 && m_CarteDAQmxSemblePouvoirMarcher)
	{
		m_PortDialog.m_bDigOut[12] = 0;
		m_PortDialog.m_Ana_2_Out[2] = 0;
		m_PortDialog.UpdateControls();
		m_PortDialog.ValueChangedPortCheckUpdateNow(true);
	}

	// on vide la liste Parametre avant d'éteindre
	while (!CParametre::Liste_Static_AllParametres.IsEmpty())
		delete CParametre::Liste_Static_AllParametres.GetHead();
	CFrameWnd::OnClose();
}


// gestionnaires de messages pour CMainFrame

void CMainFrame::OnTimer(UINT nIDEvent)
{
	CFrameWnd::OnTimer(nIDEvent);
}



void CMainFrame::OnBnClickedButtonSwitchModeManip()
{
	int TheMode;
	if((m_Affichage & MAIN) && !(m_Affichage & PORT))
		TheMode = m_Affichage - MAIN + PORT;
	else if((m_Affichage & PORT) && !(m_Affichage & MAIN))
		TheMode = m_Affichage - PORT + MAIN;
	else
		TheMode = MAIN;
	ModeDialogBar(TheMode);
}

//  change entre les fenêtres     1:MainDialog   ou   0:Port
void CMainFrame::OnMainPort()
{	
	int TheMode;
	if(!(m_Affichage & MAIN))
		TheMode = m_Affichage ^ MAIN;
	else if(m_Affichage != MAIN)
		TheMode = MAIN;
	else
		TheMode = m_AffichagePreviousPrevious;
	ModeDialogBar(TheMode);
}
void CMainFrame::OnSwitchView()
{	
	int TheMode = m_Affichage != VIEW ? VIEW : m_AffichagePrevious;
	ModeDialogBar(TheMode);
}
void CMainFrame::OnCamMode()
{	
	int TheMode;
	if(!(m_Affichage & (CAM)))
		TheMode = m_Affichage ^ (CAM + TRAITIMAGE);
	else if(m_Affichage != CAM + TRAITIMAGE)
		TheMode = CAM + TRAITIMAGE;
	else
		TheMode = m_AffichagePreviousPrevious;
	ModeDialogBar(TheMode);
}
void CMainFrame::OnTraitImageMode()
{
	int TheMode;
	if(!(m_Affichage & (TRAITIMAGE + FIT)))
		TheMode = TRAITIMAGE + FIT;
	else if(m_Affichage != TRAITIMAGE)
		TheMode = TRAITIMAGE;
	else
		TheMode = m_AffichagePreviousPrevious;
	ModeDialogBar(TheMode);
}
void CMainFrame::OnInputMode()
{	
	ModeDialogBar(INPUTDIALOG);
}
void CMainFrame::OnLaserMode()
{	
	int TheMode;
	if(!(m_Affichage & LASERDIALOG))
		TheMode = m_Affichage ^ LASERDIALOG;
	else if(m_Affichage != LASERDIALOG)
		TheMode = LASERDIALOG;
	else
		TheMode = m_AffichagePreviousPrevious;
	ModeDialogBar(TheMode);
}
void CMainFrame::OnSeqView()
{	
	ModeDialogBar(SEQVIEW);
}

void CMainFrame::OnGeneticView()
{	
	int TheMode;
	if(!(m_Affichage & GENETIC))
		TheMode = m_Affichage ^ GENETIC;
	else if(m_Affichage != GENETIC)
		TheMode = GENETIC;
	else
		TheMode = m_AffichagePreviousPrevious;
	ModeDialogBar(TheMode);
}

void CMainFrame::OnPort()
{	
		ModeDialogBar(PORT);
}
void CMainFrame::ModeDialogBar(int mode /* = ACTUALISE */)
{
	m_AffichagePreviousPrevious = m_AffichagePrevious;
	m_AffichagePrevious = m_Affichage;
	m_Affichage = (mode == ACTUALISE) ? m_Affichage : mode;

	if(m_Affichage >= CAM)
		m_wndSplitter.SetRowInfo(0, 1500, 0);
	else
		m_wndSplitter.SetRowInfo(0, m_splitterPosition, 0);
	m_wndSplitter.RecalcLayout();

	ShowControlBar(&m_MainDialog	, m_Affichage & MAIN		, FALSE);
	ShowControlBar(&m_PortDialog	, m_Affichage & PORT		, FALSE);
	ShowControlBar(&m_SeqViewDialog	, m_Affichage & SEQVIEW		, FALSE);
	ShowControlBar(&m_SeqViewDialog.m_SeqViewLeftDialog	, m_Affichage & SEQVIEW		, FALSE);
	ShowControlBar(&m_InputDialog	, m_Affichage & INPUTDIALOG	, FALSE);
//srybex 	ShowControlBar(&m_LaserDialog	, m_Affichage & LASERDIALOG	, FALSE);
// 	ShowControlBar(&m_CamDialog		, m_Affichage & CAM			, FALSE);
// 	ShowControlBar(&m_TraitImDialog	, m_Affichage & TRAITIMAGE	, FALSE);
// 	ShowControlBar(&m_FitDialog		, m_Affichage & FIT			, FALSE);
// 	ShowControlBar(&m_GeneticDialog	, m_Affichage & GENETIC		, FALSE);

	if(m_Affichage & PORT && m_PortDialog.m_bWasPaintedOnce)
		m_PortDialog.UpdateControls();
	if(m_Affichage & MAIN && m_MainDialog.m_bWasPaintedOnce)
		m_MainDialog.UpdateControls();
// 	if(m_Affichage & TRAITIMAGE && m_TraitImDialog.m_bWasPaintedOnce)
// 		m_TraitImDialog.UpdateControlsTraitImage();
}

void CMainFrame::OnFileOpen()
{
	CString OpenFilter;
	OpenFilter = "Rb2 File (*.Rb2)|*.Rb2||";

	CFileDialog FileOpenDialog( TRUE, NULL, NULL, 
		OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_ENABLESIZING 
		| OFN_EXPLORER, OpenFilter, AfxGetMainWnd());
	if( FileOpenDialog.DoModal () != IDOK ) 
		return;

	OnFileOpen(FileOpenDialog.GetPathName());
return;
}

/*
Ouvre le fichier "aFile"
si on spécifie deux entier, on ne charge que les parametres dont le numéro est entre
ces 2 entiers inclus.
*/
void CMainFrame::OnFileOpen(CString aFile, int FromParam /* = -1 */, int ToParam /* = -1 */)
{
	CString TheFile = aFile;
	if(TheFile == "")
		return;

	CString NameDate("");
	m_Save.SaveSettingsDate(NameDate);

	m_Save.SetAllSettings(TheFile);

	if(m_MainDialog.LaSeqMainDialog.m_LaVariation.MakeParametreList(FromParam, ToParam))
		m_MainDialog.OnBnClickedRadioModeRepet1();


	m_PortDialog.UpdateControls();
	m_MainDialog.UpdateControls();
	m_PortDialog.UpdateEditColor();
	m_MainDialog.UpdateEditColor();
	m_MainDialog.UpdateCheckBoxesVarie();	
	m_MainDialog.UpdateStaticText();
}
void CMainFrame::OnFileSaveRb2()
{
	CString NewFichier;
	NewFichier.Format("%s\\%s", CSettingSave::GetDirExe(), DEFAULT_CONFIG_FILE_NAME);
	m_Save.SaveSettings(NewFichier);
	NewFichier.Format("%s\\%s", CSettingSave::GetDirExe(), "Rb2WinSet.Rb2");
	((CRb2ManipApp*)AfxGetApp())->Rb2ManipWinSave.SaveWinSettings(NewFichier);

// 	NewFichier.Format("%s\\%s", CSettingSave::GetDirExe(), "SaveFitDialog");
// 	m_FitDialog.m_SaveFit.SaveSettings(NewFichier);
// 
// 	NewFichier.Format("%s\\%s", CSettingSave::GetDirExe(), SAVE_DIALOG_GENETIC_FILE_NAME);
// 	m_GeneticDialog.m_SaveGenetic.SaveSettings(NewFichier);
return;
}
void CMainFrame::OnFileSaveAsRb2()
{
	CString OpenFilter;
	OpenFilter = "Rb2 File (*.Rb2)|*.Rb2||";

	char NewFichier[1000];
	sprintf(NewFichier,"");
	CString ladate = LaDate() + "_";
	strcat(NewFichier,ladate);

	CFileDialog FileSaveDialog( FALSE, "Rb2", NewFichier, 
		OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_ENABLESIZING 
		| OFN_EXPLORER, OpenFilter, AfxGetMainWnd());
	if( FileSaveDialog.DoModal () != IDOK ) 
		return;

	m_Save.SaveSettings(FileSaveDialog.GetPathName());
return;
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	MessageBeep(1);
	char TheFile[MAX_PATH];
	DragQueryFile(hDropInfo, 0, TheFile, MAX_PATH);
	OnFileOpen(TheFile);
	CFrameWnd::OnDropFiles(hDropInfo);
}

CString CMainFrame::FindLastFile(CString PartieDuNomFichier, CString Dossier /* = "" */)
{
	CString LeDossier = Dossier;
	if(LeDossier == "")
		LeDossier = "C:\\Program Files\\Rb2Manip\\";
	CString TheFile ="";

	CFileFind f;												//
	char recherche[512];										// 
	sprintf(recherche,"%s*%s*",LeDossier,PartieDuNomFichier);	//
	BOOL bMoreFiles = f.FindFile(recherche);					// recherche le dernier fichier 
	if(!bMoreFiles)												// contenant "PartieDuNom"
	{															// dans le dossier "LeDossier"
		MessageBox(recherche, "Pas de Fichier de Référence");	//
		return "";												//
	}															//
	while (bMoreFiles)
	{
		bMoreFiles = f.FindNextFile();
	}	
	TheFile = f.GetFileName();
	TheFile.Insert(0,LeDossier);
return TheFile;
}

void CMainFrame::OnSequenceFullsequence()
{	
	CString LeDossier = "";
	CString PartieDuNom = "Seq";
	CString TheFile = FindLastFile(PartieDuNom, LeDossier);
	OnFileOpen(TheFile, 0,  199);
}

void CMainFrame::OnSequenceMolasses()
{
	CString LeDossier = "";
	CString PartieDuNom = "Mol";
	CString TheFile = FindLastFile(PartieDuNom, LeDossier);
	OnFileOpen(TheFile, 0,  199);
}

void CMainFrame::OnStaticportReferencemot()
{
	CString LeDossier = "";
	CString PartieDuNom = "MOT";
	CString TheFile = FindLastFile(PartieDuNom, LeDossier);
	OnFileOpen(TheFile, 200,  279);
}

void CMainFrame::OnStaticportReferencemolasses()
{
	CString LeDossier = "";
	CString PartieDuNom = "Mol";
	CString TheFile = FindLastFile(PartieDuNom, LeDossier);
	OnFileOpen(TheFile, 200,  279);
}

/*
Définit tous les paramètres pour une utilisation facile des "Variation" et des "SaveSettings"
PS: ne pas changer les numérotations sans avoir vérifier qu'elle n'intervient pas dans une autre partie du programme
*/
unsigned int CMainFrame::DefineAllParameters()
{	CParametre* pPar;

	new CParametre(&m_Save, Entier,	"Les Fentres    "		,"m_splitterPosition   "	,Auto, &m_splitterPosition, ""	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Les Fentres    "		,"m_Affichage   "	,Auto, &m_Affichage, ""	, CanParticipateToNothing 								);

	new CParametre(&m_Save, Entier,	"WaitBeforeLoad "		,"m_MainDialog.m_WaitBeforeLoad   "	,Auto,&m_MainDialog.m_WaitBeforeLoad, "0"	, CanParticipateToNothing 								);
	pPar = new CParametre(&m_Save, Entier,	"SampleRate     "		,"m_MainDialog.m_NbrComboSampleRate",Auto,&m_MainDialog.m_NbrComboSampleRate,"1", CanParticipateToNothing 								);
	pPar->m_MightComplainIfMissing = false; // idem

//*********************************************************************************************************************************************************************************************************
//******************	Declaration des parametres propres à une experience donnée		*******************************************************************************************************************
//*********************************************************************************************************************************************************************************************************
// ! ! ! ! ! ! ! Attention, l'ordre est important ! ! ! ! ! ! !
// Chaque parametre se fait relier au Nieme control de la DialogBar a laquelle il apartient !
//			int m_Type,		CString NomTexte,	 CString NomSave,						int Numero,		int* m_pInt/m_pDouble/m_pString,	int CanParticipateToAScan?
	new CParametre(&m_Save, Entier,	"Active-Load    "		,"m_MainDialog.m_ParamBool[1]     "	,Auto,&m_MainDialog.m_ParamBool[1]	, "1"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-Compress"		,"m_MainDialog.m_ParamBool[2]     "	,Auto,&m_MainDialog.m_ParamBool[2]	, "1"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-Melasse "		,"m_MainDialog.m_ParamBool[3]     "	,Auto,&m_MainDialog.m_ParamBool[3]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-Polar   "		,"m_MainDialog.m_ParamBool[4]     "	,Auto,&m_MainDialog.m_ParamBool[4]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-Preguide"		,"m_MainDialog.m_ParamBool[5]     "	,Auto,&m_MainDialog.m_ParamBool[5]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-Tof     "		,"m_MainDialog.m_ParamBool[6]     "	,Auto,&m_MainDialog.m_ParamBool[6]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-Fluo    "		,"m_MainDialog.m_ParamBool[7]     "	,Auto,&m_MainDialog.m_ParamBool[7]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-Absorp  "		,"m_MainDialog.m_ParamBool[8]     "	,Auto,&m_MainDialog.m_ParamBool[8]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Active-DeadTime"		,"m_MainDialog.m_ParamBool[9]     "	,Auto,&m_MainDialog.m_ParamBool[9]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Stern-Gerlach  "		,"m_MainDialog.m_ParamBool[10]    "	,Auto,&m_MainDialog.m_ParamBool[10]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Trig-Compress"			,"m_MainDialog.m_ParamBool[11]    "	,Auto,&m_MainDialog.m_ParamBool[11]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Trig-Melasse "			,"m_MainDialog.m_ParamBool[12]    "	,Auto,&m_MainDialog.m_ParamBool[12]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Trig-Polar   "			,"m_MainDialog.m_ParamBool[13]    "	,Auto,&m_MainDialog.m_ParamBool[13]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Trig-Preguide"			,"m_MainDialog.m_ParamBool[14]    "	,Auto,&m_MainDialog.m_ParamBool[14]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Trig-Tof     "			,"m_MainDialog.m_ParamBool[15]    "	,Auto,&m_MainDialog.m_ParamBool[15]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"UseWfEvap    "			,"m_MainDialog.m_ParamBool[16]    "	,Auto,&m_MainDialog.m_ParamBool[16]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Trig-Absorp  "			,"m_MainDialog.m_ParamBool[17]    "	,Auto,&m_MainDialog.m_ParamBool[17]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Trig-DeadTime"			,"m_MainDialog.m_ParamBool[18]    "	,Auto,&m_MainDialog.m_ParamBool[18]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Mol H-Speed"			,"m_MainDialog.m_ParamBool[19]    "	,Auto,&m_MainDialog.m_ParamBool[19]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[20]" 				,"m_MainDialog.m_ParamBool[20]    "	,Auto,&m_MainDialog.m_ParamBool[20]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[21]" 				,"m_MainDialog.m_ParamBool[21]    "	,Auto,&m_MainDialog.m_ParamBool[21]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[22]" 				,"m_MainDialog.m_ParamBool[22]    "	,Auto,&m_MainDialog.m_ParamBool[22]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[23]" 				,"m_MainDialog.m_ParamBool[23]    "	,Auto,&m_MainDialog.m_ParamBool[23]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[24]" 				,"m_MainDialog.m_ParamBool[24]    "	,Auto,&m_MainDialog.m_ParamBool[24]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[25]" 				,"m_MainDialog.m_ParamBool[25]    "	,Auto,&m_MainDialog.m_ParamBool[25]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[26]" 				,"m_MainDialog.m_ParamBool[26]    "	,Auto,&m_MainDialog.m_ParamBool[26]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[27]" 				,"m_MainDialog.m_ParamBool[27]    "	,Auto,&m_MainDialog.m_ParamBool[27]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[28]" 				,"m_MainDialog.m_ParamBool[28]    "	,Auto,&m_MainDialog.m_ParamBool[28]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[29]" 				,"m_MainDialog.m_ParamBool[29]    "	,Auto,&m_MainDialog.m_ParamBool[29]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[30]" 				,"m_MainDialog.m_ParamBool[30]    "	,Auto,&m_MainDialog.m_ParamBool[30]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[31]" 				,"m_MainDialog.m_ParamBool[31]    "	,Auto,&m_MainDialog.m_ParamBool[31]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[32]" 				,"m_MainDialog.m_ParamBool[32]    "	,Auto,&m_MainDialog.m_ParamBool[32]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[33]" 				,"m_MainDialog.m_ParamBool[33]    "	,Auto,&m_MainDialog.m_ParamBool[33]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[34]" 				,"m_MainDialog.m_ParamBool[34]    "	,Auto,&m_MainDialog.m_ParamBool[34]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[35]" 				,"m_MainDialog.m_ParamBool[35]    "	,Auto,&m_MainDialog.m_ParamBool[35]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[36]" 				,"m_MainDialog.m_ParamBool[36]    "	,Auto,&m_MainDialog.m_ParamBool[36]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[37]" 				,"m_MainDialog.m_ParamBool[37]    "	,Auto,&m_MainDialog.m_ParamBool[37]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[38]" 				,"m_MainDialog.m_ParamBool[38]    "	,Auto,&m_MainDialog.m_ParamBool[38]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[39]" 				,"m_MainDialog.m_ParamBool[39]    "	,Auto,&m_MainDialog.m_ParamBool[39]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[40]" 				,"m_MainDialog.m_ParamBool[40]    "	,Auto,&m_MainDialog.m_ParamBool[40]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[41]" 				,"m_MainDialog.m_ParamBool[41]    "	,Auto,&m_MainDialog.m_ParamBool[41]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[42]" 				,"m_MainDialog.m_ParamBool[42]    "	,Auto,&m_MainDialog.m_ParamBool[42]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[43]" 				,"m_MainDialog.m_ParamBool[43]    "	,Auto,&m_MainDialog.m_ParamBool[43]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[44]" 				,"m_MainDialog.m_ParamBool[44]    "	,Auto,&m_MainDialog.m_ParamBool[44]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[45]" 				,"m_MainDialog.m_ParamBool[45]    "	,Auto,&m_MainDialog.m_ParamBool[45]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[46]" 				,"m_MainDialog.m_ParamBool[46]    "	,Auto,&m_MainDialog.m_ParamBool[46]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[47]" 				,"m_MainDialog.m_ParamBool[47]    "	,Auto,&m_MainDialog.m_ParamBool[47]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[48]" 				,"m_MainDialog.m_ParamBool[48]    "	,Auto,&m_MainDialog.m_ParamBool[48]	, "0"	, CanParticipateToNothing 								);
	new CParametre(&m_Save, Entier,	"Bool[49]"				,"m_MainDialog.m_ParamBool[49]    "	,Auto,&m_MainDialog.m_ParamBool[49]	, "0"	, CanParticipateToNothing 								);
																																																	
	new CParametre(&m_Save, Double,	"Load MOT Det"			,"m_MainDialog.m_ParamInt[1]      "	,Auto,&m_MainDialog.m_ParamReal[1]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Time Z.S. On"			,"m_MainDialog.m_ParamInt[2]      "	,Auto,&m_MainDialog.m_ParamReal[2]	, "85"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Time Z.S. Off"			,"m_MainDialog.m_ParamInt[3]      "	,Auto,&m_MainDialog.m_ParamReal[3]	, "25"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"C-MOT Duration"		,"m_MainDialog.m_ParamInt[4]      "	,Auto,&m_MainDialog.m_ParamReal[4]	, "50"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"C-MOT Det"				,"m_MainDialog.m_ParamInt[5]      "	,Auto,&m_MainDialog.m_ParamReal[5]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"C-MOT Mot Int"			,"m_MainDialog.m_ParamInt[6]      "	,Auto,&m_MainDialog.m_ParamReal[6]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"C-MOT Rep Int"			,"m_MainDialog.m_ParamInt[7]      "	,Auto,&m_MainDialog.m_ParamReal[7]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"C-MOT Field Grad"		,"m_MainDialog.m_ParamInt[8]      "	,Auto,&m_MainDialog.m_ParamReal[8]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"C-MOT Field 3D Grad"	,"m_MainDialog.m_ParamInt[9]      "	,Auto,&m_MainDialog.m_ParamReal[9]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol Plateau Duration"	,"m_MainDialog.m_ParamInt[10]     "	,Auto,&m_MainDialog.m_ParamReal[10]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol Plateau Detuning"	,"m_MainDialog.m_ParamInt[11]     "	,Auto,&m_MainDialog.m_ParamReal[11]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol Plateau MOT Int"	,"m_MainDialog.m_ParamInt[12]     "	,Auto,&m_MainDialog.m_ParamReal[12]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol Plateau Rep Int"	,"m_MainDialog.m_ParamInt[13]     "	,Auto,&m_MainDialog.m_ParamReal[13]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol Slope Duration "	,"m_MainDialog.m_ParamInt[14]     "	,Auto,&m_MainDialog.m_ParamReal[14]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol End Detuning"		,"m_MainDialog.m_ParamInt[15]     "	,Auto,&m_MainDialog.m_ParamReal[15]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol End MOT Int"		,"m_MainDialog.m_ParamInt[16]     "	,Auto,&m_MainDialog.m_ParamReal[16]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Mol End Rep Int"		,"m_MainDialog.m_ParamInt[17]     "	,Auto,&m_MainDialog.m_ParamReal[17]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Polar Time 1"			,"m_MainDialog.m_ParamInt[18]     "	,Auto,&m_MainDialog.m_ParamReal[18]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Polar Time 2"			,"m_MainDialog.m_ParamInt[19]     "	,Auto,&m_MainDialog.m_ParamReal[19]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Preg Duration"			,"m_MainDialog.m_ParamInt[20]     "	,Auto,&m_MainDialog.m_ParamReal[20]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Preg Field Grad"		,"m_MainDialog.m_ParamInt[21]     "	,Auto,&m_MainDialog.m_ParamReal[21]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"TOF"					,"m_MainDialog.m_ParamInt[22]     "	,Auto,&m_MainDialog.m_ParamReal[22]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Fluo Expo"				,"m_MainDialog.m_ParamInt[23]     "	,Auto,&m_MainDialog.m_ParamReal[23]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Absorp Expo"			,"m_MainDialog.m_ParamInt[24]     "	,Auto,&m_MainDialog.m_ParamReal[24]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);			   	
	new CParametre(&m_Save, Double,	"Dead Time"				,"m_MainDialog.m_ParamInt[25]     "	,Auto,&m_MainDialog.m_ParamReal[25]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"C-MOT Ramp Dur"		,"m_MainDialog.m_ParamInt[26]     "	,Auto,&m_MainDialog.m_ParamReal[26]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"C-MOT Ramp Int"		,"m_MainDialog.m_ParamInt[27]     "	,Auto,&m_MainDialog.m_ParamReal[27]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Absorp Intensity"		,"m_MainDialog.m_ParamInt[28]     "	,Auto,&m_MainDialog.m_ParamReal[28]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[29]"				,"m_MainDialog.m_ParamInt[29]     "	,Auto,&m_MainDialog.m_ParamReal[29]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[30]"				,"m_MainDialog.m_ParamInt[30]     "	,Auto,&m_MainDialog.m_ParamReal[30]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[31]"				,"m_MainDialog.m_ParamInt[31]     "	,Auto,&m_MainDialog.m_ParamReal[31]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[32]"				,"m_MainDialog.m_ParamInt[32]     "	,Auto,&m_MainDialog.m_ParamReal[32]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[33]"				,"m_MainDialog.m_ParamInt[33]     "	,Auto,&m_MainDialog.m_ParamReal[33]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[34]"				,"m_MainDialog.m_ParamInt[34]     "	,Auto,&m_MainDialog.m_ParamReal[34]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[35]"				,"m_MainDialog.m_ParamInt[35]     "	,Auto,&m_MainDialog.m_ParamReal[35]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[36]"				,"m_MainDialog.m_ParamInt[36]     "	,Auto,&m_MainDialog.m_ParamReal[36]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[37]"				,"m_MainDialog.m_ParamInt[37]     "	,Auto,&m_MainDialog.m_ParamReal[37]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[38]"				,"m_MainDialog.m_ParamInt[38]     "	,Auto,&m_MainDialog.m_ParamReal[38]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[39]"				,"m_MainDialog.m_ParamInt[39]     "	,Auto,&m_MainDialog.m_ParamReal[39]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[40]"				,"m_MainDialog.m_ParamInt[40]     "	,Auto,&m_MainDialog.m_ParamReal[40]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[41]"				,"m_MainDialog.m_ParamInt[41]     "	,Auto,&m_MainDialog.m_ParamReal[41]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[42]"				,"m_MainDialog.m_ParamInt[42]     "	,Auto,&m_MainDialog.m_ParamReal[42]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[43]"				,"m_MainDialog.m_ParamInt[43]     "	,Auto,&m_MainDialog.m_ParamReal[43]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[44]"				,"m_MainDialog.m_ParamInt[44]     "	,Auto,&m_MainDialog.m_ParamReal[44]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[45]"				,"m_MainDialog.m_ParamInt[45]     "	,Auto,&m_MainDialog.m_ParamReal[45]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[46]"				,"m_MainDialog.m_ParamInt[46]     "	,Auto,&m_MainDialog.m_ParamReal[46]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[47]"				,"m_MainDialog.m_ParamInt[47]     "	,Auto,&m_MainDialog.m_ParamReal[47]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[48]"				,"m_MainDialog.m_ParamInt[48]     "	,Auto,&m_MainDialog.m_ParamReal[48]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[49]"				,"m_MainDialog.m_ParamInt[49]     "	,Auto,&m_MainDialog.m_ParamReal[49]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);					   	
	new CParametre(&m_Save, Double,	"Int[50]"				,"m_MainDialog.m_ParamInt[50]     "	,Auto,&m_MainDialog.m_ParamReal[50]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[51]"				,"m_MainDialog.m_ParamInt[51]     "	,Auto,&m_MainDialog.m_ParamReal[51]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[52]"				,"m_MainDialog.m_ParamInt[52]     "	,Auto,&m_MainDialog.m_ParamReal[52]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[53]"				,"m_MainDialog.m_ParamInt[53]     "	,Auto,&m_MainDialog.m_ParamReal[53]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[54]"				,"m_MainDialog.m_ParamInt[54]     "	,Auto,&m_MainDialog.m_ParamReal[54]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[55]"				,"m_MainDialog.m_ParamInt[55]     "	,Auto,&m_MainDialog.m_ParamReal[55]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[56]"				,"m_MainDialog.m_ParamInt[56]     "	,Auto,&m_MainDialog.m_ParamReal[56]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[57]"				,"m_MainDialog.m_ParamInt[57]     "	,Auto,&m_MainDialog.m_ParamReal[57]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[58]"				,"m_MainDialog.m_ParamInt[58]     "	,Auto,&m_MainDialog.m_ParamReal[58]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[59]"				,"m_MainDialog.m_ParamInt[59]     "	,Auto,&m_MainDialog.m_ParamReal[59]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[60]"				,"m_MainDialog.m_ParamInt[60]     "	,Auto,&m_MainDialog.m_ParamReal[60]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[61]"				,"m_MainDialog.m_ParamInt[61]     "	,Auto,&m_MainDialog.m_ParamReal[61]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[62]"				,"m_MainDialog.m_ParamInt[62]     "	,Auto,&m_MainDialog.m_ParamReal[62]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[63]"				,"m_MainDialog.m_ParamInt[63]     "	,Auto,&m_MainDialog.m_ParamReal[63]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[64]"				,"m_MainDialog.m_ParamInt[64]     "	,Auto,&m_MainDialog.m_ParamReal[64]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[65]"				,"m_MainDialog.m_ParamInt[65]     "	,Auto,&m_MainDialog.m_ParamReal[65]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[66]"				,"m_MainDialog.m_ParamInt[66]     "	,Auto,&m_MainDialog.m_ParamReal[66]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[67]"				,"m_MainDialog.m_ParamInt[67]     "	,Auto,&m_MainDialog.m_ParamReal[67]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[68]"				,"m_MainDialog.m_ParamInt[68]     "	,Auto,&m_MainDialog.m_ParamReal[68]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[69]"				,"m_MainDialog.m_ParamInt[69]     "	,Auto,&m_MainDialog.m_ParamReal[69]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[70]"				,"m_MainDialog.m_ParamInt[70]     "	,Auto,&m_MainDialog.m_ParamReal[70]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[71]"				,"m_MainDialog.m_ParamInt[71]     "	,Auto,&m_MainDialog.m_ParamReal[71]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[72]"				,"m_MainDialog.m_ParamInt[72]     "	,Auto,&m_MainDialog.m_ParamReal[72]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[73]"				,"m_MainDialog.m_ParamInt[73]     "	,Auto,&m_MainDialog.m_ParamReal[73]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[74]"				,"m_MainDialog.m_ParamInt[74]     "	,Auto,&m_MainDialog.m_ParamReal[74]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[75]"				,"m_MainDialog.m_ParamInt[75]     "	,Auto,&m_MainDialog.m_ParamReal[75]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[76]"				,"m_MainDialog.m_ParamInt[76]     "	,Auto,&m_MainDialog.m_ParamReal[76]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[77]"				,"m_MainDialog.m_ParamInt[77]     "	,Auto,&m_MainDialog.m_ParamReal[77]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[78]"				,"m_MainDialog.m_ParamInt[78]     "	,Auto,&m_MainDialog.m_ParamReal[78]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[79]"				,"m_MainDialog.m_ParamInt[79]     "	,Auto,&m_MainDialog.m_ParamReal[79]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[80]"				,"m_MainDialog.m_ParamInt[80]     "	,Auto,&m_MainDialog.m_ParamReal[80]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[81]"				,"m_MainDialog.m_ParamInt[81]     "	,Auto,&m_MainDialog.m_ParamReal[81]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[82]"				,"m_MainDialog.m_ParamInt[82]     "	,Auto,&m_MainDialog.m_ParamReal[82]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[83]"				,"m_MainDialog.m_ParamInt[83]     "	,Auto,&m_MainDialog.m_ParamReal[83]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[84]"				,"m_MainDialog.m_ParamInt[84]     "	,Auto,&m_MainDialog.m_ParamReal[84]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[85]"				,"m_MainDialog.m_ParamInt[85]     "	,Auto,&m_MainDialog.m_ParamReal[85]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[86]"				,"m_MainDialog.m_ParamInt[86]     "	,Auto,&m_MainDialog.m_ParamReal[86]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[87]"				,"m_MainDialog.m_ParamInt[87]     "	,Auto,&m_MainDialog.m_ParamReal[87]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[88]"				,"m_MainDialog.m_ParamInt[88]     "	,Auto,&m_MainDialog.m_ParamReal[88]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Double,	"Int[89]"				,"m_MainDialog.m_ParamInt[89]     "	,Auto,&m_MainDialog.m_ParamReal[89]	, "0"	, CanParticipateVariation | CanParticipateGeneticAlgo 	);

	pPar = new CParametre(&m_Save, String,	"FilmFromColName","m_MainDialog.m_FilmFromColName  ",Auto,&m_MainDialog.m_FilmFromColName , " "	, CanParticipateToNothing 								);
	pPar->m_MightComplainIfMissing = false; // pas grave si il n'est pas dans le fichier...
	pPar = new CParametre(&m_Save, String,	"FilmToColName  ","m_MainDialog.m_FilmToColName    ",Auto,&m_MainDialog.m_FilmToColName   , " "	, CanParticipateToNothing 								);
	pPar->m_MightComplainIfMissing = false; // idem
	pPar = new CParametre(&m_Save, Double,	"m_FilmStep_ms  ","m_MainDialog.m_FilmStep_ms      ",Auto,&m_MainDialog.m_FilmStep_ms   , "10"	, CanParticipateToNothing 								);
	pPar->m_MightComplainIfMissing = false; // idem
	pPar = new CParametre(&m_Save, Double,	"m_FilmDuree_ms ","m_MainDialog.m_FilmDuree_ms     ",Auto,&m_MainDialog.m_FilmDuree_ms  , "10"	, CanParticipateToNothing 								);
	pPar->m_MightComplainIfMissing = false; // idem

// 	new CParametre(&m_Save, String,	"PathImage      "		,"m_TraitImDialog.m_PathImage     "	,Auto,&m_TraitImDialog.m_PathImage	, " "	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier,	"ModeSave       "		,"m_TraitImDialog.m_ModeSaveImages      "	,Auto,&m_TraitImDialog.m_ModeSaveImages	, " "	, CanParticipateToNothing 					);
// 	new CParametre(&m_Save, Entier,	"4Cadran        "		,"m_TraitImDialog.m_4Ecrans       "	,Auto,&m_TraitImDialog.m_4Ecrans	, "0"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier,	"ZoomToFit      "		,"m_TraitImDialog.m_ZoomFit       "	,Auto,&m_TraitImDialog.m_ZoomFit	, "0"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier,	"NbrImageSave   "		,"m_TraitImDialog.m_NbrImageToSave"	,Auto,&m_TraitImDialog.m_NbrImageToSave,"1"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Double, "m_PixelSize_µm "		,"m_TraitImDialog.m_PixelSize_µm  "	,Auto,&m_TraitImDialog.m_PixelSize_µm,"1000", CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Double, "m_RangeAbs     "		,"m_TraitImDialog.m_RangeAbs      "	,Auto,&m_TraitImDialog.m_RangeAbs	,"255.0", CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Double, "m_IsatSurIsat0  "		,"m_TraitImDialog.m_IsatSurIsat0  "	,Auto,&m_TraitImDialog.m_IsatSurIsat0	,"1.0"	, CanParticipateVariation 							);
// 	new CParametre(&m_Save, Double, "m_Isat0         "		,"m_TraitImDialog.m_Isat0         "	,Auto,&m_TraitImDialog.m_Isat0			,"1.67"	, CanParticipateToNothing 							);
// 	new CParametre(&m_Save, Double, "m_PulseLength   "		,"m_TraitImDialog.m_PulseLength   "	,Auto,&m_TraitImDialog.m_PulseLength	,"0.001", CanParticipateVariation 							);
// 	new CParametre(&m_Save, Double, "m_AttenBeforeCam"		,"m_TraitImDialog.m_AttenBeforeCam"	,Auto,&m_TraitImDialog.m_AttenBeforeCam	,"0", CanParticipateVariation 								);
// //	pPar = new CParametre(&m_Save, Double, "m_PixelAveragePointeur","m_TraitImDialog.m_PixelAveragePointeur",Auto,&m_TraitImDialog.m_PixelAveragePointeur,"1", CanParticipateToNothing 					);
// //	pPar->m_MightComplainIfMissing = false; // idem
// 
// 	new CParametre(&m_Save, Entier, "Wait Trigger   "		,"m_CamDialog.m_WaitTrigger       "	,Auto,&m_CamDialog.m_WaitTrigger	, "1"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier, "Camera Shutter "		,"m_CamDialog.m_CamExpo           "	,Auto,&m_CamDialog.m_CamExpo		, "500"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier, "Camera Gain    "		,"m_CamDialog.m_CamGain           "	,Auto,&m_CamDialog.m_CamGain		, "500"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier, "CameraAOITopLeftX"		,"m_CamDialog.m_AoiUpLeft.x       "	,Auto,&m_CamDialog.m_AoiUpLeft.x	, "0"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier, "CameraAOITopLeftY"		,"m_CamDialog.m_AoiUpLeft.y       "	,Auto,&m_CamDialog.m_AoiUpLeft.y	, "0"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier, "CameraAOISizeX"		,"m_CamDialog.m_AoiSize.cx        "	,Auto,&m_CamDialog.m_AoiSize.cx		, "800"	, CanParticipateToNothing 								);
// 	new CParametre(&m_Save, Entier, "CameraAOISizeY"		,"m_CamDialog.m_AoiSize.cy        "	,Auto,&m_CamDialog.m_AoiSize.cy		, "600"	, CanParticipateToNothing 								);

																					
//*********************************************************************************************************************************************************************************************************
//******************	Declaration des parametres de la fenetre PortDialog		***************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************
	new CParametre(&m_Save, Entier,	"Sr88           ","m_PortDialog.m_bDigOut[0]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 0, &m_PortDialog.m_bDigOut[0]	, "1", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"MotAom         ","m_PortDialog.m_bDigOut[1]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 1, &m_PortDialog.m_bDigOut[1]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"MotZSShuter    ","m_PortDialog.m_bDigOut[2]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 2, &m_PortDialog.m_bDigOut[2]	, "1", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"03             ","m_PortDialog.m_bDigOut[3]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 3, &m_PortDialog.m_bDigOut[3]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"ZS+Repumps     ","m_PortDialog.m_bDigOut[4]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 4, &m_PortDialog.m_bDigOut[4]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"ImagAom        ","m_PortDialog.m_bDigOut[5]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 5, &m_PortDialog.m_bDigOut[5]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"ImagShutter    ","m_PortDialog.m_bDigOut[6]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 6, &m_PortDialog.m_bDigOut[6]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"914nmShutter   ","m_PortDialog.m_bDigOut[7]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 7, &m_PortDialog.m_bDigOut[7]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"08             ","m_PortDialog.m_bDigOut[8]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 8, &m_PortDialog.m_bDigOut[8]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"3Dness         ","m_PortDialog.m_bDigOut[9]       ",/*Param_*/FIRST_INDEX_PARAM_PORT + 9, &m_PortDialog.m_bDigOut[9]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"10             ","m_PortDialog.m_bDigOut[10]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 10,&m_PortDialog.m_bDigOut[10]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"11             ","m_PortDialog.m_bDigOut[11]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 11,&m_PortDialog.m_bDigOut[11]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"CamTrigger     ","m_PortDialog.m_bDigOut[12]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 12,&m_PortDialog.m_bDigOut[12]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"RepumpShutter  ","m_PortDialog.m_bDigOut[13]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 13,&m_PortDialog.m_bDigOut[13]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"RedMOTShutter  ","m_PortDialog.m_bDigOut[14]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 14,&m_PortDialog.m_bDigOut[14]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"IGBT           ","m_PortDialog.m_bDigOut[15]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 15,&m_PortDialog.m_bDigOut[15]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"16             ","m_PortDialog.m_bDigOut[16]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 16,&m_PortDialog.m_bDigOut[16]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"RedBroad       ","m_PortDialog.m_bDigOut[17]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 17,&m_PortDialog.m_bDigOut[17]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"RedSingle      ","m_PortDialog.m_bDigOut[18]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 18,&m_PortDialog.m_bDigOut[18]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"19             ","m_PortDialog.m_bDigOut[19]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 19,&m_PortDialog.m_bDigOut[19]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"20             ","m_PortDialog.m_bDigOut[20]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 20,&m_PortDialog.m_bDigOut[20]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"21             ","m_PortDialog.m_bDigOut[21]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 21,&m_PortDialog.m_bDigOut[21]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"22             ","m_PortDialog.m_bDigOut[22]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 22,&m_PortDialog.m_bDigOut[22]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"BShut          ","m_PortDialog.m_bDigOut[23]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 23,&m_PortDialog.m_bDigOut[23]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"CShut          ","m_PortDialog.m_bDigOut[24]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 24,&m_PortDialog.m_bDigOut[24]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"25             ","m_PortDialog.m_bDigOut[25]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 25,&m_PortDialog.m_bDigOut[25]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"26             ","m_PortDialog.m_bDigOut[26]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 26,&m_PortDialog.m_bDigOut[26]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"E650             ","m_PortDialog.m_bDigOut[27]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 27,&m_PortDialog.m_bDigOut[27]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"D689             ","m_PortDialog.m_bDigOut[28]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 28,&m_PortDialog.m_bDigOut[28]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"C689             ","m_PortDialog.m_bDigOut[29]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 29,&m_PortDialog.m_bDigOut[29]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"B689             ","m_PortDialog.m_bDigOut[30]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 30,&m_PortDialog.m_bDigOut[30]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"PShut             ","m_PortDialog.m_bDigOut[31]      ",/*Param_*/FIRST_INDEX_PARAM_PORT + 31,&m_PortDialog.m_bDigOut[31]	, "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"V-Bz		    ","m_PortDialog.m_Ana_1_Out[0]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 50,&m_PortDialog.m_Ana_1_Out[0]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);	 	
	new CParametre(&m_Save, Entier,	"ZS Det         ","m_PortDialog.m_Ana_1_Out[1]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 51,&m_PortDialog.m_Ana_1_Out[1]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);	 	
	new CParametre(&m_Save, Entier,	"3DLattRamp     ","m_PortDialog.m_Ana_1_Out[2]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 52,&m_PortDialog.m_Ana_1_Out[2]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	); 	
	new CParametre(&m_Save, Entier,	"BAttenLaser    ","m_PortDialog.m_Ana_1_Out[3]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 53,&m_PortDialog.m_Ana_1_Out[3]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"Ana 04         ","m_PortDialog.m_Ana_1_Out[4]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 54,&m_PortDialog.m_Ana_1_Out[4] , "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"Ana 05         ","m_PortDialog.m_Ana_1_Out[5]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 55,&m_PortDialog.m_Ana_1_Out[5] , "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"Ana 06         ","m_PortDialog.m_Ana_1_Out[6]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 56,&m_PortDialog.m_Ana_1_Out[6]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);	 	
	new CParametre(&m_Save, Entier,	"I_MOT          ","m_PortDialog.m_Ana_1_Out[7]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 57,&m_PortDialog.m_Ana_1_Out[7]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"IntExtra0      ","m_PortDialog.m_Ana_2_Out[0]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 60,&m_PortDialog.m_Ana_2_Out[0]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"ImagInt        ","m_PortDialog.m_Ana_2_Out[1]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 61,&m_PortDialog.m_Ana_2_Out[1]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"GuideCurent    ","m_PortDialog.m_Ana_2_Out[2]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 62,&m_PortDialog.m_Ana_2_Out[2]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"ImagDet        ","m_PortDialog.m_Ana_2_Out[3]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 63,&m_PortDialog.m_Ana_2_Out[3]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"Vert_1         ","m_PortDialog.m_Ana_2_Out[4]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 64,&m_PortDialog.m_Ana_2_Out[4]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"Vert_2         ","m_PortDialog.m_Ana_2_Out[5]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 65,&m_PortDialog.m_Ana_2_Out[5]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"IntExtra4      ","m_PortDialog.m_Ana_2_Out[6]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 66,&m_PortDialog.m_Ana_2_Out[6]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"IntExtra5      ","m_PortDialog.m_Ana_2_Out[7]     ",/*Param_*/FIRST_INDEX_PARAM_PORT + 67,&m_PortDialog.m_Ana_2_Out[7]	, "0", CanParticipateVariation | CanParticipateGeneticAlgo 	);
	new CParametre(&m_Save, Entier,	"DigSave[0]     ","m_PortDialog.m_PortInt32_Save[0]",/*Param_*/FIRST_INDEX_PARAM_PORT + 70,&m_PortDialog.m_PortInt32_Save[0], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[1]     ","m_PortDialog.m_PortInt32_Save[1]",/*Param_*/FIRST_INDEX_PARAM_PORT + 71,&m_PortDialog.m_PortInt32_Save[1], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[2]     ","m_PortDialog.m_PortInt32_Save[2]",/*Param_*/FIRST_INDEX_PARAM_PORT + 72,&m_PortDialog.m_PortInt32_Save[2], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[3]     ","m_PortDialog.m_PortInt32_Save[3]",/*Param_*/FIRST_INDEX_PARAM_PORT + 73,&m_PortDialog.m_PortInt32_Save[3], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[4]     ","m_PortDialog.m_PortInt32_Save[4]",/*Param_*/FIRST_INDEX_PARAM_PORT + 74,&m_PortDialog.m_PortInt32_Save[4], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[5]     ","m_PortDialog.m_PortInt32_Save[5]",/*Param_*/FIRST_INDEX_PARAM_PORT + 75,&m_PortDialog.m_PortInt32_Save[5], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[6]     ","m_PortDialog.m_PortInt32_Save[6]",/*Param_*/FIRST_INDEX_PARAM_PORT + 76,&m_PortDialog.m_PortInt32_Save[6], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[7]     ","m_PortDialog.m_PortInt32_Save[7]",/*Param_*/FIRST_INDEX_PARAM_PORT + 77,&m_PortDialog.m_PortInt32_Save[7], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[8]     ","m_PortDialog.m_PortInt32_Save[8]",/*Param_*/FIRST_INDEX_PARAM_PORT + 78,&m_PortDialog.m_PortInt32_Save[8], "0", CanParticipateToNothing	);
	new CParametre(&m_Save, Entier,	"DigSave[9]     ","m_PortDialog.m_PortInt32_Save[9]",/*Param_*/FIRST_INDEX_PARAM_PORT + 79,&m_PortDialog.m_PortInt32_Save[9], "0", CanParticipateToNothing	);
	// On se definit les parametres pour les Memoires Analogiques de le fenetre port																			
	// Chaque Sortie Analogique a le meme m_NomSave et le meme m_Numero pour une memoire donnée.																
	char pNameSave[64]; 
	for(int iemePortSaveAna = 0; iemePortSaveAna < 10; iemePortSaveAna++)
	{
		sprintf(pNameSave,"m_PortDialog.m_PortAna_Save[%d][%d]          ", iemePortSaveAna, 0);		CString NameSave(pNameSave);
//2011	new CParametre(&m_Save, Entier,	"AnaSave        "		,NameSave								,/*Param_*/FIRST_INDEX_PARAM_PORT + 80 + iemePortSaveAna,	&m_PortDialog.m_PortAna_Save[iemePortSaveAna][0], "0", CanParticipateToNothing	,NUMBER_ANA_OUTPUT	,0	,-1	,-1	,-1);
	}
	// On se fait le lien entre les sorties digitales Par défaut d'une séquence et les bon parametre
	for(int i = 0; i < NUMBER_DIG_OUTPUT; i++)																														   
	{						
		CParametre* pParam = (CParametre::Param)(/*Param_*/FIRST_INDEX_PARAM_PORT + i);																														   
		Sequence::SetDefaultParamDigOut(i, pParam);
	}																																											   
	// idem analogiques
	for(int indexAnaOut = 0; indexAnaOut < NUMBER_ANA_OUTPUT; indexAnaOut++)																													   
	{																																											   
		CParametre* pParam = (CParametre::Param)(
			( indexAnaOut < NUMBER_ANA_OUTPUT_CARD_ANA_1
			? /*Param_*/FIRST_INDEX_PARAM_PORT + 50 
			: /*Param_*/FIRST_INDEX_PARAM_PORT + 60 - NUMBER_ANA_OUTPUT_CARD_ANA_1)
			+ indexAnaOut);
		Sequence::SetDefaultParamAnaOut(indexAnaOut, pParam);
	}
//*********************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************
//*********************************************************************************************************************************************************************************************************

return 0;
}


void CMainFrame::OnPrintMainDialog()
{
	CPoint PrintJusquA = CPoint(605,795);

	CPrntScreen * ScrCap;
	ScrCap = new CPrntScreen("Some error ocurred!!!","Message");
	ScrCap->DoPrntScreen(1,2,false, &m_MainDialog);//, CPoint(0,0), PrintJusquA);
	delete ScrCap;
	ScrCap=NULL;

	m_Save.SaveSettingsDate(CString("C:\\Rb2Manip_PrintConfig\\Printed"));
	Beep(1000,500);}

void CMainFrame::OnPrintPortDialog()
{
	CPoint PrintJusquA = CPoint(605,850);

	OnMainPort();
	m_PortDialog.RedrawWindow();

	CPrntScreen * ScrCap;
	ScrCap = new CPrntScreen("Some error ocurred!!!","Message");
	ScrCap->DoPrntScreen(3,2,false, &m_PortDialog, CPoint(0,0), PrintJusquA);
	delete ScrCap;
	ScrCap=NULL;

	OnMainPort();
	m_Save.SaveSettingsDate(CString("C:\\Rb2Manip_PrintConfig\\Printed"));
	Beep(1000,500);}


void CMainFrame::OnPrintBothDialog()
{
	CPoint PrintJusquA = CPoint(2*605,850);

	OnMainPort();
    ShowControlBar(&m_MainDialog, true, FALSE);
	m_MainDialog.RedrawWindow();
	ShowControlBar(&m_PortDialog, true, FALSE);
	m_PortDialog.RedrawWindow();

	CPrntScreen * ScrCap;
	ScrCap = new CPrntScreen("Some error ocurred!!!","Message");
	ScrCap->DoPrntScreen(3,2,false, &m_MainDialog, CPoint(0,0), PrintJusquA);
	delete ScrCap;
	ScrCap=NULL;

	OnMainPort();
	m_Save.SaveSettingsDate(CString("C:\\Rb2Manip_PrintConfig\\Printed"));
	Beep(1000,500);
}

void CMainFrame::OnPrintSequence()
{
	CPoint PrintJusquA = CPoint(605,850);

	OnSwitchView();
	RedrawWindow();

	CPrntScreen * ScrCap;
	ScrCap = new CPrntScreen("Some error ocurred!!!","Message");
	ScrCap->DoPrntScreen(2,2,false);
	delete ScrCap;
	ScrCap=NULL;

	OnSwitchView();
	m_Save.SaveSettingsDate(CString("C:\\Rb2Manip_PrintConfig\\Printed"));
	Beep(1000,500);
}
