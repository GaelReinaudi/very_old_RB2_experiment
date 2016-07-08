// DialogVariation.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DialogVariation.h"

// Boîte de dialogue CDialogVariation

IMPLEMENT_DYNAMIC(CDialogVariation, CDialog)
CDialogVariation::CDialogVariation(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogVariation::IDD, pParent)
{
}

//CDialogVariation::CDialogVariation(CVariation* pLaVar, CString NomVar, int* pVal, int ValDebut, int ValFin, int LePas, CWnd* pParent /*=NULL*/)
//	: CDialog(CDialogVariation::IDD, pParent)
//{
//	m_pLaVar = pLaVar;
//	m_pLeParam		=	NULL		;
//	m_NomParametre	=	NomVar		;
//	m_pValeur		=	pVal		;
//	m_VarEditFrom	=	ValDebut	;
//	m_VarEditTo		=	ValFin		;
//	m_VarEditStep	=	LePas		;
//	m_Var_Varie		=	0			;
//}

CDialogVariation::CDialogVariation(CVariation* pLaVar, CParametre* pLeParam, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogVariation::IDD, pParent)
{
	m_pLaVar = pLaVar;
//	if(pLeParam->m_Type != Entier)
//	{
//		MessageBox("Not An Integer Value ! ! !");
//		return;
//	}
	m_pLeParam		=	pLeParam			;
//	m_NomParametre	=	pLeParam->m_NomTexte;
//	m_pValeur		=	pLeParam->Get_pVal();

	m_Var_Varie		=	pLeParam->Varie		;

	double Val = m_pLeParam->Get_Val();

	m_VarEditFrom	=	Val;

	if(pLeParam->m_From == -1 && pLeParam->m_To == -1 && pLeParam->m_Step == -1)
	{
		m_VarEditTo		=	Val ? Val * 5 : 5;
		m_VarEditStep	=	Val ? Val : 1;
	}
	else
	{
		m_VarEditFrom	=	pLeParam->m_From;
		m_VarEditTo		=	pLeParam->m_To	;
		m_VarEditStep	=	pLeParam->m_Step;
	}
}

CDialogVariation::~CDialogVariation()
{
}

BOOL CDialogVariation::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString SetAsVariant("Set This Parameter As a Variable : \n\n\t\t");
	SetAsVariant.Append(m_pLeParam->m_NomTexte);
	SetDlgItemText(	IDC_VAR_STATC_TEXTE			,	SetAsVariant	);	

	m_EditFrom.SetValue(	m_VarEditFrom	);		
	m_EditTo.SetValue(		m_VarEditTo		);		
	m_EditStep.SetValue(	m_VarEditStep	);	

	UpdateCheckBoxesVarie(	m_pLaVar->GetMaxVarie() + 1	);
	UpdateStaticText();

	this->GetDlgItem(IDC_VARIATION_NUMEDIT_FROM)->SetFocus();
	return FALSE; // return TRUE unless you set the focus to a control
}

// cf Doc&Astuces : "FAQ VC++ et MFC - Intercepter _entrée_.htm"
BOOL CDialogVariation::OnCommand(WPARAM wParam, LPARAM lParam)	
{
	CWnd *pWnd = GetFocus();
	switch(wParam)
	{
	case IDOK: if(pWnd!=GetDlgItem(IDOK))
			   {
				   reinterpret_cast<CDialog *>(this)->NextDlgCtrl();
				   return FALSE;
			   }
			   break;
	case IDCANCEL: if(pWnd!=GetDlgItem(IDCANCEL))
				   {
					   //   return FALSE;
				   }
				   break;
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CDialogVariation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VARIATION_NUMEDIT_FROM, m_EditFrom);
	DDX_Control(pDX, IDC_VARIATION_NUMEDIT_TO, m_EditTo);
	DDX_Control(pDX, IDC_VARIATION_NUMEDIT_STEP, m_EditStep);
}


BEGIN_MESSAGE_MAP(CDialogVariation, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages CDialogVariation
BEGIN_EVENTSINK_MAP(CDialogVariation, CDialog)
	ON_EVENT(CDialogVariation, IDC_VARIATION_NUMEDIT_FROM	, 1, ValueChangedVariationNumeditFrom, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogVariation, IDC_VARIATION_NUMEDIT_TO		, 1, ValueChangedVariationNumeditTo, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogVariation, IDC_VARIATION_NUMEDIT_STEP	, 1, ValueChangedVariationNumeditStep, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDialogVariation, IDC_VAR_CHECK_01		, 1, ValueChangedVarDialogCheck01, VTS_BOOL)
	ON_EVENT(CDialogVariation, IDC_VAR_CHECK_02		, 1, ValueChangedVarDialogCheck02, VTS_BOOL)
	ON_EVENT(CDialogVariation, IDC_VAR_CHECK_03		, 1, ValueChangedVarDialogCheck03, VTS_BOOL)
	ON_EVENT(CDialogVariation, IDC_VAR_CHECK_04		, 1, ValueChangedVarDialogCheck04, VTS_BOOL)
	ON_EVENT(CDialogVariation, IDC_VAR_CHECK_05		, 1, ValueChangedVarDialogCheck05, VTS_BOOL)
END_EVENTSINK_MAP()

#define BLABLA00 VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange
void CDialogVariation::ValueChangedVariationNumeditFrom(BLABLA00)
{	
	m_VarEditFrom = Value->dblVal;	
	UpdateCheckBoxesVarie();
}
void CDialogVariation::ValueChangedVariationNumeditTo(BLABLA00)
{	
	m_VarEditTo = Value->dblVal;	
	UpdateCheckBoxesVarie();	
}
void CDialogVariation::ValueChangedVariationNumeditStep(BLABLA00)
{
	m_VarEditStep = Value->dblVal;	
	Verifie();
	UpdateCheckBoxesVarie();	
}


void CDialogVariation::ValueChangedVarDialogCheck01(BOOL Value)
{	if(!Value)return;	m_Var_Varie		=	1	;	UpdateCheckBoxesVarie();	}
void CDialogVariation::ValueChangedVarDialogCheck02(BOOL Value)
{	if(!Value)return;	m_Var_Varie		=	2	;	UpdateCheckBoxesVarie();	}
void CDialogVariation::ValueChangedVarDialogCheck03(BOOL Value)
{	if(!Value)return;	m_Var_Varie		=	3	;	UpdateCheckBoxesVarie();	}
void CDialogVariation::ValueChangedVarDialogCheck04(BOOL Value)
{	if(!Value)return;	m_Var_Varie		=	4	;	UpdateCheckBoxesVarie();	}
void CDialogVariation::ValueChangedVarDialogCheck05(BOOL Value)
{	if(!Value)return;	m_Var_Varie		=	5	;	UpdateCheckBoxesVarie();	}

// Update Check boxes
void CDialogVariation::UpdateCheckBoxesVarie(int OrdreSuivantVariant /* =0 */)
{
	// on ne donnera un OrdreSuivantVariant à l'ouverture de la boite de dialog seulement
	if(OrdreSuivantVariant > 0)
	{
		GetDlgItem(IDC_VAR_CHECK_01)->EnableWindow( OrdreSuivantVariant >= 1);
		GetDlgItem(IDC_VAR_CHECK_02)->EnableWindow( OrdreSuivantVariant >= 2);
		GetDlgItem(IDC_VAR_CHECK_03)->EnableWindow( OrdreSuivantVariant >= 3);
		GetDlgItem(IDC_VAR_CHECK_04)->EnableWindow( OrdreSuivantVariant >= 4);
		GetDlgItem(IDC_VAR_CHECK_05)->EnableWindow( OrdreSuivantVariant >= 5);

		if(m_Var_Varie == 0)
			m_Var_Varie = OrdreSuivantVariant;
	}
	
	CheckDlgButton(IDC_VAR_CHECK_01, m_Var_Varie == 1);
	CheckDlgButton(IDC_VAR_CHECK_02, m_Var_Varie == 2);
	CheckDlgButton(IDC_VAR_CHECK_03, m_Var_Varie == 3);
	CheckDlgButton(IDC_VAR_CHECK_04, m_Var_Varie == 4);
	CheckDlgButton(IDC_VAR_CHECK_05, m_Var_Varie == 5);

	// si deux parametres varient ensemble, on fixe le meme nombre de pas
	bool IsNotTheFirstForThisVarie = ( m_Var_Varie < m_pLaVar->GetMaxVarie() + 1 );
	if(IsNotTheFirstForThisVarie)
	{
		for(unsigned int iemeVariant = 1; iemeVariant <= m_pLaVar->nVariants; iemeVariant++)
		{
			CParametre* pLeIemeParam = m_pLaVar->GetParametreIemeVariant(iemeVariant);
			if(m_Var_Varie == pLeIemeParam->Varie)
			{
				unsigned int numStep = pLeIemeParam->GetNumberStep();
//20070503				if(m_VarEditTo < m_VarEditFrom)
//					m_VarEditTo = min(m_VarEditTo, m_VarEditFrom - numStep + 1);
//				else
//					m_VarEditTo = max(m_VarEditTo, m_VarEditFrom + numStep - 1);

				m_VarEditStep = (m_VarEditTo - m_VarEditFrom) / (numStep-1);
			}
		}
		m_EditTo.SetValue(m_VarEditTo);
		m_EditStep.SetValue(m_VarEditStep);
	}
	GetDlgItem(IDC_VARIATION_NUMEDIT_STEP)->EnableWindow( !IsNotTheFirstForThisVarie);
}

// Update Static Text
void CDialogVariation::UpdateStaticText()
{
	unsigned int nVariantsSeqMainDialog = m_pLaVar->nVariants;
	for(unsigned int staticText = 1; staticText <= 5; staticText++)
	{
		CString text("");
		for(unsigned int i = 1; i <= nVariantsSeqMainDialog; i++)
		{
			if(m_pLaVar->GetParametreIemeVariant(i)->Varie == staticText)
				text.Append(m_pLaVar->GetTextAllParametre(i));
		}

		if(staticText == 1)	SetDlgItemText(IDC_VAR_TEXTE_INFO_1, text);
		if(staticText == 2)	SetDlgItemText(IDC_VAR_TEXTE_INFO_2, text);
		if(staticText == 3)	SetDlgItemText(IDC_VAR_TEXTE_INFO_3, text);
		if(staticText == 4)	SetDlgItemText(IDC_VAR_TEXTE_INFO_4, text);
		if(staticText == 5)	SetDlgItemText(IDC_VAR_TEXTE_INFO_5, text);

		if(staticText == 1)	SetDlgItemText(IDC_VAR_TEXTE_1, "Varie 1");
		if(staticText == 2)	SetDlgItemText(IDC_VAR_TEXTE_2, "Varie 2");
		if(staticText == 3)	SetDlgItemText(IDC_VAR_TEXTE_3, "Varie 3");
		if(staticText == 4)	SetDlgItemText(IDC_VAR_TEXTE_4, "Varie 4");
		if(staticText == 5)	SetDlgItemText(IDC_VAR_TEXTE_5, "Varie 5");
	}


}


// Remplit les champs From To Step Varie du CParametre
CParametre* CDialogVariation::ActualiseCParametreValues(void)
{
	m_pLeParam->m_From		= m_VarEditFrom	;
	m_pLeParam->m_To		= m_VarEditTo	;
	m_pLeParam->m_Step		= m_VarEditStep	;

	m_pLeParam->Varie		= m_Var_Varie	;

	return m_pLeParam;
}

// verifie les données pour éviter des truc absurde comme de 10 à 0 par pas de 1
// et corrige si possible ---->>>> par pas de  -1  !! 
void CDialogVariation::Verifie()
{
	if( (m_VarEditStep > 0 && (m_VarEditTo - m_VarEditFrom) > 0) || (m_VarEditStep < 0 && (m_VarEditTo - m_VarEditFrom) < 0 ) )
		return;
	else
	{
		MessageBox(
"Please ! ! !... Put a correct step !\n\
Something not null and with the good sign\n\
\n\
Value Will Be Adjusted for you! ",\
					"What are You Doing?");
		
		m_VarEditStep = -m_VarEditStep;

		if( m_VarEditStep == 0 && (m_VarEditTo - m_VarEditFrom) > 0 )
		{
			m_VarEditStep = max(1,(m_VarEditTo - m_VarEditFrom) / 10);
		}
		if( m_VarEditStep == 0 && (m_VarEditTo - m_VarEditFrom) < 0 )
		{
			m_VarEditStep = min(-1,(m_VarEditTo - m_VarEditFrom) / 10);
		}
		if( (m_VarEditTo - m_VarEditFrom) == 0 )
		{
			m_VarEditTo = m_VarEditFrom + 10;
			m_VarEditStep = max(1,(m_VarEditTo - m_VarEditFrom) / 10);
		}

		m_EditFrom.SetValue(	m_VarEditFrom	);		
		m_EditTo.SetValue(		m_VarEditTo		);		
		m_EditStep.SetValue(	m_VarEditStep	);	
	}
}





