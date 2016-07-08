#pragma once
#include "resource.h"
#include "NewPortXPS.h"
#include "afxwin.h"

// Boîte de dialogue CDialogTranslationNewPort
#define m_XPS (*m_pXPS)

class CDialogTranslationNewPort : public CDialog
{
	DECLARE_DYNAMIC(CDialogTranslationNewPort)

public:
	CDialogTranslationNewPort(CWnd* pParent = NULL);   // constructeur standard
	CDialogTranslationNewPort(CNewPortXPS* pTranslationXPS, CWnd* pParent = NULL);
	virtual ~CDialogTranslationNewPort();

// Données de boîte de dialogue
	enum { IDD = IDD_DIALOG_TRANSLATION };

private:
	double m_ValueSlidePos;
	UINT_PTR m_IdTimerCheckingGatheringResult;
	CNiReal64Matrix MatrixForMonitoringXPSPosVit;
	double m_timestepPosVit;

public:
	bool m_UseTheXPSOfTheDialog;
	CNewPortXPS* m_pXPS;
	bool m_AutoSave;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void PointerValueChangedTranslationSlidePosition_1(long Pointer, VARIANT* Value);
	void MouseUpTranslationSlidePosition_1(short Button, short Shift, long x, long y);
	void PointerValueChangedTranslationSlidePosition_2(long Pointer, VARIANT* Value);
	void MouseUpTranslationSlidePosition_2(short Button, short Shift, long x, long y);
	void ValueChangedTranslationEditPosition_1(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedTranslationEditPosition_2(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	CEdit m_EditText, m_EditEvent, m_EditAction, m_EditGatheringFileName;
	CNiSlide m_NiSlidePosition_1, m_NiSlidePosition_2;
	CNiNumEdit m_NiEditPosition_1, m_NiEditPosition_2;
	CNiButton m_NiChekTrackPosition, m_NiChekTrackVelocity, m_NiCheckSavePVT;
	CNiButton m_NiCheckControlled, m_NiCheckControlled2, m_NiCheckMonitorTrackingPositions, m_NiCheckMonitorTrackingGetFile;
	void ClickTranslationCheckControlled();
	void ClickTranslationCheckConnected();
	CNiButton m_NiCheckConnected, m_NiCheckGatheringGetFile, m_NiCheckAutoSave;
	virtual BOOL OnInitDialog();
	void ActualiseGraphs();
	void ActualiseControls(bool checkPVT = false);
	CNiNumEdit m_NiEditPvtTotalTime, m_NiEditPvtVelocityAmplitude, m_NiEditPvtPositonAmplitude, m_NiEditGatheringFreq, m_NiEditGatheringDuration, m_NiEditDelayMonitorPosVit;
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	CNiGraph m_NiGraphVitesse, m_NiGraphPosition;
	void DblClickTranslationGraphVitesse();
	void ActualiseGroupInformation();
	CComboBox m_ComboTrig, m_ComboPVT, m_ComboGroupName, m_ComboPositionerName;
	CComboBox m_ComboAnalogIn;
	CEdit m_EditPVTFileName, m_EditIP, m_EditGroupName;
	afx_msg void OnEnChangeTranslationEditPvtFilename();
	afx_msg void OnEnChangeTranslationEditIP();
	afx_msg void OnEnChangeTranslationEditGroupName();
	afx_msg void OnCbnSelchangeTranslationComboTrigger();
	CNiButton m_NiCheckStart;
	void ValueChangedTranslationStart(BOOL Value);
	CNiNumEdit m_NiEditNbrExecution;
	void ValueChangedCwnumedit1(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	afx_msg void OnCbnSelchangeTranslationComboPvt();
	void ValueChangedTranslationCheckJerkNull(BOOL Value);
	void ValueChangedTranslationCheckSave(BOOL Value = FALSE);
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeTranslationComboGroupName();
	void ValueChangedTranslationCheckInitialize(BOOL Value);
	void ValueChangedTranslationCheckResetGroup(BOOL Value);
	void ValueChangedTranslationCheckReboot(BOOL Value);
	void ValueChangedTranslationCheckRemoveAllEvents(BOOL Value);
	void ValueChangedTranslationEditTotalTimePvt(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedTranslationCheckTrackPosition(BOOL Value);
	void ValueChangedTranslationCheckTrackVelocity(BOOL Value);
	void ValueChangedTranslationEditPvtVelocityAmplitude(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedTranslationEditPvtPositonAmplitude(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedTranslationCheckRecord_PositionVelocity(BOOL Value);
	void ValueChangedTranslationEditGatheringFreq(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedTranslationEditGatheringDuration(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedTranslationCheckGatheringGetFile(BOOL Value);
	void ValueChangedTranslationCheckPvtTrigger(BOOL Value);
	void ValueChangedTranslationCheckAutoSave(BOOL Value);
	void ClickTranslationCheckTrackPosition();
	void ActualiseMonitoringGraph(double timestep = 0);
	afx_msg void OnTimer(UINT nIDEvent);
	void ValueChangedTranslationCheckMonitorTrackingPositions(BOOL Value);
	void ValueChangedTranslationEditDelayMonitorPosVit(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	double ValueChangedTranslationCheckMonitorTrackingGetFile(BOOL Value);
};
