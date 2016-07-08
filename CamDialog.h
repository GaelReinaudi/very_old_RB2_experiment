#pragma once

#include "stdafx.h"
#include "ProDialog.h"

#include "TraitImageDialog.h"

using namespace std;
using namespace Bcam;

//#include "CBcamTopologyViewCtl.h"

enum{Mono8Bits, Mono16Bits};
enum{SetByUser, TriggerWindow};
enum{CadranAuto = -1};

// CCamDialog

//! Use OneShot (=true) or ContinuousShot(=false) for grabbing
const bool USE_ONESHOT = false;
//! Number of buffers used for grabbing
const unsigned int NUM_BUFFERS = 2;
//
const UINT MyWM_MESSAGE = WM_USER * 2;
//! Message fired by the GrabThread if the grab is finshed
const UINT WM_GRAB_FINISHED = MyWM_MESSAGE;
//! Message fired by the GrabThread if the user stops the grabbing
const UINT WM_GRAB_STOPPED = MyWM_MESSAGE + 1;
//! Message fired by the GrabThread if something has gone wrong
const UINT WM_ERROR = MyWM_MESSAGE + 2;


class CCamDialog : public CProDialog
{
	DECLARE_DYNAMIC(CCamDialog)

public:
	CCamDialog();
	virtual ~CCamDialog();

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

	void SetSerieBitmapCam(CDibImagePtr& PtrDib, int& Cadran);

protected:
	// Definit si trig, et quel type de TriggerMode, Expo, Polarité , ....
	int m_WaitTrigger;
	int m_ExpoMode;
	int m_TrigPolar;

	unsigned long m_CamExpo;
	unsigned long m_CamGain;
	// le nombre de nuance de gris : 8bit ou 12bit (qui sont les 12 premiers d'un 16bit)
	unsigned int m_BitPerPixels;

	// Les Bitmaps

	// Le coin superieur gauche de l'AOI en unité pixel
	CPoint m_AoiUpLeft;
	// La taille de l'AOI en unité pixel
	CSize m_AoiSize;
	// zoom
	int m_ZoomFit;
	// la taille d'un cadran
	CSize m_TailleCadran;
	// la taille en pixel de la partie de l'image effectivemant vue.
	CSize m_TailleImageVue;

CDibImagePtr m_ThePtrDib[NUM_BUFFERS+1]; 

	CSerieImage m_SerieCam;

public:
	// L'objet BCam
	CBcam m_Bcam_1;
	//! True if live grabbing is under way
	bool m_LiveGrabbing;
	// un truc qui dit que tu veux relancer le livegrabbing apres qu'il se soit arrété, 
	// pour actualiser des parametres de la caméra par exemple.
	bool m_WantsToReGrabLive;
	//! Index of the buffer which is currently displayed (-1 = none)
	int m_DisplayedBufferIndex;
	//! Queue holding the buffers recwived
	queue<unsigned int> m_BufferQueue;
	//! Thread waiting for grab commands to succeed
	CThread m_GrabThread;
	//! The function of the thread waiting for grab commands to succeed
	static DWORD WINAPI GrabThreadProc(void* pParameter);
	//! Used to measure the frame rate
	CStopWatch m_StopWatch;
	//! Frame rate
	CMovingAvg<double> m_Fps;

	protected:
  //{{AFX_MSG(CMainFrame)
  //! Check if someonewants us to die
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  
  //! Fired if the application should exit
  afx_msg void OnAppExit();
  
  //! Fired if grab is to be started
  afx_msg void OnGrabLive(int NumberOfPictures = 0);
  
  //! Fired if the GrabThread sends a messages indicating that the grab is finished
  afx_msg LRESULT OnGrabFinished(WPARAM wParam, LPARAM lParam);
  
  //! Fired if grab is to be finished
  afx_msg void OnGrabStop();
  
  //! Fired if the GrabThread sends a messages indicating that it has terminated
  afx_msg LRESULT OnGrabStopped(WPARAM wParam, LPARAM lParam);
  
  //! Fired if an error in a thread occurred
  afx_msg LRESULT OnError(WPARAM wParam, LPARAM lParam);
  //}}AFX_MSG

	// L'objet TopologyViewer, control dans la DialogBar qui montre l'arborescence des caméras.
//	CBcamTopologyViewCtl m_BcamTopo;

	CComboBox m_ComboTrigMode;
	CComboBox m_ComboBits;
	int m_NbrComboBit;
	int m_NbrComboTrigMode;
	CComboBox m_ComboComputStatus;
	CNiSlide m_NiSlideExpo;
	CNiSlide m_NiSlideGain;
	CNiNumEdit m_NiEditExpo;
	CNiNumEdit m_NiEditGain;
	CNiButton m_NiCheckTrig;

	DECLARE_EVENTSINK_MAP()
public:
	void ValueChangedCamCheckGrab(BOOL Value);
	void ValueChangedCamCheckSnap(BOOL Value);
	bool ConnectToCam(CBcam* pCam = NULL);

public:
	//! Called by the applicaition object if the message loop runs dry
	BOOL OnIdle( LONG lCount );

	afx_msg void OnCbnSelchangeComboCamTriggerMode();
	afx_msg void OnCbnSelchangeComboCamBits();
	afx_msg void OnCbnSelchangeComboCamComputerStatus();

	int UpdateControls();
	LRESULT OnSetShutter();//WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSetGain();//WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void PointerValueChangedCamSlideExpo(long Pointer, VARIANT* Value);
	void PointerValueChangedCamSlideGain(long Pointer, VARIANT* Value);
	void ValueChangedCamEditExpo(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedCamEditGain(VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	// fonction de conversion m_Gain --> dB dans une string qui se finit par " dB"
	double CamGainVERSgain_dB(int mG);
	// idem expo
	double CamExpoVERSexpo_ms(int mG);


	void ValueChangedCamCheckTrig(BOOL Value);
	void ValueChangedCamCheckAoiZone(BOOL Value);
	void ValueChangedCamCheckAoiAll(BOOL Value);

	// nombre de milliseconde depuis l'allumage du PC...
	unsigned int m_CompteurPrecis_T0;
	// nombre de milliseconde depuis la derniere fois ou on a reseter le compteur
	unsigned int m_CompteurPrecisMillisecondesSinceLastReset;
	// nombre de milliseconde depuis la derniere fois ou on a appellé Compteur Precis Millisecondes()
	unsigned int m_CompteurPrecisMillisecondesSinceLastCall;
private:
	// renvoye le nombre de milliseconde depuis le dernier Reset()
	unsigned int CompteurPrecisMillisecondes(bool ResetToZero = false ); 
	int m_NbrImageWithoutTimeOut;

	friend class CTraitImageDialog;
	friend class CMainFrame;
};


