#pragma once

#include "ProDialog.h"
#include "Parametre.h"
#include "DAQmxTasksAndCodes.h"
#include "stdafx.h"
// CInputDialog

class CInputDialog : public CProDialog
{
	DECLARE_DYNAMIC(CInputDialog)

public:
	CInputDialog();
	virtual ~CInputDialog();

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

	CNiGraph	m_graph1;

public:
	afx_msg void OnPaint();

public:
	CDAQmxDynamicCardAnalogInput* p_AnalogInput;

protected:

public:
	DECLARE_EVENTSINK_MAP()
	void ValueChangedInputStartAcq0(BOOL Value);

	// chope le voltage d'une vois, et renvoye la moyenne
	double GetInputSeq(long IndexAnaIn, double Rate, long NumberSamples, CNiReal64Matrix& DataInput);
	// triggé, chope le voltage d'une vois, et renvoye la moyenne
	double GetInputSeqTrigged(long IndexAnaIn, double Rate, long NumberSamples, CNiReal64Matrix& DataInput);

};


