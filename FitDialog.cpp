// PortDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include "DialogParam.h"
#include ".\fitdialog.h"
#include "muParser.h"
using namespace mu;

#define MIN_LENGTH_FOR_CUT 20
#define LIMIT_SATURATED_PERCENT_WARNING 10.0
#define DO_NOT_CHECK_HIGHEST -0664294618.0

	CPen FitPenRouge	(PS_SOLID,	1, RGB(255 , 0   , 0	));
	CPen FitPenMauve	(PS_SOLID,	1, RGB(255 , 100 , 255	));
	CPen FitPenMauve2	(PS_SOLID,	2, RGB(255 , 100 , 255	));
	CPen FitPenJaune	(PS_SOLID,	1, RGB(255 , 255 , 0	));
	CPen FitPenGreen	(PS_SOLID,	1, RGB(0   , 255 , 0	));
	CPen FitPenGreen2	(PS_SOLID,	2, RGB(0   , 255 , 0	));

// CFitDialog
#define BoolStart true
IMPLEMENT_DYNAMIC(CFitDialog, CProDialog)
CFitDialog::CFitDialog()
: m_SaveFit("FitDialog", "Fit", false), m_WasFittingBeforeRightClickDown(false), m_RightClickingNow(false), m_LeftClickingNow(false)
{
	new CParametre(&m_SaveFit, Entier,	"Le nombre max d'iter"	,"m_NbrIterFitDialog  "	,Auto,&m_NbrIterFitDialog, "30"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"Le type de Fit "		,"m_NbrComboFit       "	,Auto,&m_NbrComboFit	, "0"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"ViseurTop      "		,"m_ZoneViseur.top    "	,Auto,&m_ZoneViseur.top	, "20"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"ViseurLeft     "		,"m_ZoneViseur.left   "	,Auto,&m_ZoneViseur.left, "20"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"ViseurBottom   "		,"m_ZoneViseur.bot    "	,Auto,&m_ZoneViseur.bottom, "120", CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"ViseurRight    "		,"m_ZoneViseur.right  "	,Auto,&m_ZoneViseur.right, "120", CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"ViseurCroixX   "		,"m_CentreCroix.x       "	,Auto,&m_CentreCroix.x	, "70"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"ViseurCroixY   "		,"m_CentreCroix.y       "	,Auto,&m_CentreCroix.y	, "70"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgRigClicUpX"		,"m_RightClickUp.x    "	,Auto,&m_RightClickUp.x	, "20"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgRigClicUpY"		,"m_RightClickUp.y    "	,Auto,&m_RightClickUp.y	, "20"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgRigClicDnX"		,"m_RightClickDown.x  "	,Auto,&m_RightClickDown.x, "70"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgRigClicDnY"		,"m_RightClickDown.y  "	,Auto,&m_RightClickDown.y, "70"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgLefClicUpX"		,"m_LeftClickUp.x    "	,Auto,&m_LeftClickUp.x	, "20"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgLefClicUpY"		,"m_LeftClickUp.y    "	,Auto,&m_LeftClickUp.y	, "20"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgLefClicDnX"		,"m_LeftClickDown.x  "	,Auto,&m_LeftClickDown.x, "70"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"FitDlgLefClicDnY"		,"m_LeftClickDown.y  "	,Auto,&m_LeftClickDown.y, "70"	, CanParticipateToNothing	);
	new CParametre(&m_SaveFit, Entier,	"Param_Fit_Int "		,"m_b[1]              "	,Auto,&m_b[1]			, "1000", CanParticipateToNothing, MAX_PARAM_FIT);
	// les valeurs de fit
	new CParametre(&m_SaveFit,Double,	"Param_Fit_Dbl "		,"m_f[1]              "	,Auto,&m_f[1]			, "1000", CanParticipateToNothing, MAX_PARAM_FIT);
	// Des résultats calculés à partir des m_f et m_Expres(sion)
	for(int i = 0; i < MAX_CALCULATED_VALUE; i++)
	{
		CString str; 
		str.Format("Expres_%s%d", int(log10(double(i)))?"":"0", i);
		new CParametre(&m_SaveFit, String, str, str, Auto, &m_Expres[i]	, "0"	, CanParticipateToNothing	);
		str.Format("Calcul_%s%d", int(log10(double(i)))?"":"0", i);
		new CParametre(&m_SaveFit,Double, str, str, Auto, &m_ValeurCalcul[i]	, "0"	, CanParticipateToNothing	);
	}

	m_b[1 ]=1;	m_b[2 ]=1;	m_b[3 ]=1;	m_b[4 ]=1;	m_b[5 ]=1;	m_b[6 ]=1;	m_b[7 ]=1;	m_b[8 ]=1;
	m_b[9 ]=1;    m_b[10]=1;	m_b[11]=1;	m_b[12]=1;	m_b[13]=1;	m_b[14]=1;	m_b[15]=1;	m_b[16]=1;
	m_f[1 ]=1.0;	m_f[2 ]=1.0;	m_f[3 ]=1.0;	m_f[4 ]=1.0;	m_f[5 ]=1.0;	m_f[6 ]=1.0;	m_f[7 ]=1.0;	m_f[8 ]=1.0;
	m_f[9 ]=1.0;	m_f[10]=1.0;	m_f[11]=1.0;	m_f[12]=1.0;	m_f[13]=1.0;	m_f[14]=1.0;	m_f[15]=1.0;	m_f[16]=1.0;
	m_MustFit = false;
	m_NbrComboFit = Gauss2D;
	m_pCurrentFit[1] = NULL;
	m_pCurrentFit[2] = NULL;
	
	CString Nom("");//Gaussian 1D Vertical & Horizontal ;Double Gaussian 1D Vertical & Horizontal ;Gaussian 2D ;Gaussian 2D with Tilt Angle ;Double Gaussian 2D Tige-Shaped;
	// Definitions des Parametres Initiaux, des fits ...
	Nom = "1D-Gaussian Vertical & Horizontal";
	CString pDesNomsH[4]		={	  "H_Amplit,MaxWhite,/... "	, "H_Center,PTX,mm"	, "H_Delta ,PTX,mm"	, "H_Offset,MaxWhite,/... "	};
	float pParamguessH[4]		={	  0.3						, 0.5				, 0.5				, 0.0				};
	float pCabbageH[2*4]		={	  0.0,4.0					, -0.5,1.5			, -2.0,2.0			, -0.05,0.7			};
	CFitting* pm_FitGaussienneH = new CFitting(Nom, FitLine, fUneGaussienne, 4, pDesNomsH, pParamguessH, pCabbageH, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitGaussienneH = *pm_FitGaussienneH;	delete pm_FitGaussienneH;

	Nom = "1D-Double Gaussian Vertical & Horizontal";
	CString pDesNomsDoubleH[7]	={	  "H_Ampl_1,MaxWhite,/... "	, "H_Cent_1,PTX,mm"	, "H_Del_1 ,PTX,mm"	, "H_Offset,MaxWhite,/... "	, "H_Ampl_2,MaxWhite,/... "	, "H_Cent_2,PTX,mm"	, "H_Del_2 ,PTX,mm"	};
	float pParamguessDoubleH[7]	={	  0.15				, 0.5						, 0.5				, 0.0				, 0.3				, 0.5				, 0.2				};
	float pCabbageDoubleH[2*7]	={	  -0.01,4.0			, -0.5,1.5					, -2.0,2.0			, -0.05,0.7			, -0.01,4.0			, -0.5,1.5			, -2.0,2.0			};
	CFitting* pm_FitDoubleGaussienneH= new CFitting(Nom, FitLine, fUneDoubleGaussienne, 7, pDesNomsDoubleH, pParamguessDoubleH, pCabbageDoubleH, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitDoubleGaussienneH = *pm_FitDoubleGaussienneH;	delete pm_FitDoubleGaussienneH;

	Nom = "1D-Gaussian Vertical & Horizontal";
	CString pDesNomsV[4]		={	  "V_Amplit,MaxWhite,/... "	, "V_Center,PTX,mm"	, "V_Delta ,PTX,mm"	, "V_Offset,MaxWhite,/... "};
	float pParamguessV[4]		={	  0.3						, 0.5				, 0.5				, 0.0				};
	float pCabbageV[2*4]		={	  0.0,4.0					, -0.5,1.5			, -2.0,2.0			, -0.05,0.7			};
	CFitting* pm_FitGaussienneV= new CFitting(Nom, FitLine, fUneGaussienne, 4, pDesNomsV, pParamguessV, pCabbageV, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitGaussienneV = *pm_FitGaussienneV;	delete pm_FitGaussienneV;

	Nom = "1D-Double Gaussian Vertical & Horizontal";
	CString pDesNomsDoubleV[7]	={	  "V_Ampl_1,MaxWhite,/... "	, "V_Cent_1,PTX,mm"	, "V_Del_1 ,PTX,mm"	, "V_Offset,MaxWhite,/... "	, "V_Ampl_2,MaxWhite,/... "	, "V_Cent_2,PTX,mm"	, "V_Del_2 ,PTX,mm"	};
	float pParamguessDoubleV[7]	={	  0.15						, 0.5				, 0.5				, 0.0						, 0.3						, 0.5				, 0.2				};
	float pCabbageDoubleV[2*7]	={	  -0.01,4.0					, -0.5,1.5			, -2.0,2.0			, -0.05,0.7					, -0.01,4.0					, -0.5,1.5			, -2.0,2.0		};
	CFitting* pm_FitDoubleGaussienneV= new CFitting(Nom, FitLine, fUneDoubleGaussienne, 7, pDesNomsDoubleV, pParamguessDoubleV, pCabbageDoubleV, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitDoubleGaussienneV = *pm_FitDoubleGaussienneV;	delete pm_FitDoubleGaussienneV;

	Nom = "2D-Gaussian with Tilt Angle";
	CString pDesNoms2D[7]		={	  "2DAmplit,MaxWhite,/... "	, "2DCent_x,PTX,mm"	, "2D_Del_x,PTX,mm"	, "2DOffset,MaxWhite,/... "	, "2DCent_y,PTY,mm"	, "2D_Del_y,PTY,mm"	, "2D_Angle,1,°"	};
	float pParamguess2D[7]		={	  0.3						, 0.0				, 0.5				, 0.0						, 0.0				, 0.2				, 0.0				};
	float pCabbage2D[2*7]		={	  -1.0,4.0					, -1.5,1.5			, -2.0,2.0			, -0.1,0.7					, -1.5,1.5			, -2.0,2.0			, -1e5,1e5		};
	CFitting* pm_FitGaussienne2DTilt = new CFitting(Nom, FitRect, fUneGaussienne2DTilt, 7, pDesNoms2D, pParamguess2D, pCabbage2D, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitGaussienne2DTilt = *pm_FitGaussienne2DTilt;	delete pm_FitGaussienne2DTilt;
 
	Nom = "2D-Gaussian";
	CFitting* pm_FitGaussienne2D = new CFitting(Nom, FitRect, fUneGaussienne2D, 6, pDesNoms2D, pParamguess2D, pCabbage2D, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitGaussienne2D = *pm_FitGaussienne2D;	delete pm_FitGaussienne2D;

	Nom = "2D- Gaussian Tige-Shaped";
	CString pDesNoms2DTigeGauss[5]		={	  "Ampl,MaxWhite,/... "		, "Cent,PTY,mm"		, "Del ,PTY,mm"		, "Offset,MaxWhite,/... "	, "Angle ,1,°"	};
	float pParamguess2DTigeGauss[5]		={	  0.5						, 0.0				, 0.5				, 0							, 0.0			};
	float pCabbage2DTigeGauss[2*5]		={	  0.0,4.0					, -2.5,2.5			, -2.0,2.0			, -0.05,0.7					,-1e5,1e5		};
	CFitting* pm_FitGaussienne2DTigeGauss = new CFitting(Nom, FitRect, fUneGaussienne2DTigeGauss, 5, pDesNoms2DTigeGauss, pParamguess2DTigeGauss, pCabbage2DTigeGauss, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitGaussienne2DTigeGauss = *pm_FitGaussienne2DTigeGauss;	delete pm_FitGaussienne2DTigeGauss;

	Nom = "2D-Double Gaussian Tige-Shaped";
	CString pDesNoms2DTigeDoubleGauss[8]		={	  "V_Ampl_1,MaxWhite,/... "	, "V_Cent_1,PTY,mm"	, "V_Del_1 ,PTY,mm"	, "Offset,MaxWhite,/... "	, "V_Ampl_2,MaxWhite,/... "	, "V_Cent_2,PTY,mm"	, "V_Del_2 ,PTY,mm"	, "Angle ,1,°"	};
	float pParamguess2DTigeDoubleGauss[8]		={	  0.5						, 0.0				, 0.15				, 0							, 0.15						, 0.0				, 0.5				, 0.0			};
	float pCabbage2DTigeDoubleGauss[2*8]		={	  0.0,4.0					, -1.0,1.0			, -1.0,1.0			, -0.05,0.7					, 0.0,4.0					, -2.5,2.5			, -2.0,2.0			,-1e5,1e5		};
	CFitting* pm_FitGaussienne2DTigeDoubleGauss = new CFitting(Nom, FitRect, fUneGaussienne2DTigeDoubleGauss, 8, pDesNoms2DTigeDoubleGauss, pParamguess2DTigeDoubleGauss, pCabbage2DTigeDoubleGauss, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_FitGaussienne2DTigeDoubleGauss = *pm_FitGaussienne2DTigeDoubleGauss;	delete pm_FitGaussienne2DTigeDoubleGauss;

	Nom = "2D-Condensat";
	CString pDesNoms2DCondensat[10]		={	  "ThqAmp,MaxWhite,/... "	, "Cent_x,PTX,mm"	, "ThqDel_x,PTX,mm"	, "Offset,MaxWhite,/... "	, "Cent_y,PTY,mm"	, "ThqDel_y,PTY,mm"	, "BECAmp,MaxWhite,/... "	, "BECDel_x,PTX,mm"	, "BECDel_y,PTX,mm"	, "BECAngle,1,°"	};
	float pParamguess2DCondensat[10]	={	  0.2						, 0.0				, 0.5				, 0.0						, 0.0				, 0.2				, 0.5						, 0.15				, 0.15				, 0.0				};
	float pCabbage2DCondensat[2*10]		={	  -1.0,4.0					, -1.5,1.5			, -10.0,10.0		, -0.1,0.7					, -1.5,1.5			, -10.0,10.0		, -1.0,4.0					, -2.0,2.0			, -2.0,2.0			, -1e5,1e5			};
	CFitting* pm_Fit2DCondensat = new CFitting(Nom, FitRect, fUne2DCondensat, 10, pDesNoms2DCondensat, pParamguess2DCondensat, pCabbage2DCondensat, m_NiEditStart, &m_NiEditLambda, &m_NiEditChi2, &m_NiSlideFitProgress);
	m_Fit2DCondensat = *pm_Fit2DCondensat;	delete pm_Fit2DCondensat;

	CString fff("SaveFitDialog.Fit");
	m_SaveFit.SetAllSettings(fff, false);

	CFitting::SetParamEnPlus(m_Expres, m_ValeurCalcul, m_StatMoyenne, m_StatStdDev, MAX_CALCULATED_VALUE);
}

CFitDialog::~CFitDialog()
{	
}

void CFitDialog::DoDataExchange(CDataExchange* pDX)
{
//	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_FIT_COMBO_THE_FIT	, m_ComboTheFit);
	DDX_Control(pDX, IDC_FIT_START_01 	, m_NiEditStart[ 1]);
	DDX_Control(pDX, IDC_FIT_START_02 	, m_NiEditStart[ 2]);
	DDX_Control(pDX, IDC_FIT_START_03 	, m_NiEditStart[ 3]);
	DDX_Control(pDX, IDC_FIT_START_04	, m_NiEditStart[ 4]);
	DDX_Control(pDX, IDC_FIT_START_05	, m_NiEditStart[ 5]);
	DDX_Control(pDX, IDC_FIT_START_06	, m_NiEditStart[ 6]);
	DDX_Control(pDX, IDC_FIT_START_07	, m_NiEditStart[ 7]);
	DDX_Control(pDX, IDC_FIT_START_08	, m_NiEditStart[ 8]);
	DDX_Control(pDX, IDC_FIT_START_09	, m_NiEditStart[ 9]);
	DDX_Control(pDX, IDC_FIT_START_10	, m_NiEditStart[10]);
	DDX_Control(pDX, IDC_FIT_START_11	, m_NiEditStart[11]);
	DDX_Control(pDX, IDC_FIT_START_12	, m_NiEditStart[12]);
	DDX_Control(pDX, IDC_FIT_START_13	, m_NiEditStart[13]);
	DDX_Control(pDX, IDC_FIT_START_14	, m_NiEditStart[14]);
	DDX_Control(pDX, IDC_FIT_START_15	, m_NiEditStart[15]);
	DDX_Control(pDX, IDC_FIT_START_16	, m_NiEditStart[16]);
	DDX_Control(pDX, IDC_FIT_LAMBDA	, m_NiEditLambda);
	DDX_Control(pDX, IDC_FIT_CHI2	, m_NiEditChi2	);
	DDX_Control(pDX, IDC_FIT_CHECK_FIT	, m_NiCheckFit	);
	DDX_Control(pDX, IDC_FIT_GRAPH_01	, m_NiGraph01);
	DDX_Control(pDX, IDC_FIT_EDIT_MAX_ITER	, m_NiEditMaxIter);
	DDX_Control(pDX, IDC_FIT_CHECK_PIXEL_SATURATED	, m_NiButtonPixelSaturated);
	DDX_Control(pDX, IDC_FIT_SLIDE_PROGRESS_1	, m_NiSlideFitProgress);
	DDX_Control(pDX, IDC_FIT_LIST_CALCUL	, m_ListCalcul);
	DDX_Control(pDX, IDC_FIT_CHECK_AVERAGE	, m_NiCheckAverage	);
	DDX_Control(pDX, IDC_FIT_EDIT_AVERAGE_SHOTS	, m_NiEditAverageShots);
	//}}AFX_DATA_MAP
}

void CFitDialog::OnPaint()
{
	CPaintDC dc(this);
	if(m_bWasPaintedOnce == false)
	{
		ValueChangedFitCheckHighest(false);

		while(m_ComboTheFit.DeleteString(0));
		m_ComboTheFit.AddString(m_FitGaussienne2D.m_Nom);
		m_ComboTheFit.AddString(m_FitGaussienne2DTilt.m_Nom);
		m_ComboTheFit.AddString(m_FitGaussienne2DTigeGauss.m_Nom);
		m_ComboTheFit.AddString(m_FitGaussienne2DTigeDoubleGauss.m_Nom);
		m_ComboTheFit.AddString(m_Fit2DCondensat.m_Nom);

		// selectionne la bonne case (dont l'index est sauvé dans le fichier de sauvegarde)
		m_ComboTheFit.SetCurSel(m_NbrComboFit);

		IdcName[1]=IDC_FIT_STATIC_01; IdcName[2]=IDC_FIT_STATIC_02; IdcName[3]=IDC_FIT_STATIC_03; IdcName[4]=IDC_FIT_STATIC_04; IdcName[5]=IDC_FIT_STATIC_05; IdcName[6]=IDC_FIT_STATIC_06; IdcName[7]=IDC_FIT_STATIC_07; IdcName[8]=IDC_FIT_STATIC_08; IdcName[9]=IDC_FIT_STATIC_09; IdcName[10]=IDC_FIT_STATIC_10; IdcName[11]=IDC_FIT_STATIC_11; IdcName[12]=IDC_FIT_STATIC_12; IdcName[13]=IDC_FIT_STATIC_13; IdcName[14]=IDC_FIT_STATIC_14; IdcName[15]=IDC_FIT_STATIC_15; IdcName[16]=IDC_FIT_STATIC_16;
		IdcAdjust[1]=IDC_FIT_ADJUST_01; IdcAdjust[2]=IDC_FIT_ADJUST_02; IdcAdjust[3]=IDC_FIT_ADJUST_03; IdcAdjust[4]=IDC_FIT_ADJUST_04; IdcAdjust[5]=IDC_FIT_ADJUST_05; IdcAdjust[6]=IDC_FIT_ADJUST_06; IdcAdjust[7]=IDC_FIT_ADJUST_07; IdcAdjust[8]=IDC_FIT_ADJUST_08; IdcAdjust[9]=IDC_FIT_ADJUST_09; IdcAdjust[10]=IDC_FIT_ADJUST_10; IdcAdjust[11]=IDC_FIT_ADJUST_11; IdcAdjust[12]=IDC_FIT_ADJUST_12; IdcAdjust[13]=IDC_FIT_ADJUST_13; IdcAdjust[14]=IDC_FIT_ADJUST_14; IdcAdjust[15]=IDC_FIT_ADJUST_15; IdcAdjust[16]=IDC_FIT_ADJUST_16;
		IdcStart[1]=IDC_FIT_START_01; IdcStart[2]=IDC_FIT_START_02; IdcStart[3]=IDC_FIT_START_03; IdcStart[4]=IDC_FIT_START_04; IdcStart[5]=IDC_FIT_START_05; IdcStart[6]=IDC_FIT_START_06; IdcStart[7]=IDC_FIT_START_07; IdcStart[8]=IDC_FIT_START_08; IdcStart[9]=IDC_FIT_START_09; IdcStart[10]=IDC_FIT_START_10; IdcStart[11]=IDC_FIT_START_11; IdcStart[12]=IDC_FIT_START_12; IdcStart[13]=IDC_FIT_START_13; IdcStart[14]=IDC_FIT_START_14; IdcStart[15]=IDC_FIT_START_15; IdcStart[16]=IDC_FIT_START_16;
		IdcUnit[1]=IDC_FIT_UNIT_01; IdcUnit[2]=IDC_FIT_UNIT_02; IdcUnit[3]=IDC_FIT_UNIT_03; IdcUnit[4]=IDC_FIT_UNIT_04; IdcUnit[5]=IDC_FIT_UNIT_05; IdcUnit[6]=IDC_FIT_UNIT_06; IdcUnit[7]=IDC_FIT_UNIT_07; IdcUnit[8]=IDC_FIT_UNIT_08; IdcUnit[9]=IDC_FIT_UNIT_09; IdcUnit[10]=IDC_FIT_UNIT_10; IdcUnit[11]=IDC_FIT_UNIT_11; IdcUnit[12]=IDC_FIT_UNIT_12; IdcUnit[13]=IDC_FIT_UNIT_13; IdcUnit[14]=IDC_FIT_UNIT_14; IdcUnit[15]=IDC_FIT_UNIT_15; IdcUnit[16]=IDC_FIT_UNIT_16;
		OnCbnSelchangeFitComboTheFit();
		m_pMainFrame->m_TraitImDialog.m_NiCheckSaveFitting.SetEnabled(m_MustFit);
		m_bWasPaintedOnce = true;

		m_ListCalcul.InsertColumn(0, "Expression", LVCFMT_LEFT, 60);
		m_ListCalcul.InsertColumn(1, "Value", LVCFMT_LEFT, 50);
		m_ListCalcul.InsertColumn(2, "Moyenne", LVCFMT_LEFT, 50);
		m_ListCalcul.InsertColumn(3, "Std Dev", LVCFMT_LEFT, 50);
		m_ListCalcul.InsertColumn(4, "Relatif", LVCFMT_LEFT, 50);
		// Des résultats calculés à partir des m_f et m_Expres(sion)
		for(int i = 0; i < MAX_CALCULATED_VALUE; i++)
		{
			m_ListCalcul.InsertItem(i, m_Expres[i]);
		}
		// checkbox de la liste
		m_ListCalcul.SetExtendedStyle(m_ListCalcul.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
		m_NiEditMaxIter.SetValue(m_NbrIterFitDialog);
		ResetStatistics();
	}
	
	// Pour les Fit, on affiche les valeurs
	// si il y a plus de un fit, on les met à la suite.
	int nTotalPar = 0;
	for(int iemeFit = 1; iemeFit <= MAX_FIT_SIMULTANNE; iemeFit++)
	{
		if(m_pCurrentFit[iemeFit])
		{
			// ligne juste pour que le Fitting prenne conscience de certain parametre tel que m_MaxWhiteValue
			m_pCurrentFit[iemeFit]->GetDibMaxWhiteValue(m_pMainFrame->m_TraitImDialog.m_pSerieCourante->m_ptrBitmap[1]);
		}
	}
	CalculateExpression();
}

BOOL CFitDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd = GetFocus();
    if(wParam == IDOK)
    {
		reinterpret_cast<CDialog *>(this)->NextDlgCtrl();
		reinterpret_cast<CDialog *>(this)->PrevDlgCtrl();
		return FALSE;
	}
	return CProDialog::OnCommand(wParam, lParam);
}


void CFitDialog::OnDropFiles(HDROP hDropInfo)
{
//	MessageBox("rien d'implémenté");
	m_pMainFrame->OnDropFiles(hDropInfo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Gestion de tous les controls					///////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(CFitDialog, CProDialog)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_01, 1, ValueChangedFitAdjust01 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_02, 1, ValueChangedFitAdjust02 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_03, 1, ValueChangedFitAdjust03 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_04, 1, ValueChangedFitAdjust04 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_05, 1, ValueChangedFitAdjust05 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_06, 1, ValueChangedFitAdjust06 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_07, 1, ValueChangedFitAdjust07 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_08, 1, ValueChangedFitAdjust08 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_09, 1, ValueChangedFitAdjust09 , VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_10, 1, ValueChangedFitAdjust10, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_11, 1, ValueChangedFitAdjust11, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_12, 1, ValueChangedFitAdjust12, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_13, 1, ValueChangedFitAdjust13, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_14, 1, ValueChangedFitAdjust14, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_15, 1, ValueChangedFitAdjust15, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_ADJUST_16, 1, ValueChangedFitAdjust16, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_01 , 1, ValueChangedFitStart01 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_02 , 1, ValueChangedFitStart02 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_03 , 1, ValueChangedFitStart03 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_04 , 1, ValueChangedFitStart04 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_05 , 1, ValueChangedFitStart05 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_06 , 1, ValueChangedFitStart06 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_07 , 1, ValueChangedFitStart07 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_08 , 1, ValueChangedFitStart08 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_09 , 1, ValueChangedFitStart09 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_10 , 1, ValueChangedFitStart10 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_11 , 1, ValueChangedFitStart11 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_12 , 1, ValueChangedFitStart12 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_13 , 1, ValueChangedFitStart13 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_14 , 1, ValueChangedFitStart14 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_15 , 1, ValueChangedFitStart15 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_START_16 , 1, ValueChangedFitStart16 , VTS_PVARIANT VTS_PVARIANT VTS_BOOL)

	ON_EVENT(CFitDialog, IDC_FIT_TO_TRAITI, 1, ValueChangedFitToTraiti, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_FIT, 1, ValueChangedFitCheckFit, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_RESET_START, 1, ValueChangedFitCheckResetStart, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_FROM_PREVIOUS, 1, ValueChangedFitCheckFromPrevious, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_WRITE_PROFILE_ON_DIB, 1, ValueChangedFitWriteProfileOnDib, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_CABBAGES, 1, ValueChangedFitCheckCabbages, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_DISCARED_SAT, 1, ValueChangedFitCheckDiscaredSat, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_HIGHEST, 1, ValueChangedFitCheckHighest, VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_EDIT_MAX_ITER, 1, ValueChangedFitEditMaxIter, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_STOP_STABLE, 1, ValueChangedFitCheckStopStable, VTS_BOOL)
	//ON_EVENT(CFitDialog, IDC_FIT_GRAPH_01, 5, PlotMouseDownFitGraph01, VTS_PI2 VTS_PI2 VTS_PVARIANT VTS_PVARIANT VTS_PI2 VTS_PI4)
	ON_EVENT(CFitDialog, IDC_FIT_CHECK_AVERAGE, 1, ValueChangedFitCheckAverage, VTS_BOOL)
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CFitDialog, CProDialog)
	ON_WM_DROPFILES()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_FIT_COMBO_THE_FIT, OnCbnSelchangeFitComboTheFit)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_FIT_LIST_CALCUL, OnLvnEndlabeleditFitListCalcul)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FIT_LIST_CALCUL, OnLvnItemchangedFitListCalcul)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_FIT_LIST_CALCUL, OnLvnBeginlabeleditFitListCalcul)
	ON_NOTIFY(NM_CLICK, IDC_FIT_LIST_CALCUL, OnNMClickFitListCalcul)
END_MESSAGE_MAP()

void CFitDialog::ValueChangedFitEditMaxIter(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	int val = int(Value->dblVal);
	if(m_pCurrentFit[1])
		m_pCurrentFit[1]->SetNbrMaxIter(val);
}

void CFitDialog::MousseRightDown(UINT nFlags, CPoint point)
{
	CPoint PixelImage = EcranToPixelImage(point);
	// y'at-il une image?
	CSize SizeDib = m_pMainFrame->m_TraitImDialog.m_pSerieCourante->GetSizeSerie();
	// sommes nous dans l'image
	if(PixelImage.x >= SizeDib.cx - 2 || PixelImage.y >= SizeDib.cy - 2
		|| PixelImage.x <= 2 || PixelImage.y <= 2)
		;//return;

	// on stop le fitting momentanement
	m_WasFittingBeforeRightClickDown = m_MustFit;
	m_pMainFrame->m_FitDialog.m_NiCheckFit.SetValue(false);

	m_RightClickingNow = true;
	m_RightClickDown = PixelImage;
	m_CentreCroix = PixelImage;
	m_RightClickUp   = PixelImage;

	SetZoneViseur(m_CentreCroix, m_RightClickUp);
}

void CFitDialog::MousseLeftDown(UINT nFlags, CPoint point)
{
	CPoint PixelImage = EcranToPixelImage(point);
	// y'at-il une image?
	CSize SizeDib = m_pMainFrame->m_TraitImDialog.m_pSerieCourante->GetSizeSerie();
	// sommes nous dans l'image
	if(PixelImage.x >= SizeDib.cx - 2 || PixelImage.y >= SizeDib.cy - 2
		|| PixelImage.x <= 1 || PixelImage.y <= 1)
		;//return;

	m_LeftClickingNow = true;
	m_LeftClickDown = PixelImage;
	m_LeftClickUp   = PixelImage;
}

void CFitDialog::MousseRightUp(UINT nFlags, CPoint point)
{
	if(!m_RightClickingNow)
		return;
	CPoint PixelImage = EcranToPixelImage(point);
	MousseMovedOnView(nFlags, PixelImage);
	m_RightClickingNow = false;
//	// si le viseur n'est pas rien, on relance le fit si m_WasFittingBeforeRightClickDown
//	if(m_ZoneViseur.Width() != 0 && m_ZoneViseur.Height() != 0)
//		m_pMainFrame->m_FitDialog.m_NiCheckFit.SetValue(m_WasFittingBeforeRightClickDown);
}

void CFitDialog::MousseLeftUp(UINT nFlags, CPoint point)
{
	if(!m_LeftClickingNow)
		return;
	CPoint PixelImage = EcranToPixelImage(point);
	m_LeftClickUp   = PixelImage;
	m_LeftClickingNow = false;
	m_View.Invalidate(false);
}

void CFitDialog::MousseMovedOnView(UINT nFlags, CPoint point)
{
	if(!(m_RightClickingNow || m_LeftClickingNow))
		return;
	CPoint PixelImage = EcranToPixelImage(point);
	if((nFlags & MK_RBUTTON) == MK_RBUTTON)
	{
		// on limite la zone à certaine valeur discrète de pas STEP_ZONE_VISEUR, autour de m_CentreCroix
		CPoint GridProche;
		GridProche.x = long(abs((PixelImage.x - m_CentreCroix.x)) / double(STEP_ZONE_VISEUR) + 0.5) * STEP_ZONE_VISEUR + m_CentreCroix.x;
		GridProche.y = long(abs((PixelImage.y - m_CentreCroix.y)) / double(STEP_ZONE_VISEUR) + 0.5) * STEP_ZONE_VISEUR + m_CentreCroix.y;
		while(GridProche.x <= 1)
			GridProche.x += STEP_ZONE_VISEUR;
		while(GridProche.y <= 1)
			GridProche.y += STEP_ZONE_VISEUR;
		while(GridProche.x >= 2 * m_CentreCroix.x)
			GridProche.x -= STEP_ZONE_VISEUR;
		while(GridProche.y >= 2 * m_CentreCroix.y)
			GridProche.y -= STEP_ZONE_VISEUR;
		// sauf si on est en dessous d'une certaine distance, comme 2*STEP_ZONE_VISEUR par exemple
		m_RightClickUp = PixelImage;
		if(abs(GridProche.x - m_CentreCroix.x) >= 2 * STEP_ZONE_VISEUR)
			m_RightClickUp.x = GridProche.x;
		if(abs(GridProche.y - m_CentreCroix.y) >= 2 * STEP_ZONE_VISEUR)
			m_RightClickUp.y = GridProche.y;
		SetZoneViseur(m_CentreCroix, m_RightClickUp);
	}
	if((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		m_LeftClickUp = PixelImage;
		m_View.Invalidate(false);
	}
}
void CFitDialog::SetZoneViseur(CPoint Centre, CPoint& To)
{
	// y'at-il une image?
	CSize SizeDib = m_pMainFrame->m_TraitImDialog.m_pSerieCourante->GetSizeSerie();
	// On va redessiner la partie souhaité de la view
	m_View.Invalidate(false);//Rect(m_ZoneViseur);

	// Le Viseur doit ne pas dépasser de l'image
	CPoint LimitUp  (min(SizeDib.cx, 2*Centre.x) - 1	,	min(SizeDib.cy, 2*Centre.y) - 1);
	CPoint LimitDown(max(0, 2*Centre.x - SizeDib.cx) + 1,	max(0, 2*Centre.y - SizeDib.cy) + 1 );
	if(SizeDib == CSize(0,0))
	{
		LimitUp = CPoint(2*Centre.x - 1, 2*Centre.y - 1);
		LimitDown = CPoint(1, 1);
	}
	CPoint& Coin1 = To;
	Coin1.x = max( min( Coin1.x,	LimitUp.x), LimitDown.x);
	Coin1.y = max( min( Coin1.y,	LimitUp.y), LimitDown.y);
	CPoint Coin2 = Centre + Centre - Coin1;
	// On peut maintenant assigner à m_Zone Viseur, et on le met dans le bon sens avec NormalizeRect
	m_ZoneViseur = CRect(Coin1, Coin2);
	m_ZoneViseur.NormalizeRect();
}

void CFitDialog::DessineTraitReference(CDC* pDC)
{
	if(m_LeftClickDown == m_LeftClickUp)
		return;

	CDC& dc = *pDC;
	dc.SelectObject(&FitPenJaune);	
	dc.MoveTo(	PixelImageToEcran(	m_LeftClickDown));
	dc.LineTo(	PixelImageToEcran(	m_LeftClickUp));
//	////////
//	CRect LeRectPoint = CRect(PixelImageToEcran(m_LeftClickUp), CSize(1, 1));
//	CPoint qsdq((m_pMainFrame->m_TraitImDialog.m_PixelAveragePointeur - 1) / 2,(m_pMainFrame->m_TraitImDialog.m_PixelAveragePointeur - 1) / 2);
//	int inflate = PixelImageToEcran(qsdq).x;
//	LeRectPoint.InflateRect(inflate, inflate);
//	dc.Rectangle(LeRectPoint);
}

void CFitDialog::DessineViseur(CDC* pDC)
{
	if(!m_ZoneViseur.Height() && !m_ZoneViseur.Width())
		return;

	CDC& dc = *pDC;

	dc.SelectObject(&FitPenGreen);	
	
	// le viseur
	dc.MoveTo(	PixelImageToEcran(	CPoint(	m_CentreCroix	+ CPoint(0, +m_ZoneViseur.Height()/2)	)));
	dc.LineTo(	PixelImageToEcran(	CPoint(	m_CentreCroix	+ CPoint(0, -m_ZoneViseur.Height()/2)	)));
	dc.MoveTo(	PixelImageToEcran(	CPoint(	m_CentreCroix	+ CPoint(+m_ZoneViseur.Width()/2, 0)	)));
	dc.LineTo(	PixelImageToEcran(	CPoint(	m_CentreCroix	+ CPoint(-m_ZoneViseur.Width()/2, 0)	)));

	// le cadre du viseur
	dc.MoveTo(	PixelImageToEcran(	m_ZoneViseur.TopLeft()											));
	dc.LineTo(	PixelImageToEcran(	m_ZoneViseur.TopLeft()		+ CPoint(0, +m_ZoneViseur.Height())	));
	dc.LineTo(	PixelImageToEcran(	m_ZoneViseur.BottomRight()										));
	dc.LineTo(	PixelImageToEcran(	m_ZoneViseur.BottomRight()	+ CPoint(0, -m_ZoneViseur.Height())	));
	dc.LineTo(	PixelImageToEcran(	m_ZoneViseur.TopLeft()											));

	// dessin de coupes
	// un rectangle de hauteur 1
	CRect CoupeH(m_ZoneViseur.left, m_ZoneViseur.CenterPoint().y, m_ZoneViseur.right, m_ZoneViseur.CenterPoint().y);
	DessineCoupe(m_pMainFrame->m_TraitImDialog.m_pSerieCourante->m_ptrBitmap[1], CoupeH, CRect(1,400,120,600), &dc);
	// un rectangle de largeur 1
	CRect CoupeV(m_ZoneViseur.CenterPoint().x, m_ZoneViseur.top, m_ZoneViseur.CenterPoint().x, m_ZoneViseur.bottom);
	DessineCoupe(m_pMainFrame->m_TraitImDialog.m_pSerieCourante->m_ptrBitmap[1], CoupeV, CRect(500,400,700,520), &dc);

	// récupération de la moyenne des pixel dans le viseur, ainsi que du nombre de pixel saturés
	CPointDbl SumSat = GetSumAndSatInZone(m_pMainFrame->m_TraitImDialog.m_pSerieCourante->m_ptrBitmap[1], m_ZoneViseur);
	m_SaturesPixelViseur = SumSat.y;
	// le boutton qui indique si c'est trop saturé
	if(m_ZoneViseur.Size().cx && m_ZoneViseur.Size().cy)
	{
		double SatPercent = double(m_SaturesPixelViseur) / m_ZoneViseur.Size().cx / m_ZoneViseur.Size().cy * 100.0;
		CString StrSatPercent; StrSatPercent.Format(_T("OK : %.1f %%"), SatPercent);
		m_NiButtonPixelSaturated.SetOffText(StrSatPercent);

		StrSatPercent; StrSatPercent.Format(_T("Not OK : %.1f %%"), SatPercent);
		m_NiButtonPixelSaturated.SetOnText(StrSatPercent);

		if(SatPercent > LIMIT_SATURATED_PERCENT_WARNING)
			CheckDlgButton(IDC_FIT_CHECK_PIXEL_SATURATED, true);
		else
			CheckDlgButton(IDC_FIT_CHECK_PIXEL_SATURATED, false);
	}

	if(m_MustFit)
		m_pCurrentFit[1]->DessineFonction(&dc);
}
// Dessine une coupe d'un bitmap dans un rectangle dans un CDC (par defaut:le dc de m_View)
void CFitDialog::DessineCoupe(CDibImagePtr& unPtrDib, CRect ZoneCoupe, CRect ZoneAffiche, CDC* pUnDc /* = NULL*/)
{
	if(!bool(unPtrDib))
		return;
	// qq vérifes
	ZoneCoupe.NormalizeRect();
	ZoneAffiche.NormalizeRect();
	if(    (ZoneCoupe.Height() < MIN_LENGTH_FOR_CUT && ZoneCoupe.Width() < MIN_LENGTH_FOR_CUT)
		|| !unPtrDib
		|| ZoneCoupe.TopLeft().x < 0
		|| ZoneCoupe.TopLeft().y < 0
		|| ZoneCoupe.BottomRight().x > unPtrDib->GetSize().cx
		|| ZoneCoupe.BottomRight().y > unPtrDib->GetSize().cy  
	  )	
		return;
	if(  (ZoneCoupe.Height() > 0 &&  ZoneCoupe.Width() > 0)
		|| !unPtrDib
	  )	
	{
		MessageBox("Pas Bonne Zone Pour la Coupe !");	
		return;	
	}

	CDC& dc = *m_View.GetDC();
	dc.SelectObject(&FitPenGreen2);

	long NumPt = long(sqrt( double(ZoneCoupe.Width()*ZoneCoupe.Width() + ZoneCoupe.Height()*ZoneCoupe.Height()) ));
	bool AfficheHorizontale = (ZoneCoupe.Width() > ZoneCoupe.Height());

	// on crée un objet CFitting juste pour accéder à CFitting, et pour pouvoir extraire les ordonnée de la coupe
	CFitting UnFit;
	float* pOrdonnee = new float[NumPt];
	CPoint* pMultiPt = new CPoint[NumPt];
	// pour pouvoir les pixels, il faut que la taille soit de 1 au moins
	if(AfficheHorizontale)
		ZoneCoupe.bottom++;
	else
		ZoneCoupe.right++;
	UnFit.GetFloatPixelsFromZone(unPtrDib, ZoneCoupe, NULL, pOrdonnee);

	// Suivant le sans d'affichage, on met la taille des ordonnée de l'affichage egale a celle de la coupe
	// et, Suivant la hauteur en ordonnée, on doit ajuster l'affichage pour que m_MaxWhiteValue tienne dans l'affichage
	// on va assigner un tableau de point qui sera facile a tracer aprés grace a dc.Polyline
	if(AfficheHorizontale)
	{
		double rescale =  double(ZoneAffiche.Height()) / double(UnFit.GetMaxWhiteValue());
		ZoneAffiche.right = ZoneAffiche.left + ZoneCoupe.Width();
		for(int i = 0; i < NumPt; i++)
		{
			pMultiPt[i].x = ZoneAffiche.left + i;
			pMultiPt[i].y = ZoneAffiche.bottom - long(pOrdonnee[i] * rescale);
		}
	}
	else
	{
		double rescale =  double(ZoneAffiche.Width()) / double(UnFit.GetMaxWhiteValue());
		ZoneAffiche.bottom = ZoneAffiche.top + ZoneCoupe.Height();
		for(int i = 0; i < NumPt; i++)
		{
			pMultiPt[i].y = ZoneAffiche.top + i;
			pMultiPt[i].x = ZoneAffiche.right - long(pOrdonnee[i] * rescale);
		}
	}

	dc.SelectObject(&FitPenRouge);	
	// on dessine le cadre de la zone d'affichage de la coupe
	dc.MoveTo(	/*PixelImageToEcran*/(	ZoneAffiche.TopLeft()											));
	dc.LineTo(	/*PixelImageToEcran*/(	ZoneAffiche.TopLeft()		+ CPoint(0, +ZoneAffiche.Height())	));
	dc.LineTo(	/*PixelImageToEcran*/(	ZoneAffiche.BottomRight()										));
	dc.LineTo(	/*PixelImageToEcran*/(	ZoneAffiche.BottomRight()	+ CPoint(0, -ZoneAffiche.Height())	));
	dc.LineTo(	/*PixelImageToEcran*/(	ZoneAffiche.TopLeft()											));

	dc.Polyline(pMultiPt, NumPt);

	delete pOrdonnee;
	delete pMultiPt;
	if(pUnDc)
		m_View.ReleaseDC(&dc);

return;
}


void CFitDialog::ValueChangedFitToTraiti(BOOL Value)
{
	m_pMainFrame->ModeDialogBar(TRAITIMAGE);
}


void CFitDialog::OnCbnSelchangeFitComboTheFit()
{
	m_NbrComboFit = m_ComboTheFit.GetCurSel();
	CString NomDuFit("");
	m_ComboTheFit.GetLBText(m_NbrComboFit, NomDuFit);

	if(0){}
	else if(NomDuFit == m_FitGaussienneH.m_Nom)
	{
		m_pCurrentFit[1] = &(m_FitGaussienneV);
		m_pCurrentFit[2] = &(m_FitGaussienneH);
	}
	else if(NomDuFit == m_FitDoubleGaussienneH.m_Nom)
	{
		m_pCurrentFit[1] = &(m_FitDoubleGaussienneV);
		m_pCurrentFit[2] = &(m_FitDoubleGaussienneH);
	}
	else if(NomDuFit == m_FitGaussienne2D.m_Nom)
	{
		m_pCurrentFit[1] = &(m_FitGaussienne2D);
		m_pCurrentFit[2] = NULL;
	}
	else if(NomDuFit == m_FitGaussienne2DTilt.m_Nom)
	{
		m_pCurrentFit[1] = &(m_FitGaussienne2DTilt);
		m_pCurrentFit[2] = NULL;
	}
	else if(NomDuFit == m_FitGaussienne2DTigeGauss.m_Nom)
	{
		m_pCurrentFit[1] = &(m_FitGaussienne2DTigeGauss);
		m_pCurrentFit[2] = NULL;
	}
	else if(NomDuFit == m_FitGaussienne2DTigeDoubleGauss.m_Nom)
	{
		m_pCurrentFit[1] = &(m_FitGaussienne2DTigeDoubleGauss);
		m_pCurrentFit[2] = NULL;
	}
	else if(NomDuFit == m_Fit2DCondensat.m_Nom)
	{
		m_pCurrentFit[1] = &(m_Fit2DCondensat);
		m_pCurrentFit[2] = NULL;
	}
	if(m_pCurrentFit[1] == NULL)
		return;
	m_pCurrentFit[1]->ResetToGuess();
	// Pour les Fit, on affiche les controls qu'il faut.
	// si il y a plus de un fit, on les met à la suite.
	int nTotalPar = 0;
	for(int j = 1; j <= MAX_FIT_SIMULTANNE; j++)
	{
		if( m_pCurrentFit[j] )
		{
			CFitting& Fit = *m_pCurrentFit[j];
			if(m_bWasPaintedOnce)
			{
				for(int i = 1; i <= MAX_PARAM_FIT; i++) 
				{// de 1 à m_nParam1 pour le 1er, de m_nParam1+1 à m_nParam1+m_nParam2, etc...
					Fit.ExportParamEnUnites(&m_f[nTotalPar + 1], MAX_PARAM_DIALOG - nTotalPar);
				}
			}
			m_pCurrentFit[j]->SetPixelSize(m_pMainFrame->m_TraitImDialog.m_PixelSize_µm, false);
			for(int i = 1; i <= MAX_PARAM_FIT; i++) 
			{// de 1 à m_nParam1 pour le 1er, de m_nParam1+1 à m_nParam1+m_nParam2, etc...
				if(nTotalPar + i < MAX_PARAM_DIALOG)
				{
					SetDlgItemText( IdcName  [nTotalPar + i], Fit.GetParamNom(i));
					SetDlgItemText( IdcUnit  [nTotalPar + i], Fit.GetParamUnit(i));
					CheckDlgButton(	IdcAdjust[nTotalPar + i], m_b[i]	);
					m_NiEditStart[nTotalPar + i].SetValue(m_f[nTotalPar + i]);
				}
			}
			nTotalPar += Fit.GetNbrParam();
		}
	}
	// les autres controles ne sont pas affichés
	for(int i = 1; i <= MAX_PARAM_DIALOG; i++)
	{
		GetDlgItem(IdcName[i]	)->ShowWindow(i <= nTotalPar);
		GetDlgItem(IdcAdjust[i]	)->ShowWindow(i <= nTotalPar);
		GetDlgItem(IdcStart[i]	)->ShowWindow(i <= nTotalPar);
		GetDlgItem(IdcUnit[i]	)->ShowWindow(i <= nTotalPar);
	}
}

void CFitDialog::ActualiseFitParameters(int iemeParamDialog)
{
	// On actualise les parametres de chaque fit en fonction de ce qui est cliqué et entré dans la fenetre
	// si il y a plus de un fit, on les met à la suite.
	int nTotalPar = 0;
	for(int j = 1; j <= MAX_FIT_SIMULTANNE; j++)
	{
		if( m_pCurrentFit[j] )
		{
			CFitting& Fit = *m_pCurrentFit[j];
			int iemeDuFit = iemeParamDialog - nTotalPar;
			if(iemeDuFit > 0 && iemeDuFit <= Fit.GetNbrParam())
			{
				Fit.SetParamAjustableEnUnites(iemeDuFit, m_f[iemeParamDialog]);
				Fit.SetBoolAdjust(iemeDuFit, m_b[iemeParamDialog]);
			}
			nTotalPar += Fit.GetNbrParam();
		}
	}
	CalculateExpression();
}

void CFitDialog::ValueChangedFitCheckFit(BOOL Value)
{
	CSerieImage& LaSerie = 	*m_pMainFrame->m_TraitImDialog.m_pSerieCourante;

	m_MustFit = Value?true:false;
	m_pMainFrame->m_TraitImDialog.m_NiCheckSaveFitting.SetEnabled(m_MustFit);
	// si on ne fit pas, on annule l'info m_FitAllreadyComputed
	if(!m_MustFit)
	{
		LaSerie.SetCurrentImageIndex(Reset_Fit_Allready_Computed);
		m_View.Invalidate(true);
		return;
	}
	if(LaSerie.m_FitAllreadyComputed)
	{
		return;
	}
	CDC& dc = *m_View.GetDC();

	// bon, On fit
	for(int iemeFit = 1; iemeFit <= MAX_FIT_SIMULTANNE; iemeFit++)
	{
		// si il y a un fit bien sur
		if(m_pCurrentFit[iemeFit])
		{
			// ligne juste pour que le Fitting prenne conscience de certain parametre tel que m_MaxWhiteValue
			m_pCurrentFit[iemeFit]->GetDibMaxWhiteValue(LaSerie.m_ptrBitmap[1]);
			// si le buffer qui contient les données double existe, c'est qu'il a été calculé, et on l'utilise
			if(LaSerie.Get_pResultatCalculImageY())
				m_pCurrentFit[iemeFit]->LanceUnFit(LaSerie.GetSizeSerie(), LaSerie.Get_pResultatCalculImageY(), m_ZoneViseur, &dc);
			else
				m_pCurrentFit[iemeFit]->LanceUnFit(LaSerie.m_ptrBitmap[1], m_ZoneViseur, &dc);
		}
	}
	LaSerie.m_FitAllreadyComputed = true;
	m_View.ReleaseDC(&dc);

	CalculateStatistics_WithThose_ValeurCalcul();
}


void CFitDialog::ValueChangedFitCheckResetStart(BOOL Value)
{
	if(Value == 0)
		return;
	m_pCurrentFit[1]->ResetToGuess();
}
void CFitDialog::ValueChangedFitCheckFromPrevious(BOOL Value)
{
	CFitting::FittingFromPreviousPos(Value);
}
void CFitDialog::ValueChangedFitWriteProfileOnDib(BOOL Value)
{
	if(Value)
	{
		m_pCurrentFit[1]->WriteProfileOnDib(m_pMainFrame->m_TraitImDialog.m_pSerieCourante->m_ptrBitmap[1], m_ZoneViseur);
	}
	else
	{
		m_pMainFrame->m_TraitImDialog.m_pSerieCourante->OuvreImage();
	}
	m_pMainFrame->m_TraitImDialog.AfficheNewBitmapSerieCourante();
}
void CFitDialog::ValueChangedFitCheckCabbages(BOOL Value)
{
	CFitting::CabbagesChecking(Value?true:false);
}
void CFitDialog::ValueChangedFitCheckDiscaredSat(BOOL Value)
{
	CFitting::DiscaredSaturatedPoints(Value?true:false);
}
void CFitDialog::ValueChangedFitCheckStopStable(BOOL Value)
{
	CFitting::StopWhenStable(Value?true:false);
}
void CFitDialog::AddValueToGraph(double ValueToAppend, unsigned int TimeInMilliSec)
{
	// la valeur la plus haute est la myonne entre le point précédent et le nouveau
	double OldValOscillo = m_ValOscilloscope;
	m_ValOscilloscope = ValueToAppend;
	if(m_HighestValOscilloscope != DO_NOT_CHECK_HIGHEST)
		m_HighestValOscilloscope = max(m_HighestValOscilloscope, (ValueToAppend + OldValOscillo)/2);

	long ChartLength = 200;
	m_NiGraph01.SetChartLength(ChartLength);

	// fixe le minimum et maximum sur l'axe x
//	double initialrange = 5.0;
//	m_NiGraph01.GetAxes().Item("XAxis").SetMinMax(0.0, initialrange);

	// plot n° 0 , on ajoute un point m_ValOscilloscope
	// a l'abscisse correspondant à la date de prise d'image
	unsigned int MilliSecLastPointAdded = m_MilliSecLastResetCompteur;
	m_MilliSecLastResetCompteur = TimeInMilliSec;
	double IncrementX = (m_MilliSecLastResetCompteur - MilliSecLastPointAdded) / 1000.0;
//	if(IncrementX > initialrange)
//	{
//		return;
//	}
	m_NiGraph01.ChartY(m_ValOscilloscope, IncrementX);
	m_NiGraph01.GetAxes().Item("XAxis").AutoScaleNow();
//	m_NiGraph01.GetAxes().Item("XAxis").SetMinimum(0);

	if(m_HighestValOscilloscope != DO_NOT_CHECK_HIGHEST)
	{
		// une ligne droite entre deux point de hauteur m_HighestValOscilloscope, et d'abscisse debut-fin du graph
		// ainsi que le droite maxi/e
		CNiReal64Vector UnVecteurHighest(2, m_HighestValOscilloscope);
		CNiReal64Vector UnVecteurHighest_Sur_e(2, m_HighestValOscilloscope * exp(-1.0));
        double miniX = m_NiGraph01.GetAxes().Item("XAxis").GetMinimum();
        double maxiX = m_NiGraph01.GetAxes().Item("XAxis").GetMaximum();
		m_NiGraph01.Plots.Item("Plot-2").PlotY(UnVecteurHighest		, miniX + 0.1, maxiX - miniX - 0.2);
		m_NiGraph01.Plots.Item("Plot-3").PlotY(UnVecteurHighest_Sur_e, miniX + 0.1, maxiX - miniX - 0.2);
	}

	// fixe le minimum sur l'axe y=0, et le maximum en autoscale.
	m_NiGraph01.GetAxes().Item("YAxis-1").AutoScaleNow();
	m_NiGraph01.GetAxes().Item("YAxis-1").SetMinimum(0);

	// pour trouver le temps de décroissance exponentielle
	// on mesure le temps qu'il faut pour passer de 90% de Highest
	// à 90% de Highest_Sur_e
	CNiAnnotation annotation = m_NiGraph01.Annotations.Item(1);
	if(m_ValOscilloscope < 0.9 * m_HighestValOscilloscope
		&& OldValOscillo > 0.9 * m_HighestValOscilloscope
		&& m_HighestValOscilloscope != DO_NOT_CHECK_HIGHEST)
	{
		m_MilliSecInf90Highest = TimeInMilliSec;
		annotation.Visible = false;
	}
	if(m_ValOscilloscope < 0.9 * m_HighestValOscilloscope * exp(-1.0)
		&& OldValOscillo > 0.9 * m_HighestValOscilloscope * exp(-1.0)
		&& m_HighestValOscilloscope != DO_NOT_CHECK_HIGHEST
		&& m_MilliSecInf90Highest)// seulement si on a pas fixé m_MilliSecInf90Highest à 0, lla valeur s'affiche, pour avoir m_MilliSecInf90Highest != 0, il ffaut que m_ValOscilloscope repasse a m_highest
	{
		m_MilliSecInf90Highest_Sur_e = TimeInMilliSec;
		CString StrTime; StrTime.Format(_T("%3.1f s"), (m_MilliSecInf90Highest_Sur_e - m_MilliSecInf90Highest) / 1000.0);
		annotation.Caption.SetText(StrTime);
		annotation.Caption.SetXCoordinate(m_NiGraph01.GetAxes().Item("XAxis").GetMaximum());
		annotation.Caption.SetYCoordinate(m_NiGraph01.GetAxes().Item("YAxis-1").GetMaximum());
		annotation.Visible = true;
		m_MilliSecInf90Highest = 0;
	}
	if(m_HighestValOscilloscope == DO_NOT_CHECK_HIGHEST)
		annotation.Visible = false;
}

void CFitDialog::ValueChangedFitCheckHighest(BOOL Value)
{	bool val = Value? true : false;
	if(!val)
	{
		m_HighestValOscilloscope = DO_NOT_CHECK_HIGHEST;
		m_NiGraph01.Plots.Item("Plot-2").ClearData();
		m_NiGraph01.Plots.Item("Plot-3").ClearData();
	}
	else
	{	// on quite la valeur DO_NOT_CHECK_HIGHEST
		m_HighestValOscilloscope++;
		// remise a zero du compteur precis
//		m_pMainFrame->m_CamDialog.CompteurPrecisMillisecondes(true);
		m_NiGraph01.Plots.Item("Plot-2").ClearData();
		m_NiGraph01.Plots.Item("Plot-3").ClearData();
		m_NiGraph01.Plots.Item("Plot-1").ClearData();
	}
}



void CFitDialog::OnLvnEndlabeleditFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	if(pDispInfo->item.pszText)
		m_Expres[pDispInfo->item.iItem] = pDispInfo->item.pszText;
	m_ListCalcul.SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, m_Expres[pDispInfo->item.iItem]);
	CalculateExpression();
	*pResult = 0;
}
void CFitDialog::OnLvnItemchangedFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CFitting::ResetParamEnPlus();
	for(int indexCalc = 0; indexCalc < MAX_CALCULATED_VALUE; indexCalc++)
	{
		bool isChecked = ListView_GetCheckState(m_ListCalcul.GetSafeHwnd( ), indexCalc);
		if(isChecked)
			CFitting::AddParamEnPlus(m_Expres + indexCalc, m_ValeurCalcul + indexCalc, m_StatMoyenne + indexCalc, m_StatStdDev + indexCalc);
	}
	*pResult = 0;
}
void CFitDialog::OnLvnBeginlabeleditFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}
void CFitDialog::OnNMClickFitListCalcul(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}
void CFitDialog::CalculateExpression()
{
	if(m_pCurrentFit[1] == NULL)
		return;
	CString resultat;
	// un parser : prend une string formula, et en fait une fonction
	mu::Parser parser;
	// on definit les constantes
	CFitting & Fit = *m_pCurrentFit[1];
	double chi = Fit.GetChiCarre();
	double somme = Fit.GetSommePixels();
	double NombrAtomeGaussienne2D = 2 * 3.14159 * m_f[1] * m_f[3] * m_f[6] / 2.907e-7/*sigma0 en mm²*/;
	parser.DefineVar("a", &m_f[1]);	parser.DefineVar("A", &m_f[1]);
	parser.DefineVar("b", &m_f[2]);	parser.DefineVar("B", &m_f[2]);
	parser.DefineVar("c", &m_f[3]);	parser.DefineVar("C", &m_f[3]);
	parser.DefineVar("d", &m_f[4]);	parser.DefineVar("D", &m_f[4]);
	parser.DefineVar("e", &m_f[5]);	parser.DefineVar("E", &m_f[5]);
	parser.DefineVar("f", &m_f[6]);	parser.DefineVar("F", &m_f[6]);
	parser.DefineVar("g", &m_f[7]);	parser.DefineVar("G", &m_f[7]);
	parser.DefineVar("h", &m_f[8]);	parser.DefineVar("H", &m_f[8]);
	parser.DefineVar("i", &m_f[9]);	parser.DefineVar("I", &m_f[9]);
	parser.DefineVar("j", &m_f[10]);parser.DefineVar("J", &m_f[10]);
	parser.DefineVar("k", &chi);   	parser.DefineVar("K", &chi);  
	parser.DefineVar("s", &somme); 	parser.DefineVar("S", &somme);   
	parser.DefineVar("NatGauss2D", &NombrAtomeGaussienne2D);	parser.DefineVar("natgauss2d", &NombrAtomeGaussienne2D);

	for(int indexCalc = 0; indexCalc < MAX_CALCULATED_VALUE; indexCalc++)
	{
		// puis la formule
		std::string formule = m_Expres[indexCalc];
		parser.SetExpr(formule);
		try
		{
			m_ValeurCalcul[indexCalc] = parser.Eval();
		}
		catch(Parser::exception_type &e)
		{
			m_ListCalcul.SetItemText(indexCalc, 1, "Error");
			continue;
		}
		resultat.Format("%.3g", m_ValeurCalcul[indexCalc]);
		// on enleve les 0 en trop devant l'exposant
		resultat.Replace("+0", "+");	resultat.Replace("+0", "+");
		resultat.Replace("-0", "-");	resultat.Replace("-0", "-");
		resultat.Replace("+", "");	
		m_ListCalcul.SetItemText(indexCalc, 1, resultat);
	}
}
void CFitDialog::ValueChangedFitCheckAverage(BOOL Value)
{
	ResetStatistics();
	if(!Value)
		CalculateStatistics_WithThose_ValeurCalcul();
}
void CFitDialog::ResetStatistics()
{	
	m_StatNbrShotsCourant = 0;
	for(int indexCalc = 0; indexCalc < MAX_CALCULATED_VALUE; indexCalc++)
	{
		m_StatSommeCalcul[indexCalc] = 0;
		m_StatSommeCalculCarre[indexCalc] = 0;
		CString strAverageNbr = "";
		strAverageNbr.Format("Averaged Over %d", m_StatNbrShotsCourant);
		m_NiCheckAverage.SetOnText(strAverageNbr);
	}
}
void CFitDialog::CalculateStatistics_WithThose_ValeurCalcul()
{	// si on average pas
	CString strMoy = "", strStD = "", strFrac = "";
	bool DoStat = m_NiCheckAverage.GetValue();
	int NbrOverWichToAverage = int(m_NiEditAverageShots.GetValue());
	if(NbrOverWichToAverage == 0)
		NbrOverWichToAverage = 1e9;
	if(!DoStat || m_StatNbrShotsCourant >= NbrOverWichToAverage)
		ResetStatistics();

	m_StatNbrShotsCourant++;
	for(int indexCalc = 0; indexCalc < MAX_CALCULATED_VALUE; indexCalc++)
	{
		if(DoStat)
		{
			m_StatSommeCalcul[indexCalc] += m_ValeurCalcul[indexCalc];
			m_StatSommeCalculCarre[indexCalc] += m_ValeurCalcul[indexCalc] * m_ValeurCalcul[indexCalc];
			m_StatMoyenne[indexCalc] = m_StatSommeCalcul[indexCalc] / double(m_StatNbrShotsCourant);
			double NSommeDesCarre = double(m_StatNbrShotsCourant) * m_StatSommeCalculCarre[indexCalc];
			double SommeAuCarre = m_StatSommeCalcul[indexCalc] * m_StatSommeCalcul[indexCalc];
			double NstDevCarre = NSommeDesCarre - SommeAuCarre;
			// tenons compte de la precision de maniere à ne pas afficher des e-16, quand en fait c'est zero.
			if(NstDevCarre < NSommeDesCarre * 1e-14)
				NstDevCarre = 0;
			m_StatStdDev[indexCalc] = sqrt(NstDevCarre) / double(m_StatNbrShotsCourant);

			strMoy.Format("%.3g", m_StatMoyenne[indexCalc]);
			strStD.Format("%.3g", m_StatStdDev[indexCalc]);
			strFrac.Format("%.2g", m_StatStdDev[indexCalc] / m_StatMoyenne[indexCalc]);
			strMoy.Replace("+0", "+");	strMoy.Replace("+0", "+");
			strMoy.Replace("-0", "-");	strMoy.Replace("-0", "-");
			strMoy.Replace("+", "");	
			strStD.Replace("+0", "+");	strStD.Replace("+0", "+");
			strStD.Replace("-0", "-");	strStD.Replace("-0", "-");
			strStD.Replace("+", "");	
			strFrac.Replace("+0", "+");	strFrac.Replace("+0", "+");
			strFrac.Replace("-0", "-");	strFrac.Replace("-0", "-");
		}
			m_ListCalcul.SetItemText(indexCalc, 2, strMoy);
			m_ListCalcul.SetItemText(indexCalc, 3, strStD);
			m_ListCalcul.SetItemText(indexCalc, 4, strFrac);
	}
	CString strAverageNbr = "";
	strAverageNbr.Format("Averaged Over %d", m_StatNbrShotsCourant);
	m_NiCheckAverage.SetOnText(strAverageNbr);
}



