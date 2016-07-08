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
				/*	_ZEEMAN				 0 */	1	,
				/*	_MOT_AOM			 1 */	1	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	1	,
				/*	_REP_CLIC			 4 */	1*m_pMainFrame->m_PortDialog.m_bDigOut[25]	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	1	,
				/*	_DEPUMP_AOM			 7 */	m_pMainFrame->m_PortDialog.m_bDigOut[23]	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	, //m_pMainFrame->m_PortDialog.m_3dMeanCurrent	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	m_pMainFrame->m_PortDialog.m_Ana_1_Out[6] > 0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_ON_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_ON_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	0	,
				/*	_MONST_VERT_PD2_OFF	22 */	1
													);	
			// on assigne des sorties analogiques à la dernière colonne en donnant "Quelle_Analogique", "Debut" et "Fin"
			// ou bien DEFAULT_VALUE pour prendre les valeurs du port
			// les voie non-spécifiées continue la valeur de la colonne précédente
			// pour la première colonne, elles prennent DEFAULT_VALUE !!!
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 	
					_3D_MOT_1_CURENT,	m_pMainFrame->m_PortDialog.m_Ana_1_Out[4]	,	m_pMainFrame->m_PortDialog.m_Ana_1_Out[4]	,
					_3D_MOT_2_CURENT,	m_pMainFrame->m_PortDialog.m_Ana_1_Out[5]	,	m_pMainFrame->m_PortDialog.m_Ana_1_Out[5]	,
					_MONSTER_LOCK_1	,	m_ParamReal[29]	,	m_ParamReal[29] ,
					//_MONSTER_LOCK_2	,	m_ParamReal[29] * double(m_ParamReal[61]) / 100.0	,	m_ParamReal[29] * double(m_ParamReal[61]) / 100.0	,
					_MONST_VERT_LOCK_1,	m_ParamReal[30]	,	m_ParamReal[30] ,
					//_MONST_VERT_LOCK_2,	m_ParamReal[30] * double(m_ParamReal[63]) / 100.0	,	m_ParamReal[30] * double(m_ParamReal[63]) / 100.0	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
														);
		}
		if(m_ParamReal[3])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Load Zeeman Off"
										,	MILLI_SECONDE_INT(m_ParamReal[3]) );
			// on assigne des sorties à la dernière colonne
			LaSeq.SetCol_Digitals_DefaultPrecedentColonne(	LastColonne
														,   _ZEEMAN		,	0
														,   _REP_CLIC	,	1
														,   _DEPUMP_AOM	,	0
														, 	_IMAG_CLIC	,	DEFAULT_VALUE);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MONSTER_LOCK_1		,	PRECEDENT_VALUE	,	m_ParamReal[31] ,
				//_MONSTER_LOCK_2		,	PRECEDENT_VALUE	,	m_ParamReal[31] * double(m_ParamReal[61]) / 100.0	,
				_MONST_VERT_LOCK_1	,	PRECEDENT_VALUE	,	m_ParamReal[32] ,
				//_MONST_VERT_LOCK_2	,	PRECEDENT_VALUE	,	m_ParamReal[32] * double(m_ParamReal[63]) / 100.0	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
														);
		}
	}
	// Compression
	if(m_ParamBool[2])
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
			LaSeq.SetCol_32Digitals( LastColonne, 	 	
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	1	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	1	,
				/*	_REP_CLIC			 4 */	1	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	, //m_pMainFrame->m_PortDialog.m_3dMeanCurrent	,
				/*	_2D_MOT				11 */	0	,//////////////////////
				/*	_2D_COMPRESS		12 */	m_pMainFrame->m_PortDialog.m_Ana_1_Out[6]	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	m_ParamBool[11]	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_ON_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_ON_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	0	,
				/*	_MONST_VERT_PD2_OFF	22 */	1
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET			,	PRECEDENT_VALUE						,	IntermediatePointDetuning		,
				_MOT_INT 			,	PRECEDENT_VALUE						,	IntermediatePointMotIntensity	,
				_REP_INT 			,	PRECEDENT_VALUE						,	IntermediatePointRepIntensity	,
//				_3D_MOT_1_CURENT	,	m_pMainFrame->m_PortDialog.m_Ana_1_Out[4]	,	IntermediatePoint3dGradient1	,
//				_3D_MOT_2_CURENT	,	m_pMainFrame->m_PortDialog.m_Ana_1_Out[5]	,	IntermediatePoint3dGradient2	,
				_2D_C_MOT_CURENT	,	PRECEDENT_VALUE						,	IntermediatePoint2dGradient		,
				_MONSTER_LOCK_1		,	m_ParamReal[37]	,	IntermediatePointMonsterLaser ,
				//_MONSTER_LOCK_2		,	m_ParamReal[37] * double(m_ParamReal[61]) / 100.0	,	IntermediatePointMonsterLaser * double(m_ParamReal[61]) / 100.0	,
				_MONST_VERT_LOCK_1	,	m_ParamReal[38]	,	IntermediatePointMonsterVerti ,
				//_MONST_VERT_LOCK_2	,	m_ParamReal[38] * double(m_ParamReal[63]) / 100.0	,	IntermediatePointMonsterVerti * double(m_ParamReal[63]) / 100.0	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
														);
		}
		if(NCC & 4 && TimeSecondColonne)
		{
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne)
										,	"Compress2"
										,	MILLI_SECONDE_INT(TimeSecondColonne) );
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET			,	IntermediatePointDetuning		,	FinalPointDetuning,
				_MOT_INT 			,	IntermediatePointMotIntensity	,	FinalPointMotIntensity	,
				_REP_INT 			,	IntermediatePointRepIntensity	,	FinalPointRepIntensity	,
//				_3D_MOT_1_CURENT	,	IntermediatePoint3dGradient1	,	FinalPoint3dGradient1	,
//				_3D_MOT_2_CURENT	,	IntermediatePoint3dGradient2	,	FinalPoint3dGradient2	,
				_2D_C_MOT_CURENT	,	IntermediatePoint2dGradient		,	FinalPoint2dGradient,
				_MONSTER_LOCK_1		,	IntermediatePointMonsterLaser	,	FinalPointMonsterLaser	,
				//_MONSTER_LOCK_2		,	IntermediatePointMonsterLaser * double(m_ParamReal[61]) / 100.0	,	FinalPointMonsterLaser * double(m_ParamReal[61]) / 100.0	,
				_MONST_VERT_LOCK_1	,	IntermediatePointMonsterVerti	,	FinalPointMonsterVerti	,
				//_MONST_VERT_LOCK_2	,	IntermediatePointMonsterVerti * double(m_ParamReal[63]) / 100.0	,	FinalPointMonsterVerti * double(m_ParamReal[63]) / 100.0	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
														);
			}
		if(NCC & 8 && TimeThirdColonne)
		{
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2)
										,	"Compress3"
										,	MILLI_SECONDE_INT(TimeThirdColonne) );
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET			,	m_ParamReal[5]		,	m_ParamReal[5]	,
				_MOT_INT 			,	m_ParamReal[6]		,	m_ParamReal[6]	,
				_REP_INT 			,	m_ParamReal[7]		,	m_ParamReal[7]	,
//				_3D_MOT_1_CURENT	,	Value3d1			,	Value3d1		,
//				_3D_MOT_2_CURENT	,	Value3d2			,	Value3d2		,
				_2D_C_MOT_CURENT	,	m_ParamReal[8]		,	m_ParamReal[8]	,
				_MONSTER_LOCK_1		,	FinalPointMonsterLaser		,	m_ParamReal[39]	,
				//_MONSTER_LOCK_2		,	FinalPointMonsterLaser * double(m_ParamReal[61]) / 100.0		,	m_ParamReal[39] * double(m_ParamReal[61]) / 100.0	,
				_MONST_VERT_LOCK_1	,	PRECEDENT_VALUE	,	m_ParamReal[40]	,
				//_MONST_VERT_LOCK_2	,	PRECEDENT_VALUE	,	m_ParamReal[40] * double(m_ParamReal[63]) / 100.0	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
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
			LaSeq.SetCol_32Digitals( LastColonne, 	 	
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	1	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	1	,
				/*	_REP_CLIC			 4 */	1	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	m_ParamBool[12]	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_ON_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_ON_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	0	,
				/*	_MONST_VERT_PD2_OFF	22 */	1
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET			,	m_ParamReal[11]	,	m_ParamReal[11]		,
				_MOT_INT 			,	m_ParamReal[12]	,	m_ParamReal[12]	,
				_REP_INT 			,	m_ParamReal[13]	,	m_ParamReal[13]	,
//				_3D_MOT_1_CURENT	,	0	,	0	,
//				_3D_MOT_2_CURENT	,	0	,	0	,
				_2D_C_MOT_CURENT	,	0	,	0	,
				_MONSTER_LOCK_1		,	PRECEDENT_VALUE	,	m_ParamReal[33]	,
				//_MONSTER_LOCK_2		,	PRECEDENT_VALUE	,	m_ParamReal[33] * double(m_ParamReal[61]) / 100.0	,
				_MONST_VERT_LOCK_1	,	PRECEDENT_VALUE	,	m_ParamReal[34]	,
				//_MONST_VERT_LOCK_2	,	PRECEDENT_VALUE	,	m_ParamReal[34] * double(m_ParamReal[63]) / 100.0	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
														);
		}
		// une deuxieme colonne pour la pente
		if(m_ParamReal[14])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Melasse2_µs"
										,	MICRO_SECONDE_INT(m_ParamReal[14]) );
			LaSeq.SetCol_32Digitals( LastColonne, 	 	
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	1	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	1	,
				/*	_REP_CLIC			 4 */	1	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	m_ParamBool[12]	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_ON_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_ON_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	0	,
				/*	_MONST_VERT_PD2_OFF	22 */	1
													);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET			,	m_ParamReal[11]	,	m_ParamReal[15]	,
				_MOT_INT 			,	m_ParamReal[12]	,	m_ParamReal[16]	,
				_REP_INT 			,	m_ParamReal[13]	,	m_ParamReal[17]	,
//				_3D_MOT_1_CURENT	,	0	,	0	,
//				_3D_MOT_2_CURENT	,	0	,	0	,
				_2D_C_MOT_CURENT	,	0	,	0	,
				_MONSTER_LOCK_1		,	PRECEDENT_VALUE	,	m_ParamReal[35]	,
				//_MONSTER_LOCK_2		,	PRECEDENT_VALUE	,	m_ParamReal[35] * double(m_ParamReal[61]) / 100.0	,
				_MONST_VERT_LOCK_1	,	PRECEDENT_VALUE	,	m_ParamReal[36]	,
				//_MONST_VERT_LOCK_2	,	PRECEDENT_VALUE	,	m_ParamReal[36] * double(m_ParamReal[63]) / 100.0	,
					_POS_XPS_2			, XPS_TRACKING_OR_NOT
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
			LaSeq.SetCol_32Digitals( LastColonne, 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	m_ParamBool[19]	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	0,//m_ParamBool[23]	,
				/*	_REP_CLIC			 4 */	0	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	m_ParamBool[13]	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_ON_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_ON_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	0	,
				/*	_MONST_VERT_PD2_OFF	22 */	1
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_AOM, DEFAULT_VALUE, DEFAULT_VALUE,
				_MOT_DET, m_ParamReal[50], m_ParamReal[50],
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
			LaSeq.SetCol_32Digitals( LastColonne, 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	0	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	m_ParamBool[25]	,
				/*	_REP_CLIC			 4 */	0,//m_ParamBool[24]	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	m_ParamBool[13]	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_ON_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_ON_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	0	,
				/*	_MONST_VERT_PD2_OFF	22 */	1
														);	
			LaSeq.SetCol_Analogs_Continue( LastColonne); // continue toute les valeur de la colonne précédente
		}
	}
	// Monster Laser Only
	if(m_ParamBool[5])
	{
		// 8 colonnes
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// en fait, On definit un seuil de consigne m_ParamReal[60], en dessous duquel, 
		// on décide de passer à une photodiode plus précise (m_ParamReal[61] % plus précise)
		// Donc on peut potentiellement couper chacune des 8 colonnes en deux : PhDiode 1, et PhDiode 2
		int MonsterSeuil_Ph1_Ph2 = m_ParamReal[60];
		double MonsterMultipl_Ph2 = double(m_ParamReal[61]) / 100.0;
		// Valeurs à utiliser en pratique (qui seront calculée par la fonction SpecialMonster_With_2_PhotoDiode(...)
		bool MonsterFaut2Colonnes_Ph1_Ph2, MonsterPremiereColonneEstEnPh2;
		double duree_Col1, duree_Col2, Col1_pHd1_From, Col1_pHd1_To, Col2_pHd1_From, Col2_pHd1_To, Col1_pHd2_From, Col1_pHd2_To, Col2_pHd2_From, Col2_pHd2_To;
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		int LastValueLaser = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_MONSTER_LOCK_1).y;
		int LastValueMonsterVerti = LaSeq.GetColonne( LastColonne)->GetLastPointWf(_MONST_VERT_LOCK_1).y;

		for(int iemeCol = 1; iemeCol <= 8; iemeCol++)
		{
			if(m_ParamReal[40 + iemeCol])
			{
				// pour la 1ere colonne, on prend la valeur de la colonne précédente, sinon
				double StartValue = (iemeCol == 1) ? LastValueLaser : m_NiEdit[50 + iemeCol - 1].GetValue();//m_ParamReal[50 + iemeCol - 1];
				double ToValue = m_NiEdit[50 + iemeCol].GetValue();//m_ParamReal[50 + iemeCol];
				SpecialMonster_With_2_PhotoDiode( StartValue, ToValue, MILLI_SECONDE_INT(m_ParamReal[40 + iemeCol]), MonsterSeuil_Ph1_Ph2, MonsterMultipl_Ph2
												/*bool*/	, MonsterFaut2Colonnes_Ph1_Ph2, MonsterPremiereColonneEstEnPh2
												/*double*/	, duree_Col1, duree_Col2, Col1_pHd1_From, Col1_pHd1_To, Col2_pHd1_From, Col2_pHd1_To, Col1_pHd2_From, Col1_pHd2_To, Col2_pHd2_From, Col2_pHd2_To);
				// PS, il faut juste faire gaffe au fait que les duree_Col1/2 sont des double, et on peut perdre un unité en faisant l'arodi de duree_Col1, et duree_Col2 = Total-duree_Col1!!!!!!
				// donc on recalcul duree_Col2
				int Duree_Col1_Int = int(duree_Col1);
				int Duree_Col2_Int = int(duree_Col2);
				if(Duree_Col1_Int + Duree_Col2_Int < int(duree_Col1 + duree_Col2))
					Duree_Col1_Int++;

				// pour la 1ere colonne, on prend la valeur de la colonne précédente, sinon
				double StartValueVerti = (iemeCol == 1) ? LastValueMonsterVerti : m_NiEdit[70 + iemeCol - 1].GetValue();
				double ToValueVerti = m_NiEdit[70 + iemeCol].GetValue();
				double ValIntMonsterVerti_1 = IntermediateValue(StartValueVerti, ToValueVerti, Duree_Col1_Int + Duree_Col2_Int, Duree_Col1_Int);
				double ValIntMonsterVerti_2 = IntermediateValue(StartValueVerti * double(m_ParamReal[63]) / 100.0, ToValueVerti * double(m_ParamReal[63]) / 100.0, Duree_Col1_Int + Duree_Col2_Int, Duree_Col1_Int);

				CString strName; 
				strName.Format("Monster%d", iemeCol);
				if(Duree_Col1_Int)
				{
					LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(LastColonne)
												,	strName
												,	Duree_Col1_Int );
					LaSeq.SetCol_Digitals_DefaultPrecedentColonne( LastColonne,   
						_ZEEMAN				,	0	,
						_MOT_AOM			,	0	,
						_MOT_CLIC			,	0	,
						_REP_AOM			,	0	,
						_REP_CLIC			,	DEFAULT_VALUE	,
						_IMAG_AOM			,	0	,
						_IMAG_CLIC			,	DEFAULT_VALUE	,
						_DEPUMP_AOM			,	0	,
						_H_LAUNCH			,	0	,
						_V_LAUNCH			,	0	,
						_3D_MOT				,	m_ParamBool[38]	,
						_2D_MOT				,	0	,
						_2D_COMPRESS		,	0	,
						_2D_PREGUIDE		,	m_ParamBool[20]	,
						_BIG_BIAS_PREGUIDE	,	0	,
						_PINCH_COIL			,	0	,
						_B_POLAR			,	0	,
						_TRIGGER			,	0	,
						_CAM_TRIGGER		,	0	,
						_MONST_PD1_OFF		,	MonsterPremiereColonneEstEnPh2 ? Monst_ON_Ph2_sortie_1 : Monst_ON_Ph1_sortie_1	,
						_MONST_PD2_OFF		,	MonsterPremiereColonneEstEnPh2	? Monst_ON_Ph2_sortie_2 : Monst_ON_Ph1_sortie_2	,
						_MONST_VERT_PD1_OFF	,	0	,
						_MONST_VERT_PD2_OFF	,	1	
						);	
					LaSeq.SetCol_1Digital(-1,   23 + iemeCol - 1, false);
					LaSeq.SetCol_1Digital(-1,   23 + iemeCol, true);
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
						_MONSTER_LOCK_1,	Col1_pHd1_From,	Col1_pHd1_To ,
						//_MONSTER_LOCK_2,	Col1_pHd2_From,	Col1_pHd2_To ,
						_MONST_VERT_LOCK_1,	StartValueVerti ,	ValIntMonsterVerti_1,
						//_MONST_VERT_LOCK_2,	StartValueVerti * double(m_ParamReal[63]) / 100.0,	ValIntMonsterVerti_2, 
						_POS_XPS_2		  ,	(m_NiCheck[35].Value) ? m_NiEdit[max(71, 70 + iemeCol - 1)].GetValue() : DEFAULT_VALUE ,	(m_NiCheck[35].Value) ? m_NiEdit[70 + iemeCol].GetValue() : DEFAULT_VALUE
						);
				}
				if(Duree_Col2_Int)
				{
					LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne)
												,	strName
												,	Duree_Col2_Int );
					LaSeq.SetCol_1Digital(-1,   _MONST_PD1_OFF, MonsterPremiereColonneEstEnPh2	? Monst_ON_Ph1_sortie_1 : Monst_ON_Ph2_sortie_1);
					LaSeq.SetCol_1Digital(-1,   _MONST_PD2_OFF, MonsterPremiereColonneEstEnPh2 ? Monst_ON_Ph1_sortie_2 : Monst_ON_Ph2_sortie_2);
					LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
						_MONSTER_LOCK_1,	Col2_pHd1_From,	Col2_pHd1_To ,
						//_MONSTER_LOCK_2,	Col2_pHd2_From,	Col2_pHd2_To ,
						_MONST_VERT_LOCK_1,	ValIntMonsterVerti_1,	ToValueVerti,
						//_MONST_VERT_LOCK_2,	ValIntMonsterVerti_2,	ToValueVerti * double(m_ParamReal[63]) / 100.0, 
						_POS_XPS_2		  ,	m_NiCheck[35].Value ? m_NiEdit[max(71, 70 + iemeCol - 1)].GetValue() : DEFAULT_VALUE ,	m_NiCheck[35].Value ? m_NiEdit[70 + iemeCol].GetValue() : DEFAULT_VALUE
						);
				}
				// on assigne la colonne 5 une formule d'un .txt avec comme point initiale le point de la col4 et comme
				if(m_ParamBool[37] && iemeCol == 5)
				{	//																fichier				  Tini		Tfin(sec)					npoints  valeur de départ
					LaSeq.SetCol_1Analog_FromFunction_ofX(-1, _MONSTER_LOCK_1, "c:\\LaserCurveHoriz.txt", 0.0, m_ParamReal[40 + iemeCol]*0.001, 1000, StartValue);
					ActualiseNiEdit(50 + iemeCol, LaSeq.GetColonne( LastColonne)->GetLastPointWf(_MONSTER_LOCK_1).y, false);
					LaSeq.SetCol_1Analog_FromFunction_ofX(-1, _MONST_VERT_LOCK_1, "c:\\LaserCurveVert.txt", 0.0, m_ParamReal[40 + iemeCol]*0.001, 1000, StartValue);
					ActualiseNiEdit(70 + iemeCol, LaSeq.GetColonne( LastColonne)->GetLastPointWf(_MONST_VERT_LOCK_1).y, false);
				}
			}
		}
		if(m_ParamBool[16])
		{
		//	CWaveForme* pwfmainCopy = new CWaveForme(LaWfMainDialog);
		//	LaSeq.GetColonne( LastColonne)->AssignWaveFormeToAnalog(3, pwfmainCopy);
		}
	}

// tests:
if(m_ParamBool[16])
LaSeq.DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToBeginOfCol(
	m_FilmFromColName
	, MILLI_SECONDE_INT(m_ParamReal[80])
	, "TOF");

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
			LaSeq.SetCol_32Digitals( LastColonne, 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	0	,
				/*	_MOT_CLIC			 2 */	0	,
				/*	_REP_AOM			 3 */	0	,
				/*	_REP_CLIC			 4 */	DEFAULT_VALUE	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	m_ParamBool[10]	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_OFF_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_OFF_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	1	,
				/*	_MONST_VERT_PD2_OFF	22 */	0
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MONSTER_LOCK_1,	0,	0 ,
				//_MONSTER_LOCK_2,	0,	0 ,
				_MONST_VERT_LOCK_1,	0,	0 ,
				//_MONST_VERT_LOCK_2,	0,	0 ,
				_GUIDE_CURRENT,	m_ParamReal[64],	m_ParamReal[64]
				);
			// si Stern et Gerlach
			if(m_ParamBool[10] && DureeTOF > DureeStern)
			{
				// on recopie la colonne pour y changer Stern off
				LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(LastColonne)
											,	"SternOff"
											,	DureeStern	);
				LaSeq.SetCol_1Digital(-1, _2D_PREGUIDE, 0);
			}
		}
	}
	// Fluo
	if(m_ParamBool[7])
	{
		if(m_ParamReal[47])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Fluo00"
										,	MILLI_SECONDE_INT(m_ParamReal[47]) );
			LaSeq.SetCol_32Digitals( LastColonne, 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	0	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	0	,
				/*	_REP_CLIC			 4 */	1	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_OFF_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_OFF_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	1	,
				/*	_MONST_VERT_PD2_OFF	22 */	0
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET, DEFAULT_VALUE	,	DEFAULT_VALUE ,
				_MOT_INT, DEFAULT_VALUE	,	DEFAULT_VALUE ,
				_REP_INT, DEFAULT_VALUE	,	DEFAULT_VALUE
				);
		}		
		if(m_ParamReal[23])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Fluo1"
										,	MILLI_SECONDE_INT(m_ParamReal[23]) );
			LaSeq.SetCol_32Digitals( LastColonne, 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	1	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	1	,
				/*	_REP_CLIC			 4 */	1	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	DEFAULT_VALUE	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	1	,
				/*	_MONST_PD1_OFF		19 */	Monst_OFF_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_OFF_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	1	,
				/*	_MONST_VERT_PD2_OFF	22 */	0
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_MOT_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_REP_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_MONSTER_LOCK_1		,	0	,	0	,
				_3D_MOT_1_CURENT	,	0	,	0	,
				_3D_MOT_2_CURENT	,	0	,	0	,
				_2D_C_MOT_CURENT	,	0	,	0	,
				//_MONSTER_LOCK_2		,	0	,	0	,
				_MONST_VERT_LOCK_1	,	0	,	0	
				//_MONST_VERT_LOCK_2	,	0	,	0
														);
		}
	}	
	// Absorption
	if(m_ParamBool[8] && LaSeq.GetColonne( LastColonne))
	{
		// test 
//		LaSeq.DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToEndOfCol("Monst", MILLI_SECONDE_INT(m_ParamReal[4]));

		// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
		int dureeAdvance = MILLI_SECONDE_INT(m_ParamReal[49]);
		LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvance,
				_IMAG_CLIC	,	m_ParamBool[31] ,
				_REP_CLIC	,	m_ParamBool[32]*m_pMainFrame->m_PortDialog.m_bDigOut[26] ,
				_DEPUMP_AOM	,	m_ParamBool[32]*m_pMainFrame->m_PortDialog.m_bDigOut[26] 
				);

		if(m_ParamReal[59])
		{
			int dureeAdvanceTrig = MICRO_SECONDE_INT(m_ParamReal[59]);
			LaSeq.SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals( LastColonne, dureeAdvanceTrig,
				_CAM_TRIGGER	,	1	);
		}
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Abs1_µs"
										,	MICRO_SECONDE_INT(m_ParamReal[24]) );
			LaSeq.SetCol_32Digitals( LastColonne, 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	0	,
				/*	_MOT_CLIC			 2 */	0	,
				/*	_REP_AOM			 3 */	m_ParamBool[32]	,
				/*	_REP_CLIC			 4 */	m_ParamBool[32]*m_pMainFrame->m_PortDialog.m_bDigOut[26]	,
				/*	_IMAG_AOM			 5 */	m_ParamBool[31]	,
				/*	_IMAG_CLIC			 6 */	m_ParamBool[31]	,
				/*	_DEPUMP_AOM			 7 */	m_ParamBool[32]*m_pMainFrame->m_PortDialog.m_bDigOut[26]	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	1	,
				/*	_MONST_PD1_OFF		19 */	Monst_OFF_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_OFF_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	1	,
				/*	_MONST_VERT_PD2_OFF	22 */	0
														);	
			LaSeq.SetCol_Analogs_Which_Debut_Fin( LastColonne, 
				_MOT_DET			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_MOT_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_REP_INT 			,	DEFAULT_VALUE	,	DEFAULT_VALUE	,
				_3D_MOT_1_CURENT	,	0	,	0	,
				_3D_MOT_2_CURENT	,	0	,	0	,
				_2D_C_MOT_CURENT	,	0	,	0	,
				_MONSTER_LOCK_1		,	0	,	0	,
				//_MONSTER_LOCK_2		,	0	,	0	,
				_MONST_VERT_LOCK_1	,	0	,	0	
				//_MONST_VERT_LOCK_2	,	0	,	0
														);
	
			// on recopie la colonne pour y changer 2 sorties analogiques
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne( LastColonne)
										,	"Abs2"
										,	MILLI_SECONDE_INT(m_ParamReal[23])	);
			LaSeq.SetCol_1Digital(-1,   	_IMAG_AOM	, 0);
			LaSeq.SetCol_1Digital(-1,   	_CAM_TRIGGER, 0);
		
	
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2)
										,	"Abs3_µs"		);
	
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2)
										,	"Abs4"		);
	
			LaSeq.InsertNewColonne_CopiedFromCol( AfterLastColonne	,	LaSeq.GetColonne(-2)
										,	"Abs5_µs"		);
			LaSeq.SetCol_1Digital(-1,   	_IMAG_AOM	, 0);
	}
	// Dead Time
	if(m_ParamBool[9])
	{
		if(m_ParamReal[25])
		{
			LaSeq.InsertNewColonne( AfterLastColonne	,	"Dead1"
										,	MILLI_SECONDE_INT(m_ParamReal[25]) );
			LaSeq.SetCol_32Digitals( LastColonne, 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	0	,
				/*	_MOT_CLIC			 2 */	0	,
				/*	_REP_AOM			 3 */	0	,
				/*	_REP_CLIC			 4 */	0	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	0	,
				/*	_DEPUMP_AOM			 7 */	0	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_OFF_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_OFF_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	1	,
				/*	_MONST_VERT_PD2_OFF	22 */	0
														);	
			LaSeq.SetCol_Analogs_Continue( LastColonne);
		}
		if(m_ParamReal[22])
		{
			LaSeq.InsertNewColonne_CopiedFromCol(0	,	LaSeq.GetColonne(1)
										,	"Load00"
										,	MILLI_SECONDE_INT(m_ParamReal[22]));
			LaSeq.SetCol_32Digitals( 1 , 	  
				/*	_ZEEMAN				 0 */	0	,
				/*	_MOT_AOM			 1 */	1	,
				/*	_MOT_CLIC			 2 */	1	,
				/*	_REP_AOM			 3 */	0	,
				/*	_REP_CLIC			 4 */	1*m_pMainFrame->m_PortDialog.m_bDigOut[25]	,
				/*	_IMAG_AOM			 5 */	0	,
				/*	_IMAG_CLIC			 6 */	1	,
				/*	_DEPUMP_AOM			 7 */	m_pMainFrame->m_PortDialog.m_bDigOut[23]	,
				/*	_H_LAUNCH			 8 */	0	,
				/*	_V_LAUNCH			 9 */	0	,
				/*	_3D_MOT				10 */	0	,
				/*	_2D_MOT				11 */	0	,
				/*	_2D_COMPRESS		12 */	0	,
				/*	_2D_PREGUIDE		13 */	0	,
				/*	_BIG_BIAS_PREGUIDE	14 */	0	,
				/*	_PINCH_COIL			15 */	0	,
				/*	_B_POLAR			16 */	0	,
				/*	_TRIGGER			17 */	0	,
				/*	_CAM_TRIGGER		18 */	0	,
				/*	_MONST_PD1_OFF		19 */	Monst_ON_Ph1_sortie_1	,
				/*	_MONST_PD2_OFF		20 */	Monst_ON_Ph1_sortie_2	,
				/*	_MONST_VERT_PD1_OFF	21 */	0	,
				/*	_MONST_VERT_PD2_OFF	22 */	1
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