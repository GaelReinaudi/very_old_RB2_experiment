#pragma once
#include "Parametre.h"

#define MAX_PROGRESS_BAR 5

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////      CVariation       ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class CVariation	// un objet r�unissant les diff�rents pointeurs vers les param�tres qui vont varier
{
public:
	CVariation(void);
	~CVariation(void);

public:
	// Le Nombre Courant de Parametres* dans la CVariation : commence � 1
	unsigned int nVariants;		

//	// Le nombre de choses(1,2,3..parametres) qui varient ind�pendemment
//	int m_VariMax

	// Une CListe de pointeur vers les Parametres variant
	CList_ParametrePointeur Liste_Variants;	

	// jusqu'� xx parametres peuvent varier dans la meme s�quence. cette liste est compos�e de 1 et de 0
	// les 1, ou 0 adjascents signifient que les param_etres varient ensemble
//**/	char VariEnsemble[64];

	// Nombre de Step pour faire varier tous les param�tres : commence � 1
	unsigned int nSteps;	

	// Le Step, parmis les nSteps, o� on se trouve : commence � 1
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
	// Supprime tous les param�tres de la variation qui correspondent � la iemVarie diamension de la varaition
	void RemoveVarie(int iemeVarie);							

	// Renvoye le nombre de pas qui sera fait dans toute la s�quence en variant les param�tres les uns apr�s les autres
	unsigned int GetTotalSteps(unsigned int iemeParametre = -1);

	// Renvoye le iemeVariant CParametre* de la Liste_Variants
	CParametre* GetParametreIemeVariant(unsigned int iemeVariant);	

	// Renvoye le max des ".Varie" des CParametre de la Variation
	int GetMaxVarie();

	// Va Modifier les Param�tres variant en accord avec ce qui doit varier
	int AssignParamNextStep(CProgressCtrl** ProgBarDansLaCDialogBar, bool Initialise = false, bool RemetLesProgressBarAZero = false);

	// Renvoye une CString qui contient les d�tails de la CVariation
	CString GetTextAllParametre(unsigned int iemeVariant = 0, bool NotTheNameButTheProgName = false);

	// dit si le param�tre numero Num est d�j� dans la CVariation renvoye la varie
	int ParamAlready(int Num);		

	// Va �crire le r�sum� de ce qui se passe dans la variatin, da
	CString SumUpToFile(CString FileName, CString HeadOfTheFile = CString(""));
};
