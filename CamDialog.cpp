// CamDialog.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "Rb2Manip.h"
#include "MainFrm.h"
#include <io.h>
#include <float.h>
#include ".\camdialog.h"

using namespace Bvc;
// CCamDialog

int temporaireement = 0;

IMPLEMENT_DYNAMIC(CCamDialog, CProDialog)
CCamDialog::CCamDialog()
: m_WaitTrigger(0), m_ExpoMode(0), m_TrigPolar(1), m_AoiSize(1392,1040), m_AoiUpLeft(0,0), m_LiveGrabbing(false)
, m_DisplayedBufferIndex(-1), m_CamExpo(1), m_CamGain(191), m_WantsToReGrabLive(false), m_BitPerPixels(16), m_NbrImageWithoutTimeOut(0)
{
	m_NbrComboBit = Mono16Bits;
	m_NbrComboTrigMode = SetByUser;
}

CCamDialog::~CCamDialog()
{
	for(int i=0; i<NUM_BUFFERS; ++i)
		m_ThePtrDib[i].Release();    
}


BEGIN_MESSAGE_MAP(CCamDialog, CProDialog)
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_GRAB_FINISHED, OnGrabFinished)
	ON_MESSAGE(WM_GRAB_STOPPED, OnGrabStopped)
	ON_MESSAGE(WM_ERROR, OnError)
	ON_CBN_SELCHANGE(IDC_CAM_COMBO_TRIGGER_MODE, OnCbnSelchangeComboCamTriggerMode)
	ON_CBN_SELCHANGE(IDC_CAM_COMBO_BITS, OnCbnSelchangeComboCamBits)
	ON_CBN_SELCHANGE(IDC_CAM_COMBO_COMPUTER_STATUS, OnCbnSelchangeComboCamComputerStatus)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCamDialog, CProDialog)
	ON_EVENT(CCamDialog, IDC_CAM_CHECK_GRAB, 1, ValueChangedCamCheckGrab, VTS_BOOL)
	ON_EVENT(CCamDialog, IDC_CAM_CHECK_SNAP, 1, ValueChangedCamCheckSnap, VTS_BOOL)
	ON_EVENT(CCamDialog, IDC_CAM_SLIDE_EXPO, 1, PointerValueChangedCamSlideExpo, VTS_I4 VTS_PVARIANT)
	ON_EVENT(CCamDialog, IDC_CAM_SLIDE_GAIN, 1, PointerValueChangedCamSlideGain, VTS_I4 VTS_PVARIANT)
	ON_EVENT(CCamDialog, IDC_CAM_EDIT_EXPO, 1, ValueChangedCamEditExpo, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CCamDialog, IDC_CAM_EDIT_GAIN, 1, ValueChangedCamEditGain, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CCamDialog, IDC_CAM_CHECK_TRIG, 1, ValueChangedCamCheckTrig, VTS_BOOL)
	ON_EVENT(CCamDialog, IDC_CAM_CHECK_AOI_ZONE, 1, ValueChangedCamCheckAoiZone, VTS_BOOL)
	ON_EVENT(CCamDialog, IDC_CAM_CHECK_AOI_ALL, 1, ValueChangedCamCheckAoiAll, VTS_BOOL)
END_EVENTSINK_MAP()

BOOL CCamDialog::OnInitDialogBar()
{
	// Support for DDX mechanism
	// If you do not want DDX then
	// do not call base class
	CProDialog::OnInitDialogBar();

	// Update any controls NOT supported by DDX
	// CBitmapButton is one
//	m_OKButtton.AutoLoad(IDOK, this);

	return TRUE;
}
void CCamDialog::DoDataExchange(CDataExchange* pDX)
{
	ASSERT(pDX);

	CProDialog::DoDataExchange(pDX);

	// DDX_??? functions to associate control with
	// data or control object
	// Call UpdateData(TRUE) to get data at any time
	// Call UpdateData(FALSE) to set data at any time

	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_CAM_COMBO_TRIGGER_MODE		, m_ComboTrigMode);
	DDX_Control(pDX, IDC_CAM_COMBO_BITS				, m_ComboBits);
	DDX_Control(pDX, IDC_CAM_COMBO_COMPUTER_STATUS	, m_ComboComputStatus);
	DDX_Control(pDX, IDC_CAM_SLIDE_EXPO				, m_NiSlideExpo);
	DDX_Control(pDX, IDC_CAM_SLIDE_GAIN				, m_NiSlideGain);
	DDX_Control(pDX, IDC_CAM_EDIT_EXPO				, m_NiEditExpo);
	DDX_Control(pDX, IDC_CAM_EDIT_GAIN				, m_NiEditGain);
	DDX_Control(pDX, IDC_CAM_CHECK_TRIG				, m_NiCheckTrig);
	//}}AFX_DATA_MAP
}

// Gestionnaires de messages CCamDialog


void CCamDialog::OnPaint()
{
	CPaintDC dc(this);
	if(m_bWasPaintedOnce == false)
	{
		m_bWasPaintedOnce = true;
		m_pMainFrame->m_TraitImDialog.OnPaint();

		m_ComboTrigMode.SetCurSel(1);
		m_ComboComputStatus.SetCurSel(1);
		m_ComboBits.SetCurSel(m_NbrComboBit);
		m_ComboTrigMode.SetCurSel(m_NbrComboTrigMode);
		OnCbnSelchangeComboCamBits();
		m_NiEditExpo.SetValue(CamExpoVERSexpo_ms(m_CamExpo));
		m_NiEditGain.SetValue(CamGainVERSgain_dB(m_CamGain));
		m_NiCheckTrig.SetValue(m_WaitTrigger);
	}
}

void CCamDialog::SetSerieBitmapCam(CDibImagePtr& PtrDib, int& Cadran)
{
	// appele du compteur précis a pour effet de changer m_CompteurPrecisMillisecondesSinceLastCall
	int msSinceLastReset = CompteurPrecisMillisecondes();
	int msSinceLastPicture = m_CompteurPrecisMillisecondesSinceLastCall;

	bool HaveToIncreaseImageNumber_AfterTheTreatmentOfThisOne = false;
	// si cadran auto, on met dans le cadran suivant que les temps entre images sont < TimeOut
	// si on est en attente de Trigger bien sur
	if(Cadran == CadranAuto && m_WaitTrigger)
	{
		// si l'image arrive très tard (ie > m_TimeOutMillisec)
		// ou aussi si on en a recu déja xxx m_NbrImageToSave
		// , on suppose que c'est la salve d'image suivante,
		// --> on fixe le m_NbrImageWithoutTimeOut à 1
		// PS si le m_NbrImageWithoutTimeOut == 0, on a pas recu d'image encore, donc on ne fait pas Next

		m_NbrImageWithoutTimeOut++;
		// si on a assez d'image
		if(m_NbrImageWithoutTimeOut >= m_pMainFrame->m_TraitImDialog.m_NbrImageToSave)
			HaveToIncreaseImageNumber_AfterTheTreatmentOfThisOne = true;
		// si timeout
		// on ne compte un timeout que si on a eut au moins une image
		if(msSinceLastPicture > m_pMainFrame->m_TraitImDialog.m_TimeOutMillisec
			&& m_NbrImageWithoutTimeOut > 1)
		{
			HaveToIncreaseImageNumber_AfterTheTreatmentOfThisOne = false;
			m_SerieCam.Next(true);
			m_pMainFrame->m_TraitImDialog.m_NiEditCurrentImage.SetValue(m_SerieCam.GetCurrentImageIndex()); // ce qui a pour effet de changer EditCurrent et donc m_NbrImageWithoutTimeOut = 0
			m_NbrImageWithoutTimeOut = 1; 
		}

		// si on doit sauver qu'une image a chaque fois
		if(m_pMainFrame->m_TraitImDialog.m_NbrImageToSave == 1)
			Cadran = 1;
		else // sinon, on commence par celle en haut à droite
			Cadran = m_NbrImageWithoutTimeOut + 1;
	}

	// si on est en modeSave, et que 
	// on attend trigger ou bien on est pas  LiveGrabbing
	// alors, on peut sauver
	if(m_pMainFrame->m_TraitImDialog.m_ModeSaveImages 
		&& (m_WaitTrigger || !m_LiveGrabbing))
	{
		m_SerieCam.m_MustSavedCadran
			= m_pMainFrame->m_TraitImDialog.m_NiCheckStartSave.GetValue() ? SaveAllCadrans : DontSave;
	}
	// si on est en fluo, sans trigger, on copy l'image dans la serie (cadran 1) et on l'affiche
	if(!m_SerieCam.IsInModeAbsorp() && !m_WaitTrigger) 
	{
		Cadran = 1;
		HaveToIncreaseImageNumber_AfterTheTreatmentOfThisOne = true;
//		m_SerieCam.CopyBitmapToCadranSerieIndexCourant(1, PtrDib);
//		m_pMainFrame->m_TraitImDialog.AfficheNewBitmap1(m_SerieCam.m_ptrBitmap[1]);
	}

	m_SerieCam.m_FitAllreadyComputed = false;

	int CadranToSave = 0;
	switch(Cadran)
	{
	case 1:	
		m_SerieCam.CopyBitmapToCadranSerieIndexCourant(1, PtrDib);
		m_pMainFrame->m_TraitImDialog.AfficheNewBitmap1(m_SerieCam.m_ptrBitmap[1]);
		CadranToSave = SaveCadran_1;
		break;
	case 2:	
		m_SerieCam.CopyBitmapToCadranSerieIndexCourant(2, PtrDib);
		m_pMainFrame->m_TraitImDialog.AfficheNewBitmap2(m_SerieCam.m_ptrBitmap[2]);
		CadranToSave = SaveCadran_2;
		break;
	case 3:	
		m_SerieCam.CopyBitmapToCadranSerieIndexCourant(3, PtrDib);
		m_pMainFrame->m_TraitImDialog.AfficheNewBitmap3(m_SerieCam.m_ptrBitmap[3]);
		CadranToSave = SaveCadran_3;
		break;
	case 4:	m_SerieCam.CopyBitmapToCadranSerieIndexCourant(4, PtrDib);
		m_pMainFrame->m_TraitImDialog.AfficheNewBitmap4(m_SerieCam.m_ptrBitmap[4]);
		CadranToSave = SaveCadran_4;
		break;
	default: /* = 1 ! */ 
		m_SerieCam.CopyBitmapToCadranSerieIndexCourant(1, PtrDib);		
		break;
	}
	// on sauve ?
	if(m_SerieCam.m_MustSavedCadran && m_WaitTrigger && m_LiveGrabbing)
	{
		if(m_SerieCam.IsInModeAbsorp() && Cadran == 4)
			m_SerieCam.SaveAbsBitmap(SaveCadran_2 + SaveCadran_3 + SaveCadran_4);
		if(!m_SerieCam.IsInModeAbsorp())
			m_SerieCam.SaveFluoBitmap();
	}

	// calculons l'image par absorption (si il n'y a rien a calculer, ca ne fait rien...)
	if(m_SerieCam.m_AbsCompute && m_SerieCam.IsInModeAbsorp() && m_WaitTrigger && m_LiveGrabbing
		&& m_NbrImageWithoutTimeOut >= m_pMainFrame->m_TraitImDialog.m_NbrImageToSave)
	{
		m_SerieCam.CalculateAbsorptionImage(m_SerieCam.m_MustSavedCadran & SaveCadran_1);
		m_pMainFrame->m_TraitImDialog.AfficheNewBitmap1(m_SerieCam.m_ptrBitmap[1]);
		m_SerieCam.SaveAbsBitmap(m_SerieCam.m_MustSavedCadran & SaveCadran_1); 
	}

	if(HaveToIncreaseImageNumber_AfterTheTreatmentOfThisOne)
	{
		m_SerieCam.Next(false);
		m_NbrImageWithoutTimeOut = 0; 
		m_pMainFrame->m_TraitImDialog.m_NiEditCurrentImage.SetValue(m_SerieCam.GetCurrentImageIndex()); // ce qui a pour effet de changer EditCurrent et donc m_NbrImageWithoutTimeOut = 0
	}
return;
}

// error checking macro
#define errChk(fCall) if (error = (fCall), error < 0) {goto Error;} else
bool CCamDialog::ConnectToCam(CBcam* pCam /* = NULL */)
{
	CBcam & TheCam = bool(pCam) ? *pCam : m_Bcam_1;

	if(TheCam.IsOpen())
	{
		TheCam.Close();
		//MessageBox("Allready Connected !", _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	// Check for a camera
	if(CBcam::DeviceNames().size() == 0)
	{
		MessageBox("No camera detected. \n\nThis may be because, in the \nMeasuremant and Automation National Instrument, \nThe bad driver used", _T("Error"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	// Get the device name of the first camera
	CString DeviceName = *(CBcam::DeviceNames().begin());
	
	// Open the driver object
	TheCam.Open( DeviceName );
	
	// This is the parameter set for : max image size, mono8, max. frame rate
	const DCSVideoFormat	VideoFormat = DCS_Format7;
	const DCSVideoMode		VideoMode   = DCS_Mode0;
	DCSColorCode ColorCode;

	switch(m_BitPerPixels)
	{
	case 8:
		ColorCode 	= DCSColor_Mono8;
		break;
	case 16:
		ColorCode 	= DCSColor_Mono16;
		break;
	default:
		MessageBox("Bad Color Depth", "", 1);	return false;
	}

	// Get information about this parameter set
	CString				ColorCodeName	= BcamUtility::ColorCodeName(ColorCode);
	unsigned short		BitsPerPixel	= BcamUtility::BitsPerPixel(ColorCode);
	CSize				ImageSize		= TheCam.FormatSeven[VideoMode].MaxSize();

	// Set the format, mode and color
	TheCam.SetVideoMode(VideoFormat, VideoMode, DCS_IgnoreFrameRate);
	TheCam.FormatSeven[VideoMode].ColorCoding = ColorCode;

	//Enable the hardware trigger
	TheCam.Trigger.OnOff = m_WaitTrigger ? true : false;
	if(m_WaitTrigger)
	{
			TheCam.Trigger.Mode = DCSTriggerMode(m_ExpoMode);
			TheCam.Trigger.Polarity = Polarity_t(m_TrigPolar);
	}

	TheCam.FormatSeven[VideoMode].Position = m_AoiUpLeft;
	TheCam.FormatSeven[VideoMode].Size = m_AoiSize;

	CString aoitexte;
	aoitexte.Format("AOI:%d x %d", m_AoiSize.cx, m_AoiSize.cy);
	m_pMainFrame->m_wndStatusBar.SetPaneText(4, aoitexte);
	double pixelsizeee = m_pMainFrame->m_FitDialog.m_pCurrentFit[1]->m_PixelSizeFit_µm / 1000.0;
	aoitexte.Format("=%gmmx%gmm)", double(m_AoiSize.cx) * pixelsizeee, double(m_AoiSize.cy) * pixelsizeee);
	m_pMainFrame->m_wndStatusBar.SetPaneText(5, aoitexte);
		
	// Set recommended speed
	unsigned long BytePerPacketMax = TheCam.FormatSeven[VideoMode].BytePerPacket.Max();
	unsigned long BytePerPacket = BytePerPacketMax;
	TheCam.FormatSeven[VideoMode].BytePerPacket = BytePerPacket;

	// Get more information about this parameter set (depending on the parameters already set)
	unsigned long BytePerFrame      = TheCam.FormatSeven[VideoMode].BytePerFrame();
	unsigned long PixelPerFrame     = TheCam.FormatSeven[VideoMode].PixelPerFrame();
	unsigned long PacketsPerFrame   = TheCam.FormatSeven[VideoMode].PacketsPerFrame();
	unsigned long PaddingBytes      = (BytePerPacket*PacketsPerFrame) - (BitsPerPixel*PixelPerFrame/8);
	// Tell the user
	CString Buffer, B;	Buffer += (B.Format("VideoFormat       \t= %lu\n", VideoFormat), B);	Buffer += (B.Format("VideoMode         \t= %lu\n", VideoMode), B);	Buffer += (B.Format("ColorCode         \t= %s\n", ColorCodeName ), B);	Buffer += (B.Format("BitsPerPixel      \t= %u\n", BitsPerPixel ), B);	Buffer += (B.Format("ImageSize         \t= %lu x %lu\n", m_AoiSize.cx, m_AoiSize.cy ), B);	Buffer += (B.Format("BytePerFrame     \t= %lu\n", BytePerFrame ), B);	Buffer += (B.Format("PixelPerFrame     \t= %lu\n", PixelPerFrame ), B);	Buffer += (B.Format("BytePerPacket    \t= %lu\n", BytePerPacket ), B);	Buffer += (B.Format("PacketsPerFrame   \t= %lu\n", PacketsPerFrame ), B);	Buffer += (B.Format("PaddingBytes      \t= %lu\n", PaddingBytes ), B);
//	MessageBox(Buffer, _T("OnGrabFormat7"), MB_OK | MB_ICONINFORMATION);

	OnSetShutter();
	OnSetGain();
return true;
}

void CCamDialog::ValueChangedCamCheckSnap(BOOL Value)
{
	bool value =	Value ? true : false;
	if(!value) return;

	// si on est LiveGrabbing, on arrete. 
	// et on relance le Snap
	if(m_LiveGrabbing)
	{
		m_LiveGrabbing = false;
		CheckDlgButton(IDC_CAM_CHECK_GRAB, false);
		CheckDlgButton(IDC_CAM_CHECK_SNAP, true);
		return;
	}

	CDibImagePtr& PtrDib = m_ThePtrDib[NUM_BUFFERS];
//	CBcam m_Bcam_OneShot;
	CBcam & TheCam = m_Bcam_1;
	try
	{ 
		ConnectToCam(&TheCam);
		// crée un bitmapp pour recevoir l'image prise
		PtrDib.Create( m_AoiSize, m_BitPerPixels, CDib::TopDown, CDib::Monochrome );
		// Get a pointer to the image buffer
		void* PtrPixel  = PtrDib->GetPixels();
		// Allocate resources (max num buffers, MaxImageSize
		TheCam.AllocateResources(1, m_AoiSize.cx * m_AoiSize.cy * m_BitPerPixels / 8);
		TheCam.GrabImage( PtrPixel , m_AoiSize.cx * m_AoiSize.cy * m_BitPerPixels / 8, 3000);

		int DansLeCadran = 1;
		SetSerieBitmapCam(PtrDib, DansLeCadran);

		OnGrabStopped(0,0);
	} 
	catch( BcamException &e )
	{
		CString Buffer, B;
		Buffer += (B.Format("Exception 0x%X occurred\n", e.Error() ), B);
		Buffer += (B.Format("Message = %s\n", e.Description() ), B);
		Buffer += (B.Format("Context = %s\n", e.Context()), B);
		MessageBox(Buffer, _T("OnGrabSingleImage"), MB_OK | MB_ICONEXCLAMATION);
	}
}

void CCamDialog::ValueChangedCamCheckGrab(BOOL Value)
{
	bool grab =	Value ? true : false;
	if(grab)
	{
		Sleep(100);
		OnGrabLive();
		Sleep(100);
	}
	else
	{
		OnGrabStop();
	}
		
	// on arrete de sauver si on est sans trigg
	if(!m_WaitTrigger && grab)
	{
		m_pMainFrame->m_TraitImDialog.ValueChangedImageCheckStart(false);
	}
	else
		m_pMainFrame->m_TraitImDialog.ValueChangedImageCheckStart(m_pMainFrame->m_TraitImDialog.m_NiCheckStartSave.GetValue());

}

void CCamDialog::ValueChangedCamCheckTrig(BOOL Value)
{	CBcam & TheCam = m_Bcam_1;
	m_WaitTrigger = Value ? true : false;	

	// on arrete de sauver si on m_LivGrabbing et sans trigg
	if(!m_WaitTrigger && m_LiveGrabbing)
	{
		m_pMainFrame->m_TraitImDialog.ValueChangedImageCheckStart(false);
	}
	else
		m_pMainFrame->m_TraitImDialog.ValueChangedImageCheckStart(m_pMainFrame->m_TraitImDialog.m_NiCheckStartSave.GetValue());

//	m_LiveGrabbing = false;
	try
	{	// Check for a camera
		if(CBcam::DeviceNames().size() == 0)
		{	return;	}
		bool m_WasNotOpen = !TheCam.IsOpen();
		if(m_WasNotOpen)
			return;
		//Enable the hardware trigger
		TheCam.Trigger.OnOff = m_WaitTrigger ? true : false;
		if(m_WaitTrigger)
		{
			TheCam.Trigger.Mode = DCSTriggerMode(m_ExpoMode);
			TheCam.Trigger.Polarity = Polarity_t(m_TrigPolar);
		}
	}
	catch( BcamException &e )
	{	CString Buffer, B;
		Buffer += (B.Format("Exception 0x%X occurred\n", e.Error() ), B);
		Buffer += (B.Format("Message = %s\n", e.Description() ), B);
		Buffer += (B.Format("Context = %s\n", e.Context()), B);
		MessageBox(Buffer, _T("OnSetShutter"), MB_OK | MB_ICONEXCLAMATION);	}
}

void CCamDialog::OnCbnSelchangeComboCamBits()
{	CBcam & TheCam = m_Bcam_1;
	m_NbrComboBit = m_ComboBits.GetCurSel();

	switch(m_NbrComboBit)
	{
	case Mono8Bits:
		m_BitPerPixels = 8;
		break;

	case Mono16Bits:
		m_BitPerPixels = 16;
		break;
	}
	// on restart un éventuel LiveGrab
	if(CBcam::DeviceNames().size() == 0)
		return;
	bool m_WasNotOpen = !TheCam.IsOpen();
	if(m_WasNotOpen)
		return;
	else
	{
		CheckDlgButton(IDC_CAM_CHECK_GRAB, false);
		m_WantsToReGrabLive = true;
	}
}

void CCamDialog::OnCbnSelchangeComboCamTriggerMode()
{
	m_NbrComboTrigMode = m_ComboTrigMode.GetCurSel();

	switch(m_NbrComboTrigMode)
	{
	case SetByUser:
		m_ExpoMode = 0;
		break;

	case TriggerWindow:
		m_ExpoMode = 1;
		break;
	}
	// on allume la fonction WaitTrigger
	CheckDlgButton(IDC_CAM_CHECK_TRIG, false);		
}

void CCamDialog::OnCbnSelchangeComboCamComputerStatus()
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}

/*!                BASLER From LiveViewMFC
Called it the user wants to start a live grab. 
Opens the driver, initilizes the camera and enqueues #NUM_BUFFERS grab commands.
When the grab commands are finished they turn up in the #GrabThreadProc.
*/
void CCamDialog::OnGrabLive(int NumberOfPictures /* = 0 */) 
{
	CBcam & TheCam = m_Bcam_1;
	try
	{
		ConnectToCam();

		// Create suitable bitmaps
		for(int i=0; i<NUM_BUFFERS; ++i)
			m_ThePtrDib[i].Create(m_AoiSize, m_BitPerPixels, CDib::TopDown, CDib::Monochrome);

		// Allocate Resources 
		TheCam.AllocateResources(NUM_BUFFERS, m_AoiSize.cx * m_AoiSize.cy * m_BitPerPixels / 8);

		// Start the grab thread
		if ( ! m_GrabThread.Create(&GrabThreadProc, this, THREAD_PRIORITY_HIGHEST) )
		{
			throw  BcamException(::GetLastError(), "CMainFrame::OnGrabLive : Create Grab Thread");
		}

		// Indicate that we're grabbing live now
		m_LiveGrabbing = true;

		// remise a zero du compteur precis
		m_pMainFrame->m_CamDialog.CompteurPrecisMillisecondes(true);

		m_StopWatch.Start();

		// Enqueue Grab commands 
		for(i=0; i<NUM_BUFFERS; ++i)
		{
			TheCam.GrabImageAsync((void*)m_ThePtrDib[i]->GetPixels(), 
				m_AoiSize.cx * m_AoiSize.cy * m_BitPerPixels / 8, 
				(void*)(__int64)i, 
				USE_ONESHOT);  
		}

		// Switch on the camera (in case we're using ContinuousShot)
		if(!USE_ONESHOT)
			TheCam.ContinuousShot = true;
	} 
	CATCH_MSGBOX( "CMainFrame::OnGrabLive" )        
}
/*!                BASLER From LiveViewMFC
Waits on the completion port for finished commands to show up.
- In case of a a finished grab command a #WM_GRAB_FINISHED message is send to the GUI thread
causing the #OnGrabFinished method to be fired.
- In case a #NotifyQuit message turns up a #WM_GRAB_STOPPED message is send to the GUI thread
causing the #OnGrabStopped method to be fired.
- If an error occurs a #WM_ERROR message is send to the GUI thread
causing the #OnError method to be fired.
*/
DWORD CCamDialog::GrabThreadProc(void* pParameter)
{
	CCamDialog* This = (CCamDialog*) pParameter;  // instance pointer
	CBcam & TheCam = This->m_Bcam_1;
	try
	{
		for(;;)
		{
			FunctionCode_t FunctionCode;
			unsigned long ErrorCode;
			void *pContext;
			// Wait for things to show up at the driver's completion port
			TheCam.WaitForCompletion(&FunctionCode, &ErrorCode, &pContext, INFINITE); 
			if(ErrorCode)
				throw BcamException(ErrorCode, "CMainFrame::GrabThreadProc1");
			switch(FunctionCode)
			{
			case AsyncGrabImage:
				{
					// Stop the watch and restart it again immediately
					float ElapsedTime = (float)This->m_StopWatch.Stop(true);
					// Notfy the GDI thread that the buffer with the BufferIndex has been grabbed
					unsigned int BufferIndex = *(unsigned int*)&pContext;
					This->PostMessage(WM_GRAB_FINISHED, *(WPARAM*)&BufferIndex, *(LPARAM*)&ElapsedTime ); 
				}
				break;
				// This means the queue is empty now ==> suicide
			case NotifyQuit:
				// Tell the GdiThread to clean up
				This->PostMessage(WM_GRAB_STOPPED, 0, 0 ); 
				// Finish thread
				return 0;
				// Don't care about other function codes like AsyncSetGain etc.
			default:
				ErrorCode = 1;
				break;
			}
		}
	}
	catch (BcamException&  e )
	{
		// Forward error to GdiThread for display
		BcamException* pE = new BcamException(e);
		This->PostMessage(WM_ERROR, (unsigned __int64)pE, 0); 
		return e.Error();
	}
	catch (...)
	{
		// Forward error to GdiThread for display
		BcamException* pE = new BcamException(DISP_E_EXCEPTION, "CMainFrame::GrabThreadProc2");
		This->PostMessage(WM_ERROR, (unsigned __int64)pE, 0); 
		return DISP_E_EXCEPTION;
	}
return 0;
}
/*!                BASLER From LiveViewMFC
Called if the #GrabThreadProc receives a finished grab command. The wParam includes the buffer's index.
Enqueues the buffer's index to the #m_BufferQueue. The BufferQueue will be emplied by #OnIdle.
*/
afx_msg LRESULT CCamDialog::OnGrabFinished(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CString teeempo;
		teeempo.Format("GF%d", temporaireement++);
		m_pMainFrame->m_wndStatusBar.SetPaneText(1, teeempo);

// store the index of the grabbed buffer in the buffer queue
		m_BufferQueue.push(*(unsigned int*)&wParam);

		// Update framerate statistics
		float ElapsedTime = *((float*)&lParam);
		if(ElapsedTime > FLT_EPSILON || ElapsedTime < -FLT_EPSILON)
			m_Fps.Add(1.0 / ElapsedTime);

		// Show framerate
		CString Buffer;
		Buffer.Format("%3.1f fps", m_Fps.Avg());
		m_pMainFrame->m_wndStatusBar.SetPaneText(m_pMainFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_01), Buffer);
	}
	CATCH_MSGBOX( "CMainFrame::OnGrabFinished" )            
return 0;
}

/*!                BASLER From LiveViewMFC
Called each time all messages in the message queue of the GUI thread are processed and
no more messages are waiting.
- If the #m_BufferQueue contains buffer indices these are removes. For each buffer 
a new grab command is enqueued. When the grab commands are finished they turn up in 
the #GrabThreadProc. The most recent buffer is put to m_view for display.
- Updates the user interface's enable stati.
*/
BOOL CCamDialog::OnIdle(LONG lCount)
{
	CBcam & TheCam = m_Bcam_1;
	try
	{
		// If the grab is to be stopped, don't enqueue buffers any more
		if(m_LiveGrabbing)
		{
			// For all buffer which might have been pushed to the queue by OnGrabFinished
			while(0 < m_BufferQueue.size())
			{
				// Get the buffer's index from the queue
				unsigned int BufferIndex = m_BufferQueue.front();
				m_BufferQueue.pop();        
				// If more than one buffer is in the queue display only the most recent
				if(0 < m_BufferQueue.size() )
				{ 
					// Enqueue a new Grab command 
					TheCam.GrabImageAsync(
					(void*)m_ThePtrDib[BufferIndex]->GetPixels(), 
					m_ThePtrDib[BufferIndex]->GetTotalPixelBytes(), 
					(void*)(__int64)BufferIndex,
					USE_ONESHOT);  
				}
				else 
				{
					// Enqueue the buffer currently being displayed
					if(m_DisplayedBufferIndex != -1)
					{
						// Enqueue a new Grab command 
						TheCam.GrabImageAsync(
							(void*)m_ThePtrDib[m_DisplayedBufferIndex]->GetPixels(), 
							m_ThePtrDib[m_DisplayedBufferIndex]->GetTotalPixelBytes(), 
							(void*)(__int64)BufferIndex,
							USE_ONESHOT);  
					}
					// Remember which buffer is now displayed
					m_DisplayedBufferIndex = BufferIndex;
					
//////////////////////////////////////////////////////////////////////////////////////////         

					// on dit a CamDialog de choisir le Cadran en fonction du temps de prise de l'image, et du m_TimeOutMillisec de TraitImageDialog
					int DansLeCadran = CadranAuto;
					SetSerieBitmapCam(m_ThePtrDib[BufferIndex], DansLeCadran);
				}
			}
		}
	}
	CATCH_MSGBOX( "CMainFrame::OnIdle" ) 
return false;
}
/*!                BASLER From LiveViewMFC
Called if the user wants the live grab to stop. Enqueues a #NotifyQuit message to the grab
queue. Sets the #m_LiveGrabbing flag to false to prevent new grab commands from being enqueued.
When all commands currently being in the queue are finished the NotifyQuit message will turn up 
in the #GrabThreadProc.
*/
void CCamDialog::OnGrabStop() 
{
	CBcam & TheCam = m_Bcam_1;
	try
	{
		// This will prevent new images from being enqueued
		m_LiveGrabbing = false;    
		// Enqueue a "clean-up" message which will show up after all images being currently
		// in the queue have been received
/*return;*/		TheCam.Notify(NotifyQuit, NULL);
	}
	CATCH_MSGBOX( "CMainFrame::OnGrabStop" )    
}

/*!                BASLER From LiveViewMFC
Called if the #GrabThreadProc receives a #NotifyQuit message. This means that the command queue is
empty now and everything can be cleaned up.
*/
afx_msg LRESULT CCamDialog::OnGrabStopped(WPARAM wParam, LPARAM lParam) 
{
	CBcam & TheCam = m_Bcam_1;
	try
	{
		CString teeempo;
		teeempo.Format("GS%d", temporaireement++);
		m_pMainFrame->m_wndStatusBar.SetPaneText(2, teeempo);

		// Clean up queue
		while(0 < m_BufferQueue.size())
			m_BufferQueue.pop();    
		// forget about the displayed buffer
		m_DisplayedBufferIndex = -1;    
		// switch off camera (in case we're using ContinuousShot)
		if(!USE_ONESHOT)
			TheCam.ContinuousShot = false;    
		// shutdown thread
		m_GrabThread.Release();  // this waits for the thread to die    
		// Cancel pending I/O requests.
		// This is optional since closing the driver will cancel pending requests automatically
		TheCam.Cancel();
		// Release bandwidth, isoch channels and isoch transfer contexts 
		// This is optional since closing the driver will trigger this automatically.
		// Warning! Calling FreeResources is not allowed, if there are still pending I/O requests, 
		// so call Cancel() first
		TheCam.FreeResources();    
		// Close driver 
		TheCam.Close();    
		// Delete bitmaps
		for(int i=0; i<NUM_BUFFERS; ++i)
			m_ThePtrDib[i].Release();    
		// Reset frame rate display
		m_Fps.Reset();
		m_pMainFrame->m_wndStatusBar.SetPaneText(m_pMainFrame->m_wndStatusBar.CommandToIndex(ID_INDICATOR_01), "");   	
	}
	CATCH_MSGBOX( "CMainFrame::OnGrabStopped" )
	if(m_WantsToReGrabLive)
		CheckDlgButton(IDC_CAM_CHECK_GRAB, true);
	m_WantsToReGrabLive = false;
return 0;
}

/*!                BASLER From LiveViewMFC
Called if an error occurred in the #GrabThreadProc. Shows an error message box.
*/
afx_msg LRESULT CCamDialog::OnError(WPARAM wParam, LPARAM lParam) 
{
		CString teeempo;
		teeempo.Format("ER%d", temporaireement++);
		m_pMainFrame->m_wndStatusBar.SetPaneText(3, teeempo);

	// Show the error message coming from another thread
	BcamException* pE = (BcamException*)wParam;
	CString Buffer, B;
	Buffer += (B.Format("Exception 0x%X occurred\n", pE->Error() ), B);
	Buffer += (B.Format("Message = %s\n", pE->Description() ), B);
	Buffer += (B.Format("Context = %s\n", pE->Context()), B);
	MessageBox(Buffer, _T("CMainFrame::OnError"), MB_OK | MB_ICONEXCLAMATION);

	delete pE;

	return 0;
}

/*!                BASLER From LiveViewMFC
Makes sure the application terminates cleanly if closed via the system menu
*/
void CCamDialog::OnSysCommand(UINT nID, LPARAM lParam) 
{
	CBcam & TheCam = m_Bcam_1;
	if(nID == SC_CLOSE)
	{
		if(m_LiveGrabbing)
		{
			// Enqueue a "clean-up" message which will show up after all images being currently
			// in the queue have been received
			TheCam.Notify(NotifyQuit, NULL);
			// Wait until the GrabThread died
			::WaitForSingleObject((HANDLE)m_GrabThread,INFINITE);
			// The rest will clean up itselfe via the destructors
		}
	}
//  CFrameWnd::OnSysCommand(nID, lParam);
}

int CCamDialog::UpdateControls()
{
return 0;
}

LRESULT CCamDialog::OnSetShutter()//WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	CBcam & TheCam = m_Bcam_1;
	try
	{	// Check for a camera
		if(CBcam::DeviceNames().size() == 0)
		{	return 0;	}
		bool m_WasNotOpen = !TheCam.IsOpen();
		if(m_WasNotOpen)
		{	
	return 0;
			m_WasNotOpen=true;
			// Get the device name of the first camera
	        CString DeviceName = *(CBcam::DeviceNames().begin());
	        // Create the driver object and open the driver
			TheCam.Open( DeviceName );	
		}
		TheCam.Shutter.Raw = m_CamExpo;
		if(m_WasNotOpen)
		{	m_WasNotOpen=false;
			// Get the device name of the first camera
			CString DeviceName = *(CBcam::DeviceNames().begin());
			// Create the driver object and open the driver
			TheCam.Close();	}
	} 
	catch( BcamException &e )
	{	CString Buffer, B;
		Buffer += (B.Format("Exception 0x%X occurred\n", e.Error() ), B);
		Buffer += (B.Format("Message = %s\n", e.Description() ), B);
		Buffer += (B.Format("Context = %s\n", e.Context()), B);
		MessageBox(Buffer, _T("OnSetShutter"), MB_OK | MB_ICONEXCLAMATION);	}
return 0;
}
LRESULT CCamDialog::OnSetGain()//WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{	CBcam & TheCam = m_Bcam_1;
	try
	{	// Check for a camera
		if(CBcam::DeviceNames().size() == 0)
		{	//	MessageBox("No camera present", _T("Error"), MB_OK | MB_ICONEXCLAMATION);
			return 0;	}
		bool m_WasNotOpen = !TheCam.IsOpen();
		if(!TheCam.IsOpen())
		{
	return 0;
			m_WasNotOpen=true;
			// Get the device name of the first camera
			CString DeviceName = *(CBcam::DeviceNames().begin());
	        // Create the driver object and open the driver
			TheCam.Open( DeviceName );
		}
		TheCam.Gain.Raw = m_CamGain;
		if(m_WasNotOpen==true)
		{	m_WasNotOpen=false;
			// Get the device name of the first camera
			CString DeviceName = *(CBcam::DeviceNames().begin());
			// Create the driver object and open the driver
			TheCam.Close();	}
	} 
	catch( BcamException &e )
	{	CString Buffer, B;
		Buffer += (B.Format("Exception 0x%X occurred\n", e.Error() ), B);
		Buffer += (B.Format("Message = %s\n", e.Description() ), B);
		Buffer += (B.Format("Context = %s\n", e.Context()), B);
		MessageBox(Buffer, _T("OnSetGain"), MB_OK | MB_ICONEXCLAMATION);	}
return 0;
}
void CCamDialog::PointerValueChangedCamSlideExpo(long Pointer, VARIANT* Value)
{	double expo_ms = Value->dblVal;
	unsigned int cam_expo = (unsigned long)(max(1,(expo_ms*1000-20)/20));
	expo_ms = (cam_expo*20.0+20.0)/1000.0;
	if(GetFocus() == GetDlgItem(IDC_CAM_SLIDE_EXPO))
		m_NiEditExpo.SetValue(expo_ms);
}

void CCamDialog::PointerValueChangedCamSlideGain(long Pointer, VARIANT* Value)
{	double gain_dB = Value->dblVal;
	unsigned int cam_gain = (unsigned long)max(		658.*(exp(.1151292546*gain_dB+.6010898385)-1.)/(exp(.1151292546*gain_dB+.6010898385)+1.)  
									,	28.24858757*gain_dB+147.4858757);;
	gain_dB = CamGainVERSgain_dB(cam_gain);
	if(GetFocus() == GetDlgItem(IDC_CAM_SLIDE_GAIN))
		m_NiEditGain.SetValue(gain_dB);
}

void CCamDialog::ValueChangedCamEditExpo(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	double expo_ms = Value->dblVal;
	// on transforme le double en valeur pour la caméra
	unsigned int cam_expo = (unsigned long) (min(4095, (max(1.0, (expo_ms*1000-20)/20 ))));
	// on réactualise la valeur double sur la valeur pour caméra : comme ca, la valeur est une valeur autorisée.
	expo_ms = (cam_expo*20.0+20.0)/1000.0;
    // on réactualise le controle.
	m_NiEditExpo.SetValue(expo_ms);
	m_CamExpo = cam_expo;
	// si on est pas en trainde bouger le curseur, on l'actualise
	if(!(GetFocus() == GetDlgItem(IDC_CAM_SLIDE_EXPO)))
		m_NiSlideExpo.SetValue(expo_ms);
	OnSetShutter();
}

void CCamDialog::ValueChangedCamEditGain(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange)
{
	double gain_dB = Value->dblVal;
	// on transforme le double en valeur pour la caméra
	// le +0.5, c'est pour avoir un arrondi a l'entier le plus proche, et non pas a l'entier inferieur
	unsigned int cam_gain = (unsigned long)(0.5 + max(	658.*(exp(.1151292546497023*gain_dB+.6010898385260956)-1.)/(exp(.1151292546497023*gain_dB+.6010898385260956)+1.)
												,	28.24858757062147*gain_dB+147.4858757062147) );
	// on réactualise la valeur double sur la valeur pour caméra : comme ca, la valeur est une valeur autorisée.
	gain_dB = CamGainVERSgain_dB(cam_gain);
	// on réactualise le controle.
	m_NiEditGain.SetValue(gain_dB);
	m_CamGain = min(1023, max(192, cam_gain));
	// si on est pas en train de bouger le curseur, on l'actualise
	if(!(GetFocus() == GetDlgItem(IDC_CAM_SLIDE_GAIN)))
		m_NiSlideGain.SetValue(gain_dB);
	OnSetGain();
}
// fonction de conversion cam_gain --> dB
double CCamDialog::CamGainVERSgain_dB(int mG)
{
	double dB = -654231.0;
	if(mG <= 192) 
	{dB = 0.0;	}
	if(mG > 192 && mG <= 511) 
	{dB = (20*log10((658.0+mG)/(658.0-mG)) - 5.221);	}
	if(mG >= 512 && mG <= 1023) 
	{dB = (0.0354*mG - 5.221);	}
	if(mG >= 1024) 
	{dB = (0.0354*1023.0 - 5.221);	}
return dB;
}
// fonction de conversion cam_gain --> dB
double CCamDialog::CamExpoVERSexpo_ms(int mE)
{
return (mE*20.0+20.0)/1000.0;
}

void CCamDialog::ValueChangedCamCheckAoiZone(BOOL Value)
{	if(Value)
		return;
	bool WasTrigged = m_WaitTrigger;
	ValueChangedCamCheckTrig(false);

	CBcam & TheCam = m_Bcam_1;
	try
	{	//{{	// Check for a camera
		if(CBcam::DeviceNames().size() == 0)
		{	return;	}
		bool m_WasNotOpen = !TheCam.IsOpen();

		CRect& rectviseur = m_pMainFrame->m_FitDialog.m_ZoneViseur;
		CPoint& croixviseur = m_pMainFrame->m_FitDialog.m_CentreCroix;
		// Si l'AOI devient too small on ne fait rien
		if(rectviseur.Size().cx <= 2 * STEP_ZONE_VISEUR || rectviseur.Size().cy <= 2 * STEP_ZONE_VISEUR)
			;//return;
		m_AoiSize = rectviseur.Size();
		m_AoiUpLeft += rectviseur.TopLeft();
		// La zone devient l'AOI, en plus petit
		croixviseur -= rectviseur.TopLeft();
		CPoint upleft(STEP_ZONE_VISEUR, STEP_ZONE_VISEUR);
		rectviseur = CRect(upleft, CPoint(croixviseur + croixviseur - upleft));
		rectviseur.NormalizeRect();

		if(m_WasNotOpen)
			return;
		else
		{
			CheckDlgButton(IDC_CAM_CHECK_GRAB, false);
			m_WantsToReGrabLive = true;
		}
	}
	catch( BcamException &e )
	{	CString Buffer, B;
		Buffer += (B.Format("Exception 0x%X occurred\n", e.Error() ), B);
		Buffer += (B.Format("Message = %s\n", e.Description() ), B);
		Buffer += (B.Format("Context = %s\n", e.Context()), B);
		MessageBox(Buffer, _T("OnSetShutter"), MB_OK | MB_ICONEXCLAMATION);	}
	ValueChangedCamCheckTrig(WasTrigged);
return;
}

void CCamDialog::ValueChangedCamCheckAoiAll(BOOL Value)
{	if(Value)
		return;
	CBcam & TheCam = m_Bcam_1;
	try
	{	// Check for a camera
		if(CBcam::DeviceNames().size() == 0)
		{	return;	}
		bool m_WasNotOpen = !TheCam.IsOpen();
		if(m_WasNotOpen)
		{	
			m_WasNotOpen=true;
			// Get the device name of the first camera
	        CString DeviceName = *(CBcam::DeviceNames().begin());
	        // Create the driver object and open the driver
			TheCam.Open( DeviceName );	
		}
		// si la taille était déjà max on ne fait rien
		if(m_AoiSize == TheCam.FormatSeven[DCS_Mode0].MaxSize())
			;
		else
		{
			CRect& rectviseur = m_pMainFrame->m_FitDialog.m_ZoneViseur;
			CPoint& croixviseur = m_pMainFrame->m_FitDialog.m_CentreCroix;
			// La Zone viseur reste au meme endroit de l'image: il faut ajouter un offset pour l'amener au bon endroit dans le contxte d'affichage
			croixviseur += m_AoiUpLeft;
			CPoint upleft = m_AoiUpLeft/* + rectviseur.TopLeft()*/;

			m_AoiSize = TheCam.FormatSeven[DCS_Mode0].MaxSize();
			m_AoiUpLeft = CPoint(0,0);

			rectviseur = CRect(upleft, CPoint(croixviseur + croixviseur - upleft));
			rectviseur.NormalizeRect();
		}

		if(m_WasNotOpen)
		{	m_WasNotOpen=false;
			// Get the device name of the first camera
			CString DeviceName = *(CBcam::DeviceNames().begin());
			// Create the driver object and open the driver
			TheCam.Close();	
		}
		else
		{
			m_WantsToReGrabLive = true;
			CheckDlgButton(IDC_CAM_CHECK_GRAB, false);
		}
		m_View.Invalidate();
	} 
	catch( BcamException &e )
	{	CString Buffer, B;
		Buffer += (B.Format("Exception 0x%X occurred\n", e.Error() ), B);
		Buffer += (B.Format("Message = %s\n", e.Description() ), B);
		Buffer += (B.Format("Context = %s\n", e.Context()), B);
		MessageBox(Buffer, _T("OnSetShutter"), MB_OK | MB_ICONEXCLAMATION);	}
return;
}


unsigned int CCamDialog::CompteurPrecisMillisecondes(bool ResetToZero /* = false */)
{
	LARGE_INTEGER Ticks, Freq;
	QueryPerformanceCounter(&Ticks); // renvoye le nombre de ticks du High-resolutin-counter
	QueryPerformanceFrequency(&Freq);// et sa fréquence
	unsigned int LesMilliSec = (unsigned int)(Ticks.QuadPart / (Freq.QuadPart / 1000.0));

	int DepuisDernierReset = LesMilliSec - m_CompteurPrecis_T0;
	m_CompteurPrecisMillisecondesSinceLastCall = DepuisDernierReset - m_CompteurPrecisMillisecondesSinceLastReset;
	m_CompteurPrecisMillisecondesSinceLastReset = DepuisDernierReset;
	if(ResetToZero)
	{
		m_CompteurPrecis_T0 = LesMilliSec;
		m_CompteurPrecisMillisecondesSinceLastReset = 0;
	}
return m_CompteurPrecisMillisecondesSinceLastReset;
}