// CamDialog.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include ".\seqviewdialog.h"

// CSeqViewDialog

IMPLEMENT_DYNAMIC(CSeqViewDialog, CProDialog)
CSeqViewDialog::CSeqViewDialog()
: m_CurentSeqIndex(1), m_WaitTrigger(0)
{
	p_DAQmxSeqAna1 = NULL;
	p_DAQmxSeqDig = NULL;
	p_MasterTask = NULL;
	p_Slave1Task = NULL;
}
CSeqViewDialog::~CSeqViewDialog()
{
}
BEGIN_MESSAGE_MAP(CSeqViewDialog, CProDialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_SEQVIEW_COMBO_FILES, OnCbnSelchangeSeqviewComboFiles)
//	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL CSeqViewDialog::OnInitDialogBar()
{
	CProDialog::OnInitDialogBar();
	return TRUE;
}

BOOL CSeqViewDialog::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	if (!CProDialog::Create(pParentWnd, nIDTemplate, nStyle, nID))
	{
        return 0; // fail to create
	}
	if (!m_SeqViewLeftDialog.Create(pParentWnd, IDD_SEQ_VIEW_LEFT_DIALOG,CBRS_LEFT|CBRS_TOOLTIPS, IDD_SEQ_VIEW_LEFT_DIALOG))
	{
        return 0; // fail to create
	}
	return 1;
}
//void CSeqViewDialog::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	m_pMainFrame->ShowControlBar(&m_SeqViewLeftDialog, bShow, nStatus);
//	CProDialog::OnShowWindow(bShow, nStatus);
//}

void CSeqViewDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time
	DDX_Control(pDX, IDC_SEQVIEW_LIST_COLONNE, m_ListCol);
	DDX_Control(pDX, IDC_SEQVIEW_START, m_NiButtonStart);
	DDX_Control(pDX, IDC_SEQVIEW_COMBO_FILES, m_Combo_Files);
	DDX_Control(pDX, IDC_SEQVIEW_CHECK_WAITTRIGGER, m_NiButtonWaitTrigger);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
void CSeqViewDialog::OnPaint()
{
	CPaintDC dc(this);
	if(m_bWasPaintedOnce == false)
	{
		m_bWasPaintedOnce = true;
		Sequence::p_ListCtrlColonnes = &m_ListCol;
		Sequence::p_ListCtrlLignesDig = &m_SeqViewLeftDialog.m_ListLigneDig;
		Sequence::p_ListCtrlLignesAna = &m_SeqViewLeftDialog.m_ListLigneAna;
		m_Seq[m_CurentSeqIndex] = m_pMainFrame->m_MainDialog.LaSeqMainDialog;
		// On reforme chaque séquence à partir des paramètres qui la définnissent 
		for(int iemeSeq = 1; iemeSeq <= NBR_SEQUENCE_VIEW; iemeSeq++)
		{
//			m_Seq[iemeSeq].MakeCParametrantFrom_Sequence_AndConstrParam();
		}
		m_View.Invalidate();
	}
	m_pMainFrame->m_MainDialog.pLaSeqUsedForNiCards = &(m_Seq[m_CurentSeqIndex]);
	//m_pMainFrame->m_MainDialog.m_bUseSeqView = true;
	m_pMainFrame->m_MainDialog.m_bVarieActive = false;
}
void CSeqViewDialog::MousseMoved(int WhichView, UINT nFlags, CPoint point)
{
	CRect rectChange;
	switch(WhichView)
	{
	case 1:
		break;
	case 2:
		switch(nFlags)
		{
		case MK_CONTROL:
			// on retrouve la case cliquée
			CPoint AnaCol = m_Seq[m_CurentSeqIndex].Pointed_AnaIndexY_IemeColX(point);
			rectChange = m_Seq[m_CurentSeqIndex].SetAnaContiWithPrevious(AnaCol.x, AnaCol.y, DoNothing);
			break;
		}
		break;
	}
	m_View.InvalidateRect(rectChange);
}
void CSeqViewDialog::MousseLeftDown(int WhichView, UINT nFlags, CPoint point)
{
	CRect rectChange;
	switch(WhichView)
	{
	case 1:
		switch(nFlags)
		{
		case MK_CONTROL + MK_LBUTTON:
			// si c'est sur un digital, ca se charge de faire le switch
			rectChange = m_Seq[m_CurentSeqIndex].ClickedDigitalToSwitch(point);
			break;
		case MK_SHIFT + MK_LBUTTON:
		//	rectChange = SetContinuity(;
			break;
		case MK_SHIFT + MK_CONTROL + MK_LBUTTON:
			MessageBeep(1);
			break;
		default :
			break;
		//	rectChange = StartSelectionDigital(point);
		}
		break;
	case 2:
		rectChange = m_Seq[m_CurentSeqIndex].ClickedAnalogToSet(point);
	}
	m_View.InvalidateRect(rectChange);
}
void CSeqViewDialog::MousseRightDown(int WhichView, UINT nFlags, CPoint point)
{
	CMenu menu; POINT pp;	GetCursorPos(&pp);
	int SelectionMade = 0;
	Sequence& LaSeq = m_Seq[m_CurentSeqIndex];
	CPoint digcol, anacol;

	CMenu* p_popup = NULL;
	switch(WhichView)
	{
	case 1:
		digcol = LaSeq.Pointed_DigIndexY_IemeColX(point);
		if(digcol.x == ERREUR || digcol.y == ERREUR)
			break;
		menu.LoadMenu(IDR_SEQ_VIEW_POPUP_DIG); 
		ASSERT (menu.m_hMenu != NULL);
		p_popup = menu.GetSubMenu(0); 
		//// on check suivant l'état de continuité...
		//bool state = LaSeq.GetDigContiWithPrevious(digcol.x, digcol.x);
		//p_popup->CheckMenuItem(0, state);
		ASSERT (p_popup != NULL);
		SelectionMade = p_popup->TrackPopupMenu(TPM_NONOTIFY | TPM_RETURNCMD, pp.x, pp.y, this);
		break;
	case 2:
		anacol = LaSeq.Pointed_AnaIndexY_IemeColX(point);
		if(anacol.x == ERREUR || anacol.y == ERREUR)
			break;
		menu.LoadMenu(IDR_SEQ_VIEW_POPUP_ANA); 
		ASSERT (menu.m_hMenu != NULL);
		p_popup = menu.GetSubMenu(0); 
		//// on check suivant l'état de continuité...
		//bool state = LaSeq.GetDigContiWithPrevious(digcol.x, digcol.x);
		//p_popup->CheckMenuItem(0, state);
		ASSERT (p_popup != NULL);
		SelectionMade = p_popup->TrackPopupMenu(TPM_NONOTIFY | TPM_RETURNCMD, pp.x, pp.y, this);
		break;
	}

	// Traitons la selection
	int OldStateConti = 0;
	CRect redess(0,0,0,0);
	switch(SelectionMade)
	{
	case ID_DIG_CONTINUITYWITHPREVIOUS:
		OldStateConti = LaSeq.GetDigContiWithPrevious(digcol.x, digcol.y);
		redess = LaSeq.SetDigContiWithPrevious(digcol.x, digcol.y, !OldStateConti);
		m_View.InvalidateRect(redess);
		break;
	case ID_ANA_CONTINUITYWITHPREVIOUS_NOCONTINUITY:
		OldStateConti = LaSeq.GetAnaContiWithPrevious(anacol.x, anacol.y);
		redess = LaSeq.SetAnaContiWithPrevious(anacol.x, anacol.y, NoConti);
		m_View2.InvalidateRect(redess);
		break;
	case ID_ANA_CONTINUITYWITHPREVIOUS_CONTINUITY:
		OldStateConti = LaSeq.GetAnaContiWithPrevious(anacol.x, anacol.y);
		redess = LaSeq.SetAnaContiWithPrevious(anacol.x, anacol.y, ContinueAGauche);
		m_View2.InvalidateRect(redess);
		break;
	case ID_ANA_CONTINUITYWITHPREVIOUS_SLOPECONTINUITY:
		OldStateConti = LaSeq.GetAnaContiWithPrevious(anacol.x, anacol.y);
		redess = LaSeq.SetAnaContiWithPrevious(anacol.x, anacol.y, SlopeContinueAGauche);
		m_View2.InvalidateRect(redess);
		break;
	case ID_ANA_COMPLEXWAVEFORM:
		LaSeq.GetColonne(anacol.x)->m_AnaChannel[anacol.y]->SetGraphically();
		m_View2.InvalidateRect(redess);
		break;
	}
		//Agir en fonction du résultat retourné.
	//0 indique que le menu a été annulé
	//Sinon, comparer le resultat avec les identifiants des différentes options
	menu.DestroyMenu();
}

BEGIN_EVENTSINK_MAP(CSeqViewDialog, CProDialog)
	ON_EVENT(CSeqViewDialog, IDC_SEQVIEW_START, 1, ValueChangedSeqviewStart, VTS_BOOL)
	ON_EVENT(CSeqViewDialog, IDC_SEQVIEW_CHECK_WAITTRIGGER, 1, ValueChangedSeqviewCheckWaittrigger, VTS_BOOL)
END_EVENTSINK_MAP()

void CSeqViewDialog::ValueChangedSeqviewCheckWaittrigger(BOOL Value)
{
	m_WaitTrigger = Value ? 1 : 0;
	if(m_WaitTrigger)
	{
		m_NiButtonWaitTrigger.SetOnText("0 Trig Received");
	}
}

void CSeqViewDialog::ValueChangedSeqviewStart(BOOL Value)
{	int value = Value;
	// Load the hourglass cursor
	HCURSOR lhCursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);	SetCursor(lhCursor);

	Sequence& LaSeq = m_Seq[m_CurentSeqIndex];
	if(value == TRUE)
	{
		PrepareNiCards();
		PrepareSeqAndNiBuffer(LaSeq);

		p_DAQmxSeqDig->SendNiMatrixDataToTheCard(	m_NiBufferDig	);
		p_DAQmxSeqAna1->SendNiMatrixDataToTheCard(	m_NiBufferAna1	);

		p_Slave1Task->Start();	// FIRST the Slaves 
		p_MasterTask->Start();	// THEN the Master	
	}
	if(value == FALSE)
	{
		if(p_DAQmxSeqAna1)
			p_DAQmxSeqAna1->GetTask().Stop();
		if(p_DAQmxSeqDig)
			p_DAQmxSeqDig->GetTask().Stop();

		FreeNiCards();	
	}
	// Load the hourglass cursor
	lhCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);	SetCursor(lhCursor);
}

int CSeqViewDialog::PrepareNiCards()
{
	p_DAQmxSeqDig	= new CDAQmxDynamicCardDigital32();
	p_DAQmxSeqAna1	= new CDAQmxDynamicCardAnalog1();
	p_DAQmxSeqAna2	= new CDAQmxDynamicCardAnalog2();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//" synchronisation des montres ! "											//
// from : C:\Program Files\National Instruments\MeasurementStudioVS2003\VCNET\Examples\DAQmx\Synchronization\Multi-Device\AIFiniteAcquisition
	// maitre																	//
	p_MasterTask = &p_DAQmxSeqAna1->GetTask();						//
	// esclave																	//
	p_Slave1Task = &p_DAQmxSeqDig->GetTask();						//
																					//
	// First, verify the master task so we can query its properties.				//
	p_MasterTask->Control(DAQmxTaskVerify);												//
	// Verify the slave task so we can query its properties.						//
	p_Slave1Task->Control(DAQmxTaskVerify);												//
	// Next, find out what device the master is using.								//
	// This is so we can build terminal strings using the							//
	// master device name.															//
	CString AOfirstPhysChanName = p_MasterTask->AOChannels[0].PhysicalName;				//
	CString AOdeviceName = AOfirstPhysChanName.Left(AOfirstPhysChanName.Find(_T('/'))+1);//
	CString AOterminalNameBase = CString(_T("/")) + AOdeviceName;					//
	// slave device name.															//
	CString DOfirstPhysChanName = p_Slave1Task->DOChannels[0].PhysicalName;				//
	CString DOdeviceName = DOfirstPhysChanName.Left(DOfirstPhysChanName.Find(_T('/'))+1);//
	CString DOterminalNameBase = CString(_T("/")) + DOdeviceName;					//
																					//
	// E-Series Synchronization														//
	p_Slave1Task->Timing.MasterTimebaseSource = p_MasterTask->Timing.MasterTimebaseSource;		//
	p_Slave1Task->Timing.MasterTimebaseRate   = p_MasterTask->Timing.MasterTimebaseRate;			//
																					//
	if(m_WaitTrigger)
	{
		// Configure a digital edge start trigger so the master task				//
		// start on an external trigger.											//
		p_MasterTask->Triggers.StartTrigger.ConfigureDigitalEdgeTrigger(					//
			DOterminalNameBase + _T("PFI6"),// correspond à "ACK1 input"		//
			DAQmxDigitalEdgeStartTriggerEdgeRising);								//
		p_MasterTask->Triggers.PauseTrigger.ConfigureDigitalLevelTrigger(					//
			DOterminalNameBase + _T("PFI2"),// correspond à "REQ1 input"		//
			DAQmxDigitalLevelPauseTriggerConditionHigh);							//
	}
																					//
	// Configure a digital edge start trigger so both tasks							//
	// start together: the slave is triggered by the p_MasterTask					//
	p_Slave1Task->Triggers.StartTrigger.ConfigureDigitalEdgeTrigger(				//
		AOterminalNameBase + _T("ao/StartTrigger"),									//
		DAQmxDigitalEdgeStartTriggerEdgeRising);									//
	if(m_WaitTrigger)
	{
		p_Slave1Task->Triggers.PauseTrigger.ConfigureDigitalLevelTrigger(					//
			AOterminalNameBase + _T("ao/PauseTrigger"),								//
			DAQmxDigitalLevelPauseTriggerConditionHigh);							//
	}

	p_MasterTask->Control(DAQmxTaskVerify);												//
	p_Slave1Task->Control(DAQmxTaskVerify);												//

	p_MasterTask->Control(DAQmxTaskCommit);												//
	p_Slave1Task->Control(DAQmxTaskCommit);												//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
return 1;
}
int CSeqViewDialog::PrepareSeqAndNiBuffer(Sequence& LaSeq)
{
	LaSeq.MakeDigBuffer();
	LaSeq.MakeAnaBuffer();
	// les infos relatives a la sequence seront affichées dans la barre de status
	CString InfoSeq;
	InfoSeq.Format(_T("Sample Rate: %.3g/s   Total Duration: %dms"), Sequence::m_SampleRate, LaSeq.m_nPeriodes*1000/Sequence::m_SampleRate);
	InfoSeq.Replace("e+0","E");InfoSeq.Replace("E0","E");InfoSeq.Replace("E0","E");
	m_pMainFrame->m_wndStatusBar.SetPaneText(0, InfoSeq);
	m_NiBufferDig = p_DAQmxSeqDig->GenerateNiMatrixDataForTheCard(LaSeq.GetDigBuffer(), LaSeq.m_nPeriodes);
	m_NiBufferAna1 = p_DAQmxSeqAna1->GenerateNiMatrixDataForTheCard(LaSeq.GetAnaBuffer(), LaSeq.m_nPeriodes);
	
	//////////////////////////  Ici commencent les histoires de				////////////////////////
	//////////////////////////  Start, Wait, Repete,... de la sequence		////////////////////////

	// si on attend un trigger, on ne fait tourner la séquence qu'une seule fois
	if(m_WaitTrigger && !LaSeq.m_nRepetition)
	{
		p_Slave1Task->Configure(LaSeq.m_nPeriodes * 1);
		p_MasterTask->Configure(LaSeq.m_nPeriodes * 1);
	}
	else if( LaSeq.m_nRepetition )
	{
		p_Slave1Task->Configure(LaSeq.m_nPeriodes * LaSeq.m_nRepetition);
		p_MasterTask->Configure(LaSeq.m_nPeriodes * LaSeq.m_nRepetition);
	}
return 1;
}
int CSeqViewDialog::FreeNiCards()
{
	if(p_DAQmxSeqDig)
	{
		delete p_DAQmxSeqDig;
		p_DAQmxSeqDig = NULL;
	}
	if(p_DAQmxSeqAna1)
	{
		delete p_DAQmxSeqAna1;
		p_DAQmxSeqAna1 = NULL;
	}
return 1;
}
int CSeqViewDialog::PrepareTriggerStuffs(Sequence& LaSeq)
{

return 1;
}

// ColonneListCtrl

IMPLEMENT_DYNAMIC(ColonneListCtrl, CListCtrl)
ColonneListCtrl::ColonneListCtrl()
{
}

ColonneListCtrl::~ColonneListCtrl()
{
}


BEGIN_MESSAGE_MAP(ColonneListCtrl, CListCtrl)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, OnHdnBegintrack)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// Gestionnaires de messages ColonneListCtrl
BOOL ColonneListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY   *pHDN = (HD_NOTIFY*)lParam;
	if(pHDN->hdr.code == HDN_BEGINTRACKW || pHDN->hdr.code == HDN_BEGINTRACKA || pHDN->hdr.code == HDN_DIVIDERDBLCLICKW || pHDN->hdr.code == HDN_DIVIDERDBLCLICKA)
	{
		*pResult = TRUE;   
		return TRUE;          
	}
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}


void ColonneListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;
}
void ColonneListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_View.OnHScroll(nSBCode, nPos, pScrollBar);
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSeqViewDialog::OnCbnSelchangeSeqviewComboFiles()
{
	m_Seq[m_CurentSeqIndex].LoadNewSequenceFromFile();
	m_View.Invalidate();
return;
}

// Boîte de dialogue CSeqViewLeftDialog
IMPLEMENT_DYNAMIC(CSeqViewLeftDialog, CProDialog)
CSeqViewLeftDialog::CSeqViewLeftDialog()
{
}

CSeqViewLeftDialog::~CSeqViewLeftDialog()
{
}


BOOL CSeqViewLeftDialog::OnInitDialogBar()
{
	CProDialog::OnInitDialogBar();
	return TRUE;
}
void CSeqViewLeftDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time
	DDX_Control(pDX, IDC_SEQVIEW_LIST_LIGNE_DIG, m_ListLigneDig);
	DDX_Control(pDX, IDC_SEQVIEW_LIST_LIGNE_ANA, m_ListLigneAna);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CSeqViewLeftDialog, CProDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages CSeqViewLeftDialog


