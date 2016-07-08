// InputDialog.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include "InputDialog.h"
#include ".\inputdialog.h"
// CInputDialog

IMPLEMENT_DYNAMIC(CInputDialog, CProDialog)
CInputDialog::CInputDialog()
{
}

CInputDialog::~CInputDialog()
{
}


BEGIN_MESSAGE_MAP(CInputDialog, CProDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CInputDialog::OnInitDialogBar()
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
void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_INPUT_GRAPH_01, m_graph1);
	//}}AFX_DATA_MAP
}

// Gestionnaires de messages CInputDialog


void CInputDialog::OnPaint()
{
	CPaintDC dc(this);
	if(m_bWasPaintedOnce == false)
	{
		m_bWasPaintedOnce = true;
	}
}
BEGIN_EVENTSINK_MAP(CInputDialog, CProDialog)
	ON_EVENT(CInputDialog, IDC_INPUT_START_ACQ_0, 1, ValueChangedInputStartAcq0, VTS_BOOL)
END_EVENTSINK_MAP()

void CInputDialog::ValueChangedInputStartAcq0(BOOL Value)
{ bool val = Value? true : false;
	if(val)
	{
		CNiReal64Matrix DataInput;
		GetInputSeqTrigged(0, 100, 100, DataInput);
	}
	else
		if(p_AnalogInput)
			delete p_AnalogInput;
}

double CInputDialog::GetInputSeq(long IndexAnaIn, double Rate, long NumberSamples, CNiReal64Matrix& DataInput)
{
	p_AnalogInput = new CDAQmxDynamicCardAnalogInput();
	CDAQmxTaskDynamicCardAnalogInput &Task = p_AnalogInput->GetTask();
	Task.Configure(IndexAnaIn, -10, 10);
	Task.Timing.ConfigureSampleClock("", Rate, DAQmxSampleClockActiveEdgeRising
									, DAQmxSampleQuantityModeFiniteSamples, NumberSamples);
	Task.Control(DAQmxTaskVerify);
	// chope les données
	p_AnalogInput->GetData(DataInput);

	// calcul de la moyenne
	unsigned int row, col;
	DataInput.GetSize(row, col);
	double Somme = DataInput.Sum();

	// Plot data
	m_graph1.PlotY(DataInput);

	return (Somme / (row * col));
}

double CInputDialog::GetInputSeqTrigged(long IndexAnaIn, double Rate, long NumberSamples, CNiReal64Matrix& DataInput)
{
	p_AnalogInput = new CDAQmxDynamicCardAnalogInput();
	CDAQmxTaskDynamicCardAnalogInput &Task = p_AnalogInput->GetTask();
	Task.Configure(IndexAnaIn, -10, 10);
	Task.Timing.ConfigureSampleClock("", Rate, DAQmxSampleClockActiveEdgeRising
									, DAQmxSampleQuantityModeFiniteSamples, NumberSamples);
	// Défini le Trigger pour commencer l'aquisition
	Task.Triggers.StartTrigger.ConfigureDigitalEdgeTrigger("PFI0", DAQmxDigitalEdgeStartTriggerEdgeRising);

	Task.Control(DAQmxTaskVerify);

	// chope les données
	p_AnalogInput->GetData(DataInput);

	// calcul de la moyenne
	unsigned int row, col;
	DataInput.GetSize(row, col);
	double Somme = DataInput.Sum();

	// Plot data
	m_graph1.PlotY(DataInput);

	return (Somme / (row * col));
}







