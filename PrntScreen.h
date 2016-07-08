/////////////////////////////////////////////////////////////////////////////
//
// CPrntScreen Class : A Print Screen Class
// ----------------------------------------
//
/////////////////////////////////////////////////////////////////////////////
//
// PrntScreen.h : header file
//
// Version 1.2
//
// Changelog:
//   Bugfix in stretching function, which caused empty pages in some printers
/////////////////////////////////////////////////////////////////////////////
#pragma once
class CPrntScreen
{
    public:
       CPrntScreen(char* sErrTxt=NULL, char* sCaption=NULL);
       virtual ~CPrntScreen();
       void DoPrntScreen(int fArea=0, int fOrientation=2, bool bDialog=false, CWnd* pTheDialogToPrint=NULL, CPoint From=CPoint(0,0), CPoint To=CPoint(0,0));

    protected:
       HANDLE DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal );
       char * sErrortext;
       char * sMsgCaption;
};
