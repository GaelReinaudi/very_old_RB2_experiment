#pragma once

//srybex #include "SerieImage.h"
//#include "WaveForme.h"
//#include "math.h"
//#include <bcam.h>

#define MAX_PARAM_FIT 16
#define VALEUR_COURANTE -123.456
#define PARAM_EN_PLUS_POSSIBLE 100
using namespace std;
//srybex using namespace Bcam;

enum{FitRect, FitLine};

void fUneGaussienne(float xy, float a[], float *y, float dyda[], int na);
void fUneDoubleGaussienne(float xy, float a[], float *y, float dyda[], int na);
void fUneGaussienne2D(float x, float a[], float *y, float dyda[], int na);
void fUneGaussienne2DTilt(float x, float a[], float *y, float dyda[], int na);
void fUneGaussienne2DTigeGauss(float x, float a[], float *y, float dyda[], int na);
void fUneGaussienne2DTigeDoubleGauss(float x, float a[], float *y, float dyda[], int na);
void fUne2DCondensat(float x, float a[], float *y, float dyda[], int na);

void mrqmin(float x[], float y[], float sig[], int ndata, float a[], int ia[],
			int ma, float **covar, float **alpha, float *chisq,
			void (*fonc)(float, float [], float *, float [], int), float *alamda);

class CFitting
{
public:
	CFitting(void);
	CFitting(CString NomDuFit, bool Is1D, void(*TheFonc)(float, float [], float *, float [], int)
		, int npar, CString* pDesNoms, float* pTheParamGuess, float* pTheCabbage, CNiNumEdit* pNiEditParam = NULL, CNiNumEdit* pNiEditLambda = NULL, CNiNumEdit* pNiEditChi2 = NULL, CNiSlide* pNiSlide = NULL);
	~CFitting(void);

private:
	// des parametres communs a tous les Fit
public:
	// Nom du fit. ex: Gaussian 2D with Tilt Angle 
	CString m_Nom;
	// la taille d'un pixel en mm
	static double m_PixelSizeFit_µm;
private:
	static bool m_CheckCabbages;
	static bool m_DiscaredSaturatedPoints;
	static bool m_FitFromPrevious;
	static int m_StopWhenStable;
private:
	// Les controls qu'il faut actualiser;
    CNiNumEdit* m_pNiEditStart;
	CNiNumEdit* m_pNiEditLambda;
	CNiNumEdit* m_pNiEditChi2;
	CNiSlide* m_pNiSlide;
	// Indique si on fit une ligne(1D), ou un tableau 2D, 
	// Utile pour savoir quelle partie du Viseur nous interresse pour le fit (ligne centrale, ou bien tout le viseur)
	bool m_IsFit1D;
	// nombre de points qui seront fittés
	int m_nPoints;
	long m_nPtX;
	long m_nPtY;
	// Le Rectangle qui correspond à la zone de l'image dans le contexte d'affichage
	CRect m_ZoneToFit;
	// nombre de ParamAjustables
	long m_nParam;
	// le nombre d'itération par fit
	static int m_NbrIter ;

	// Valeur maximale pouvant etre prise par un pixel de l'image. ie: La valeur du blanc (image 8bit:255, image 12bit:4095)
	long m_MaxWhiteValue;
	double m_SommePixelZone;

	// matrice des sigma qui est en fait un vecteur : matrix(1,1,1,m_nPoints);
	float** m_pSigma;
	// matrice des covariances matrix(1,m_nParam,1,m_nParam);
	float** m_pCovar;
	// matrice des Alpha matrix(1,m_nParam,1,m_nParam);
	float** m_pAlpha;
	// La fonction à utiliser pour le Fit
	void (*m_LaFonction)(float, float [], float *, float [], int);

	float m_ChiCarre;
	float m_Lambda;
	float m_YFitted;

	// les X des données à fitter
	float** m_pDataX;
	// les Y des données à fitter
	float** m_pDataY;
	// les Y des données fittée
	float** m_pFitY;

	// Noms des parametres ajustables
	CString m_pStringNomParams[MAX_PARAM_FIT+1];
	// Les Unités des grandeurs
	CString m_pStringUnites[MAX_PARAM_FIT+1];
	// Les Nom des Echelles relatives pour aider le fit a trouver de bon guess : 1, PTX, ou PTY
	CString m_pStringEchelleFit[MAX_PARAM_FIT+1];
	// tableau des Parametres à ajuster pour le Fit
	float m_pParamAjustables[MAX_PARAM_FIT+1];
	// Les Resultats des Fit dans leurs unités
	float m_pParamEnUnites[MAX_PARAM_FIT+1];
	// Les Multiplicateur a appliquer pour passer des donnée du fit au parametre dans les bonnes unités
	float m_pMultiplicateurPourUnites[MAX_PARAM_FIT+1];
	// tableau de int qui indique si un ParamAjustables doit etre ajusté ou non
	int m_pDoitAjuster[MAX_PARAM_FIT+1];
	// Guess des parametres ajustables. sera multiplié par "multiplicateur" = 1, nPointX, ou nPointY
	float m_pParamGuess[MAX_PARAM_FIT+1];
	// Les parametre au dela desquels, on considere etre dans les cabbage
	float m_pCabbageDown[MAX_PARAM_FIT+1];
	float m_pCabbageUp[MAX_PARAM_FIT+1];

public:
	// pointeur vers des param en plus a ecrire dans le fichier de save des resultats du fit
	static CString* m_pParamEnPlus_Nom[PARAM_EN_PLUS_POSSIBLE];
	static double* m_pParamEnPlus_Val[PARAM_EN_PLUS_POSSIBLE];
	static double* m_pParamEnPlus_Moy[PARAM_EN_PLUS_POSSIBLE];
	static double* m_pParamEnPlus_StD[PARAM_EN_PLUS_POSSIBLE];
	static int m_pParamEnPlus_Combien;

private:
	// fait les m_pMultiplicateurPourUnites[j]
	void MakeMultiplicateurPourUnites();
	// prepare l'utilisation d'une instance de Fitting : Zone a fiter, nbr points, etc...
	void PrepareForUse(CRect Zone);
	// crée les matrix nécessaires a numerical recipes
	void CreateNrMatrix();
	// Commence l'iteration jusqu'a convergence ou probleme
	int StartIter(CDC* pUnDC = NULL);
	// renvoye non 0 si le critere de fit stable est rempli
	int CritereStopFit();
	// Actualise les dialog control dont l'adresse a ete passé l'ors de la création de l'instance de CFitting
	void ActualiseDialogControls(int niemeIter, int Status = -1);
	// Stop le Fitting process
	void Stop();
	// libère la memoire
	void Nettoye();
	// le fit est il dans les cabbages? Renvoye le numero du premier param dans les cabbages
	int InTheCabbage();
public:
	// lance un fit de la Zone a partir d'un tableau de double qui représente les valeurs double de chaque points d'une imege de TailleTotaleImage, et affiche si pUnDC
	int LanceUnFit(CSize TailleTotaleImage, double* pDataImageY, CRect Zone, CDC* pUnDC = NULL);
	// Lance un Fit sur le ptrDib, dans la Zone, et affiche si pUnDC
//srybex 	int LanceUnFit(CDibImagePtr& ptrDib, CRect Zone, CDC* pUnDC = NULL);
	/*
	Fixe un parametre à partir d'une partie de son nom
	Si on donne aLaValeur != VALEUR_COURANTE, alors, on fixe à cette valeur
	renvoye le nombre de param dont l'état a effectivement été changé à fixés
	ie:0 si les param étaient déjà fixés.
	*/
	int ResteFixe(CString nom_a_fixer = CString(""), double aLaValeur = VALEUR_COURANTE);
	void ExportParamEnUnites(double* pFirstParam, int NbrMaxExporter = MAX_PARAM_FIT);
	int GetNbrParam();
	int GetMaxWhiteValue();
	CString GetParamNom(int Numero);
	CString GetParamUnit(int Numero);
	float GetChiCarre(){return m_ChiCarre;};
	double GetSommePixels(){return m_SommePixelZone;};
	void SetNbrMaxIter(int Niter);
	void SetPixelSize(double PSize, bool ActualiseDialogControls = true);
	void SetParamAjustableEnUnites(int Numero, double ValeurParam);
	void SetBoolAdjust(int Numero, int MustBeAdjusted);
	static void ResetParamEnPlus();
	static void SetParamEnPlus(CString* pStr0, double* pVal0, double* pMoy0, double* pStD0, int Nombre);
	static void AddParamEnPlus(CString* pStr0, double* pVal0, double* pMoy0, double* pStD0);
	static void CabbagesChecking(bool yesorno);
	static void DiscaredSaturatedPoints(bool yesorno);
	static void FittingFromPreviousPos(bool yesorno);
	static void StopWhenStable(bool yesorno);
	/*
	extrait des doubles d'un tableau pTotaleDataY correspondant a une image de TailleTotale, depuis une Zone, met les donnés dans dataX et dataY
	En fait, comme GetFloatPixelsFromZone, mais a partir d'un tableau de double
	*/
	CPointDbl GetFloatPixelsFromDoubleZone(CSize TailleTotale, double* pTotaleDataY, CRect Zone, float* dataX = NULL, float* dataY = NULL);
	/*
	Ca regarde les pixels dans tout un rectangle :
	ne verifie rien
	va a l'adresse du pixel From
		incrémente la position x jusqu'à +=NumPointX, puis incrémente y de 1
		et on recomence ca jusqu'à atteindre y+=NumPointY
	met la valeur dans m_pDataX et m_pDataY
	Renvoye un CPointDbl x=PixelSommeZone, y=PixelSaturesZone
	si on cherche en dehors de l'image, Renvoye CPoint x=y=HORS_IMAGE
	*/
	CPointDbl GetFloatPixelsFromZone(CDibImagePtr& ptrDib, CRect Zone, float* dataX = NULL, float* dataY = NULL);
	// sert à fournir la valeur de MaxWhiteValue au CFitting
	// renvoye 0 ssi bool(ptrDib) = 0; 
	int GetDibMaxWhiteValue(CDibImagePtr& ptrDib);

	void WriteProfileOnDib(CDibImagePtr& ptrDib, CRect Zone);
	// reset les m_pParamAjustables sur leurs Guess initiaux
	void ResetToGuess();

	// Sauve le resultat du Fit
	int SaveResult(CString FileName, double ImageIndex, float PixelSize, float AbsorpScaling, bool ImagePasBonne, int ImageNumero, CString FileNameVariation = ""	, CString BonusNom1 = CString(""), float BonusValeur1 = float(0.0)
																																							, CString BonusNom2 = CString(""), float BonusValeur2 = float(0.0));
	// surcharge pour permettre l'écriture de plusieur Fit dans le meme fichier
	int SaveResult(CFitting* pDeuxiemeFit, CString FileName, double ImageIndex, float PixelSize, float AbsorpScaling, bool ImagePasBonne, int ImageNumero, CString FileNameVariation = ""	, CString BonusNom1 = "", float BonusValeur1 = float(0.0)
																																													, CString BonusNom2 = "", float BonusValeur2 = float(0.0));
	// dessine la bonne fonction
	void DessineFonction(CDC* pUnDc);
	// Dessine la Gaussienne 2D 
	void Dessine2DGaussienne(CDC* pUnDc);
	// Dessine le Condensat 2D : nuage thermique rond + bec avec angle
	void Dessine2DCondensat(CDC* pUnDc);
	// Dessine la 2DTigeDoubleGauss 
	void Dessine2DTigeDoubleGauss(CDC* pUnDc, bool DessineUneSeuleGaussienne = false);
};
