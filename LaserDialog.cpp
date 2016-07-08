// LaserDialog.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include ".\laserdialog.h"
// CLaserDialog

#define ACTUALISE_LASER_STATUS_MS 1000
#define SECURITY_TIME_S 40
#define HAS_JUST_CONNECTED 1000056

#define ORDER_FROM_LASER_STATUS_CHECKING 123123

IMPLEMENT_DYNAMIC(CLaserDialog, CProDialog)
CLaserDialog::CLaserDialog()
{	
	try 
	{
		port.Open(1, 57600, CSerialPort::NoParity, 8, 
			CSerialPort::OneStopBit, 
			CSerialPort::NoFlowControl);
	
		COMMTIMEOUTS timeouts;
		timeouts.ReadTotalTimeoutConstant = 100;
		timeouts.WriteTotalTimeoutConstant = 100;
		timeouts.WriteTotalTimeoutMultiplier = 10;
		timeouts.ReadTotalTimeoutMultiplier = 10;
		timeouts.ReadIntervalTimeout = 10;
		port.SetTimeouts(timeouts);
	}
	catch (CSerialException* pEx)
	{
		TRACE(_T("Handle Exception, Message:%s\n"), 
			pEx->GetErrorMessage());
		pEx->Delete();
	}
	m_nTimer = 0;
	m_Security_TimeAbove20 = 0;
	m_Busy_DontAskStatusInTimer = false;
}

CLaserDialog::~CLaserDialog()
{
	try 
	{
		port.Close();	
	}
	catch (CSerialException* pEx)
	{
		TRACE(_T("Handle Exception, Message:%s\n"), 
			pEx->GetErrorMessage());
		pEx->Delete();
	}
}


BEGIN_MESSAGE_MAP(CLaserDialog, CProDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CLaserDialog::OnInitDialogBar()
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
void CLaserDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_LASER_SLIDE_INTENSITY, m_NiSlideLaserIntensity);
	DDX_Control(pDX, IDC_LASER_SLIDE_WARMING, m_NiSlideLaserWarming);
	DDX_Control(pDX, IDC_LASER_EDIT_INTENSITY, m_NiEditLaserIntensity);
	DDX_Control(pDX, IDC_LASER_EDIT_POWER_MEASURED, m_NiEditLaserPowerMeasured);
	DDX_Control(pDX, IDC_LASER_EDIT_TEMP_MEASURED, m_NiEditLaserTempMeasured);
	DDX_Control(pDX, IDC_LASER_CHECK_EMISSION, m_NiCheckLaserEmission);
	DDX_Control(pDX, IDC_LASER_CONNECTION, m_NiCheckConnection);
	//}}AFX_DATA_MAP
}

// Gestionnaires de messages CLaserDialog


void CLaserDialog::OnPaint()
{
	CPaintDC dc(this);
	if(m_bWasPaintedOnce == false)
	{
		for(int i = 0; i <32; i++)
			BitStatus[i] = 0;
		m_bWasPaintedOnce = true;
	}
}
BEGIN_EVENTSINK_MAP(CLaserDialog, CProDialog)
//	ON_EVENT(CLaserDialog, IDC_INPUT_START_ACQ_0, 1, ValueChangedInputStartAcq0, VTS_BOOL)
ON_EVENT(CLaserDialog, IDC_LASER_CHECK_EMISSION, 1, ValueChangedLaserCheckEmission, VTS_BOOL)
ON_EVENT(CLaserDialog, IDC_LASER_CHECK_GUIDING, 1, ValueChangedLaserCheckGuiding, VTS_BOOL)
ON_EVENT(CLaserDialog, IDC_LASER_SLIDE_INTENSITY, DISPID_MOUSEUP, MouseUpLaserSlideIntensity, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
ON_EVENT(CLaserDialog, IDC_LASER_SLIDE_INTENSITY, 1, PointerValueChangedLaserSlideIntensity, VTS_I4 VTS_PVARIANT)
ON_EVENT(CLaserDialog, IDC_LASER_EDIT_INTENSITY, 1, ValueChangedLaserEditIntensity, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
ON_EVENT(CLaserDialog, IDC_LASER_CONNECTION, DISPID_CLICK, ClickLaserConnection, VTS_NONE)
END_EVENTSINK_MAP()


CString CLaserDialog::WriteRS232(CString TheString, int Pane)
{	
	if(!port.IsOpen())
	{
		MessageBox("The Serial Port 1 is Not Opened ! ! ");
		return "Not Open";
	}
	if(m_nTimer)
	{
		KillTimer(m_nTimer);
		m_nTimer = 0;
	}
	port.Flush();
	port.ClearWriteBuffer();
	port.ClearReadBuffer();
	// on convertit tout en UpperCase
	TheString.MakeUpper();
	// sécurité : 
	// quand le laser se trouve sous 20% , ou eteint,	ALORS m_Security_TimeAbove20 = 0
	// quand le laser est au dela de 20% est allumé,	ALORS m_Security_TimeAbove20++ tout les ID_CLOCK_TIMER_LASER_DIALOG
	// quand m_Security_TimeAbove20 a une valeur >= double(SECURITY_TIME_S) / (double(ACTUALISE_LASER_STATUS_MS)/double(1000))
	//													ALORS, on peut passer au dessus de 40%
	if(TheString.Find("SDC") != -1)
	{
		// extrayons la valeur du setpoint demandé
		double LaValSetPoint = 0;
		CString LaStrSetPoint = TheString; char* unchar;
		int posChiffre = TheString.FindOneOf("0123456789");
		if(posChiffre != -1)
			LaStrSetPoint.Delete(0, posChiffre);
		LaValSetPoint= strtod(LaStrSetPoint, &unchar);
		// on actualise l'editBox
		m_NiEditLaserIntensity.SetValue(LaValSetPoint);
		// si c'est trop, c'est trop tot, et bien il faut appliquer des sanctions : 
		// et on remplace la valeur donner au port serie
		if(m_Security_TimeAbove20 < double(SECURITY_TIME_S) / (double(ACTUALISE_LASER_STATUS_MS)/double(1000)) 
			&& LaValSetPoint > 40.0)
		{
			TheString.Delete(posChiffre, 1000);
			TheString += "40.0";
			m_NiSlideLaserIntensity.SetValue(40.0);
			LaValSetPoint = 40.0;
		}
	}
	// sécurité : si on start l'émmission
	if(TheString.Find("EMON") != -1)
	{
		// quel est le setpoint
		CString TheSetPointNOW = WriteRS232("RCS");
		// On renvoye tous simplement le setpont courant dans WriteRS232(...), qui va passer dans la boucle de securité ci-dessus
		TheSetPointNOW.MakeUpper();
		TheSetPointNOW.Replace("RCS", "SDC");
		WriteRS232(TheSetPointNOW);
	}

	char sBuf[16];
	int NumSent = -1;
	sprintf(sBuf, TheString + "\r");
	CString Chaine = "";
	try 
	{
		DWORD stringLength = DWORD(strlen(sBuf));
		NumSent = port.Write(sBuf, stringLength);
		Sleep(50);
		int stopcond = 20;
		char temp1char = '\r';
		do
		{
			port.Read(&temp1char, 1);
			Chaine += temp1char;
			stopcond--;
		} while (temp1char != '\r' && stopcond);
	}
	catch(CSerialException* pEx)
	{
		TRACE(_T("Handle Exception, Message:%s\n"), 
			pEx->GetErrorMessage());
		pEx->Delete();
	}
	// si on recoit une réponse du laser, alors, on allume le timer 
	// qui prend le statu du laser, et qui gere la sécurité : "SECURITY_TIME_S"
	if(!m_nTimer && Chaine != "\r")
	{
		m_nTimer = SetTimer(ID_CLOCK_TIMER_LASER_DIALOG, ACTUALISE_LASER_STATUS_MS, NULL);
		m_NiCheckConnection.SetValue(true);
	}
	if(Chaine == "\r")
	{
		if(m_nTimer)
		{
			KillTimer(m_nTimer);
			m_nTimer = 0;
		}
		m_nTimer = 0;
		if(m_NiCheckConnection.GetValue())
			MessageBox("Pas de réponse du laser!");
		m_NiCheckConnection.SetValue(false);
	}

	Chaine.Remove('\r');

	if(Pane)
		m_pMainFrame->m_wndStatusBar.SetPaneText(Pane, Chaine);

	port.Flush();
	port.ClearWriteBuffer();
	port.ClearReadBuffer();

	return Chaine;
}

void CLaserDialog::ValueChangedLaserCheckEmission(BOOL Value)
{
	// si le message vient du program, pour actualiser le bouton, juste pour info, 
	// on va peut etre quand meme pas allumer le laser, on ne sait jamais,...
	if(Value == ORDER_FROM_LASER_STATUS_CHECKING)
		return;

	bool val = Value?true:false;
	if(val)
		WriteRS232("EMON");
	else
		WriteRS232("EMOFF");
	// voyons voir le status : laser starting
	OnTimer(ID_CLOCK_TIMER_LASER_DIALOG);
	if(val && BitStatus[15]) // laser starting
		return;
	else if(val)
	{
//		CheckDlgButton(	IDC_LASER_CHECK_EMISSION, false);
//		MessageBox("Check the safety requirements :\r\n\r\n-Water cooling = 5L/min\r\n\r\n-Emergency buttons disarmed\r\n\r\n-Door closed", "Laser emission is not starting !");
//		OnTimer(ID_CLOCK_TIMER_LASER_DIALOG);
	}
}

void CLaserDialog::ValueChangedLaserCheckGuiding(BOOL Value)
{
	bool val = Value?true:false;

	if(val)
		WriteRS232("ABN");
	else
		WriteRS232("ABF");
}
void CLaserDialog::PointerValueChangedLaserSlideIntensity(long Pointer, VARIANT* Value)
{
	m_ValueSlidePercent = Value->dblVal;
	m_NiEditLaserIntensity.SetValue(m_ValueSlidePercent);
}
void CLaserDialog::MouseUpLaserSlideIntensity(short Button, short Shift, long x, long y)
{
	CString ValueToSet;
	int entier = int(m_ValueSlidePercent);
	int decimal = int(10*m_ValueSlidePercent-10*entier);
	ValueToSet.Format(_T("SDC %d.%d"), entier, decimal);
	CString Reply;
	Reply = WriteRS232(ValueToSet);

	// si on est out of range, on dit erreur, et puis on fixe le courant à une valeur acceptable
	if(Reply.Find("ERR") != -1)
	{
		
		Reply = WriteRS232("RNC");
		CString MiniVal = Reply;
		// MiniVal enleve tout les caractères de Reply avant les valeurs numériques
		MiniVal.Delete(0, MiniVal.FindOneOf("0123456789"));
		MessageBox(CString("Value will be set at : ") + MiniVal + " %", Reply);
		char* unchar;
		m_NiSlideLaserIntensity.SetValue(strtod(MiniVal, &unchar));
		// puis on relance la fonction avec des 0 en param pour éviter d'etre bloqué dans cette fonction
		if(Button ==0 && Shift == 0 && x == 0 && y == 0)
			return;
		else
			MouseUpLaserSlideIntensity(0,0,0,0);
	}
}

void CLaserDialog::ValueChangedLaserEditIntensity(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	double val = Value->dblVal;
	// si on est pas en train de bouger le curseur, on l'actualise
	if(!(GetFocus() == GetDlgItem(IDC_LASER_SLIDE_INTENSITY)))
	{
		m_NiSlideLaserIntensity.SetValue(val);
		MouseUpLaserSlideIntensity(1,1,1,1);
	}
}

void CLaserDialog::OnTimer(UINT nIDEvent)
{
//	if(!m_pMainFrame->m_LaserDialog.IsWindowVisible())
//		return;
	if(nIDEvent == ID_CLOCK_TIMER_LASER_DIALOG && m_nTimer)
	{
		int PumpSetPoint = Status();
		int OutputPower	 = MeasuredPower();
		MeasuredTemp();

		// sécurité : 
		// quand le laser se trouve sous 20% , ou eteint,	ALORS m_Security_TimeAbove20 = 0
		// quand le laser est au dela de 20% est allumé,	ALORS m_Security_TimeAbove20++ tout les ID_CLOCK_TIMER_LASER_DIALOG
		// quand m_Security_TimeAbove20 a une valeur >= double(SECURITY_TIME_S) / (double(ACTUALISE_LASER_STATUS_MS)/double(1000))
		//													ALORS, on peut passer au dessus de 40%
		if(PumpSetPoint < 20 || !OutputPower || !m_nTimer)
			m_Security_TimeAbove20 = 0;
		else
			m_Security_TimeAbove20++;

		double WarmingSur100 = 100 * m_Security_TimeAbove20 
								/ (double(SECURITY_TIME_S) / (double(ACTUALISE_LASER_STATUS_MS)/double(1000)));
		m_NiSlideLaserWarming.SetValue(min(100.0, WarmingSur100));
	}		
}
int CLaserDialog::Status()
{
	// Lisons le Status
	CString Reply = WriteRS232("STA", 7);
	// LaStrStat enleve tout les caractères de Reply avant les valeurs numériques
	char* unchar;
	CString LaStrStat = Reply;
	LaStrStat.Delete(0, LaStrStat.FindOneOf("0123456789"));
	int LaValStat = strtol(LaStrStat, &unchar, 10);
	// on extrait chaque bit grace à :
	for(int i = 0; i < 32; i++)
		BitStatus[i] = GetIemeBitFromInt32Bit(i, LaValStat);
	// on actualise les controles
	CheckDlgButton(	IDC_LASER_STATUS_01,	BitStatus[1	]	);		
	CheckDlgButton(	IDC_LASER_STATUS_02,	BitStatus[2	]	);		
	CheckDlgButton(	IDC_LASER_STATUS_04,	BitStatus[4	]	);		
	CheckDlgButton(	IDC_LASER_STATUS_05,	BitStatus[5	]	);		
	CheckDlgButton(	IDC_LASER_STATUS_06,	BitStatus[6	]	);		
	CheckDlgButton(	IDC_LASER_STATUS_07,	BitStatus[7	]	);		
	CheckDlgButton(	IDC_LASER_STATUS_08,	BitStatus[8	]	);		
	CheckDlgButton(	IDC_LASER_STATUS_11	,	BitStatus[11]	);		
	CheckDlgButton(	IDC_LASER_STATUS_12	,	BitStatus[12]	);		
	CheckDlgButton(	IDC_LASER_STATUS_15	,	BitStatus[15]	);		
	CheckDlgButton(	IDC_LASER_STATUS_22	,	BitStatus[22]	);		
	CheckDlgButton(	IDC_LASER_STATUS_30	,	BitStatus[30]	);
	
	// si emission pas starting
	if(BitStatus[2] && !BitStatus[15])
	{
		CheckDlgButton(	IDC_LASER_CHECK_EMISSION,	ORDER_FROM_LASER_STATUS_CHECKING	);		
		m_NiCheckLaserEmission.SetOnText("Emission !");
		m_NiCheckLaserEmission.SetOnColor(CNiColor(255,255,0));
	}
	// si starting
	else if(BitStatus[15])
	{
		CheckDlgButton(	IDC_LASER_CHECK_EMISSION,	ORDER_FROM_LASER_STATUS_CHECKING	);		
		m_NiCheckLaserEmission.SetOnText("STARTING...");
		m_NiCheckLaserEmission.SetOnColor(CNiColor(128,128,128));
	}
	// si ni émission ni starting
	else if(!BitStatus[2])
	{
		CheckDlgButton(	IDC_LASER_CHECK_EMISSION,	BitStatus[2	]	);		
	}

	// Lisons le Set Point Courrant
	Reply = WriteRS232("RCS", 10);
	// Enleve tout les caractères de Reply avant les valeurs numériques, 
	CString LaStrSetPoint = Reply;
	int posChiffre = LaStrSetPoint.FindOneOf("0123456789");
	if(posChiffre == -1)
		return 0;
	else
		LaStrSetPoint.Delete(0, posChiffre);
	double LaValSetPoint = strtod(LaStrSetPoint, &unchar);
	m_NiEditLaserIntensity.SetValue(LaValSetPoint);

	return int(LaValSetPoint);
}
int CLaserDialog::MeasuredPower()
{
	// Lisons la puissance mesurée par le laser
	CString Reply = WriteRS232("ROP", 8);
	char* unchar;
	// Enleve tout les caractères de Reply avant les valeurs numériques, 
	CString LaStrPower = Reply;
	int posChiffre = LaStrPower.FindOneOf("0123456789");
	if(posChiffre == -1)
		LaStrPower = "0";
	else
		LaStrPower.Delete(0, posChiffre);

	int LaValPower;
	LaValPower = strtol(LaStrPower, &unchar, 10);
	m_NiEditLaserPowerMeasured.SetValue(LaValPower);
	return LaValPower;
}
double CLaserDialog::MeasuredTemp()
{
	// Lisons la température mesurée par le laser
	CString Reply = WriteRS232("RCT", 9);
	// Enleve tout les caractères de Reply avant les valeurs numériques, 
	CString LaStrTemp = Reply;
	char* unchar;
	LaStrTemp.Delete(0, LaStrTemp.FindOneOf("0123456789"));
	double LaValTemp;
	if(LaStrTemp == "" || LaStrTemp.Find("OFF") != -1 )
		LaValTemp = 0.0;
	else
		LaValTemp = strtod(LaStrTemp, &unchar);
	m_NiEditLaserTempMeasured.SetValue(LaValTemp);
	return LaValTemp;
}


void CLaserDialog::ClickLaserConnection()
{
	if(m_NiCheckConnection.GetValue())
		return;
	// si on vien juste de se connecter, on enleve la protection histoire de na pas couper le laser s'il était déjà à 100%
	m_Security_TimeAbove20 = HAS_JUST_CONNECTED;
	Status();
}
