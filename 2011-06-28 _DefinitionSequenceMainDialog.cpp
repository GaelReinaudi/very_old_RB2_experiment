# include "stdafx.h"
#include "MainDialog.h"
#include "MainFrm.h"

//#define Monst_ON_Ph1_sortie_1 0		
//#define Monst_ON_Ph2_sortie_1 0
//#define Monst_OFF_Ph1_sortie_1 1
//#define Monst_OFF_Ph2_sortie_1 1
//#define Monst_ON_Ph1_sortie_2 1
//#define Monst_ON_Ph2_sortie_2 0
//#define Monst_OFF_Ph1_sortie_2 1
//#define Monst_OFF_Ph2_sortie_2 0
#define Monst_ON_Ph1_sortie_1 0		
#define Monst_ON_Ph2_sortie_1 1
#define Monst_OFF_Ph1_sortie_1 1
#define Monst_OFF_Ph2_sortie_1 0
#define Monst_ON_Ph1_sortie_2 0
#define Monst_ON_Ph2_sortie_2 0
#define Monst_OFF_Ph1_sortie_2 0
#define Monst_OFF_Ph2_sortie_2 0

double uneFonc(double x){return x*x;}

#define XPS_TRACKING_OR_NOT		((m_NiCheck[35].Value) ? m_NiEdit[71].GetValue() : DEFAULT_VALUE) ,	((m_NiCheck[35].Value) ? m_NiEdit[71].GetValue() : DEFAULT_VALUE)

unsigned int CMainDialog::DefinitionSequenceMainDialog(Sequence& LaSeq)
{
	// Loading
	if(m_ParamBool[1])
	{
		if(m_ParamReal[2])
		{
			// on ajoute une Colonne à la fin de la séquence (vide ici)
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Load Zeeman On"
										,	MILLI_SECONDE_INT(m_ParamReal[2]) );
			// on assigne des sorties à la dernière colonne (qui est aussi la premiere puissqu'elle est seule !!!)
			LaSeq.SetCol_32Digitals( LastColonne, 
				/*	_ZEEMAN			,	 0 */	1	,
				/*	_MOT_AOM		,	 1 */	1	,
				/*	_MOT_CLIC		,	 2 */	1	,
				/*	_MAIN_IGBT		,	 3 */	1	,
				/*	_REPUMPS		,	 4 */	m_ParamBool[32]	,
				/*	_IMAG_AOM		,	 5 */	0	,
				/*	_IMAG_CLIC		,	 6 */	0	,
				/*	_LATT_SHUT		,	 7 */	DEFAULT_VALUE	,
				/*	_LAT_BACK		,	 8 */	DEFAULT_VALUE	,
				/*	_NUSD_09		,	 9 */	0	,
				/*	_NUSD_10		,	10 */	0	,
				/*	_2D_MOT			,	11 */	0	,
				/*	_CAM_TRIGGER	,	12 */	0	,
				/*	_NUSD_13		,	13 */	0	,
				/*	_NUSD_14		,	14 */	0	,
				/*	_TRANS_COOL		,	15 */	DEFAULT_VALUE	,
				/*	_NUSD_16		,	16 */	0	,
				/*	_RBROAD			,	17 */	DEFAULT_VALUE	,
				/*	_RSINGLE		,	18 */	DEFAULT_VALUE	,
				/*	_NUSD_19		,	19 */	0	,
				/*	_NUSD_20		,	20 */	0	,
				/*	_NUSD_21		,	21 */	0	,
				/*	_NUSD_22		,	22 */	0
													);	
			// on assigne des sorties analogiques à la dernière colonne en donnant "Quelle_Analogique", "Debut" et "Fin"
			// ou bien DEFAULT_VALUE pour prendre les valeurs du port
			// les voie non-spécifiées continue la valeur de la colonne précédente
			// pour la première colonne, elles prennent DEFAULT_VALUE !!!
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 	
					_MOT_COILS,	m_ParamReal[29]	,	m_ParamReal[29]	
														);
		}
	}
	// Magnetic trap
	if(m_ParamBool[2])
	{
		// col 1
		double BStartValue = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_MOT_COILS).y;
		double BToValue = m_ParamReal[5];
		if(m_ParamReal[4]) {
			LaSeq.InsertNewColonne( AfterLastColonne
				,	"Load No Rep"
				,	MILLI_SECONDE_INT(m_ParamReal[4]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
				_REPUMPS		,	0	
				);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 
			, _MOT_COILS,	BStartValue,	BToValue
			);
		}
		// col 2
		BStartValue = m_ParamReal[5];
		BToValue = m_ParamReal[6];
		if(m_ParamReal[26]) {
			LaSeq.InsertNewColonne( AfterLastColonne
				,	"Load No Rep"
				,	MILLI_SECONDE_INT(m_ParamReal[26]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
				_REPUMPS		,	0	
				);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 
				, _MOT_COILS,	BStartValue,	BToValue
				);
		}
		// col 3
		BStartValue = m_ParamReal[6];
		BToValue = m_ParamReal[7];
		if(m_ParamReal[27]) {
			LaSeq.InsertNewColonne( AfterLastColonne
				,	"Load No Rep"
				,	MILLI_SECONDE_INT(m_ParamReal[27]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
				_REPUMPS		,	0	
				);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne 
				, _MOT_COILS,	BStartValue,	BToValue
				);
		}
	}
	
	// shut off ZS
	if(m_ParamReal[3])
	{
// 		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
// 		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[3]);
// 		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance, _ZEEMAN, 0);

		LaSeq.InsertNewColonne( AfterLastColonne	,	"Load Zeeman Off"
			,	MILLI_SECONDE_INT(m_ParamReal[3]) );
		// on assigne des sorties à la dernière colonne
		LaSeq.SetCol_Digitals_DefaultPrecedentColonne(	LastColonne
			,   _ZEEMAN		,	0
			);	
		LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne
			);
	}
	// turn Rep back on
	if(m_ParamReal[21])
	{
		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[21]);
		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance, _REPUMPS, 1);
	}

	// Compression
	if(0)
	{
		short NCC = 0; // 2 puissance le Nombre de Colonnes pour la Compression
		if(m_ParamReal[4] <= m_ParamReal[26] && m_ParamReal[4] <= m_ParamReal[27])
			NCC = 2;
		else if(m_ParamReal[4] >  m_ParamReal[26] && m_ParamReal[4] >  m_ParamReal[27] && m_ParamReal[26] != m_ParamReal[27])
			NCC = 2 + 4 + 8;
		else if(m_ParamReal[26] != m_ParamReal[27])
			NCC = 2 + 4;
		else
			NCC = 2 + 8;
	
		int TimeFirstColonne	= (NCC == 2) ? m_ParamReal[4] : min(m_ParamReal[26], m_ParamReal[27]);
		int TimeSecondColonne	= (NCC == 2 + 4) ? (m_ParamReal[4] - TimeFirstColonne) 
									/*2 + 4 + 8*/: abs(m_ParamReal[26] - m_ParamReal[27]);
		int TimeThirdColonne	= m_ParamReal[4] - max(m_ParamReal[26], m_ParamReal[27]);
	
		int Value3d1 = m_pMainFrame->m_PortDialog.Current3dCoil_i(1, m_ParamReal[9], m_pMainFrame->m_PortDialog.m_3dDesEquilibr);
		int Value3d2 = m_pMainFrame->m_PortDialog.Current3dCoil_i(2, m_ParamReal[9], m_pMainFrame->m_PortDialog.m_3dDesEquilibr);
		double IntermediatePointDetuning		= IntermediateValue(m_ParamReal[1]								, m_ParamReal[5]	, m_ParamReal[26], TimeFirstColonne);
		double IntermediatePoint2dGradient		= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[6]	, m_ParamReal[8]	, m_ParamReal[26], TimeFirstColonne);
		double IntermediatePoint3dGradient1		= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[4]	, Value3d1		, m_ParamReal[26], TimeFirstColonne);
		double IntermediatePoint3dGradient2		= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[5]	, Value3d2		, m_ParamReal[26], TimeFirstColonne);
		double IntermediatePointMotIntensity	= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[1]	, m_ParamReal[6]	, m_ParamReal[27], TimeFirstColonne);
		double IntermediatePointRepIntensity	= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[2]	, m_ParamReal[7]	, m_ParamReal[27], TimeFirstColonne);
		double IntermediatePointMonsterLaser	= IntermediateValue(m_ParamReal[37]								, m_ParamReal[37], max(m_ParamReal[27], m_ParamReal[26]), TimeFirstColonne);
		double IntermediatePointMonsterVerti	= IntermediateValue(m_ParamReal[38]								, m_ParamReal[38], max(m_ParamReal[27], m_ParamReal[26]), TimeFirstColonne);
	
		double FinalPointDetuning		= IntermediateValue(m_ParamReal[1]								, m_ParamReal[5]	, m_ParamReal[26], TimeFirstColonne + TimeSecondColonne);
		double FinalPoint2dGradient		= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[6]	, m_ParamReal[8]	, m_ParamReal[26], TimeFirstColonne + TimeSecondColonne);
		double FinalPoint3dGradient1	= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[4]	, Value3d1		, m_ParamReal[26], TimeFirstColonne + TimeSecondColonne);
		double FinalPoint3dGradient2	= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[5]	, Value3d2		, m_ParamReal[26], TimeFirstColonne + TimeSecondColonne);
		double FinalPointMotIntensity	= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[1]	, m_ParamReal[6]	, m_ParamReal[27], TimeFirstColonne + TimeSecondColonne);
		double FinalPointRepIntensity	= IntermediateValue(m_pMainFrame->m_PortDialog.m_Ana_1_Out[2]	, m_ParamReal[7]	, m_ParamReal[27], TimeFirstColonne + TimeSecondColonne);
		double FinalPointMonsterLaser	= IntermediateValue(m_ParamReal[37]								, m_ParamReal[37], max(m_ParamReal[27], m_ParamReal[26]), TimeFirstColonne + TimeSecondColonne);
		double FinalPointMonsterVerti	= IntermediateValue(m_ParamReal[38]								, m_ParamReal[38], max(m_ParamReal[27], m_ParamReal[26]), TimeFirstColonne + TimeSecondColonne);
	
		// Compress
		if(NCC & 2 && TimeFirstColonne)
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Compress1"
										,	MILLI_SECONDE_INT(TimeFirstColonne) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	 	
				_ZEEMAN			,	0	,
				_MOT_AOM		,	1	,
				_MOT_CLIC		,	1	,
				_MAIN_IGBT		,	1	,
				_REPUMPS		,	0	,
				_IMAG_AOM		,	0	,
				_IMAG_CLIC		,	DEFAULT_VALUE	,
				_LATT_SHUT		,	DEFAULT_VALUE	,
				_LAT_BACK		,	DEFAULT_VALUE	,
				_NUSD_09		,	0	,
				_NUSD_10		,	0	,
				_2D_MOT			,	0	,//////////////////////
				_CAM_TRIGGER	,	0	,
				_NUSD_13		,	0	,
				_NUSD_14		,	0	,
				_NUSD_16		,	0	,
				_RBROAD			,	DEFAULT_VALUE	,
				_RSINGLE		,	DEFAULT_VALUE	,
				_NUSD_19		,	Monst_ON_Ph1_sortie_1	,
				_NUSD_20		,	Monst_ON_Ph1_sortie_2	,
				_NUSD_21		,	0	,
				_NUSD_22		,	1
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_INT			,	PRECEDENT_VALUE								,	IntermediatePointDetuning		,
				_ZSL_DET 			,	PRECEDENT_VALUE								,	IntermediatePointMotIntensity	,
				_REP_INT 			,	PRECEDENT_VALUE								,	IntermediatePointRepIntensity	
														);
		}
		if(NCC & 4 && TimeSecondColonne)
		{
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne)
										,	"Compress2"
										,	MILLI_SECONDE_INT(TimeSecondColonne) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	 	
					_ZEEMAN	,	0	
									);
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_INT			,	IntermediatePointDetuning		,	FinalPointDetuning,
				_ZSL_DET 			,	IntermediatePointMotIntensity	,	FinalPointMotIntensity	,
				_REP_INT 			,	IntermediatePointRepIntensity	,	FinalPointRepIntensity	
														);
			}
		if(NCC & 8 && TimeThirdColonne)
		{
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2)
										,	"Compress3"
										,	MILLI_SECONDE_INT(TimeThirdColonne) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	 	
					_ZEEMAN	,	0	
										);
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_INT			,	m_ParamReal[5]		,	m_ParamReal[5]	,
				_ZSL_DET 			,	m_ParamReal[6]		,	m_ParamReal[6]	,
				_REP_INT 			,	m_ParamReal[7]		,	m_ParamReal[7]	
// 			,
// 				_ANA_04				,	m_ParamReal[31]	,	m_ParamReal[37]	,
// 				_ANA_05				,	m_ParamReal[30]	,	m_ParamReal[38]	
														);
		}
	}
	// Melasse
	if(m_ParamBool[3])
	{
		// le plateau
		if(m_ParamReal[10])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Melasse1_µs"
										,	MICRO_SECONDE_INT(m_ParamReal[10]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	 	
				_MOT_AOM		,	1	,
				_MOT_CLIC		,	1	,
				_MAIN_IGBT		,	0	,
				_REPUMPS		,	0	
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_INT			,	m_ParamReal[11]	,	m_ParamReal[11]		,
				_ZSL_DET 			,	m_ParamReal[12]	,	m_ParamReal[12]	,
				_REP_INT 			,	m_ParamReal[13]	,	m_ParamReal[13]	
														);
		}
		// une deuxieme colonne pour la pente
		if(m_ParamReal[14])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Melasse2_µs"
										,	MICRO_SECONDE_INT(m_ParamReal[14]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	 	
				_MOT_AOM		,	1	,
				_MOT_CLIC		,	1	,
				_MAIN_IGBT		,	0	
													);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_INT			,	m_ParamReal[11]	,	m_ParamReal[15]	,
				_ZSL_DET 			,	m_ParamReal[12]	,	m_ParamReal[16]	,
				_REP_INT 			,	m_ParamReal[13]	,	m_ParamReal[17]	,
				_2D_C_MOT_CURENT	,	0	,	0	,
				_MOT_COILS		,	PRECEDENT_VALUE	,	m_ParamReal[35]	,
				_ANA_05	,	PRECEDENT_VALUE	,	m_ParamReal[36]	
													);
		}
	}
	// Polarisation	
	if(m_ParamBool[4])
	{
		// 2 colonnes dont voila la premiere
		if(m_ParamReal[18])
		{
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne)
										,	"Polar1_µs"
										,	MICRO_SECONDE_INT(m_ParamReal[18]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
				_ZEEMAN			,	0	,
				_MOT_AOM		,	m_ParamBool[19]	,
				_MOT_CLIC		,	1	,
				_MAIN_IGBT		,	0	,
				_REPUMPS		,	0	,
				_IMAG_AOM		,	0	,
				_IMAG_CLIC		,	DEFAULT_VALUE	,
				_LATT_SHUT		,	0	,
				_LAT_BACK		,	0	,
				_NUSD_09		,	0	,
				_NUSD_10		,	0	,
				_2D_MOT			,	0	,
				_CAM_TRIGGER	,	0	,
				_NUSD_13		,	0	,
				_NUSD_14		,	0	,
				_NUSD_16		,	m_ParamBool[13]	,
				_RBROAD			,	DEFAULT_VALUE	,
				_RSINGLE		,	DEFAULT_VALUE	,
				_NUSD_19		,	Monst_ON_Ph1_sortie_1	,
				_NUSD_20		,	Monst_ON_Ph1_sortie_2	,
				_NUSD_21		,	0	,
				_NUSD_22		,	1
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_AOM, DEFAULT_VALUE, DEFAULT_VALUE,
				_MOT_INT, m_ParamReal[50], m_ParamReal[50],
				_REP_INT, DEFAULT_VALUE, DEFAULT_VALUE	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
				);
		}
		// et la seconde
		if(m_ParamReal[19])
		{
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(0)
										,	"Polar2_µs"
										,	MICRO_SECONDE_INT(m_ParamReal[19]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
				_ZEEMAN			,	0	,
				_MOT_AOM		,	0	,
				_MOT_CLIC		,	1	,
				_MAIN_IGBT		,	0	,
				_REPUMPS		,	0	,
				_IMAG_AOM		,	0	,
				_IMAG_CLIC		,	DEFAULT_VALUE	,
				_LATT_SHUT		,	0	,
				_LAT_BACK		,	0	,
				_NUSD_09		,	0	,
				_NUSD_10		,	0	,
				_2D_MOT			,	0	,
				_CAM_TRIGGER	,	0	,
				_NUSD_13		,	0	,
				_NUSD_14		,	0	,
				_NUSD_16		,	m_ParamBool[13]	,
				_RBROAD			,	DEFAULT_VALUE	,
				_RSINGLE		,	DEFAULT_VALUE	,
				_NUSD_19		,	Monst_ON_Ph1_sortie_1	,
				_NUSD_20		,	Monst_ON_Ph1_sortie_2	,
				_NUSD_21		,	0	,
				_NUSD_22		,	1
														);	
			LaSeq.SetCol_Analogs_Continue( LastColonne); // continue toute les valeur de la colonne précédente
		}
	}
	// Monster Laser Only
	if(m_ParamBool[5])
	{
		// 8 colonnes
		int LastValueLaser = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_MOT_COILS).y;

		for(int iemeCol = 1; iemeCol <= 8; iemeCol++)
		{
			if(m_ParamReal[40 + iemeCol])
			{
				// pour la 1ere colonne, on prend la valeur de la colonne précédente, sinon
				double StartValue = (iemeCol == 1) ? LastValueLaser : m_NiEdit[50 + iemeCol - 1].GetValue();//m_ParamReal[50 + iemeCol - 1];
				double ToValue = m_NiEdit[50 + iemeCol].GetValue();//m_ParamReal[50 + iemeCol];
				int Duree_Col1_Int = int(MILLI_SECONDE_INT(m_ParamReal[40 + iemeCol]));

				CString strName; 
				strName.Format("Monster%d", iemeCol);
				if(Duree_Col1_Int)
				{
					LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(LastColonne)
												,	strName
												,	Duree_Col1_Int );
					LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
						_REPUMPS			,	0	,
						_MOT_AOM			,	0	,
						_MOT_CLIC			,	0	,
						_MAIN_IGBT			,	iemeCol == 1 || iemeCol > 6 ? 0 : 1	,
						_RBROAD				,	iemeCol <= 4	,
						_RSINGLE			,	iemeCol > 4	&& iemeCol <= 6
						);	
					if(iemeCol == 8)
						LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
						_MOT_AOM			,	0	,
						_MOT_CLIC			,	0	,
						_MAIN_IGBT			,	0	,
						_RBROAD				,	0	,
						_RSINGLE			,	0	, 
						_LAT_BACK			,	1 
						);
					LaSeq.SetCol_1Digital(-1,   23 + iemeCol - 1, false);
					LaSeq.SetCol_1Digital(-1,   23 + iemeCol, true);
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
						_MOT_COILS,	StartValue,	ToValue 
						);
				}
			}
		}
	}

	// TOF
	if(m_ParamBool[6])
	{
		if(m_ParamReal[20])
		{
			int DureeTOF = MICRO_SECONDE_INT(m_ParamReal[20]);
			int DureeStern = MILLI_SECONDE_INT(2);
			// si Stern et Gerlach
			if(m_ParamBool[10] && DureeTOF > DureeStern)
				DureeTOF -= DureeStern;

			LaSeq.InsertNewColonne( AfterLastColonne	,	"TOF_µs"
										,	DureeTOF );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
				_MOT_AOM		,	0	,
				_MOT_CLIC		,	0	,
				_RBROAD			,	0	,
				_RSINGLE		,	0	,
				_MAIN_IGBT		,	0	,
				_LATT_SHUT		,	0	,
				_LAT_BACK		,	0	
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_COILS,	0,	0 ,
				_ANA_05,	0,	0 ,
				_GUIDE_CURRENT,	m_ParamReal[64],	m_ParamReal[64]
				);
			// si Stern et Gerlach
			if(m_ParamBool[10] && DureeTOF > DureeStern)
			{
				// on recopie la colonne pour y changer Stern off
				LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(LastColonne)
											,	"SternOff"
											,	DureeStern	);
				LaSeq.SetCol_1Digital(-1, _NUSD_13, 0);
			}
		}
	}
	// Fluo
	if(m_ParamBool[7] && !m_ParamBool[8] && LaSeq.GetColonne( LastColonne)) {
		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[49]);
		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance, _IMAG_CLIC, m_ParamBool[31]);

		if(m_ParamReal[59]) {
			int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);
			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals(LastColonne, dureeAdvanceTrig, _CAM_TRIGGER, 1);
		}
		if(m_ParamReal[23]) {
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Fluo1"	,	MICRO_SECONDE_INT(m_ParamReal[23]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
				_MOT_AOM		,	0	,
				_MOT_CLIC		,	0	,
				_ZEEMAN			,	0	,
				_IMAG_AOM		,	1	,
				_IMAG_CLIC		,	1	,
				_CAM_TRIGGER	,	1	);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_INT			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_ZSL_DET 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_REP_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_MOT_COILS				,	0				,	0				,
				_ANA_05				,	0				,	0				);
		}
	}	
	// Absorption
	if(m_ParamBool[7] && m_ParamBool[8] && LaSeq.GetColonne( LastColonne)) {
		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[49]);
		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance, _IMAG_CLIC, m_ParamBool[31]);

		if(m_ParamReal[59]) {
			int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);
			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceTrig, _CAM_TRIGGER, 1);
		}
		LaSeq.InsertNewColonne( AfterLastColonne, "Abs1" , MICRO_SECONDE_INT(m_ParamReal[23]));
		LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
				_MOT_AOM		,	0	,
				_MOT_CLIC		,	0	,
				_ZEEMAN			,	0	,
				_IMAG_AOM		,	1	,
				_IMAG_CLIC		,	1	,
				_CAM_TRIGGER	,	1	);	

		LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_INT			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_ZSL_DET 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_REP_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_MOT_COILS				,	0				,	0				,
				_ANA_05				,	0				,	0				);
	
		// on recopie la colonne pour y changer 2 sorties analogiques
		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne),	"Abs2",	MILLI_SECONDE_INT(m_ParamReal[24]));
		LaSeq.SetCol_1Digital(-1,   	_IMAG_AOM	, 0);
		LaSeq.SetCol_1Digital(-1,   	_CAM_TRIGGER, 0);
		LaSeq.SetCol_1Digital(-1,   	_LATT_SHUT	, 0);

		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2), "Abs3"	);
		LaSeq.SetCol_1Digital(-1,   	_LATT_SHUT	, 0);
		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2), "Abs4"	);
		LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2), "Abs5"	);
		LaSeq.SetCol_1Digital(-1,   	_IMAG_AOM	, 0);
		if(m_ParamReal[59]) {
			int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);
			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne - 3, dureeAdvanceTrig, _CAM_TRIGGER, 1);
			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne - 1, dureeAdvanceTrig, _CAM_TRIGGER, 1);
		}
	}
	// Dead Time
	if(m_ParamBool[9])
	{
		if(m_ParamReal[25])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Dead1"
										,	MILLI_SECONDE_INT(m_ParamReal[25]) );
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne, 	  
				_ZEEMAN			,	0	,
				_MOT_AOM		,	0	,
				_MOT_CLIC		,	0	,
				_MAIN_IGBT		,	0	,
				_REPUMPS		,	0	,
				_IMAG_AOM		,	0	,
				_IMAG_CLIC		,	0	,
				_LAT_BACK		,	0	,
				_NUSD_09		,	0	,
				_NUSD_10		,	0	,
				_2D_MOT			,	0	,
				_CAM_TRIGGER	,	0	,
				_NUSD_13		,	0	,
				_NUSD_14		,	0	,
				_NUSD_16		,	0	,
				_RBROAD			,	0	,
				_RSINGLE		,	0	,
				_NUSD_19		,	0	,
				_NUSD_20		,	0	,
				_NUSD_21		,	0	,
				_NUSD_22		,	0
														);	
			LaSeq.SetCol_Analogs_Continue( LastColonne);
		}
		if(m_ParamReal[22])
		{
			LaSeq.InsertNewColonne_CopiedFromCol(0	,	LaSeq.GetColonne(1)
										,	"Load00"
										,	MILLI_SECONDE_INT(m_ParamReal[22]));
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne( 1 , 	  
				_ZEEMAN			,	0	,
				_MOT_AOM		,	1	,
				_MOT_CLIC		,	1	,
				_MAIN_IGBT		,	0	,
				_REPUMPS		,	0	,
				_IMAG_AOM		,	0	,
				_IMAG_CLIC		,	0	,
				_LAT_BACK		,	0	,
				_NUSD_09		,	0	,
				_NUSD_10		,	0	,
				_2D_MOT			,	0	,
				_CAM_TRIGGER	,	0	,
				_NUSD_13		,	0	,
				_NUSD_14		,	0	,
				_NUSD_16		,	0	,
				_RBROAD			,	0	,
				_RSINGLE		,	0	,
				_NUSD_19		,	0	,
				_NUSD_20		,	0	,
				_NUSD_21		,	0	,
				_NUSD_22		,	0
														);	
		}
	}
	LaSeq.SetCol_1Analog( LastColonne, 
		_POS_XPS_2		,	((m_NiCheck[35].Value) ? PRECEDENT_VALUE : DEFAULT_VALUE) ,	((m_NiCheck[35].Value) ? m_NiEdit[71].GetValue() : DEFAULT_VALUE)
		);
//	// set une relation entre 2 analog durant toute la sequence
//	if(m_NiCheck[36].Value)
//		for(int i = 1; i <= LaSeq.GetnColonnes(); i++)
//			LaSeq.SetCol_1Analog_FromFunction_ofOtherAnalog(i, 14, 15, "c:\\Formula_For_Rb2Manip.txt", 100);
return LaSeq.CalculateNbrPeriodes();
}	