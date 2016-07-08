#pragma once

class CSettingSave;
class CParametre;
typedef CList <CParametre*,CParametre*> CList_ParametrePointeur;// CList : pratique pour ranger des objets de maniere dynamique et efficace

enum{Entier, Double, String};
enum{Auto = -1};
enum{ CanParticipateToNothing = 0
	, CanParticipateVariation = 1
	, CanParticipateGeneticAlgo = 2};

# define FOR_ALL_PARAMETERS_DEFINED_do	 	    POSITION pos = CParametre::Liste_Static_AllParametres.GetHeadPosition();	\
												while(pos)																	\
												{																			\
												CParametre* pLeParam;														\
												pLeParam = CParametre::Liste_Static_AllParametres.GetNext(pos);				\


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////       CParametre       ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Une instance de CParametre regroupera les informations relatives � un parametre de la s�quence, 
et plus g�n�ralement, du programme.
*/

class CParametre
{
public:
	CParametre(void);
	CParametre(int unType, CString unNomTexte, CString unNomSave, int unNumero, void* unp_Val, CString ValDefaut, int unMustBeSaved = 1, bool unVarie = 0, double unFrom = -1, double unTo = -1, double unStep = -1, unsigned int unnSteps = 0, unsigned int unCourantStep = 0);
	CParametre(CSettingSave* pInstanceSave, int unType, CString unNomTexte, CString unNomSave, int unNumero, void* unp_Val, CString ValDefaut, int CanParticipateToScan = 0, int unMustBeSaved = 1, bool unVarie = 0, double unFrom = -1, double unTo = -1, double unStep = -1, unsigned int unnSteps = 0, unsigned int unCourantStep = 0);
	~CParametre(void);
	
	// membre statique :le nombre de Parametres instanci�s
	static unsigned int m_StaticNumberParametre;	
	
	// Une CListe (de pointeurs) de tous les param�tres d�clar�s
	static CList_ParametrePointeur Liste_Static_AllParametres;

public:
	// Le texte relatif au parametre tel qu'on le lit dans le programme en cas d'affichage
	CString m_NomTexte;	
	// Le nom qui rep�re les parametre dans le fichier de sauvegarde config
	CString m_NomSave;	
	// L'adresse d'une eventuelle instance de CSettingSave auquel le Parametre est li�
	CSettingSave* m_pSettingSave;
	// un num�ro de parametre pour etre identifi� par le programme facilement
	int m_Numero;		
	// un chiffre pour savoir si le parametre peut participer � un scan ou autre manipulation d�finie dans une autre classe (comme Algo genetic par exemple !)
	int m_CanParticipateTo;
	// ID du control auquel est r'attach� le parametre.
	int m_Idc;

private:
	// pointeur vers la valeur du param�tre
	// ou vers le d�but d'un tableau de valeurs ! (ce qui revient au m�me d'ailleurs)
	// seul l'un des trois pointeur suivant doit etre utiliser pour un param
	int* m_pInt;
	double* m_pDouble;
	CString* m_pString;

public:
	// Indique si on doit sauver le param�tre et combien de valeurs seront sauv�s si pVal est l'adresse d'un tableau de valeurs
	int m_MustBeSaved;
	// Indique que le param�tre pourrait �tre rep�r� comme manquant lors du chargement d'un fichier de sauvegarde.
	// true par default ! si false, Le programme ne se plaindra jamais que ce parametre manque
	// pratique pour des parametre optionnel
	bool m_MightComplainIfMissing;

	// de quel Type est le paam � sauver : Entier, Double, String,...
	int m_Type;	

	// indique si le parametre fait parti d'une variation
	// et � quel dimension de l'exploration des parametre de la variation il appartient
	int Varie;			

	double m_From;
	double m_To;
	double m_Step;			

	// Nombre de Step pour faire varier le param�tre : commence � 1
	unsigned int nSteps;	

	// Le Step, parmis les nSteps, o� on se trouve : commence � 1	
	unsigned int CourantStep;	

public:

	// Une fonction pour renvoyer le pointeur vers le param�tre dont le "m_Numero" est "LeNumero"
	static CParametre* Param(int LeNumero);
	// Une fonction pour renvoyer le pointeur vers le param�tre dont le m_PointeurVersValue est LePointeur
	static CParametre* Param(void* LePointeur);
	// Renvoye le pointeur vers la Valeur du Parametre
	void* Get_pVal();
	// Renvoye la valeur pour un entier, ou un double
	const double  Get_Val(int iemeValeurTableau = 0);
	// Renvoye la Valeur Int du Parametre, ou la iemeValeurTableau suivante dans le m�moire (tableau)
	const int	  Get_Val_Int(int iemeValeurTableau = 0);
	// Renvoye la Valeur Double du Parametre, ou la iemeValeurTableau suivante dans le m�moire (tableau)
	const double  Get_Val_Double(int iemeValeurTableau = 0);
	// Renvoye la Valeur CString du Parametre, ou la iemeValeurTableau suivante dans le m�moire (tableau)
	const CString Get_Val_String(int iemeValeurTableau = 0);
	// Change la Valeur du Parametre, ou la iemeValeurTableau suivante dans le m�moire (tableau)
	void Set_Val(int	 NewVal, int iemeValeurTableau = 0);
	void Set_Val(double  NewVal, int iemeValeurTableau = 0);
	void Set_Val(CString NewVal, int iemeValeurTableau = 0);
	// Incr�mente la Valeur du Parametre de Step
	void StepUp();
	// Renvoye le nombre de pas qui sera fait
	unsigned int GetNumberStep();	
};
