#pragma once

#include "ProDialog.h"
#include "Parametre.h"
#include "DAQmxTasksAndCodes.h"
#include "stdafx.h"
#include "serialport.h"

// CLaserDialog

class CLaserDialog : public CProDialog
{
	DECLARE_DYNAMIC(CLaserDialog)

public:
	CLaserDialog();
	virtual ~CLaserDialog();

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

public:
	afx_msg void OnPaint();

public:
	DECLARE_EVENTSINK_MAP()

	void ValueChangedLaserCheckEmission(BOOL Value);
	CString WriteRS232(CString TheString, int Pane = 0);
private:
	int m_nTimer;
	CSerialPort port;
	bool m_Busy_DontAskStatusInTimer;
	double m_ValueSlidePercent;
	CNiSlide m_NiSlideLaserIntensity;
	CNiSlide m_NiSlideLaserWarming;
	CNiNumEdit m_NiEditLaserIntensity;
	CNiNumEdit m_NiEditLaserPowerMeasured;
	CNiNumEdit m_NiEditLaserTempMeasured;
	CNiButton m_NiCheckLaserEmission, m_NiCheckConnection;
	bool BitStatus[32];
		// sécurité : 
		// quand le laser se trouve sous 20% , ou eteint,	ALORS m_Security_TimeAbove20 = 0
		// quand le laser est au dela de 20% est allumé,	ALORS m_Security_TimeAbove20++ tout les ID_CLOCK_TIMER_LASER_DIALOG
		// quand m_Security_TimeAbove20 a une valeur >= double(SECURITY_TIME_S) / (double(ACTUALISE_LASER_STATUS_MS)/double(1000))
		//													ALORS, on peut passer au dessus de 40%
	// PS: le controle de securité s'effectuera directement dans WriteRS232("SDC....
	int m_Security_TimeAbove20;

public:
	void ValueChangedLaserCheckGuiding(BOOL Value);
	int CLaserDialog::Status();
	int CLaserDialog::MeasuredPower();
	double CLaserDialog::MeasuredTemp();

	void MouseUpLaserSlideIntensity(short Button, short Shift, long x, long y);
	void PointerValueChangedLaserSlideIntensity(long Pointer, VARIANT* Value);
	void ValueChangedLaserEditIntensity(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	afx_msg void OnTimer(UINT nIDEvent);
	void ClickLaserConnection();
};


