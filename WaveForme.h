#pragma once
#include "SettingSave.h"

#define WF_DEFAULT_MAX_ABSCISSE 1.0

#define FOR_ALL_POINTS_do	 	    POSITION pos = m_Liste_Points.GetHeadPosition();	\
									int indexLePoint = -1;							\
									while(pos)										\
									{												\
									indexLePoint++;									\
									CPointDbl& LePoint = m_Liste_Points.GetNext(pos);	\

#define FOR_ALL_POINTS_in_Wf_do(Wf)	POSITION pos = (Wf).m_Liste_Points.GetHeadPosition();	\
										int indexLePoint = -1;								\
										while(pos)											\
										{													\
										indexLePoint++;										\
										CPointDbl& LePoint = (Wf).m_Liste_Points.GetNext(pos);	\

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////      CPointDbl        ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
class CPointDouble
{
public:
	CPointDouble(void){x=0.0;y=0.0;};
	CPointDouble(double xx, double yy){x=xx;y=yy;};
	CPointDouble operator= (const CPointDouble pd){x=pd.x;y=pd.y; return pd;};
	bool operator==(const CPointDouble pd)const
	{	if(x==pd.x && y==pd.y) 	return true;
		else return false;	};
	bool operator!=(const CPointDouble pd)const
	{	if(x!=pd.x || y!=pd.y) 	return true; 
		else return false;	};
public:
	double x;
	double y;
};
#define CPointDbl CPointDouble
///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////      CWaveForme       ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef CList <CPointDbl,CPointDbl> CList_Point;// CList : pratique pour ranger des objet de maniere dynamique et efficace
class CWaveForme;
typedef CList <CWaveForme*,CWaveForme*> CList_WF;// CList : pratique pour ranger des objet de maniere dynamique et efficace
enum{WfGriseeGauche = 1, WfGriseeMilieu = 2, WfGriseeDroite = 4};
enum{ WF_DESSINNE_CADRE			= 1
	, WF_VALGAUCHE_GRISEE		= 2
	, WF_VALDROITE_GRISEE		= 4
	, WF_VALGAUCHE_VISIBLE		= 8
	, WF_VALDROITE_VISIBLE		= 16
	, WF_TRAITGAUCHE_VISIBLE	= 32
	, WF_TRAITGAUCHE_GRISEE		= 64
	, WF_FOND_GRISEE			= 128
	, WF_FONDGAUCHE_GRISEE		= 256
	, WF_FONDMILIEU_GRISEE		= 512
	, WF_FONDDROITE_GRISEE		= 1024	};
class CWaveForme
{
public:
	CWaveForme(void);						// constructeur
	CWaveForme(double ValDebut, double ValFin);// constructeur
	CWaveForme(double ValConst);		// constructeur
	CWaveForme(const CWaveForme &UneWF);	// constructeur
	~CWaveForme(void);

    // Membre static : une liste pour répertorier toutes les WF créées : pratique pour tout nétoyer à la fin
	static CList_WF Liste_WaveFormes;

private:

	// La liste qui contient les points de la Wave-Forme
	CList_Point m_Liste_Points;

	// Un point est repéré par sa position sur un axe arbitraire

	// une Wf a un moyen de se sauver:
	CSettingSave m_SaveWf;
	// Pour construire (ou sauver) les Points a partir de (ou dans) un ficher
	int m_nPoints;
	double* m_ConstrWf_X;
	double* m_ConstrWf_Y;

//	// le dernier rectangle dans lequel cette WF a été dessiné
//	CRect m_LastRectDessinLaWf;

// quelques méthodes pour travailler sur les Points de la Wf
// et accéder aux variables membres
public:
	// supprime tous les points de la WF
	void ClearPointList();
	// Ajoute un point à la WF. TimeArbUnit est sur un axe arbitraire de 0 à ...
	// Si on précise SiDoubletAtEnd_NajoutePas = true, le point n'est pas ajouté si il y est déjà à la fin = GetTail()
	// renvoye true si le point est ajouté
	bool AddPoint(double TimeArbUnit, double Val, bool SiDoubletAtEnd_NajoutePas = false);
	// Ajoute la WaveFormeToAppend à la suite de celle-ci, en lui donnant la ScaleDureeToAppend
	// renvoye le nombre de points ajoutés
	int AddWaveFormeAtEnd(CWaveForme & WaveFormeToAppend, double ScaleDureeToAppend, double ScaleOrdonnee = 1.0);
	// Supprime le point numéro indexP
	void RemovePoint(int indexP);	
	// Supprime le point en question
	void RemovePoint(CPointDbl PointAEnlever);	
	// change la position du indexP
	void MovePoint(int indexP, double NewAbscisse, double NewOrdonnee);
	// Ré-arrnge la m_Liste_Points en fonction des abscisses (par switchage)
	// Si on fournit pIndexPointActualise, Actualise la nouvelle position de pIndexPointActualise
	void SortListePoints(int* pIndexPointActualise = NULL);
	// Renvoye le nombre de Point de la WF
	int GetnPoints(){
		m_nPoints = int(m_Liste_Points.GetCount()); 
		return m_nPoints;	};
	// Renvoye la longueur de la WF
	double GetLength(){	if(m_nPoints >= 2) return GetPoint(-1).x - GetPoint(0).x; else return 0.0;	};
	// Renormalise La longueur à NewTotalLength et renvoye OldTotalLength
	double NormaliseLength(double NewTotalLength)
	{	double length = GetLength();	double Multiplicateur = NewTotalLength / length;
		for(int i = 0; i < GetnPoints(); i++)
		{	CPointDbl& LeiemePoint = GetPoint(i);
			MovePoint(i, LeiemePoint.x * Multiplicateur, LeiemePoint.y);	}
		return length;	};
	// Renvoye la "POSITION" du indexP point de la WF
	POSITION GetPointPosList(int indexPoint);
	// Renvoye une copie du point de la WF (0 based index) (negatifs = depuis la fin)
	CPointDbl GetPoint(int indexP);
	// Trouve l'index du premier point précédent ou égale à l'Abscisse
	// -1 : aucun
	int Find_PreceedingPoint(double Abscisse){	for(int i = 0; i < GetnPoints(); i++){	if(GetPoint(i).x <= Abscisse) continue;	return i - 1;}	return GetnPoints() - 1;	};
	// Trouve l'index du premier point suivant ou égale à l'Abscisse
	// -1 : aucun
	int Find_FolowingPoint(double Abscisse){	for(int i = 0; i < GetnPoints(); i++){	if(GetPoint(i).x >= Abscisse) return i;	}				return -1;	};
	// split une Wf à une certaine proportion (0 à 1) de son abscisse, et met la deuxieme partie dans new_SecondePart dont le pointeur est renvoyé
	CWaveForme* SplitReturn_new_SecondePart(double AtProportionAbscisse); 
	// Trouve le premier point où la Wf est strictement sous LimitValue
	// -1 : aucun
	int Find_FirstPoint_BelowValueStrict(double LimitValue)
	{	for(int i = 0; i < GetnPoints(); i++)
		{	if(GetPoint(i).y < LimitValue)
				return i;	}
		return -1;	};
	// Trouve le premier point où la Wf est égale ou supérieur à LimitValue
	// -1 : aucun
	int Find_FirstPoint_AboveValueincluding(double LimitValue)
	{	for(int i = 0; i < GetnPoints(); i++)
		{	if(GetPoint(i).y >= LimitValue)
				return i;	}
		return -1;	};

	// interpolation d'un point quelque part dans la WaveForme
	double InterpolateLinear(double Abscisse);
	// Remplit une NiMatrix SurQuelleLigne et à partir de la colonne ApartirDeQuelEchantillon, et avec un pas de temps timeStep
	void InterpolateLinearFillNiMatrix(CNiReal64Matrix & LaMatrix, double timeStep, int SurQuelleLigne, int ApartirDeQuelEchantillon = 0);

	// Dessine la WaveForme dans le CRect
	// grisée 1 WfGriseeGauche + 2 WfGriseeMilieu + 4 WfGriseeDroite
	void DessineWF(CDC& dc, CRect Circonscrit, int OptionsDessin = WF_VALGAUCHE_VISIBLE + WF_VALDROITE_VISIBLE + WF_TRAITGAUCHE_VISIBLE, int IndexColor = -1, CPointDbl Rect_MinMax = CPointDbl(-10000.0,10000.0));

	// renvoye la formule contenue dans le fichier ou le "" si le fichier n'existe pas
	CString GetFormula_FromFile(CString filename);

	// fabrique les points de la wave forme suivant une fonction de ses coordonnées x : y = F(x)
	// on précise FromX et ToX, ainsi que le nombre de points de défintion de cette fonction (plus il y a de points, plus une courbe sera bien définie !)
	// si on spécifie WarpFirstPointTo , tout est multiplié par le bon facteur pour que le premier point soit d'ordonnée cette valeur
//srybex 	void SetFromFunction_ofX(double FromX, double ToX, int NbrPoints, double Fy_ofx(double x), double WarpFirstPointTo = 0);
//srybex 	void SetFromFunction_ofX(double FromX, double ToX, int NbrPoints, CString Formula_Fofx, double WarpFirstPointTo = 0);
	// fabrique les points de la wave forme suivant une fonction d'une autre WaveForm : y = F(y_autreWf)
	// on précise le nombre de points de défintion de cette fonction (plus il y a de points, plus une courbe sera bien définie !)
//srybex 	void SetFromFunction_ofOtherWfY(CWaveForme* pOtherWf, int NbrPoints, double Fy_ofy(double y_other));
//srybex 	void SetFromFunction_ofOtherWfY(CWaveForme* pOtherWf, int NbrPoints, CString Formula_Fofx);

	void SetGraphically();
	void MakeCParametreWf();
	void LoadFromFileWf(CString filename);
	void SaveToFileWf(CString filename);
	// trouve tous les fichier WF dans un Dossier, et rempli une eventuelle Combo
	static CString CWaveForme::FindAllWaveFormesInAFolder(CString FolderName, CComboBox* pCombo = NULL);
};