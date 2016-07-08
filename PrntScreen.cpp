/////////////////////////////////////////////////////////////////////////////
//
// CPrntScreen Class : A Print Screen Class
// ----------------------------------------
//
// Version 1.2
/////////////////////////////////////////////////////////////////////////////
//
// PrntScreen.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// How to use:
// In your application, add
//     #include "PrntScreen.h"
//
// For capturing and printing, add:
//
//     CPrntScreen * ScrCap;
//     ScrCap = new CPrntScreen("Impossible to print!","Error!");
//     ScrCap->DoPrntScreen(1,0,true);
//     delete ScrCap;
//     ScrCap = NULL;
//
//  or:
//
//     CPrntScreen * ScrCap;
//     ScrCap = new CPrntScreen();
//     ScrCap->DoPrntScreen();
//     delete ScrCap;
//     ScrCap = NULL;
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "PrntScreen.h"


// --------------------------------------------------------------<CPrntScreen>---
//                              CPrntScreen::CPrntScreen
// Effect: Constructor
//
// Parameters:
//
//       char* sErrTxt:  Pointer to the text to be shown in a message box
//                       in case, printing went wrong.
//                       Omitting or passing NULL uses default text.
//
//       char* sCaption: Pointer to text for the Caption of MessageBox,
//                       which will be shown, if printing went wrong.
//                       Omitting or passing NULL uses default caption.
// ------------------------------------------------------------------------------

CPrntScreen::CPrntScreen(char* sErrTxt, char * sCaption)
{
  if (sErrTxt!=NULL)
    sErrortext=sErrTxt;
  else
    sErrortext="Unable to print screen!";

  if (sCaption!=NULL)
    sMsgCaption=sCaption;
  else
    sMsgCaption="CPrntScreen";
}


// -------------------------------------------------------------<~CPrntScreen>---
//                              CPrntScreen::~CPrntScreen
// Effect: Destructor
// ------------------------------------------------------------------------------

CPrntScreen::~CPrntScreen()
{
}


// --------------------------------------------------------------<DoPrntScreen>---
//  CPrntScreen::DoPrntScreen()
//  ---------------------------
//
//  Effect: Capture the desktop screen and prints to default printer
//
//  Parameters:
//      int fArea:        determines the area of screen to be captured;
//                        can be 0, 1 or 2:
//                        0 = Whole screen (Default)
//                        1 = Foreground window
//                        2 = Foreground window - Client area
//
//      int fOrientation: determines the print orientation;
//                        can be:  1 or 2
//                        1 (DMORIENT_PORTRAIT)  = Portrait (Default)
//                        2 (DMORIENT_LANDSCAPE) = Landscape
//                        This parameter is only used, if bDialog=false
//
//      bool bDialog:     Flag, which determines, if a printer selection
//                        dialog should be shown  or the default
//                        printer shall be used.
//                        false = use default printer (Default)
//                        true  = show printer dialog
//
// -----------------------------------------------------------------------------
void CPrntScreen::DoPrntScreen(int fArea, int fOrientation, bool bDialog, CWnd* pTheDialogToPrint, CPoint From/*(0,0)*/, CPoint To/*(0,0)*/)
{
  CDC     screenDC,memDC;
  HDC     hscreenDC;
  HWND    wHndle;
  CSize   sizeClient, sizeScreen;
  RECT    rectClient;
  POINT   pt1, pt2;
  int     nX, nY, nX2, nY2;       // coordinates of rectangle to grab

  //determine the area of screen to be captured
  //-------------------------------------------
  switch (fArea)
  {
  case 0:
      //Capture Whole screen
      wHndle=NULL;
      hscreenDC = ::GetDC(wHndle);
      screenDC.Attach(hscreenDC);
      sizeClient.cx = screenDC.GetDeviceCaps(HORZRES);
      sizeClient.cy = screenDC.GetDeviceCaps(VERTRES);
      sizeScreen.cx = screenDC.GetDeviceCaps(HORZRES);
      sizeScreen.cy = screenDC.GetDeviceCaps(VERTRES);
      screenDC.DPtoLP(&sizeScreen);
      break;

  case 1:
      //Determine screen size
      wHndle=NULL;
      hscreenDC = ::GetDC(wHndle);
      screenDC.Attach(hscreenDC);
      sizeScreen.cx = screenDC.GetDeviceCaps(HORZRES);
      sizeScreen.cy = screenDC.GetDeviceCaps(VERTRES);
      screenDC.DPtoLP(&sizeScreen);
      screenDC.Detach();
      ReleaseDC(wHndle,hscreenDC);

      // Get Foreground window
      wHndle=GetForegroundWindow();
      hscreenDC = ::GetWindowDC(wHndle);
      screenDC.Attach(hscreenDC);

      // Calculate resolution of capture area (Window)
      sizeClient.cx = screenDC.GetDeviceCaps(HORZRES);
      sizeClient.cy = screenDC.GetDeviceCaps(VERTRES);
      GetWindowRect(wHndle,&rectClient);
      nX    = rectClient.left;
      nY    = rectClient.top;
      nX2   = rectClient.right;
      nY2   = rectClient.bottom;

      //Let's check if dialog was pulled partially outside of view
      if ((nX < 0)||(nY < 0)||(nX2 > sizeClient.cx) || (nY2 > sizeClient.cy))
      {
        //in this case, we will force the dialog to screen center first
        
        HWND hwndOwner; 
        RECT rc, rcDlg, rcOwner; 
 
        // Get the desktop and dialog box rectangles. 
        hwndOwner = GetDesktopWindow(); 
        GetWindowRect(hwndOwner, &rcOwner); 
        GetWindowRect(wHndle, &rcDlg); 
        CopyRect(&rc, &rcOwner); 
 
        // Offset the owner and dialog box rectangles so that 
        // right and bottom values represent the width and 
        // height, and then offset the owner again to discard 
        // space taken up by the dialog box. 
        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top); 
        OffsetRect(&rc, -rc.left, -rc.top); 
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom); 

        // The new position is the sum of half the remaining 
        // space and the owner's original position. 
        SetWindowPos(wHndle, 
            HWND_TOP, 
            rcOwner.left + (rc.right / 2), 
            rcOwner.top + (rc.bottom / 2), 
            0, 0,          // ignores size arguments 
            SWP_NOSIZE); 

        //Update window
        UpdateWindow(wHndle);

        //and retrieve rectangles again
        GetWindowRect(wHndle,&rectClient);
        nX    = rectClient.left;
        nY    = rectClient.top;
        nX2   = rectClient.right;
        nY2   = rectClient.bottom;
      }

      //Limit to screen size
      if (nX < 0) nX = 0;
      if (nY < 0) nY = 0;
      if (nX2 > sizeClient.cx) nX2 = sizeClient.cx;
      if (nY2 > sizeClient.cy) nY2 = sizeClient.cy;
      sizeClient.cx = nX2 - nX;
      sizeClient.cy = nY2 - nY;
      break;

  case 2:
      //Determine screen size
      wHndle=NULL;
      hscreenDC = ::GetDC(wHndle);
      screenDC.Attach(hscreenDC);
      sizeScreen.cx = screenDC.GetDeviceCaps(HORZRES);
      sizeScreen.cy = screenDC.GetDeviceCaps(VERTRES);
      screenDC.DPtoLP(&sizeScreen);
      screenDC.Detach();
      ReleaseDC(wHndle,hscreenDC);

      // Get Foreground window - Client area
      wHndle=GetForegroundWindow();
      hscreenDC = ::GetDC(wHndle);
      screenDC.Attach(hscreenDC);

      // Calculate resolution of capture area (Client)
      sizeClient.cx = screenDC.GetDeviceCaps(HORZRES);
      sizeClient.cy = screenDC.GetDeviceCaps(VERTRES);
      GetClientRect(wHndle,&rectClient);
      nX    = rectClient.left;
      nY    = rectClient.top;
      nX2   = rectClient.right;
      nY2   = rectClient.bottom;

      //Let's check if dialog was pulled partially outside of view
      if ((nX < 0)||(nY < 0)||(nX2 > sizeClient.cx) || (nY2 > sizeClient.cy))
      {
        //in this case, we will force the dialog to screen center first
        
        HWND hwndOwner; 
        RECT rc, rcDlg, rcOwner; 
 
        // Get the desktop and dialog box rectangles. 
        hwndOwner = GetDesktopWindow(); 
        GetWindowRect(hwndOwner, &rcOwner); 
        GetWindowRect(wHndle, &rcDlg); 
        CopyRect(&rc, &rcOwner); 
 
        // Offset the owner and dialog box rectangles so that 
        // right and bottom values represent the width and 
        // height, and then offset the owner again to discard 
        // space taken up by the dialog box. 
        OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top); 
        OffsetRect(&rc, -rc.left, -rc.top); 
        OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom); 

        // The new position is the sum of half the remaining 
        // space and the owner's original position. 
        SetWindowPos(wHndle, 
            HWND_TOP, 
            rcOwner.left + (rc.right / 2), 
            rcOwner.top + (rc.bottom / 2), 
            0, 0,          // ignores size arguments 
            SWP_NOSIZE); 

        //Update window
        UpdateWindow(wHndle);

        //and retrieve rectangles again
        GetWindowRect(wHndle,&rectClient);
      }

      //Limit client area and to screen size
      pt1.x = rectClient.left;
      pt1.y = rectClient.top;
      pt2.x = rectClient.right;
      pt2.y = rectClient.bottom;
      ClientToScreen(wHndle,&pt1);
      ClientToScreen(wHndle,&pt2);
      nX  = pt1.x;
      nY  = pt1.y;
      nX2 = pt2.x;
      nY2 = pt2.y;
      if (nX < 0) nX = 0;
      if (nY < 0) nY = 0;
      if (nX2 > sizeClient.cx) nX2 = sizeClient.cx;
      if (nY2 > sizeClient.cy) nY2 = sizeClient.cy;
      sizeClient.cx = nX2 - nX;
      sizeClient.cy = nY2 - nY;
      break;

  case 3:	// par Gael
      //Capture Dialog
	  if(!pTheDialogToPrint)
		  MessageBox(NULL,_TEXT(sErrortext),_TEXT(sMsgCaption),MB_OK|MB_ICONERROR);
	  wHndle= pTheDialogToPrint->m_hWnd;
	  hscreenDC = ::GetDC(wHndle);
	  screenDC.Attach(hscreenDC);
	  sizeClient.cx = To.x ? To.x : 1500;
	  sizeClient.cy = To.y ? To.y : 1500;

	  sizeScreen.cx = screenDC.GetDeviceCaps(HORZRES);
	  sizeScreen.cy = screenDC.GetDeviceCaps(VERTRES);

	  screenDC.DPtoLP(&sizeScreen);
      break;

  default:
     return;
  }

  //Screen Capture to image
  CBitmap *oldImage = new CBitmap,srcImage;
  memDC.CreateCompatibleDC(&screenDC);
  screenDC.DPtoLP(&sizeClient);
  srcImage.CreateCompatibleBitmap(&screenDC,sizeClient.cx,sizeClient.cy);
  oldImage = memDC.SelectObject(&srcImage);
  memDC.BitBlt(0,0,sizeClient.cx,sizeClient.cy,&screenDC,0,0,SRCCOPY);
  screenDC.Detach();
  ReleaseDC(wHndle,hscreenDC);


  // Here starts the printing part
  //-------------------------------
  double zoomx, zoomy, scrXY, prnXY;
  PRINTDLG pd;
  LPDEVMODE lpDevMode;
  CSize sizePrn;
  DOCINFO di;
  CDC *pDC = new CDC;

  //Clear PrintDialog structure
  memset( &pd, 0, sizeof(PRINTDLG) ) ;

  //Fill out PrintDialog structure
  pd.lStructSize = sizeof(PRINTDLG);
  pd.hDC = NULL;
  pd.hwndOwner = NULL;
  pd.hInstance = NULL;
  pd.nMaxPage = 1;
  pd.nMinPage = 1;
  pd.nFromPage = 1;
  pd.nToPage = 1;
  pd.nCopies = 1;
  pd.hDevMode = NULL;
  pd.hDevNames = NULL;
  if (bDialog)
    pd.Flags = PD_RETURNDC | PD_USEDEVMODECOPIESANDCOLLATE;
  else
    pd.Flags = PD_RETURNDC | PD_RETURNDEFAULT;

  //Call PrintDialog
  if(PrintDlg(&pd))
  {
    pDC->Attach(pd.hDC);

    //Check, if printer is capable for raster operations
    int iRasterCaps=GetDeviceCaps(pd.hDC,RASTERCAPS);
    if  ((iRasterCaps&RC_STRETCHBLT)==0)
    {
      //No, it isn´t, so error message and off we are
      MessageBox(NULL,_TEXT(sErrortext),_TEXT(sMsgCaption),MB_OK|MB_ICONERROR);
    }
    else
    { // OK so far, now we can start printing

      // Only if default printer is used, we adjust 
      // the orientation as given by parameter
      if (!bDialog)
      {
          //Check if orientation parameter is valid
          if ((fOrientation!=DMORIENT_LANDSCAPE)&&(fOrientation!=DMORIENT_PORTRAIT))
            fOrientation=DMORIENT_PORTRAIT;

          // Adjust desired orientation
          lpDevMode = (LPDEVMODE)::GlobalLock(pd.hDevMode);
          if(lpDevMode != NULL)
          {
            lpDevMode->dmOrientation=(short)fOrientation;
            pDC->ResetDC(lpDevMode);
            ::GlobalUnlock(pd.hDevMode);
          }
      }

      // Get printers page dimensions
      sizePrn.cx = pDC->GetDeviceCaps(HORZRES);
      sizePrn.cy = pDC->GetDeviceCaps(VERTRES);

      //Clean page
      pDC->PatBlt( 0, 0, sizePrn.cx, sizePrn.cy, WHITENESS);

      // Set magnitude of stretching 
      // (in relation of screen to captured area to page size)
      scrXY = (double)sizeScreen.cx/(double)sizeScreen.cy;
      prnXY = (double)sizePrn.cx/(double)sizePrn.cy;
      if (scrXY>prnXY) 
      {
        zoomx = ((double)sizeClient.cx/(double)sizeScreen.cx);
        zoomy = ((double)sizeClient.cy/(double)sizeScreen.cy)*(prnXY/scrXY);
      }
      else
      {
        zoomx = ((double)sizeClient.cx/(double)sizeScreen.cx)*(scrXY/prnXY);
        zoomy = ((double)sizeClient.cy/(double)sizeScreen.cy);
      }

      //Limit to zoom x1
      if (zoomx>1.0)
      { 
        zoomy=zoomy/zoomx;
        zoomx=1.0;
      }
      if (zoomy>1.0)
      { 
        zoomx=zoomx/zoomy;
        zoomy=1.0;
      }

      //now set OUR page size
      sizePrn.cx = (long)(sizePrn.cx*zoomx);
      sizePrn.cy = (long)(sizePrn.cy*zoomy);
      pDC->LPtoDP(&sizePrn);

      // Start printing
      di.cbSize = sizeof(DOCINFO);
      di.lpszDocName = sMsgCaption;
      di.lpszOutput = (LPTSTR) NULL;
      di.lpszDatatype = (LPTSTR) NULL;
      di.fwType = 0;

      ::StartDoc(pd.hDC, &di);
      ::StartPage(pd.hDC);

      //This was the old, not always working version. 
      // pDC->StretchBlt(0,0,sizePrn.cx,sizePrn.cy,&memDC,
      //                 0,0,sizeClient.cx,sizeClient.cy,SRCCOPY); 


      //This is the new version following the suggestion of Dieter Hammer (thanks). 
      //I shamlessly copied his code here.
      //---------------------------------------------------------------------------
      //Copy the bitmap (Device dependant bitmap or DDB)
      //to a Device indepedant bitmap (DIB)
      CPalette pal;
      LPVOID lpDIBBits;
      LPBITMAPINFOHEADER lpbi;
      HANDLE hDIB = DDBToDIB(srcImage, BI_RGB, &pal );
      lpbi = (LPBITMAPINFOHEADER)hDIB;
      int nColors = lpbi->biClrUsed ? lpbi->biClrUsed : 1 << lpbi->biBitCount;
      BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB;
      if( bmInfo.bmiHeader.biBitCount > 8 )
      {
        lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors 
                    + bmInfo.bmiHeader.biClrUsed) 
                    + ((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
      }
      else
      {
        lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
      }
      HDC hDC = pDC->GetSafeHdc();

      //Copy de DIB to the printer device
      //Using DIB rather than DDB because DDB can
      //have an unpredicable behavior with network printer !!!!!
      StretchDIBits(
                     hDC,              // hDC
                     0,                // DestX
                     0,                // DestY
                     (int) sizePrn.cx, // nDestWidth
                     (int) sizePrn.cy, // nDestHeight
                     0, // SrcX
                     0, // SrcY
                     sizeClient.cx,    // wSrcWidth
                     sizeClient.cy,    // wSrcHeight
                     lpDIBBits,        // lpBits
                     &bmInfo,          // lpBitsInfo
                     DIB_RGB_COLORS,   // wUsage
                     SRCCOPY);         // dwROP

      ::EndPage(pd.hDC);
      ::EndDoc(pd.hDC);
      GlobalFree(hDIB);
    }
  }
  else if ((!bDialog)||(CommDlgExtendedError()!=0))
  {
     MessageBox(NULL,_TEXT(sErrortext),_TEXT(sMsgCaption),MB_OK|MB_ICONERROR);
  }

  //Cleanup
  DeleteDC(pd.hDC);
  delete pDC;
  memDC.SelectObject(oldImage);

}


//This below code is entirely from Zafir Anjum, posted August 5, 1998 at CodeGuru.
//See http://www.codeguru.com/Cpp/G-M/bitmap/article.php/c1765 for details.
//
HANDLE CPrntScreen::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal ) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				                  // Start scan line
				(DWORD)bi.biHeight,	    	// # of scan lines
				(LPBYTE)lpbi 	         		// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		    // address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	ReleaseDC(NULL,hDC);
	return hDIB;
}
