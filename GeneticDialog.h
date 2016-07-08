#pragma once

#include "ProDialog.h"
#include <ga/ga.h>
#include <ga/GARealGenome.h>

// CGeneticDialog
#define MAX_SAVE_DISPO 100
#define MAX_PARAM_GENETIC 100
#define MAX_PARAM_MAXIMINI 10
#define SAVE_DIALOG_GENETIC_FILE_NAME "SaveGenticDialog"

class CGeneticDialog : public CProDialog
{
	DECLARE_DYNAMIC(CGeneticDialog)

public:
	CGeneticDialog();
	virtual ~CGeneticDialog();

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

	CComboBox m_ComboSave, m_ComboTypeAlgo;
	CListCtrl m_ListCtrlParamDispo, m_ListCtrlParamGenetic, m_ListCtrlBestGenomes;

	// les pointeur vers les Save (Instance de SettingSave) de la liste ComboSave, l'indexage correspond !
	CSettingSave* m_pSaveIndex[MAX_SAVE_DISPO];
	int m_IndexSave, m_TotalIndexSave;
public:
	// pour sauver les parmaetre de l'algo
	CSettingSave m_SaveGenetic;
	afx_msg void OnPaint();

private:
	static CNiGraph m_Graph1;
	CNiButton m_NiStart;
	CNiNumEdit m_NiValue, m_NiFrom, m_NiTo, m_NiStep;
	CNiNumEdit m_NiPopuSize, m_NiGeneration, m_NiCrossOver, m_NiMutation, m_NiCurrentGeneration, m_NiCurrentConvergence, m_NiCurrentCrossOver, m_NiCurrentDeviation, m_NiCurrentDiversity, m_NiCurrentMaxScore, m_NiCurrentMeanScore, m_NiCurrentMutation;

	GAGeneticAlgorithm* pLeAlgoGene;
	GARealGenome* pLeGenome;
	GAGenome* m_pInitialParams;
	// nombre de parametre faisant partis du processus du AlgoGéné
	// Valeur Value From To Step des affichages NumEdit comun aux lignes sélectionnées
	double m_ComuneValue, m_ComuneFrom, m_ComuneTo, m_ComuneStep;
	int m_PopuSize, m_Generation, m_TypeAlgo;
	double m_CrossOverProba, m_MutationProba;
	int m_nParamGenetic, m_Constr_nParam;
	int m_ListIndexInSave[MAX_PARAM_GENETIC];
	double m_ListFromInSave[MAX_PARAM_GENETIC];
	double m_ListToInSave[MAX_PARAM_GENETIC];
	double m_ListStepInSave[MAX_PARAM_GENETIC];
	// l'adresse des CParametres en question
	static CList_ParametrePointeur m_ListParamGenetic;

private:
	// Configure l'algorithm. Cf gademo dans les example windows de GaLib
	void ConfigureGa(GAGenome::Evaluator UneFonctionEvaluation);
	void ActualiseListCtrlParamDispo();
	void ActualiseListCtrlParamGenetic(bool DeleteAll = true);
	void AddGenerationToGraph(int nGene, double YBestScoreGene, double YMeanGeneScore, double YBestScoreEver);
	// ajoute le ieme param de la liste Dipo dans la list Genetic
	void AddIemeParamFromTheListDispo(int ieme, double From = -1.0, double To = -1.0, double Step = -1.0);
	CWinThread* pLeThreadEvolution;
	// actualise les controles qui donne les stat du GA
	void ActualiseGAStatistics(bool OnStepFinished = false);
	// actulaise la liste de popubest
	void ActualiseGABestGenomes(GAPopulation& popuBest);
	// sets a genome with the current values of the parameters
	GARealGenome CurrentValuesToGenome();
	// sauve sur le disque
	void SaveGenome(GAGenome& TheGenome, CString filename);
	// indique que l'algo est en train de fonctionner
	volatile bool m_IsEvolving;
public:
	// OnIdle est la fonction qui sera appellée par le thread de l'algo génétic
	// car enfait, c'est (dans certain cas) compliqué de lancer des fonction depuis un nouveau thread
	// avec des ActiveX par exemple, ou la gestion des Handel de fenetre....etc
	// donc le thread ira choper des info qui seront calculée par Idle
	BOOL OnIdle( LONG lCount );
	unsigned int GetTimePrecisMilliSecondeScinceComputerON();
	void ActualiseParam_LanceUneSequence(GAGenome& TheGenome);
	// indexExpressionSequence est le l'index de l'expression calculée dans la fenetre FitDialog pour renvoyer ce reultat à Fonction evaluation
	// en temps que score du TheGenome à évaluer
	bool IsFitResultReady(int indexExpressionFitDialog , double & ResultatEventuel);
	GAGenome* m_pTheGenomeToEvaluate;
	double m_ResultOfIdleEvaluation;
	// indique qu'on doit lancer une séquence dans Idle
	volatile bool m_LetsStartSequenceInIdle;
	// puis, qu'on doit attendre le resultat du fit dans Idle
	volatile bool m_WaitResultEvaluationIdle;
	// indique que la séquence a effectivement été lancée
	volatile bool m_SequenceLancee;
	// indique l'heure, en milliseconde à laquelle on a démaré la séquence
	volatile unsigned int m_TimeMilliSecWhenSequenceLancee;
	// duree de la séquence lancée
	unsigned int m_DureeDeLaSequenceMilliSec;


	DECLARE_EVENTSINK_MAP()
public:
	static float FonctionEvaluationQuiLanceUneSequenceEtAttendLeResultatDuFitImage(GAGenome& TheGenome);
	static UINT StartEvolution(LPVOID pParam);
	afx_msg void OnCbnSelchangeGeneticComboSave();
	afx_msg void OnNMClickGeneticListParamGenetic(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownGeneticListParamGenetic(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkGeneticListParamDispo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeGeneticComboTypeAlgo();
	void ValueChangedGeneticCheckStart(BOOL Value);
	void ValueChangedGeneticEditValue(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedGeneticEditFrom(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedGeneticEditTo(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedGeneticEditStep(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedGeneticEditPopuSize(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedGeneticEditGeneration(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedGeneticEditCrossOver(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedGeneticEditMutation(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	afx_msg void OnNMClickGeneticListBestGenomes(NMHDR *pNMHDR, LRESULT *pResult);
};


