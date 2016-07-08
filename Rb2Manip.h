// Rb2Manip.h : fichier d'en-tête principal pour l'application Rb2Manip
//
#pragma once

#ifndef __AFXWIN_H__
	#error inclut 'stdafx.h' avant d'inclure ce fichier pour PCH
#endif

#include "resource.h"       // symboles principaux

#include "stdafx.h"
#include "Rb2Doc.h"
#include "Rb2View.h"


// CRb2ManipApp :
// Consultez Rb2Manip.cpp pour l'implémentation de cette classe
//

class CRb2ManipApp : public CWinApp
{
/////////////////////////// CF : Doc&Astuces  : "CView Access From Anywhere"
public:
	CRb2View* m_pView;
	CRb2View* m_pView2;
	CRb2View* GetMyView();
	CRb2View* GetMyView2();
	void StoreMyView(CRb2View* pView);
/////////////////////////// End CF : Doc&Astuces  : "CView Access From Anywhere"

public:
	CRb2ManipApp();
	CSettingSave Rb2ManipWinSave;

// Substitutions
public:
	virtual BOOL InitInstance();

// Implémentation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);

/*PourPictureBox*/// /////////// Juste pour utiliser GdiPlus 
/*PourPictureBox*/// private:
/*PourPictureBox*/// 	//not used! just for the initialising of gdiplus
/*PourPictureBox*/// 	GdiplusStartupInput m_gdiplusStartupInput;
/*PourPictureBox*/// 	ULONG_PTR m_pGdiToken;

public:
	virtual int ExitInstance();
};

extern CRb2ManipApp theApp;
