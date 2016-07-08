#include "StdAfx.h"
#include "SettingSave.h"
#include "Parametre.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////        CParametre        ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
Initialisation des variables static
*/
unsigned int CParametre::m_StaticNumberParametre = 0;
CList_ParametrePointeur CParametre::Liste_Static_AllParametres;


CParametre::CParametre(void)
{
	m_Type			= Entier;
	m_NomTexte		= "warning_you_should_not_see_this";
	m_NomSave		= "warning_you_should_not_see_this";
	m_pSettingSave	= NULL;
	m_Numero		= -1111;
	m_CanParticipateTo = -1111;
	m_pInt			= NULL;
	m_pDouble		= NULL;
	m_pString		= NULL;
	m_MustBeSaved	= 0;
	Varie			= 0;
	m_From			= -1111;
	m_To			= -1111;
	m_Step			= -1111;
	nSteps			= -1111;
	CourantStep		= -1111;
	m_StaticNumberParametre++;
	m_MightComplainIfMissing = true;
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()),"C'est pas bien de déclarer un CParametre comme ça ! "
											 ,"CParametre::CParametre(void)", 1);
}
/*
 Un paramètre déclaré comme cela sera ajouté à la CList static "Liste_Static_AllParametres"
*/
CParametre::CParametre(int unType, CString unNomTexte, CString unNomSave, int unNumero, void* unp_Val, CString ValDefaut, int unMustBeSaved, bool unVarie, double unFrom, double unTo, double unStep, unsigned int unnSteps, unsigned int unCourantStep)
{
	m_Type			= unType		;
	m_NomTexte		= unNomTexte	;
	m_NomSave		= unNomSave		;

	m_pSettingSave	= NULL;
	if(unNumero == Auto)
		m_Numero		= m_StaticNumberParametre + 1;
	else
		m_Numero		= unNumero		;

	m_CanParticipateTo = 0;
	m_Idc			= 0;

	m_pInt			= NULL;
	m_pDouble		= NULL;
	m_pString		= NULL;
	char  *stopstring;
	switch(m_Type)
	{
	case Entier:
		m_pInt		= (int*)unp_Val	;
		if(ValDefaut != "")
			Set_Val(strtol(ValDefaut, &stopstring,10));
		break;
	case Double:
		m_pDouble	= (double*)unp_Val;
		if(ValDefaut != "")
			Set_Val(strtod(ValDefaut, &stopstring));
		break;
	case String:
		m_pString	= (CString*)unp_Val;
		if(ValDefaut != "")
			Set_Val(ValDefaut);
		break;
	}
	m_MustBeSaved	= unMustBeSaved	;
	Varie			= unVarie		;
	m_From			= unFrom		;
	m_To			= unTo			;
	m_Step			= unStep		;
	nSteps			= unnSteps		;
	CourantStep		= unCourantStep	;
	m_StaticNumberParametre++;
	m_MightComplainIfMissing = true;

	Liste_Static_AllParametres.AddTail(this);
}

CParametre::CParametre(CSettingSave* pInstanceSave, int unType, CString unNomTexte, CString unNomSave, int unNumero, void* unp_Val, CString ValDefaut, int CanParticipateToScan, int unMustBeSaved, bool unVarie, double unFrom, double unTo, double unStep, unsigned int unnSteps, unsigned int unCourantStep)
{
	m_pSettingSave	= pInstanceSave;
	m_pSettingSave->m_List_pParam.AddTail(this);

	m_Type			= unType		;
	m_NomTexte		= unNomTexte	;
	m_NomSave		= unNomSave		;

	if(unNumero == Auto)
		m_Numero	= m_StaticNumberParametre + 1;
	else
		m_Numero	= unNumero		;

	m_CanParticipateTo = CanParticipateToScan;
	m_Idc			= 0;

	m_pInt			= NULL;
	m_pDouble		= NULL;
	m_pString		= NULL;
	char  *stopstring;
	switch(m_Type)
	{
	case Entier:
		m_pInt		= (int*)unp_Val	;
		if(ValDefaut != "")
			Set_Val(strtol(ValDefaut, &stopstring,10));
		break;
	case Double:
		m_pDouble	= (double*)unp_Val;
		if(ValDefaut != "")
			Set_Val(strtod(ValDefaut, &stopstring));
		break;
	case String:
		m_pString	= (CString*)unp_Val;
		if(ValDefaut != "")
			Set_Val(ValDefaut);
		break;
	}
	m_MustBeSaved	= unMustBeSaved	;
	Varie			= unVarie		;
	m_From			= unFrom		;
	m_To			= unTo			;
	m_Step			= unStep		;
	nSteps			= unnSteps		;
	CourantStep		= unCourantStep	;
	m_StaticNumberParametre++;
	m_MightComplainIfMissing = true;
	Liste_Static_AllParametres.AddTail(this);
}

CParametre::~CParametre(void)
{	
	if(m_pSettingSave)
	{
		POSITION pos = m_pSettingSave->m_List_pParam.Find(this);
		m_pSettingSave->m_List_pParam.RemoveAt(pos);
	}
	POSITION pos = Liste_Static_AllParametres.Find(this);
	Liste_Static_AllParametres.RemoveAt(pos);

	m_StaticNumberParametre--;
}

/*
Renvoye le nombre de pas qui sera fait
*/
unsigned int CParametre::GetNumberStep()
{
	nSteps = 1 + int((abs(m_To - m_From))/abs(m_Step));
return nSteps;
}

/*
Renvoye le pointeur vers la Valeur du Parametre
ou vers le début d'un tableau de valeurs (c'est la meme chose de toute manière)
*/
void* CParametre::Get_pVal()
{
	switch(m_Type)
	{
	case Entier:
		return m_pInt;
	case Double:
		return m_pDouble;
	case String:
		return m_pString;
	}
return NULL;
}

/*
Renvoye la Valeur Int du Parametre
Si on ne précise pas la iemeValeurTableau, on a la première valeur
Sinon, on a les valeurs suivantes : 0:début du tableau, 1:valeur suivante, etc...
*/
const double  CParametre::Get_Val(int iemeValeurTableau /* = 0 */)
{	
	if(this == NULL)
		return 0;
	if(m_Type == Entier)
	{
		return double(*(m_pInt + iemeValeurTableau));
	}
	if(m_Type == Double)
	{	
		return *(m_pDouble + iemeValeurTableau);
	}
	CString Str; Str.Format(_T("Tu demandes d'un parametre ni double ni entier \rParamètre n°: %d : "), m_Type, m_Numero);
	Str += m_NomSave;
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()),Str, "", 1);
return 0;	
}

const int	  CParametre::Get_Val_Int(int iemeValeurTableau /* = 0 */)
{	
	if(this == NULL)
		return 0;
	if(m_Type == Entier)
	{
		return *(m_pInt + iemeValeurTableau);
	}
	CString Str; Str.Format(_T("Tu demandes un entier à un parametre qui est déclaré de type : %d \rParamètre n°: %d : "), m_Type, m_Numero);
	Str += m_NomSave;
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()),Str, "", 1);
return 0;	
}
const double  CParametre::Get_Val_Double(int iemeValeurTableau /* = 0 */)
{
	if(this == NULL)
		return 0.0;
	if(m_Type == Double)
	{	
		return *(m_pDouble + iemeValeurTableau);
	}
	CString Str; Str.Format(_T("Tu demandes un double à un parametre qui est déclaré de type : %d \rParamètre n°: %d : "), m_Type, m_Numero);
	Str += m_NomSave;
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()),Str, "", 1);
return 0;	
}
const CString CParametre::Get_Val_String(int iemeValeurTableau /* = 0 */)
{	
	if(this == NULL)
		return CString("");
	if(m_Type == String)
	{	
		return *(m_pString + iemeValeurTableau);
	}
	CString Str; Str.Format(_T("Tu demandes un CString à un parametre qui est déclaré de type : %d \rParamètre n°: %d : "), m_Type, m_Numero);
	Str += m_NomSave;
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()),Str, "", 1);	
return 0;	
}

/*
Change la Valeur du Parametre
Si on ne précise pas la iemeValeurTableau, on a la première valeur
Sinon, on a les valeurs suivantes : 0:début du tableau, 1:valeur suivante, etc...
*/
void CParametre::Set_Val(double	 NewVal, int iemeValeurTableau /* = 0 */)
{
	if(this == NULL)
		return;
	if(m_Type == Entier)
	{
		*(m_pInt + iemeValeurTableau)		= int(NewVal);
		return;
	}
	if(m_Type == Double)
	{
		*(m_pDouble + iemeValeurTableau)	= NewVal;
		return;
	}
	if(m_Type == String)
	{
		(*(m_pString + iemeValeurTableau)).Format("%g", NewVal);
		return;
	}
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()),"Impossible d'allouer unevaleur", "", 1);	return;
}
void CParametre::Set_Val(int  NewVal, int iemeValeurTableau /* = 0 */)
{
	if(this == NULL)
		return;
	Set_Val(double(NewVal), iemeValeurTableau);
}
void CParametre::Set_Val(CString NewVal, int iemeValeurTableau /* = 0 */)
{
	if(this == NULL)
		return;
	if(m_Type != String)	
	{	
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()),"Tu essayes de fixer un String à un parametre qui n'est pas déclaré comme tel", "", 1);	return;	
	}
	*(m_pString + iemeValeurTableau) = NewVal;
return;
}

/*
Incrémente la Valeur du Parametre de Step
*/
void CParametre::StepUp()
{	
	if(m_Type != Entier && m_Type != Double)
	{	
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()),"Tu essayes de SteUp() un entier pour parametre qui n'est pas déclaré comme tel", "", 1);	return;	
	}
	if(m_Type == Entier)
	{
		*m_pInt += int(m_Step);
	}
	if(m_Type == Double)
	{
		*m_pDouble += m_Step;
	}
}

/*
Renvoyer un pointeur vers le paramètre dont le "m_Numero" est "LeNumero"
*/
CParametre* CParametre::Param(int LeNumero)
{
	FOR_ALL_PARAMETERS_DEFINED_do
		if(pLeParam->m_Numero == LeNumero)
		{
			return pLeParam;
		}
	}
CString info; info.Format(_T("Paramètre non-trouvé à partir de son numéro ! : n° %d"), LeNumero);
MessageBox(*((CFrameWnd*)AfxGetMainWnd()), info, "", 1);
return NULL;
}

/*
Renvoyer un pointeur vers le paramètre dont le m_PointeurVersValue est LePointeur
*/
CParametre* CParametre::Param(void* LePointeur)
{
	FOR_ALL_PARAMETERS_DEFINED_do
		if(pLeParam->m_Type == Entier)
			if(pLeParam->m_pInt		== LePointeur)
				return pLeParam;
		if(pLeParam->m_Type == String)
			if(pLeParam->m_pString	== LePointeur)
				return pLeParam;
		if(pLeParam->m_Type == Double)
			if(pLeParam->m_pDouble	== LePointeur)
				return pLeParam;
	}
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()),"Paramètre non-trouvé à partir du pointeur !", "", 1);
return NULL;
}














