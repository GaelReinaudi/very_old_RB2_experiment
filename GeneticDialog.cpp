// GeneticDialog.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "resource.h"
#include "SettingSave.h"

#define INSTANTIATE_REAL_GENOME
#define GALIB_USE_BORLAND_INST
#include <ga/ga.h>
#include <ga/GARealGenome.h>

#include ".\geneticdialog.h"
// redeffinission de max et min car ga.h les #undef...
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


enum{ItemName, ItemCurrentValue, ItemFrom, ItemTo, ItemStep};
#define NOT_COMUNE -55555
#define N_BEST_GENOME_POPULATION 9
#define ParamGeneticIndex(index) (m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(index)))
// CGeneticDialog
CList_ParametrePointeur CGeneticDialog::m_ListParamGenetic;
CNiGraph CGeneticDialog::m_Graph1;

IMPLEMENT_DYNAMIC(CGeneticDialog, CProDialog)
CGeneticDialog::CGeneticDialog()
:  m_SaveGenetic("Genetic Algorithm", "gen", false)
, m_TypeAlgo(0)
{
	pLeAlgoGene = NULL;
	pLeGenome = NULL;
	m_IsEvolving = false;
	m_LetsStartSequenceInIdle = false;
	m_WaitResultEvaluationIdle = false;
	m_SequenceLancee = false;
	m_TimeMilliSecWhenSequenceLancee = 0;
	m_ResultOfIdleEvaluation = -1.0;
	pLeThreadEvolution = NULL;
	m_IndexSave = -1;
	m_ListParamGenetic.RemoveAll();
	m_nParamGenetic = 0;

	CString nameSave(SAVE_DIALOG_GENETIC_FILE_NAME);
	// d'abord le nombre de param de l'algorithm pour pouvoir charger le bon nombre ensuite
	m_Constr_nParam = m_SaveGenetic.GetIntSettings(nameSave, "m_nParamGenetic");
	if(m_Constr_nParam == PARAM_INT_NON_TROUVE)
		m_Constr_nParam = 0;
	new CParametre(&m_SaveGenetic, Entier, "Nomber Param Algo", "m_nParamGenetic", Auto, &m_nParamGenetic, "");
	new CParametre(&m_SaveGenetic, Entier, "Which Save", "m_IndexSave", Auto, &m_IndexSave, "0");
	new CParametre(&m_SaveGenetic, Entier, "List of Index In the Save", "m_ListIndexInSave", Auto, &m_ListIndexInSave, "-1", CanParticipateToNothing, max(1, m_Constr_nParam));
	new CParametre(&m_SaveGenetic, Double, "List of From In the Save", "m_ListFromInSave" , Auto, &m_ListFromInSave, "-1", CanParticipateToNothing, max(1, m_Constr_nParam));
	new CParametre(&m_SaveGenetic, Double, "List of To In the Save"  , "m_ListToInSave"   , Auto, &m_ListToInSave, "-1", CanParticipateToNothing, max(1, m_Constr_nParam));
	new CParametre(&m_SaveGenetic, Double, "List of Step In the Save", "m_ListStepInSave" , Auto, &m_ListStepInSave, "-1", CanParticipateToNothing, max(1, m_Constr_nParam));
	new CParametre(&m_SaveGenetic, Entier, "Poulation Size", "m_PopuSize", Auto, &m_PopuSize, "100");
	new CParametre(&m_SaveGenetic, Entier, "Generation Number", "m_Generation", Auto, &m_Generation, "100");
	new CParametre(&m_SaveGenetic, Entier, "Type Algorithm", "m_TypeAlgo", Auto, &m_TypeAlgo, "0");
	new CParametre(&m_SaveGenetic, Double, "CrossOver Proba", "m_CrossOverProba", Auto, &m_CrossOverProba, "0.5");
	new CParametre(&m_SaveGenetic, Double, "Mutation Proba", "m_MutationProba", Auto, &m_MutationProba, "0.01");
	m_SaveGenetic.SetAllSettings(nameSave, false);
}
CGeneticDialog::~CGeneticDialog()
{	if(pLeAlgoGene)
	{	delete pLeAlgoGene;
		pLeAlgoGene = NULL;
	}
	if(pLeGenome)
	{	delete pLeGenome;
		pLeGenome = NULL;
	}
}
BOOL CGeneticDialog::OnInitDialogBar()
{
	CProDialog::OnInitDialogBar();
	return TRUE;
}

void CGeneticDialog::DoDataExchange(CDataExchange* pDX)
{
	CProDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GENETIC_GRAPH_1			, m_Graph1);
	DDX_Control(pDX, IDC_GENETIC_COMBO_SAVE			, m_ComboSave);
	DDX_Control(pDX, IDC_GENETIC_COMBO_TYPE_ALGO	, m_ComboTypeAlgo);
	DDX_Control(pDX, IDC_GENETIC_LIST_PARAM_DISPO	, m_ListCtrlParamDispo);
	DDX_Control(pDX, IDC_GENETIC_LIST_PARAM_GENETIC	, m_ListCtrlParamGenetic);
	DDX_Control(pDX, IDC_GENETIC_LIST_BEST_GENOMES	, m_ListCtrlBestGenomes);
	DDX_Control(pDX, IDC_GENETIC_EDIT_VALUE			, m_NiValue);
	DDX_Control(pDX, IDC_GENETIC_EDIT_FROM			, m_NiFrom);
	DDX_Control(pDX, IDC_GENETIC_EDIT_TO			, m_NiTo);
	DDX_Control(pDX, IDC_GENETIC_EDIT_STEP			, m_NiStep);
	DDX_Control(pDX, IDC_GENETIC_CHECK_START		, m_NiStart);
	DDX_Control(pDX, IDC_GENETIC_EDIT_POPU_SIZE		, m_NiPopuSize);
	DDX_Control(pDX, IDC_GENETIC_EDIT_GENERATION			, m_NiGeneration		);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CROSS_OVER	, m_NiCrossOver);
	DDX_Control(pDX, IDC_GENETIC_EDIT_MUTATION		, m_NiMutation);

	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_GENERATION	, m_NiCurrentGeneration);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_CONVERGENCE	, m_NiCurrentConvergence);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_CROSSOVER		, m_NiCurrentCrossOver	);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_DEVIATION		, m_NiCurrentDeviation	);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_DIVERSITY		, m_NiCurrentDiversity	);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_MAXSCORE		, m_NiCurrentMaxScore	);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_MEANSCORE		, m_NiCurrentMeanScore	);
	DDX_Control(pDX, IDC_GENETIC_EDIT_CURRENT_MUTATION		, m_NiCurrentMutation	);
}

BEGIN_MESSAGE_MAP(CGeneticDialog, CProDialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_GENETIC_COMBO_SAVE, OnCbnSelchangeGeneticComboSave)
	ON_NOTIFY(NM_CLICK, IDC_GENETIC_LIST_PARAM_GENETIC, OnNMClickGeneticListParamGenetic)
	ON_NOTIFY(LVN_KEYDOWN, IDC_GENETIC_LIST_PARAM_GENETIC, OnLvnKeydownGeneticListParamGenetic)
	ON_NOTIFY(NM_DBLCLK, IDC_GENETIC_LIST_PARAM_DISPO, OnNMDblclkGeneticListParamDispo)
	ON_CBN_SELCHANGE(IDC_GENETIC_COMBO_TYPE_ALGO, OnCbnSelchangeGeneticComboTypeAlgo)
	ON_NOTIFY(NM_CLICK, IDC_GENETIC_LIST_BEST_GENOMES, OnNMClickGeneticListBestGenomes)
END_MESSAGE_MAP()
BEGIN_EVENTSINK_MAP(CGeneticDialog, CProDialog)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_CHECK_START, 1, ValueChangedGeneticCheckStart, VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_VALUE, 1, ValueChangedGeneticEditValue, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_FROM, 1, ValueChangedGeneticEditFrom, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_TO, 1, ValueChangedGeneticEditTo, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_STEP, 1, ValueChangedGeneticEditStep, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_POPU_SIZE, 1, ValueChangedGeneticEditPopuSize, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_GENERATION, 1, ValueChangedGeneticEditGeneration, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_CROSS_OVER, 1, ValueChangedGeneticEditCrossOver, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CGeneticDialog, IDC_GENETIC_EDIT_MUTATION, 1, ValueChangedGeneticEditMutation, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
END_EVENTSINK_MAP()
void CGeneticDialog::ConfigureGa(GAGenome::Evaluator UneFonctionEvaluation) 
{
	if(pLeAlgoGene)
	{	delete pLeAlgoGene;
		pLeAlgoGene = NULL;
	}
	if(pLeGenome)
	{	delete pLeGenome;
		pLeGenome = NULL;
	}
	// voir "ex21.C":
	// This genome is created using an array of allele sets.  This means that each
	// element of the genome will assume a value in its corresponding allele set.
	// For example, since the first allele set is [0,10], the first element of the
	// genome will be in [0,10].  Notice that you can add allele sets in many other
	// ways than those shown.

	if(m_ListParamGenetic.GetCount() < 1)
		return;

	GARealAlleleSetArray LesAlleles;
	// chopons tous les ParamGenetic
	for(int indexParam = 0; indexParam < m_ListParamGenetic.GetCount(); indexParam++)
	{
		CParametre& LeParam = *ParamGeneticIndex(indexParam);
		// ajoutons un allele, fixe si la ligne de la liste n'est pas cochée
		if(ListView_GetCheckState(m_ListCtrlParamGenetic.GetSafeHwnd( ), indexParam))
			if(LeParam.m_Step != 0.0)
				LesAlleles.add(LeParam.m_From, LeParam.m_To, LeParam.m_Step);
			else
				LesAlleles.add(LeParam.m_From, LeParam.m_To);
		else if(LeParam.m_Type == Entier)
			LesAlleles.add(LeParam.Get_Val_Int(), LeParam.Get_Val_Int(), 0.0);
		else if(LeParam.m_Type == Double)
			LesAlleles.add(LeParam.Get_Val_Double(), LeParam.Get_Val_Double(), 0.0);
	}

	// on en fait un génome
	pLeGenome = new GARealGenome(LesAlleles, UneFonctionEvaluation);

	// Les parametre de l'algo
	// Now that we have the genomes, create a parameter list that will be used for
	// all of the genetic algorithms and all of the genomes.
	GAParameterList LesParametresAlgo;
	GASteadyStateGA::registerDefaultParameters(LesParametresAlgo);
	LesParametresAlgo.set(gaNpopulationSize, m_PopuSize);
	LesParametresAlgo.set(gaNnGenerations, m_Generation);
	LesParametresAlgo.set(gaNpCrossover, m_CrossOverProba);	// probability of crossover
	LesParametresAlgo.set(gaNpMutation, m_MutationProba);	// probability of mutation
	LesParametresAlgo.set(gaNscoreFrequency, 1);
	LesParametresAlgo.set(gaNnBestGenomes, N_BEST_GENOME_POPULATION);
	LesParametresAlgo.set(gaNflushFrequency, 50);
	LesParametresAlgo.set(gaNselectScores, (int)GAStatistics::AllScores);

	// L'algorithm a proprement parler
	switch(m_TypeAlgo)
	{
		case 0:	
			pLeAlgoGene = new GASimpleGA(*pLeGenome); 
			break;
		case 1: 
			pLeAlgoGene = new GASteadyStateGA(*pLeGenome); 
			break;
		case 2: 
			pLeAlgoGene = new GAIncrementalGA(*pLeGenome); 
			break;
	}
	pLeAlgoGene->parameters(LesParametresAlgo);
	pLeAlgoGene->set(gaNscoreFilename, "ResultatsAlgoGene.dat");
}


void CGeneticDialog::ValueChangedGeneticCheckStart(BOOL Value)
{
	if(Value)
	{
//		CString fname = CString("SaveBeforeGenetic");
//		m_pMainFrame->m_Save.SaveSettings(fname);
		//		m_pMainFrame->m_Save.SaveSettings(CString("C:\\Rb2Manip_GeneticBestOf\\") + fname);

		m_IsEvolving = false;
		m_LetsStartSequenceInIdle = false;
		m_WaitResultEvaluationIdle = false;
		m_SequenceLancee = false;
		m_TimeMilliSecWhenSequenceLancee = 0;
		m_ResultOfIdleEvaluation = -1.0;

		ConfigureGa(FonctionEvaluationQuiLanceUneSequenceEtAttendLeResultatDuFitImage);
		m_Graph1.ClearData();
		
		// mise en memoire des param initiaux
		m_pInitialParams = CurrentValuesToGenome().clone();
		// et on l'ajoute à la population initiale
        ((GAPopulation&)pLeAlgoGene->population()).replace(m_pInitialParams->clone());

		pLeThreadEvolution = AfxBeginThread((AFX_THREADPROC)StartEvolution, this);
	}
	else
	{
//		m_pMainFrame->m_CamDialog.ValueChangedCamCheckGrab(false);
		m_IsEvolving = false;
//		m_LetsStartSequenceInIdle = false;
//		m_WaitResultEvaluationIdle = false;
//		m_SequenceLancee = false;
///		m_TimeMilliSecWhenSequenceLancee = 0;
//		m_ResultOfIdleEvaluation = -1.0;
//		pLeThreadEvolution = NULL;
//		pLeAlgoGene->flushScores();
		Sleep(1000);
//		m_pMainFrame->m_CamDialog.ValueChangedCamCheckGrab(true);
	}
}
UINT CGeneticDialog::StartEvolution(LPVOID pParam)
{
	CGeneticDialog* pThisDialog = reinterpret_cast<CGeneticDialog*>(pParam);
	if(pThisDialog->pLeAlgoGene == NULL || pThisDialog->m_IsEvolving/*already*/)
		return 0;
	GAGeneticAlgorithm* pTheAlgo = (GAGeneticAlgorithm*)(pThisDialog->pLeAlgoGene);
    //--------------------------------------------------------------------------
    //  Because this thread is going to access an ActiveX control, it must
    //  call CoInitialize.
    //--------------------------------------------------------------------------
    ::CoInitialize(NULL);

	pThisDialog->m_IsEvolving = true; // !!
	pThisDialog->m_pInitialParams->evaluate();
    pTheAlgo->initialize();
	int nGeneration = 0;
	while(!pTheAlgo->done() && pThisDialog->m_IsEvolving)
	{
		pTheAlgo->step();
		nGeneration++;
		pThisDialog->ActualiseGAStatistics(true);
		Sleep(100);
	}

	::CoInitialize(NULL);

	pThisDialog->m_NiStart.SetValue(false);
	return 0;
}

BOOL CGeneticDialog::OnIdle(LONG lCount )
{
	if( ! (m_WaitResultEvaluationIdle || m_LetsStartSequenceInIdle) )
		return FALSE;
	if(!m_IsEvolving)
	{
		m_NiStart.SetValue(false);
		return FALSE;
	}
	// si on attend pas encore le resultat du fit, c'est qu'il faut commencer par lancer une séquence
	if(!m_WaitResultEvaluationIdle && m_LetsStartSequenceInIdle && !m_SequenceLancee)
	{
		GARealGenome& genome = (GARealGenome&)(*m_pTheGenomeToEvaluate);
		// if we are The algo is waiting a value
		ActualiseParam_LanceUneSequence(genome);
		return TRUE;
	}
	// si la séquence est lancée et qu'elle s'est probablement fini (en fonction du temps), 
	// alors, on peut attendre le fit result
	if(!m_WaitResultEvaluationIdle && m_SequenceLancee
	   && (GetTimePrecisMilliSecondeScinceComputerON() > m_TimeMilliSecWhenSequenceLancee + m_DureeDeLaSequenceMilliSec))
	{
		//AfxMessageBox("m_WaitResultEvaluationIdle = true Now?");
		// and NOW, we wait for the fit
		m_WaitResultEvaluationIdle = true;
		m_SequenceLancee = false;
		return FALSE;
	}
	// si on a déjà lancer une séquence (dans un Idle anterieur), on attend le Fit result
	if(m_WaitResultEvaluationIdle)
	{
		if(IsFitResultReady(0, m_ResultOfIdleEvaluation))
		{
			// we say that it is ok to read m_ResultOfIdleEvaluation
			m_LetsStartSequenceInIdle = false;
			m_SequenceLancee = false;
			m_WaitResultEvaluationIdle = false;
		}
	}
return TRUE;
}
unsigned int CGeneticDialog::GetTimePrecisMilliSecondeScinceComputerON()
{	
	LARGE_INTEGER Ticks, Freq;
	QueryPerformanceCounter(&Ticks); // renvoye le nombre de ticks du High-resolutin-counter
	QueryPerformanceFrequency(&Freq);// et sa fréquence
	unsigned int LesMilliSec = (unsigned int)(Ticks.QuadPart / (Freq.QuadPart / 1000.0));
	return LesMilliSec;
}

void CGeneticDialog::ActualiseGAStatistics(bool OnStepFinished /* = false */)
{
	if(!m_IsEvolving)
		return;
	GAStatistics& statistics = (GAStatistics&)pLeAlgoGene->statistics();
	int nGene = statistics.generation();
	unsigned long nCrossOverEver = statistics.crossovers();
	unsigned long nMutationEver = statistics.mutations();
	unsigned long nReplacement = statistics.replacements();
	float Convergence = statistics.convergence();
    
	m_NiCurrentGeneration	.SetValue(nGene);
	m_NiCurrentConvergence	.SetValue(Convergence);
	m_NiCurrentCrossOver	.SetValue(nCrossOverEver);
	m_NiCurrentMutation		.SetValue(nMutationEver);


	if(!OnStepFinished)
		return;
	GARealGenome& bestIndividualEver = (GARealGenome&)statistics.bestIndividual();
	GARealGenome& bestIndividualGene = (GARealGenome&)pLeAlgoGene->population().best();
//	bestIndividualGene.evaluate(gaTrue);
	//bestIndividualEver.evaluate(gaTrue);

	GAPopulation& bestPopulation = (GAPopulation&)statistics.bestPopulation();
	bestPopulation.sort(gaFalse);
	ActualiseGABestGenomes(bestPopulation);

	float BestScoreEver = statistics.maxEver();
	//statistics.nBestGenomes();
	//statistics.nConvergence();
	//statistics.popEvals();
	//statistics.recordDiversity();
	float CurrentGene_MaxScore		= statistics.current(statistics.Maximum);
	float CurrentGene_MinScore		= statistics.current(statistics.Minimum);
	float CurrentGene_MeanScore		= statistics.current(statistics.Mean);
	float CurrentGene_Deviation		= statistics.current(statistics.Deviation);
	float CurrentGene_Diversity		= statistics.current(statistics.Diversity);
	float AverageOfMaximumScores	= statistics.offlineMax();
	float AverageOfAllScores		= statistics.online();
	m_NiCurrentDeviation	.SetValue(CurrentGene_Deviation);
	m_NiCurrentDiversity	.SetValue(CurrentGene_Diversity);
	m_NiCurrentMaxScore		.SetValue(CurrentGene_MaxScore);
	m_NiCurrentMeanScore	.SetValue(CurrentGene_MeanScore);


	if(OnStepFinished)
	{
		AddGenerationToGraph(nGene, CurrentGene_MaxScore, CurrentGene_MeanScore, BestScoreEver);
		CString fname;
		fname.Format("Generation_%s%s%s%d"
			, int(log10(double(nGene)/100)) > 0?"":"0"
			, int(log10(double(nGene)/10))  > 0?"":"0"
			, int(log10(double(nGene)))     > 0?"":"0"
			, nGene);
//		SaveGenome(bestIndividualGene, CString("C:\\Rb2Manip_GeneticBestOf\\") + fname);
	}
}
void CGeneticDialog::ActualiseGABestGenomes(GAPopulation& popuBest)
{
	CString score;
	m_ListCtrlBestGenomes.DeleteAllItems();
	m_ListCtrlBestGenomes.InsertItem(0, "Initial :");
	score.Format("%4g", m_pInitialParams->fitness());
	m_ListCtrlBestGenomes.SetItemText(0, 1, score);
	for(int i = 1; i <= N_BEST_GENOME_POPULATION; i++)
	{
		score.Format("%4g", popuBest.individual(i - 1).fitness());
		m_ListCtrlBestGenomes.InsertItem(i, score);
		//m_ListCtrlBestGenomes.SetItemText(i, 1, score);
	}
}
GARealGenome CGeneticDialog::CurrentValuesToGenome()
{
	GARealAlleleSetArray LesAlleles;
	// chopons tous les ParamGenetic
	for(int indexParam = 0; indexParam < m_ListParamGenetic.GetCount(); indexParam++)
	{
		CParametre& LeParam = *ParamGeneticIndex(indexParam);
		// ajoutons un allele, fixe si la ligne de la liste n'est pas cochée
		if(LeParam.m_Type == Entier)
			LesAlleles.add(LeParam.Get_Val_Int(), LeParam.Get_Val_Int(), 0.0);
		else if(LeParam.m_Type == Double)
			LesAlleles.add(LeParam.Get_Val_Double(), LeParam.Get_Val_Double(), 0.0);
	}
return GARealGenome(LesAlleles, FonctionEvaluationQuiLanceUneSequenceEtAttendLeResultatDuFitImage);
}
void CGeneticDialog::SaveGenome(GAGenome& TheGenome, CString filename)
{
	CGeneticDialog& GeneticDialog = *(CGeneticDialog*)(this);//&m_pMainFrame->m_GeneticDialog);
	GARealGenome& genome = (GARealGenome&)TheGenome;
	// on fait un CSettingSave temporaire pour sauver les parm temporarire du genome.
	CSettingSave SaveTemporaire("SaveTemporaireGenetic", "genome", false);
	CParametre* pParamTempGene[MAX_PARAM_GENETIC];

	// affectons les valeurs aux parametres
	for(int indexParam = 0; indexParam < genome.length(); indexParam++)
	{
		CParametre& LeParam = *(GeneticDialog.m_ListParamGenetic.GetAt(GeneticDialog.m_ListParamGenetic.FindIndex(indexParam)));
		if(LeParam.m_Type == Entier)
			LeParam.Set_Val(int(genome.gene(indexParam)));
		if(LeParam.m_Type == Double)
			LeParam.Set_Val(genome.gene(indexParam));

		pParamTempGene[indexParam] = new CParametre(&SaveTemporaire, LeParam.m_Type, LeParam.m_NomTexte, LeParam.m_NomSave, Auto, LeParam.Get_pVal(), "");
	}
	SaveTemporaire.SaveSettings(filename);
	for(int indexParam = 0; indexParam < genome.length(); indexParam++)
	{
		delete (pParamTempGene[indexParam]);
	}
}

void CGeneticDialog::OnPaint()
{
	CPaintDC dc(this);
	if(!m_bWasPaintedOnce)
	{
		m_bWasPaintedOnce = true;
		// ajout des SettingSave pouvant participer
		m_TotalIndexSave = 0;
		FOR_ALL_SAVE_SETTING_DEFINED_do
			if(pLeSave->m_ContientTrucPouvantVarier)
			{
				// on index l'adresse:
				m_pSaveIndex[m_TotalIndexSave] = pLeSave;
				m_TotalIndexSave++;
			}
		}
		// Let's put all the param that were in the algorithm
		if(m_ListIndexInSave[0] >= 0)
		for(int iemeListIndexInSave = 0; iemeListIndexInSave < m_Constr_nParam; iemeListIndexInSave++)
		{
			AddIemeParamFromTheListDispo(m_ListIndexInSave[iemeListIndexInSave]
										, m_ListFromInSave[iemeListIndexInSave]
										, m_ListToInSave[iemeListIndexInSave]
										, m_ListStepInSave[iemeListIndexInSave]);
		}
		// ajout des SettingSave pouvant participer
		for(int iemeSave = 0; iemeSave < m_TotalIndexSave; iemeSave++)
		{
			CString NameDuSave = m_pSaveIndex[iemeSave]->GetName();
			m_ComboSave.AddString(NameDuSave);
		}
		// fabrication d'une jolie ListeCtrl pour les param dispo
		m_ListCtrlParamDispo.InsertColumn(ItemName, "Description", LVCFMT_LEFT, 150);
		m_ListCtrlParamDispo.InsertColumn(ItemName, "Name Save", LVCFMT_LEFT, 150);
		// et pour les param de l'algo
		m_ListCtrlParamGenetic.SetExtendedStyle(m_ListCtrlParamGenetic.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
		m_ListCtrlParamGenetic.InsertColumn(ItemName, "Name", LVCFMT_LEFT, 100);
		m_ListCtrlParamGenetic.InsertColumn(ItemCurrentValue, "Value", LVCFMT_LEFT, 50);
		m_ListCtrlParamGenetic.InsertColumn(ItemFrom, "From", LVCFMT_LEFT, 50);
		m_ListCtrlParamGenetic.InsertColumn(ItemTo	, "To"	, LVCFMT_LEFT, 50);
		m_ListCtrlParamGenetic.InsertColumn(ItemStep, "Step", LVCFMT_LEFT, 40);

		ActualiseListCtrlParamGenetic();

		m_ListCtrlBestGenomes.SetExtendedStyle(m_ListCtrlBestGenomes.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
		m_ListCtrlBestGenomes.InsertColumn(0, "Generation", LVCFMT_LEFT, 70);
		m_ListCtrlBestGenomes.InsertColumn(1, "Score", LVCFMT_LEFT, 70);

		m_ComboTypeAlgo.AddString("SIMPLE");
		m_ComboTypeAlgo.AddString("STEADY_STATE");
		m_ComboTypeAlgo.AddString("INCREMENTAL");
		m_ComboTypeAlgo.SetCurSel(m_TypeAlgo);
		m_ComboSave.SetCurSel(m_IndexSave);
		OnCbnSelchangeGeneticComboSave();
	}
	ActualiseListCtrlParamGenetic(false);
}

void CGeneticDialog::ActualiseListCtrlParamDispo()
{
	// néttoyage
	m_ListCtrlParamDispo.DeleteAllItems();
	// de quelle save s'agit t'il?
	m_IndexSave = m_ComboSave.GetCurSel();
	if(m_IndexSave < 0 || m_IndexSave >= MAX_SAVE_DISPO)
		return;
	// remplissons la ListParamSave
	CSettingSave* pLeSave = m_pSaveIndex[m_IndexSave];
	int iemeParamThatCanBePartOfGeneticAlgo = -1;
	FOR_ALL_PARAMETERS_IN_THIS_SAVE_do(pLeSave)
		if(pLeParam->m_CanParticipateTo & CanParticipateGeneticAlgo)
		{
			iemeParamThatCanBePartOfGeneticAlgo++;
			m_ListCtrlParamDispo.InsertItem(iemeParamThatCanBePartOfGeneticAlgo, pLeParam->m_NomSave);
			m_ListCtrlParamDispo.SetItemText(iemeParamThatCanBePartOfGeneticAlgo, 1, pLeParam->m_NomTexte);
		}
	}
}

void CGeneticDialog::ActualiseListCtrlParamGenetic(bool DeleteAll /* = true */)
{
	m_NiPopuSize.SetValue(m_PopuSize);
	m_NiGeneration.SetValue(m_Generation);
	m_NiCrossOver.SetValue(m_CrossOverProba);
	m_NiMutation.SetValue(m_MutationProba);

	//// néttoyage
	if(DeleteAll || m_ListParamGenetic.GetCount() != m_ListCtrlParamGenetic.GetItemCount())
	{
		m_ListCtrlParamGenetic.DeleteAllItems();
		m_nParamGenetic = 0;
	}

	CString strFrom, strValue, strTo, strStep;
	// remplissons la List
	m_Constr_nParam = m_ListParamGenetic.GetCount();
	for(int i = 0; i < m_Constr_nParam; i++)
	{
		CParametre& LePar = *ParamGeneticIndex(i);
		// ajout de lignes si on le demande, ou si le nombre de ligne a changé
		if(DeleteAll || m_ListParamGenetic.GetCount() != m_ListCtrlParamGenetic.GetItemCount())
		{
			m_ListCtrlParamGenetic.InsertItem(i, LePar.m_NomTexte);
			ListView_SetCheckState(m_ListCtrlParamGenetic.GetSafeHwnd( ), i, true);
			m_nParamGenetic++;
		}
		switch(LePar.m_Type)
		{
		case Entier:	
			strFrom.Format("%d", LePar.m_From); 
			strValue.Format("%d", LePar.Get_Val_Int()); 
			strTo.Format("%d", LePar.m_To); 
			strStep.Format(LePar.m_Step ? "%d" : "Continu", LePar.m_Step); break;
		case Double:	
			strFrom.Format("%f", LePar.m_From); 
			strValue.Format("%f", LePar.Get_Val_Double()); 
			strTo.Format("%f", LePar.m_To); 
			strStep.Format(LePar.m_Step ? "%f" : "Continu", LePar.m_Step); break;
		}
		m_ListCtrlParamGenetic.SetItemText(i, ItemCurrentValue	, strValue	);
		m_ListCtrlParamGenetic.SetItemText(i, ItemFrom			, strFrom	);
		m_ListCtrlParamGenetic.SetItemText(i, ItemTo			, strTo		);
		m_ListCtrlParamGenetic.SetItemText(i, ItemStep			, strStep	);
		m_ListFromInSave[i] = LePar.m_From;
		m_ListToInSave[i] = LePar.m_To;
		m_ListStepInSave[i] = LePar.m_Step;

		// actualisation de la list d'index des param pour sauver dans le fichier de config
		// trouvons d'abord l'index
		int iemeParamThatCanBePartOfGeneticAlgo = -1;
		FOR_ALL_PARAMETERS_IN_THIS_SAVE_do(m_pSaveIndex[m_IndexSave])
			if(pLeParam->m_CanParticipateTo & CanParticipateGeneticAlgo)
				iemeParamThatCanBePartOfGeneticAlgo++;
			if(&LePar == pLeParam) // puis notons la valeur
				m_ListIndexInSave[i] = iemeParamThatCanBePartOfGeneticAlgo;
		}
	}
	CParametre::Param(m_ListIndexInSave)->m_MustBeSaved = m_nParamGenetic;
	CParametre::Param(m_ListFromInSave)->m_MustBeSaved = m_nParamGenetic;
	CParametre::Param(m_ListToInSave)->m_MustBeSaved = m_nParamGenetic;
	CParametre::Param(m_ListStepInSave)->m_MustBeSaved = m_nParamGenetic;
	
	if(m_Constr_nParam == 0)
		m_ListIndexInSave[0] = -1;
}
void CGeneticDialog::AddGenerationToGraph(int nGene, double YBestScoreGene, double YMeanGeneScore, double YBestScoreEver)
{
	m_Graph1.SetChartLength(nGene);
	m_Graph1.GetPlots().Item("BestScoreGene").ChartXY(nGene, YBestScoreGene);
	m_Graph1.GetPlots().Item("MeanScoreGene").ChartXY(nGene, YMeanGeneScore);
	m_Graph1.GetPlots().Item("BestScoreEver").ChartXY(nGene, YBestScoreEver);
	m_Graph1.GetAxes().Item(1).AutoScaleNow();
	m_Graph1.GetAxes().Item(1).SetMinimum(1);
	m_Graph1.GetAxes().Item(2).AutoScaleNow();
	m_Graph1.GetAxes().Item(2).SetMinimum(0);
}

void CGeneticDialog::OnNMClickGeneticListParamGenetic(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Quand on a clické, on actualise les EditBox NiNumEdit en fonction du (des) élément(s) sélectionné(s)
	POSITION pos = m_ListCtrlParamGenetic.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	// on check si des valeurs de value, de from, de to, ou de step sont identique ou non,
	// pour afficher la valeur commune ou non !
	// le premier élément de la selection
	int nItem = m_ListCtrlParamGenetic.GetNextSelectedItem(pos);
	m_ComuneValue	= m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_Type == Entier ? m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->Get_Val_Int(): m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->Get_Val_Double();
	m_ComuneFrom	= m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_Type == Entier ? m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_From		: m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_From;
	m_ComuneTo		= m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_Type == Entier ? m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_To			: m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_To;
	m_ComuneStep	= m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_Type == Entier ? m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_Step		: m_ListParamGenetic.GetAt(m_ListParamGenetic.FindIndex(nItem))->m_Step;

	// on choppe les éléments suivants sélectionnés
	while(pos)
	{
		nItem = m_ListCtrlParamGenetic.GetNextSelectedItem(pos);
		// en cas de non égalité, on fait m_ComuneXXX = NOT_COMUNE
		if(m_ComuneValue != (ParamGeneticIndex(nItem)->m_Type == Entier ? ParamGeneticIndex(nItem)->Get_Val_Int() : ParamGeneticIndex(nItem)->Get_Val_Double()))
			m_ComuneValue = NOT_COMUNE;
		if(m_ComuneFrom	!= (ParamGeneticIndex(nItem)->m_Type == Entier ? ParamGeneticIndex(nItem)->m_From : ParamGeneticIndex(nItem)->m_From))
			m_ComuneFrom = NOT_COMUNE;
		if(m_ComuneTo!= (ParamGeneticIndex(nItem)->m_Type == Entier ? ParamGeneticIndex(nItem)->m_To : ParamGeneticIndex(nItem)->m_To))
			m_ComuneTo = NOT_COMUNE;
		if(m_ComuneStep != (ParamGeneticIndex(nItem)->m_Type == Entier ? ParamGeneticIndex(nItem)->m_Step : ParamGeneticIndex(nItem)->m_Step))
			m_ComuneStep = NOT_COMUNE;
	}

	// si tous on une valeur comune, on l'affiche
	//	if(m_ComuneValue != NOT_COMUNE)
	m_NiValue.SetValue(m_ComuneValue);
	m_NiFrom.SetValue(m_ComuneFrom);
	m_NiTo.SetValue(m_ComuneTo);
	m_NiStep.SetValue(m_ComuneStep);

	*pResult = 0;
}


void CGeneticDialog::ValueChangedGeneticEditValue(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	if(Value->dblVal == NOT_COMUNE)
		return;
	// sinon, on affecte la valeur a tous les paramGenetic sélectionés
	POSITION pos = m_ListCtrlParamGenetic.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	else
	{
		int nItem;
		while(pos)
		{
			nItem = m_ListCtrlParamGenetic.GetNextSelectedItem(pos);
			// en cas de non égalité, on fait m_ComuneXXX = NOT_COMUNE
			if(ParamGeneticIndex(nItem)->m_Type == Entier) 
				ParamGeneticIndex(nItem)->Set_Val((int)Value->dblVal);
			else
				ParamGeneticIndex(nItem)->Set_Val(Value->dblVal);
		}
		m_ComuneValue = Value->dblVal;
		ActualiseListCtrlParamGenetic(false);
	}
}

void CGeneticDialog::ValueChangedGeneticEditFrom(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	if(Value->dblVal == NOT_COMUNE)
		return;
	// sinon, on affecte la valeur a tous les paramGenetic sélectionés
	POSITION pos = m_ListCtrlParamGenetic.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	else
	{
		int nItem;
		while(pos)
		{
			nItem = m_ListCtrlParamGenetic.GetNextSelectedItem(pos);
			// en cas de non égalité, on fait m_ComuneXXX = NOT_COMUNE
			if(ParamGeneticIndex(nItem)->m_Type == Entier) 
				ParamGeneticIndex(nItem)->m_From = (int)Value->dblVal;
			else
				ParamGeneticIndex(nItem)->m_From = Value->dblVal;
		}
		m_ComuneFrom = Value->dblVal;
		ActualiseListCtrlParamGenetic(false);
	}
}

void CGeneticDialog::ValueChangedGeneticEditTo(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	if(Value->dblVal == NOT_COMUNE)
		return;
	// sinon, on affecte la valeur a tous les paramGenetic sélectionés
	POSITION pos = m_ListCtrlParamGenetic.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	else
	{
		int nItem;
		while(pos)
		{
			nItem = m_ListCtrlParamGenetic.GetNextSelectedItem(pos);
			// en cas de non égalité, on fait m_ComuneXXX = NOT_COMUNE
			if(ParamGeneticIndex(nItem)->m_Type == Entier) 
				ParamGeneticIndex(nItem)->m_To = (int)Value->dblVal;
			else
				ParamGeneticIndex(nItem)->m_To = Value->dblVal;
		}
		m_ComuneTo = Value->dblVal;
		ActualiseListCtrlParamGenetic(false);
	}
}

void CGeneticDialog::ValueChangedGeneticEditStep(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	if(Value->dblVal == NOT_COMUNE)
		return;
	// sinon, on affecte la valeur a tous les paramGenetic sélectionés
	POSITION pos = m_ListCtrlParamGenetic.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	else
	{
		int nItem;
		while(pos)
		{
			nItem = m_ListCtrlParamGenetic.GetNextSelectedItem(pos);
			// en cas de non égalité, on fait m_ComuneXXX = NOT_COMUNE
			if(ParamGeneticIndex(nItem)->m_Type == Entier) 
				ParamGeneticIndex(nItem)->m_Step = (int)Value->dblVal;
			else
				ParamGeneticIndex(nItem)->m_Step = Value->dblVal;
		}
		m_ComuneStep = Value->dblVal;
		ActualiseListCtrlParamGenetic(false);
	}
}

void CGeneticDialog::OnLvnKeydownGeneticListParamGenetic(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// Quand on a Préssé Suprimé, on élimine les element
	if(pLVKeyDow->wVKey != VK_DELETE)
		return;

	POSITION pos = m_ListCtrlParamGenetic.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	// on choppe les éléments sélectionnés
	int elementDejaEnleve = 0;
	while(pos)
	{
		int nItem = m_ListCtrlParamGenetic.GetNextSelectedItem(pos);
		m_ListParamGenetic.RemoveAt(m_ListParamGenetic.FindIndex(nItem - elementDejaEnleve));
		elementDejaEnleve++;
	}
	ActualiseListCtrlParamGenetic();
	*pResult = 0;
}

void CGeneticDialog::OnNMDblclkGeneticListParamDispo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// on transfert les element selectionné vers la list des paramGenetic
	POSITION pos = m_ListCtrlParamDispo.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	// on choppe les éléments sélectionnés
	while(pos)
	{
		int nItem = m_ListCtrlParamDispo.GetNextSelectedItem(pos);
		AddIemeParamFromTheListDispo(nItem);
	}
	ActualiseListCtrlParamGenetic();
	*pResult = 0;
}

void CGeneticDialog::AddIemeParamFromTheListDispo(int ieme, double From, double To, double Step /* = -1.0 = -1.0 = -1.0 */)
{
	int iemeParamThatCanBePartOfGeneticAlgo = -1;
	FOR_ALL_PARAMETERS_IN_THIS_SAVE_do(m_pSaveIndex[m_IndexSave])
		if(pLeParam->m_CanParticipateTo & CanParticipateGeneticAlgo)
			iemeParamThatCanBePartOfGeneticAlgo++;
		// Le param en question...
		if(iemeParamThatCanBePartOfGeneticAlgo == ieme)
			// Si il n'est pas dans la liste deja...
			if(m_ListParamGenetic.Find(pLeParam) == NULL)
			{	// on l'ajoute !
				m_ListParamGenetic.AddTail(pLeParam);
				// si on a préciser From To Step
				if(From != -1.0 && To != -1.0 && Step != -1.0)
				{
					pLeParam->m_From = From;
					pLeParam->m_To = To;
					pLeParam->m_Step = Step;
				}
				return;
			}
	}
}
void CGeneticDialog::ValueChangedGeneticEditPopuSize(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_PopuSize = int(Value->dblVal);
}

void CGeneticDialog::ValueChangedGeneticEditGeneration(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_Generation = int(Value->dblVal);
}

void CGeneticDialog::ValueChangedGeneticEditCrossOver(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_CrossOverProba = Value->dblVal;
}

void CGeneticDialog::ValueChangedGeneticEditMutation(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_MutationProba = Value->dblVal;
}
void CGeneticDialog::OnCbnSelchangeGeneticComboSave()
{
	ActualiseListCtrlParamDispo();
}
void CGeneticDialog::OnCbnSelchangeGeneticComboTypeAlgo()
{
	m_TypeAlgo = m_ComboTypeAlgo.GetCurSel();
}

void CGeneticDialog::OnNMClickGeneticListBestGenomes(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
	*pResult = 0;
}

