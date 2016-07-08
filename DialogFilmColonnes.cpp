// DialogParam.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "Parametre.h"
#include ".\dialogfilmcolonnes.h"


// Boîte de dialogue CDialogFilmColonnes

IMPLEMENT_DYNAMIC(CDialogFilmColonnes, CDialog)
CDialogFilmColonnes::CDialogFilmColonnes(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFilmColonnes::IDD, pParent)
{
	m_bWasPaintedOnce = false;
}

CDialogFilmColonnes::CDialogFilmColonnes(Sequence & UneSeq, CString PartNameFromCol, CString PartNameToCol, double Step_ms, double Duree_ms, double StartAfterDebutCol_ms)
	: CDialog(CDialogFilmColonnes::IDD, NULL)
{
	m_bWasPaintedOnce = false;
	m_pSeq = &UneSeq;
	m_PartNameFromCol = PartNameFromCol;
	m_PartNameToCol = PartNameToCol;
	m_Step_ms = Step_ms;
	m_Duree_ms = Duree_ms;
}

CDialogFilmColonnes::~CDialogFilmColonnes()
{
}

void CDialogFilmColonnes::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_PARAM_COMBO_PARAM, m_ComboParam);
	DDX_Control(pDX, IDC_FILM_LIST_COL_FROM , m_ListColFromTo );
	DDX_Control(pDX, IDC_FILM_EDIT_STEP     , m_EditStep_ms     );
	DDX_Control(pDX, IDC_FILM_EDIT_DUREE    , m_EditDuree_ms    );
}

BEGIN_MESSAGE_MAP(CDialogFilmColonnes, CDialog)
	ON_WM_PAINT()
//	ON_LBN_SELCHANGE(IDC_PARAM_LIST_PARAM, OnLbnSelchangeParamListParam)
ON_LBN_SELCHANGE(IDC_FILM_LIST_COL_FROM, OnLbnSelchangeFilmListColFrom)
END_MESSAGE_MAP()

void CDialogFilmColonnes::OnPaint()
{
	if(!m_bWasPaintedOnce)
	{
		int nCol = m_pSeq->GetnColonnes();
		m_bWasPaintedOnce = true;
		// on remplit la liste des colonnes
		for(int iemeCol = 1; iemeCol <= nCol; iemeCol++)
		{
			m_ListColFromTo.AddString(m_pSeq->GetColonne(iemeCol)->m_NameCol);
		}
		m_EditStep_ms.SetValue(m_Step_ms);
		m_EditDuree_ms.SetValue(m_Duree_ms);
		// on choisit celles entre "qui porte un nom contenant m_PartNameFromCol" incluse
		// et "qui porte un nom contenant m_PartNameToCol" non incluse
		int ligneFrom = m_ListColFromTo.FindString(0, m_PartNameFromCol);
		int ligneTo = m_PartNameToCol == "" ? nCol : m_ListColFromTo.FindString(0, m_PartNameToCol);
		if(ligneTo == -1)
			ligneTo = nCol;
		ligneTo--;
		if(ligneTo <= ligneFrom)
			m_ListColFromTo.SetSel(ligneFrom);
		else
			m_ListColFromTo.SelItemRange(TRUE, ligneFrom, ligneTo);
		// on dit que la selection a changé pour actualiser les string
		OnLbnSelchangeFilmListColFrom();
	}
}

void CDialogFilmColonnes::OnLbnSelchangeFilmListColFrom()
{
	int firstSel = -1;
	int lastSel = -1;
	int nCol = m_pSeq->GetnColonnes();
	// trouvons la première colonne selectionnée, et la dernière selectionnée
	for(int index = 0; index < nCol; index++)
	{
		if(m_ListColFromTo.GetSel(index) && firstSel == -1)
			firstSel = index;
		if(m_ListColFromTo.GetSel(index))
			lastSel = index;
	}
	// on update les string m_PartNameFromCol
	m_ListColFromTo.GetText(firstSel   , m_PartNameFromCol);
	// et m_PartNameFromCol, si on est à la toute fin, on enregistre ""
	if(lastSel >= nCol - 1)
		m_PartNameToCol = "";
	else
		m_ListColFromTo.GetText(lastSel + 1, m_PartNameToCol);

	// actualisons la duree maximale du film
	m_Duree_ms = 0;
	for(int iemeCol = firstSel + 1; iemeCol <= lastSel + 1; iemeCol++)
	{
		m_Duree_ms += m_pSeq->GetColonne(iemeCol)->GetDuree() / MILLI_SECONDE(1);
	}
	m_EditDuree_ms.SetValue(m_Duree_ms);
	m_EditDuree_ms.RedrawWindow();
}
BEGIN_EVENTSINK_MAP(CDialogFilmColonnes, CDialog)
	ON_EVENT(CDialogFilmColonnes, IDC_FILM_EDIT_STEP, 1, ValueChangedFilmEditStep, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogFilmColonnes, IDC_FILM_EDIT_DUREE, 1, ValueChangedFilmEditDuree, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
END_EVENTSINK_MAP()

void CDialogFilmColonnes::ValueChangedFilmEditStep(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_Step_ms = Value->dblVal;
}

void CDialogFilmColonnes::ValueChangedFilmEditDuree(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_Duree_ms = Value->dblVal;
}
