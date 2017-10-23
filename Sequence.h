#pragma once

#include "Colonne.h"
#include "Variation.h"

#define DEFAULT_SEQ_NAME "Un-named"
#define DEFAULT_SEQ_FILE_NAME "C:\\"

#define MILLI_SECONDE(chiffre) (((double)chiffre * Sequence::m_SampleRate) / 1000.0)
#define MILLI_SECONDE_INT(chiffre) (int)MILLI_SECONDE(chiffre)
#define MICRO_SECONDE(chiffre) ((double)chiffre * Sequence::m_SampleRate / 1000000.0)
#define MICRO_SECONDE_INT(chiffre) (int)MICRO_SECONDE(chiffre)
#define MILLI_VOLT *(0.001)

#define PERIODE_TIMER_SCAN_PARAM 1000 // milli-secondes

//#define LAST_V_SEQ(ieme) LaSeq.GetColonne(-2)->GetLastPointWf(ieme).y
//#define PLATEAU_CONTINUITY_LAST_VALUE 	LAST_V_SEQ(0),LAST_V_SEQ(0),LAST_V_SEQ(1),LAST_V_SEQ(1),LAST_V_SEQ(2),LAST_V_SEQ(2),LAST_V_SEQ(3),LAST_V_SEQ(3),LAST_V_SEQ(4),LAST_V_SEQ(4),LAST_V_SEQ(5),LAST_V_SEQ(5),LAST_V_SEQ(6),LAST_V_SEQ(6),LAST_V_SEQ(7),LAST_V_SEQ(7)
													
#define NOMBRE_MAX_COLONNES_SAUVE 20

enum {LastColonne = -1, AfterLastColonne = -1, FirstColonne = 1, BeforeFirstColonne = 0};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////        Sequence        ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#define ERREUR 31416
enum{NoConti = 0, Constance = 1, ContinueAGauche = 2, SlopeContinueAGauche = 4, DoNothing = 1024*1024};
typedef CList <Colonne*,Colonne*> CList_pColonne;// CList : pratique pour ranger des objets de manière dynamique et efficace

class Sequence
{
public:
	Sequence(void);
	Sequence(Sequence& UneSequence);
	~Sequence(void);

private:
	CString m_NameSeq;						// une séquence a un nom
	CString m_FileSave;						// une séquence a un fichier de stockage
	CString m_Description;					// l'utilisateur peut décrire la séquence
	CSettingSave m_SeqSave;					// une séquence a sont lot de parametre à sauver
public:
	unsigned int m_nPeriodes;				// nombre de periodes dans la séquence
	static double m_SampleRate;		// samples par seconde
	int m_nRepetition;						// le nombre de fois que la séquence est sensée etre jouée

	void LoadNewSequenceFromFile(CString filename = "");// Load remplace la sequence par celle decrite dans le fichier
	void SaveSequenceToFile(CString filename = "");		// Sauve la sequence dans le fichier

private:
	int m_nColonnes;						// Le Nombre Courant de Colonnes dans la séquence
	CList_pColonne m_Liste_p_Colonnes;		// une sequence contient des colonnes (Liste de pointeurs vers les colonnes)
private:
	// Une Liste static de Cparametre qui donne la valeur par defaut des sorties digitales
	static CParametre* m_ParamDefaultDigOut[NUMBER_DIG_OUTPUT];
	// Une Liste static de Cparametre qui donne la valeur par defaut des sorties digitales
	static CParametre* m_ParamDefaultAnaOut[NUMBER_ANA_OUTPUT];

public:
	static CParametre* GetDefaultParamAnaOut(int indexAna){if(indexAna < NUMBER_ANA_OUTPUT) return m_ParamDefaultAnaOut[indexAna]; else return NULL;};
	static CParametre* GetDefaultParamDigOut(int indexDig){if(indexDig < NUMBER_DIG_OUTPUT) return m_ParamDefaultDigOut[indexDig]; else return NULL;};
	static void SetDefaultParamAnaOut(int indexAna, CParametre* pParam){if(indexAna < NUMBER_ANA_OUTPUT) m_ParamDefaultAnaOut[indexAna] = pParam;};
	static void SetDefaultParamDigOut(int indexDig, CParametre* pParam){if(indexDig < NUMBER_DIG_OUTPUT) m_ParamDefaultDigOut[indexDig] = pParam;};

public:
	static CListCtrl* p_ListCtrlColonnes;	// un eventuel pointeur vers un CListCtrl à actualiser dans un DialogBar comme m_SeqView
	static CListCtrl* p_ListCtrlLignesDig;	// un eventuel pointeur vers un CListCtrl à actualiser dans un DialogBar comme m_SeqView
	static CListCtrl* p_ListCtrlLignesAna;	// un eventuel pointeur vers un CListCtrl à actualiser dans un DialogBar comme m_SeqView
public:
	CVariation m_LaVariation;				// La CVariation associée à la séquence

private:
	unsigned long* p_DigBuffer;				// Le pointeur vers le buffer qui contiendra les sorties digitales lu par les cartes
	CNiUInt32Matrix m_DigBufferNiMatrix[NUMBER_CARDS_DIG_OUTPUT];	// Le pointeur vers le buffer qui contiendra les sorties digitales lu par les cartes
	double* p_AnaBuffer[NUMBER_ANA_OUTPUT];	// Les pointeurs vers les buffers qui contiendront les sorties analogiques lues par les cartes
	CWaveForme m_WholeSeqWaveForme[NUMBER_ANA_OUTPUT]; // Chaque sortie Analogique peut avoir sa WaveForme, qui la définit sur TOUTE la durée de la séquence. Passer par cette étape pourra grandement optimiser le temps passé pour faire les  interpolations linéaires
	CNiReal64Matrix m_AnaBufferNiMatrix[NUMBER_CARDS_ANA_OUTPUT];
public:
	// renvoye une référence à la matrice Pour la iemeCard digital 
	CNiUInt32Matrix& GetDigBufferNiMatrix_Card(int iemeDigCard){CString strCards; strCards.Format("There is only %d Digital Cards", NUMBER_CARDS_DIG_OUTPUT);
		if(iemeDigCard > NUMBER_CARDS_DIG_OUTPUT) AfxMessageBox(strCards, MB_ICONERROR);
		return m_DigBufferNiMatrix[iemeDigCard - 1];};
	CNiReal64Matrix& GetAnaBufferNiMatrix_Card(int iemeAnaCard){CString strCards; strCards.Format("There is only %d Digital Cards", NUMBER_CARDS_ANA_OUTPUT);
		if(iemeAnaCard > NUMBER_CARDS_ANA_OUTPUT) AfxMessageBox(strCards, MB_ICONERROR);
		return m_AnaBufferNiMatrix[iemeAnaCard - 1];};
private:
	// Toutes les données de construction
	// ie: les données nécéssaire à la construction d'une séquence from scratch
	// de manière flexible et automatisée
	// on va aussi pouvoir créer (ou détruire) les CParametre qui qualifient une séquence, pour pouvoir les faire varier...
	CString*		m_Constr_Name; // Nom de chaque Colonne.
	int*			m_Constr_Duration; // durée de chaque Colonne. Si négatif: Colonne pas Activée.
	int*			m_Constr_Duration_µs; // durée de chaque Colonne en µs. Si négatif: Colonne pas Activée.
    unsigned long*	m_Constr_Dig32bit; // Le 32bit représantant les sorties digitales .
	int*			m_Constr_ColStartingGroupe; // la colonne commence t-elle un groupe de colonne donné
	unsigned long*	m_Constr_Dig32ContiWithPrevious;// Chaque Dig continue t-elle la précédente: le Dig32Bit dit oui ou non bit a bit
	double*			m_Constr_AnaDebut[NUMBER_ANA_OUTPUT];	// la valeur de début des WaveForme de chaque AnaOut de chaque Colonne.
	double*			m_Constr_AnaFin[NUMBER_ANA_OUTPUT];	// la valeur de fin   des WaveForme de chaque AnaOut de chaque Colonne.
	int*			m_Constr_AnaContiWithPrevious[NUMBER_ANA_OUTPUT];	// Chaque Ana continue t-elle la précédente: non, conti, contiunuité de la pente,... ?

	// Actualsie le CtrlList pour l'affichage de donnée de Colonne et de ligne dans une boit de Dialog type m_SewView
	void ActualiseCtrlListCol(CDC& dc);
	void ActualiseCtrlListLigneDig(CDC& dc);
	void ActualiseCtrlListLigneAna(CDC& dc);
	// Ajoute une colonne dans les m_Constr_Param**
	void AddColonne_Constr_Param(int AfterWhichCol_0Debut);
	// Fonction qui va actualiser les m_Constr_param, à partir de la séquence actuelle...
	void Make_Constr_Param();
	// crée les CParametre relatifs à la séquence
	// ie les parmetres qui pourront varier, est qui définnissent la séquence
	int MakeCParametrantFrom_Sequence_AndConstrParam();
	// puis on certain Cparametre redondant
	// en accord avec les infos de type continuité, continuité de pente, etc...
	int MakeNotRedondant_Constr_param();
	// Rerouve le CParametre qui définit la valeur (,) de manière univoque
	// utile pour les enchainements de ContiAGauche et Constance
    CParametre* GetCParamMostLeftThatDefineThisValue(int iemeCol, int indexAna, int Debut0_Fin1);
	// Apres avoir définit les valeurs Cparametré de la séquence, on actualise celle relatives
	// au SlopeContinuité et autre propriété grace aux CParametre qui les entourent et les definissent
	int ActualiseSequenceFromCParametrant();
	// Appelé par ActualiseSequenceFromCParametrant pour traiter spécifiquement les link dus
	// au SlopeContinuité.
	int ActualiseSequenceFromLinkedSlopeConti();
	// trouve le ieme cluster de slope conti, ou autre propriété étendues sur plusieur colonne
	// Renvoye CRect: left=PremiereColCluster, right=DerniereColCluster, top=TypeProprieteEtendue, bottom=IlExisteEncoreDesCluster
	// si il ne le trouve pas alors CRect(0,0,0,0)
	CRect FindCluster_IemeAna(int indexAna, int iemeCluster);
	// Construit la séquence à partir des m_Constr_param
	void MakeSequenceFrom_Constr_Param();
public:
	// Get le parametre de continuité pour la IndexDig de la iemeCol
	bool GetDigContiWithPrevious(int iemeCol, int IndexDig);
	// Get le parametre de continuité pour la IndexAna de la iemeCol
	int GetAnaContiWithPrevious(int iemeCol, int IndexDig);
	// Set le parametre de continuité pour la IndexDig de la iemeCol
	// renvoye le CRect a redessiner dans une eventuelle vue
	CRect SetDigContiWithPrevious(int iemeCol, int IndexDig, bool state);
	// Set le parametre de continuité pour la IndexAna de la iemeCol
	// renvoye le CRect a redessiner dans une eventuelle vue
	CRect SetAnaContiWithPrevious(int iemeCol, int IndexAna, int typeConti);

public:
	// opérateur =
	void operator =(Sequence& UneSequence);
	// quelques méthodes pour accéder aux variables membres
	// Renvoye le nombre de colonnes de la séqeunce
	int GetnColonnes(){return m_nColonnes;};						
	// Renvoye le nombre de périodes de la séquence
	unsigned int CalculateNbrPeriodes();				
	// Renvoye un pointeur vers le buffer digital
	unsigned long* GetDigBuffer();						
	// Renvoye un pointeur vers les buffers analogiques
	double** GetAnaBuffer();				

	// quelques méthodes pour travailler sur les colonnes de la séquence
	// supprime toutes les colonnes
	void ClearColonneList(bool DestroyAllWF = false);	
	// Insert une Colonne en fonction de Where_0Debut_m1Fin Copiée depuis pUneColCopie si fournie
	// de 0 à m_nColonnes, on insert la colonne avant where
	// si == m_nColonnes, on insert la colonne apres la dernière
	// -1 --> à la fin, -2 avant la derniere, etc...
	// Renvoye le pointeur vers la Colonne créee
	Colonne* InsertNewColonne_CopiedFromCol(int Where_0Debut_m1Fin, Colonne* pUneColCopie, CString UnNewNom = "", unsigned int UnNewNombrePeriode = 0);	
	// Insert une Colonne en fonction de Where_0Debut_m1Fin
	// de 0 à m_nColonnes, on insert la colonne avant where
	// si == m_nColonnes, on insert la colonne apres la dernière
	// -1 --> à la fin, -2 avant la derniere, etc...
	// Renvoye le pointeur vers la Colonne créee
	Colonne* InsertNewColonne(int Where_0Debut_m1Fin, CString UnNewNom, unsigned int UnNewNombrePeriode = 0);

	// Assigne a la indexAna de la IemeCol la pente definie par AnaDebut et AnaFin et Renvoye le nombre de WaveForme Créées pour l'occasion
	unsigned int SetCol_1Analog(int IemeCol, unsigned char indexAnaOut, double AnaDebut, double AnaFin)
	{if(!GetColonne(IemeCol))return 0; return GetColonne(IemeCol)->Set1Analog(indexAnaOut, AnaDebut, AnaFin);};
	// Assigne a la IemeCol les pentes definies par Debut et Fin  de la voie analogique Which	// on peut passer jusquèà 32 parametres
	// les Analogiques dont on ne spécifie pas continuent la colonne précédente
	// Renvoye le nombre de WaveForme Créées pour l'occasion
	unsigned int SetCol_Analogs_Which_Debut_Fin(int IemeCol, WhichDebFin_32_ParamH)
	{if(!GetColonne(IemeCol))return 0; return GetColonne(IemeCol)->SetAnalogs_Which_Debut_Fin(WhichDebFin_32_ParamF);};
	// Assigne a la IemeCol des WaveForm qui continuent la colonne précédente
	// Renvoye le nombre de WaveForme Créées pour l'occasion
	unsigned int SetCol_Analogs_Continue(int IemeCol)
	{return SetCol_Analogs_Which_Debut_Fin(IemeCol);};
	// met la sortie digitale indexDigOut de la IemeCol à la valeur TheState, ou 0 par defaut. Renvoye la somme de tous les bit : compris entre 0 et 32
	unsigned char SetCol_1Digital(int IemeCol,  int indexDigOut, int TheState)
	{if(!GetColonne(IemeCol))return 0; return GetColonne(IemeCol)->Set1Digital(indexDigOut, TheState);};
	// Assigne a la IemeCol les chanel 0 à 31 avec un apriori de 0 pour les valeurs non données et Renvoye le nombre corespondant à la représentation 32 bit
	unsigned long SetCol_32Digitals(int IemeCol, Set32DigParamH)
	{if(!GetColonne(IemeCol))return 0; return GetColonne(IemeCol)->Set32Digital(Set32DigParamF);};
	// Assigne a la IemeCol les valeurs. On peut passer jusqu'à 32 parametres
	// les Digitales dont on ne spécifie pas la valeur continuent la colonne précédente
	unsigned int SetCol_Digitals_DefaultPrecedentColonne(int IemeCol, WhichDigValH)
	{if(!GetColonne(IemeCol))return 0; return GetColonne(IemeCol)->SetDigitals_DefaultPrecedentColonne(WhichDigValF);};
	// Assigne a la indexAnaOut, une WaveForm à partir d'une fonction(x) compilée
	// ou d'une CString "Formule(x)" 
	// ou d'une CString "Adresse d'un fichier contenant une formule(x)"
	// on précise la plage FromX->ToX sur laquelle la formule est utilisée
	// ainsi que le nombres de points à définir (plus il y a de points, plus une courbe sera bien définie !)
	void SetCol_1Analog_FromFunction_ofX(int IemeCol, unsigned char indexAnaOut, double Fy_ofx(double x), double FromX, double ToX, int NbrPoints, double WarpFirstPointTo = 0)
	{if(!GetColonne(IemeCol))return; GetColonne(IemeCol)->Set1AnalogFromFunction_ofX(indexAnaOut, Fy_ofx, FromX, ToX, NbrPoints, WarpFirstPointTo);};
	void SetCol_1Analog_FromFunction_ofX(int IemeCol, unsigned char indexAnaOut, CString Formula_Fofx, double FromX, double ToX, int NbrPoints, double WarpFirstPointTo = 0)
	{if(!GetColonne(IemeCol))return; GetColonne(IemeCol)->Set1AnalogFromFunction_ofX(indexAnaOut, Formula_Fofx, FromX, ToX, NbrPoints, WarpFirstPointTo);};
	// Assigne a la indexAnaOut, une WaveForm à partir des valeurs d'une autre sortie analogique via une f(y_autre) compilée
	// ou d'une CString "Formule(x)" 
	// ou d'une CString "Adresse d'un fichier contenant une formule(x)"
	// on précise le nombres de points à définir (plus il y a de points, plus une courbe sera bien définie !)
	void SetCol_1Analog_FromFunction_ofOtherAnalog(int IemeCol, unsigned char indexAnaSlave, unsigned char indexAnaMaster, double Fy_ofx(double x), int NbrPoints)
	{if(!GetColonne(IemeCol))return; GetColonne(IemeCol)->Set1AnalogFromFunction_ofOtherAnalog(indexAnaSlave, indexAnaMaster, Fy_ofx, NbrPoints);};
	void SetCol_1Analog_FromFunction_ofOtherAnalog(int IemeCol, unsigned char indexAnaSlave, unsigned char indexAnaMaster, CString Formula_Fofx, int NbrPoints)
	{if(!GetColonne(IemeCol))return; GetColonne(IemeCol)->Set1AnalogFromFunction_ofOtherAnalog(indexAnaSlave, indexAnaMaster, Formula_Fofx, NbrPoints);};

	// à partir de maintenant, toute colonne ajoutée, se verra appliquer entre indexAnaSlave et indexAnaMaster
	// la relation définie par f(y_autre) compilée
	// ou d'une CString "Formule(x)" 
	// ou d'une CString "Adresse d'un fichier contenant une formule(x)"
	// on précise le nombres de points à définir (plus il y a de points, plus une courbe sera bien définie !) 
	void SetFromNowOn_RelationBetween_2Analogs(unsigned char indexAnaSlave, unsigned char indexAnaMaster, CString Formula_Fofx, int NbrPoints);

	// Split la iemeCol en deux, en affectant au plus NbrPeriodeFirstPart à la première
	// et en ajustant toute les WaveForme pour garder des pente comme il faut !
	void SplitColonne(int iemeCol, int NbrPeriodeFirstPart);
	// Split la Séquence, un NbrPeriodeBefore précédent la fin de la iemeCol 
	// et Set les digitals spécifiées aux valeurs spécifiées depuis l'incision, jusqu'avant la iemeCol
	// renvoye un double qui donne la Colonne coupée, et à quelle proportion, sous la forme
	// "indexCol, proportion" : 1,25 === colonne coupée: 2eme(index 1), à un quart de sa longueur
	double SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals(int iemeCol, int NbrPeriodeBefore, WhichDigValH);
	// Coupe la séquence un nombre PlusNbrPeriodeAfter après le début de la FromIemeCol (ou de la premiere colonne dont le nom contient PartNameFromCol)
	// et rend non Actif (pour la séquence) tous ce qui se trouve entre cette coupure
	// et la le UpToIemeCol, ou la fin si on ne précise pas (colonne 0 = fin + 1 = derniere periode de la colonne de fin)
	// (ou jusquà la premiere colonne dont le nom contient PartNameUpToCol)
	int DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToEndOfCol(int FromBeginOfCol, int PlusNbrPeriodeAfter, int UpToEndOfCol = -1);
	int DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToEndOfCol(CString PartNameFromCol, int PlusNbrPeriodeAfter, CString PartNameUpToCol = "");
	// pareil, mais UpToBeginOfCol
	int DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToBeginOfCol(CString PartNameFromCol, int PlusNbrPeriodeAfter, CString PartNameUpToCol = "");
	// Retire une colonne de la séquence
	// 1 = premiere, etc...,  -1 = la derniere, -2 = l'avant derniere, etc...
	void RemoveColonne(int iemeCol);	
	// set la durée de la iemeCol Colonne de la séquence
	// 1 = premiere, etc...,  -1 = la derniere, -2 = l'avant derniere, etc...
	// Renvoye le nombre de periodes de la séquence
	int SetColonneDuration(int iemeCol, int nPeriodes);
	// Renvoye l'index de la colonne* dans la sequence
	int GetColNumero(Colonne* pLaCol);
	// Renvoye l'index de la premiere colonne dont le nom contient PartNameCol
	int GetColNumero(CString & PartNameCol);
	// Renvoye un pointeur vers la iemeCol Colonne de la séquence
	// 1 = premiere, etc...,  -1 = la derniere, -2 = l'avant derniere, etc...
	Colonne* GetColonne(int iemeCol);	
	// Renvoye la durée de la iemeCol Colonne de la séquence
	// 1 = premiere, etc...,  -1 = la derniere, -2 = l'avant derniere, etc...
	int GetColDuration(int iemeCol);
	// Renvoye la durée de la partie Activée de la iemeCol Colonne de la séquence
	// La durée active est la duree qui sera effectivement envoyée vers les cartes
	// 1 = premiere, etc...,  -1 = la derniere, -2 = l'avant derniere, etc...
	int GetColActiveDuration(int iemeCol);
	// rend actif toute la colonne si on ne précise rien, 
	// désactive toute la colonne si NbrPeriodeActive = 0, sinon, on active juste une partie
	// renvoye le nombre de periodes effectivement active
	int ActivateColonne(int iemeCol, int Aactiver = -1);
	// Renvoye le numero de la colonne pointée : 1 = premiere, etc...
	int PointedIemeColonne(CPoint point);
	// Renvoye le pointeur vers la Colonne "cliquée" dans le CDC
	Colonne* PointedColonne(CPoint point);
	// Renvoye l'index de la sortie Digitale et iemeCol "cliquée" dans le CDC
	CPoint Pointed_DigIndexY_IemeColX(CPoint point);
	// Renvoye l'index de la sortie Analogique "cliquée" dans le CDC
	CPoint Pointed_AnaIndexY_IemeColX(CPoint point);
	// Renvoye l'index de la sortie en y
	// et en x, le Gauche Milieu Droite....
	CPoint Pointed_AnaIndexY_PosWfX(CPoint point);
	// Renvoye l'adresse du param "cliqué" dans le CDC de la vue 1
	void* PointedParamSequenceView1(CPoint point);
	// Renvoye l'adresse du param "cliqué" dans le CDC de la vue 2
	void* PointedParamSequenceView2(CPoint point);
	// Switch une sortie digitale de la sequence en fonction du point ou on a cliquer dans le CDC, true si ca marche
	CRect ClickedDigitalToSwitch(CPoint point);
	// Set une sortie analogue de la sequence en fonction du point ou on a cliquer dans le CDC, true si ca marche
	CRect ClickedAnalogToSet(CPoint point);

	// dessine la sortie digitale 
	CRect Dessine_1_Digital(CDC& dc, int iemeCol, int indexDig, int IndexColor = -1);
	// Dessine la case de iemeCol (1 based), et d'index (0 based)
	CRect Dessine_1_Analog(CDC& dc, int iemeCol, int indexAna, int IndexColor = -1, int grisee = 0);
	// Dessine la sequence dans le CDC
	// Renvoye le CRect dans lequel tout a été dessiné
	// 1: sortie digitales
	// 2: sorties analogiques
	CRect DessineSeqView1(CDC& dc);
	CRect DessineSeqView2(CDC& dc);
	// renvoye le ratio de la duree d'une colonne quand celle-ci fait partie d'une colonne splitée, re-splitée, re-re-splitée, etc
	double GetRatioDureeColonneDansUnSplit(int iemeCol);
	// renvoye le rectangle correspondant a la (iemeCol, indexDig) pour le dessin
	CRect GetRectDigOut(int iemeCol, int indexDig);
	// renvoye le rectangle correspondant a la (iemeCol, indexAna) pour le dessin
	CRect GetRectAnaOut(int iemeCol, int indexAna);
	
	static int m_ColWidth; // largeur d'une colonne
	static int m_DigHeight; // hauteur d'une ligne Digitale
	static int m_AnaHeight; // hauteur d'une ligne analogique
	static int m_EcartHoriz;
	static int m_EcartVert; 
	static CPoint m_FirstDigTopLeft; // Point TopLeft de la toute premiere case digitale
	static CPoint m_FirstAnaTopLeft; // Point TopLeft de la toute premiere case Analogique
	static int m_NbrDisplayDigChan;// nombre de dig afficher par colonne
	static int m_NbrDisplayAnaChan;// nombre de Ana afficher par colonne

	static int m_AnaMax_mV; // valeur max d'une case analogique
	static int m_AnaMin_mV; // valeur min d'une case analogique

	int LButtonDownOnSeq(UINT nFlags, CPoint point);
	int MouseMoveOnSeq(UINT nFlags, CPoint point);
	int RButtonDownOnSeq(UINT nFlags, CPoint point);
	int RButtonUpOnSeq(UINT nFlags, CPoint point);
	int MouseWheelOnSeq(UINT nFlags, short zDelta, CPoint pt);

	// construit le buffer digital à partir des données de la séquence
	// renvoye le nombre de periodes de la sequence (comptant à partir de 0)
	int MakeDigBuffer();	
	// construit les NUMBER_ANA_OUTPUT WaveForme qui définnissent toute la séquence
	// renvoye le nombre de points total
	int MakeWholeSeqWaveForme();
	// construit le buffer analogique à partir des données de la séquence
	// renvoye le nombre de periode de la sequence (comptant à partir de 0)
	int MakeAnaBuffer();

	// detruit le buffer digital à partir des données de la séquence
	void ClearDigBuffer();				
	// " " " "
	void ClearAnaBuffer();	
};


/*
Donne le ieme (de 0 à 31) bit d'un unsigned long
*/
bool GetIemeBitFromInt32Bit(unsigned char iemeBit, unsigned long Lint32Bit);
// Set le ieme bit d'un unsigned long, renvoye la valeur précédente
bool SetIemeBitInsideInt32Bit(unsigned char iemeBit, unsigned long & Lint32Bit, bool NewState);
// Utile pour calculer des points intermediaires dans la séquence, comme pour la compression
// Calcul la valeur sur une pente (qui va de From à To, sur un longueur Length)
// à une distance cu du debut de la pente.
double IntermediateValue(double From, double To, double Length, double Cut);
double IntermediateTime(double From, double To, double Length, double ValeurInter);
void SpecialMonster_With_2_PhotoDiode(double LaserFrom, double LaserTo, double LaserTimeTotal, double LeSeuil_Ph1_Ph2, double LeMultipl_Ph2
									  , bool& IlFaut2Colonnes_Ph1_Ph2, bool& LaPremiereColonneEstEnPh2
									  , double& duree_Col1, double& duree_Col2
									  , double& Col1_pHd1_From, double& Col1_pHd1_To
									  , double& Col2_pHd1_From, double& Col2_pHd1_To
									  , double& Col1_pHd2_From, double& Col1_pHd2_To
									  , double& Col2_pHd2_From, double& Col2_pHd2_To);

#define CONVERT_INT32_IN_LIST_BOOL(lint) \
	GetIemeBitFromInt32Bit(0, lint), GetIemeBitFromInt32Bit(1, lint), GetIemeBitFromInt32Bit(2, lint), GetIemeBitFromInt32Bit(3, lint), GetIemeBitFromInt32Bit(4, lint), GetIemeBitFromInt32Bit(5, lint), GetIemeBitFromInt32Bit(6, lint), GetIemeBitFromInt32Bit(7, lint), \
	GetIemeBitFromInt32Bit(8, lint), GetIemeBitFromInt32Bit(9, lint), GetIemeBitFromInt32Bit(10, lint), GetIemeBitFromInt32Bit(11, lint), GetIemeBitFromInt32Bit(12, lint), GetIemeBitFromInt32Bit(13, lint), GetIemeBitFromInt32Bit(14, lint), GetIemeBitFromInt32Bit(15, lint), \
	GetIemeBitFromInt32Bit(16, lint), GetIemeBitFromInt32Bit(17, lint), GetIemeBitFromInt32Bit(18, lint), GetIemeBitFromInt32Bit(19, lint), GetIemeBitFromInt32Bit(20, lint), GetIemeBitFromInt32Bit(21, lint), GetIemeBitFromInt32Bit(22, lint), GetIemeBitFromInt32Bit(23, lint), \
	GetIemeBitFromInt32Bit(24, lint), GetIemeBitFromInt32Bit(25, lint), GetIemeBitFromInt32Bit(26, lint), GetIemeBitFromInt32Bit(27, lint), GetIemeBitFromInt32Bit(28, lint), GetIemeBitFromInt32Bit(29, lint), GetIemeBitFromInt32Bit(30, lint), GetIemeBitFromInt32Bit(31, lint)

#define CONVERT_TABLEAU_DEBUT_FIN_IN_LIST(adresseIntDebut, adresseIntFin) \
	adresseIntDebut[0], adresseIntFin[0], \
	adresseIntDebut[1], adresseIntFin[1], \
	adresseIntDebut[2], adresseIntFin[2], \
	adresseIntDebut[3], adresseIntFin[3], \
	adresseIntDebut[4], adresseIntFin[4], \
	adresseIntDebut[5], adresseIntFin[5], \
	adresseIntDebut[6], adresseIntFin[6], \
	adresseIntDebut[7], adresseIntFin[7]