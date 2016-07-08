// TraitImageDialog.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include <io.h>
#include ".\traitimagedialog.h"
//Included for Origin automation server access 
#include "OriginClient.h"

using namespace Bvc;

// CTraitImageDialog

IMPLEMENT_DYNAMIC(CTraitImageDialog, CProDialog)
CTraitImageDialog::CTraitImageDialog()
: m_ZoomFit(0), m_4Ecrans(0), m_NowComputing(false), m_PathImage("C:\\none.bmp"), m_pSerieCourante(NULL),
  m_SaveFitResult(true), m_AndOpenOrigin(false), m_TimeOutMillisec(0), m_NbrImageToSave(1), m_ModeSaveImages(false)
, m_pOriginClient(NULL), m_PixelAveragePointeur(1)
{
	m_CalculationType = ClassicalAbsorption;
//	for(int i = 1; i <= 4; i++)
//		m_ImageAffichageCadran8bit[i] = NULL;
}

CTraitImageDialog::~CTraitImageDialog()
{
	//if(IS_VALID_ORIGIN_PTR(m_pOriginClient))
	//{
	//	m_pOriginClient->DisconnectOrigin();
	//	delete m_pOriginClient;
	//	m_pOriginClient = NULL;
	//}
}

BEGIN_MESSAGE_MAP(CTraitImageDialog, CProDialog)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_IMAGE_COMBO_CALCULATION, OnCbnSelchangeImageComboCalculation)
	ON_EN_CHANGE(IDC_IMAGE_EDIT_NAME, OnEnChangeImageEditName)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTraitImageDialog, CProDialog)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_BROWSE		, 1, ValueChangedImageCheckBrowse		, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_FIRST		, 1, ValueChangedImageEditFirst			, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_LAST			, 1, ValueChangedImageEditLast			, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_RECALC		, 1, ValueChangedImageCheckRecalc		, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_ONLY_ABS	, 1, ValueChangedImageCheckOnlyAbs		, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_START		, 1, ValueChangedImageCheckStart		, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_SERIES		, 1, ValueChangedImageCheckSeries		, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_SCALE_ABS	, 1, ValueChangedImageEditScaleAbs		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_I_SAT_SUR_ISAT_0, 1, ValueChangedImageEditISatSurIsat0,VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_PULSE_LENGH	, 1, ValueChangedImageEditPulseLengh	, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_OD_CAMERA	, 1, ValueChangedImageEditOdCamera		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_ZOOM_FIT	, 1, ValueChangedImageCheckZoomFit		, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_CURENT		, 1, ValueChangedImageEditCurent		, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_TO_FITTING	, 1, ValueChangedImageCheckToFitting	, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_OVERWRITE_ABS, 1, ValueChangedImageCheckOverwriteAbs, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_PIXEL_SIZE, 1, ValueChangedImageEditPixelSize, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_SAVE_FITTING, 1, ValueChangedImageCheckSaveFitting, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_TYPE_IMAGING, 1, ValueChangedImageCheckTypeImaging, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_TIME_OUT, 1, ValueChangedImageEditTimeOut, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_SET_TO_0, 1, ValueChangedImageCheckSetTo0, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_BACK_MINUS_5, 1, ValueChangedImageCheckBackMinus5, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_BACK_MINUS_10, 1, ValueChangedImageCheckBackMinus10, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_FIND_FIRST_LAST, 1, ValueChangedImageCheckFindFirstLast, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_AND_OPEN_ORIGIN, 1, ValueChangedImageCheckOpenOrigin, VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_I_SAT_0, 1, ValueChangedImageEditISat0, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_EDIT_PIXEL_AVERAGE_POINTEUR, 1, ValueChangedImageEditPixelAveragePointeur, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CTraitImageDialog, IDC_IMAGE_CHECK_CONVERT_8BIT, 1, ValueChangedImageCheckConvert8bit, VTS_BOOL)
END_EVENTSINK_MAP()

BOOL CTraitImageDialog::OnInitDialogBar()
{
	CProDialog::OnInitDialogBar();
	return TRUE;
}
void CTraitImageDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);
	CProDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_IMAGE_COMBO_CALCULATION	, m_ComboCalculation);

	DDX_Control(pDX, IDC_IMAGE_CHECK_BROWSE			, m_NiCheckBrowse);
	DDX_Control(pDX, IDC_IMAGE_CHECK_RECALC			, m_NiCheckReCalculate);
	DDX_Control(pDX, IDC_IMAGE_CHECK_OVERWRITE_ABS	, m_NiCheckOverWrite);
	DDX_Control(pDX, IDC_IMAGE_CHECK_ONLY_ABS		, m_NiCheckOnlyAbs);
	DDX_Control(pDX, IDC_IMAGE_CHECK_SAVE_FITTING	, m_NiCheckSaveFitting);
	DDX_Control(pDX, IDC_IMAGE_CHECK_AND_OPEN_ORIGIN, m_NiCheckAndOpenOrigin);
	DDX_Control(pDX, IDC_IMAGE_CHECK_START			, m_NiCheckStartSave);
	DDX_Control(pDX, IDC_IMAGE_CHECK_SERIES			, m_NiCheckSeries);
	DDX_Control(pDX, IDC_IMAGE_CHECK_TYPE_IMAGING	, m_NiCheckTypeImaging);
	DDX_Control(pDX, IDC_IMAGE_CHECK_ZOOM_FIT		, m_NiCheckZoomFit);
	DDX_Control(pDX, IDC_IMAGE_CHECK_TO_FITTING		, m_NiCheckToFitting);

	DDX_Control(pDX, IDC_IMAGE_EDIT_SCALE_ABS		, m_NiEditRangeAbs);
	DDX_Control(pDX, IDC_IMAGE_EDIT_PIXEL_SIZE		, m_NiEditPixelSize);
	DDX_Control(pDX, IDC_IMAGE_EDIT_CURENT			, m_NiEditCurrentImage);
	DDX_Control(pDX, IDC_IMAGE_EDIT_FIRST			, m_NiEditFirstImage);
	DDX_Control(pDX, IDC_IMAGE_EDIT_LAST			, m_NiEditLastImage );
	DDX_Control(pDX, IDC_IMAGE_EDIT_TIME_OUT		, m_NiEditTimeOut);
	DDX_Control(pDX, IDC_IMAGE_EDIT_INDEX_MULTIPLICATION	, m_NiEditIndexMultiplication);

	DDX_Control(pDX, IDC_IMAGE_EDIT_I_SAT_SUR_ISAT_0, m_NiEditIsatSurIsat0);
	DDX_Control(pDX, IDC_IMAGE_EDIT_I_SAT_0			, m_NiEditIsat0);
	DDX_Control(pDX, IDC_IMAGE_EDIT_PULSE_LENGH		, m_NiEditPulseLength);
	DDX_Control(pDX, IDC_IMAGE_EDIT_OD_CAMERA		, m_NiEditAttenBeforeCam);

	DDX_Control(pDX, IDC_IMAGE_EDIT_NAME			, m_EditName);
	DDX_Control(pDX, IDC_IMAGE_EDIT_FOLDER			, m_EditFolder);

	DDX_Control(pDX, IDC_IMAGE_EDIT_PIXEL_AVERAGE_POINTEUR	, m_NiEditPixelAveragePointeur);
	DDX_Control(pDX, IDC_IMAGE_EDIT_INFO	, m_EditInfo);
	//}}AFX_DATA_MAP
}

// Gestionnaires de messages CTraitImageDialog


void CTraitImageDialog::OnPaint()
{
	CPaintDC dc(this);
	bool OldModeSave = m_ModeSaveImages;
	m_ModeSaveImages = bool(m_pMainFrame->m_Affichage & CAM);

	if(m_ModeSaveImages)
	{
		m_pSerieCourante = &(m_pMainFrame->m_CamDialog.m_SerieCam);
	}
	else
	{
		m_pSerieCourante = &m_SerieTrait;
	}

	if(m_bWasPaintedOnce == false)
	{
		m_bWasPaintedOnce = true;
		m_ComboCalculation.SetCurSel(m_CalculationType);

		m_pMainFrame->m_FitDialog.m_ZoneViseur.NormalizeRect();
		// si on était pas en mode save avant, et la non plus, on load le nom du fichier d'image
		if(!m_ModeSaveImages && !OldModeSave)
			BrowseImage(m_PathImage);
		// si on était en mode save avant, et la aussi, on load le nom du fichier pour sauver
		if(m_ModeSaveImages && OldModeSave)
			BrowseImage(m_PathImage);

		m_NiCheckBrowse.SetValue(false);

		//m_NiCheckOverWrite.SetEnabled(false);
		m_NiEditIsatSurIsat0.SetValue(m_IsatSurIsat0);
		m_NiEditIsat0.SetValue(m_Isat0);
		m_NiEditPulseLength.SetValue(m_PulseLength);
		m_NiEditAttenBeforeCam.SetValue(m_AttenBeforeCam);
	}
	UpdateControlsTraitImage();
}

/*
Update tous les controles de la TraitImageDialog au cas ou on les aurait changés programmaticaly
Renvoye...?
*/
int CTraitImageDialog::UpdateControlsTraitImage()
{
	m_NiEditCurrentImage.SetValue(m_pSerieCourante->GetCurrentImageIndex());		
	m_NiEditFirstImage.SetValue(m_pSerieCourante->m_FirstIndex);		
	m_NiEditLastImage.SetValue(m_pSerieCourante->m_LastIndex);		
	m_NiEditPixelSize.SetValue(m_PixelSize_µm);
	m_NiEditRangeAbs.SetValue(m_RangeAbs);

	m_NiEditIsat0.SetValue(m_Isat0);
	m_NiEditIsatSurIsat0.SetValue(m_IsatSurIsat0);
	m_NiEditPulseLength.SetValue(m_PulseLength);
	m_NiEditAttenBeforeCam.SetValue(m_AttenBeforeCam);
	
	m_NiCheckOnlyAbs.SetValue(!m_4Ecrans);	
	m_NiCheckTypeImaging.SetValue(m_pSerieCourante->IsInModeAbsorp());

	m_NiCheckReCalculate.SetValue(CSerieImage::m_AbsCompute);
	m_NiCheckOverWrite.SetValue(CSerieImage::m_OverWrite);
	m_NiCheckSeries.SetValue(CSerieImage::m_NbrSeries);		
	m_NiCheckZoomFit.SetValue(m_ZoomFit);		
	m_NiCheckToFitting.SetValue(m_pMainFrame->m_Affichage & FIT);		

	m_EditName.SetWindowText(m_pSerieCourante->m_BaseName);
	m_EditFolder.SetWindowText(m_pSerieCourante->TrouveFinFolder(2));

	m_NiEditPixelAveragePointeur.SetValue(m_PixelAveragePointeur);
	bool ModeAbs = m_pSerieCourante->IsInModeAbsorp();

	GetDlgItem(IDC_IMAGE_CHECK_FIND_FIRST_LAST)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_CHECK_BACK_MINUS_10)->ShowWindow(m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_CHECK_BACK_MINUS_5)->ShowWindow(m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_CHECK_SAVE_FITTING)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_EDIT_INDEX_MULTIPLICATION)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_STATIC_MULTIPLICATION_INDEX)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_CHECK_AND_OPEN_ORIGIN)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_CHECK_SERIES)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_EDIT_FIRST)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_IMAGE_EDIT_LAST)->ShowWindow(!m_ModeSaveImages);
	GetDlgItem(IDC_STATIC_RANGE)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_STATIC_ODCAM)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_STATIC_TIMEOUT)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_STATIC_ISAT_0)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_EDIT_I_SAT_0)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_EDIT_PIXEL_AVERAGE_POINTEUR)->ShowWindow(true);
	GetDlgItem(IDC_STATIC_PIXEL_AVERAGE)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_STATIC_ISAT_EFF)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_STATIC_PULSE)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_EDIT_SCALE_ABS)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_EDIT_TIME_OUT)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_EDIT_I_SAT_SUR_ISAT_0)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_EDIT_OD_CAMERA)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_EDIT_PULSE_LENGH)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_STATIC_CALCUL)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_COMBO_CALCULATION)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_CHECK_ONLY_ABS)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_CHECK_OVERWRITE_ABS)->ShowWindow(ModeAbs);
	GetDlgItem(IDC_IMAGE_CHECK_RECALC)->ShowWindow(ModeAbs);

	if(m_ModeSaveImages)
	{
		m_NiCheckStartSave.SetValue(m_pSerieCourante->m_MustSavedCadran);		
		m_NiCheckStartSave.SetOffText("Save");
		m_NiCheckStartSave.SetOnText("Save");
	//	m_NiEditCurrentImage.SetMode(CNiNumEdit::NumEditModes::ModeControl);
		m_NiCheckTypeImaging.SetMode(CNiButton::ButtonModes::ModeSwitchWhenPressed);
		SetDlgItemText(IDC_STATIC_IMAGE_INDEX, "Next to be taken");

	}
	else
	{
		m_NiCheckStartSave.SetValue(m_NowComputing);		
		m_NiCheckStartSave.SetOffText("Start");
		m_NiCheckStartSave.SetOnText("Stop");
	//	m_NiEditCurrentImage.SetMode(CNiNumEdit::NumEditModes::ModeIndicator);
		m_NiCheckTypeImaging.SetMode(CNiButton::ButtonModes::ModeIndicator);
		SetDlgItemText(IDC_STATIC_IMAGE_INDEX, "Current");
	}
return 0;
}

void CTraitImageDialog::ValueChangedImageCheckStart(BOOL Value)
{
	bool val = Value?true:false;

	// si on est en modeSave
	// et (XaitTrigger ou bien Pas liveGrabbing)
	// alors, on reset le compteur precis pour pouvoir prendre des images
	if(m_ModeSaveImages 
		&& (m_pMainFrame->m_CamDialog.m_WaitTrigger || !m_pMainFrame->m_CamDialog.m_LiveGrabbing))
	{
		m_pSerieCourante->m_MustSavedCadran = val ? SaveAllCadrans : DontSave;
		m_NiCheckOverWrite.SetValue(val);
		// remise a zero du compteur, car on peut entrer en mode prise d'image par abs
		m_pMainFrame->m_CamDialog.CompteurPrecisMillisecondes(true);
		// on sauve les données relative à la  CamSerie
		if(val)
			m_pMainFrame->m_CamDialog.m_SerieCam.SaveTexteDescritpionSerie(
			"gain_dB", m_pMainFrame->m_CamDialog.CamGainVERSgain_dB(m_pMainFrame->m_CamDialog.m_CamGain)
			,"expo_ms", m_pMainFrame->m_CamDialog.CamExpoVERSexpo_ms(m_pMainFrame->m_CamDialog.m_CamExpo)	);
	}
	else if (!m_ModeSaveImages)
	{
		m_pSerieCourante->m_MustSavedCadran = DontSave;

		m_pSerieCourante->SetCurrentImageIndex(m_pSerieCourante->m_FirstIndex, true);
		m_pSerieCourante->SetCurrentImageIndex(Reset_Fit_Allready_Computed);
		CSerieImage::m_IndexSerieCourante = 1;
		m_pSerieCourante->OuvreImage();
		AfficheNewBitmapSerieCourante();

		m_Date = LaDate();
		m_Nom_DateDuStart = m_pSerieCourante->m_FolderPath + m_pSerieCourante->m_BaseName + m_Date + ".txt";

		if(val && !m_NowComputing)
		{
			// le fichier existe t-il déjà ?
			bool dontexist = _access(m_Nom_DateDuStart,00) ? true : false;
			if(!dontexist && m_SaveFitResult) // on le supprime
				remove(m_Nom_DateDuStart);//, CString("OverWitten") + m_Nom_DateDuStart);
			m_NowComputing = true;  // donne l'ordre à IDLE de computer (sans bloquer l'ordi avec 100% CPU)
		}
		else
		{
			CSerieImage::m_NbrSeries = 0;
			m_NowComputing = false; // donne l'ordre à IDLE de stopper 
			UpdateControlsTraitImage();
		}
	}
}

BOOL CTraitImageDialog::OnIdle(LONG lCount)	
{
	if(!m_NowComputing)
		return true;
	// Quand on calcul des serie d'image :
	// si on n'est pas a la fin d'une serie ie à sa derniere image
	if(m_pSerieCourante->GetCurrentImageIndex() <= m_pSerieCourante->m_LastIndex)
	{
		// on veut peut etre savoir si l'ouverture s'est bien faite, ie si trois image se sont ouvertes pour l'absorption
		bool WasOpenedOk = true;
		UpdateControlsTraitImage();
		m_pSerieCourante->SetCurrentImageIndex(Reset_Fit_Allready_Computed);
		bool IsInModeAbs = m_pSerieCourante->IsInModeAbsorp();
		// a t-on ouvert le bon nombre d'image?
		WasOpenedOk = m_pSerieCourante->OuvreImage() == (IsInModeAbs ? ImageAbs : ImageFluo) 
													 ? true : false;
		AfficheNewBitmapSerieCourante();
		// dans ce cas là, on Save et on note si le Save s'est bien fait
		bool IsSaved = true;
		// on peut soit sauver le numéro d'une mauvaise image, soit ne carrément pas mettre la ligne en question
		if(/*WasOpenedOk && */m_SaveFitResult && m_pMainFrame->m_FitDialog.m_MustFit)
		{
			IsSaved = m_pMainFrame->m_FitDialog.m_pCurrentFit[1]->SaveResult(m_Nom_DateDuStart, m_pSerieCourante->GetCurrentImageIndex() * m_NiEditIndexMultiplication.Value, 0.0, 0.0, !WasOpenedOk, 0);
		}
        m_pSerieCourante->Next(true);
		m_pSerieCourante->SetCurrentImageIndex(Reset_Fit_Allready_Computed);
	} // sinon : on a fini une serie
	else
	{
		// on ouvre les resultats dans Origin
		if(m_AndOpenOrigin)
			OpenResultsInOrigin();

		m_pSerieCourante->SetCurrentImageIndex(m_pSerieCourante->m_LastIndex, true);
		m_pSerieCourante->SetCurrentImageIndex(Reset_Fit_Allready_Computed);
		// Si il y avait plusieur series : on prend la suivante
		if(CSerieImage::m_NbrSeries > 1)
		{
			CString Path = m_pSerieCourante->TakeNextSerieInList();
			// si il y en a une suivante, on actualise m_Nom_DateDuStart
			if(Path != "")
				m_Nom_DateDuStart = m_pSerieCourante->m_FolderPath + m_pSerieCourante->m_BaseName + m_Date + ".txt";
			else // sinon, on arrete le Fitting Series
				m_NowComputing = false;
		}
		else
			m_NowComputing = false;

		UpdateControlsTraitImage();
	}
return true;	
}

void CTraitImageDialog::OpenResultsInOrigin()
{
	m_Origin.OnConnectOrigin();
	Sleep(200);
	// on ne fit pas les param du fit, mais le resultat de calcul, qui se trouve en dernier dans le fichier txt du resultat du fit
	int NombreParamAnePasFiter = m_pMainFrame->m_FitDialog.m_pCurrentFit[1]->GetNbrParam();
	int NombreParamAfiter = m_pMainFrame->m_FitDialog.m_pCurrentFit[1]->m_pParamEnPlus_Combien;
	m_Origin.ExecuteOriginCFunction("ImportFilePlot", m_Nom_DateDuStart
		, NombreParamAfiter >= 1 ? NombreParamAnePasFiter + 1 : -1
		, NombreParamAfiter >= 2 ? NombreParamAnePasFiter + 2 : -1
		, NombreParamAfiter >= 3 ? NombreParamAnePasFiter + 3 : -1
		, NombreParamAfiter >= 4 ? NombreParamAnePasFiter + 4 : -1
		, NombreParamAfiter >= 5 ? NombreParamAnePasFiter + 5 : -1
		, NombreParamAfiter >= 6 ? NombreParamAnePasFiter + 6 : -1
		, NombreParamAfiter >= 7 ? NombreParamAnePasFiter + 7 : -1
		, NombreParamAfiter >= 8 ? NombreParamAnePasFiter + 8 : -1
		, NombreParamAfiter >= 9 ? NombreParamAnePasFiter + 9 : -1
		, NombreParamAfiter >= 10? NombreParamAnePasFiter + 10: -1);
//	CString NomSave = m_Nom_DateDuStart;
//	// ajout de la date jusqu'au jour au debut du nom du fichier
//	NomSave.Insert(NomSave.ReverseFind('\\') + 1, m_pMainFrame->LaDate(StopJour) + "_");
//	NomSave.Replace(".txt", ".OPJ");
//	Sleep(2000);
//	m_Origin.Save(NomSave);
	m_Origin.OnDisconnectOrigin();
}

void CTraitImageDialog::ValueChangedImageCheckBrowse(BOOL Value)
{
	m_Origin.OnDisconnectOrigin();
	//if(IS_VALID_ORIGIN_PTR(m_pOriginClient))
	//{
	//	m_pOriginClient->DisconnectOrigin();
	//	delete m_pOriginClient;
	//	m_pOriginClient = NULL;
	//}
	//return;
	bool val = Value?true:false; 
	if(!val)
		return;
	BrowseImage();
	UpdateControlsTraitImage();
    

}
void CTraitImageDialog::BrowseImage(CString File /* = "" */)
{
	CString Path;
	if(File == "" || File == "0")
	{
		CFileDialog dlg(!m_ModeSaveImages, _T("bmp"), _T(m_pSerieCourante->m_BaseName), 
			OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_ENABLESIZING 
		| OFN_EXPLORER | OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"), this);
		if(dlg.DoModal() != IDOK)
			return;
		Path = dlg.GetPathName();
	}
	else
		Path = File;
	// si le fichier n'existe pas: bye
	if(!m_ModeSaveImages && _access(Path, 04) == -1)
		return;
	// On actualise le m_PathName qui sert à sauver le chemin dans le fichier de config
	m_PathImage = Path;

	if(m_ModeSaveImages)
	{
		if(m_NbrImageToSave > 1)
			m_pSerieCourante->New(Path, m_ModeSaveImages, ImageAbs);
		else
			m_pSerieCourante->New(Path, m_ModeSaveImages, ImageFluo);
	}
	else
	{
		int FluoOuAbs = m_pSerieCourante->New(Path);
		AfficheNewBitmapSerieCourante();
//		if(FluoOuAbs == ImageAbs)
//			m_4Ecrans = 1;
		if(FluoOuAbs == ImageFluo)
			m_4Ecrans = 0;
	}
	UpdateControlsTraitImage();
}
void CTraitImageDialog::ValueChangedImageCheckSeries(BOOL Value)
{
	bool val = Value?true:false;
	if(m_ModeSaveImages)
	{
	}
	else
	{
		CSerieImage::m_NbrSeries = val;
		if(!CSerieImage::m_NbrSeries)
			return;

		m_pSerieCourante->FindAllSeriesOfFolder();
		UpdateControlsTraitImage();
	}
}
void CTraitImageDialog::OnEnChangeImageEditName()
{
	CString NameChanged;
	GetDlgItemText(IDC_IMAGE_EDIT_NAME, NameChanged);
	m_pSerieCourante->m_BaseName = NameChanged;
}


// ! Releases any bitmap which is hold by the view
void CTraitImageDialog::ReleaseBitmaps()
{
	m_pSerieCourante->m_ptrBitmap[1].Release();
	m_pSerieCourante->m_ptrBitmap[2].Release();
	m_pSerieCourante->m_ptrBitmap[3].Release();
	m_pSerieCourante->m_ptrBitmap[4].Release();
}

void CTraitImageDialog::AfficheNewBitmapSerieCourante()
{
	AfficheNewBitmap1(m_pSerieCourante->m_ptrBitmap[1]);
	AfficheNewBitmap2(m_pSerieCourante->m_ptrBitmap[2]);
	AfficheNewBitmap3(m_pSerieCourante->m_ptrBitmap[3]);
	AfficheNewBitmap4(m_pSerieCourante->m_ptrBitmap[4]);
}
void CTraitImageDialog::ActualiseAffichage4Image()
{	
	if(m_ImageAffichageCadran8bit[1] && bool(m_ImageAffichageCadran8bit[1]))
		AfficheNewBitmap1(m_ImageAffichageCadran8bit[1]);
	if(m_ImageAffichageCadran8bit[2] && bool(m_ImageAffichageCadran8bit[2]))
		AfficheNewBitmap2(m_ImageAffichageCadran8bit[2]);	
	if(m_ImageAffichageCadran8bit[3] && bool(m_ImageAffichageCadran8bit[3]))
		AfficheNewBitmap3(m_ImageAffichageCadran8bit[3]);
	if(m_ImageAffichageCadran8bit[4] && bool(m_ImageAffichageCadran8bit[4]))
		AfficheNewBitmap4(m_ImageAffichageCadran8bit[4]);	
}
void CTraitImageDialog::AfficheNewBitmap1(CDibImagePtr& unPtrDib)
{	CDibImagePtr & ThePtrBitmap = unPtrDib;
	if(!bool(ThePtrBitmap))
		return;
	CRect rect;		m_View.GetClientRect(&rect);
	SetTailleCadranTailleImageVue(rect, ThePtrBitmap->GetSize());
	CPoint PtFrom(0, 0);
	// si on n'essaye pas de simplement rafrachir m_ImageAffichageCadran8bit[1] dans l'écran, on copy, puis on affiche
//	if(ThePtrBitmap != m_ImageAffichageCadran8bit[1])
	if(!(ThePtrBitmap == m_ImageAffichageCadran8bit[1]))
		CopyBitmapTo(ThePtrBitmap, m_ImageAffichageCadran8bit[1], 8);
	AfficheNewBitmapFromPoint(m_ImageAffichageCadran8bit[1], PtFrom);

	CDC* pDC = m_View.GetDC();
	m_pMainFrame->m_FitDialog.DessineTraitReference(pDC);
	m_pMainFrame->m_FitDialog.DessineViseur(pDC);
	// affichage de la longueur réelle du segment dans l'indicateur 6 //////////////////////////////
	double pixelsizeee = m_PixelSize_µm / 1000.0;												  //
	CPoint SegFrom = m_pMainFrame->m_FitDialog.m_LeftClickDown;									  //
	CPoint SegTo = m_pMainFrame->m_FitDialog.m_LeftClickUp;										  //
	CString infoSeg;																			  //
	infoSeg.Format("Seg=%gmm", sqrt(double(   (SegFrom.x - SegTo.x) * (SegFrom.x - SegTo.x)		  //
												 + (SegFrom.y - SegTo.y) * (SegFrom.y - SegTo.y)  //
												 )	) * pixelsizeee);							  //
	m_pMainFrame->m_wndStatusBar.SetPaneText(6, infoSeg);										  //
	////////////////////////////////////////////////////////////////////////////////////////////////
	// affichage de la longueur réelle de la zone de fit ///////////////////////////////////////////
	CString infoZone;																			  //
	infoZone.Format("Zone:%gmm x %gmm", double(m_pMainFrame->m_FitDialog.m_ZoneViseur.Width()) * pixelsizeee
									,   double(m_pMainFrame->m_FitDialog.m_ZoneViseur.Height()) * pixelsizeee);							  //
	m_pMainFrame->m_wndStatusBar.SetPaneText(7, infoZone);										  //
	////////////////////////////////////////////////////////////////////////////////////////////////
	// on actualise la mesure de luminosité sous le pointeur en faisant croire qu'on y a bougé la sourie.
	MousseMovedOnView(0, m_View.m_LastPointMoved);
	// should we LancUnFit ?
	if(m_pMainFrame->m_FitDialog.m_MustFit && !m_pSerieCourante->m_FitAllreadyComputed)
		m_pMainFrame->m_FitDialog.ValueChangedFitCheckFit(true);
	
	// on actualise le graph oscilloscope...
	for(int indexCalc = 0; indexCalc < MAX_CALCULATED_VALUE; indexCalc++)
	{
		if(m_ModeSaveImages)
			if(ListView_GetCheckState(m_pMainFrame->m_FitDialog.m_ListCalcul.GetSafeHwnd( ), indexCalc))
				m_pMainFrame->m_FitDialog.AddValueToGraph(m_pMainFrame->m_FitDialog.m_ValeurCalcul[indexCalc], m_pMainFrame->m_CamDialog.m_CompteurPrecisMillisecondesSinceLastReset);
	}

	m_View.ReleaseDC(pDC);
}

void CTraitImageDialog::AfficheNewBitmap2(CDibImagePtr& unPtrDib)
{	CDibImagePtr & ThePtrBitmap = unPtrDib;
	if(!bool(ThePtrBitmap))
		return;
	CRect rect;		m_View.GetClientRect(&rect);
	SetTailleCadranTailleImageVue(rect, ThePtrBitmap->GetSize());
	CPoint PtFrom(m_TailleCadran.cx, 0);
	// si on n'essaye pas de copier m_ImageAffichageCadran8bit[2] dans lui meme, on copy
	if(!(ThePtrBitmap == m_ImageAffichageCadran8bit[2]))
		CopyBitmapTo(ThePtrBitmap, m_ImageAffichageCadran8bit[2], 8);
	AfficheNewBitmapFromPoint(m_ImageAffichageCadran8bit[2], PtFrom);
}
void CTraitImageDialog::AfficheNewBitmap3(CDibImagePtr& unPtrDib)
{	CDibImagePtr & ThePtrBitmap = unPtrDib;
	if(!bool(ThePtrBitmap))
		return;
	CRect rect;		m_View.GetClientRect(&rect);
	SetTailleCadranTailleImageVue(rect, ThePtrBitmap->GetSize());
	CPoint PtFrom(0, m_TailleCadran.cy);
	// si on n'essaye pas de copier m_ImageAffichageCadran8bit[3] dans lui meme, on copy
	if(!(ThePtrBitmap == m_ImageAffichageCadran8bit[3]))
		CopyBitmapTo(ThePtrBitmap, m_ImageAffichageCadran8bit[3], 8);
	AfficheNewBitmapFromPoint(m_ImageAffichageCadran8bit[3], PtFrom);
}
void CTraitImageDialog::AfficheNewBitmap4(CDibImagePtr& unPtrDib)
{	CDibImagePtr & ThePtrBitmap = unPtrDib;
	if(!bool(ThePtrBitmap))
		return;
	CRect rect;		m_View.GetClientRect(&rect);
	SetTailleCadranTailleImageVue(rect, ThePtrBitmap->GetSize());
	CPoint PtFrom(m_TailleCadran.cx, m_TailleCadran.cy);
	// si on n'essaye pas de copier m_ImageAffichageCadran8bit[4] dans lui meme, on copy
	if(!(ThePtrBitmap == m_ImageAffichageCadran8bit[4]))
		CopyBitmapTo(ThePtrBitmap, m_ImageAffichageCadran8bit[4], 8);
	AfficheNewBitmapFromPoint(m_ImageAffichageCadran8bit[4], PtFrom);
}
void CTraitImageDialog::AfficheNewBitmapFromPoint(CDibImagePtr& unPtrDib, CPoint TopLeftInView /* = CPoint(0,0)*/)
{
	CDibImagePtr& ThePtrBitmap = unPtrDib;
	// Si l'image est en 16 bit, on ne peut pas l'afficher, donc on la convertit en 8 bit pour l'affichage uniquement
	unsigned short NbrBits = ThePtrBitmap->GetBitsPerPixel();
//	if(NbrBits == 16)
//		ThePtrBitmap = ConvertFrom16To8BitsGrayScale(ThePtrBitmap);

	CDC& dc = *(m_View.GetDC());
	CRect rect;		m_View.GetClientRect(&rect);
	CDC dcMem;		dcMem.CreateCompatibleDC(&dc);		dc.SetStretchBltMode(COLORONCOLOR);

	// Trouve la taille du bitmap
	CSize Size;
	if(bool(unPtrDib))
		unPtrDib->GetSize(&Size); 
	else
		Size = GetSizePtrBitmap();

	SetTailleCadranTailleImageVue(rect, Size);

	// Draw the image
	if((bool)ThePtrBitmap)
	{
		dcMem.SelectObject(CBitmap::FromHandle((HBITMAP)ThePtrBitmap->GetBitmapHandle()));
		if(m_ZoomFit)//	
			dc.StretchBlt(
			TopLeftInView.x, TopLeftInView.y,			// coords du pt sup-gauche dans le contexte d'affichage (=fenetre)
			m_TailleCadran.cx, m_TailleCadran.cy,		// largeur dans le contexte d'affichage (=fenetre)
			&dcMem,									// le contexte d'affichage (=fenetre)
			0, 0,					// coords du pt sup-gauche dans L'IMAGE
			m_TailleImageVue.cx, m_TailleImageVue.cy,		// taille "vue" de L'IMAGE : dépend de m_ZoomFit
			SRCCOPY);
		else
			dc.BitBlt(
			TopLeftInView.x, TopLeftInView.y,	// coords du pt sup-gauche dans le contexte d'affichage (=fenetre)
			m_TailleCadran.cx, m_TailleCadran.cy,	// largeur dans le contexte d'affichage (=fenetre)
			&dcMem,									// le contexte d'affichage (=fenetre)
			0, 0,					// coords du pt sup-gauche dans L'IMAGE
			SRCCOPY);
	}
	m_View.ReleaseDC(&dc);
}

// Fixe les grandeurs m_TailleCadran, m_TailleImageVue et m_ZoomFactor
void CTraitImageDialog::SetTailleCadranTailleImageVue(CRect rect, CSize Size)
{
	if(m_CurrentSizeImage != Size)
		;//m_View.Invalidate(true);
	m_CurrentSizeImage = Size;
	if(m_4Ecrans && (m_NbrImageToSave > 1 || !m_ModeSaveImages))
	{
		m_TailleCadran.cx = rect.Width()/2;	// taille du cadran en pixel
		m_TailleCadran.cy = rect.Height()/2;
	}
	else
	{
		m_TailleCadran.cx = rect.Width();
		m_TailleCadran.cy = rect.Height();	
	}
	if(m_ZoomFit)
	{	
		if(double(Size.cy)/double(Size.cx) <= double(m_TailleCadran.cy)/double(m_TailleCadran.cx))
		{
			m_TailleImageVue.cx = Size.cx;
			m_TailleImageVue.cy = m_TailleCadran.cy * Size.cx/m_TailleCadran.cx;	
			m_ZoomFactor = double(m_TailleCadran.cx) / max(1,double(m_TailleImageVue.cx));	
		}
		if(double(Size.cy)/double(Size.cx) > double(m_TailleCadran.cy)/double(m_TailleCadran.cx))
		{
			m_TailleImageVue.cx = m_TailleCadran.cx * Size.cy/m_TailleCadran.cy;
			m_TailleImageVue.cy = Size.cy;	
			m_ZoomFactor = double(m_TailleCadran.cy) / max(1,double(m_TailleImageVue.cy));
		}
	} 
	else
	{	
		m_TailleImageVue.cx = m_TailleCadran.cx;
		m_TailleImageVue.cy = m_TailleCadran.cy;	
		m_ZoomFactor = 1.0;
	}
	CSerieImage::m_ZoomEcranImage = m_ZoomFactor;
}

// Trouve la taille du bitmap
CSize CTraitImageDialog::GetSizePtrBitmap()
{
	CSize Size;		
	if(		   (bool)m_pSerieCourante->m_ptrBitmap[1])
		m_pSerieCourante->m_ptrBitmap[1]->GetSize(&Size);
	else	if((bool)m_pSerieCourante->m_ptrBitmap[2])
		m_pSerieCourante->m_ptrBitmap[2]->GetSize(&Size);
	else	if((bool)m_pSerieCourante->m_ptrBitmap[3])
		m_pSerieCourante->m_ptrBitmap[3]->GetSize(&Size);
	else	if((bool)m_pSerieCourante->m_ptrBitmap[4])
		m_pSerieCourante->m_ptrBitmap[4]->GetSize(&Size);
	return Size;
}

void CTraitImageDialog::MousseMovedOnView(UINT nFlags, CPoint point)
{
	if(nFlags)
		return;
	CPoint PixelImage = EcranToPixelImage(point);
	CString infos, etplus;
	infos.Format(_T("Pos: (%d , %d)"), PixelImage.x, PixelImage.y);
	CRect LeRectPoint = CRect(PixelImage, CSize(1, 1));
	int inflate = (m_PixelAveragePointeur - 1) / 2;
	LeRectPoint.InflateRect(inflate, inflate);

	if(m_ImageAffichageCadran8bit[1] && bool(m_ImageAffichageCadran8bit[1]))
	{
		double Somme1Pix;
		// ca va nous renvoyer un CPoint(PixelSommeZone, PixelSaturesZone) sur une zone de 1 point
		CDibImagePtr& ThePtrImage = m_pSerieCourante->m_ptrBitmap[1];
        if(!ThePtrImage)
			return;
		Somme1Pix = GetSumAndSatInZone(ThePtrImage, LeRectPoint).x;
		unsigned short NbrBits = ThePtrImage->GetBitsPerPixel();
		int MaxWhiteValue;
		switch(NbrBits)
		{	
		case 8:		MaxWhiteValue = 255;	break;
		case 16:	MaxWhiteValue = 4095;	break;
		}
		etplus.Format(_T("Light: %d /%d"), int(Somme1Pix), MaxWhiteValue);
		m_pMainFrame->m_wndStatusBar.SetPaneText(m_pMainFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_09), infos);
		m_pMainFrame->m_wndStatusBar.SetPaneText(m_pMainFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_10), etplus);
	}	
	m_EditInfo.SetWindowText(m_pSerieCourante->InfoZoneImageSansAtomes(LeRectPoint));
}

void CTraitImageDialog::ValueChangedImageCheckRecalc(BOOL Value)
{	
	CSerieImage::m_AbsCompute = Value ? true : false;
	m_pSerieCourante->SetCurrentImageIndex(Reset_Fit_Allready_Computed);
	// si on a cliqué true on calcule
	if(CSerieImage::m_AbsCompute)
	{
		m_pSerieCourante->CalculateAbsorptionImage(CSerieImage::m_OverWrite);
		m_NiCheckOverWrite.SetEnabled(true);
	}
	// sinon, on rouvre l'image telle qu'elle est sur le disque
	else
	{
		m_pSerieCourante->OuvreImage();
		m_NiCheckOverWrite.SetValue(false);
		m_NiCheckOverWrite.SetEnabled(false);
	}
	AfficheNewBitmapSerieCourante();
}
void CTraitImageDialog::ValueChangedImageCheckOverwriteAbs(BOOL Value)
{
	CSerieImage::m_OverWrite = Value?true:false;
	if(CSerieImage::m_OverWrite)
		m_pSerieCourante->CalculateAbsorptionImage(CSerieImage::m_OverWrite);
}
void CTraitImageDialog::ValueChangedImageCheckOnlyAbs(BOOL Value)
{	m_4Ecrans = (Value? 0 : 1);			m_View.Invalidate();	}
void CTraitImageDialog::ValueChangedImageCheckZoomFit(BOOL Value)
{	m_ZoomFit =  (Value? 1 : 0);		m_View.Invalidate(true);	}

void CTraitImageDialog::ValueChangedImageCheckSaveFitting(BOOL Value)
{	m_SaveFitResult =  (Value? true : false);	
	if(!m_SaveFitResult)
		m_NiCheckAndOpenOrigin.SetValue(false);
}

void CTraitImageDialog::ValueChangedImageCheckOpenOrigin(BOOL Value)
{	m_AndOpenOrigin =  (Value? true : false);	}

void CTraitImageDialog::ValueChangedImageEditFirst(BLABLA00)
{
	m_pSerieCourante->m_FirstIndex = int( Value->dblVal );
}

void CTraitImageDialog::ValueChangedImageEditLast(BLABLA00)
{
	m_pSerieCourante->m_LastIndex = int( Value->dblVal );
}

void CTraitImageDialog::ValueChangedImageEditCurent(BLABLA00)
{
	if(m_NowComputing)
		return;
	// si la valeur est différente de la valeur courante, c'est qu'on va attendre une nouvelle salve d'image:
	if(int( Value->dblVal ) != m_pSerieCourante->GetCurrentImageIndex())
		m_pMainFrame->m_CamDialog.m_NbrImageWithoutTimeOut = 0; // pour dire a CamDialog qu'on attend la premiere image d'une salve

	m_pSerieCourante->SetCurrentImageIndex(int( Value->dblVal ), !m_ModeSaveImages);
	if(m_ModeSaveImages)
	{
		return;
	}

	m_pSerieCourante->SetCurrentImageIndex(Reset_Fit_Allready_Computed);
	m_pSerieCourante->OuvreImage(COURANTE);
	AfficheNewBitmapSerieCourante();
}
void CTraitImageDialog::ValueChangedImageEditScaleAbs(BLABLA00)
{
	m_RangeAbs = Value->dblVal;
	CSerieImage::m_RangeAbsSerie = m_RangeAbs;
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}

void CTraitImageDialog::ValueChangedImageEditPixelSize(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_PixelSize_µm = Value->dblVal;
	CSerieImage::m_PixelSizeSerie = m_PixelSize_µm;
	for(int j = 1; j <= MAX_FIT_SIMULTANNE; j++)
	{
		if(m_pMainFrame->m_FitDialog.m_pCurrentFit[j])
			m_pMainFrame->m_FitDialog.m_pCurrentFit[j]->SetPixelSize(m_PixelSize_µm);
	}
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}
void CTraitImageDialog::ValueChangedImageEditISatSurIsat0(BLABLA00)
{
	m_IsatSurIsat0 = Value->dblVal;
	CSerieImage::m_IsatSurIsat0Serie = m_IsatSurIsat0; 
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}
void CTraitImageDialog::ValueChangedImageEditPulseLengh(BLABLA00)
{
	m_PulseLength = Value->dblVal;
	CSerieImage::m_PulseLengthSerie = m_PulseLength; 
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}
void CTraitImageDialog::ValueChangedImageEditOdCamera(BLABLA00)
{
	m_AttenBeforeCam = Value->dblVal;
	CSerieImage::m_AttenBeforeCamSerie = m_AttenBeforeCam; 
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}
void CTraitImageDialog::ValueChangedImageEditISat0(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_Isat0 = Value->dblVal;
	CSerieImage::m_Isat0Serie = m_Isat0; 
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}

void CTraitImageDialog::ValueChangedImageEditPixelAveragePointeur(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	m_PixelAveragePointeur = int(Value->dblVal);
}

void CTraitImageDialog::ValueChangedImageCheckToFitting(BOOL Value)
{
	bool val = Value ? true : false;
	int TheMode;
	if(val)
		TheMode = m_pMainFrame->m_Affichage | FIT;
	else
		TheMode = m_pMainFrame->m_Affichage - (m_pMainFrame->m_Affichage & FIT);
	m_pMainFrame->ModeDialogBar(TheMode);
}


void CTraitImageDialog::OnCbnSelchangeImageComboCalculation()
{
	int oldCalculationType = m_CalculationType;
	m_CalculationType = m_ComboCalculation.GetCurSel();

	CSerieImage::m_CalculationType = m_CalculationType;

	if(oldCalculationType != m_CalculationType)
		if(CSerieImage::m_AbsCompute)
			m_pSerieCourante->CalculateAbsorptionImage(CSerieImage::m_OverWrite);
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}



void CTraitImageDialog::ValueChangedImageCheckTypeImaging(BOOL Value)
{
	if(m_ModeSaveImages)
	{
		m_NbrImageToSave = Value ? 3 : 1;
		m_pSerieCourante->SetModeAbsorp(Value);
		m_TimeOutMillisec = int(m_NiEditTimeOut.GetValue());
		m_pMainFrame->m_CamDialog.m_NiCheckTrig.SetValue(Value);
	}
	else
		m_pSerieCourante->SetModeAbsorp(Value);

	UpdateControlsTraitImage();
	if(!CSerieImage::m_AbsCompute)
		return;
	ValueChangedImageCheckRecalc(true);
}

void CTraitImageDialog::ValueChangedImageEditTimeOut(BLABLA00)
{
	m_TimeOutMillisec = int(Value->dblVal);
}

void CTraitImageDialog::ValueChangedImageCheckSetTo0(BOOL Value)
{
	if(Value)
		return;
	m_NiEditCurrentImage.SetValue(123456789.0);
	m_NiEditCurrentImage.SetValue(m_NiEditFirstImage.GetValue());
}

void CTraitImageDialog::ValueChangedImageCheckBackMinus5(BOOL Value)
{
	if(!Value)
		return;
	double val = m_NiEditCurrentImage.GetValue();
	m_NiEditCurrentImage.SetValue(123456789.0);
	m_NiEditCurrentImage.SetValue(max(0, val - 5.0));
}

void CTraitImageDialog::ValueChangedImageCheckBackMinus10(BOOL Value)
{
	if(!Value)
		return;
	double val = m_NiEditCurrentImage.GetValue();
	m_NiEditCurrentImage.SetValue(123456789.0);
	m_NiEditCurrentImage.SetValue(max(0, val - 10.0));
}


void CTraitImageDialog::ValueChangedImageCheckFindFirstLast(BOOL Value)
{
	if(!Value)
		return;
	m_pSerieCourante->TrouveFirstIndex();
	UpdateControlsTraitImage();
}




void CTraitImageDialog::ValueChangedImageCheckConvert8bit(BOOL Value)
{
	CopyBitmapTo(m_pSerieCourante->m_ptrBitmap[1], m_pSerieCourante->m_ptrBitmap8BitsImageCalculePourSauver, 8);
	SaveBMP(m_pSerieCourante->m_ptrBitmap8BitsImageCalculePourSauver, m_pSerieCourante->m_PathName + CString("__Conv_8bit"));
}





