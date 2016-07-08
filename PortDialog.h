#pragma once

#include "Sequence.h"
#include "Parametre.h"
#include "Variation.h"
#include "DAQmxTasksAndCodes.h"
#include "ProDialog.h"

// CPortDialog
/*
Je fabrique une Classe CPortDialog dérivée de la CProDialog pour pouvoir 
y déclarer les fonctions qui gèrent las controls ActiveX de National Instruments
en effet, les ON_EVENT ne marche que dans un CDialog...
*/
class CPortDialog : public CProDialog
{
	DECLARE_DYNAMIC(CPortDialog)

public:
	CPortDialog();
	virtual ~CPortDialog();
	afx_msg void OnPaint();

public:
	int m_bUpdateAuto;

	int m_bDigOut[NUMBER_DIG_OUTPUT];
	int m_3dMeanCurrent;
	int m_3dDesEquilibr;
	int Current3dCoil_i(int Coil_i, int Mean, int DesEq);// crée les courant dans 3d coil 1 & 2  a partir de la donné de Mean et de DesEq
	int m_Ana_1_Out[NUMBER_ANA_OUTPUT_CARD_ANA_1];
	int m_Ana_2_Out[NUMBER_ANA_OUTPUT_CARD_ANA_2] ;

	// sauvegarde mémoires digitales de la fenetre port : 
	// m_PortInt32_Save[0] à 9 pour switcher rapidement entre configurations Port
	int m_PortInt32_Save[10];

	// dans m_PortAna_Save[10][16] on sauve les 16 sorties analogiques
	int m_PortAna_Save[10][16];
	// indique que les donné du PortDialog ont été changées depuis le dernier sauvetage en m_PortInt32_Save..
	bool m_bPortChanged;
	// instance du NumEdit pour pouvoir changer sa couleur ou sin caractere Enabled
	CNiNumEdit	m_NiEditSaveNum;



protected:
	DECLARE_MESSAGE_MAP()
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
	DECLARE_EVENTSINK_MAP()
public:
    bool ValueChangedPortCheckUpdateNow(BOOL Value);
	void ValueChangedPortCheckUpdateAuto(BOOL Value);

	// dessine la Séquence
	void DrawSeq();
	// Indique que les parametres ont changés depuis le dernier chargement
	void CPortDialog::PortChanged();

	void ValueChangedPortDigOut00(BOOL Value);
	void ValueChangedPortDigOut01(BOOL Value);
	void ValueChangedPortDigOut02(BOOL Value);
	void ValueChangedPortDigOut03(BOOL Value);
	void ValueChangedPortDigOut04(BOOL Value);
	void ValueChangedPortDigOut05(BOOL Value);
	void ValueChangedPortDigOut06(BOOL Value);
	void ValueChangedPortDigOut07(BOOL Value);
	void ValueChangedPortDigOut08(BOOL Value);
	void ValueChangedPortDigOut09(BOOL Value);
	void ValueChangedPortDigOut10(BOOL Value);
	void ValueChangedPortDigOut11(BOOL Value);
	void ValueChangedPortDigOut12(BOOL Value);
	void ValueChangedPortDigOut13(BOOL Value);
	void ValueChangedPortDigOut14(BOOL Value);
	void ValueChangedPortDigOut15(BOOL Value);
	void ValueChangedPortDigOut16(BOOL Value);
	void ValueChangedPortDigOut17(BOOL Value);
	void ValueChangedPortDigOut18(BOOL Value);
	void ValueChangedPortDigOut19(BOOL Value);
	void ValueChangedPortDigOut20(BOOL Value);

	void ValueChangedPortDigOut21 (BOOL Value);
	void ValueChangedPortDigOut22 (BOOL Value);
	void ValueChangedPortDigOut23 (BOOL Value);
	void ValueChangedPortDigOut24 (BOOL Value);
	void ValueChangedPortDigOut25 (BOOL Value);
	void ValueChangedPortDigOut26 (BOOL Value);
	void ValueChangedPortDigOut27 (BOOL Value);
	void ValueChangedPortDigOut28 (BOOL Value);
	void ValueChangedPortDigOut29 (BOOL Value);
	void ValueChangedPortDigOut30(BOOL Value);
	void ValueChangedPortDigOut31(BOOL Value);

#define BLABLA00 VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange
	void ValueChangedPortAna1Out00(BLABLA00);
	void ValueChangedPortAna1Out01(BLABLA00);
	void ValueChangedPortAna1Out02(BLABLA00);
	void ValueChangedPortAna1Out03(BLABLA00);
	void ValueChangedPortAna1Out04(BLABLA00);
	void ValueChangedPortAna1Out05(BLABLA00);
	void ValueChangedPortAna1Out06(BLABLA00);
	void ValueChangedPortAna1Out07(BLABLA00);
	void ValueChangedPortAna2Out00(BLABLA00);
	void ValueChangedPortAna2Out01(BLABLA00);
	void ValueChangedPortAna2Out02(BLABLA00);

	void ValueChangedPortAna2Out03(BLABLA00);
	void ValueChangedPortAna2Out04(BLABLA00);
	void ValueChangedPortAna2Out05(BLABLA00);
	void ValueChangedPortAna2Out06(BLABLA00);
	void ValueChangedPortAna2Out07(BLABLA00);

	// Assigne à la mémoire NumSave, Active cette mémoire, Renvoye le nombre dont la représentation binaire est "les sorties 0 à 31"
	void SavePortInMemory(unsigned short NumSave);
	// Assigne à la mémoire NumSave les Sorties Digital stockées sous la forme, d'un nombre dont la représentation binaire est "les sorties 0 à 31", Renvoye ce nombre
	unsigned long SavePort_DigInt32Bit(unsigned short NumSave);
	// Assigne à la mémoire NumSave les Sorties Analogiques stockées sous la forme de int dans le tableau m_PortAna_Save[NumSave][indexAnaOut]
	void SavePort_Ana(unsigned short NumSave);
	// Recall la mémoire NumSave et Update la fentre port, puis les cartes, puis dessine la séquence
	void RecallPortFromMemory(unsigned short NumSave);
	// Recall la mémoire NumSave : les Sorties Digital 
	void RecallPort_DigInt32Bit(unsigned short NumSave);
	// Recall la mémoire NumSave : les Sorties Analogic 
	void RecallPort_Ana(unsigned short NumSave);

	void DblClickCheckPortSave0();
	void DblClickCheckPortSave1();
	void DblClickCheckPortSave2();
	void DblClickCheckPortSave3();
	void DblClickCheckPortSave4();
	void DblClickCheckPortSave5();
	void DblClickCheckPortSave6();
	void DblClickCheckPortSave7();
	void DblClickCheckPortSave8();
	void DblClickCheckPortSave9();
public:
	// Update tous les controles de la CPortDialog au cas ou on les aurait changés programmaticaly
	int CPortDialog::UpdateControls();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void KeyDownEditPortSaveNum(short* KeyCode, short Shift);

	// ici : on ouvre une CDialogVariation Box.
	// Si on clique IDOK : On crée un CParametre qu'on ajoute à la "m_LaVariation" de "LaSequence"
    void AddToVariation(void* LePointeur);
	void UpdateEditColor(bool TousBlancs = false);

	CNiNumEdit	m_NiEditAna1Out00		;
	CNiNumEdit	m_NiEditAna1Out01		;
	CNiNumEdit	m_NiEditAna1Out02		;
	CNiNumEdit	m_NiEditAna1Out03		;
	CNiNumEdit	m_NiEditAna1Out04		;
	CNiNumEdit	m_NiEditAna1Out05		;
	CNiNumEdit	m_NiEditAna1Out06		;
	CNiNumEdit	m_NiEditAna1Out07	;
	CNiNumEdit	m_NiEditAna2Out00		;
	CNiNumEdit	m_NiEditAna2Out01		;
	CNiNumEdit	m_NiEditAna2Out02	;
	CNiNumEdit	m_NiEditAna2Out03	;
	CNiNumEdit	m_NiEditAna2Out04	;
	CNiNumEdit	m_NiEditAna2Out05	;
	CNiNumEdit	m_NiEditAna2Out06	;
	CNiNumEdit	m_NiEditAna2Out07	;

	void MouseUpEditPortAna1Out00		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna1Out01		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna1Out02		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna1Out03		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna1Out04		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna1Out05		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna1Out06		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna1Out07	(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out00		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out01		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out02	(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out03		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out04		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out05		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out06		(short Button, short Shift, long x, long y);
	void MouseUpEditPortAna2Out07		(short Button, short Shift, long x, long y);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


