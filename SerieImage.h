#pragma once

//srybex #include "BvcDib.h"
//srybex #include "Utilities.h"
#include "SettingSave.h"
#include "WaveForme.h"

#include <queue>
//srybex #include <bcam.h>
using namespace std;
using namespace Bcam;

#define CDibImagePtr CDibPtr
#define POINT_DISCARDED (float(-0.0123456))

#define HORS_IMAGE -1111
#define PAS_IMAGE -2222

enum{ClassicalAbsorption, HighIntensityAbsorption, Soustraction_3m2To1};

enum{LAST = -3, FIRST = -2, COURANTE = -1};

enum{NoImage = 0, ImageFluo = 1, ImageBad = 2, ImageAbs = 3};

enum{Reset_Fit_Allready_Computed = -2718};

enum{
	DontSave		= 0, 
	SaveCadran_1	= 1, 
	SaveCadran_2	= 2, 
	SaveCadran_3	= 4, 
	SaveCadran_4	= 8, 
	SaveAllCadrans	= 1+2+4+8	};

class CSerieImage
{
public:
	CSerieImage(void);
	~CSerieImage(void);

public:
	// Les Bitmaps
	CDibImagePtr m_ptrBitmap[4+1];
	// Un espace suplémentaire pour pouvoir éventuellement convertir en 8 bits le bitmap calculé (le [1])
	CDibImagePtr m_ptrBitmap8BitsImageCalculePourSauver;

	char m_PathName[2048];

	CString m_FolderPath;
	CString m_BaseName;
	int m_FirstIndex;
	int m_LastIndex;
	// Quels Image doit on sauver en ModeSave:
	int m_MustSavedCadran;
	// de combien l'image est zoomée dans l'écran
	static double m_ZoomEcranImage;
private:
	// le numero courant de l'image mis privée car en changeant d'iae, il y a plusieur choses a faire
	int m_ImageCourante;

private:
	// pointeur vers le tableau de données double qui représente les valeur d'une image, resultat d'un calcul
	// si on a caluler l'image, il sera non null, sinon, néttoyer et NULL (cf. PrepareCleanResultatCalculImage)
	double* m_pResultatCalculImageY;
	// Cree ou Clean les buffer si dessus
	void PrepareCleanResultatCalculImage(unsigned long nPoints = 0);
public:
	// un indicateur de si le fit de l'iamge courante a déjà été computée, ou non.
	bool m_FitAllreadyComputed;

public:
	// liste des FirstNames d'un bmp de chaque serie pour pouvoir faire des series de serie
	static CString m_FirstImagesPathsForSeries[128];
	static int m_IndexSerieCourante;
	static int m_NbrSeries;

	// l'absorption doit etre (re)calculée et le type de calcul
	static bool m_AbsCompute;
	static bool m_OverWrite;
	static int m_CalculationType;
	static double m_RangeAbsSerie;
	static double m_PixelSizeSerie;

	// pour l'imagerie haute intensité
	static double m_IsatSurIsat0Serie   ;
	static double m_Isat0Serie          ;
	static double m_PulseLengthSerie    ;
	static double m_AttenBeforeCamSerie ;

	// sensibilité d'un pixel de la caméra en photon pour élevé le signal de 1 bit sur 255
	static double m_PhotonParBitSur255;
	static double m_PhotonEnergie_J;

	// C'est une Serie par absorption, ou non
	static bool m_ModeAbsorp;
private:
	// le nombre de nuance de gris : 8bit ou 12bit (qui sont les 12 premiers d'un 16bit)
//	unsigned short m_MaxWhiteValueSerie;
	// La taille de l'image en unité pixel
	CSize m_Size;

public:
	// à partir d'un nouveau Chemin d'acces à une image, actualise toute les nouvelles valeurs de l'objet Serie
	int New(CString Path, bool ForSaving = false, int nbrImageToSave = 1);
	/*
	trouve toute les series d'un Folder et assigne la première à l'objet Serie courant
	renvoye le nombre de Series trouvées
	*/
	int FindAllSeriesOfFolder();
	// prend la Serie suivante dans la liste de m_FirstImagesPathsForSeries et renvoye le Path
	CString TakeNextSerieInList();

private:
	// affiche l'image Fluo dont on donne le "dossier" nom de "base" et "numero" d'image. renvoye 1 si ouvert
	int OuvreImageFluo(int numero = -1, CString dossier = "" , CString basename = "");
	// affiche l'image Fluo dont on donne le "dossier" nom de "base" et "numero" d'image
	int OuvreImageAbs(int numero = -1, CString dossier = "" , CString basename = "");
public:
	// affiche l'image dont on donne le "dossier" nom de "base" et "numero" d'image
	int OuvreImage(int numero = -1, CString dossier = "" , CString basename = "");
	bool IsInModeAbsorp(){return m_ModeAbsorp;};
	// set le mode en absorption ou en normal, renvoye la valeur de m_ModeAbsorp avant
	bool SetModeAbsorp(bool mode);
	// Donne l'ordre de calculer l'image par absorption
	void CalculateAbsorptionImage(bool ReSaveResultingImage = false);
	// soustrait 2 bitmaps : [3] - [2] et le met dans [1]
	void SoustraitBitmapAbs_3m2To1();
	// Calcul l'absorption classique : Ln(([3]-[4])/([2]-[4])) et le met dans [1]
	void ComputeClassicalAbs_Ln3m4s2m4To1();
	// Calcul l'absorption Haute intensité
	void ComputeHighIntensityAbsorption();
	// Next image
	int Next(bool ReleaseTheBitmaps = false);
	// Renvoye l'ancien index
	int GetCurrentImageIndex(){return m_ImageCourante;};
	// Renvoye l'ancien index
	int SetCurrentImageIndex(int NewCurrentIndex, bool ReleaseTheBitmaps = false);
	// renvoie les "NbrSousDossier" dernier dossier de "adresse"
	CString TrouveFinFolder(int NbrSousDossier, CString adresse = "");
	// renvoie le nom de base d'un Nom
	CString TrouveBase(CString Nom);
	// trouve le premier fichier (index) d'un nom de "base" dans le "dossier"
	int TrouveFirstIndex(CString base = "", CString dossier = "");
	// trouve le dernier fichier (index) d'un nom de "base" dans le "dossier"
	int TrouveLastIndex(CString base, CString dossier);
	// renvoie l'index situé à la fin d'un nom de fichier, -1 si il n'y a pas de chiffre
	int TrouveIndex(char* Fichier);
	CSize GetSizeSerie(){return m_Size;}
//	bool FitIsAllreadyComputed(){return m_FitAllreadyComputed;}
	double* Get_pResultatCalculImageY(){return m_pResultatCalculImageY;}
	//! Releases any bitmap which is hold by the view
	void ReleaseBitmaps();
	// dessine l'image 1 2 3 4
	void CopyBitmapToCadranSerieIndexCourant(int WhichCadran, CDibImagePtr& PtrDib);

	// Sauve l'image Fluo dont on donne le "dossier" nom de "base" et "numero" d'image
	int SaveFluoBitmap(int numero = -1, CString dossier = "" , CString basename = "");
	// Sauve l'image Abs dont on donne le "dossier" nom de "base" et "numero" d'image
	int SaveAbsBitmap(int WhichCadran = SaveAllCadrans, int numero = -1, CString dossier = "" , CString basename = "");
	// Sauve un fichier texte idoine correspondant aux images sauvées, et qui dit les paramètres utilisés pour la capture, ou autre
	void SaveTexteDescritpionSerie(CString t1 ="", double v1 =0, CString t2 ="", double v2 =0, CString t3 ="", double v3 =0, CString t4 ="", double v4 =0, CString t5 ="", double v5 =0, CString t6 ="", double v6 =0, CString t7 ="", double v7 =0, CString t8 ="", double v8 =0, CString t9 ="", double v9 =0, CString t10 ="", double v10=0);

	// Renvoye une CString contenant les informations relatives à la Zone précisée de l'image, toute l'image si on ne précise pas Zone
	CString InfoZoneImageSansAtomes(CRect Zone = CRect(-1,-1,0,0));
};

// Sauve une image sous fichier Bitmap
void SaveBMP(CDibImagePtr& PtrDibToSave, CString FileName);
// Sauve une image sous fichier Tiff
void SaveTIFF(CDibImagePtr& PtrDibToSave, CString FileName);
// copy une image dans un autre emplacement, en convertissant éventuellement à un BitPerPixel different
void CopyBitmapTo(CDibImagePtr& PtrDibToCopy, CDibImagePtr& PtrDibDestination, int BitPerPixelDestination_8_16 = 0 /* = NoConversion */);
// Convertit les coordonnée image en coordonnée ecran
CPoint PixelImageToEcran(CPoint PointInImage);
// Et vi versa
CPoint EcranToPixelImage(CPoint PointOnScreen);
// renvoye un CPoint contenant la somme (x) et le nombre de pixel saturé (y) dans une Zone de l'image, toute l'image si on ne spécifie pas Zone
CPointDbl GetSumAndSatInZone(CDibImagePtr& ptrDib, CRect Zone = CRect(-1,-1,0,0));
// comme GetSumAndSatInZone, mais pour un tableau de double représentant une image
// renvoye un CPoint contenant la somme (x) et le nombre de pixel saturé (y) dans une Zone de l'image (tableau de double), toute l'image si on ne spécifie pas Zone
CPointDbl GetSumAndSatInDoubleZone(CSize TailleTotale, double* pTotaleDataY, CRect Zone, double ValSat);
