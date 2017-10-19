// PortDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "PortDialog.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include "DAQmxTasksAndCodes.h"
#include "DialogVariation.h"
#include ".\portdialog.h"

// CPortDialog

IMPLEMENT_DYNAMIC(CPortDialog, CProDialog)
CPortDialog::CPortDialog()
: 
m_bUpdateAuto(1),
m_bPortChanged(0)
{
	for(int indexDig = 0; indexDig < NUMBER_DIG_OUTPUT; indexDig++)
		m_bDigOut[indexDig] = 0;
	for(int indexAnaOut = 0; indexAnaOut < NUMBER_ANA_OUTPUT; indexAnaOut++)
	{
		m_Ana_1_Out[indexAnaOut] = 0;
		m_Ana_2_Out[indexAnaOut] = 0;
	}

	for(int iemePortSave = 0; iemePortSave <= 10 - 1; iemePortSave++)
	{
		m_PortInt32_Save[iemePortSave] = 0;
		for(int indexAnaOut = 0; indexAnaOut < NUMBER_ANA_OUTPUT; indexAnaOut++)
		{
			m_PortAna_Save[iemePortSave][indexAnaOut] = 0;
		}
	}
}

CPortDialog::~CPortDialog()
{
}

BOOL CPortDialog::OnInitDialogBar()
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
void CPortDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_00		, m_NiEditAna1Out00);
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_01		, m_NiEditAna1Out01);
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_02		, m_NiEditAna1Out02);
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_03		, m_NiEditAna1Out03);
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_04		, m_NiEditAna1Out04);
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_05		, m_NiEditAna1Out05);
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_06		, m_NiEditAna1Out06);
	DDX_Control(pDX, IDC_PORT_ANA_1_OUT_07		, m_NiEditAna1Out07);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_00		, m_NiEditAna2Out00);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_01		, m_NiEditAna2Out01);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_02		, m_NiEditAna2Out02);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_03		, m_NiEditAna2Out03);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_04		, m_NiEditAna2Out04);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_05		, m_NiEditAna2Out05);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_06		, m_NiEditAna2Out06);
	DDX_Control(pDX, IDC_PORT_ANA_2_OUT_07		, m_NiEditAna2Out07);
//	DDX_Control(pDX, IDC_PORT_EDIT_SAVE_NUM		, m_NiEditSaveNum);
	//}}AFX_DATA_MAP
}

void CPortDialog::OnPaint()
{
	CPaintDC dc(this);
	if(!m_bWasPaintedOnce)
	{
		UpdateEditColor( !m_pMainFrame->m_MainDialog.m_bVarieActive );
	}
	m_bWasPaintedOnce = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Gestion de tous les controls de la PortDialog  ///////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(CPortDialog, CProDialog)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_UPDATE_NOW, 1, ValueChangedPortCheckUpdateNowButStartStopSequence, VTS_BOOL)
	
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_UPDATE_MODE		, 1, ValueChangedPortCheckUpdateAuto, VTS_BOOL)

	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_00	, 1, ValueChangedPortDigOut00, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_01	, 1, ValueChangedPortDigOut01, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_02	, 1, ValueChangedPortDigOut02, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_03	, 1, ValueChangedPortDigOut03, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_04	, 1, ValueChangedPortDigOut04, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_05	, 1, ValueChangedPortDigOut05, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_06	, 1, ValueChangedPortDigOut06, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_07	, 1, ValueChangedPortDigOut07, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_08	, 1, ValueChangedPortDigOut08, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_09	, 1, ValueChangedPortDigOut09, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_10	, 1, ValueChangedPortDigOut10, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_11	, 1, ValueChangedPortDigOut11, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_12	, 1, ValueChangedPortDigOut12, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_13	, 1, ValueChangedPortDigOut13, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_14	, 1, ValueChangedPortDigOut14, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_15	, 1, ValueChangedPortDigOut15, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_16	, 1, ValueChangedPortDigOut16, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_17	, 1, ValueChangedPortDigOut17, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_18	, 1, ValueChangedPortDigOut18, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_19	, 1, ValueChangedPortDigOut19, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_20	, 1, ValueChangedPortDigOut20, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_21	, 1, ValueChangedPortDigOut21, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_22	, 1, ValueChangedPortDigOut22, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_23	, 1, ValueChangedPortDigOut23, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_24	, 1, ValueChangedPortDigOut24, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_25	, 1, ValueChangedPortDigOut25, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_26	, 1, ValueChangedPortDigOut26, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_27	, 1, ValueChangedPortDigOut27, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_28	, 1, ValueChangedPortDigOut28, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_29	, 1, ValueChangedPortDigOut29, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_30	, 1, ValueChangedPortDigOut30, VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_DIG_OUT_31	, 1, ValueChangedPortDigOut31, VTS_BOOL)
	
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_00	, 1, ValueChangedPortAna1Out00, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_01	, 1, ValueChangedPortAna1Out01, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_02	, 1, ValueChangedPortAna1Out02, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_03	, 1, ValueChangedPortAna1Out03, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_04	, 1, ValueChangedPortAna1Out04, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_05	, 1, ValueChangedPortAna1Out05, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_06	, 1, ValueChangedPortAna1Out06, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_07	, 1, ValueChangedPortAna1Out07, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_00	, 1, ValueChangedPortAna2Out00, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_01	, 1, ValueChangedPortAna2Out01, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_02	, 1, ValueChangedPortAna2Out02, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_03	, 1, ValueChangedPortAna2Out03, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_04	, 1, ValueChangedPortAna2Out04, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_05	, 1, ValueChangedPortAna2Out05, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_06	, 1, ValueChangedPortAna2Out06, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_07	, 1, ValueChangedPortAna2Out07, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)

	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_0	, DISPID_DBLCLICK, DblClickCheckPortSave0, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_1	, DISPID_DBLCLICK, DblClickCheckPortSave1, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_2	, DISPID_DBLCLICK, DblClickCheckPortSave2, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_3	, DISPID_DBLCLICK, DblClickCheckPortSave3, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_4	, DISPID_DBLCLICK, DblClickCheckPortSave4, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_5	, DISPID_DBLCLICK, DblClickCheckPortSave5, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_6	, DISPID_DBLCLICK, DblClickCheckPortSave6, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_7	, DISPID_DBLCLICK, DblClickCheckPortSave7, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_8	, DISPID_DBLCLICK, DblClickCheckPortSave8, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_CHECK_SAVE_9	, DISPID_DBLCLICK, DblClickCheckPortSave9, VTS_NONE)
	ON_EVENT(CPortDialog, IDC_PORT_EDIT_SAVE_NUM, DISPID_KEYDOWN , KeyDownEditPortSaveNum, VTS_PI2 VTS_I2)

	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_00	, DISPID_MOUSEUP, MouseUpEditPortAna1Out00	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_01	, DISPID_MOUSEUP, MouseUpEditPortAna1Out01	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_02	, DISPID_MOUSEUP, MouseUpEditPortAna1Out02	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_03	, DISPID_MOUSEUP, MouseUpEditPortAna1Out03	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_04	, DISPID_MOUSEUP, MouseUpEditPortAna1Out04	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_05	, DISPID_MOUSEUP, MouseUpEditPortAna1Out05	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_06	, DISPID_MOUSEUP, MouseUpEditPortAna1Out06	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_1_OUT_07	, DISPID_MOUSEUP, MouseUpEditPortAna1Out07	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_00	, DISPID_MOUSEUP, MouseUpEditPortAna2Out00	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_01	, DISPID_MOUSEUP, MouseUpEditPortAna2Out01	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_02	, DISPID_MOUSEUP, MouseUpEditPortAna2Out02	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_03	, DISPID_MOUSEUP, MouseUpEditPortAna2Out03	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_04	, DISPID_MOUSEUP, MouseUpEditPortAna2Out04	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_05	, DISPID_MOUSEUP, MouseUpEditPortAna2Out05	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_06	, DISPID_MOUSEUP, MouseUpEditPortAna2Out06	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CPortDialog, IDC_PORT_ANA_2_OUT_07	, DISPID_MOUSEUP, MouseUpEditPortAna2Out07	, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()		  		

BEGIN_MESSAGE_MAP(CPortDialog, CProDialog)
	ON_WM_DROPFILES()
	ON_WM_PAINT()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Bouttons d'update : "mode" et "now!"
/////////////////////////////////////////////////////////////////////////////////////////////////

bool CPortDialog::ValueChangedPortCheckUpdateNowButStartStopSequence(BOOL Value)
{	
	m_pMainFrame->m_MainDialog.ValueChangedButtonStartSequence(Value);
return 1;
}

bool CPortDialog::ValueChangedPortCheckUpdateNow(BOOL Value)
{	
// 	m_pMainFrame->m_MainDialog.ValueChangedButtonStartSequence(Value);
// return 1;
	if(Value == false) 
		return false;

	m_pMainFrame->m_CarteDAQmxSemblePouvoirMarcher = false;

	CDAQmxStaticCardDigital32 userCodeDigital;
	CDAQmxStaticCardAnalog1 userCodeAnalog1;
// 	CDAQmxStaticCardAnalog2 userCodeAnalog2;

	CNiUInt32Vector dataDigital = userCodeDigital.GenerateNiMatrixDataForTheCard(m_bDigOut);
	CNiReal64Vector dataAnalog1 = userCodeAnalog1.GenerateNiMatrixDataForTheCard(m_Ana_1_Out);
// 	CNiReal64Vector dataAnalog2 = userCodeAnalog2.GenerateNiMatrixDataForTheCard(m_Ana_2_Out);

// 	userCodeAnalog2.SendNiMatrixDataToTheCard( dataAnalog2 );
	if(Value == UPDATE_ONLY_ANALOG2)
	{
		m_pMainFrame->m_CarteDAQmxSemblePouvoirMarcher = true;
		return true;
	}
	userCodeDigital.SendNiMatrixDataToTheCard( dataDigital );
	userCodeAnalog1.SendNiMatrixDataToTheCard( dataAnalog1 );

	m_pMainFrame->m_CarteDAQmxSemblePouvoirMarcher = true;
	return true;
}

void CPortDialog::ValueChangedPortCheckUpdateAuto(BOOL Value)
{	
	m_bUpdateAuto		=	Value ? false : true;	
	ValueChangedPortCheckUpdateNow(true);
	GetDlgItem(IDC_PORT_CHECK_UPDATE_NOW)->EnableWindow(!m_bUpdateAuto);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void CPortDialog::DrawSeq()
{
	if(m_bUpdateAuto && m_bWasPaintedOnce)
		m_pMainFrame->m_MainDialog.DrawSeq();
}
void CPortDialog::PortChanged()
{
	if(m_bUpdateAuto && m_bWasPaintedOnce)							
	{												
		m_bPortChanged = true;							
		short CheckBox = -1;						
		KeyDownEditPortSaveNum(&CheckBox, 0);		
//		m_NiEditSaveNum.Enabled = !m_bPortChanged;	
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//						Bouttons Diodes
/////////////////////////////////////////////////////////////////////////////////////////////////

void CPortDialog::ValueChangedPortDigOut00(BOOL Value){	m_bDigOut[0]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut01(BOOL Value){	m_bDigOut[1]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut02(BOOL Value){	m_bDigOut[2]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut03(BOOL Value){	m_bDigOut[3]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut04(BOOL Value){	m_bDigOut[4]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut05(BOOL Value){	m_bDigOut[5]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut06(BOOL Value){	m_bDigOut[6]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut07(BOOL Value){	m_bDigOut[7]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut08(BOOL Value){	m_bDigOut[8]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut09(BOOL Value){	m_bDigOut[9]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut10(BOOL Value){	m_bDigOut[10]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut11(BOOL Value){	m_bDigOut[11]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut12(BOOL Value){	m_bDigOut[12]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut13(BOOL Value){	m_bDigOut[13]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut14(BOOL Value){	m_bDigOut[14]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut15(BOOL Value){	m_bDigOut[15]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut16(BOOL Value){	m_bDigOut[16]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut17(BOOL Value){	m_bDigOut[17]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut18(BOOL Value){	m_bDigOut[18]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut19(BOOL Value){	m_bDigOut[19]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut20(BOOL Value){	m_bDigOut[20]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut21(BOOL Value){	m_bDigOut[21] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut22(BOOL Value){	m_bDigOut[22] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut23(BOOL Value){	m_bDigOut[23] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut24(BOOL Value){	m_bDigOut[24] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut25(BOOL Value){	m_bDigOut[25] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut26(BOOL Value){	m_bDigOut[26] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut27(BOOL Value){	m_bDigOut[27] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut28(BOOL Value){	m_bDigOut[28] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut29(BOOL Value){	m_bDigOut[29] 	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut30(BOOL Value){	m_bDigOut[30]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortDigOut31(BOOL Value){	m_bDigOut[31]	=	Value ? true : false;	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Les EditBox ActiveX de National Instruments 
/////////////////////////////////////////////////////////////////////////////////////////////////

int CPortDialog::Current3dCoil_i(int Coil_i, int Mean, int DesEq)
{
	int value = 0;
	if(Coil_i == 1)
		value = (int)(double(Mean) + (double(Mean) * (double(DesEq) / 100.0)));
	if(Coil_i == 2)
		value = (int)(double(Mean) - (double(Mean) * (double(DesEq) / 100.0)));

	return min(10000, max(0, value));
}

void CPortDialog::ValueChangedPortAna1Out00(BLABLA00){	m_Ana_1_Out[0]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna1Out01(BLABLA00){	m_Ana_1_Out[1]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna1Out02(BLABLA00){	m_Ana_1_Out[2]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna1Out03(BLABLA00){	m_Ana_1_Out[3]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna1Out04(BLABLA00){	m_Ana_1_Out[4]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna1Out05(BLABLA00){	m_Ana_1_Out[5]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna1Out06(BLABLA00){	m_Ana_1_Out[6]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna1Out07(BLABLA00){	m_Ana_1_Out[7]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out00(BLABLA00){	m_Ana_2_Out[0]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out01(BLABLA00){	m_Ana_2_Out[1]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out02(BLABLA00){	m_Ana_2_Out[2]	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out03(BLABLA00){	m_Ana_2_Out[3] 	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out04(BLABLA00){	m_Ana_2_Out[4] 	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out05(BLABLA00){	m_Ana_2_Out[5] 	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out06(BLABLA00){	m_Ana_2_Out[6] 	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}
void CPortDialog::ValueChangedPortAna2Out07(BLABLA00){	m_Ana_2_Out[7] 	=		int( Value->dblVal );	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);	if(m_bUpdateAuto) DrawSeq();	PortChanged();	}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Les DoubleClick Pour assigner les mémoires m_PortInt32_Save[0] à 9
// DblClick assign à la mémoire. Taper un chiffre dans EDIT_SAVE_NUM r'appelle la mémoire
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
Assigne à la mémoire NumSave
Active cette mémoire
Renvoye le nombre dont la représentation binaire est "les sorties 0 à 31"
*/
void CPortDialog::SavePortInMemory(unsigned short NumSave)
{
	SavePort_DigInt32Bit(NumSave);
	SavePort_Ana(NumSave);

	short CheckBox = -1;
	switch(NumSave)
	{
	case 0:	CheckBox = VK_NUMPAD0;	break;
	case 1:	CheckBox = VK_NUMPAD1;	break;
	case 2:	CheckBox = VK_NUMPAD2;	break;
	case 3:	CheckBox = VK_NUMPAD3;	break;
	case 4:	CheckBox = VK_NUMPAD4;	break;
	case 5:	CheckBox = VK_NUMPAD5;	break;
	case 6:	CheckBox = VK_NUMPAD6;	break;
	case 7:	CheckBox = VK_NUMPAD7;	break;
	case 8:	CheckBox = VK_NUMPAD8;	break;
	case 9:	CheckBox = VK_NUMPAD9;	break;
	}
	KeyDownEditPortSaveNum(&CheckBox, 0);
	GetDlgItem(IDC_PORT_EDIT_SAVE_NUM)->SetFocus();
	MessageBeep(1);
return;	
}
/*
Assigne à la mémoire NumSave les Sorties Digital stockées sous la forme 
d'un nombre dont la représentation binaire est "les sorties 0 à 31"
Renvoye ce nombre
*/
unsigned long CPortDialog::SavePort_DigInt32Bit(unsigned short NumSave)
{
	unsigned long hexa = 0;																
	unsigned long puissanceDe2 = 1;														
	for(unsigned char iChan = 0; iChan < NUMBER_DIG_OUTPUT_CARD_DIG_1; iChan++)
	{																			
		hexa += m_bDigOut[iChan] * puissanceDe2;		
		puissanceDe2 *= 2;														
	}
	m_PortInt32_Save[NumSave] = hexa;
return hexa;	
}
/*
Assigne à la mémoire NumSave les Sorties Analogiques stockées sous la forme 
de int dans le tableau m_PortAna_Save[NumSave][indexAnaOut]
*/
void CPortDialog::SavePort_Ana(unsigned short NumSave)
{
	for(int indexAnaOut = 0; indexAnaOut < NUMBER_ANA_OUTPUT; indexAnaOut++)
	{
		m_PortAna_Save[NumSave][indexAnaOut] = Sequence::GetDefaultParamAnaOut(indexAnaOut)->Get_Val();
	}
return;
}
/*
Recall la mémoire NumSave
Update la fentre port, puis les cartes, puis dessine la séquence
*/
void CPortDialog::RecallPortFromMemory(unsigned short NumSave)
{
	RecallPort_DigInt32Bit(NumSave);
	RecallPort_Ana(NumSave);
	m_bUpdateAuto = false;	//
    UpdateControls();		// astuce pour éviter de faire la séquence à chaque update d'un controle
	m_bUpdateAuto = true;	//
    UpdateControls();
	ValueChangedPortCheckUpdateNow(m_bUpdateAuto);
	DrawSeq();
	m_bPortChanged = false;
//	m_NiEditSaveNum.Enabled = !m_bPortChanged;
return;
}
/*
Recall la mémoire NumSave : les Sorties Digital 
*/
void CPortDialog::RecallPort_DigInt32Bit(unsigned short NumSave)
{
	unsigned long hexa;
	unsigned long puissanceDe2 = 2;
	hexa = m_PortInt32_Save[NumSave];		 

	for(unsigned char iChan = 0; iChan < NUMBER_DIG_OUTPUT_CARD_DIG_1; iChan++)
	{
		unsigned long Bit = 0;
		if(puissanceDe2)
			// Bit est le reste de la division par puissanceDe2
			Bit = hexa % puissanceDe2;
		else // il faut faire gaffe à ca car 2^32 = 0, et oui.
			Bit = hexa;

		m_bDigOut[iChan] = ( Bit ? 1 : 0 );
		
		hexa -= Bit;
		puissanceDe2 *= 2;
	}
return;
}
/*
Recall la mémoire NumSave : les Sorties Analogic
*/
void CPortDialog::RecallPort_Ana(unsigned short NumSave)
{
	for(int indexAnaOut = 0; indexAnaOut < NUMBER_ANA_OUTPUT; indexAnaOut++)
	{
		if(Sequence::GetDefaultParamAnaOut(indexAnaOut))
			Sequence::GetDefaultParamAnaOut(indexAnaOut)->Set_Val( m_PortAna_Save[NumSave][indexAnaOut] );
	}
return;
}
// DblClick pour sauver une Config dans une mémoire du Port
void CPortDialog::DblClickCheckPortSave0()	{	SavePortInMemory(0);	}
void CPortDialog::DblClickCheckPortSave1()	{	SavePortInMemory(1);	}
void CPortDialog::DblClickCheckPortSave2()	{	SavePortInMemory(2);	}
void CPortDialog::DblClickCheckPortSave3()	{	SavePortInMemory(3);	}
void CPortDialog::DblClickCheckPortSave4()	{	SavePortInMemory(4);	}
void CPortDialog::DblClickCheckPortSave5()	{	SavePortInMemory(5);	}
void CPortDialog::DblClickCheckPortSave6()	{	SavePortInMemory(6);	}
void CPortDialog::DblClickCheckPortSave7()	{	SavePortInMemory(7);	}
void CPortDialog::DblClickCheckPortSave8()	{	SavePortInMemory(8);	}
void CPortDialog::DblClickCheckPortSave9()	{	SavePortInMemory(9);	}
/*
gère l'appuie sur une touche numérique pour sélectionner la mémoire PortDialog
*/
void CPortDialog::KeyDownEditPortSaveNum(short* KeyCode, short Shift)
{
	CheckDlgButton(IDC_PORT_CHECK_SAVE_0, 0);	CheckDlgButton(IDC_PORT_CHECK_SAVE_1, 0);
	CheckDlgButton(IDC_PORT_CHECK_SAVE_2, 0);	CheckDlgButton(IDC_PORT_CHECK_SAVE_3, 0);
	CheckDlgButton(IDC_PORT_CHECK_SAVE_4, 0);	CheckDlgButton(IDC_PORT_CHECK_SAVE_5, 0);
	CheckDlgButton(IDC_PORT_CHECK_SAVE_6, 0);	CheckDlgButton(IDC_PORT_CHECK_SAVE_7, 0);
	CheckDlgButton(IDC_PORT_CHECK_SAVE_8, 0);	CheckDlgButton(IDC_PORT_CHECK_SAVE_9, 0);

    switch(*KeyCode)
	{
	case VK_NUMPAD0:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 0);	CheckDlgButton(IDC_PORT_CHECK_SAVE_0, 1);	RecallPortFromMemory(0);	break;
	case VK_NUMPAD1:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 1);	CheckDlgButton(IDC_PORT_CHECK_SAVE_1, 1);	RecallPortFromMemory(1);	break;
	case VK_NUMPAD2:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 2);	CheckDlgButton(IDC_PORT_CHECK_SAVE_2, 1);	RecallPortFromMemory(2);	break;
	case VK_NUMPAD3:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 3);	CheckDlgButton(IDC_PORT_CHECK_SAVE_3, 1);	RecallPortFromMemory(3);	break;
	case VK_NUMPAD4:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 4);	CheckDlgButton(IDC_PORT_CHECK_SAVE_4, 1);	RecallPortFromMemory(4);	break;
	case VK_NUMPAD5:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 5);	CheckDlgButton(IDC_PORT_CHECK_SAVE_5, 1);	RecallPortFromMemory(5);	break;
	case VK_NUMPAD6:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 6);	CheckDlgButton(IDC_PORT_CHECK_SAVE_6, 1);	RecallPortFromMemory(6);	break;
	case VK_NUMPAD7:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 7);	CheckDlgButton(IDC_PORT_CHECK_SAVE_7, 1);	RecallPortFromMemory(7);	break;
	case VK_NUMPAD8:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 8);	CheckDlgButton(IDC_PORT_CHECK_SAVE_8, 1);	RecallPortFromMemory(8);	break;
	case VK_NUMPAD9:	CheckDlgButton(IDC_PORT_EDIT_SAVE_NUM, 9);	CheckDlgButton(IDC_PORT_CHECK_SAVE_9, 1);	RecallPortFromMemory(9);	break;
	}

//	GetDlgItem(IDC_PORT_EDIT_SAVE_NUM)->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						
/////////////////////////////////////////////////////////////////////////////////////////////////

/*
Update tous les controles de la CPortDialog au cas ou on les aurait changés programmaticaly
Renvoye...?
*/
int CPortDialog::UpdateControls()
{
	CheckDlgButton(	IDC_PORT_CHECK_UPDATE_MODE	,	!m_bUpdateAuto	);	
	CheckDlgButton(	IDC_PORT_DIG_OUT_00	,	m_bDigOut[0]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_01	,	m_bDigOut[1]	);	
	CheckDlgButton(	IDC_PORT_DIG_OUT_02	,	m_bDigOut[2]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_03	,	m_bDigOut[3]	);	
	CheckDlgButton(	IDC_PORT_DIG_OUT_04	,	m_bDigOut[4]	);	
	CheckDlgButton(	IDC_PORT_DIG_OUT_05	,	m_bDigOut[5]	);	
	CheckDlgButton(	IDC_PORT_DIG_OUT_06	,	m_bDigOut[6]	);	
	CheckDlgButton(	IDC_PORT_DIG_OUT_07	,	m_bDigOut[7]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_08	,	m_bDigOut[8]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_09	,	m_bDigOut[9]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_10	,	m_bDigOut[10]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_11	,	m_bDigOut[11]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_12	,	m_bDigOut[12]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_13	,	m_bDigOut[13]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_14	,	m_bDigOut[14]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_15	,	m_bDigOut[15]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_16	,	m_bDigOut[16]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_17	,	m_bDigOut[17]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_18	,	m_bDigOut[18]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_19	,	m_bDigOut[19]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_20	,	m_bDigOut[20]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_21	,	m_bDigOut[21]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_22 ,	m_bDigOut[22]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_23 ,	m_bDigOut[23]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_24 ,	m_bDigOut[24]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_25 ,	m_bDigOut[25]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_26 ,	m_bDigOut[26]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_27 ,	m_bDigOut[27]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_28 ,	m_bDigOut[28]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_29 ,	m_bDigOut[29]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_30 ,	m_bDigOut[30]	);
	CheckDlgButton(	IDC_PORT_DIG_OUT_31	,	m_bDigOut[31]	);

	CheckDlgButton(	IDC_PORT_ANA_1_OUT_00	,	m_Ana_1_Out[0]	);
	CheckDlgButton(	IDC_PORT_ANA_1_OUT_01	,	m_Ana_1_Out[1]	);
	CheckDlgButton(	IDC_PORT_ANA_1_OUT_02	,	m_Ana_1_Out[2]	);
	CheckDlgButton(	IDC_PORT_ANA_1_OUT_03	,	m_Ana_1_Out[3]	);	
	CheckDlgButton(	IDC_PORT_ANA_1_OUT_04	,	m_Ana_1_Out[4]	);	
	CheckDlgButton(	IDC_PORT_ANA_1_OUT_05	,	m_Ana_1_Out[5]	);
	CheckDlgButton(	IDC_PORT_ANA_1_OUT_06	,	m_Ana_1_Out[6]	);
	CheckDlgButton(	IDC_PORT_ANA_1_OUT_07	,	m_Ana_1_Out[7]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_00	,	m_Ana_2_Out[0]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_01	,	m_Ana_2_Out[1]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_02	,	m_Ana_2_Out[2]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_03	,	m_Ana_2_Out[3]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_04	,	m_Ana_2_Out[4]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_05	,	m_Ana_2_Out[5]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_06	,	m_Ana_2_Out[6]	);
	CheckDlgButton(	IDC_PORT_ANA_2_OUT_07	,	m_Ana_2_Out[7]	);
	RedrawWindow(0,0, RDW_UPDATENOW );
return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//						Gestion des Ctrl + click sur les controles des paramètres à varier 
/////////////////////////////////////////////////////////////////////////////////////////////////

/*
ici : on ouvre une CDialogVariation Box.
Si on clique IDOK : On crée un CParametre qu'on ajoute à la "m_LaVariation" de "LaSequence"
*/
void CPortDialog::AddToVariation(void* LePointeur) 
{
	m_pMainFrame->m_MainDialog.AddToVariation(LePointeur);
}
void CPortDialog::MouseUpEditPortAna1Out00	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[0]);}
void CPortDialog::MouseUpEditPortAna1Out01	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[1]);}
void CPortDialog::MouseUpEditPortAna1Out02	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[2]);}
void CPortDialog::MouseUpEditPortAna1Out03	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[3]);}
void CPortDialog::MouseUpEditPortAna1Out04	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[4]);}
void CPortDialog::MouseUpEditPortAna1Out05	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[5]);}
void CPortDialog::MouseUpEditPortAna1Out06	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[6]);}
void CPortDialog::MouseUpEditPortAna1Out07	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_1_Out[7]);}
void CPortDialog::MouseUpEditPortAna2Out00	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[0]);}
void CPortDialog::MouseUpEditPortAna2Out01	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[1]);}
void CPortDialog::MouseUpEditPortAna2Out02	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[2]);}
void CPortDialog::MouseUpEditPortAna2Out03	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[3]);}
void CPortDialog::MouseUpEditPortAna2Out04	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[4]);}
void CPortDialog::MouseUpEditPortAna2Out05	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[5]);}
void CPortDialog::MouseUpEditPortAna2Out06	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[6]);}
void CPortDialog::MouseUpEditPortAna2Out07	(short Button, short Shift, long x, long y)	{	if(Shift != 2)	{return;}	AddToVariation(&m_Ana_2_Out[7]);}
// Update la Back Color des NumEdit
void CPortDialog::UpdateEditColor(bool TousBlancs /* = false */)
{	
	CVariation& LaVar =  m_pMainFrame->m_MainDialog.LaSeqMainDialog.m_LaVariation;
	unsigned int nVariantsSeqMainDialog = LaVar.nVariants;

	for(unsigned int i = 1; i <= nVariantsSeqMainDialog; i++)
	{
		CParametre* pLePar = LaVar.GetParametreIemeVariant(i);
		if(true)
		{
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[0]	){	m_NiEditAna1Out00.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[1]	){	m_NiEditAna1Out01.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[2]	){	m_NiEditAna1Out02.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[3]	){	m_NiEditAna1Out03.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[4]){	m_NiEditAna1Out04.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[5]){	m_NiEditAna1Out05.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[6]	){	m_NiEditAna1Out06.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_1_Out[7]	){	m_NiEditAna1Out07.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[0]	){	m_NiEditAna2Out00.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[1]	){	m_NiEditAna2Out01.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[2]	){	m_NiEditAna2Out02.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[3]	){	m_NiEditAna2Out03.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[4]	){	m_NiEditAna2Out04.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[5]	){	m_NiEditAna2Out05.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[6]	){	m_NiEditAna2Out06.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
			if(pLePar->Get_pVal() ==	&m_Ana_2_Out[7]	){	m_NiEditAna2Out07.SetBackColor( CNiColor(255,255, pLePar->Varie * !TousBlancs ? 0 : 255) ); }	
		}																																	   
	}																																		   
}


void CPortDialog::OnDropFiles(HDROP hDropInfo)
{
	m_pMainFrame->OnDropFiles(hDropInfo);
}

//void CPortDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
//{
//	m_pMainFrame->m_MainDialog.OnKeyDown(nChar, nRepCnt, nFlags);
//	CProDialog::OnKeyDown(nChar, nRepCnt, nFlags);
//}

BOOL CPortDialog::PreTranslateMessage(MSG* pMsg)
{
	return m_pMainFrame->m_MainDialog.PreTranslateMessage(pMsg);
}
