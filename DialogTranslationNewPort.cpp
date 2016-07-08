// DialogTranslationNewPort.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include ".\dialogtranslationnewport.h"
#include "NewPortXPS.h"

enum{PeriodeMonitoringXPSPosVit_ms = 5000};
// Boîte de dialogue CDialogTranslationNewPort

IMPLEMENT_DYNAMIC(CDialogTranslationNewPort, CDialog)
CDialogTranslationNewPort::CDialogTranslationNewPort(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTranslationNewPort::IDD, pParent)
{
	m_UseTheXPSOfTheDialog = true;
	m_AutoSave = false;
	m_pXPS = new CNewPortXPS();
	m_pXPS->m_pEditXPSResult = &m_EditText;
	m_pXPS->m_pEditXPSEvent = &m_EditEvent;
	m_pXPS->m_pEditXPSAction = &m_EditAction;
	m_pXPS->m_pComboPVT = &m_ComboPVT;
	m_pXPS->m_pComboGroupName = &m_ComboGroupName;
	m_pXPS->m_pComboPositionerName = &m_ComboPositionerName;
	m_pXPS->m_pNiGraphVitesse[1] = &m_NiGraphVitesse;
	m_pXPS->m_pNiGraphPosition[1] = &m_NiGraphPosition;
	m_IdTimerCheckingGatheringResult = -1;
}

CDialogTranslationNewPort::CDialogTranslationNewPort(CNewPortXPS* pTranslationXPS, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTranslationNewPort::IDD, pParent)
{
	m_UseTheXPSOfTheDialog = false;
	m_AutoSave = false;
	m_pXPS = pTranslationXPS;
	m_pXPS->m_pEditXPSResult = &m_EditText;
	m_pXPS->m_pEditXPSEvent = &m_EditEvent;
	m_pXPS->m_pEditXPSAction = &m_EditAction;
	m_pXPS->m_pComboPVT = &m_ComboPVT;
	m_pXPS->m_pComboGroupName = &m_ComboGroupName;
	m_pXPS->m_pComboPositionerName = &m_ComboPositionerName;
	m_pXPS->m_pNiGraphVitesse[1] = &m_NiGraphVitesse;
	m_pXPS->m_pNiGraphPosition[1] = &m_NiGraphPosition;
	m_IdTimerCheckingGatheringResult = -1;
}

CDialogTranslationNewPort::~CDialogTranslationNewPort()
{
	m_pXPS->m_pEditXPSResult = NULL;
	m_pXPS->m_pEditXPSEvent = NULL;
	m_pXPS->m_pEditXPSAction = NULL;
	m_pXPS->m_pComboPVT = NULL;
	m_pXPS->m_pComboGroupName = NULL;
	m_pXPS->m_pComboPositionerName = NULL;
	m_pXPS->m_pNiGraphVitesse[1] = NULL;
	m_pXPS->m_pNiGraphPosition[1] = NULL;
	if(m_UseTheXPSOfTheDialog)
		delete m_pXPS;

}
BOOL CDialogTranslationNewPort::OnInitDialog()
{
	CDialog::OnInitDialog();


	ActualiseControls();
	ActualiseGraphs();
	m_ComboAnalogIn.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}
void CDialogTranslationNewPort::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	m_EditIP.SetWindowText("129.199.119.138");
}
void CDialogTranslationNewPort::ActualiseGraphs()
{
	m_XPS.ActualiseGraphs();
}
void CDialogTranslationNewPort::ActualiseControls(bool checkPVT /* = false */)
{

//	CPoint Trig_GPIO = m_XPS.GetTriggerToUseForTrajectoryViaScript();
//	m_ComboTrig.SetCurSel(Trig_GPIO.x);
	m_NiEditNbrExecution.SetValue(m_XPS.PVTNbrExecutionGet());
	m_NiCheckConnected.SetValue(m_XPS.IsConnected());

	// What is the group state ?
	CString GroupStateXPS = m_XPS.GetGroupStateString();

	m_NiCheckControlled.SetValue(GroupStateXPS.Find("Ready state") != -1);
	// si on est en tracking
	bool isTracking = GroupStateXPS.Find("tracking state") != -1;
	if(isTracking)
	{
		// controle jaune
		m_NiCheckControlled.SetOffColor(CNiColor(255,255,0));
		m_NiGraphVitesse.Axes.Item(2).FormatString = "k*.001\"m\"";
	}
	else
	{
		m_NiCheckControlled.SetOffColor(CNiColor(255,0,0));
		m_NiGraphVitesse.Axes.Item(2).FormatString = "k*.001\"m/s\"";
	}
		m_NiGraphVitesse.GetPlots().Item(1).Visible = !isTracking;
		m_NiGraphVitesse.GetPlots().Item("AnalogIn").Visible = isTracking;
		m_NiGraphVitesse.GetPlots().Item("RealPos").Visible = isTracking;
		m_NiGraphVitesse.GetPlots().Item("ErrorPos").Visible = isTracking;
		m_NiGraphVitesse.Axes.Item(3).Visible = isTracking;
		m_NiGraphPosition.GetPlots().Item(1).Visible = !isTracking;
		m_NiGraphPosition.GetPlots().Item("AnalogIn").Visible = isTracking;
		m_NiGraphPosition.GetPlots().Item("RealPos").Visible = isTracking;
		m_NiGraphPosition.GetPlots().Item("ErrorPos").Visible = isTracking;
		m_NiGraphPosition.Axes.Item(3).Visible = isTracking;
	m_NiCheckMonitorTrackingPositions.SetEnabled(isTracking);
	m_NiChekTrackPosition.Value = isTracking;

	m_NiCheckControlled.SetOffText(GroupStateXPS);
	m_NiCheckControlled.SetOnText(GroupStateXPS);

	CString PosStateXPS = m_XPS.GetPostionnerStatusString();

	m_EditIP.SetWindowText(m_XPS.GetIP());
	m_EditGroupName.SetWindowText(m_XPS.GetGroupName());

	if(!m_XPS.IsConnected())
		return;

	CString strPosExact("");
	strPosExact.Format("%f", m_XPS.GetPos(1));
	m_NiSlidePosition_1.GetAxis().SetCaption(strPosExact + "mm");
	m_NiSlidePosition_1.SetValue(m_XPS.GetPosTheoric(1));
	m_NiEditPosition_1.SetValue(m_XPS.GetPosTheoric(1));
	strPosExact.Format("%f", m_XPS.GetPos(2));
	m_NiSlidePosition_2.GetAxis().SetCaption(strPosExact + "mm");
	m_NiSlidePosition_2.SetValue(m_XPS.GetPosTheoric(2));
	m_NiEditPosition_2.SetValue(m_XPS.GetPosTheoric(2));

	m_EditEvent.SetWindowText(m_XPS.GetAllEventsAndActions());
//	m_EditAction.SetWindowText(m_XPS.GetAction());

	if(!checkPVT)
		return;

	m_EditPVTFileName.SetWindowText(m_XPS.PVTFileNameForSaveGet());

	CPointDbl Verfi = m_XPS.PVTVerifie();
	CPointDbl AbsoluteMinMax = m_XPS.m_PvtMinMaxAbsolutePosition[1/*arevoir?*/];
	// si c'est pas un bon PVT
	if(Verfi == PVT_FILE_DOESNT_EXIST)
	{
		m_NiCheckStart.SetOffText("No File !");
		m_NiCheckStart.SetOffTextColor(CNiColor(255,255,0));
		m_NiSlidePosition_1.GetPointers().Item("RangePVTMin").SetValue(0);
		m_NiSlidePosition_1.GetPointers().Item("RangePVTMax").SetValue(0);
		return;
	}
	if(!isTracking)
		m_NiEditPvtTotalTime.SetValue(m_XPS.m_WfVitesse[1].GetPoint(-1).x);
	m_NiEditPvtVelocityAmplitude.SetValue(m_XPS.m_PvtMaxVelocityMaxAcceleration[1].x);
	m_NiEditPvtPositonAmplitude.SetValue(m_XPS.m_PvtMinMaxAbsolutePosition[1].y - m_XPS.m_PvtMinMaxAbsolutePosition[1].x);
	if(Verfi == PVT_VERIFY_ERROR)
	{
		m_NiCheckStart.SetOffText("Error !");
		m_NiCheckStart.SetOffTextColor(CNiColor(255,255,0));
		m_NiSlidePosition_1.GetPointers().Item("RangePVTMin").SetValue(0);
		m_NiSlidePosition_1.GetPointers().Item("RangePVTMax").SetValue(0);
		return;
	}
	CPointDbl limitMinMax = m_XPS.GetMinMaxPosition(1);
	if(		AbsoluteMinMax.x < limitMinMax.x
		||	AbsoluteMinMax.y > limitMinMax.y)
	{
		m_NiCheckStart.SetOffText("Out Of Range !");
		m_NiCheckStart.SetOffTextColor(CNiColor(255,255,0));
		m_NiSlidePosition_1.SetInteriorColor(CNiColor(255,0,0));
	}
	else
	{
		m_NiCheckStart.SetOffText("Start");
		m_NiSlidePosition_1.SetInteriorColor(CNiColor(0,0,255));
	}
	m_NiCheckStart.SetOffTextColor(CNiColor(0,0,0));
	m_NiSlidePosition_1.GetPointers().Item("RangePVTMin").SetValue(AbsoluteMinMax.x);
	m_NiSlidePosition_1.GetPointers().Item("RangePVTMax").SetValue(AbsoluteMinMax.y);
}
BOOL CDialogTranslationNewPort::OnCommand(WPARAM wParam, LPARAM lParam)
{
    CWnd *pWnd = GetFocus();
    if(wParam == IDOK && pWnd != GetDlgItem(IDOK))
    {
		reinterpret_cast<CDialog *>(this)->NextDlgCtrl();
		reinterpret_cast<CDialog *>(this)->PrevDlgCtrl();
		return FALSE;
	}
	return CDialog::OnCommand(wParam, lParam);
}
void CDialogTranslationNewPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_TEXT, m_EditText);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_EVENT, m_EditEvent);
	DDX_Control(pDX, IDC_TRANSLATION_SLIDE_POSITION_1, m_NiSlidePosition_1);
	DDX_Control(pDX, IDC_TRANSLATION_SLIDE_POSITION_2, m_NiSlidePosition_2);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_POSITION_1, m_NiEditPosition_1);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_POSITION_2, m_NiEditPosition_2);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_CONTROLLED, m_NiCheckControlled);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_CONTROLLED2, m_NiCheckControlled2);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_TRACK_POSITION, m_NiChekTrackPosition);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_TRACK_VELOCITY, m_NiChekTrackVelocity);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_CONNECTED, m_NiCheckConnected);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_AUTO_SAVE, m_NiCheckAutoSave);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_PVT_TOTAL_TIME, m_NiEditPvtTotalTime);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_DELAY_MONITOR_POS_VIT, m_NiEditDelayMonitorPosVit);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_PVT_VELOCITY_AMPLITUDE, m_NiEditPvtVelocityAmplitude);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_PVT_POSITON_AMPLITUDE, m_NiEditPvtPositonAmplitude);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_GATHERING_FREQ, m_NiEditGatheringFreq);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_GATHERING_DURATION, m_NiEditGatheringDuration);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_GATHERING_GET_FILE, m_NiCheckGatheringGetFile);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_GAHERING_FILE_NAME, m_EditGatheringFileName);
	DDX_Control(pDX, IDC_TRANSLATION_GRAPH_VITESSE, m_NiGraphVitesse);
	DDX_Control(pDX, IDC_TRANSLATION_COMBO_TRIGGER, m_ComboTrig);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_PVT_FILENAME, m_EditPVTFileName);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_IP, m_EditIP);
	DDX_Control(pDX, IDC_TRANSLATION_EDIT_GROUP_NAME, m_EditGroupName);
	DDX_Control(pDX, IDC_TRANSLATION_START, m_NiCheckStart);
	DDX_Control(pDX, IDC_CWNUMEDIT1, m_NiEditNbrExecution);
	DDX_Control(pDX, IDC_TRANSLATION_COMBO_PVT, m_ComboPVT);
	DDX_Control(pDX, IDC_TRANSLATION_COMBO_GROUP_NAME, m_ComboGroupName);
	DDX_Control(pDX, IDC_TRANSLATION_COMBO_POSITIONER_NAME, m_ComboPositionerName);
	DDX_Control(pDX, IDC_TRANSLATION_GRAPH_POSITION, m_NiGraphPosition);
	DDX_Control(pDX, IDC_TRANSLATION_COMBO_ANALOG_IN, m_ComboAnalogIn);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_SAVE, m_NiCheckSavePVT);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_MONITOR_TRACKING_POSITIONS, m_NiCheckMonitorTrackingPositions);
	DDX_Control(pDX, IDC_TRANSLATION_CHECK_MONITOR_TRACKING_GET_FILE, m_NiCheckMonitorTrackingGetFile);
}


BEGIN_MESSAGE_MAP(CDialogTranslationNewPort, CDialog)
	ON_EN_CHANGE(IDC_TRANSLATION_EDIT_PVT_FILENAME, OnEnChangeTranslationEditPvtFilename)
	ON_EN_CHANGE(IDC_TRANSLATION_EDIT_IP, OnEnChangeTranslationEditIP)
	ON_EN_CHANGE(IDC_TRANSLATION_EDIT_GROUP_NAME, OnEnChangeTranslationEditGroupName)
	ON_CBN_SELCHANGE(IDC_TRANSLATION_COMBO_TRIGGER, OnCbnSelchangeTranslationComboTrigger)
	ON_CBN_SELCHANGE(IDC_TRANSLATION_COMBO_PVT, OnCbnSelchangeTranslationComboPvt)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_TRANSLATION_COMBO_GROUP_NAME, OnCbnSelchangeTranslationComboGroupName)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogTranslationNewPort
BEGIN_EVENTSINK_MAP(CDialogTranslationNewPort, CDialog)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_SLIDE_POSITION_1, 1, PointerValueChangedTranslationSlidePosition_1, VTS_I4 VTS_PVARIANT)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_SLIDE_POSITION_2, 1, PointerValueChangedTranslationSlidePosition_2, VTS_I4 VTS_PVARIANT)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_SLIDE_POSITION_1, DISPID_MOUSEUP, MouseUpTranslationSlidePosition_1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_SLIDE_POSITION_2, DISPID_MOUSEUP, MouseUpTranslationSlidePosition_2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_POSITION_1, 1, ValueChangedTranslationEditPosition_1, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_POSITION_2, 1, ValueChangedTranslationEditPosition_2, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_CONTROLLED, DISPID_CLICK, ClickTranslationCheckControlled, VTS_NONE)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_CONNECTED, DISPID_CLICK, ClickTranslationCheckConnected, VTS_NONE)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_GRAPH_VITESSE, DISPID_DBLCLICK, DblClickTranslationGraphVitesse, VTS_NONE)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_START, 1, ValueChangedTranslationStart, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_CWNUMEDIT1, 1, ValueChangedCwnumedit1, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_JERK_NULL, 1, ValueChangedTranslationCheckJerkNull, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_SAVE, 1, ValueChangedTranslationCheckSave, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_INITIALIZE, 1, ValueChangedTranslationCheckInitialize, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_RESET_GROUP, 1, ValueChangedTranslationCheckResetGroup, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_REBOOT, 1, ValueChangedTranslationCheckReboot, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_REMOVE_ALL_EVENTS, 1, ValueChangedTranslationCheckRemoveAllEvents, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_PVT_TOTAL_TIME, 1, ValueChangedTranslationEditTotalTimePvt, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_TRACK_POSITION, 1, ValueChangedTranslationCheckTrackPosition, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_PVT_VELOCITY_AMPLITUDE, 1, ValueChangedTranslationEditPvtVelocityAmplitude, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_PVT_POSITON_AMPLITUDE, 1, ValueChangedTranslationEditPvtPositonAmplitude, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_TRACK_VELOCITY, 1, ValueChangedTranslationCheckTrackVelocity, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_RECORD_POSITION_VELOCITY, 1, ValueChangedTranslationCheckRecord_PositionVelocity, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_GATHERING_FREQ, 1, ValueChangedTranslationEditGatheringFreq, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_GATHERING_DURATION, 1, ValueChangedTranslationEditGatheringDuration, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_GATHERING_GET_FILE, 1, ValueChangedTranslationCheckGatheringGetFile, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_PVT_TRIGGER, 1, ValueChangedTranslationCheckPvtTrigger, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_AUTO_SAVE, 1, ValueChangedTranslationCheckAutoSave, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_TRACK_POSITION, DISPID_CLICK, ClickTranslationCheckTrackPosition, VTS_NONE)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_MONITOR_TRACKING_POSITIONS, 1, ValueChangedTranslationCheckMonitorTrackingPositions, VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_EDIT_DELAY_MONITOR_POS_VIT, 1, ValueChangedTranslationEditDelayMonitorPosVit, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogTranslationNewPort, IDC_TRANSLATION_CHECK_MONITOR_TRACKING_GET_FILE, 1, ValueChangedTranslationCheckMonitorTrackingGetFile, VTS_BOOL)
END_EVENTSINK_MAP()
void CDialogTranslationNewPort::ClickTranslationCheckConnected()
{
	if(m_XPS.IsConnected())
		m_XPS.XPSDisConnect();
	else
	{
		m_XPS.XPSConnect();
	}
	OnCbnSelchangeTranslationComboGroupName();
	ActualiseControls();
}
void CDialogTranslationNewPort::ClickTranslationCheckControlled()
{
}

void CDialogTranslationNewPort::MouseUpTranslationSlidePosition_1(short Button, short Shift, long x, long y)
{	m_XPS.SetPos(1, m_ValueSlidePos);
	m_NiEditPosition_1.SetValue(m_ValueSlidePos);
	ActualiseControls(true);}
void CDialogTranslationNewPort::PointerValueChangedTranslationSlidePosition_1(long Pointer, VARIANT* Value)
{	double val = Value->dblVal;
	m_ValueSlidePos = val;}
void CDialogTranslationNewPort::ValueChangedTranslationEditPosition_1(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{	double val = Value->dblVal;
	m_NiSlidePosition_1.SetValue(val);
	m_ValueSlidePos = val;
	m_XPS.SetPos(1, m_ValueSlidePos);
	ActualiseControls(true);}
void CDialogTranslationNewPort::MouseUpTranslationSlidePosition_2(short Button, short Shift, long x, long y)
{	m_XPS.SetPos(2, m_ValueSlidePos);
	m_NiEditPosition_2.SetValue(m_ValueSlidePos);
	ActualiseControls(true);}
void CDialogTranslationNewPort::PointerValueChangedTranslationSlidePosition_2(long Pointer, VARIANT* Value)
{	double val = Value->dblVal;
	m_ValueSlidePos = val;}
void CDialogTranslationNewPort::ValueChangedTranslationEditPosition_2(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{	double val = Value->dblVal;
	m_NiSlidePosition_2.SetValue(val);
	m_ValueSlidePos = val;
	m_XPS.SetPos(2, m_ValueSlidePos);
	ActualiseControls(true);}

void CDialogTranslationNewPort::DblClickTranslationGraphVitesse()
{
	m_XPS.m_WfVitesse[1].SetGraphically();
	m_XPS.PVTCalculatePositionToHaveJerk_0();
	if(m_AutoSave)
		ValueChangedTranslationCheckSave();
	else
	{
		m_XPS.PVTContentStringWrite();
		ActualiseGraphs();
		ActualiseControls(true);
		m_ComboPVT.SelectString(0, m_XPS.PVTFileGet());
	}
}

void CDialogTranslationNewPort::OnEnChangeTranslationEditPvtFilename()
{
	CString namesave;
	m_EditPVTFileName.GetWindowText(namesave);
	m_XPS.PVTFileNameForSaveSet(namesave);
}
void CDialogTranslationNewPort::OnEnChangeTranslationEditIP()
{
	CString strIP;
	m_EditIP.GetWindowText(strIP);
	m_XPS.SetNewIP(strIP);
}

void CDialogTranslationNewPort::OnEnChangeTranslationEditGroupName()
{
}
void CDialogTranslationNewPort::OnCbnSelchangeTranslationComboGroupName()
{
	if(!m_XPS.IsConnected())
		return;
	CString GroupName;
	m_ComboGroupName.GetLBText(m_ComboGroupName.GetCurSel(), GroupName);
	m_XPS.SetNewGroupName(GroupName);
	ActualiseGroupInformation();

	ActualiseControls();	
	ActualiseGraphs();
}
void CDialogTranslationNewPort::ActualiseGroupInformation()
{
	if(!m_XPS.IsConnected())
		return;

// pour la 1
	// si le min max travel n'est pas (0,0), on enable le slider
	CPointDbl minmaxTravel = m_XPS.GetPositionerLimits(1);
	bool enableSlider = minmaxTravel.x != minmaxTravel.y;
	m_NiSlidePosition_1.SetEnabled(enableSlider);
	m_NiEditPosition_1.SetEnabled(enableSlider);
	m_NiCheckControlled.SetValue(enableSlider);
	if(enableSlider)
	{
		// puis la Range du slide et du NiEdit
		m_NiEditPosition_1.RangeChecking = false;
		m_NiSlidePosition_1.SetValue(m_XPS.GetPos(1));
		m_NiSlidePosition_1.GetAxis().SetMinMax(minmaxTravel.x, minmaxTravel.y);
		ActualiseControls();
		m_NiEditPosition_1.SetMinMax(minmaxTravel.x, minmaxTravel.y);
		m_NiEditPosition_1.RangeChecking = true;
	}

// pour la 2
	// si le min max travel n'est pas (0,0), on enable le slider
	minmaxTravel = m_XPS.GetPositionerLimits(2);
	enableSlider = minmaxTravel.x != minmaxTravel.y;
	m_NiSlidePosition_2.SetEnabled(enableSlider);
	m_NiEditPosition_2.SetEnabled(enableSlider);
	m_NiCheckControlled.SetValue(enableSlider);
	if(enableSlider)
	{
		// puis la Range du slide et du NiEdit
		m_NiEditPosition_2.RangeChecking = false;
		m_NiSlidePosition_2.SetValue(m_XPS.GetPos(2));
		m_NiSlidePosition_2.GetAxis().SetMinMax(minmaxTravel.x, minmaxTravel.y);
		ActualiseControls();
		m_NiEditPosition_2.SetMinMax(minmaxTravel.x, minmaxTravel.y);
		m_NiEditPosition_2.RangeChecking = true;
	}
}
void CDialogTranslationNewPort::OnCbnSelchangeTranslationComboTrigger()
{
	int whichSelection = m_ComboTrig.GetCurSel();
	int whichTTL = 0;
	int whichGPIO = 0;

	switch(whichSelection)
	{
	case 1:	whichTTL = 1;	whichGPIO = 1;	break;
	case 2:	whichTTL = 2;	whichGPIO = 1;	break;
	case 3:	whichTTL = 3;	whichGPIO = 1;	break;
	case 4:	whichTTL = 4;	whichGPIO = 1;	break;
	case 5:	whichTTL = 5;	whichGPIO = 1;	break;
	case 6:	whichTTL = 6;	whichGPIO = 1;	break;
	case 7:	whichTTL = 7;	whichGPIO = 1;	break;
	case 8:	whichTTL = 8;	whichGPIO = 1;	break;
	}
	m_XPS.SetTriggerForTrajectoryViaScript(whichTTL, whichGPIO);
	ActualiseControls();
}

void CDialogTranslationNewPort::ValueChangedTranslationStart(BOOL Value)
{
	ActualiseControls(true);
	bool val = Value ? true : false;
	if(!val)
		return;
	m_XPS.PVTExecute();
	ActualiseControls(true);	
}

void CDialogTranslationNewPort::ValueChangedCwnumedit1(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_XPS.PVTNbrExecutionSet(int(Value->dblVal));
}

void CDialogTranslationNewPort::OnCbnSelchangeTranslationComboPvt()
{
	CString FileName;
	m_ComboPVT.GetLBText(m_ComboPVT.GetCurSel(), FileName);
	bool isGoodPVTFile = m_XPS.PVTFileSet(FileName);
	ActualiseControls(true);	
	ActualiseGraphs();
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckJerkNull(BOOL Value)
{
	if(Value)
		return;

	m_XPS.PVTCalculatePositionToHaveJerk_0();
	if(m_AutoSave)
		ValueChangedTranslationCheckSave();
	else
	{
		m_XPS.PVTContentStringWrite();
		ActualiseGraphs();
		ActualiseControls(true);
		m_ComboPVT.SelectString(0, m_XPS.PVTFileGet());
	}
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckSave(BOOL Value)
{	bool val = Value ? true : false;
	if(val)
		return;
	
	CString questionME;
	CString FileNameSave = m_XPS.PVTFileNameForSaveGet();
	// ajoutons le .trj si il n'est pas la
	if(FileNameSave.Find(".trj") != FileNameSave.GetLength() - 4)
		FileNameSave += ".trj";
	questionME.Format("Do you want to write on this file:\n\n%s      ?", FileNameSave);
	int whatdo = m_AutoSave ? IDOK : AfxMessageBox(questionME, MB_OKCANCEL);
	if(whatdo != IDOK)
		return;
	m_XPS.PVTContentStringWrite(true, FileNameSave);
	m_XPS.PVTFindAllFiles();

	ActualiseControls(true);
	ActualiseGraphs();
	m_ComboPVT.SelectString(0, m_XPS.PVTFileGet());
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckResetGroup(BOOL Value)
{
	if(Value)
		return;
	m_XPS.ResetGroup();
}
void CDialogTranslationNewPort::ValueChangedTranslationCheckInitialize(BOOL Value)
{
	if(Value)
		return;
	m_XPS.InitializeHome();	
	ActualiseControls(true);
	ActualiseGraphs();
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckReboot(BOOL Value)
{
	if(Value)
		return;
	m_XPS.RebootXPS();
	m_NiCheckConnected.SetValue(false);
	ClickTranslationCheckConnected();
}
void CDialogTranslationNewPort::ValueChangedTranslationCheckRemoveAllEvents(BOOL Value)
{
	if(Value)
		return;
	m_XPS.RemoveAllEventAndAction();
	ActualiseControls();
}
void CDialogTranslationNewPort::ValueChangedTranslationEditTotalTimePvt(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	double val = Value->dblVal;
	if(val < 1e-6)
		return;
	if(!m_XPS.PVT_ResizeToTotalTime(val))
		return;
	if(m_AutoSave)
		ValueChangedTranslationCheckSave();
	else
	{
		m_XPS.PVTContentStringWrite();
		ActualiseGraphs();
		ActualiseControls(true);
		m_ComboPVT.SelectString(0, m_XPS.PVTFileGet());
	}
}

void CDialogTranslationNewPort::ValueChangedTranslationEditPvtVelocityAmplitude(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	double val = Value->dblVal;
	if(abs(val) < 1e-6)
		return;
	if(!m_XPS.PVT_ResizeToVelocityAmplitude(1, val))
		return;
	if(m_AutoSave)
		ValueChangedTranslationCheckSave();
	else
	{
		m_XPS.PVTContentStringWrite();
		ActualiseGraphs();
		ActualiseControls(true);
		m_ComboPVT.SelectString(0, m_XPS.PVTFileGet());
	}
}

void CDialogTranslationNewPort::ValueChangedTranslationEditPvtPositonAmplitude(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	double val = Value->dblVal;
	if(abs(val) < 1e-6)
		return;
	if(!m_XPS.PVT_ResizeToPositonAmplitude(1, val))
		return;
	if(m_AutoSave)
		ValueChangedTranslationCheckSave();
	else
	{
		m_XPS.PVTContentStringWrite();
		ActualiseGraphs();
		ActualiseControls(true);
		m_ComboPVT.SelectString(0, m_XPS.PVTFileGet());
	}
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckTrackPosition(BOOL Value)
{
}
void CDialogTranslationNewPort::ClickTranslationCheckTrackPosition()
{
	bool Value = m_NiChekTrackPosition.Value;
	if(Value)
	{
		// debut de tracking sur la voie définie par la combo
		m_XPS.TrackingPosition(1, m_ComboAnalogIn.GetCurSel() + 1);
	}
	else
	{
		m_XPS.TrackingDisable();
	}
	ActualiseControls();
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckTrackVelocity(BOOL Value)
{
}


void CDialogTranslationNewPort::ValueChangedTranslationCheckRecord_PositionVelocity(BOOL Value)
{
	if(Value)
	{
		double AquireFreq = m_NiEditGatheringFreq.GetValue();
		double AquireDura = m_NiEditGatheringDuration.GetValue();
		m_XPS.Record_PositionVelocity(1, AquireFreq, AquireDura);
		// on renvoye les parametre vraiment utilisés
		m_NiEditGatheringFreq.SetValue(AquireFreq);
		m_NiEditGatheringDuration.SetValue(AquireDura);
	}
	else
	{
		m_XPS.Record_Trigger_Remove();
	}
	ActualiseControls();
}

void CDialogTranslationNewPort::ValueChangedTranslationEditGatheringFreq(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{}

void CDialogTranslationNewPort::ValueChangedTranslationEditGatheringDuration(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{}

void CDialogTranslationNewPort::ValueChangedTranslationCheckGatheringGetFile(BOOL Value)
{
	if(Value)
		return;
	CString LocalFolder = DEFAULT_LOCAL_FOLDER_RECORD_POSITION_VITESSE;
	CString filename;
	CString strNumber = "00";
	m_EditGatheringFileName.GetWindowText(filename);
	CString LocalFileName = LocalFolder + filename + CString("_") + strNumber + ".dat";
	// si le fichier existe deja,
	int number = 0;
	while(_access(LocalFileName, 00) != -1)
	{
		number++;
		strNumber.Format("%s%d", int(log10(double(number))) ? "" : "0", number);
		LocalFileName = LocalFolder + filename + CString("_") + strNumber + ".dat";
	}

	m_XPS.Record_SaveAndRapatrie(LocalFileName);
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckPvtTrigger(BOOL Value)
{
	if(Value)
		m_XPS.SetTriggerForTrajectoryViaScript(1, 1);
	else
		m_XPS.SetTriggerForTrajectoryViaScript(0, 0);
	ActualiseControls();
}

void CDialogTranslationNewPort::ValueChangedTranslationCheckAutoSave(BOOL Value)
{
	m_AutoSave = Value ? true : false;
	m_NiCheckSavePVT.SetEnabled(!m_AutoSave);
}



void CDialogTranslationNewPort::ValueChangedTranslationCheckMonitorTrackingPositions(BOOL Value)
{
	if(Value)
	{
		double AquireFreq = m_NiEditGatheringFreq.GetValue();
		double AquireDura = m_NiEditGatheringDuration.GetValue();
		m_XPS.Record_AnalogTrack1And2AndPosition1And2(1, AquireFreq, AquireDura);
		// on renvoye les parametre vraiment utilisés
		m_NiEditGatheringFreq.SetValue(AquireFreq);
		m_NiEditGatheringDuration.SetValue(AquireDura);

		// on lance le timer qui checkera le fichier GATHERED_DATA_FILE_NAME toute les xxx ms
		m_IdTimerCheckingGatheringResult = SetTimer(1, PeriodeMonitoringXPSPosVit_ms, 0);
	}
	else
	{
		m_XPS.Record_Trigger_Remove();
		// on enleve le timer
		KillTimer(m_IdTimerCheckingGatheringResult);
	}
	ActualiseControls();
}
void CDialogTranslationNewPort::OnTimer(UINT nIDEvent)
{
	return;

	double timestep = ValueChangedTranslationCheckMonitorTrackingGetFile(false);
	if(timestep == 0)
		return;
	ActualiseMonitoringGraph(timestep);

	CDialog::OnTimer(nIDEvent);
}

void CDialogTranslationNewPort::ActualiseMonitoringGraph(double timestep)
{
	if(MatrixForMonitoringXPSPosVit.GetCols() != 4)
		return;
	if(timestep != 0)
		m_timestepPosVit = timestep;
	CNiReal64Vector AnalogIn1;
	CNiReal64Vector AnalogIn2;
	CNiReal64Vector RealPos1 ;
	CNiReal64Vector RealPos2 ;
	MatrixForMonitoringXPSPosVit.CopyColumn(0, AnalogIn1);
	MatrixForMonitoringXPSPosVit.CopyColumn(1, AnalogIn2);
	MatrixForMonitoringXPSPosVit.CopyColumn(2, RealPos1 );
	MatrixForMonitoringXPSPosVit.CopyColumn(3, RealPos2 );

	// en mm
	AnalogIn1.Scale(10.0);
	AnalogIn2.Scale(10.0);

	CNiReal64Vector ErrorPos1;
	CNiReal64Vector ErrorPos2;
	// on peut retarder l'un des signaux afin de voir si on loqk bien, mais en retard
	double retard = m_NiEditDelayMonitorPosVit.Value;
	if(retard > 1e-9)
	{
		int nbrRowTrop = int(retard / m_timestepPosVit  + 0.000001 );
		double fracRow = retard / m_timestepPosVit - double(nbrRowTrop);
		// on interpole entre deux point
		double factFirstPoint = 1.0 - fracRow;
		double factSeconPoint = fracRow;

		int nRow = RealPos1.GetSize() - nbrRowTrop - 1;
		if(nRow < 1)
			return;
		ErrorPos1.SetSize(nRow);
		ErrorPos2.SetSize(nRow);
		// on met les valeurs dans ErrorPos
		for(int i = 0; i < nRow; i++)
		{
			ErrorPos1[i] = RealPos1[i + nbrRowTrop] - factSeconPoint * AnalogIn1[i] - factFirstPoint * AnalogIn1[i + 1];
			ErrorPos2[i] = RealPos2[i + nbrRowTrop] - factSeconPoint * AnalogIn2[i] - factFirstPoint * AnalogIn2[i + 1];
		}
	}
	else
	{
		ErrorPos1 = RealPos1 - AnalogIn1;
		ErrorPos2 = RealPos2 - AnalogIn2;
	}

	m_NiGraphVitesse.Plots.Item("ErrorPos").PlotY(ErrorPos1, 0, m_timestepPosVit);
	m_NiGraphVitesse.Plots.Item("AnalogIn").PlotY(AnalogIn1, 0, m_timestepPosVit);
	m_NiGraphVitesse.Plots.Item("RealPos").PlotY(RealPos1, 0, m_timestepPosVit);
	m_NiGraphVitesse.Axes.Item(1).AutoScaleNow();
	m_NiGraphVitesse.Axes.Item(2).AutoScaleNow();
	m_NiGraphVitesse.Axes.Item("ErrorPos").AutoScaleNow();
	m_NiGraphPosition.Plots.Item("ErrorPos").PlotY(ErrorPos2, 0, m_timestepPosVit);
	m_NiGraphPosition.Plots.Item("AnalogIn").PlotY(AnalogIn2, 0, m_timestepPosVit);
	m_NiGraphPosition.Plots.Item("RealPos").PlotY(RealPos2, 0, m_timestepPosVit);
	m_NiGraphPosition.Axes.Item(1).AutoScaleNow();
	m_NiGraphPosition.Axes.Item(2).AutoScaleNow();
	m_NiGraphPosition.Axes.Item("ErrorPos").AutoScaleNow();
}

void CDialogTranslationNewPort::ValueChangedTranslationEditDelayMonitorPosVit(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	if(m_NiGraphPosition.Axes.Item(3).Visible)
		ActualiseMonitoringGraph();
}

double CDialogTranslationNewPort::ValueChangedTranslationCheckMonitorTrackingGetFile(BOOL Value)
{
	if(Value)
		return 0;
	double step = m_XPS.Record_SaveAndRapatrie("c:\\MonitorXPSPosVit.txt", &MatrixForMonitoringXPSPosVit);
	if(step > 1e-9)
		ActualiseMonitoringGraph(step);
	return step;
}
