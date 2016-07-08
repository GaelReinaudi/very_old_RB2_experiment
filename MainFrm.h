// MainFrm.h : interface de la classe CMainFrame

/////////////////////////// CF : Doc&Astuces  : "CView Access From Anywhere" 
#define m_View	(*(/*(CScrollView*)*/(((CRb2ManipApp*)AfxGetApp())->GetMyView())))
#define m_View2	(*(/*(CScrollView*)*/(((CRb2ManipApp*)AfxGetApp())->GetMyView2())))
/////////////////////////// End CF : Doc&Astuces  : "CView Access From Anywhere"

#define UPDATE_ONLY_ANALOG2 2

#pragma once

#include "Rb2Doc.h"
#include "Rb2View.h"

#include "Sequence.h"
#include "MainDialog.h"
#include "PortDialog.h"
#include "SeqViewDialog.h"
//srybex #include "CamDialog.h"
//srybex #include "TraitImageDialog.h"
//srybex #include "FitDialog.h"
#include "InputDialog.h"
//srybex #include "LaserDialog.h"
//srybex #include "GeneticDialog.h"
#include "SettingSave.h"

#define FIRST_INDEX_PARAM_PORT 5000

// redeffinission de max et min car ga.h les #undef...
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

enum
{ VIEW				= 0
,  MAIN				= 1
, PORT				= 2
, SEQVIEW			= 4
, INPUTDIALOG		= 8
, LASERDIALOG		= 16
, CAM				= 32
, TRAITIMAGE		= 64
, FIT				= 128
, GENETIC			= 256
, ACTUALISE			= 128*128*128*128 };

class CMainFrame : public CFrameWnd
{
	
protected: // cr�ation � partir de la s�rialisation uniquement
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributs
public:

// Op�rations
public:

// Substitutions
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Impl�mentation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	CSplitterWnd m_wndSplitter;

public:  // membres incorpor�s de la barre de contr�le
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
//	CRb2View	m_TheView;

public:									
	// fenetre   MainDialog  ,  Port  ,  ProDialog  ,  Seq View Only
	int m_Affichage;						
	int m_AffichagePrevious;						
	int m_AffichagePreviousPrevious;		
	int m_splitterPosition;
	// une instance de CSettingSave pour sauver les parametres qui lui sont attch� lors de la d�claration CParametre(CSettingSave* pInstanceSave, int unType,....etc
	CSettingSave m_Save;				
	// fen�tre Dialog s�quence
	CMainDialog m_MainDialog;			
	// fen�tre Dialog port
	CPortDialog m_PortDialog;

	// fen�tre Cam�ra
//srybex 	CCamDialog m_CamDialog;		

	// fen�tre TraitImage
//srybex 	CTraitImageDialog m_TraitImDialog;		

	// fen�tre Fit
//srybex 	CFitDialog m_FitDialog;		
	
	// fen�tre Input
	CInputDialog m_InputDialog;		
	
	// fen�tre Laser
//srybex 	CLaserDialog m_LaserDialog;		
	
	// fen�tre Algorithme genetique
//srybex 	CGeneticDialog m_GeneticDialog;		
	
	// fen�tre Dialog Pro
	CSeqViewDialog m_SeqViewDialog;		

	// indique que les Carte DAQmx semble marcher dans la mesure ou l'�tape 
	// ValueChangedPortCheckUpdateNow est arriver jusqu'au bout sans "ErrorCatch"
	// Comme ca, sur un ordi qui n'a pas de carte, on peut �viter qq erreurs
	bool m_CarteDAQmxSemblePouvoirMarcher;

// Fonctions g�n�r�es de la table des messages
protected:
	afx_msg void OnSwitchView();

	afx_msg void OnCamMode();
	afx_msg void OnTraitImageMode();
	afx_msg void OnInputMode();
	afx_msg void OnLaserMode();
	afx_msg void OnSeqView();
	afx_msg void OnGeneticView();
	afx_msg void OnMainPort();
	afx_msg void OnPort();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
public:
	void ModeDialogBar(int mode = ACTUALISE);

	DECLARE_MESSAGE_MAP()
public:

	unsigned int DefineAllParameters();

	char m_temp[256];
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnBnClickedButtonSwitchModeManip();
	Sequence Seq1;

	afx_msg void OnClose();
	afx_msg void OnExitWithoutSaving();

	//  change entre les fen�tres
	afx_msg void OnExitwithoutsaving();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveRb2();
	afx_msg void OnFileSaveAsRb2();
	afx_msg void OnFileOpen(CString aFile, int FromParam = -1, int ToParam = -1);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CString FindLastFile(CString PartieDuNomFichier, CString Dossier = "");
	afx_msg void OnSequenceFullsequence();
	afx_msg void OnSequenceMolasses();
	afx_msg void OnStaticportReferencemot();
	afx_msg void OnStaticportReferencemolasses();

	afx_msg void OnPrintMainDialog();
	afx_msg void OnPrintPortDialog();
	afx_msg void OnPrintBothDialog();
	afx_msg void OnPrintSequence();
};

// Le pointeur vers La MainFrame ! ! ! !
extern	CMainFrame* m_pMainFrame;

