#pragma once

#include "ProDialog.h"
#include "stdafx.h"

//srybex #include "SerieImage.h"
#include "SpeakWithOriginDlg.h"

//srybex #include "Utilities.h"
#include <queue>
//srybex #include <bcam.h>

using namespace std;
using namespace Bcam;

class COriginClient;

// CTraitImageDialog

class CTraitImageDialog : public CProDialog
{
	DECLARE_DYNAMIC(CTraitImageDialog)

public:
	CTraitImageDialog();
	virtual ~CTraitImageDialog();

protected:
	DECLARE_MESSAGE_MAP()

/////////////////////////// CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDialogBar)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
protected:
	virtual BOOL OnInitDialogBar();
/////////////////////////// End CF : Doc&Astuces  : "CDialogBar with InitDialog and DDX.htm"

public:
	afx_msg void OnPaint();
	//! Called by the applicaition object if the message loop runs dry
	BOOL OnIdle( LONG lCount );
	// Update tous les controles de la TraitImageDialo au cas ou on les aurait changés programmaticaly
	int UpdateControlsTraitImage();	
	CNiButton m_NiCheckBrowse;
	// la combo pour le type de Calcul a effectuer sur les images
	CComboBox m_ComboCalculation;
	int m_CalculationType;
	// Le Chemin de la dernière SerieImage pour réouvrir à l'ouverture du programme.
	CString m_PathImage;

protected:
	// Doit t'on lire ou sauver des images
	bool m_ModeSaveImages;
	// Nombre de millisecondes de Timout pour que le programme se dise qu'il a loupé une image et qu'il faut passer à l'index suivant
	int m_TimeOutMillisec;
		// combien d'image à sauver ?
	int m_NbrImageToSave;

	// zoom
	int m_ZoomFit;
	bool m_SaveFitResult;
	bool m_AndOpenOrigin;
	CSize m_CurrentSizeImage;
	double m_ZoomFactor;
	// la taille d'un cadran
	CSize m_TailleCadran;
	// la taille en pixel de la partie de l'image effectivemant vue dans le cadran.
	CSize m_TailleImageVue;

	CNiNumEdit m_NiEditCurrentImage, m_NiEditTimeOut, m_NiEditFirstImage, m_NiEditLastImage, m_NiEditIndexMultiplication;
	CNiButton m_NiCheckReCalculate, m_NiCheckStartSave, m_NiCheckOverWrite, m_NiCheckSaveFitting, m_NiCheckAndOpenOrigin, m_NiCheckTypeImaging, m_NiCheckOnlyAbs, m_NiCheckSeries, m_NiCheckZoomFit, m_NiCheckToFitting;
	CEdit m_EditName, m_EditFolder;
	int m_4Ecrans;
	// La Serie d'image traitée par TraitImage en mode TRAITIMAGE
	CSerieImage m_SerieTrait;
	// Pointeur vers la Série Courante
	CSerieImage* m_pSerieCourante;
	// Image des 4 cadrans converties en 8 bit pour l'affichage
	CDibImagePtr m_ImageAffichageCadran8bit[4+1];
	// une string qui contient une date
	CString m_Date;
	// Nom de la serie, daté du moment ou on a starté la serie, ou la serie de serie
	CString m_Nom_DateDuStart;
	// indique que le programme compute dans OnIdle()
	bool m_NowComputing;
	// Instance client d'origin 7.5
	CSpeakWithOriginDlg m_Origin;
	COriginClient* m_pOriginClient;

	//
	CNiNumEdit m_NiEditRangeAbs;
	CNiNumEdit m_NiEditPixelSize;
	double m_RangeAbs;
	double m_PixelSize_µm;
	double m_IsatSurIsat0, m_Isat0, m_PulseLength, m_AttenBeforeCam;
	CNiNumEdit m_NiEditIsatSurIsat0
			 , m_NiEditIsat0
			 , m_NiEditPulseLength
			 , m_NiEditAttenBeforeCam;
	CNiNumEdit m_NiEditPixelAveragePointeur;
	int m_PixelAveragePointeur;
	CEdit m_EditInfo;


public:

	//! Releases any bitmap which is hold by the view
	void ReleaseBitmaps();
	void AfficheNewBitmapSerieCourante();
	// dessine l'image 1 2 3 4
	void AfficheNewBitmap1(CDibImagePtr& unPtrDib);
	void AfficheNewBitmap2(CDibImagePtr& unPtrDib);
	void AfficheNewBitmap3(CDibImagePtr& unPtrDib);
	void AfficheNewBitmap4(CDibImagePtr& unPtrDib);
	void ActualiseAffichage4Image();
private:
	// dessine une image a partir d'un point
	void AfficheNewBitmapFromPoint(CDibImagePtr& unPtrDib, CPoint TopLeftInView = CPoint(0,0));
	// Convertit une image en format 16 bit GrayScale vers une en 8 bit GrayScale
	//CDibImagePtr ConvertFrom16To8BitsGrayScale(CDibImagePtr& XBitsPtrDib);

public:
	void SetTailleCadranTailleImageVue(CRect rect, CSize Size);
	CSize GetSizePtrBitmap();
	void OpenResultsInOrigin();
	void ValueChangedImageCheckBrowse(BOOL Value);
	void BrowseImage(CString File = "" );

	void MousseMovedOnView(UINT nFlags, CPoint point);
	
	DECLARE_EVENTSINK_MAP()

	void ValueChangedImageEditFirst(BLABLA00);
	void ValueChangedImageEditLast(BLABLA00);
	void ValueChangedImageCheckRecalc(BOOL Value);
	void ValueChangedImageCheckOnlyAbs(BOOL Value);
	void ValueChangedImageCheckStart(BOOL Value);
	void ValueChangedImageCheckSeries(BOOL Value);
	void ValueChangedImageEditScaleAbs(BLABLA00);
	void ValueChangedImageEditISatSurIsat0(BLABLA00);
	void ValueChangedImageEditPulseLengh(BLABLA00);
	void ValueChangedImageEditOdCamera(BLABLA00);
	void ValueChangedImageCheckZoomFit(BOOL Value);
	void ValueChangedImageEditCurent(BLABLA00);
	void ValueChangedImageCheckToFitting(BOOL Value);

	friend class CFitDialog;
	friend class CCamDialog;
	friend class CParametre;
	friend class CMainFrame;
	friend class CGeneticDialog;

	afx_msg void OnCbnSelchangeImageComboCalculation();
	void ValueChangedImageCheckOverwriteAbs(BOOL Value);
	void ValueChangedImageEditPixelSize(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedImageCheckSaveFitting(BOOL Value);
	afx_msg void OnEnChangeImageEditName();
	void ValueChangedImageCheckTypeImaging(BOOL Value);
	void ValueChangedImageEditTimeOut(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedImageCheckSetTo0(BOOL Value);
	void ValueChangedImageCheckBackMinus5(BOOL Value);
	void ValueChangedImageCheckBackMinus10(BOOL Value);
	void ValueChangedImageCheckFindFirstLast(BOOL Value);
	void ValueChangedImageCheckOpenOrigin(BOOL Value);
	void ValueChangedImageEditISat0(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedImageEditPixelAveragePointeur(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedImageCheckConvert8bit(BOOL Value);
};


