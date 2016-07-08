#pragma once

#include "ProDialog.h"
#include "resource.h"
#include "Fitting.h"

enum{	Gauss1D, DoubleGauss1D, Gauss2D, Gauss2DTilt, DoubleGauss2DTige	};

#define MAX_FIT_SIMULTANNE 2
#define MAX_PARAM_DIALOG 16
#define MAX_CALCULATED_VALUE 10
#define STEP_ZONE_VISEUR 20
// CFitDialog
class CFitDialog : public CProDialog
{
	DECLARE_DYNAMIC(CFitDialog)

public:
	CFitDialog();
	virtual ~CFitDialog();
	afx_msg void OnPaint();

public:
	CSettingSave m_SaveFit;
	CFitting* m_pCurrentFit[MAX_FIT_SIMULTANNE+1];

	// Les NiEdit
	CNiNumEdit m_NiEditMaxIter, m_NiEditAverageShots;
	CNiNumEdit	 m_NiEditStart[MAX_PARAM_DIALOG+1];
	CNiNumEdit m_NiEditLambda, m_NiEditChi2;
	CNiButton m_NiButtonPixelSaturated, m_NiCheckFit, m_NiCheckAverage;
	CNiSlide m_NiSlideFitProgress;
	CNiGraph m_NiGraph01;
	// liste des valeurs calculée et expression
	CListCtrl m_ListCalcul;
	double m_ValeurCalcul[MAX_CALCULATED_VALUE];
	CString m_Expres[MAX_CALCULATED_VALUE];
	// pour faire des statistics
	// nombre d'image utilisée actuellement pour faire les stat
	int m_StatNbrShotsCourant;
	// index de l'image qui va servir à faire les stat, utile pour n'utiliser qu'une fois une image
	int m_StatIndexImage;
	// somme des valeurs en question
	double m_StatSommeCalcul[MAX_CALCULATED_VALUE];
	// somme des valeurs au carré
	double m_StatSommeCalculCarre[MAX_CALCULATED_VALUE];
	// d'ou la moyenne
	double m_StatMoyenne[MAX_CALCULATED_VALUE];
	// et la standard deviation
	double m_StatStdDev[MAX_CALCULATED_VALUE];


	unsigned int m_MilliSecLastResetCompteur;// temps en ms depuis lequel on a pas reset le CompteurPrecis
	unsigned int m_MilliSecInf90Highest;// temps auquel la valeur est passée sous les 90% de Highest
	unsigned int m_MilliSecInf90Highest_Sur_e;// temps auquel la valeur est passée sous les 90% de Highest
	double m_ValOscilloscope;
	double m_HighestValOscilloscope;

	// Les Valeur 
	int    m_b[MAX_PARAM_DIALOG + 1];
	double m_f[MAX_PARAM_DIALOG + 1];
	// Une Manière de se retrouver rapidemant parmis les IDC_ de la boite de dialog
	// en les rangeant dans une liste
	int IdcName[MAX_PARAM_DIALOG+1];
	int IdcAdjust[MAX_PARAM_DIALOG+1];
	int IdcStart[MAX_PARAM_DIALOG+1];
//	CNiNumEdit* IdcStartpNi[MAX_PARAM_DIALOG+1];
	int IdcUnit[MAX_PARAM_DIALOG+1];

	CComboBox m_ComboTheFit;
    int m_NbrComboFit;
	int m_NbrIterFitDialog;
	// doit on fiter?
	bool m_MustFit;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:	
	CFitting m_FitGaussienneH;
	CFitting m_FitDoubleGaussienneH;
	CFitting m_FitGaussienneV;
	CFitting m_FitDoubleGaussienneV;
	CFitting m_FitGaussienne2D;
	CFitting m_FitGaussienne2DTilt;
	CFitting m_FitGaussienne2DTigeGauss;
	CFitting m_FitGaussienne2DTigeDoubleGauss;
	CFitting m_Fit2DCondensat;

	CPoint m_RightClickDown, m_RightClickUp, m_LeftClickDown, m_LeftClickUp;
	bool   m_RightClickingNow, m_LeftClickingNow, m_WasFittingBeforeRightClickDown;

	bool m_paramReset;
public:
	void MousseRightDown(UINT nFlags, CPoint point);
	void MousseRightUp(UINT nFlags, CPoint point);
	void MousseLeftDown(UINT nFlags, CPoint point);
	void MousseLeftUp(UINT nFlags, CPoint point);
	void MousseMovedOnView(UINT nFlags, CPoint point);
	// Le Rectangle qui correspond à la zone de l'image dans le contexte d'affichage
	CRect m_ZoneViseur;
	// Le Point de l'intersection dans la croix du viseur, au cas ou elle serait ailleur dans le viseur,... peut etre pour plus tard.
	CPoint m_CentreCroix;
	// Set la m_ZoneViseur du Fit bien comme il faut.
	void SetZoneViseur(CPoint Centre, CPoint& To);
	// Dessine le trait de reference sur l'image
	void DessineTraitReference(CDC* pDC);
	// Dessine le Viseur	
	void DessineViseur(CDC* pDC);
	// Dessine une coupe d'un bitmap dans un rectangle dans un CDC (par defaut:le dc de m_View)
	// La ZoneCoupe doit en fait etre une ligne vertical ou horizontal (pour l'instant)
	// plus tard, peut etre il y aura des coupes en biais, d'un coin à l'autre de ZoneCoupe
	void DessineCoupe(CDibImagePtr& unPtrDib, CRect ZoneCoupe, CRect UnRect, CDC* pUnDc = NULL);

	double m_SommePixelViseur;
	double m_SaturesPixelViseur;

public:
	DECLARE_EVENTSINK_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void ValueChangedFitAdjust01(BOOL Value){	m_b[1 ]	=	Value? 1: 0;	ActualiseFitParameters(1 );};
	void ValueChangedFitAdjust02(BOOL Value){	m_b[2 ]	=	Value? 1: 0;	ActualiseFitParameters(2 );};
	void ValueChangedFitAdjust03(BOOL Value){	m_b[3 ]	=	Value? 1: 0;	ActualiseFitParameters(3 );};
	void ValueChangedFitAdjust04(BOOL Value){	m_b[4 ]	=	Value? 1: 0;	ActualiseFitParameters(4 );};
	void ValueChangedFitAdjust05(BOOL Value){	m_b[5 ]	=	Value? 1: 0;	ActualiseFitParameters(5 );};
	void ValueChangedFitAdjust06(BOOL Value){	m_b[6 ]	=	Value? 1: 0;	ActualiseFitParameters(6 );};
	void ValueChangedFitAdjust07(BOOL Value){	m_b[7 ]	=	Value? 1: 0;	ActualiseFitParameters(7 );};
	void ValueChangedFitAdjust08(BOOL Value){	m_b[8 ]	=	Value? 1: 0;	ActualiseFitParameters(8 );};
	void ValueChangedFitAdjust09(BOOL Value){	m_b[9 ]	=	Value? 1: 0;	ActualiseFitParameters(9 );};
	void ValueChangedFitAdjust10(BOOL Value){	m_b[10]	=	Value? 1: 0;	ActualiseFitParameters(10);};
	void ValueChangedFitAdjust11(BOOL Value){	m_b[11]	=	Value? 1: 0;	ActualiseFitParameters(11);};
	void ValueChangedFitAdjust12(BOOL Value){	m_b[12]	=	Value? 1: 0;	ActualiseFitParameters(12);};
	void ValueChangedFitAdjust13(BOOL Value){	m_b[13]	=	Value? 1: 0;	ActualiseFitParameters(13);};
	void ValueChangedFitAdjust14(BOOL Value){	m_b[14]	=	Value? 1: 0;	ActualiseFitParameters(14);};
	void ValueChangedFitAdjust15(BOOL Value){	m_b[15]	=	Value? 1: 0;	ActualiseFitParameters(15);};
	void ValueChangedFitAdjust16(BOOL Value){	m_b[16]	=	Value? 1: 0;	ActualiseFitParameters(16);};
	void ValueChangedFitStart01(BLABLA00){		m_f[1 ]	=	Value->dblVal;	ActualiseFitParameters(1 );};
	void ValueChangedFitStart02(BLABLA00){		m_f[2 ]	=	Value->dblVal;	ActualiseFitParameters(2 );};
	void ValueChangedFitStart03(BLABLA00){		m_f[3 ]	=	Value->dblVal;	ActualiseFitParameters(3 );};
	void ValueChangedFitStart04(BLABLA00){		m_f[4 ]	=	Value->dblVal;	ActualiseFitParameters(4 );};
	void ValueChangedFitStart05(BLABLA00){		m_f[5 ]	=	Value->dblVal;	ActualiseFitParameters(5 );};
	void ValueChangedFitStart06(BLABLA00){		m_f[6 ]	=	Value->dblVal;	ActualiseFitParameters(6 );};
	void ValueChangedFitStart07(BLABLA00){		m_f[7 ]	=	Value->dblVal;	ActualiseFitParameters(7 );};
	void ValueChangedFitStart08(BLABLA00){		m_f[8 ]	=	Value->dblVal;	ActualiseFitParameters(8 );};
	void ValueChangedFitStart09(BLABLA00){		m_f[9 ]	=	Value->dblVal;	ActualiseFitParameters(9 );};
	void ValueChangedFitStart10(BLABLA00){		m_f[10]	=	Value->dblVal;	ActualiseFitParameters(10);};
	void ValueChangedFitStart11(BLABLA00){		m_f[11]	=	Value->dblVal;	ActualiseFitParameters(11);};
	void ValueChangedFitStart12(BLABLA00){		m_f[12]	=	Value->dblVal;	ActualiseFitParameters(12);};
	void ValueChangedFitStart13(BLABLA00){		m_f[13]	=	Value->dblVal;	ActualiseFitParameters(13);};
	void ValueChangedFitStart14(BLABLA00){		m_f[14]	=	Value->dblVal;	ActualiseFitParameters(14);};
	void ValueChangedFitStart15(BLABLA00){		m_f[15]	=	Value->dblVal;	ActualiseFitParameters(15);};
	void ValueChangedFitStart16(BLABLA00){		m_f[16]	=	Value->dblVal;	ActualiseFitParameters(16);};

	void ActualiseFitParameters(int iemeParam);
	void ValueChangedFitToTraiti(BOOL Value);
	afx_msg void OnCbnSelchangeFitComboTheFit();
	void ValueChangedFitCheckFit(BOOL Value);

	void ValueChangedFitCheckResetStart(BOOL Value);
	void ValueChangedFitCheckFromPrevious(BOOL Value);
	void ValueChangedFitWriteProfileOnDib(BOOL Value);
	void ValueChangedFitCheckCabbages(BOOL Value);
	void ValueChangedFitCheckDiscaredSat(BOOL Value);
	void AddValueToGraph(double ValueToAppend, unsigned int TimeInMilliSec);
	void ValueChangedFitCheckHighest(BOOL Value);
	void ValueChangedFitEditMaxIter(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedFitCheckStopStable(BOOL Value);
	//void PlotMouseDownFitGraph01(short* Button, short* Shift, VARIANT* xData, VARIANT* YData, short* PlotIndex, long* PointIndex);
	afx_msg void OnLvnEndlabeleditFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult);
	void CalculateExpression();
	void ResetStatistics();
	void CalculateStatistics_WithThose_ValeurCalcul();
	void ValueChangedFitCheckAverage(BOOL Value);
};

