#pragma once
#include "Parametre.h"

#define MAX_PROGRESS_BAR 5

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////      CVariation       ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class CVariation	// un objet réunissant les différents pointeurs vers les paramètres qui vont varier
{
public:
	CVariation(void);
	~CVariation(void);

public:
	// Le Nombre Courant de Parametres* dans la CVariation : commence à 1
	unsigned int nVariants;		

//	// Le nombre de choses(1,2,3..parametres) qui varient indépendemment
//	int m_VariMax

	// Une CListe de pointeur vers les Parametres variant
	CList_ParametrePointeur Liste_Variants;	

	// jusqu'à xx parametres peuvent varier dans la meme séquence. cette liste est composée de 1 et de 0
	// les 1, ou 0 adjascents signifient que les param_etres varient ensemble
//**/	char VariEnsemble[64];

	// Nombre de Step pour faire varier tous les paramètres : commence à 1
	unsigned int nSteps;	

	// Le Step, parmis les nSteps, où on se trouve : commence à 1
	unsigned int m_CourantVarieStep;

	// le nombre de repete qui est actualiser en meme temps que celui de la Sequence
	unsigned int m_Repete;

private:
	// supprime tous les Parametres
	void ClearParametreList();										
public:			
	// Fait une liste des Parametres qui Varient
	bool MakeParametreList( int FromParam = -1, int ToParam = -1);										
	// Ajoute un Parametre dans la liste en classant en fonction de "Varie"
	void AddParametre(CParametre* pNewPar);							
	// Supprime tous les paramêtres de la variation qui correspondent à la iemVarie diamension de la varaition
	void RemoveVarie(int iemeVarie);							

	// Renvoye le nombre de pas qui sera fait dans toute la séquence en variant les paramètres les uns après les autres
	unsigned int GetTotalSteps(unsigned int iemeParametre = -1);

	// Renvoye le iemeVariant CParametre* de la Liste_Variants
	CParametre* GetParametreIemeVariant(unsigned int iemeVariant);	

	// Renvoye le max des ".Varie" des CParametre de la Variation
	int GetMaxVarie();

	// Va Modifier les Paramètres variant en accord avec ce qui doit varier
	int AssignParamNextStep(CProgressCtrl** ProgBarDansLaCDialogBar, bool Initialise = false, bool RemetLesProgressBarAZero = false);

	// Renvoye une CString qui contient les détails de la CVariation
	CString GetTextAllParametre(unsigned int iemeVariant = 0, bool NotTheNameButTheProgName = false);

	// dit si le paramètre numero Num est déjà dans la CVariation renvoye la varie
	int ParamAlready(int Num);		

	// Va écrire le résumé de ce qui se passe dans la variatin, da
	CString SumUpToFile(CString FileName, CString HeadOfTheFile = CString(""));
};
