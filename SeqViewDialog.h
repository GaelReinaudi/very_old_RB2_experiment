#pragma once
#include "resource.h"

// ColonneListCtrl

class ColonneListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(ColonneListCtrl)

public:
	ColonneListCtrl();
	virtual ~ColonneListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

// Boîte de dialogue CSeqViewLeftDialog
class CSeqViewLeftDialog : public CProDialog
{
	DECLARE_DYNAMIC(CSeqViewLeftDialog)

public:
	CSeqViewLeftDialog();   // constructeur standard
	virtual ~CSeqViewLeftDialog();

	DECLARE_MESSAGE_MAP()
/////////////////////////// CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL()
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
protected:
	virtual BOOL OnInitDialogBar();
/////////////////////////// End CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
protected:
	CListCtrl m_ListLigneDig;
	CListCtrl m_ListLigneAna;

friend class CSeqViewDialog;

};

#include "ProDialog.h"
#include "stdafx.h"
#include "Sequence.h"

#define NBR_SEQUENCE_VIEW 1
// CSeqViewDialog
class CSeqViewDialog : public CProDialog
{
	DECLARE_DYNAMIC(CSeqViewDialog)

public:
	CSeqViewDialog();
	virtual ~CSeqViewDialog();
	CSeqViewLeftDialog m_SeqViewLeftDialog;		

protected:
	DECLARE_MESSAGE_MAP()

/////////////////////////// CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL()
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
protected:
	virtual BOOL OnInitDialogBar();
/////////////////////////// End CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
protected:
	Sequence m_Seq[NBR_SEQUENCE_VIEW + 1];
	// le numero de la séquence courante
	char m_CurentSeqIndex;


	CComboBox m_Combo_Files;
	ColonneListCtrl m_ListCol;
	CNiButton m_NiButtonStart;
	CNiButton m_NiButtonWaitTrigger;
	int m_WaitTrigger;
public:
	afx_msg void OnPaint();
	// WhichView = 1, 2, 4=CtrlList, ...
	void MousseRightDown(int WhichView, UINT nFlags, CPoint point);
	// WhichView = 1, 2, 4=CtrlList, ...
	void MousseLeftDown(int WhichView, UINT nFlags, CPoint point);
	void MousseMoved(int WhichView, UINT nFlags, CPoint point);

	DECLARE_EVENTSINK_MAP()
public:
	void ValueChangedSeqviewStart(BOOL Value);
	void ValueChangedSeqviewCheckWaittrigger(BOOL Value);


	CDAQmxDynamicCardAnalog1* p_DAQmxSeqAna1;
	CDAQmxTaskDynamicCardAnalog1* p_MasterTask;
	CNiReal64Matrix m_NiBufferAna1;

	CDAQmxDynamicCardDigital32* p_DAQmxSeqDig;
	CDAQmxTaskDynamicCardDigital32* p_Slave1Task;
	CNiUInt32Matrix m_NiBufferDig;

	CDAQmxDynamicCardAnalog2* p_DAQmxSeqAna2;
	CDAQmxTaskDynamicCardAnalog2* p_Slave2Task;
	CNiReal64Matrix m_NiBufferAna2;

	int PrepareNiCards();
	int PrepareSeqAndNiBuffer(Sequence& LaSeq);
	int FreeNiCards();
	int PrepareTriggerStuffs(Sequence& LaSeq);
friend class Sequence;
friend class CSeqViewLeftDialog;
friend class ColonneListCtrl;
afx_msg void OnCbnSelchangeSeqviewComboFiles();
virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
//afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


