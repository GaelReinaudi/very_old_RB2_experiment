#include "StdAfx.h"
#include "Resource.h"

#include "Variation.h"
#include "SettingSave.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////        CVariation        //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CVariation::CVariation(void)
{
	nVariants = unsigned int(0);
	nSteps = unsigned int(0);
	m_CourantVarieStep = unsigned int(0);
	m_Repete = unsigned int(0);
}

CVariation::~CVariation(void)
{
}
/*
Supprime tous les Paramêtres
*/
void CVariation::ClearParametreList()
{
	Liste_Variants.RemoveAll();
	nVariants = 0;
}

/*
Fait une liste des Parametres qui Varient
return true si il y a une Varie au moins
*/
bool CVariation::MakeParametreList(int FromParam /* = -1 */, int ToParam /* = -1 */)
{
	ClearParametreList();
	// on crée la liste de Parametres qui Varie(nt)
	bool ExistVarie = false;

	FOR_ALL_PARAMETERS_DEFINED_do		
		if(pLeParam && pLeParam->Varie)
		{
			AddParametre(pLeParam);
			ExistVarie = true;
		}
	}
return ExistVarie;
}

/*
Ajoute un Parametre dans la liste en classant en fonction de "Varie"
*/
void CVariation::AddParametre(CParametre* pNewPar)
{	
	int VarieNew = pNewPar->Varie;
	for(unsigned int i = 0; i < nVariants; i++)
	{
		POSITION pos = Liste_Variants.FindIndex(i);
		if(VarieNew < Liste_Variants.GetAt(pos)->Varie)
		{
			Liste_Variants.InsertBefore(pos, pNewPar);
			nVariants++;
			return;
		}
	}

	Liste_Variants.AddTail(pNewPar);
	nVariants++;
	return;
}
/*
Supprime tous les paramêtres de la variation qui correspondent à la iemVarie dimension 
de la variation
*/
void CVariation::RemoveVarie(int iemeVarie)
{
	for(unsigned int i = 0; i < nVariants; i++)
	{
		POSITION pos = Liste_Variants.FindIndex(i);
		
		if(iemeVarie == Liste_Variants.GetAt(pos)->Varie)
		{
			Liste_Variants.GetAt(pos)->Varie = 0;
		
			Liste_Variants.RemoveAt(pos);
			i--;
			nVariants--;
		}
	}
	for(unsigned int i = 0; i < nVariants; i++)
	{
		POSITION pos = Liste_Variants.FindIndex(i);
		
		if(iemeVarie < Liste_Variants.GetAt(pos)->Varie)
		{
			Liste_Variants.GetAt(pos)->Varie -= 1;
		}
	}
return;
}
/*
Renvoye le nombre de pas qui sera fait dans la séquence 
en variant les paramètres les uns après les autres (compte jusqu'à la iemeParametre : commence à 0)
Si on ne précise pas iemeParametre, on à toute la séquence.
*/
unsigned int CVariation::GetTotalSteps(unsigned int iemeParametre /* = -1 */ )
{
	nVariants = 0;
	nSteps = 1;
	int VarieCourant = 0;
	POSITION pos = Liste_Variants.GetHeadPosition();
	while(pos && (iemeParametre == -1 || nVariants <= iemeParametre) )
	{
		CParametre* pLeParam = Liste_Variants.GetNext(pos);
		// si on passe effectivement à un parametre qui varient indépandemment des précédents
		if(pLeParam->Varie == VarieCourant + 1)
		{
			VarieCourant =  pLeParam->Varie;
			nSteps *= pLeParam->GetNumberStep();
		}
		nVariants++;
	}
return nSteps;
}
/*
Renvoye le iemeVariant CParametre* de la Liste_Variants
*/
CParametre* CVariation::GetParametreIemeVariant(unsigned int iemeVariant)
{
	POSITION pos = Liste_Variants.GetHeadPosition();
	CParametre* pLeParam;
	unsigned int count = 0;
	while( pos )
	{
		count++;
		pLeParam = Liste_Variants.GetNext(pos);
		if(count == iemeVariant)
			return pLeParam;
	}
return NULL;
}

// Renvoye le max des ".Varie" des CParametre de la Variation
int CVariation::GetMaxVarie()
{
	int LeMaxVarie = 0;

	for(unsigned int i = 1; i <= nVariants; i++)
	{
		LeMaxVarie = max(LeMaxVarie , GetParametreIemeVariant( i )->Varie);
	}
return LeMaxVarie;
}

/*
Va Modifier les Paramètres variant en accord avec ce qui doit varier
Renvoye le Varie qui a été modifié en dernier
ou 0 si on a fini de faire tout varier
On passe en parametre les ProgressBar[i], i de 1 à MAX_PROGRESS_BAR
*/
int CVariation::AssignParamNextStep(CProgressCtrl** ProgBarDansLaCDialogBar, bool Initialise /* = false */ , bool RemetLesProgressBarAZero /* = false */ )
{	
	CProgressCtrl* pProgress[MAX_PROGRESS_BAR+1];

	for(char i = 1; i <= MAX_PROGRESS_BAR; i++)
	{
		pProgress[i] = ProgBarDansLaCDialogBar[i];
	}

	POSITION pos = Liste_Variants.GetHeadPosition();
    if(pos == NULL)
		return 0;
	CParametre* pPar = Liste_Variants.GetNext(pos);
	int VarieCourant = 0;
// si on passe le "Initialise = 1" à la fonction
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(Initialise)																							//
	{																										//
		m_CourantVarieStep = 1;																				//
		while(1)																							//
		{																									//
			pProgress[pPar->Varie]->SetRange32(0, pPar->GetNumberStep());				//
			pProgress[pPar->Varie]->SetStep(1);												//
			pProgress[pPar->Varie]->SetPos(RemetLesProgressBarAZero? 0 : 1);				//
			
			pPar->Set_Val( pPar->m_From );																	//
			if(pos == NULL)																					//
				return VarieCourant;																		//
			pPar = Liste_Variants.GetNext(pos);																//
		}																									//
	}																										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_CourantVarieStep++;

	int VarieMax = GetMaxVarie();

	bool PassonsAuVarieSuivant = true;
	// boucle pour faire varier tous les groupes de parametres 
	// correspondant à différents Varie
	while(1)
	{
		VarieCourant++;
		if(PassonsAuVarieSuivant == false)
			return VarieCourant;
		else
			PassonsAuVarieSuivant = false;
		// boucle pour faire varier tous les parametres d'un meme VarieCourant
		// si ils l'un d'eux est en buttée : PassonsAuVarieSuivant = 1;
		bool JamaisPasse = true; // pour faire un seul StepIt(); pour tous les param d'un meme Varie
		while( pPar->Varie == VarieCourant )
		{
			double StepSign = pPar->m_Step >= 0 ? 1 : -1;
			double CurrentValPar = pPar->Get_Val();

			if( (CurrentValPar + pPar->m_Step) * StepSign > pPar->m_To * StepSign 
				|| PassonsAuVarieSuivant )
			{
				pPar->Set_Val( pPar->m_From );
				pProgress[VarieCourant]->SetPos(1);
				PassonsAuVarieSuivant = true;
				if(VarieCourant >= VarieMax)
					return 0;
			}
			else
			{
				pPar->StepUp();
				if(JamaisPasse)
					pProgress[VarieCourant]->StepIt();
				JamaisPasse = false;
			}
			if(pos == NULL)
			{	
				return VarieCourant;
			}
			else
				pPar = Liste_Variants.GetNext(pos);
		}
	}
return -1;
}

/*
Renvoye une CString qui contient le détail du  iemeVariant
si on ne donne pas iemeVariant, alors :
Renvoye une CString qui contient les détails de la CVariation
si on donne NotTheNameButTheProgName = true, alors le nom du param qui varie est celui du programme sans le "m_", ex:"MelasseFinMotIntensity"
*/
CString CVariation::GetTextAllParametre(unsigned int iemeVariant /* =0 */, bool NotTheNameButTheProgName /* = false */)
{	
	CString detail("");
	POSITION pos = Liste_Variants.GetHeadPosition();
	unsigned int count = 0;
	while(pos)
	{
		char temp[32];
		CParametre& LeParam = *Liste_Variants.GetNext(pos);
		count++;
		if( !iemeVariant || count == iemeVariant)
		{
			if(NotTheNameButTheProgName)
			{
				int pos = LeParam.m_NomSave.Find("m_",2) + 2;
				detail += LeParam.m_NomSave.Tokenize("_ ", pos);
			}
			else
				detail += LeParam.m_NomTexte;
			detail.Append(	"  From "	);	sprintf(temp,"%g", LeParam.m_From);	detail.Append(	temp	);
			detail.Append(	"  To "	)	;	sprintf(temp,"%g", LeParam.m_To);	detail.Append(	temp	);
			detail.Append(	"  Step "	);	sprintf(temp,"%g", LeParam.m_Step);	detail.Append(	temp	);
			detail.Append("\t\r\n");
		}
	}
return detail;
}
/*
dit si le paramètre pointé est déjà dans la CVariation
*/
int CVariation::ParamAlready(int Num)
{
	POSITION pos = Liste_Variants.GetHeadPosition();
	int iemeVarie = 0;
	while(pos)
	{
		iemeVarie++;
		if(Num == Liste_Variants.GetNext(pos)->m_Numero)
			return iemeVarie;
	}
return 0;
}

/*
Va écrire le résumé de ce qui se passe dans la variatin, dans un fichier
*/
CString CVariation::SumUpToFile(CString FileName, CString HeadOfTheFile /* = "" */)
{
	// l'éventuel Header donné en parametre
	CString TextePourRb2Cam("");
	TextePourRb2Cam += HeadOfTheFile + CString(" \t\r\n \t\r\n") ;

	// le numero courant du pas de la variation
	CString NumeroDuPasDeLaVariation , NombreDePasTotal;
	NumeroDuPasDeLaVariation.Format(_T("%d"), m_CourantVarieStep); // numero
	NombreDePasTotal.Format(_T("%d"), GetTotalSteps()); // total
	TextePourRb2Cam += CString("Pas_Total_Numero \t") + NumeroDuPasDeLaVariation + CString("\tsur\t") + NombreDePasTotal + CString(" \t\r\n \t\r\n") ;

	// le nombre de répétitions par pas
	CString Nombre;
	Nombre.Format(_T("%d"), m_Repete); // numero
	TextePourRb2Cam += CString("Nombre_Repete_Par_Pas \t") + Nombre + CString(" \t\r\n \t\r\n") ;

	// les Varie !
	for(int iemeVarie = 1; iemeVarie <= GetMaxVarie(); iemeVarie++)
	{
		// on récupère le texte des parametres qui varient en ième position de la CVariation
		CString TexteUneVarie("");
		for(unsigned int iemeVariant = 1; iemeVariant <= nVariants; iemeVariant++)
		{
			CParametre* LePar = GetParametreIemeVariant(iemeVariant);
			if(LePar->Varie == iemeVarie)
			{
				// on ajoute le texte de ce variant
				TexteUneVarie += GetTextAllParametre(iemeVariant, true);
				// on ajoute la valeur courante
				double LaValeur = LePar->Get_Val();
				CString Sval;
				Sval.Format(_T("%g"), LaValeur);
				TexteUneVarie += CString(" =\t") + Sval + CString(" \t");

				// on peut ajouter une liste où il y a toutes les valeurs prises par chaque parametre
				bool liste = true;
				if(liste)
				{
					CString ElementDeLaListe("");
					TexteUneVarie += "[";
					for(unsigned int NumeroStep = 1; NumeroStep <= LePar->GetNumberStep(); NumeroStep++)
					{
						bool derniereVirgule = (NumeroStep == LePar->GetNumberStep());
						ElementDeLaListe.Format(_T(derniereVirgule ? "%g": "%g,"), LePar->m_From + (NumeroStep - 1) * LePar->m_Step);
						TexteUneVarie += ElementDeLaListe;
					}

					TexteUneVarie += "]";
				}

				// à la ligne
				TexteUneVarie += CString(" \t\r\n \t\r\n");;
			}
		}
		// Des Varies Différentes sont séparées par des _____
		TexteUneVarie += CString("__________________________________________\t\r\n \t\r\n");
		TextePourRb2Cam += TexteUneVarie;
	}

	// on sauve
	CTextFile	DialogCam("txt", " \t\r\n");
	DialogCam.WriteTextFile(FileName
								, TextePourRb2Cam
							);
return TextePourRb2Cam;
}


