#pragma once

#include "Sequence.h"
#include "Variation.h"
#include "DAQmxTasksAndCodes.h"
#include "ProDialog.h"

#define FICHIER_DIALOG_POUR_RB2CAM   "DialogAvecRb2Cam.txt"
#define DOSSIER_DIALOG_POUR_RB2CAM   "C:\\"

#define	RETROUVE_PROGRESS_BARS	ProgressBars[1] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_01 );		ProgressBars[2] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_02 );		ProgressBars[3] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_03 );		ProgressBars[4] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_04 );		ProgressBars[5] = (CProgressCtrl*) GetDlgItem( IDC_MAIN_VAR_PROGRESS_05 );

// CMainDialog
/*
Je fabrique une Classe CMainDialog dérivée de la CDialogBar pour pouvoir 
y déclarer les fonctions qui gèrent les controls ActiveX de National Instruments
en effet, les ON_EVENT ne marche que dans un CDialog...
*/
class CMainDialog : public CProDialog

{
	DECLARE_DYNAMIC(CMainDialog)

public:
	CMainDialog();
	virtual ~CMainDialog();

/////////////////////////// CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDialogBar)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
protected:
	virtual BOOL OnInitDialogBar();
/////////////////////////// End CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"

public:
	BOOL OnIdle(LONG lCount);

public:
	Sequence LaSeqMainDialog;
	Sequence* pLaSeqUsedForNiCards;
	
	// pointeur vers LaSeqPro, qui est une copie de LaSeqMainDialog, modifiée par des click dans le graphique.
	Sequence* pLaSeqView;
	// indique que c'est LaSeqPro qui doit etre donnée au cartes et non pas LaSeqMainDialog
	bool m_bUseSeqView;

	CDAQmxDynamicCardDigital32* p_DigitalCodeMainDialog	;
	CDAQmxDynamicCardAnalog1* p_Analog1CodeMainDialog	;
	CDAQmxDynamicCardAnalog2* p_Analog2CodeMainDialog	;
	// maitre																		//
#define	master  p_Analog1CodeMainDialog->GetTask()
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//		// esclave																		//
#define slave1 p_DigitalCodeMainDialog->GetTask()
#define slave2 p_Analog2CodeMainDialog->GetTask()
	
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:	
	// indique l'attente d'un trigger externe pour lancer la sequence, stock aussi le nombre de trigg reçus
	int m_WaitTrigger;
	// indique les répétitions de la séquence
	int m_Repete;
	// La Combo pour la Sample Rate
	CComboBox m_ComboSampleRate;
	unsigned int m_NbrComboSampleRate;

	// indique que la sequence is running
	bool m_bIsRunning;
	// indique que la variation est active (potentiellement)
	bool m_bVarieActive;
	// indique que le programme est en train de parcourir une Variation et qu'il doit ré-écreire les buffers dans "OnIdle"
	int m_IsDoingVariation;

	//// indique que l'un des parametre de la fenetre Main a changé
	//bool m_bMainChanged;

public:	
	CNiButton	m_NiButtonWaitTrigger;
	CNiNumEdit	m_NiEditWaitBeforeLoad;

	bool m_bUpdateAuto;
	void ActualiseNiEdit(int which, double NewVal, bool ReCalculateSeq){
		bool autoupdt = m_bUpdateAuto;	m_bUpdateAuto = ReCalculateSeq;	m_NiEdit[which].SetValue(NewVal);	m_bUpdateAuto = autoupdt;};

#define NOMBRE_NI_EDIT 90
	CNiNumEdit m_NiEdit[NOMBRE_NI_EDIT];
#define NOMBRE_NI_BUTTON 50
	CNiButton m_NiCheck[NOMBRE_NI_BUTTON];

	// les Progress Bar:
	CProgressCtrl* ProgressBars[MAX_PROGRESS_BAR+1];

	int m_WaitBeforeLoad;

	int m_ParamBool[100];
	double m_ParamReal[100];
	CString m_FilmFromColName, m_FilmToColName;
	double m_FilmStep_ms, m_FilmDuree_ms;
	CWaveForme LaWfMainDialog;

protected:
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ValueChangedMainCheck01(BOOL Value);	
	void ValueChangedMainCheck02(BOOL Value);	
	void ValueChangedMainCheck03(BOOL Value);	
	void ValueChangedMainCheck04(BOOL Value);	
	void ValueChangedMainCheck05(BOOL Value);	
	void ValueChangedMainCheck06(BOOL Value);	
	void ValueChangedMainCheck07(BOOL Value);	
	void ValueChangedMainCheck08(BOOL Value);	
	void ValueChangedMainCheck09(BOOL Value);
	void ValueChangedMainCheck10(BOOL Value);
	void ValueChangedMainCheck11(BOOL Value);
	void ValueChangedMainCheck12(BOOL Value);
	void ValueChangedMainCheck13(BOOL Value);
	void ValueChangedMainCheck14(BOOL Value);
	void ValueChangedMainCheck15(BOOL Value);
	void ValueChangedMainCheck16(BOOL Value);
	void ValueChangedMainCheck17(BOOL Value);
	void ValueChangedMainCheck18(BOOL Value);
	void ValueChangedMainCheck19(BOOL Value);
	void ValueChangedMainCheck20(BOOL Value);
	void ValueChangedMainCheck21(BOOL Value);
	void ValueChangedMainCheck22(BOOL Value);
	void ValueChangedMainCheck23(BOOL Value);
	void ValueChangedMainCheck24(BOOL Value);
	void ValueChangedMainCheck25(BOOL Value);
	void ValueChangedMainCheck26(BOOL Value);
	void ValueChangedMainCheck27(BOOL Value);
	void ValueChangedMainCheck28(BOOL Value);
	void ValueChangedMainCheck29(BOOL Value);
	void ValueChangedMainCheck30(BOOL Value);
	void ValueChangedMainCheck31(BOOL Value);	
	void ValueChangedMainCheck32(BOOL Value);	
	void ValueChangedMainCheck33(BOOL Value);	
	void ValueChangedMainCheck34(BOOL Value);	
	void ValueChangedMainCheck35(BOOL Value);	
	void ValueChangedMainCheck36(BOOL Value);	
	void ValueChangedMainCheck37(BOOL Value);	
	void ValueChangedMainCheck38(BOOL Value);	
	void ValueChangedMainCheck39(BOOL Value);
	void ValueChangedMainCheck40(BOOL Value);
	void ValueChangedMainCheck41(BOOL Value);
	void ValueChangedMainCheck42(BOOL Value);
	void ValueChangedMainCheck43(BOOL Value);
	void ValueChangedMainCheck44(BOOL Value);
	void ValueChangedMainCheck45(BOOL Value);
	void ValueChangedMainCheck46(BOOL Value);
	void ValueChangedMainCheck47(BOOL Value);
	void ValueChangedMainCheck48(BOOL Value);
	void ValueChangedMainCheck49(BOOL Value);


public:
#define BLABLA00 VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange
	void ValueChangedMainEdit01(BLABLA00);
	void ValueChangedMainEdit02(BLABLA00);
	void ValueChangedMainEdit03(BLABLA00);
	void ValueChangedMainEdit04(BLABLA00);
	void ValueChangedMainEdit05(BLABLA00);
	void ValueChangedMainEdit06(BLABLA00);
	void ValueChangedMainEdit07(BLABLA00);
	void ValueChangedMainEdit08(BLABLA00);
	void ValueChangedMainEdit09(BLABLA00);
	void ValueChangedMainEdit10(BLABLA00);
	void ValueChangedMainEdit11(BLABLA00);
	void ValueChangedMainEdit12(BLABLA00);
	void ValueChangedMainEdit13(BLABLA00);
	void ValueChangedMainEdit14(BLABLA00);
	void ValueChangedMainEdit15(BLABLA00);
	void ValueChangedMainEdit16(BLABLA00);
	void ValueChangedMainEdit17(BLABLA00);
	void ValueChangedMainEdit18(BLABLA00);
	void ValueChangedMainEdit19(BLABLA00);
	void ValueChangedMainEdit20(BLABLA00);
	void ValueChangedMainEdit21(BLABLA00);
	void ValueChangedMainEdit22(BLABLA00);
	void ValueChangedMainEdit23(BLABLA00);
	void ValueChangedMainEdit24(BLABLA00);
	void ValueChangedMainEdit25(BLABLA00);
	void ValueChangedMainEdit26(BLABLA00);
	void ValueChangedMainEdit27(BLABLA00);
	void ValueChangedMainEdit28(BLABLA00);
	void ValueChangedMainEdit29(BLABLA00);
	void ValueChangedMainEdit30(BLABLA00);
	void ValueChangedMainEdit31(BLABLA00);
	void ValueChangedMainEdit32(BLABLA00);
	void ValueChangedMainEdit33(BLABLA00);
	void ValueChangedMainEdit34(BLABLA00);
	void ValueChangedMainEdit35(BLABLA00);
	void ValueChangedMainEdit36(BLABLA00);
	void ValueChangedMainEdit37(BLABLA00);
	void ValueChangedMainEdit38(BLABLA00);
	void ValueChangedMainEdit39(BLABLA00);
	void ValueChangedMainEdit40(BLABLA00);
	void ValueChangedMainEdit41(BLABLA00);
	void ValueChangedMainEdit42(BLABLA00);
	void ValueChangedMainEdit43(BLABLA00);
	void ValueChangedMainEdit44(BLABLA00);
	void ValueChangedMainEdit45(BLABLA00);
	void ValueChangedMainEdit46(BLABLA00);
	void ValueChangedMainEdit47(BLABLA00);
	void ValueChangedMainEdit48(BLABLA00);
	void ValueChangedMainEdit49(BLABLA00);
	void ValueChangedMainEdit50(BLABLA00);
	void ValueChangedMainEdit51(BLABLA00);
	void ValueChangedMainEdit52(BLABLA00);
	void ValueChangedMainEdit53(BLABLA00);
	void ValueChangedMainEdit54(BLABLA00);
	void ValueChangedMainEdit55(BLABLA00);
	void ValueChangedMainEdit56(BLABLA00);
	void ValueChangedMainEdit57(BLABLA00);
	void ValueChangedMainEdit58(BLABLA00);
	void ValueChangedMainEdit59(BLABLA00);
	void ValueChangedMainEdit60(BLABLA00);
	void ValueChangedMainEdit61(BLABLA00);
	void ValueChangedMainEdit62(BLABLA00);
	void ValueChangedMainEdit63(BLABLA00);
	void ValueChangedMainEdit64(BLABLA00);
	void ValueChangedMainEdit65(BLABLA00);
	void ValueChangedMainEdit66(BLABLA00);
	void ValueChangedMainEdit67(BLABLA00);
	void ValueChangedMainEdit68(BLABLA00);
	void ValueChangedMainEdit69(BLABLA00);
	void ValueChangedMainEdit70(BLABLA00);
	void ValueChangedMainEdit71(BLABLA00);
	void ValueChangedMainEdit72(BLABLA00);
	void ValueChangedMainEdit73(BLABLA00);
	void ValueChangedMainEdit74(BLABLA00);
	void ValueChangedMainEdit75(BLABLA00);
	void ValueChangedMainEdit76(BLABLA00);
	void ValueChangedMainEdit77(BLABLA00);
	void ValueChangedMainEdit78(BLABLA00);
	void ValueChangedMainEdit79(BLABLA00);
	void ValueChangedMainEdit80(BLABLA00);
	void ValueChangedMainEdit81(BLABLA00);
	void ValueChangedMainEdit82(BLABLA00);
	void ValueChangedMainEdit83(BLABLA00);
	void ValueChangedMainEdit84(BLABLA00);
	void ValueChangedMainEdit85(BLABLA00);
	void ValueChangedMainEdit86(BLABLA00);
	void ValueChangedMainEdit87(BLABLA00);
	void ValueChangedMainEdit88(BLABLA00);
	void ValueChangedMainEdit89(BLABLA00);

	
	void ValueChangedButtonStartSequence(BOOL Value);
	void ConfigureNiCards();
	void LanceSequenceNiCards();
	void StopSequenceNiCards();
	void CleanNiCards();

	// dessine la Séquence
	void DrawSeq();
	// Fabrique la séquence à partir de toutes les données de la MainDialogBar
	unsigned int MakeSequence(Sequence* pLaSequence, bool MakeBuffers = false);	
	// Definit la séquence MainDialog a partir des valeur des ParamInt et ParamBool
	unsigned int DefinitionSequenceMainDialog(Sequence& LaSeq);
	// Update tous les controles de la MainDialog au cas ou on les aurait changés programmaticaly
	int UpdateControls();	

	void MouseUpMainEdit01	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit02	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit03	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit04	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit05	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit06	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit07	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit08	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit09	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit10	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit11	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit12	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit13	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit14	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit15	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit16	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit17	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit18	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit19	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit20	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit21	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit22	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit23	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit24	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit25	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit26	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit27	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit28	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit29	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit30	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit31	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit32	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit33	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit34	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit35	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit36	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit37	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit38	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit39	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit40	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit41	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit42	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit43	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit44	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit45	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit46	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit47	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit48	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit49	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit50	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit51	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit52	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit53	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit54	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit55	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit56	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit57	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit58	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit59	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit60	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit61	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit62	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit63	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit64	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit65	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit66	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit67	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit68	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit69	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit70	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit71	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit72	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit73	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit74	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit75	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit76	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit77	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit78	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit79	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit80	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit81	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit82	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit83	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit84	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit85	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit86	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit87	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit88	(short Button, short Shift, long x, long y);
	void MouseUpMainEdit89	(short Button, short Shift, long x, long y);

/**/
	// ici : on ouvre une CDialogVariation Box.
	// Si on clique IDOK : On crée un CParametre qu'on ajoute à la "m_LaVariation" de "LaSequence"
    void AddToVariation(int NumParam);
	// Si on clique IDOK : On crée un CParametre qu'on ajoute à la "m_LaVariation" de "LaSequence"
    void AddToVariation(void* LePointeur);
	// Update Check boxes
	void UpdateCheckBoxesVarie(int OrdreSuivantVariant = 0 );
	// Update Static Text
	void UpdateStaticText();
	void UpdateEditColor(bool TousBlancs = false);
	void ValueChangedMainCheckVar00(BOOL Value);
	void ValueChangedMainCheckVar01(BOOL Value);
	void ValueChangedMainCheckVar02(BOOL Value);
	void ValueChangedMainCheckVar03(BOOL Value);
	void ValueChangedMainCheckVar04(BOOL Value);
	void ValueChangedMainCheckVar05(BOOL Value);
/**/
	afx_msg void OnBnClickedRadioModeRepet1();
	afx_msg void OnBnClickedRadioModeRepet2();
	afx_msg void OnBnClickedRadioModeRepet3();
	void ClickEditModeRepet();
	void ValueChangedMainEditModeRepet(BLABLA00);
	void EnableVariationCtrl(bool State);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT nIDEvent);
	// Envoye un mail à l'Adresse
//srybex 	bool SendMeAnEMail(CString Adresse, CString Sujet = CString(""),  CString Corps = CString(""), CString Attachement = CString(""));

	afx_msg void OnCbnSelchangeMainComboSampleRate();
	void ValueChangedMainCheckWaitTrigger(BOOL Value);
	void ValueChangedMainEditWaitBeforeLoad(BLABLA00);
	void MouseUpMainEditWaitBeforeLoad(short Button, short Shift, long x, long y);
	void DblClickMainCheck16();

//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnBnSimulClickButtonStartStopSeq();
	afx_msg void OnBnSimulClickButtonStopSeq();
	afx_msg void OnBnSimulClickButtonReStartSeq();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#define _MOT_INT			0
#define _ZSL_DET 			1
#define _REP_INT 			2
#define _PROBE_B_ATTEN		3
#define _ANA_04	4
#define _ANA_05	5
#define _ANA_06	6
#define _I_MOT		7	//2013-12-18 renamed to _I_MOT from _MONSTER_LOCK_2.  

#define _GUIDE_CURRENT		10
#define _MONST_VERT_LOCK_1	12
#define _MONST_VERT_LOCK_2	13
#define _POS_XPS_1			14
#define _POS_XPS_2			15

#define _ZEEMAN				 0
#define _MOT_AOM			 1
#define _MOT_CLIC			 2
#define _MAIN_IGBT			 3
#define _REPUMPS			 4
#define _IMAG_AOM			 5
#define _IMAG_CLIC			 6  //Imaging Shutter (commented by Chih-Hsi on 2017-10-18.)
#define _LATT_SHUT			 7
#define _LAT_BACK			 8
#define _NUSD_09			 9
#define _NUSD_10			10	
#define _2D_MOT				11	
#define _CAM_TRIGGER		12	
#define _RepumpShutter		13	//Added 2013-12-2.
#define _RedMOTShutter		14	//Added 2013-12-2. 
#define _IGBT			15		//2013-12-18 changed from _TRANS_COOL to _IGBT.  
#define _NUSD_16			16	
#define _RBROAD				17	
#define _RSINGLE			18	
#define _NUSD_19			19
#define _NUSD_20			20
#define _NUSD_21	21
#define _NUSD_22	22
#define _B_SHUT		23//Added 2013-11-21.
#define _C_SHUT		24//Added 2013-11-21. 
#define _NUSD_25	25
#define _NUSD_26	26
#define _LASER_E_27	27
#define _LASER_D_28	28
#define _LASER_C_29	29
#define _LASER_B_30	30
#define _PROB_SHUT	31
