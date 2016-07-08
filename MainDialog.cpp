// MainDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainDialog.h"
#include "MainFrm.h"
//srybex #include "SMTPConnection2.h"
#include "DialogVariation.h"
#include "DialogFilmColonnes.h"
//#include "NewPortXPS.h"

// CMainDialog

IMPLEMENT_DYNAMIC(CMainDialog, CProDialog)
CMainDialog::CMainDialog()
:	m_bUpdateAuto(true), m_bUseSeqView(false), m_WaitTrigger(0), m_WaitBeforeLoad(0), m_Repete(0), m_bIsRunning(0), m_bVarieActive(0), m_IsDoingVariation(0)
{
	m_ParamBool[1] = (1); m_ParamBool[2] = (0); m_ParamBool[3] = (1); m_ParamBool[4] = (1); m_ParamBool[5] = (1);
	m_ParamBool[6] = (0); m_ParamBool[7] = (0); m_ParamBool[8] = (0); m_ParamBool[10] = (0); m_ParamBool[11] = (0);
	m_ParamBool[12] = (0); m_ParamBool[13] = (0); m_ParamBool[14] = (0); m_ParamBool[15] = (0); m_ParamBool[16] = (0); m_ParamBool[17] = (0);
	m_ParamReal[1] = (1000) ;m_ParamReal[2] = (85); m_ParamReal[3] = (15); m_ParamReal[4] = (50); m_ParamReal[5] = (1000);
	m_ParamReal[6] = (9000); m_ParamReal[7] = (1000); m_ParamReal[8] = (1000); m_ParamReal[9] = (0); m_ParamReal[26] = (5); m_ParamReal[27] = (1); m_ParamReal[10] = (10); m_ParamReal[11] = (1000);
	m_ParamReal[12] = (1000); m_ParamReal[13] = (1000); m_ParamReal[14] = (10);
	m_ParamReal[15] = (1000); m_ParamReal[16] = (1000); m_ParamReal[17] = (1000); m_ParamReal[18] = (10);
	m_ParamReal[19] = (10); m_ParamReal[20] = (10); m_ParamReal[21] = (1000); m_ParamReal[22] = (10);
	m_ParamReal[23] = (10); m_ParamReal[24] = (10); m_ParamReal[28] = (6600);
	m_ParamBool[19] = (true); m_ParamBool[20] = (true);
	m_ParamBool[21] = (true); m_ParamBool[22] = (true); m_ParamBool[23] = (true);
	m_ParamBool[24] = (true); m_ParamBool[25] = (false); m_ParamBool[26] = (true);
	m_ParamBool[27] = (true); m_ParamBool[28] = (true); m_ParamBool[29] = (true); m_ParamBool[30] = (true);
	m_FilmFromColName = "";
	m_FilmToColName = "";
	m_FilmStep_ms = 0;
	m_FilmDuree_ms = 0;
p_DigitalCodeMainDialog = NULL;
p_Analog1CodeMainDialog = NULL;
p_Analog2CodeMainDialog = NULL;
m_NbrComboSampleRate = 0;
pLaSeqView = NULL;
}

CMainDialog::~CMainDialog()
{
}

BOOL CMainDialog::OnInitDialogBar()
{
	// Support for DDX mechanism
	// If you do not want DDX then
	// do not call base class
	CProDialog::OnInitDialogBar();

	// Update any controls NOT supported by DDX
	// CBitmapButton is one
//	m_OKButtton.AutoLoad(IDOK, this);

	return TRUE;
}
void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
//	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_MAIN_COMBO_SAMPLE_RATE		, m_ComboSampleRate);
	DDX_Control(pDX, IDC_MAIN_CHECK_WAIT_TRIGGER	, m_NiButtonWaitTrigger);
	DDX_Control(pDX, IDC_MAIN_EDIT_WAIT_BEFORE_LOAD	, m_NiEditWaitBeforeLoad);
															DDX_Control(pDX, IDC_MAIN_EDIT_01	, m_NiEdit[ 1]);	DDX_Control(pDX, IDC_MAIN_EDIT_02	, m_NiEdit[ 2]);	DDX_Control(pDX, IDC_MAIN_EDIT_03	, m_NiEdit[ 3]);	DDX_Control(pDX, IDC_MAIN_EDIT_04	, m_NiEdit[ 4]);	DDX_Control(pDX, IDC_MAIN_EDIT_05	, m_NiEdit[ 5]);	DDX_Control(pDX, IDC_MAIN_EDIT_06	, m_NiEdit[ 6]);	DDX_Control(pDX, IDC_MAIN_EDIT_07	, m_NiEdit[ 7]);	DDX_Control(pDX, IDC_MAIN_EDIT_08	, m_NiEdit[ 8]);	DDX_Control(pDX, IDC_MAIN_EDIT_09	, m_NiEdit[ 9]);
	DDX_Control(pDX, IDC_MAIN_EDIT_10	, m_NiEdit[10]);	DDX_Control(pDX, IDC_MAIN_EDIT_11	, m_NiEdit[11]);	DDX_Control(pDX, IDC_MAIN_EDIT_12	, m_NiEdit[12]);	DDX_Control(pDX, IDC_MAIN_EDIT_13	, m_NiEdit[13]);	DDX_Control(pDX, IDC_MAIN_EDIT_14	, m_NiEdit[14]);	DDX_Control(pDX, IDC_MAIN_EDIT_15	, m_NiEdit[15]);	DDX_Control(pDX, IDC_MAIN_EDIT_16	, m_NiEdit[16]);	DDX_Control(pDX, IDC_MAIN_EDIT_17	, m_NiEdit[17]);	DDX_Control(pDX, IDC_MAIN_EDIT_18	, m_NiEdit[18]);	DDX_Control(pDX, IDC_MAIN_EDIT_19	, m_NiEdit[19]);
	DDX_Control(pDX, IDC_MAIN_EDIT_20	, m_NiEdit[20]);	DDX_Control(pDX, IDC_MAIN_EDIT_21	, m_NiEdit[21]);	DDX_Control(pDX, IDC_MAIN_EDIT_22	, m_NiEdit[22]);	DDX_Control(pDX, IDC_MAIN_EDIT_23	, m_NiEdit[23]);	DDX_Control(pDX, IDC_MAIN_EDIT_24	, m_NiEdit[24]);	DDX_Control(pDX, IDC_MAIN_EDIT_25	, m_NiEdit[25]);	DDX_Control(pDX, IDC_MAIN_EDIT_26	, m_NiEdit[26]);	DDX_Control(pDX, IDC_MAIN_EDIT_27	, m_NiEdit[27]);	DDX_Control(pDX, IDC_MAIN_EDIT_28	, m_NiEdit[28]);	DDX_Control(pDX, IDC_MAIN_EDIT_29	, m_NiEdit[29]);
	DDX_Control(pDX, IDC_MAIN_EDIT_30	, m_NiEdit[30]);	DDX_Control(pDX, IDC_MAIN_EDIT_31	, m_NiEdit[31]);	DDX_Control(pDX, IDC_MAIN_EDIT_32	, m_NiEdit[32]);	DDX_Control(pDX, IDC_MAIN_EDIT_33	, m_NiEdit[33]);	DDX_Control(pDX, IDC_MAIN_EDIT_34	, m_NiEdit[34]);	DDX_Control(pDX, IDC_MAIN_EDIT_35	, m_NiEdit[35]);	DDX_Control(pDX, IDC_MAIN_EDIT_36	, m_NiEdit[36]);	DDX_Control(pDX, IDC_MAIN_EDIT_37	, m_NiEdit[37]);	DDX_Control(pDX, IDC_MAIN_EDIT_38	, m_NiEdit[38]);	DDX_Control(pDX, IDC_MAIN_EDIT_39	, m_NiEdit[39]);
	DDX_Control(pDX, IDC_MAIN_EDIT_40	, m_NiEdit[40]);	DDX_Control(pDX, IDC_MAIN_EDIT_41	, m_NiEdit[41]);	DDX_Control(pDX, IDC_MAIN_EDIT_42	, m_NiEdit[42]);	DDX_Control(pDX, IDC_MAIN_EDIT_43	, m_NiEdit[43]);	DDX_Control(pDX, IDC_MAIN_EDIT_44	, m_NiEdit[44]);	DDX_Control(pDX, IDC_MAIN_EDIT_45	, m_NiEdit[45]);	DDX_Control(pDX, IDC_MAIN_EDIT_46	, m_NiEdit[46]);	DDX_Control(pDX, IDC_MAIN_EDIT_47	, m_NiEdit[47]);	DDX_Control(pDX, IDC_MAIN_EDIT_48	, m_NiEdit[48]);	DDX_Control(pDX, IDC_MAIN_EDIT_49	, m_NiEdit[49]);
	DDX_Control(pDX, IDC_MAIN_EDIT_50	, m_NiEdit[50]);	DDX_Control(pDX, IDC_MAIN_EDIT_51	, m_NiEdit[51]);	DDX_Control(pDX, IDC_MAIN_EDIT_52	, m_NiEdit[52]);	DDX_Control(pDX, IDC_MAIN_EDIT_53	, m_NiEdit[53]);	DDX_Control(pDX, IDC_MAIN_EDIT_54	, m_NiEdit[54]);	DDX_Control(pDX, IDC_MAIN_EDIT_55	, m_NiEdit[55]);	DDX_Control(pDX, IDC_MAIN_EDIT_56	, m_NiEdit[56]);	DDX_Control(pDX, IDC_MAIN_EDIT_57	, m_NiEdit[57]);	DDX_Control(pDX, IDC_MAIN_EDIT_58	, m_NiEdit[58]);	DDX_Control(pDX, IDC_MAIN_EDIT_59	, m_NiEdit[59]);
	DDX_Control(pDX, IDC_MAIN_EDIT_60	, m_NiEdit[60]);	DDX_Control(pDX, IDC_MAIN_EDIT_61	, m_NiEdit[61]);	DDX_Control(pDX, IDC_MAIN_EDIT_62	, m_NiEdit[62]);	DDX_Control(pDX, IDC_MAIN_EDIT_63	, m_NiEdit[63]);	DDX_Control(pDX, IDC_MAIN_EDIT_64	, m_NiEdit[64]);	DDX_Control(pDX, IDC_MAIN_EDIT_65	, m_NiEdit[65]);	DDX_Control(pDX, IDC_MAIN_EDIT_66	, m_NiEdit[66]);	DDX_Control(pDX, IDC_MAIN_EDIT_67	, m_NiEdit[67]);	DDX_Control(pDX, IDC_MAIN_EDIT_68	, m_NiEdit[68]);	DDX_Control(pDX, IDC_MAIN_EDIT_69	, m_NiEdit[69]);
	DDX_Control(pDX, IDC_MAIN_EDIT_70	, m_NiEdit[70]);	DDX_Control(pDX, IDC_MAIN_EDIT_71	, m_NiEdit[71]);	DDX_Control(pDX, IDC_MAIN_EDIT_72	, m_NiEdit[72]);	DDX_Control(pDX, IDC_MAIN_EDIT_73	, m_NiEdit[73]);	DDX_Control(pDX, IDC_MAIN_EDIT_74	, m_NiEdit[74]);	DDX_Control(pDX, IDC_MAIN_EDIT_75	, m_NiEdit[75]);	DDX_Control(pDX, IDC_MAIN_EDIT_76	, m_NiEdit[76]);	DDX_Control(pDX, IDC_MAIN_EDIT_77	, m_NiEdit[77]);	DDX_Control(pDX, IDC_MAIN_EDIT_78	, m_NiEdit[78]);	DDX_Control(pDX, IDC_MAIN_EDIT_79	, m_NiEdit[79]);
	DDX_Control(pDX, IDC_MAIN_EDIT_80	, m_NiEdit[80]);	DDX_Control(pDX, IDC_MAIN_EDIT_81	, m_NiEdit[81]);	DDX_Control(pDX, IDC_MAIN_EDIT_82	, m_NiEdit[82]);	DDX_Control(pDX, IDC_MAIN_EDIT_83	, m_NiEdit[83]);	DDX_Control(pDX, IDC_MAIN_EDIT_84	, m_NiEdit[84]);	DDX_Control(pDX, IDC_MAIN_EDIT_85	, m_NiEdit[85]);	DDX_Control(pDX, IDC_MAIN_EDIT_86	, m_NiEdit[86]);	DDX_Control(pDX, IDC_MAIN_EDIT_87	, m_NiEdit[87]);	DDX_Control(pDX, IDC_MAIN_EDIT_88	, m_NiEdit[88]);	DDX_Control(pDX, IDC_MAIN_EDIT_89	, m_NiEdit[89]);
//	DDX_Control(pDX, IDC_MAIN_EDIT_90	, m_NiEdit[90]);	DDX_Control(pDX, IDC_MAIN_EDIT_91	, m_NiEdit[91]);	DDX_Control(pDX, IDC_MAIN_EDIT_92	, m_NiEdit[92]);	DDX_Control(pDX, IDC_MAIN_EDIT_93	, m_NiEdit[93]);	DDX_Control(pDX, IDC_MAIN_EDIT_94	, m_NiEdit[94]);	DDX_Control(pDX, IDC_MAIN_EDIT_95	, m_NiEdit[95]);	DDX_Control(pDX, IDC_MAIN_EDIT_96	, m_NiEdit[96]);	DDX_Control(pDX, IDC_MAIN_EDIT_97	, m_NiEdit[97]);	DDX_Control(pDX, IDC_MAIN_EDIT_98	, m_NiEdit[98]);	DDX_Control(pDX, IDC_MAIN_EDIT_99	, m_NiEdit[99]);
															DDX_Control(pDX, IDC_MAIN_CHECK_01	, m_NiCheck[ 1]);	DDX_Control(pDX, IDC_MAIN_CHECK_02	, m_NiCheck[ 2]);	DDX_Control(pDX, IDC_MAIN_CHECK_03	, m_NiCheck[ 3]);	DDX_Control(pDX, IDC_MAIN_CHECK_04	, m_NiCheck[ 4]);	DDX_Control(pDX, IDC_MAIN_CHECK_05	, m_NiCheck[ 5]);	DDX_Control(pDX, IDC_MAIN_CHECK_06	, m_NiCheck[ 6]);	DDX_Control(pDX, IDC_MAIN_CHECK_07	, m_NiCheck[ 7]);	DDX_Control(pDX, IDC_MAIN_CHECK_08	, m_NiCheck[ 8]);	DDX_Control(pDX, IDC_MAIN_CHECK_09	, m_NiCheck[ 9]);
	DDX_Control(pDX, IDC_MAIN_CHECK_10	, m_NiCheck[10]);	DDX_Control(pDX, IDC_MAIN_CHECK_11	, m_NiCheck[11]);	DDX_Control(pDX, IDC_MAIN_CHECK_12	, m_NiCheck[12]);	DDX_Control(pDX, IDC_MAIN_CHECK_13	, m_NiCheck[13]);	DDX_Control(pDX, IDC_MAIN_CHECK_14	, m_NiCheck[14]);	DDX_Control(pDX, IDC_MAIN_CHECK_15	, m_NiCheck[15]);	DDX_Control(pDX, IDC_MAIN_CHECK_16	, m_NiCheck[16]);	DDX_Control(pDX, IDC_MAIN_CHECK_17	, m_NiCheck[17]);	DDX_Control(pDX, IDC_MAIN_CHECK_18	, m_NiCheck[18]);	DDX_Control(pDX, IDC_MAIN_CHECK_19	, m_NiCheck[19]);
	DDX_Control(pDX, IDC_MAIN_CHECK_20	, m_NiCheck[20]);	DDX_Control(pDX, IDC_MAIN_CHECK_21	, m_NiCheck[21]);	DDX_Control(pDX, IDC_MAIN_CHECK_22	, m_NiCheck[22]);	DDX_Control(pDX, IDC_MAIN_CHECK_23	, m_NiCheck[23]);	DDX_Control(pDX, IDC_MAIN_CHECK_24	, m_NiCheck[24]);	DDX_Control(pDX, IDC_MAIN_CHECK_25	, m_NiCheck[25]);	DDX_Control(pDX, IDC_MAIN_CHECK_26	, m_NiCheck[26]);	DDX_Control(pDX, IDC_MAIN_CHECK_27	, m_NiCheck[27]);	DDX_Control(pDX, IDC_MAIN_CHECK_28	, m_NiCheck[28]);	DDX_Control(pDX, IDC_MAIN_CHECK_29	, m_NiCheck[29]);
	DDX_Control(pDX, IDC_MAIN_CHECK_30	, m_NiCheck[30]);	DDX_Control(pDX, IDC_MAIN_CHECK_31	, m_NiCheck[31]);	DDX_Control(pDX, IDC_MAIN_CHECK_32	, m_NiCheck[32]);	DDX_Control(pDX, IDC_MAIN_CHECK_33	, m_NiCheck[33]);	DDX_Control(pDX, IDC_MAIN_CHECK_34	, m_NiCheck[34]);	DDX_Control(pDX, IDC_MAIN_CHECK_35	, m_NiCheck[35]);	DDX_Control(pDX, IDC_MAIN_CHECK_36	, m_NiCheck[36]);	DDX_Control(pDX, IDC_MAIN_CHECK_37	, m_NiCheck[37]);	DDX_Control(pDX, IDC_MAIN_CHECK_38	, m_NiCheck[38]);	DDX_Control(pDX, IDC_MAIN_CHECK_39	, m_NiCheck[39]);
	DDX_Control(pDX, IDC_MAIN_CHECK_40	, m_NiCheck[40]);	DDX_Control(pDX, IDC_MAIN_CHECK_41	, m_NiCheck[41]);	DDX_Control(pDX, IDC_MAIN_CHECK_42	, m_NiCheck[42]);	DDX_Control(pDX, IDC_MAIN_CHECK_43	, m_NiCheck[43]);	DDX_Control(pDX, IDC_MAIN_CHECK_44	, m_NiCheck[44]);	DDX_Control(pDX, IDC_MAIN_CHECK_45	, m_NiCheck[45]);	DDX_Control(pDX, IDC_MAIN_CHECK_46	, m_NiCheck[46]);	DDX_Control(pDX, IDC_MAIN_CHECK_47	, m_NiCheck[47]);	DDX_Control(pDX, IDC_MAIN_CHECK_48	, m_NiCheck[48]);	DDX_Control(pDX, IDC_MAIN_CHECK_49	, m_NiCheck[49]);
//	DDX_Control(pDX, IDC_MAIN_CHECK_50	, m_NiCheck[50]);	DDX_Control(pDX, IDC_MAIN_CHECK_51	, m_NiCheck[51]);	DDX_Control(pDX, IDC_MAIN_CHECK_52	, m_NiCheck[52]);	DDX_Control(pDX, IDC_MAIN_CHECK_53	, m_NiCheck[53]);	DDX_Control(pDX, IDC_MAIN_CHECK_54	, m_NiCheck[54]);	DDX_Control(pDX, IDC_MAIN_CHECK_55	, m_NiCheck[55]);	DDX_Control(pDX, IDC_MAIN_CHECK_56	, m_NiCheck[56]);	DDX_Control(pDX, IDC_MAIN_CHECK_57	, m_NiCheck[57]);	DDX_Control(pDX, IDC_MAIN_CHECK_58	, m_NiCheck[58]);	DDX_Control(pDX, IDC_MAIN_CHECK_59	, m_NiCheck[59]);
//	DDX_Control(pDX, IDC_MAIN_CHECK_60	, m_NiCheck[60]);	DDX_Control(pDX, IDC_MAIN_CHECK_61	, m_NiCheck[61]);	DDX_Control(pDX, IDC_MAIN_CHECK_62	, m_NiCheck[62]);	DDX_Control(pDX, IDC_MAIN_CHECK_63	, m_NiCheck[63]);	DDX_Control(pDX, IDC_MAIN_CHECK_64	, m_NiCheck[64]);	DDX_Control(pDX, IDC_MAIN_CHECK_65	, m_NiCheck[65]);	DDX_Control(pDX, IDC_MAIN_CHECK_66	, m_NiCheck[66]);	DDX_Control(pDX, IDC_MAIN_CHECK_67	, m_NiCheck[67]);	DDX_Control(pDX, IDC_MAIN_CHECK_68	, m_NiCheck[68]);	DDX_Control(pDX, IDC_MAIN_CHECK_69	, m_NiCheck[69]);
//	DDX_Control(pDX, IDC_MAIN_CHECK_70	, m_NiCheck[70]);	DDX_Control(pDX, IDC_MAIN_CHECK_71	, m_NiCheck[71]);	DDX_Control(pDX, IDC_MAIN_CHECK_72	, m_NiCheck[72]);	DDX_Control(pDX, IDC_MAIN_CHECK_73	, m_NiCheck[73]);	DDX_Control(pDX, IDC_MAIN_CHECK_74	, m_NiCheck[74]);	DDX_Control(pDX, IDC_MAIN_CHECK_75	, m_NiCheck[75]);	DDX_Control(pDX, IDC_MAIN_CHECK_76	, m_NiCheck[76]);	DDX_Control(pDX, IDC_MAIN_CHECK_77	, m_NiCheck[77]);	DDX_Control(pDX, IDC_MAIN_CHECK_78	, m_NiCheck[78]);	DDX_Control(pDX, IDC_MAIN_CHECK_79	, m_NiCheck[79]);
//	DDX_Control(pDX, IDC_MAIN_CHECK_80	, m_NiCheck[80]);	DDX_Control(pDX, IDC_MAIN_CHECK_81	, m_NiCheck[81]);	DDX_Control(pDX, IDC_MAIN_CHECK_82	, m_NiCheck[82]);	DDX_Control(pDX, IDC_MAIN_CHECK_83	, m_NiCheck[83]);	DDX_Control(pDX, IDC_MAIN_CHECK_84	, m_NiCheck[84]);	DDX_Control(pDX, IDC_MAIN_CHECK_85	, m_NiCheck[85]);	DDX_Control(pDX, IDC_MAIN_CHECK_86	, m_NiCheck[86]);	DDX_Control(pDX, IDC_MAIN_CHECK_87	, m_NiCheck[87]);	DDX_Control(pDX, IDC_MAIN_CHECK_88	, m_NiCheck[88]);	DDX_Control(pDX, IDC_MAIN_CHECK_89	, m_NiCheck[89]);
//	DDX_Control(pDX, IDC_MAIN_CHECK_90	, m_NiCheck[90]);	DDX_Control(pDX, IDC_MAIN_CHECK_91	, m_NiCheck[91]);	DDX_Control(pDX, IDC_MAIN_CHECK_92	, m_NiCheck[92]);	DDX_Control(pDX, IDC_MAIN_CHECK_93	, m_NiCheck[93]);	DDX_Control(pDX, IDC_MAIN_CHECK_94	, m_NiCheck[94]);	DDX_Control(pDX, IDC_MAIN_CHECK_95	, m_NiCheck[95]);	DDX_Control(pDX, IDC_MAIN_CHECK_96	, m_NiCheck[96]);	DDX_Control(pDX, IDC_MAIN_CHECK_97	, m_NiCheck[97]);	DDX_Control(pDX, IDC_MAIN_CHECK_98	, m_NiCheck[98]);	DDX_Control(pDX, IDC_MAIN_CHECK_99	, m_NiCheck[99]);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDialog, CProDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_MAIN_RADIO_MODE_REPET_1, OnBnClickedRadioModeRepet1)
	ON_BN_CLICKED(IDC_MAIN_RADIO_MODE_REPET_2, OnBnClickedRadioModeRepet2)
	ON_BN_CLICKED(IDC_MAIN_RADIO_MODE_REPET_3, OnBnClickedRadioModeRepet3)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_MAIN_COMBO_SAMPLE_RATE, OnCbnSelchangeMainComboSampleRate)
//	ON_WM_KEYDOWN()
//	ON_WM_CHAR()
//ON_WM_CHAR()
END_MESSAGE_MAP()


void CMainDialog::OnPaint()
{
	CPaintDC dc(this);	// astuce : on ne peut initaliser les valeur de m_MainDialog
						// qu'une fois celle ci construite. alors, on profite du premier message WM_PAINT pour le faire
	if(!m_bWasPaintedOnce)
	{	
		m_bWasPaintedOnce = true;
		m_pMainFrame->m_PortDialog.UpdateControls();

		// select le bon m_SampleRate
		m_ComboSampleRate.SetCurSel( m_NbrComboSampleRate );
		OnCbnSelchangeMainComboSampleRate();

		ProgressBars[1] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_01 );
		ProgressBars[2] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_02 );
		ProgressBars[3] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_03 );
		ProgressBars[4] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_04 );
		ProgressBars[5] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_05 );


		OnBnClickedRadioModeRepet3();
		m_bIsRunning = true; // empeche juste de faire la séquence à chaque changement de control
		m_ParamBool[1] = 1;	m_ParamReal[2] = max(1,m_ParamReal[2]);
		UpdateControls();	
		m_bIsRunning = false; //.................................................................
		MakeSequence(&LaSeqMainDialog);
		m_View.Invalidate();

		FOR_ALL_PARAMETERS_DEFINED_do
			if(pLeParam && pLeParam->Varie)
			{
				LaSeqMainDialog.m_LaVariation.AddParametre(pLeParam);
				OnBnClickedRadioModeRepet1();
				CheckDlgButton(IDC_MAIN_CHECK_VAR_00, true);
			}
		}
		UpdateCheckBoxesVarie();	
		UpdateStaticText();

		// Voila un truc qui envoye un WM_TIMER event toute les xxxx milisecondes
		SetTimer(ID_CLOCK_TIMER_MAIN_DIALOG, PERIODE_TIMER_SCAN_PARAM, NULL);
	}
}
BOOL CMainDialog::OnEraseBkgnd(CDC* pDC)
{
	return CProDialog::OnEraseBkgnd(pDC);
}

// Gestion de tous les controls de la MaindDialog
#include "ControlesDuMainDialog.h"
#include ".\maindialog.h"


void CMainDialog::DrawSeq()
{
	if(m_bWasPaintedOnce)
	{		
		m_bUseSeqView = false;
		if(!m_bIsRunning)
		{
			m_pMainFrame->m_MainDialog.MakeSequence(&LaSeqMainDialog);
			m_View.Invalidate();
		}
	}												
}

/*
Fabrique la séquence à partir de toutes les données de la MainDialogBar
Renvoye le nombre total de samples de la séquence.
*/
unsigned int CMainDialog::MakeSequence(Sequence* pUneSequence, bool MakeBuffers /* = false */)
{	
	Sequence& LaSeq = (*pUneSequence);
	if(!m_bUseSeqView)
	{
		// on nettoye la séquence
		LaSeq.ClearColonneList();
		DefinitionSequenceMainDialog(LaSeq);
	}
	if(LaSeq.m_nPeriodes == 0)
	{
		MessageBox("Rien dans la séquence");
		return 0;
	}
	else if(LaSeq.m_nPeriodes > 6e6) 
	{
		MessageBox("Trop de Points ! !");
		return 0;
	}
	else if(MakeBuffers)
	{
		LaSeq.MakeDigBuffer();
		LaSeq.MakeAnaBuffer();
	}

	CString InfoSeq;
	InfoSeq.Format(_T("SampRate:%.3g/s  Dur: %dms"), Sequence::m_SampleRate, (LaSeq.m_nPeriodes*1000)/int(Sequence::m_SampleRate));
	InfoSeq.Replace("e+0","E");InfoSeq.Replace("E0","E");InfoSeq.Replace("E0","E");
	m_pMainFrame->m_wndStatusBar.SetPaneText(0, InfoSeq);

return LaSeq.m_nPeriodes;
}

void CMainDialog::ValueChangedButtonStartSequence(BOOL Value)
{	bool value = Value ? true : false;
	// Load the hourglass cursor
	HCURSOR lhCursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);	SetCursor(lhCursor);

	pLaSeqUsedForNiCards = &LaSeqMainDialog;
	if(m_bUseSeqView == true)
	{
		pLaSeqUsedForNiCards = pLaSeqView;
	}
	// pour valider l'edit box modifier éventuellement
	OnCommand(IDOK, 0);
	// pareil dans la Port view
	m_pMainFrame->m_PortDialog.OnCommand(IDOK, 0);
	
	UpdateControls();	
	m_pMainFrame->m_PortDialog.UpdateControls();

	if(value == TRUE)
	{
		StopSequenceNiCards();
		CleanNiCards();
		m_pMainFrame->m_PortDialog.ValueChangedPortCheckUpdateNow(true);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ConfigureNiCards();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(m_bVarieActive && m_Repete)
		{
			m_pMainFrame->m_PortDialog.ValueChangedPortCheckUpdateNow(true);
			RETROUVE_PROGRESS_BARS // a faire avant chaque appel de AssignParamNextStep...
			pLaSeqUsedForNiCards->m_LaVariation.AssignParamNextStep(ProgressBars, true);
			UpdateControls();	m_pMainFrame->m_PortDialog.UpdateControls();
			pLaSeqUsedForNiCards->m_LaVariation.SumUpToFile( CString(DOSSIER_DIALOG_POUR_RB2CAM) + CString(FICHIER_DIALOG_POUR_RB2CAM)
											, "INITIAL_STATE");
		}
		UpdateControls();	m_pMainFrame->m_PortDialog.UpdateControls();	m_View.Invalidate();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LanceSequenceNiCards();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	if(value == FALSE)
	{
		StopSequenceNiCards();

		CleanNiCards();

		m_pMainFrame->m_PortDialog.ValueChangedPortCheckUpdateNow(true);

		UpdateControls();	m_pMainFrame->m_PortDialog.UpdateControls();
		m_View.Invalidate();
	}
	// Load the hourglass cursor
	lhCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);	SetCursor(lhCursor);
}

void CMainDialog::ConfigureNiCards()
{
	p_DigitalCodeMainDialog	= new CDAQmxDynamicCardDigital32();
	p_Analog1CodeMainDialog	= new CDAQmxDynamicCardAnalog1();
// 	p_Analog2CodeMainDialog	= new CDAQmxDynamicCardAnalog2();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// from : C:\Program Files\National Instruments\MeasurementStudioVS2003\VCNET\Examples\DAQmx\Synchronization\Multi-Device\AIFiniteAcquisition
	//" synchronisation des montres ! "												//
	//
	// First, verify the master task so we can query its properties.				//
	master.Control(DAQmxTaskVerify);												//
	// Verify the slave task so we can query its properties.						//
	slave1.Control(DAQmxTaskVerify);												//
// 	slave2.Control(DAQmxTaskVerify);												//
	// Next, find out what device the master is using.								//
	// This is so we can build terminal strings using the							//
	// master device name.															//
	CString AOfirstPhysChanName = master.AOChannels[0].PhysicalName;				//
	CString AOdeviceName = AOfirstPhysChanName.Left(AOfirstPhysChanName.Find(_T('/'))+1);//
	CString AOterminalNameBase = CString(_T("/")) + AOdeviceName;					//
	// slave1 device name.															//
	CString DOfirstPhysChanName = slave1.DOChannels[0].PhysicalName;				//
	CString DOdeviceName = DOfirstPhysChanName.Left(DOfirstPhysChanName.Find(_T('/'))+1);//
	CString DOterminalNameBase = CString(_T("/")) + DOdeviceName;					//
	// slave2 device name.															//
// 	CString AO2firstPhysChanName = slave2.AOChannels[0].PhysicalName;				//
// 	CString AO2deviceName = AOfirstPhysChanName.Left(AOfirstPhysChanName.Find(_T('/'))+1);//
// 	CString AO2terminalNameBase = CString(_T("/")) + AOdeviceName;					//
	//
	// E-Series Synchronization														//
// 	slave1.Timing.MasterTimebaseSource = master.Timing.MasterTimebaseSource;		//
// 	slave1.Timing.MasterTimebaseRate   = master.Timing.MasterTimebaseRate;			//
// 	slave2.Timing.MasterTimebaseSource = master.Timing.MasterTimebaseSource;		//
// 	slave2.Timing.MasterTimebaseRate   = master.Timing.MasterTimebaseRate;			//
	//
	//		if(m_WaitTrigger)
	{
		// Configure a digital edge start trigger so the master task				//
		// start on an external trigger.											//
		//			master.Triggers.StartTrigger.ConfigureDigitalEdgeTrigger(					//
		//				DOterminalNameBase + _T("PFI6"),// corresponds to the "ACK1 input"		//
		//				DAQmxDigitalEdgeStartTriggerEdgeRising);								//
		//			master.Triggers.PauseTrigger.ConfigureDigitalLevelTrigger(					//
		//				DOterminalNameBase + _T("PFI2"),// corresponds to the "REQ1 input"		//
		//				DAQmxDigitalLevelPauseTriggerConditionHigh);							//
	}
	//
	// Configure a digital edge start trigger so both tasks							//
	// start together: the slave is triggered by the master.						//
	slave1.Triggers.StartTrigger.ConfigureDigitalEdgeTrigger(						//
		AOterminalNameBase + _T("ao/StartTrigger"),									//
		DAQmxDigitalEdgeStartTriggerEdgeRising);									//
// 	slave2.Triggers.StartTrigger.ConfigureDigitalEdgeTrigger(						//
// 		AOterminalNameBase + _T("ao/StartTrigger"),									//
// 		DAQmxDigitalEdgeStartTriggerEdgeRising);									//

	//		if(m_WaitTrigger)
	{
		//			slave1.Triggers.PauseTrigger.ConfigureDigitalLevelTrigger(					//
		//				AOterminalNameBase + _T("ao/PauseTrigger"),								//
		//				DAQmxDigitalLevelPauseTriggerConditionHigh);							//
	}

	master.Control(DAQmxTaskVerify);												//
	slave1.Control(DAQmxTaskVerify);												//
// 	slave2.Control(DAQmxTaskVerify);												//
}
void CMainDialog::LanceSequenceNiCards()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////  Ici commencent les histoires de				////////////////////////
	//////////////////////////  Start, Wait, Repete,... de la sequence		////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int Duration = MakeSequence(pLaSeqUsedForNiCards, true);
	if(!Duration)
		return;

	CNiUInt32Matrix& DataDigital = pLaSeqUsedForNiCards->GetDigBufferNiMatrix_Card(1);
	CNiReal64Matrix& DataAnalog1 = pLaSeqUsedForNiCards->GetAnaBufferNiMatrix_Card(1);
	CNiReal64Matrix& DataAnalog2 = pLaSeqUsedForNiCards->GetAnaBufferNiMatrix_Card(2);
	// si on attend un trigger, on ne déclenche la séquence qu'une seule fois
	if(m_WaitTrigger && !m_Repete)
	{
		slave1.Configure(Duration * 1);
// 		slave2.Configure(Duration * 1);
		master.Configure(Duration * 1);
	}
	else if( m_Repete )
	{
		slave1.Configure(Duration * m_Repete);
// 		slave2.Configure(Duration * m_Repete);
		master.Configure(Duration * m_Repete);
	}

	p_DigitalCodeMainDialog->SendNiMatrixDataToTheCard(	DataDigital	);
	p_Analog1CodeMainDialog->SendNiMatrixDataToTheCard(	DataAnalog1	);
// 	p_Analog2CodeMainDialog->SendNiMatrixDataToTheCard(	DataAnalog2	);

	master.WaitUntilDone();
	slave1.WaitUntilDone();
// 	slave2.WaitUntilDone();

	slave1.Start();	// FIRST the Slaves 
// 	slave2.Start();	// FIRST the Slaves 
	master.Start();	// THEN the Master	

	m_bIsRunning = true;

	if(m_WaitTrigger)
	{
		// remise à 0 du compteur de trig
		m_WaitTrigger = 1;
		m_NiButtonWaitTrigger.SetOnText("0 Trig Received");
		return;
	}
	else if(m_Repete)
	{
		if(pLaSeqUsedForNiCards->m_LaVariation.GetMaxVarie())
		{
			slave1.WaitUntilDone();
// 			slave2.WaitUntilDone();
			slave1.Stop();
// 			slave2.Stop();
			master.Stop();

//			CNewPortXPS UnXpsPourRapatrierDesRecord;
//			if(UnXpsPourRapatrierDesRecord.XPSConnect("129.199.119.138", false))
//				UnXpsPourRapatrierDesRecord.Record_SaveAndRapatrieInDefaultFolder("Record_Laser", pLaSeqUsedForNiCards->m_LaVariation.m_CourantVarieStep, m_ParamReal[46], m_ParamReal[47], m_ParamReal[48]);

			// on va aller rentrer dans la fonction OnTimer avec ca:
			m_IsDoingVariation = 1;
		}
		else if(m_IsDoingVariation)
		{
			CheckDlgButton(IDC_MAIN_BUTTON_START_SEQUENCE, false);
			m_bIsRunning = false;
		}
	}
}
void CMainDialog::StopSequenceNiCards()
{
	if(m_IsDoingVariation)
	{
		if(m_bVarieActive)
		{
			// on veut savoir si la variation s'est fini toute seul, ou si on l'a stopée
			bool AFiniLaVariationToutSeul = (pLaSeqUsedForNiCards->m_LaVariation.m_CourantVarieStep - 1 == pLaSeqUsedForNiCards->m_LaVariation.nSteps);

			// on remet les compteur à zero
			RETROUVE_PROGRESS_BARS // a faire avant chaque appel de AssignParamNextStep...
				pLaSeqUsedForNiCards->m_LaVariation.AssignParamNextStep(ProgressBars, true, true);

			// un petit résumé de ça pour la caméra
			CString SumedUpTexte = 
				pLaSeqUsedForNiCards->m_LaVariation.SumUpToFile( CString(DOSSIER_DIALOG_POUR_RB2CAM) + CString(FICHIER_DIALOG_POUR_RB2CAM)
				, AFiniLaVariationToutSeul? "FINISHED" : "ABORTED");
			// si la série s'est terminé toute seule, on sauve le fichier config courant de Rb2Manip dans le DOSSIER_DIALOG_POUR_RB2CAM
			if(AFiniLaVariationToutSeul)
			{
				CString NomFichier = CString(DOSSIER_DIALOG_POUR_RB2CAM) + CString("ConfigLastSerieFinished.Rb2");
				m_pMainFrame->m_Save.SaveSettings(NomFichier);
//srybex 				SendMeAnEMail("reinaudi@lkb.ens.fr", "Finished"
//srybex 					, CString("La séquence s'est bien terminée\r\n") + SumedUpTexte
//srybex 					, NomFichier );
			}
		}

		MakeSequence(pLaSeqUsedForNiCards, true);			
		m_IsDoingVariation = 0;

		slave1.Stop();
// 		slave2.Stop();
		master.Stop();
		m_bIsRunning = false;
	}
}
void CMainDialog::CleanNiCards()
{
	if(p_DigitalCodeMainDialog)
	{
		delete p_DigitalCodeMainDialog;
		p_DigitalCodeMainDialog = NULL;
		m_bIsRunning = false;
	}
	if(p_Analog1CodeMainDialog)
	{
		delete p_Analog1CodeMainDialog;
		p_Analog1CodeMainDialog = NULL;
	}
	if(p_Analog2CodeMainDialog)
	{
		delete p_Analog2CodeMainDialog;
		p_Analog2CodeMainDialog = NULL;
	}
}
void CMainDialog::ValueChangedMainCheckWaitTrigger(BOOL Value)
{
	m_WaitTrigger = Value ? 1 : 0;
	if(m_WaitTrigger)
	{
		m_NiButtonWaitTrigger.SetOnText("0 Trig Received");
	}
	DrawSeq();
}

void CMainDialog::ValueChangedMainEditWaitBeforeLoad(BLABLA00)
{	m_WaitBeforeLoad	=		int( Value->dblVal );	if(m_bUpdateAuto) DrawSeq();	}


BOOL CMainDialog::OnIdle(LONG lCount)	
{
	if(!m_WaitTrigger || !m_bIsRunning)
		return TRUE;

//	if(master.IsDone && (m_WaitTrigger < m_Repete || !m_Repete))
	if(master.IsDone && !m_Repete)
	{
		// le texte sur le bouton Trigger
		CString TxtWaiting;	TxtWaiting.Format(_T("%d Trig Received"), m_WaitTrigger);
		m_NiButtonWaitTrigger.SetOnText(TxtWaiting);

		m_WaitTrigger++;
		slave1.Stop();
// 		slave2.Stop();
		master.Stop();
		slave1.Start();
// 		slave2.Start();
		master.Start();
	}

	if(master.IsDone && m_Repete)
	{
		OnBnSimulClickButtonStopSeq();
	}

return TRUE;
}

void CMainDialog::OnDropFiles(HDROP hDropInfo)
{
	m_pMainFrame->OnDropFiles(hDropInfo);
}

void CMainDialog::OnTimer(UINT nIDEvent)
{
	if(nIDEvent != ID_CLOCK_TIMER_MAIN_DIALOG)
		return;

	pLaSeqUsedForNiCards = &LaSeqMainDialog;
	if(m_bUseSeqView == true)
	{
		pLaSeqUsedForNiCards = pLaSeqView;
	}

	if( m_IsDoingVariation)
	{
//		StopSequenceNiCards();
		CleanNiCards();
//		m_pMainFrame->m_PortDialog.ValueChangedPortCheckUpdateNow(true);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ConfigureNiCards();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		RETROUVE_PROGRESS_BARS // a faire avant chaque appel de AssignParamNextStep...
		if( m_bVarieActive && m_Repete && pLaSeqUsedForNiCards->m_LaVariation.AssignParamNextStep(ProgressBars))
		{
			UpdateControls();	m_pMainFrame->m_PortDialog.UpdateControls();

//		m_pMainFrame->m_CarteDAQmxSemblePouvoirMarcher = m_pMainFrame->m_PortDialog.ValueChangedPortCheckUpdateNow(true);
			pLaSeqUsedForNiCards->m_LaVariation.SumUpToFile( CString(DOSSIER_DIALOG_POUR_RB2CAM) + CString(FICHIER_DIALOG_POUR_RB2CAM)
											, "DOING_IT");
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LanceSequenceNiCards();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//			CNewPortXPS UnXpsPourRapatrierDesRecord;
//			if(UnXpsPourRapatrierDesRecord.XPSConnect("129.199.119.138", false))
//				UnXpsPourRapatrierDesRecord.Record_SaveAndRapatrieInDefaultFolder("Record_Laser", pLaSeqUsedForNiCards->m_LaVariation.m_CourantVarieStep, m_ParamReal[46], m_ParamReal[47], m_ParamReal[48]);
		}
		else
		{	
			CheckDlgButton(IDC_MAIN_BUTTON_START_SEQUENCE, false);
		}
		
	}
}

void CMainDialog::OnCbnSelchangeMainComboSampleRate()
{
	m_NbrComboSampleRate = m_ComboSampleRate.GetCurSel();

	switch(m_NbrComboSampleRate)
	{
	case 0:
		Sequence::m_SampleRate = 100000;	// periodes par seconde
		break;
	case 1:
		Sequence::m_SampleRate = 10000;		// periodes par seconde
		break;;
	case 2:
		Sequence::m_SampleRate = 1000;		// periodes par seconde
		break;
	case 3:
		Sequence::m_SampleRate = 500000;	// periodes par seconde
		break;
	}
	if(!m_bIsRunning){MakeSequence(&LaSeqMainDialog);
	m_View.Invalidate();}
}



///////////////////////////// CF : Doc&Astuces  : "CSMTPConnection2.asp_files"
//// Envoye un mail à l'Adresse
//bool CMainDialog::SendMeAnEMail(CString Adresse, CString Sujet, CString Corps, CString Attachement)
//{
//	CoInitialize(0);
//	CMimeMessage msg;
//	msg.SetSender("reinaudi@lkb.ens.fr");
//	msg.SetSenderName("Rb2Manip");
//	msg.AddRecipient(Adresse);
//
//	//Optional
//	msg.SetSubject(Sujet);
//	msg.AddText(Corps);
//	if(Attachement != "")
//		msg.AttachFile(Attachement);
//
//	CSMTPConnection2 conn;
//	//You need to specify the domain name of the recipient email address
//	bool BienEnvoye = false;
//	if(conn.Connect("lkb.ens.fr"))
//	{
//		BienEnvoye = conn.SendMessage(msg) ? true : false;
//		conn.Disconnect();    
//	}
//return BienEnvoye;
//}

//void CGeneticDialog::ActualiseParam_LanceUneSequence(GAGenome& TheGenome)
//{
//	CGeneticDialog& GeneticDialog = *(CGeneticDialog*)(&m_pMainFrame->m_GeneticDialog);
//	GARealGenome& genome = (GARealGenome&)TheGenome;
//	// affectons les nouelles valeurs aux parametres
//	for(int indexParam = 0; indexParam < genome.length(); indexParam++)
//	{
//		CParametre& LeParam = *(GeneticDialog.m_ListParamGenetic.GetAt(GeneticDialog.m_ListParamGenetic.FindIndex(indexParam)));
//		if(LeParam.m_Type == Entier)
//			LeParam.Set_Val(int(genome.gene(indexParam)));
//		if(LeParam.m_Type == Double)
//			LeParam.Set_Val(genome.gene(indexParam));
//	}
//
//	ActualiseListCtrlParamGenetic(false);
//	m_pMainFrame->m_MainDialog.OnBnClickedRadioModeRepet1();
//	m_pMainFrame->m_MainDialog.ValueChangedButtonStartSequence(1);
//
//	m_TimeMilliSecWhenSequenceLancee = GetTimePrecisMilliSecondeScinceComputerON();
//	m_DureeDeLaSequenceMilliSec = m_pMainFrame->m_MainDialog.pLaSeqUsedForNiCards->m_nPeriodes * 1000 / Sequence::m_SampleRate;
//	
//	m_SequenceLancee = true;
//}
//bool CGeneticDialog::IsFitResultReady(int indexExpressionFitDialog , double & ResultatEventuel)
//{
//	ResultatEventuel = 0.0;
//	// has the result arrived ?
//	if(m_pMainFrame->m_TraitImDialog.m_pSerieCourante->m_FitAllreadyComputed)
//	{
//		ResultatEventuel = m_pMainFrame->m_FitDialog.m_ValeurCalcul[indexExpressionFitDialog];
//		return true;
//	}
////	// or Has the picture been not well taken: no fit beacause there is no image (time out)
////	if(GetTimePrecisMilliSecondeScinceComputerON() > m_TimeMilliSecWhenSequenceLancee + m_DureeDeLaSequenceMilliSec + 5000)
////	{
////	//	AfxMessageBox("Pas de fit result en 5 seconds apres la fin de la sequence");
////		return true;
////	}
//return false;
//}
//float CGeneticDialog::FonctionEvaluationQuiLanceUneSequenceEtAttendLeResultatDuFitImage(GAGenome& TheGenome)
//{
//	CGeneticDialog& GeneticDialog = *(CGeneticDialog*)(&m_pMainFrame->m_GeneticDialog);
//	GARealGenome& genome = (GARealGenome&)TheGenome;
//	GeneticDialog.m_pTheGenomeToEvaluate = &TheGenome;
//	// si tout les parametre sont 0.0, on est probablement en train d'initialiser la population de nBestGenome
//	// donc on retourne tout de suite
//	bool one_is_not_null = false;
//	for(int indexParam = 0; indexParam < genome.length(); indexParam++)
//	{
//		if(genome.gene(indexParam) != 0)
//			one_is_not_null = true;
//	}
//	if(!one_is_not_null)
//		return 0.0;
//	// if we are already waiting
//	if(GeneticDialog.m_WaitResultEvaluationIdle)
//	{
//		//AfxMessageBox("Error\n AlreadyWaiting result Of the evaluatin function!!");
//		return float(0.0);
//	}
//	//let's tell to MainThread.Idle() that we are waiting
//	GeneticDialog.m_LetsStartSequenceInIdle = true;
//	// and we wait Idle say it is ok
//	while( (GeneticDialog.m_WaitResultEvaluationIdle || GeneticDialog.m_LetsStartSequenceInIdle || GeneticDialog.m_SequenceLancee)
//		  && GeneticDialog.m_IsEvolving)
//		Sleep(100);// wait
//	GeneticDialog.ActualiseGAStatistics();
//	//then:
//	return float(GeneticDialog.m_ResultOfIdleEvaluation);
//}

void CMainDialog::DblClickMainCheck16()
{
	Sequence& LaSeq = LaSeqMainDialog;
//	m_FilmFromColName = "Monst";
//	m_FilmToColName = "sasbs";
//	m_FilmStep_ms = 5;
	CDialogFilmColonnes DlgFilm(LaSeq, m_FilmFromColName, m_FilmToColName, m_FilmStep_ms, m_FilmDuree_ms);
	if(DlgFilm.DoModal() != IDOK) {return;}
	// récupération des donnée
	m_FilmFromColName = DlgFilm.m_PartNameFromCol;
	m_FilmToColName = DlgFilm.m_PartNameToCol;
	m_FilmStep_ms = DlgFilm.m_Step_ms;
	m_FilmDuree_ms = DlgFilm.m_Duree_ms;

	// on définit le parmetre m_ParamReal[80] pour la variation
    	CParametre & LeParam = *CParametre::Param(&m_ParamReal[80]);
	
	LeParam.m_From = 0;
	LeParam.m_To = m_FilmDuree_ms;
	LeParam.m_Step = m_FilmStep_ms;
	
	// on enlève la varie, puis on la met
	ValueChangedMainCheckVar01(false);
	AddToVariation(&m_ParamReal[80]);
	
	// set le truc a false, mais en fait, il y a un message en attente qui est le deuxieme click du DblClk
	m_NiCheck[16].SetValue(false);

return;	if(0)
	{
//	CNewPortXPS LeXPSMainDialog;
//	LeXPSMainDialog.OpenGraphicInterface();
//	return;
	LaWfMainDialog.SetGraphically();
	// set le truc a false, mais en fait, il y a un message en attente qui est le deuxieme click du DblClk
	m_NiCheck[16].SetValue(false);
	}
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Gestion des racourcis clavier de MainDialog !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void CMainDialog::OnBnSimulClickButtonReStartSeq()
{
		OnBnSimulClickButtonStopSeq();
		OnBnSimulClickButtonStartStopSeq();
}
void CMainDialog::OnBnSimulClickButtonStartStopSeq()
{
	int val = IsDlgButtonChecked(IDC_MAIN_BUTTON_START_SEQUENCE);
	CheckDlgButton(IDC_MAIN_BUTTON_START_SEQUENCE , !val);
}
void CMainDialog::OnBnSimulClickButtonStopSeq()
{
	int val = IsDlgButtonChecked(IDC_MAIN_BUTTON_START_SEQUENCE);
	if(val)
		CheckDlgButton(IDC_MAIN_BUTTON_START_SEQUENCE , 0);
	ValueChangedButtonStartSequence(FALSE);
}

BOOL CMainDialog::PreTranslateMessage(MSG* pMsg)
{
	int nChar = pMsg->wParam;
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_ESCAPE: 
			OnBnSimulClickButtonStopSeq();		return 1;
		case VK_ADD:
			OnBnSimulClickButtonReStartSeq();	return 1;
		case VK_SUBTRACT:
			;	return 1;
		case VK_F5: 
			OnBnSimulClickButtonStartStopSeq();	return 1;
		}
	}
	//if (pMsg->message == WM_KEYDOWN)
	//{	
	//	SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
	//	return 1;
	//}
	//if (pMsg->message == WM_CHAR)
	//{	
	//	SendMessage(WM_CHAR, pMsg->wParam, pMsg->lParam);
	//	return 1;
	//}
	return CProDialog::PreTranslateMessage(pMsg);
}
//void CMainDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	switch(nChar)
//	{
//	case VK_ESCAPE: 
//		OnBnSimulClickButtonStopSeq();		return;
//	case VK_ADD:
//		OnBnSimulClickButtonReStartSeq();	return;
//	case VK_SUBTRACT:
//										;	return;
//	case VK_F5: 
//		OnBnSimulClickButtonStartStopSeq();	return;
//	}
//	CProDialog::OnKeyDown(nChar, nRepCnt, nFlags);
//}
//void CMainDialog::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//OnKeyDown(nChar, nRepCnt, nFlags);
//}
