#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include ".\Sequence.h"


// Boîte de dialogue CDialogFilmColonnes

class CDialogFilmColonnes : public CDialog
{
	DECLARE_DYNAMIC(CDialogFilmColonnes)

public:
	CDialogFilmColonnes(CWnd* pParent = NULL);   // constructeur standard
	CDialogFilmColonnes(Sequence & UneSeq, CString PartNameFromCol, CString PartNameToCol, double Step_ms, double Duree_ms, double StartAfterDebutCol_ms = 0);
	virtual ~CDialogFilmColonnes();

// Données de boîte de dialogue
	enum { IDD = IDD_DIALOG_FILM_COLONNES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	bool m_bWasPaintedOnce;
	Sequence* m_pSeq;
public:
	CString m_PartNameFromCol, m_PartNameToCol;
	double m_Step_ms, m_Duree_ms;

public:
	CListBox m_ListColFromTo;
	CNiNumEdit m_EditStep_ms, m_EditDuree_ms;
	afx_msg void OnPaint();
	afx_msg void OnLbnSelchangeFilmListColFrom();
	DECLARE_EVENTSINK_MAP()
	void ValueChangedFilmEditStep(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedFilmEditDuree(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
};
