#include "stdafx.h"

#include "SerieImage.h"
#include "math.h"

#include <io.h>

CString CSerieImage::m_FirstImagesPathsForSeries[128];
int CSerieImage::m_NbrSeries = 0;
int CSerieImage::m_IndexSerieCourante  = 0;
bool CSerieImage::m_AbsCompute = true;
bool CSerieImage::m_OverWrite = false;
int CSerieImage::m_CalculationType = ClassicalAbsorption;
double CSerieImage::m_RangeAbsSerie = 0.0;
double CSerieImage::m_PixelSizeSerie = 1.0;
double CSerieImage::m_ZoomEcranImage = 1.0;

double CSerieImage::m_IsatSurIsat0Serie   = 1.0;
double CSerieImage::m_Isat0Serie          = 1.6;
double CSerieImage::m_PulseLengthSerie    = 1e-3;
double CSerieImage::m_AttenBeforeCamSerie = 0.0;

bool CSerieImage::m_ModeAbsorp = false;

double CSerieImage::m_PhotonParBitSur255 = 365.0;
double CSerieImage::m_PhotonEnergie_J =  2.54593761e-19;

#define NBR_CHIFFRE_SAVE 5
#define NBR_CHIFFRE_OPEN 6

CSerieImage::CSerieImage(void)
: m_FirstIndex(0), m_LastIndex(0), m_BaseName(""), m_FolderPath(""), m_ImageCourante(COURANTE)
, m_Size(0,0), m_FitAllreadyComputed(false), m_pResultatCalculImageY(NULL), m_MustSavedCadran(DontSave)

{
 
}

CSerieImage::~CSerieImage(void)
{
	PrepareCleanResultatCalculImage();
	ReleaseBitmaps();
}

int CSerieImage::New(CString Path, bool ForSaving /* = false */, int nbrImageToSave /* = 1 */)
{
	char folder[2048];
	sprintf(folder, "%s", Path);

	char* FilePos = &(folder[0]);		

	FilePos = strrchr(FilePos,'\\')+1;		// trouve le dernier antislash
	*(strrchr(FilePos,'.')) = '\0';			// élimine l'extension .bmp
	m_BaseName = CString(FilePos);			// actualise m_BaseName
	CString base = TrouveBase(FilePos);		// élimine le chiffre éventuel d'index

	if(!ForSaving)
		m_FirstIndex = TrouveIndex(FilePos);
	else
		m_FirstIndex = 0;

	*(FilePos) = '\0';	// élimine le "fichier"
	m_FolderPath = folder;

	m_LastIndex = TrouveLastIndex(base, m_FolderPath);

	int mode = 0;
	if(!ForSaving)
	{
		m_BaseName = TrouveBase(m_BaseName);
		mode = OuvreImage(FIRST);
	}
	else
	{
		if(m_BaseName.ReverseFind('_') != m_BaseName.GetLength() - 1)
			m_BaseName += "_";
		if(nbrImageToSave == 3)
		{
			mode = ImageAbs;
			SetModeAbsorp(true);
		}
	}
return mode;
}


int CSerieImage::FindAllSeriesOfFolder()
{
	// cherchons dans tous les .bmp du dossier
	CFileFind f;
	CString recherche;
	recherche = m_FolderPath + "*.bmp";
	BOOL bMoreFiles = f.FindFile(recherche);
	CString AllTheBaseNames_FromTo("");

	char filename[4096];
	CString BaseCourante("");
	int FirstOne = 0;
	int LastOne = 0;

	m_NbrSeries = 0;
	// tant qu'il y en a
	if(bMoreFiles)
	{
		while (bMoreFiles)
		{
			bMoreFiles = f.FindNextFile();
			sprintf(filename,f.GetFileName());
			*(strrchr(filename,'.')) = '\0';// élimine l'extension .bmp

			// quand on trouve un nouveau nom de base
			if(BaseCourante != TrouveBase(filename))
			{	
				FirstOne = TrouveFirstIndex(TrouveBase(filename), m_FolderPath);
				LastOne =  TrouveLastIndex( TrouveBase(filename), m_FolderPath);

				// on l'ajoute à la liste
				char tempchar[10];
				BaseCourante = TrouveBase(filename);
				m_FirstImagesPathsForSeries[++m_NbrSeries] = m_FolderPath + CString(filename) + ".bmp";
				AllTheBaseNames_FromTo += BaseCourante;
				AllTheBaseNames_FromTo += "\n\tFrom\t";
				AllTheBaseNames_FromTo += CString(_ltoa(FirstOne, tempchar, 10)) + "";
				AllTheBaseNames_FromTo += "\tTo\t";
				AllTheBaseNames_FromTo += CString(_ltoa(LastOne, tempchar, 10)) + "\n";
			}
		}
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), CString("You Will Do The Next Fit Over All The Series In The Folder ! !\n\n") + AllTheBaseNames_FromTo, "Series", 1);
	}
	else 
	{
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "No pictures in this foder ! ", "SerieImage", 1);
		return m_NbrSeries;
	}

	sprintf(filename,"%s", m_FirstImagesPathsForSeries[1]);
	New(CString(filename));
return m_NbrSeries;
}

CString CSerieImage::TakeNextSerieInList()
{
	if(m_IndexSerieCourante >= m_NbrSeries)
	{
		Beep(1000,1000);
		m_NbrSeries = 0;
		return CString("");
	}

	m_IndexSerieCourante++;
	CString filename;
	filename = m_FirstImagesPathsForSeries[m_IndexSerieCourante];
	New(filename);
	return filename;
}

void CSerieImage::PrepareCleanResultatCalculImage(unsigned long nPoints /* = 0 */)
{
	// dans tout les cas, on nettoye
	if(m_pResultatCalculImageY != NULL)
	{
		delete m_pResultatCalculImageY;
		m_pResultatCalculImageY = NULL;
	}

	// si on fournit nPoints, on crée la place
	if(nPoints)
	{
		m_pResultatCalculImageY = new double[nPoints];
	}
}

int CSerieImage::OuvreImageFluo(int Unnumero /* = COURANTE */, CString Undossier /* = "" */ , CString Unbasename /* = "" */)
{
	PrepareCleanResultatCalculImage();
	int		numero		= Unnumero	;
	CString & dossier	= (Undossier  == "")	? m_FolderPath	: Undossier	;
	CString & basename	= (Unbasename == "")	? m_BaseName	: Unbasename;
	switch(numero)
	{	case COURANTE:	numero = m_ImageCourante;	break;
		case FIRST:		numero = m_FirstIndex;		break;
		case LAST:		numero = m_LastIndex;		break;	}
	char deszeros[16];
	sprintf(deszeros,"000000000000000");

	ReleaseBitmaps();
	// boucle pour ouvrir une image n° xx meme si elle est n° 000xx 
	// ex : "basename001" sera ouverte si on entre "numero = 1".
	int i = NBR_CHIFFRE_OPEN - int(log10(double(numero+0.5))) + 1;
	while(i--)
	{
		sprintf(deszeros,"000000000000000");
		*(deszeros + NBR_CHIFFRE_OPEN - i) = '\0';
		sprintf(m_PathName, "%s%s%s%d.bmp",dossier,basename,deszeros,numero);

		if( _access( m_PathName, 04 ) != -1 )
		{
			m_ptrBitmap[1].LoadBMP(m_PathName);
			m_ptrBitmap[1]->Topple();
			m_Size = m_ptrBitmap[1]->GetSize();
			m_ImageCourante = numero;
			return 1;
		}//if( _access( m_PathName, 04 ) != -1 )
	}//Des Zeros de 0 à NBR_CHIFFRE_OPEN 
return 0;
}

int CSerieImage::OuvreImageAbs(int Unnumero /* = COURANTE */, CString Undossier /* = "" */ , CString Unbasename /* = "" */)
{
	PrepareCleanResultatCalculImage();
	int		numero		= Unnumero	;
	CString & dossier	= (Undossier  == "")	? m_FolderPath	: Undossier	;
	CString & basename	= (Unbasename == "")	? m_BaseName	: Unbasename;
	switch(numero)
	{	case COURANTE:	numero = m_ImageCourante;	break;
		case FIRST:		numero = m_FirstIndex;		break;
		case LAST:		numero = m_LastIndex;		break;	}
	char deszeros[16];
	sprintf(deszeros,"000000000000000");
	char absorption[8];
	int absindex;
	int ExistAbsor = 0;		// indiquera que l'image Absor a déjà été calculée
	int ExistNbrBitmap = 0;	// indiquera que les 3 images bitmap nécéssaires à calculer l'Absor existent

	ReleaseBitmaps();

	for(absindex = 1; absindex <= 4; absindex++ )
	{
		int nCadran = absindex%4 + 1; // 2, 3, 4, puis 1
		switch(nCadran)
		{
		case 1:		sprintf(absorption,"Absor");	break;
		case 2:		sprintf(absorption,"_With");	break;
		case 3:		sprintf(absorption,"_NoAt");	break;
		case 4:		sprintf(absorption,"_Back");	break;
		}

		bool OkIFoundIt_NoNeedToDoTheOtherZeros = false;
		// boucle pour ouvrir une image n° xx meme si elle est n° 000xx 
		// ex : "basename001" sera ouverte si on entre "numero = 1".
		int i = NBR_CHIFFRE_OPEN - int(log10(double(numero+0.5))) + 1;
		while(i--)
		{
			sprintf(deszeros,"000000000000000");
			*(deszeros + NBR_CHIFFRE_OPEN - i) = '\0';

			sprintf(m_PathName, "%s%s%s%d%s.bmp",dossier,basename,deszeros,numero,absorption);

			// si on a deja trouvé le fichier bmp, on ne cherche plus avec tous les zero...
			if( !OkIFoundIt_NoNeedToDoTheOtherZeros )
				if( _access( m_PathName, 04 ) != -1)
				{
					OkIFoundIt_NoNeedToDoTheOtherZeros = true;

					switch(nCadran)
					{
					case 1:
						if(m_AbsCompute)	
							break;
						ExistAbsor = 1;
						m_ptrBitmap[1].LoadBMP(m_PathName);
						m_ptrBitmap[1]->Topple();
						m_Size = m_ptrBitmap[1]->GetSize();
						break;
					case 2:
						ExistNbrBitmap++;
						m_ptrBitmap[2].LoadBMP(m_PathName);
						m_ptrBitmap[2]->Topple();
						m_Size = m_ptrBitmap[2]->GetSize();
						break;
					case 3:
						ExistNbrBitmap++;
						m_ptrBitmap[3].LoadBMP(m_PathName);
						m_ptrBitmap[3]->Topple();
						m_Size = m_ptrBitmap[3]->GetSize();
						break;
					case 4:
						ExistNbrBitmap++;
						m_ptrBitmap[4].LoadBMP(m_PathName);
						m_ptrBitmap[4]->Topple();
						m_Size = m_ptrBitmap[4]->GetSize();
						break;
					}//switch(nCadran)

//					m_pMainFrame->m_wndStatusBar.SetPaneText(0,m_PathName);
					m_ImageCourante = numero;
				}// _access( m_PathName, 04 ) != -1
		}//Des Zeros de 0 à 7
	}//absindex de 0 à 3.

	if(ExistNbrBitmap < 3)
		MessageBeep(1);
	else if(m_AbsCompute)
	{
		CalculateAbsorptionImage(m_OverWrite);
	}
return ExistNbrBitmap;
}
int CSerieImage::OuvreImage(int Unnumero /* = COURANTE */, CString Undossier /* = "" */ , CString Unbasename /* = "" */)
{	
//	if(m_pMainFrame->m_Affichage & CAM)
//		return 0;

	int		numero		= Unnumero	;
	CString & dossier	= (Undossier  == "")	? m_FolderPath	: Undossier	;
	CString & basename	= (Unbasename == "")	? m_BaseName	: Unbasename;
	switch(numero)
	{	case COURANTE:	numero = m_ImageCourante;	break;
		case FIRST:		numero = m_FirstIndex;		break;
		case LAST:		numero = m_LastIndex;		break;
	}
	int ExistNbrBitmap = 0;	// indiquera que les 3 images bitmap nécéssaires à calculer l'Absor existent

	if(!m_ModeAbsorp) 	
	{
		OuvreImageFluo(numero, dossier, basename);
	}
	else
	{
		ExistNbrBitmap = OuvreImageAbs(numero, dossier, basename);
	}

	int valeurAretourner =  (!m_ModeAbsorp) ? 
							ImageFluo 
							:   (ExistNbrBitmap == 3) ? 
								ImageAbs 
								: ImageBad	;
	return valeurAretourner;
}

void CSerieImage::CalculateAbsorptionImage(bool ReSaveResultingImage /* = false */)
{
	switch(m_CalculationType)
	{
	case Soustraction_3m2To1:
		SoustraitBitmapAbs_3m2To1();
		break;
	case HighIntensityAbsorption:
		ComputeHighIntensityAbsorption();
		break;
	case ClassicalAbsorption:
		ComputeClassicalAbs_Ln3m4s2m4To1();
		break;
	}
	if(ReSaveResultingImage)
	{
		SaveAbsBitmap(SaveCadran_1);
	}
}
void CSerieImage::SoustraitBitmapAbs_3m2To1()
{    
	if(m_ptrBitmap[2].IsNull() || m_ptrBitmap[3].IsNull() || m_ptrBitmap[2]->GetSize() != m_ptrBitmap[3]->GetSize())	
	{ return;	}

	// On regrade si on a affaire à 8bit ou 12bit (16 en fait)
	unsigned short NbrBits = m_ptrBitmap[3]->GetBitsPerPixel();
	// Et on fixe la valeur de MaxWhiteValue qui correspond
	int MaxWhiteValue = 0;
	switch(NbrBits)
	{
	case 8:
		MaxWhiteValue = 255;
		break;
	case 16:
		MaxWhiteValue = 4095;
		break;
	}
	m_Size = m_ptrBitmap[3]->GetSize();
	long SizeX = m_Size.cx;
	long SizeY = m_Size.cy;
	unsigned long nTotalPixel = SizeX * SizeY;

	if(m_ptrBitmap[1].IsNull())
	{
		m_ptrBitmap[1].Create(m_ptrBitmap[2]->GetSize(),m_ptrBitmap[2]->GetBitsPerPixel(),m_ptrBitmap[2]->GetOrientation(),m_ptrBitmap[2]->GetPaletteType());
	}
	unsigned char* PtrSous_8bit = (unsigned char*) m_ptrBitmap[1]->GetPixels();
	unsigned char* Ptr1_8bit = (unsigned char*) m_ptrBitmap[3]->GetPixels();
	unsigned char* Ptr2_8bit = (unsigned char*) m_ptrBitmap[2]->GetPixels();
	unsigned short* PtrSous_16bit = (unsigned short*) m_ptrBitmap[1]->GetPixels();
	unsigned short* Ptr1_16bit = (unsigned short*) m_ptrBitmap[3]->GetPixels();
	unsigned short* Ptr2_16bit = (unsigned short*) m_ptrBitmap[2]->GetPixels();

    if(m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[3]->GetBitsPerPixel())
	{	AfxMessageBox("Soustraction : The pictures have not the same BitsPerPixel value !"); return;	}

	PrepareCleanResultatCalculImage(nTotalPixel);
	// par quoi va t'on multiplier la valeur trouvée pour la normaliser à m_RangeAbsSerie dans l'affichage
	double multipl_For_Displaying_RangeAbs = double(MaxWhiteValue) / m_RangeAbsSerie;
	double valeur;
	switch(NbrBits)
	{
	case 8:	
		for(unsigned long x = 0; x < nTotalPixel; ++x)
		{
			valeur = double(Ptr1_8bit[x]) - double(Ptr2_8bit[x]);
			PtrSous_8bit[x] = unsigned char(max(0, min(255.0, valeur * multipl_For_Displaying_RangeAbs)));
			m_pResultatCalculImageY[x] = valeur;
		}
		break;
	case 16:
		for(unsigned long x = 0; x < nTotalPixel; ++x)
		{
			valeur = double(Ptr1_16bit[x]) - double(Ptr2_16bit[x]);
			PtrSous_16bit[x] = unsigned char(max(0, min(4095.0, valeur * multipl_For_Displaying_RangeAbs)));
			m_pResultatCalculImageY[x] = valeur;
		}
		break;
	}
}

void CSerieImage::ComputeClassicalAbs_Ln3m4s2m4To1()
{    
	if(m_ptrBitmap[2].IsNull() || m_ptrBitmap[3].IsNull() || m_ptrBitmap[4].IsNull() || m_ptrBitmap[2]->GetSize() != m_ptrBitmap[3]->GetSize() || m_ptrBitmap[2]->GetSize() != m_ptrBitmap[4]->GetSize()
		 || m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[3]->GetBitsPerPixel() || m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[4]->GetBitsPerPixel())	
	{ return;	}

	// On regrade si on a affaire à 8bit ou 12bit (16 en fait)
	unsigned short NbrBits = m_ptrBitmap[3]->GetBitsPerPixel();
	// Et on fixe la valeur de MaxWhiteValue qui correspond
	int MaxWhiteValue = 0;
	switch(NbrBits)
	{
	case 8:
		MaxWhiteValue = 255;
		break;
	case 16:
		MaxWhiteValue = 4095;
		break;
	}
	m_Size = m_ptrBitmap[3]->GetSize();
	long SizeX = m_Size.cx;
	long SizeY = m_Size.cy;
	unsigned long nTotalPixel = SizeX * SizeY;

	if(m_ptrBitmap[1].IsNull() || m_ptrBitmap[1]->GetSize() != m_ptrBitmap[3]->GetSize() || m_ptrBitmap[1]->GetBitsPerPixel() != m_ptrBitmap[3]->GetBitsPerPixel())
	{
		m_ptrBitmap[1].Create(m_ptrBitmap[2]->GetSize(),m_ptrBitmap[2]->GetBitsPerPixel(),m_ptrBitmap[2]->GetOrientation(),m_ptrBitmap[2]->GetPaletteType());
	}
	unsigned char* PtrSous_8bit = (unsigned char*) m_ptrBitmap[1]->GetPixels();
	unsigned char* Ptr1_8bit = (unsigned char*) m_ptrBitmap[3]->GetPixels();
	unsigned char* Ptr2_8bit = (unsigned char*) m_ptrBitmap[2]->GetPixels();
	unsigned char* Ptr3_8bit = (unsigned char*) m_ptrBitmap[4]->GetPixels();
	unsigned short* PtrSous_16bit = (unsigned short*) m_ptrBitmap[1]->GetPixels();
	unsigned short* Ptr1_16bit = (unsigned short*) m_ptrBitmap[3]->GetPixels();
	unsigned short* Ptr2_16bit = (unsigned short*) m_ptrBitmap[2]->GetPixels();
	unsigned short* Ptr3_16bit = (unsigned short*) m_ptrBitmap[4]->GetPixels();

    if(m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[3]->GetBitsPerPixel() || m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[4]->GetBitsPerPixel())
	{	AfxMessageBox("Classic Absorption : The pictures have not the same BitsPerPixel value !"); return;	}

	PrepareCleanResultatCalculImage(nTotalPixel);
	// par quoi va t'on multiplier la valeur trouvée pour la normaliser à m_RangeAbsSerie dans l'affichage
	double multipl_For_RangeAbs = double(MaxWhiteValue) / m_RangeAbsSerie;
	double valeur, AvecAt, SansAt;
	switch(NbrBits)
	{
	case 8:	
		for(unsigned long x = 0; x < nTotalPixel; ++x)
		{
			SansAt = max(0.0, double(Ptr1_8bit[x]) - double(Ptr3_8bit[x]));
			AvecAt = max(0.0, double(Ptr2_8bit[x]) - double(Ptr3_8bit[x]));
			if(SansAt < 1.0 || AvecAt < 1.0)
				valeur = POINT_DISCARDED;
			else
			{
				valeur = -log(AvecAt / SansAt);
				valeur *= m_IsatSurIsat0Serie; // pour tout multiplier par m_IsatSurIsat0Serie cf article imagerie
			}
			PtrSous_8bit[x] = unsigned char(max(0, min(255.0, valeur * multipl_For_RangeAbs)));
			m_pResultatCalculImageY[x] = valeur;
		}
		break;
	case 16:
		for(unsigned long x = 0; x < nTotalPixel; ++x)
		{
			SansAt = max(0.0, double(Ptr1_16bit[x]) - double(Ptr3_16bit[x]));
			AvecAt = max(0.0, double(Ptr2_16bit[x]) - double(Ptr3_16bit[x]));
			if(SansAt < 1.0 || AvecAt < 1.0)
				valeur = POINT_DISCARDED;
			else
			{
				valeur = -log(AvecAt / SansAt);
				valeur *= m_IsatSurIsat0Serie; // pour tout multiplier par m_IsatSurIsat0Serie cf article imagerie
			}
			PtrSous_16bit[x] = unsigned short(max(0, min(4095.0, valeur * multipl_For_RangeAbs)));
			m_pResultatCalculImageY[x] = valeur;
		}
		break;
	}
	return;
}
void CSerieImage::ComputeHighIntensityAbsorption()
{    
	if(m_ptrBitmap[2].IsNull() || m_ptrBitmap[3].IsNull() || m_ptrBitmap[4].IsNull() || m_ptrBitmap[2]->GetSize() != m_ptrBitmap[3]->GetSize() || m_ptrBitmap[2]->GetSize() != m_ptrBitmap[4]->GetSize()
		 || m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[3]->GetBitsPerPixel() || m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[4]->GetBitsPerPixel())	
	{ return;	}

	// On regrade si on a affaire à 8bit ou 12bit (16 en fait)
	unsigned short NbrBits = m_ptrBitmap[3]->GetBitsPerPixel();
	// Et on fixe la valeur de MaxWhiteValue qui correspond
	int MaxWhiteValue = 0;
	double Isat0_OnTheCCD = 
		m_Isat0Serie /*mW/cm²*/
		* 10 /*W/m²*/
		/ exp(m_AttenBeforeCamSerie);
	double IsatEff_OnTheCCD = Isat0_OnTheCCD * m_IsatSurIsat0Serie;
	double PixelValue255_for_IsatEff_At780nm = 
		/*   photons   */
			IsatEff_OnTheCCD /*W/m²*/
			/ m_PhotonEnergie_J /*J*/
			* (m_PulseLengthSerie) /*s*/
			* (m_PixelSizeSerie * 1e-6) * (m_PixelSizeSerie * 1e-6) /*m²*/
	/	m_PhotonParBitSur255 /*photons par bit/255*/;
	double PixelValue4095_for_IsatEff_At780nm = 
		PixelValue255_for_IsatEff_At780nm
		* 4095.0 / 255.0;
	switch(NbrBits)
	{
	case 8:
		MaxWhiteValue = 255;
		break;
	case 16:
		MaxWhiteValue = 4095;
		break;
	}
	m_Size = m_ptrBitmap[3]->GetSize();
	long SizeX = m_Size.cx;
	long SizeY = m_Size.cy;
	unsigned long nTotalPixel = SizeX * SizeY;

	if(m_ptrBitmap[1].IsNull() || m_ptrBitmap[1]->GetSize() != m_ptrBitmap[3]->GetSize() || m_ptrBitmap[1]->GetBitsPerPixel() != m_ptrBitmap[3]->GetBitsPerPixel())
	{
		m_ptrBitmap[1].Create(m_ptrBitmap[2]->GetSize(),m_ptrBitmap[2]->GetBitsPerPixel(),m_ptrBitmap[2]->GetOrientation(),m_ptrBitmap[2]->GetPaletteType());
	}
	unsigned char* PtrSous_8bit = (unsigned char*) m_ptrBitmap[1]->GetPixels();
	unsigned char* Ptr1_8bit = (unsigned char*) m_ptrBitmap[3]->GetPixels();
	unsigned char* Ptr2_8bit = (unsigned char*) m_ptrBitmap[2]->GetPixels();
	unsigned char* Ptr3_8bit = (unsigned char*) m_ptrBitmap[4]->GetPixels();
	unsigned short* PtrSous_16bit = (unsigned short*) m_ptrBitmap[1]->GetPixels();
	unsigned short* Ptr1_16bit = (unsigned short*) m_ptrBitmap[3]->GetPixels();
	unsigned short* Ptr2_16bit = (unsigned short*) m_ptrBitmap[2]->GetPixels();
	unsigned short* Ptr3_16bit = (unsigned short*) m_ptrBitmap[4]->GetPixels();

    if(m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[3]->GetBitsPerPixel() || m_ptrBitmap[2]->GetBitsPerPixel() != m_ptrBitmap[4]->GetBitsPerPixel())
	{	AfxMessageBox("High Intensity Absorption : The pictures have not the same BitsPerPixel value !"); return;	}

	PrepareCleanResultatCalculImage(nTotalPixel);
	// par quoi va t'on multiplier la valeur trouvée pour la normaliser à m_RangeAbsSerie dans l'affichage
	double multipl_For_RangeAbs = double(MaxWhiteValue) / m_RangeAbsSerie;
	double valeur, AvecAt, SansAt;
	switch(NbrBits)
	{
	case 8:	
		for(unsigned long x = 0; x < nTotalPixel; ++x)
		{
			SansAt = max(0.0, double(Ptr1_8bit[x]) - double(Ptr3_8bit[x]));
			AvecAt = max(0.0, double(Ptr2_8bit[x]) - double(Ptr3_8bit[x]));
			if(AvecAt < 1.0 || SansAt < 1.0)
				valeur = POINT_DISCARDED;
			else
			{
				valeur = -log(AvecAt / SansAt) + (SansAt - AvecAt) / PixelValue255_for_IsatEff_At780nm;
				valeur *= m_IsatSurIsat0Serie; // pour tout multiplier par m_IsatSurIsat0Serie cf article imagerie
			}
			PtrSous_8bit[x] = unsigned char(max(0, min(255.0, valeur * multipl_For_RangeAbs)));
			m_pResultatCalculImageY[x] = valeur;
		}
		break;
	case 16:
		for(unsigned long x = 0; x < nTotalPixel; ++x)
		{
			SansAt = max(0.0, double(Ptr1_16bit[x]) - double(Ptr3_16bit[x]));
			AvecAt = max(0.0, double(Ptr2_16bit[x]) - double(Ptr3_16bit[x]));
			if(SansAt < 1.0 || AvecAt < 1.0)
				valeur = POINT_DISCARDED;
			else
			{
				valeur = -log(AvecAt / SansAt) + (SansAt - AvecAt) / PixelValue4095_for_IsatEff_At780nm;
				valeur *= m_IsatSurIsat0Serie; // pour tout multiplier par m_IsatSurIsat0Serie cf article imagerie
			}
			PtrSous_16bit[x] = unsigned short(max(0, min(4095.0, valeur * multipl_For_RangeAbs)));
			m_pResultatCalculImageY[x] = valeur;
		}
		break;
	}
	return;
}
int CSerieImage::Next(bool ReleaseTheBitmaps /* = false */)
{
	int NewCurrentIndex = m_ImageCourante + 1;
	SetCurrentImageIndex(NewCurrentIndex);
	return NewCurrentIndex;
}

int CSerieImage::SetCurrentImageIndex(int NewCurrentIndex, bool ReleaseTheBitmaps /* = false */)
{
	int oldIndex = m_ImageCourante;
	// si Reset_Fit_Allready_Computed, on ne change pas la valeur de l'index courant
	if(NewCurrentIndex == Reset_Fit_Allready_Computed)
	{
		m_FitAllreadyComputed = false;
		return oldIndex;
	}
	else
		m_ImageCourante = NewCurrentIndex;
	
	// nettoyage des images Bitmaps:
	if(ReleaseTheBitmaps)
		ReleaseBitmaps();

	return oldIndex;
}

bool CSerieImage::SetModeAbsorp(bool mode)
{
	PrepareCleanResultatCalculImage();
	bool old = m_ModeAbsorp;
	m_ModeAbsorp = mode;
	return old;
}

CString CSerieImage::TrouveFinFolder(int NbrSousDossier, CString adresse /* = "" */)
{
		char FinFolder[512];
		if(adresse == "")
			sprintf(FinFolder, "%s", m_FolderPath);
		else
			sprintf(FinFolder, "%s", adresse);
		char* FinFoldPos = &(FinFolder[0]);
		for(int i=1; i<=1+NbrSousDossier; i++)	// transforme les derniers '\' en tabulations
			if(	strrchr( &(FinFolder[0]) ,'\\') != NULL	)
			{
				FinFoldPos = strrchr( &(FinFolder[0]) ,'\\');
				*FinFoldPos = '\t';
			}
		while(strrchr(FinFoldPos,'\t') != NULL)	//  transforme les tabulations en '\'
			*(strrchr(FinFoldPos,'\t')) = '\\';	// dans l'operation, on a gagné la position FinFoldPos

		strcpy(FinFolder,FinFoldPos);			// actualise FinFolder
return FinFolder;
}

CString CSerieImage::TrouveBase(CString Nom)
{
	if(Nom.Find(".bmp") == Nom.GetLength() - 3 - 1
		&& Nom.Find(".bmp") != -1)
		Nom.Truncate(Nom.GetLength() - 3 - 1);
	char base[512];
	int posSlash = 0;
	if(Nom.ReverseFind('\\') != -1)
		posSlash = Nom.ReverseFind('\\') + 1;
	sprintf(base, "%s", Nom.GetBuffer() + posSlash);
	int taille = (int)strcspn(base,""); // donne la taille du Nom
	int ind1 = taille+1;
	int ind2 = 0;

	if( 	!strncmp(base+ind1-6,"_With",5)
		||	!strncmp(base+ind1-6,"_NoAt",5)
		||	!strncmp(base+ind1-6,"_Back",5)
		||	!strncmp(base+ind1-6,"Absor",5)		)
	{
		ind1 -= 5;
		*(base+ind1-1) = '\0';
		SetModeAbsorp(true);	
	}
	else
		SetModeAbsorp(false);	
	;

	do
	{	ind1--;
		ind2 = (int)strcspn(base+ind1,"0123456789");	} // Returns the index of the first occurrence of a number
	// cette boucle donne la position du charactere précédent le nombre final
	// ex : sfsdw1454 renvoye la position du w
	while(ind2 == 0);
	ind1++;
	*(base + ind1) = '\0';
return base;
}

int CSerieImage::TrouveIndex(char* Fichier)
{
	char* stopstring;
	char nombre[512];
	strcpy(nombre,Fichier);
	int taille = (int)strcspn(nombre,"");
	int ind1 = taille+1;
	int ind2 = 0;
	
	if( 	!strncmp(nombre+ind1-6,"_With",5)
		||	!strncmp(nombre+ind1-6,"_NoAt",5)
		||	!strncmp(nombre+ind1-6,"_Back",5)
		||	!strncmp(nombre+ind1-6,"Absor",5)		)
	{
		ind1 -= 5;
		*(nombre+ind1-1) = '\0';
	}

	do
	{	ind1--;
		ind2 = (int)strcspn(nombre+ind1,"0123456789");	}
	while(ind2 == 0);
	ind1++;
	if(ind1 == taille)	return -1;	// si il n'y a pas de chiffre à la fin
	int result = strtol(nombre+ind1,&stopstring,10);
//	CString result((nombre+ind1));
return result;
}

int CSerieImage::TrouveFirstIndex(CString base, CString dossier)
{
	CFileFind f;
	CString TheBase = base;
	char recherche[4096];
	if(base != "" && dossier != "")
		sprintf(recherche,"%s%s*.bmp",dossier,base);
	else
	{
		sprintf(recherche,"%s%s*.bmp", m_FolderPath, TrouveBase(m_PathName));
		TheBase = TrouveBase(m_PathName);
	}
	BOOL bMoreFiles = f.FindFile(recherche);
	char filename[4096];
	if(bMoreFiles)
	{
		bMoreFiles = f.FindNextFile();
		sprintf(filename,f.GetFileName());
		*(strrchr(filename,'.')) = '\0';			// élimine l'extension .bmp
		if(TrouveBase(filename) == TheBase)
		{
			m_FirstIndex = TrouveIndex(filename);
		}
		else MessageBeep(1);//MessageBox("No Good Base Name!");
	}
	else MessageBeep(1);//MessageBox("No Good Base Name!");

return m_FirstIndex;
}

int CSerieImage::TrouveLastIndex(CString base, CString dossier)
{
	m_LastIndex = 0;
	CFileFind f;
	char recherche[512];
	sprintf(recherche,"%s%s*.bmp",dossier,base);
	BOOL bMoreFiles = f.FindFile(recherche);
	char filename[512];
	if(bMoreFiles)
		while (bMoreFiles)
		{
			bMoreFiles = f.FindNextFile();
			sprintf(filename,f.GetFileName());
			*(strrchr(filename,'.')) = '\0';			// élimine l'extension .bmp
			if(TrouveBase(filename) == base)
			{
				m_LastIndex = TrouveIndex(filename);
			}
		}	
//	else MessageBox("No Good Base Name!");
return m_LastIndex;
}


void CSerieImage::ReleaseBitmaps()
{
	m_ptrBitmap[1].Release();
	m_ptrBitmap[2].Release();
	m_ptrBitmap[3].Release();
	m_ptrBitmap[4].Release();
}
void CopyBitmapTo(CDibImagePtr& PtrDibToCopy, CDibImagePtr& PtrDibDestination, int BitPerPixelDestination_8_16 /* = 0 */)
{	
	if(!bool(PtrDibToCopy))
		return;
	// créons une nouvelle image
	CSize Size = PtrDibToCopy->GetSize();
	unsigned int BitPerPixel = PtrDibToCopy->GetBitsPerPixel();
	// les BitPerPixelDestination = 8-16, Et, si on met 0, s'ajuste a l'image de départ
	int BitPerPixelDestination = BitPerPixelDestination_8_16 ? BitPerPixelDestination_8_16 : BitPerPixel;
	PtrDibDestination.Release();
	PtrDibDestination.Create(Size, BitPerPixelDestination, PtrDibToCopy->GetOrientation(), CDib::Monochrome);

	// copy bit a bit des donnée de l'image 8bit, ou bien 16 bit
	int NbrPixel = Size.cx * Size.cy;
	unsigned char*  ToBeCopiedPtrPixel_8bit		= (unsigned char*) PtrDibToCopy->GetPixels();
	unsigned char*  NewPtrPixel_8bit			= (unsigned char*) PtrDibDestination->GetPixels();
	unsigned short* ToBeCopiedPtrPixel_16bit	= (unsigned short*)PtrDibToCopy->GetPixels();
	unsigned short* NewPtrPixel_16bit			= (unsigned short*)PtrDibDestination->GetPixels();
	// si c'est de image 8bit à 8bit
	if(BitPerPixel == 8 && BitPerPixelDestination == 8)
	{	for(long i=0 ; i < NbrPixel ; i++)
		{
			NewPtrPixel_8bit[i] = ToBeCopiedPtrPixel_8bit[i];
		}
		return;
	}
	// si c'est de image 8bit à 16bit
	if(BitPerPixel == 8 && BitPerPixelDestination == 16)
	{	for(long i=0 ; i < NbrPixel ; i++)
		{
			NewPtrPixel_16bit[i] = ToBeCopiedPtrPixel_8bit[i] * 16;
		}
		return;
	}
	// si c'est de image 16bit à 8bit
	if(BitPerPixel == 16 && BitPerPixelDestination == 8)
	{	for(long i=0 ; i < NbrPixel ; i++)
		{
			NewPtrPixel_8bit[i] = ToBeCopiedPtrPixel_16bit[i] / 16;
		}
		return;
	}
	// si c'est de image 16bit à 16bit
	if(BitPerPixel == 16 && BitPerPixelDestination == 16)
	{	for(long i=0 ; i < NbrPixel ; i++)
		{
			NewPtrPixel_16bit[i] = ToBeCopiedPtrPixel_16bit[i];
		}
		return;
	}
	AfxMessageBox("NbrPixel should be 8 or 16 !");
}
void CSerieImage::CopyBitmapToCadranSerieIndexCourant(int WhichCadran, CDibImagePtr& PtrDib)
{	
	CopyBitmapTo(PtrDib, m_ptrBitmap[WhichCadran]);
}

int CSerieImage::SaveFluoBitmap(int Unnumero /* = COURANTE */, CString Undossier /* = "" */ , CString Unbasename /* = "" */)
{
	int		numero		= Unnumero	;
	CString & dossier	= (Undossier  == "")	? m_FolderPath	: Undossier	;
	CString & basename	= (Unbasename == "")	? m_BaseName	: Unbasename;
	switch(numero)
	{	case COURANTE:	numero = m_ImageCourante;	break;
		case FIRST:		numero = m_FirstIndex;		break;
		case LAST:		numero = m_LastIndex;		break;	}
	if(numero == -1)
		return 0;
	char deszeros[16];
	sprintf(deszeros,"000000000000000");
	// place le nombre de Zero nécéssaires à avoir un chiffre avec NBR_CHIFFRE_SAVE chiffres
	int NbrChiffreNumero = int(log10(double(numero))) + 1;
	int NbrZeroManquant = NBR_CHIFFRE_SAVE - NbrChiffreNumero;
	*(deszeros + NbrZeroManquant) = '\0';

	sprintf(m_PathName, "%s%s%s%d.bmp",dossier,basename,deszeros,numero);

	if(bool(m_ptrBitmap[1]))
		SaveBMP(m_ptrBitmap[1], m_PathName);
return 0;
}
int CSerieImage::SaveAbsBitmap(int WhichCadran /* = SaveAllCadrans */, int Unnumero /* = COURANTE */, CString Undossier /* = "" */ , CString Unbasename /* = "" */)
{
	int		numero		= Unnumero	;
	CString & dossier	= (Undossier  == "")	? m_FolderPath	: Undossier	;
	CString & basename	= (Unbasename == "")	? m_BaseName	: Unbasename;
	switch(numero)
	{	case COURANTE:	numero = m_ImageCourante;	break;
		case FIRST:		numero = m_FirstIndex;		break;
		case LAST:		numero = m_LastIndex;		break;	}
	if(numero == -1)
		return 0;
	char deszeros[16];
	char absorption[8];
	sprintf(deszeros,"000000000000000");
	// place le nombre de Zero nécéssaires à avoir un chiffre avec NBR_CHIFFRE_SAVE chiffres
	int NbrChiffreNumero = int(log10(double(numero))) + 1;
	int NbrZeroManquant = NBR_CHIFFRE_SAVE - NbrChiffreNumero;
	*(deszeros + NbrZeroManquant) = '\0';

	// pour chaque Cadran dans l'ordre haut-droite ; bas-gauche ; basdroite ; PUIS haut-gauche: image principale en dernier
	for(int absindex = 1; absindex <= 4; absindex++ )
	{
		int nCadran = absindex%4 + 1; // 2, 3, 4, puis 1
		// Fabrication du nom m_PathName
		sprintf(absorption,"");
		switch(nCadran)
		{
		case 2:		
			sprintf(absorption,"_With");	
			break;
		case 3:		
			sprintf(absorption,"_NoAt");	
			break;
		case 4:		
			sprintf(absorption,"_Back");	
			break;	
		case 1:		
			sprintf(absorption,"Absor");	
			break;
		}
		sprintf(m_PathName, "%s%s%s%d%s.bmp",dossier,basename,deszeros,numero,absorption);

		// sauvetage
		int CadranASauver = 0;
		unsigned int BitPerPixel = 8;
		try{if(!m_ptrBitmap[1].IsNull()) BitPerPixel = m_ptrBitmap[1]->GetBitsPerPixel();}
		catch(int sqfd){sqfd = 1;}
		switch(nCadran)
		{
		case 2:		
			CadranASauver = SaveCadran_2;
			break;
		case 3:		
			CadranASauver = SaveCadran_3;	
			break;
		case 4:		
			CadranASauver = SaveCadran_4;
			break;	
		case 1:		
			CadranASauver = SaveCadran_1;
			// on converti en 8bit pour sauver sur le disk !
			if(BitPerPixel != 8)
				CopyBitmapTo(m_ptrBitmap[1], m_ptrBitmap8BitsImageCalculePourSauver, 8);
			break;
		}
		if( bool(m_ptrBitmap[nCadran]) && (WhichCadran & CadranASauver))
			if(BitPerPixel != 8 && nCadran == 1)
				SaveBMP(m_ptrBitmap8BitsImageCalculePourSauver, m_PathName);
			else
				SaveBMP(m_ptrBitmap[nCadran], m_PathName);
	}
return 0;
}
void CSerieImage::SaveTexteDescritpionSerie(CString t1, double v1, CString t2, double v2, CString t3, double v3, CString t4, double v4, CString t5, double v5, CString t6, double v6, CString t7, double v7, CString t8, double v8, CString t9, double v9, CString t10, double v10)
{
	CString LeTexte("");
	CString alaligne(" \r\n");
	CString UnTexte;
	CString UneValeur;
	// les donnée de base
	LeTexte += LaDate() + alaligne;
	LeTexte += m_FolderPath + alaligne;
	LeTexte += m_BaseName + alaligne;

	UnTexte = "m_PixelSizeSerie"; UneValeur.Format("%4g", m_PixelSizeSerie);	
		LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = "m_RangeAbsSerie"; UneValeur.Format("%4g", m_RangeAbsSerie);	
		LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = "m_CalculationType"; UneValeur = 
			m_CalculationType == ClassicalAbsorption ? "ClassicalAbsorption" :
			m_CalculationType == HighIntensityAbsorption ? "HighIntensityAbsorption" :
	m_CalculationType == Soustraction_3m2To1 ? "Soustraction_3m2To1" : "unknown"	;	
		LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	if(m_CalculationType == HighIntensityAbsorption)
	{
		UnTexte = "m_IsatSurIsat0Serie"; UneValeur.Format("%4g", m_IsatSurIsat0Serie);	
			LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
		UnTexte = "m_Isat0Serie"; UneValeur.Format("%4g", m_Isat0Serie);	
			LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
		UnTexte = "m_PulseLengthSerie"; UneValeur.Format("%4g", m_PulseLengthSerie);	
			LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
		UnTexte = "m_AttenBeforeCamSerie"; UneValeur.Format("%4g = OD", m_AttenBeforeCamSerie);	
			LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	}

	LeTexte += alaligne;
	// Les 10 "texte = valeur"
	UnTexte = t1;	UneValeur.Format("%4g", v1);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t2;	UneValeur.Format("%4g", v2);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t3;	UneValeur.Format("%4g", v3);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t4;	UneValeur.Format("%4g", v4);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t5;	UneValeur.Format("%4g", v5);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t6;	UneValeur.Format("%4g", v6);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t7;	UneValeur.Format("%4g", v7);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t8;	UneValeur.Format("%4g", v8);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t9;	UneValeur.Format("%4g", v9);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;
	UnTexte = t10;	UneValeur.Format("%4g", v10);	if(UnTexte != "")	LeTexte += alaligne + UnTexte + CString(" = ") + UneValeur;

	CTextFile tf("txt","\r\n");
	tf.WriteTextFile(m_FolderPath + m_BaseName + "Parametre.txt", LeTexte);
return;
}

CString CSerieImage::InfoZoneImageSansAtomes(CRect Zone /* = CRect(-1,-1,0,0) */)
{
	CString Info(""), TotaleInfo("");
	CPointDbl ptSumSat = GetSumAndSatInZone(m_ptrBitmap[3], Zone);
	if(ptSumSat == CPointDbl(HORS_IMAGE, HORS_IMAGE) || ptSumSat == CPointDbl(PAS_IMAGE, PAS_IMAGE))
		return TotaleInfo;
	unsigned short NbrBits = m_ptrBitmap[3]->GetBitsPerPixel();
	int MaxWhiteValue;
	switch(NbrBits)
	{	
	case 8:		MaxWhiteValue = 255;	break;
	case 16:	MaxWhiteValue = 4095;	break;
	}

	// calculons l'énergie lumineuse correspondant à la somme
	double MeanVal = ptSumSat.x / double(Zone.Height() * Zone.Width());
	double TotaleEnergie_J = ptSumSat.x * m_PhotonParBitSur255 * m_PhotonEnergie_J * exp(m_AttenBeforeCamSerie);
	double Puissance_mW = 1000.0 * TotaleEnergie_J / m_PulseLengthSerie;
	double EnergieSurfacique_JpCM2 = TotaleEnergie_J 
									/ double(Zone.Height() * Zone.Width()) 
									/ (m_PixelSizeSerie * m_PixelSizeSerie * 1e-8);
	double Intensity_mWpCM2 = 1000.0 * EnergieSurfacique_JpCM2 / m_PulseLengthSerie;
	Info.Format("Mean Pixel Val = %.3g / %d", MeanVal, MaxWhiteValue);
	TotaleInfo += Info + CString("\r\n");
	Info.Format("Energy = %.3g J", TotaleEnergie_J);
	TotaleInfo += Info + CString("\r\n");
	Info.Format("Surfacic = %.3g J/cm²", EnergieSurfacique_JpCM2);
	TotaleInfo += Info + CString("\r\n");
	Info.Format("Power = %.3g mW", Puissance_mW);
	TotaleInfo += Info + CString("\r\n");
	Info.Format("Intensity = %.3g mW/cm²", Intensity_mWpCM2);
	TotaleInfo += Info + CString("\r\n");
return TotaleInfo;
}

void SaveBMP(CDibImagePtr& PtrDibToSave, CString FileName )
{
	PtrDibToSave->StoreBMP(FileName);
}
void SaveTIFF(CDibImagePtr& PtrDibToSave, CString FileName )
{
	
}

CPoint PixelImageToEcran(CPoint PointInImage)
{
	CPoint PointOnScreen;
	PointOnScreen.x = long(double(PointInImage.x) * CSerieImage::m_ZoomEcranImage);
	PointOnScreen.y = long(double(PointInImage.y) * CSerieImage::m_ZoomEcranImage);
return PointOnScreen;
};
CPoint EcranToPixelImage(CPoint PointOnScreen)
{
	CPoint PointInImage;
	PointInImage.x = long(double(PointOnScreen.x) / CSerieImage::m_ZoomEcranImage);
	PointInImage.y = long(double(PointOnScreen.y) / CSerieImage::m_ZoomEcranImage);
return PointInImage;
};

CPointDbl GetSumAndSatInZone(CDibImagePtr& ptrDib, CRect Zone /* = CRect(-1,-1,0,0) */)
{
	if (!(bool)ptrDib)	
		return CPointDbl(PAS_IMAGE,PAS_IMAGE);
	// On regrade si on a affaire à 8bit ou 12bit (16 en fait)
	unsigned short NbrBits = ptrDib->GetBitsPerPixel();
	// Et on fixe la valeur de m_MaxWhiteValue qui correspond
	int MaxWhiteValue = 1;
	switch(NbrBits)
	{	case 8:
		MaxWhiteValue = 255;		break;
		case 16:
		MaxWhiteValue = 4095;		break;
		return CPointDbl(PAS_IMAGE,PAS_IMAGE);
	}
	__int64 PixelSommeZone = 0;
	__int64 PixelSaturesZone = 0;
	long SizeX = ptrDib->GetSize().cx;
	long SizeY = ptrDib->GetSize().cy;
	if(Zone == CRect(-1,-1,0,0))
		Zone = CRect(CPoint(0,0), CSize(SizeX - 1, SizeY - 1));
	Zone.NormalizeRect();
	CPoint TopLeft = Zone.TopLeft();
	long NumPointX = Zone.Width();
	long NumPointY = Zone.Height();
	unsigned char*  PtrPixel_8bit  = (unsigned char*) ptrDib->GetPixels();
	unsigned short* PtrPixel_16bit = (unsigned short*)ptrDib->GetPixels();
	PtrPixel_8bit  += TopLeft.x + TopLeft.y * SizeX;
	PtrPixel_16bit += TopLeft.x + TopLeft.y * SizeX;
	if(TopLeft.x < 0 || TopLeft.y < 0 || TopLeft.x + NumPointX >= SizeX || TopLeft.y + NumPointY >= SizeY)
		return CPointDbl(HORS_IMAGE, HORS_IMAGE);

	switch(NbrBits)
	{	case 8:	
		for(long j=0 ; j < NumPointY ; j++)
		{	for(long i=0 ; i < NumPointX ; i++)
			{	PixelSommeZone += PtrPixel_8bit[i + j*SizeX];
				if(PtrPixel_8bit[i + j*SizeX] >= MaxWhiteValue)
					PixelSaturesZone++;			}		}		break;
		case 16:
		for(long j=0 ; j < NumPointY ; j++)
		{	for(long i=0 ; i < NumPointX ; i++)
			{	PixelSommeZone += PtrPixel_16bit[i + j*SizeX];
				if(PtrPixel_16bit[i + j*SizeX] >= MaxWhiteValue)
					PixelSaturesZone++;			}		}		break;
	}
	return CPointDbl(double(PixelSommeZone), double(PixelSaturesZone));
}
CPointDbl GetSumAndSatInDoubleZone(CSize TailleTotale, double* pTotaleDataY, CRect Zone, double ValSat)
{
	if (!pTotaleDataY)	
		return CPointDbl(0.0,0.0);
	double PixelSommeZone = 0;
	__int64 PixelSaturesZone = 0;
	if(Zone == CRect(-1,-1,0,0))
		Zone = CRect(CPoint(0,0), CSize(TailleTotale.cx - 1, TailleTotale.cy - 1));
	Zone.NormalizeRect();
	CPoint TopLeft = Zone.TopLeft();
	long NumPointX = Zone.Width();
	long NumPointY = Zone.Height();
	double* PtrPixelDbl;
	PtrPixelDbl = pTotaleDataY + TopLeft.x + TopLeft.y * TailleTotale.cx;
	if(TopLeft.x < 0 || TopLeft.y < 0 || TopLeft.x + NumPointX >= TailleTotale.cx || TopLeft.y + NumPointY >= TailleTotale.cy)
		return CPointDbl(HORS_IMAGE, HORS_IMAGE);
	for(long j=0 ; j < NumPointY ; j++)
	{	for(long i=0 ; i < NumPointX ; i++)
		{	PixelSommeZone += PtrPixelDbl[i + j*TailleTotale.cx];
			if(PtrPixelDbl[i + j*TailleTotale.cx] >= ValSat)
				PixelSaturesZone++;
		}
	}
	return CPointDbl(PixelSommeZone, double(PixelSaturesZone));
}
