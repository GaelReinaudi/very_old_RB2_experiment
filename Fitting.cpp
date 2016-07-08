// voir numerical recipes chapitre 15.5

#include "stdafx.h"
#include "Fitting.h"
#include "SettingSave.h"
#include <float.h>
#include "nrutil.h"
#include "nr.h"
#include "io.h"

	CPen PenRouge	(PS_SOLID,	1, RGB(255 , 0   , 0	));
	CPen PenMauve	(PS_SOLID,	1, RGB(255 , 100 , 255	));
	CPen PenMauve2	(PS_SOLID,	2, RGB(255 , 100 , 255	));
	CPen PenJaune	(PS_SOLID,	1, RGB(255 , 255 , 0	));
	CPen PenGreen	(PS_SOLID,	1, RGB(0   , 255 , 0	));
	CPen PenGreen2	(PS_SOLID,	2, RGB(0   , 255 , 0	));

enum{Stable, UnFinished, NewFitting, InCabbages};

double CFitting::m_PixelSizeFit_µm = 6.45;
int CFitting::m_NbrIter = 30;
bool CFitting::m_CheckCabbages = true;
bool CFitting::m_DiscaredSaturatedPoints = true;
bool CFitting::m_FitFromPrevious = true;
int CFitting::m_StopWhenStable = 1;
CString* CFitting::m_pParamEnPlus_Nom[] = {NULL};
double* CFitting::m_pParamEnPlus_Val[] = {NULL};
double* CFitting::m_pParamEnPlus_Moy[] = {NULL};
double* CFitting::m_pParamEnPlus_StD[] = {NULL};
int CFitting::m_pParamEnPlus_Combien = 0;

using namespace std;
CFitting::CFitting(void)
{
}

/* 
constructeur 
*/
CFitting::CFitting(	  CString NomDuFit
					, bool Is1D
					, void (*TheFonc)(float, float [], float *, float [], int)
					, int npar
					, CString* pDesNoms // contient StrNom, StrMultiplicateur, StrUnités
					, float* pTheParamGuess
					, float* pTheCabbage
					, CNiNumEdit* pNiEditParam /* = NULL */, CNiNumEdit* pNiEditLambda /* = NULL */, CNiNumEdit* pNiEditChi2 /* = NULL */, CNiSlide* pNiSlide /* = NULL */)
{ 
	m_Nom = NomDuFit;
	m_nPtX = 200;
	m_nPtY = 200;
	m_pNiEditStart	= pNiEditParam;
	m_pNiSlide		= pNiSlide;
	m_pNiEditLambda	= pNiEditLambda;
	m_pNiEditChi2	= pNiEditChi2;
	m_IsFit1D		= Is1D;
	m_LaFonction	= TheFonc;
	m_nParam		= npar;
	for(int i = 1; i <= m_nParam; i++)
	{
		int CurPos = 0;
		m_pStringNomParams[i] = pDesNoms[i-1].Tokenize(",", CurPos);
		m_pStringEchelleFit[i] = pDesNoms[i-1].Tokenize(",", CurPos);
		m_pStringUnites[i] = pDesNoms[i-1].Tokenize(",", CurPos);
	}
	MakeMultiplicateurPourUnites();

	for(int i = 1; i <= m_nParam; i++)
	{
		m_pParamGuess[i] = pTheParamGuess[i-1];
	}
	for(int i = 1; i <= m_nParam; i++)
	{
		m_pCabbageDown[i] = pTheCabbage[2*(i-1)];
		m_pCabbageUp[i] = pTheCabbage[2*(i-1)+1];
	}
	m_nPoints = 0;
	m_pCovar = NULL;
	m_pAlpha = NULL;
	m_pSigma = NULL;
	m_pDataX = NULL;
	m_pDataY = NULL;
	m_pFitY	 = NULL;

	ResteFixe(""); // rien ne reste fixe à priori
}
CFitting::~CFitting(void)
{
}
void CFitting::MakeMultiplicateurPourUnites()
{
	for(int i = 1; i <= m_nParam; i++)
	{
		double multiplicateur = 1.0;
		if(m_pStringUnites[i] == "µm")
			multiplicateur = m_PixelSizeFit_µm;
		if(m_pStringUnites[i] == "mm")
			multiplicateur = m_PixelSizeFit_µm * 1e-3;
		if(m_pStringUnites[i] == "cm")
			multiplicateur = m_PixelSizeFit_µm * 1e-4;
		if(m_pStringUnites[i] == "m")
			multiplicateur = m_PixelSizeFit_µm * 1e-6;

		//if(m_pStringUnites[i] == "photons")
		//	multiplicateur = m_PixelSizeFit_µm * 1e-6;???
		//if(m_pStringUnites[i] == "OD")
		//	multiplicateur = m_PixelSizeFit_µm * 1e-6;???

		m_pMultiplicateurPourUnites[i] = multiplicateur;

//		m_pParamEnUnites[i] = m_pParamAjustables[i] * multiplicateur;
	}
}
void CFitting::PrepareForUse(CRect Zone)
{
	m_ZoneToFit = Zone;
	m_ZoneToFit.NormalizeRect();

	m_nPtX = m_ZoneToFit.Width();
	m_nPtY = m_ZoneToFit.Height();
	int nPointsAvant = m_nPoints;
	m_nPoints = (m_nPtX * m_nPtY);
	// ! ! très important ! !
	// astuce pour passer la valeur numpointX à la fonction a travers les parametres utilisé par mrqmin() et mrqcof()
	m_pParamAjustables[0]= float(m_nPtX);

	if( (m_nPoints != nPointsAvant) && m_pCovar && m_pAlpha && m_pSigma && m_pDataX && m_pDataY && m_pFitY)
		Nettoye();
}
void CFitting::CreateNrMatrix()
{
	m_pCovar	 = 	matrix(1, m_nParam,	1, m_nParam );
	m_pAlpha	 = 	matrix(1, m_nParam,	1, m_nParam );
	m_pSigma	 = 	matrix(1, 1,		1, m_nPoints);
	m_pDataX	 = 	matrix(1, 1,		1, m_nPoints);
	m_pDataY	 = 	matrix(1, 1,		1, m_nPoints);
	m_pFitY		 = 	matrix(1, 1,		1, m_nPoints);
}


int CFitting::ResteFixe(CString nom_a_fixer /* = CString("") */, double aLaValeur /* = VALEUR_COURANTE */)
{
	if(nom_a_fixer == "")
	{
		for(int i = 1; i <= m_nParam ; i++)	
			m_pDoitAjuster[i] = 1;
return 0;
	}
	
	int NbrFixes = 0;
    for(int i = 1; i <= m_nParam ; i++)
	{
		if(m_pStringNomParams[i].Find( nom_a_fixer ) != -1 )
		{
			if(m_pDoitAjuster[i])
				NbrFixes++;
			m_pDoitAjuster[i] = 0;
			if(aLaValeur != VALEUR_COURANTE)
				m_pParamAjustables[i] = aLaValeur;
		}
	}
return NbrFixes;
}

void CFitting::ResetToGuess()
{
	for(int i = 1; i <= m_nParam; i++)
		;//m_pParamAjustables[i] = 0;
	for(int i = 1; i <= m_nParam; i++)
	{
		float multiplicateur = 1;
		if(m_pStringEchelleFit[i] == "PTX")
			multiplicateur = float(m_nPtX);
		if(m_pStringEchelleFit[i] == "PTY")
			multiplicateur = float(m_nPtY);
		if(m_pStringEchelleFit[i] == "MaxWhite")
			multiplicateur = CSerieImage::m_ModeAbsorp ? float(CSerieImage::m_RangeAbsSerie) : 1000;

		if(m_pDoitAjuster[i])
			m_pParamAjustables[i] = m_pParamGuess[i] * multiplicateur;
	}
	MakeMultiplicateurPourUnites();
	ActualiseDialogControls(0, Stable);
}
int CFitting::LanceUnFit(CSize TailleTotaleImage, double* pDataImageY, CRect Zone, CDC* pUnDC /* = NULL */)
{	// si rien n'est à ajuster, byebye
	int ShowMustGoOn = 0;
	for(int i = 1; i <= m_nParam ; i++)	
		ShowMustGoOn += m_pDoitAjuster[i] ? 1 : 0;
	if(!ShowMustGoOn)
		return 0;
	// Si on a passé autre chose que NULL, NULL : on utilise directement &m_pDataX[1][1], &m_pDataY[1][1]
	if(pDataImageY == NULL)
	{
		// preparation normalement deja faite quand on utilise LanceUnFit(NULL, NULL, ...)
		// AfxMessageBox("preparation normalement deja faite quand on utilise LanceUnFit(NULL, NULL, ...)");
	}
	else // sinon, on met pDataX et pDataY dans &m_pDataX[1][1], &m_pDataY[1][1]
	{	
		PrepareForUse(Zone);
		CreateNrMatrix();
		m_SommePixelZone = GetFloatPixelsFromDoubleZone(TailleTotaleImage, pDataImageY, Zone, &m_pDataX[1][1], &m_pDataY[1][1]).x;
	}
	// si on sature, on ne tient pas compte du point, en mettant sont sigma très grand
	double saturation = CSerieImage::m_ModeAbsorp ? CSerieImage::m_RangeAbsSerie : m_MaxWhiteValue;
	for(int i=1; i <= m_nPoints ; i++)
	{
		m_pSigma [1][i] = 1.0;
		if(m_pDataY[1][i] == POINT_DISCARDED || (m_DiscaredSaturatedPoints && m_pDataY[1][i] >= saturation) )
			m_pSigma [1][i] = 1e10;
	}

	if(!m_FitFromPrevious)
		ResetToGuess();

	// Load the hourglass cursor
	HCURSOR lhCursor = AfxGetApp()->LoadStandardCursor(IDC_WAIT);	SetCursor(lhCursor);

	int iteree = 0;
	if(m_CheckCabbages && InTheCabbage())
	//	AfxMessageBox("Initial values in the cabbages", MB_ICONERROR);
		ResetToGuess();
	else
	{
		iteree = StartIter(pUnDC);
		Stop();
	}
		ActualiseDialogControls(m_NbrIter);
		Nettoye();

	// Load the arrow cursor
	lhCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);	SetCursor(lhCursor);
	
	return iteree;
}
int CFitting::LanceUnFit(CDibImagePtr& ptrDib, CRect Zone, CDC* pUnDC /* = NULL */)
{	if(!bool(ptrDib))
		return 0;
	// si rien n'est à ajuster, byebye
	int ShowMustGoOn = 0;
	for(int i = 1; i <= m_nParam ; i++)	
		ShowMustGoOn += m_pDoitAjuster[i] ? 1 : 0;
	if(!ShowMustGoOn)
		return 0;
	
	PrepareForUse(Zone);
	CreateNrMatrix();
	m_SommePixelZone = GetFloatPixelsFromZone(ptrDib, m_ZoneToFit, &m_pDataX[1][1], &m_pDataY[1][1]).x;
	int iteree = LanceUnFit(ptrDib->GetSize(), NULL, Zone, pUnDC);
return iteree;
}
int CFitting::StartIter(CDC* pUnDC /* = NULL */)
{
	int DejaEutSaSecondeChance = 0;

	// m_Lambda < 0 permet de lancer le processus de fitting
	m_Lambda = float(-1E-1);	
	m_ChiCarre = (float)1e30;
	m_YFitted = 0.0;
	float vieuChiCarre = (float)1e30;
	float vieuUnLamda = (float)1e30;
	ActualiseDialogControls(0, NewFitting);
	
	// on fait ... itérations
	int i = m_NbrIter;
	while(i--)
	{	
		// on peut afficher le fit à chaque pas.
		bool ChaquePas = true;
		if( ChaquePas && pUnDC)
		{   
			DessineFonction(pUnDC);
			ActualiseDialogControls(m_NbrIter - i);
		}

		vieuChiCarre = m_ChiCarre;	
		vieuUnLamda = m_Lambda;

		mrqmin(m_pDataX[1], m_pDataY[1], m_pSigma[1], m_nPoints, m_pParamAjustables, m_pDoitAjuster, m_nParam, m_pCovar, m_pAlpha, &m_ChiCarre, m_LaFonction, &m_Lambda);

		// si les parametres sont vraiment trop dans les Cabbages
		// et que on est près de finir le Fit, ie: "i" qui va de m_NbrIter à 0, proche de zero
		if(m_CheckCabbages && i < m_NbrIter/4 && InTheCabbage())
		{
			ActualiseDialogControls(m_NbrIter, InCabbages);
			// on peut donner une seconde chance au Fit en fixant les parametres voulus à 0, 
			// comme par exemple : tous les parametres concernant la deuxième gaussienne
			bool SecondeChance = true;
			if(SecondeChance && DejaEutSaSecondeChance == 0)
			{
				DejaEutSaSecondeChance = 1;
				// si le "fixage" donne effectivement quelque chose de différent de ce qu'on avait avant :
				if(m_FitFromPrevious || ( ResteFixe("V_Ampl_2", float(0.01)) + ResteFixe("V_Cent_2", float(m_nPtY)/2) + ResteFixe("V_Del_2", 0.01 ) ))
				{
					ResetToGuess();	
					DejaEutSaSecondeChance = 2;
					Beep(500,1);
					// m_Lambda < 0 permet de (RE-)lancer le processus de fitting //
					m_Lambda = float(-1E-1);										 //
					m_ChiCarre = (float)1e30;												 //
					m_YFitted = 0.0;												 //
					float vieuChiCarre = (float)1e30;										 //
					float vieuUnLamda = (float)1e30;										 //
					i = m_NbrIter; //////////////////////////////////////////////////
				}
			}
			// si on a déja utiliser effectivement la seconde chance , ou qu'on en avait pas :
			else if(DejaEutSaSecondeChance > 0)
			{
				DejaEutSaSecondeChance = 1;
				ResetToGuess();	
				Beep(1000,100);
			}
		}
		
		if(m_StopWhenStable || m_Lambda > 1e38)
			if(CritereStopFit())
			{
				ActualiseDialogControls(m_NbrIter, Stable);
				break;
			}
		//// Finir le Fit si il est bon
		//if(m_ChiCarre == vieuChiCarre && m_Lambda > vieuUnLamda && m_Lambda > 0 && vieuUnLamda > 0)
		//{
		//	optim_SameChi_LambdaBiger++;
		//	// si c'est la nieme fois qu'on passe par un fit stable sans possibilité d'amélioration
		//	// avec nieme = au moins 5, mais en general m_NbrIter / 5
		//	// mais pas trop grand quand meme car on es tlimité par: float < 1e38
		//	int NbrCoupStable = min(25, max(5, m_NbrIter / 5));
		//	if(optim_SameChi_LambdaBiger == NbrCoupStable)
		//	{
		//		m_Lambda = 1e-5;
		//		ActualiseDialogControls(m_NbrIter, Stable);	
		//		break;
		//	}
		//}
		//else
		//	optim_SameChi_LambdaBiger = 0;
	}
	if(m_Lambda == 0 )
	{
		ResetToGuess();
	}


	return m_NbrIter - i - 1;
}

int CFitting::CritereStopFit()
{
	if(m_Lambda > 1e10)
		return 1;

return 0;
}

void CFitting::ActualiseDialogControls(int niemeIter, int Status /* = -1 */)
{
	if(niemeIter == -1)
		return;
	// d'abord, on va faire les m_pParamEnUnites[i]
	for(int j = 1; j <= m_nParam; j++)
	{
		m_pParamEnUnites[j] = m_pParamAjustables[j] * m_pMultiplicateurPourUnites[j];
	}

	float* ParamAAfficher = m_pParamEnUnites;

	if(!m_pNiEditStart || !m_pNiSlide)
		return;
	
	(*m_pNiSlide).SetValue(100.0 * double(niemeIter)/double(m_NbrIter));
	for(int j = 1; j <= m_nParam; j++)
	{
		if(m_pNiEditStart[j].IsWindowEnabled())
		{
			m_pNiEditStart[j].SetValue(ParamAAfficher[j]);
			m_pNiEditStart[j].RedrawWindow();
		}
	}
	m_pNiEditLambda->SetValue(m_Lambda);
	m_pNiEditChi2->SetValue(m_ChiCarre);
	m_pNiEditLambda->RedrawWindow();
	m_pNiEditChi2->RedrawWindow();
	switch(Status)
	{
	case Stable:
		(*m_pNiSlide).GetPointers().Item(1).SetFillColor(CNiColor(0,255,0));
		break;
	case UnFinished:
		(*m_pNiSlide).GetPointers().Item(1).SetFillColor(CNiColor(255,0,0));
		break;
	case NewFitting:
		(*m_pNiSlide).GetPointers().Item(1).SetFillColor(CNiColor(0,0,255));
		break;
	case InCabbages:
		(*m_pNiSlide).GetPointers().Item(1).SetFillColor(CNiColor(255,255,0));
		break;
	}
}
void CFitting::Stop()
{
	// m_Lambda = 0 permet de fermer le processus de fitting
	m_Lambda = 0.0;
	if(m_NbrIter)
		mrqmin(m_pDataX[1], m_pDataY[1], m_pSigma[1], m_nPoints, m_pParamAjustables, m_pDoitAjuster, m_nParam, m_pCovar, m_pAlpha, &m_ChiCarre, m_LaFonction, &m_Lambda);
}

void CFitting::ExportParamEnUnites(double* pFirstParam, int NbrMaxExporter /* = MAX_PARAM_FIT */)
{
	ActualiseDialogControls(0);
	if(pFirstParam)
		for(int i = 1; i <= m_nParam; i++)
			if(i <= NbrMaxExporter)
				pFirstParam[i-1] = m_pParamEnUnites[i];
			else
				return;
}
int CFitting::GetNbrParam()
{
	return m_nParam;
}
int CFitting::GetMaxWhiteValue()
{
	return m_MaxWhiteValue;
}
CString CFitting::GetParamNom(int Numero)
{
	return m_pStringNomParams[Numero];
}
CString CFitting::GetParamUnit(int Numero)
{
	return m_pStringUnites[Numero];
}
void CFitting::SetNbrMaxIter(int Niter)
{
	m_NbrIter = Niter;
}
void CFitting::SetPixelSize(double PSize, bool ShouldActualiseDialogControls /* = true */)
{
	m_PixelSizeFit_µm = PSize;
	MakeMultiplicateurPourUnites();
	if(ShouldActualiseDialogControls)
		ActualiseDialogControls(0);
}
void CFitting::ResetParamEnPlus()
{
	for( int i = 0; i < PARAM_EN_PLUS_POSSIBLE; i++)
	{
		m_pParamEnPlus_Nom[i] = NULL;
		m_pParamEnPlus_Val[i] = NULL;
		m_pParamEnPlus_StD[i] = NULL;
	}
	m_pParamEnPlus_Combien = 0;
}
void CFitting::SetParamEnPlus(CString* pStr0, double* pVal0, double* pMoy0, double* pStD0, int Nombre)
{
	ResetParamEnPlus();
	m_pParamEnPlus_Combien = Nombre;
	for( int i = 0; i < m_pParamEnPlus_Combien; i++)
	{
		m_pParamEnPlus_Nom[i] = pStr0 + i;
		m_pParamEnPlus_Val[i] = pVal0 + i;
		m_pParamEnPlus_Moy[i] = pMoy0 + i;
		m_pParamEnPlus_StD[i] = pStD0 + i;
	}
}
void CFitting::AddParamEnPlus(CString* pStr0, double* pVal0, double* pMoy0, double* pStD0)
{
	for( int i = 0; i < PARAM_EN_PLUS_POSSIBLE; i++)
	{
		if(m_pParamEnPlus_Nom[i] == NULL)
		{
			m_pParamEnPlus_Nom[i] = pStr0;
			m_pParamEnPlus_Val[i] = pVal0;
			m_pParamEnPlus_Moy[i] = pMoy0;
			m_pParamEnPlus_StD[i] = pStD0;
			m_pParamEnPlus_Combien++;
			return;
		}
	}
}

void CFitting::SetParamAjustableEnUnites(int Numero, double ValeurParam)
{
	m_pParamEnUnites[Numero] = ValeurParam;
	m_pParamAjustables[Numero] = ValeurParam / m_pMultiplicateurPourUnites[Numero];
}
void CFitting::SetBoolAdjust(int Numero, int MustBeAdjusted)
{
	m_pDoitAjuster[Numero] = MustBeAdjusted;
}
void CFitting::CabbagesChecking(bool yesorno)
{
	m_CheckCabbages = yesorno;
}
void CFitting::DiscaredSaturatedPoints(bool yesorno)
{
	m_DiscaredSaturatedPoints = yesorno;
}
void CFitting::FittingFromPreviousPos(bool yesorno)
{
	m_FitFromPrevious = yesorno;
}
void CFitting::StopWhenStable(bool yesorno)
{
	m_StopWhenStable = yesorno;
}
CPointDbl CFitting::GetFloatPixelsFromDoubleZone(CSize TailleTotale, double* pTotaleDataY, CRect Zone, float* dataX /* = 0 */, float* dataY /* = 0 */)
{
	if (!pTotaleDataY)	
		return CPointDbl(0.0,0.0);
	double PixelSommeZone = 0;
	__int64 PixelSaturesZone = 0;
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
		{	if(dataX)
				dataX[i + j*NumPointX] = float(i + j*NumPointX);
			if(dataY)
				dataY[i + j*NumPointX] = float(PtrPixelDbl[i + j*TailleTotale.cx]);
			PixelSommeZone += PtrPixelDbl[i + j*TailleTotale.cx];
			if(PtrPixelDbl[i + j*TailleTotale.cx] >= m_MaxWhiteValue)
				PixelSaturesZone++;
		}
	}
	return CPointDbl(PixelSommeZone, double(PixelSaturesZone));
}

CPointDbl CFitting::GetFloatPixelsFromZone(CDibImagePtr& ptrDib, CRect Zone, float* dataX /* = 0 */, float* dataY /* = 0 */)
{	
	if(!GetDibMaxWhiteValue(ptrDib))	
		return CPointDbl(0.0,0.0);
	// On regrade si on a affaire à 8bit ou 12bit (16 en fait)
	unsigned short NbrBits = ptrDib->GetBitsPerPixel();

	__int64 PixelSommeZone = 0;
	__int64 PixelSaturesZone = 0;
	Zone.NormalizeRect();
	CPoint TopLeft = Zone.TopLeft();
	long NumPointX = Zone.Width();
	long NumPointY = Zone.Height();
	long SizeX = ptrDib->GetSize().cx;
	long SizeY = ptrDib->GetSize().cy;
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
			{	if(dataX)
					dataX[i + j*NumPointX] = float(i + j*NumPointX);
				if(dataY)
					dataY[i + j*NumPointX] = float(PtrPixel_8bit[i + j*SizeX]);
				PixelSommeZone += PtrPixel_8bit[i + j*SizeX];
				if(PtrPixel_8bit[i + j*SizeX] >= m_MaxWhiteValue)
					PixelSaturesZone++;
			}
		}break;
		case 16:
		for(long j=0 ; j < NumPointY ; j++)
		{	for(long i=0 ; i < NumPointX ; i++)
			{	if(dataX)
					dataX[i + j*NumPointX] = float(i + j*NumPointX);
				if(dataY)
					dataY[i + j*NumPointX] = float(PtrPixel_16bit[i + j*SizeX]);
				PixelSommeZone += PtrPixel_16bit[i + j*SizeX];
				if(PtrPixel_16bit[i + j*SizeX] >= m_MaxWhiteValue)
					PixelSaturesZone++;
			}
		}break;
	}
	return CPointDbl(double(PixelSommeZone), double(PixelSaturesZone));
}

int CFitting::GetDibMaxWhiteValue(CDibImagePtr& ptrDib)
{	
	if (!(bool)ptrDib)	
		return 0;
	// On regrade si on a affaire à 8bit ou 12bit (16 en fait)
	unsigned short NbrBits = ptrDib->GetBitsPerPixel();
	// Et on fixe la valeur de m_MaxWhiteValue qui correspond
	switch(NbrBits)
	{
	case 8:		m_MaxWhiteValue = 255;		break;
	case 16:	m_MaxWhiteValue = 4095;		break;
	}
	return m_MaxWhiteValue;
}



void CFitting::WriteProfileOnDib(CDibImagePtr& ptrDib, CRect Zone)
{
	if (!(bool)ptrDib)	
		return;
	Zone.NormalizeRect();
	PrepareForUse(Zone);
	CPoint TopLeft = Zone.TopLeft();
	long NumPointX = Zone.Width();
	long NumPointY = Zone.Height();
	// On regrade si on a affaire à 8bit ou 12bit
	unsigned short NbrBits = ptrDib->GetBitsPerPixel();
	// Et on fixe la valeur de m_MaxWhiteValue qui correspond
	switch(NbrBits)
	{
	case 8:
		m_MaxWhiteValue = 255;
		break;
	case 16:
		m_MaxWhiteValue = 4095;
		break;
	}
	long SizeX = ptrDib->GetSize().cx;
	long SizeY = ptrDib->GetSize().cy;
	unsigned char*  PtrPixel_8bit  = (unsigned char*) ptrDib->GetPixels();
	unsigned short* PtrPixel_16bit = (unsigned short*)ptrDib->GetPixels();
	PtrPixel_8bit  += TopLeft.x + TopLeft.y * SizeX;
	PtrPixel_16bit += TopLeft.x + TopLeft.y * SizeX;

	m_pParamAjustables[0] = float(NumPointX);

	// par quoi va t'on multiplier la valeur trouvée pour la normaliser à m_RangeAbsSerie dans l'affichage
	double multipl_For_RangeAbs = CSerieImage::m_ModeAbsorp ? double(m_MaxWhiteValue) / CSerieImage::m_RangeAbsSerie : 1.0;

	float Adyda[MAX_PARAM_FIT+1];
	for(long j = 0 ; j < NumPointY ; j++)
	{
		for(long i = 0 ; i < NumPointX ; i++)
		{
			switch(NbrBits)
			{
			case 8:
				m_LaFonction(float(i + j*NumPointX) , m_pParamAjustables , &m_YFitted , Adyda , m_nPoints);
				PtrPixel_8bit[i + j*SizeX] = unsigned char(max(0, min(255, m_YFitted * multipl_For_RangeAbs)));
				break;
			case 16:
				m_LaFonction(float(i + j*NumPointX) , m_pParamAjustables , &m_YFitted , Adyda , m_nPoints);
				PtrPixel_16bit[i + j*SizeX] =  unsigned short(max(0, min(4095, m_YFitted * multipl_For_RangeAbs)));
				break;
			}
		}
	}
return;
}


void CFitting::Nettoye()
{
	free_matrix(m_pCovar,1,m_nParam,1,m_nParam);
	free_matrix(m_pAlpha,1,m_nParam,1,m_nParam);
	free_matrix(m_pSigma,1,1,1,m_nPoints);
	free_matrix(m_pDataX,1,1,1,m_nPoints);
	free_matrix(m_pDataY,1,1,1,m_nPoints);
	free_matrix(m_pFitY	,1,1,1,m_nPoints);
	m_pCovar = NULL;
	m_pAlpha = NULL;
	m_pSigma = NULL;
	m_pDataX = NULL;
	m_pDataY = NULL;
	m_pFitY	 = NULL;
}

int CFitting::InTheCabbage()
{
////////////////////////////////////////////////////////////////////////////////////////////////
//return 0;
////////////////////////////////////////////////////////////////////////////////////////////////
	for(int i = 1; i <= m_nParam; i++)
	{
		float multiplicateur = 1;
		if(m_pStringEchelleFit[i] == "PTX")
			multiplicateur = float(m_nPtX);
		if(m_pStringEchelleFit[i] == "PTY")
			multiplicateur = float(m_nPtY);
		if(m_pStringEchelleFit[i] == "MaxWhite")
			multiplicateur = float(m_MaxWhiteValue);

		if(	m_pParamAjustables[i] < m_pCabbageDown[i] * multiplicateur || m_pParamAjustables[i] > m_pCabbageUp[i] * multiplicateur)
			if(m_pDoitAjuster[i])
				return i;

////////////////////////////////////////////////////////////////////////////////////////////////////
		// Des condition en plus : basé sur la recherche du nom du parametre comme dans "ResteFixe"

		// par exemple ici pour la deuxième gaussienne
		// si les 2 largeur sont les meme à XX pourcent près: |L1-L2| < (|L1|+|L2|)*XX
		if(m_pStringNomParams[i].Find( "V_Del_1" ) != -1 )
			if(		abs(m_pParamAjustables[i] - m_pParamAjustables[i+4]) < (abs(m_pParamAjustables[i]) + abs(m_pParamAjustables[i+4])) * 0.3		)
				if(m_pDoitAjuster[i])
					return i;		

		// si les 2 centres sont séparés de plus que la différence des largeurs: |C1-C2| > (|L1-L2|)*XX
		if(m_pStringNomParams[i].Find( "V_Cent_1" ) != -1 )
			if(		abs(m_pParamAjustables[i] - m_pParamAjustables[i+4]) > abs(m_pParamAjustables[i+2-1] - m_pParamAjustables[i+4+2-1]) * 1		)
				if(m_pDoitAjuster[i])
					return i;		
	}
return 0;
}


/*
Sauve le resultat du Fit
*/
int CFitting::SaveResult(CString FileName, double ImageIndex, float PixelSize, float AbsorpScaling, bool ImagePasBonne, int ImageNumero, CString FileNameVariation		, CString BonusNom1 /* = "" */, float BonusValeur1 /* = 0.0 */
																																								, CString BonusNom2 /* = "" */, float BonusValeur2 /* = 0.0 */)
{
SaveResult(NULL, FileName, ImageIndex, PixelSize, AbsorpScaling, ImagePasBonne, ImageNumero, FileNameVariation	, BonusNom1, BonusValeur1, BonusNom2, BonusValeur2);
return 0;
}

#define SEPARATOR "\t"
/*
Surcharge pour permettre l'écriture de plusieur Fit dans le meme fichier
Se sert dans les Parametres de Fit, ET dans les ParamètreEnPlus qui sont affichés dans le fenètre
*/
int CFitting::SaveResult(CFitting* pDeuxiemeFit, CString FileName, double ImageIndex, float ThePixelSize, float TheAbsorpScaling
						 , bool ImagePasBonne, int TheImageNumero, CString TheFileNameVariation /* = "" */	
						 , CString BonusNom1 /* = "" */, float BonusValeur1 /* = 0.0 */
						 , CString BonusNom2 /* = "" */, float BonusValeur2 /* = 0.0 */)
{
	// PS : Si filename = "" alors une boite de dialogue CFileDialog doit s'ouvrir...
    CTextFile tf("txt","");
	CString LigneToSave;
	CString tempAppend;
	// le fichier existe t-il déjà ?
	bool dontexist = _access(FileName,00) ? true : false;

	// si le fichier txt n'existe pas
	if(dontexist) 
	{
		// la première chose écrite dans le fichier est "Index"
		LigneToSave.Format("Index  %s", SEPARATOR);
		// sur cette 1ère ligne, on fout "les noms"
		for(int i = 1; i <= m_nParam; i++)
		{	
			tempAppend.Format("%s[%s]%s", m_pStringNomParams[i], m_pStringUnites[i], SEPARATOR);
			LigneToSave += tempAppend;
		}
		// des m_pParamEnPlus eventuels
		for(int i = 0; i < m_pParamEnPlus_Combien; i++)
		{	
			int dummy = 0;
			// seulement si il y a un truc interessant:
			if(m_pParamEnPlus_Nom[i] && m_pParamEnPlus_Nom[i]->Tokenize("0123456789.", dummy) != "")
			{
				tempAppend.Format("%s[%s]%s", *m_pParamEnPlus_Nom[i], "?", SEPARATOR);
				LigneToSave += tempAppend;
			}
		}
		tempAppend.Format("%s", "\r\n");
		LigneToSave += tempAppend;
		// et ecrire les deux lignes qui nomment les Colonnes
		tf.WriteTextFile(FileName, LigneToSave);
	}	

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// maintenant, Les Valeurs à afficher !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// On fait le chois d'afficher "bad\t" si on a passé le parametre ImagePasBonne = true
	// Le numéro de l'image
	LigneToSave.Format("%g%s", ImageIndex, SEPARATOR);
	// Les Reultats de Fit dans la bonne Unité
	for(int i = 1; i <= m_nParam; i++)
	{	
		tempAppend.Format(ImagePasBonne? "bad%.3E%s" : "%.6E%s", m_pParamEnUnites[i], SEPARATOR);
		LigneToSave += tempAppend;
	}
	// des m_pParamEnPlus eventuels
	for(int i = 0; i < m_pParamEnPlus_Combien; i++)
	{	
		int dummy = 0;
		// seulement si il y a un truc interessant:
		if(m_pParamEnPlus_Nom[i] && m_pParamEnPlus_Nom[i]->Tokenize("0123456789.", dummy) != "")
		{
			tempAppend.Format(ImagePasBonne? "bad%.3E%s" : "%.6E%s", *m_pParamEnPlus_Val[i], SEPARATOR);
			LigneToSave += tempAppend;
		}
	}
	tempAppend.Format("%s", "\r\n");
	LigneToSave += tempAppend;
	tf.AppendFile(FileName, LigneToSave);
return 0;
}


double sq2 = sqrt(2.0);
double invsq2 = 1/sqrt(2.0);
double DegreeToRad = 3.141592 / 180.0;

void CFitting::DessineFonction(CDC* pUnDc)
{
	if(m_LaFonction == fUneGaussienne2D || m_LaFonction == fUneGaussienne2DTilt)
	{	
		Dessine2DGaussienne(pUnDc);
	}
	if(m_LaFonction == fUneGaussienne2DTigeGauss)
	{	
		bool UneSeuleGaussienne = true;
		Dessine2DTigeDoubleGauss(pUnDc, UneSeuleGaussienne);
	}
	if(m_LaFonction == fUneGaussienne2DTigeDoubleGauss)
	{	
		Dessine2DTigeDoubleGauss(pUnDc);
	}
	if(m_LaFonction == fUne2DCondensat)
	{	
		Dessine2DCondensat(pUnDc);
	}

}

void CFitting::Dessine2DGaussienne(CDC* pUnDc)
{
	CDC& dc = *pUnDc;
	dc.SelectObject(&PenJaune);	

	float* a = m_pParamAjustables;
	float& Ampl		= a[1];
	float& X0		= a[2];
	float& DeltaX	= a[3];
	float& Offset	= a[4];
	float& Y0		= a[5];
	float& DeltaY	= a[6];
	float& Theta	= a[7];

// dessin de l'ellipse inscrit dans le rectangle "inscrit"
	CRect inscrit = CRect(0,0,0,0);
	inscrit.MoveToXY(PixelImageToEcran(m_ZoneToFit.CenterPoint() + CPoint(int(X0), int(-Y0))));
	CPoint LesDelta = PixelImageToEcran(CPoint(int(abs(DeltaX)), int(abs(DeltaY))));
	inscrit.InflateRect( LesDelta.x, LesDelta.y);
	CPoint centerPt = inscrit.CenterPoint();

// transformation de coordonnée de l'affichage pour afficher une ellipse tournée ! ! ! ! ! ! !	//
// cf : http://www.codeproject.com/gdi/setworldtransform().asp									//
	int nGraphicsMode = SetGraphicsMode(dc, GM_ADVANCED);										//
	XFORM xform;																				//
	double RadAngle = -Theta * DegreeToRad;														//
	// si le tilt est nul, on ne change pas de coordonnées										//
	if (abs(RadAngle) < 1e10)													//
	{																							//
		xform.eM11 = (float)cos(RadAngle);														//
		xform.eM12 = (float)sin(RadAngle);														//
		xform.eM21 = (float)-sin(RadAngle);														//
		xform.eM22 = (float)cos(RadAngle);														//
		xform.eDx  = (float)(centerPt.x - cos(RadAngle)*centerPt.x + sin(RadAngle)*centerPt.y);	//
		xform.eDy  = (float)(centerPt.y - cos(RadAngle)*centerPt.y - sin(RadAngle)*centerPt.x);	//
		SetWorldTransform(dc, &xform);															//
	}																							//
																								//
//	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//
		dc.Arc(	inscrit, CPoint(0,0), CPoint(0,0) );											//
		dc.MoveTo(centerPt.x - int(LesDelta.x * sq2)	, centerPt.y						);					//
		dc.LineTo(centerPt.x + int(LesDelta.x * sq2)	, centerPt.y						);					//
		dc.MoveTo(centerPt.x							, centerPt.y - int(LesDelta.y*sq2)	);				//
		dc.LineTo(centerPt.x							, centerPt.y + int(LesDelta.y*sq2)	);				//
																								//
	if (abs(RadAngle) < 1e10)																	//
	{																							//
		xform.eM11 = (float)1.0;																//
		xform.eM12 = (float)0;																	//
		xform.eM21 = (float)0;																	//
		xform.eM22 = (float)1.0;																//
		xform.eDx  = (float)0;																	//
		xform.eDy  = (float)0;																	//
		SetWorldTransform(dc, &xform);															//
	}																							//
	SetGraphicsMode(dc, nGraphicsMode);															//
//	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//

// dessin du trait qui relie le centre de la croix et le centre de l'ellipse
	dc.SelectObject(&PenRouge);	
	dc.MoveTo(	PixelImageToEcran(	m_ZoneToFit.CenterPoint()  ));
	dc.LineTo(centerPt);
}

void CFitting::Dessine2DCondensat(CDC* pUnDc)
{
	CDC& dc = *pUnDc;
	dc.SelectObject(&PenRouge);	

	float* a = m_pParamAjustables;
	float ThqAmp	= a[1];
	float X0		= a[2];
	float ThqDX		= a[3];
	float Offset	= a[4];
	float Y0		= a[5];
	float ThqDY		= a[6];
	float BECAmp	= a[7];
	float BECDX		= a[8];
	float BECDY		= a[9];
	float Theta		= a[10];

	// dessin du BEC, puis du nuage thiq
	for(int i = 1; i <= 2; i++)
	{
		// dessin de l'ellipse inscrit dans le rectangle "inscrit"
		// BEC
		CRect inscrit = CRect(0,0,0,0);
		inscrit.MoveToXY(PixelImageToEcran(m_ZoneToFit.CenterPoint() + CPoint(int(X0), int(-Y0))));
		CPoint LesDelta = PixelImageToEcran(CPoint(int(abs(BECDX)), int(abs(BECDY))));
		inscrit.InflateRect( LesDelta.x, LesDelta.y);
		CPoint centerPt = inscrit.CenterPoint();
		// Nuage thiq
		if(i == 2)
		{
			inscrit = CRect(0,0,0,0);
			inscrit.MoveToXY(PixelImageToEcran(m_ZoneToFit.CenterPoint() + CPoint(int(X0), int(-Y0))));
			LesDelta = PixelImageToEcran(CPoint(int(abs(ThqDX)), int(abs(ThqDY))));
			inscrit.InflateRect( LesDelta.x, LesDelta.y);
			centerPt = inscrit.CenterPoint();
			Theta = 0;
		}

		// transformation de coordonnée de l'affichage pour afficher une ellipse tournée ! ! ! ! ! !//
		// cf : http://www.codeproject.com/gdi/setworldtransform().asp								//
		int nGraphicsMode = SetGraphicsMode(dc, GM_ADVANCED);										//
		XFORM xform;																				//
		double RadAngle = -Theta * DegreeToRad;														//
		// si le tilt est nul, on ne change pas de coordonnées										//
		if (abs(RadAngle) < 1e10)																	//
		{																							//
			xform.eM11 = (float)cos(RadAngle);														//
			xform.eM12 = (float)sin(RadAngle);														//
			xform.eM21 = (float)-sin(RadAngle);														//
			xform.eM22 = (float)cos(RadAngle);														//
			xform.eDx  = (float)(centerPt.x - cos(RadAngle)*centerPt.x + sin(RadAngle)*centerPt.y);	//
			xform.eDy  = (float)(centerPt.y - cos(RadAngle)*centerPt.y - sin(RadAngle)*centerPt.x);	//
			SetWorldTransform(dc, &xform);															//
		}																							//
																									//
		//	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//
		if(i == 2)
			dc.SelectObject(&PenJaune);	
		dc.Arc(	inscrit, CPoint(0,0), CPoint(0,0) );												//
		if(i == 1)
		{
			dc.MoveTo(centerPt.x - int(LesDelta.x * sq2)	, centerPt.y						);	//
			dc.LineTo(centerPt.x + int(LesDelta.x * sq2)	, centerPt.y						);	//
			dc.MoveTo(centerPt.x							, centerPt.y - int(LesDelta.y*sq2)	);	//
			dc.LineTo(centerPt.x							, centerPt.y + int(LesDelta.y*sq2)	);	//
		}
		//
		if (abs(RadAngle) < 1e10)																	//
		{																							//
			xform.eM11 = (float)1.0;																//
			xform.eM12 = (float)0;																	//
			xform.eM21 = (float)0;																	//
			xform.eM22 = (float)1.0;																//
			xform.eDx  = (float)0;																	//
			xform.eDy  = (float)0;																	//
			SetWorldTransform(dc, &xform);															//
		}																							//
		SetGraphicsMode(dc, nGraphicsMode);															//
		//	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//

		// dessin du trait qui relie le centre de la croix et le centre de l'ellipse
		// BEC
		dc.SelectObject(&PenRouge);	
		dc.MoveTo(	PixelImageToEcran(	m_ZoneToFit.CenterPoint()  ));
		if(i == 1)
			dc.LineTo(centerPt);
	}
}

void CFitting::Dessine2DTigeDoubleGauss(CDC* pUnDc, bool DessineUneSeuleGaussienne /* = false */)
{
	CDC& dc = *pUnDc;
	dc.SelectObject(&PenJaune);	

	float* a = m_pParamAjustables;
	float Ampl_1	= a[1];
	float Y0_1		= a[2];
	float DeltaY_1	= a[3];
	float Offset	= a[4];
	float Ampl_2	= DessineUneSeuleGaussienne? 0 : a[5];
	float Y0_2		= DessineUneSeuleGaussienne? 0 : a[6];
	float DeltaY_2	= DessineUneSeuleGaussienne? 0 : a[7];
	float Theta		= DessineUneSeuleGaussienne? a[5] : a[8];

	CPoint centerPt = PixelImageToEcran(m_ZoneToFit.CenterPoint());
	CPoint cent_1 =   PixelImageToEcran(m_ZoneToFit.CenterPoint() + CPoint(0, int(-Y0_1)));
	CPoint cent_2 =   PixelImageToEcran(m_ZoneToFit.CenterPoint() + CPoint(0, int(-Y0_2)));
	CPoint del_1 =    PixelImageToEcran(CPoint(0, int(DeltaY_1)));
	CPoint del_2 =    PixelImageToEcran(CPoint(0, int(DeltaY_2)));

// transformation de coordonnée de l'affichage pour afficher une ellipse tournée ! ! ! ! ! ! !	//
// cf : http://www.codeproject.com/gdi/setworldtransform().asp									//
	int nGraphicsMode = SetGraphicsMode(dc, GM_ADVANCED);										//
	XFORM xform;																				//
	double RadAngle = -Theta * DegreeToRad;														//
	// si le tilt est nul, on ne change pas de coordonnées										//
	if (abs(RadAngle) < 1e10)													//
	{																							//
		xform.eM11 = (float)cos(RadAngle);														//
		xform.eM12 = (float)sin(RadAngle);														//
		xform.eM21 = (float)-sin(RadAngle);														//
		xform.eM22 = (float)cos(RadAngle);														//
		xform.eDx  = (float)(centerPt.x - cos(RadAngle)*centerPt.x + sin(RadAngle)*centerPt.y);	//
		xform.eDy  = (float)(centerPt.y - cos(RadAngle)*centerPt.y - sin(RadAngle)*centerPt.x);	//
		SetWorldTransform(dc, &xform);															//
	}																							//
																								//
//	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//
	dc.SelectObject(&PenJaune);	
		dc.MoveTo(-1000							, cent_1.y);				//
		dc.LineTo(1000							, cent_1.y);				//
		dc.MoveTo(-1000							, cent_1.y + del_1.y);				//
		dc.LineTo(1000							, cent_1.y + del_1.y);				//
		dc.MoveTo(-1000							, cent_1.y - del_1.y);				//
		dc.LineTo(1000							, cent_1.y - del_1.y);				//

		if(!DessineUneSeuleGaussienne)
		{
			dc.SelectObject(&PenGreen);	
			dc.MoveTo(-1000							, cent_2.y);				//
			dc.LineTo(1000							, cent_2.y);				//
			dc.MoveTo(-1000							, cent_1.y + del_2.y);				//
			dc.LineTo(1000							, cent_1.y + del_2.y);				//
			dc.MoveTo(-1000							, cent_1.y - del_2.y);				//
			dc.LineTo(1000							, cent_1.y - del_2.y);				//
		}//
	if (abs(RadAngle) < 1e10)																	//
	{																							//
		xform.eM11 = (float)1.0;																//
		xform.eM12 = (float)0;																	//
		xform.eM21 = (float)0;																	//
		xform.eM22 = (float)1.0;																//
		xform.eDx  = (float)0;																	//
		xform.eDy  = (float)0;																	//
		SetWorldTransform(dc, &xform);															//
	}																							//
	SetGraphicsMode(dc, nGraphicsMode);															//
//	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	.	//

// dessin du trait qui relie le centre de la croix et le centre de l'ellipse
	dc.SelectObject(&PenRouge);	
	dc.MoveTo(	PixelImageToEcran(	m_ZoneToFit.CenterPoint()  ));
	dc.LineTo(centerPt);
}
/*
	à partir de "x" et de "na" Parameteres "a[]" --> on créée "y" et "dy/da[]"
*/
void fUneGaussienne(float X, float a[], float *y, float dyda[], int na)
{
	float& Ampl		= a[1];
	float& X0		= a[2];
	float& DeltaX	= a[3];
	float& Offset	= a[4];

	float& dy_Sur_dAmpl		= dyda[1];
	float& dy_Sur_dX0		= dyda[2];
	float& dy_Sur_dDeltaX	= dyda[3];
	float& dy_Sur_dOffset	= dyda[4];

	// on a ici "na=4"
	float fac,ex,arg;
	*y=0.0;

	arg = (X-X0)/DeltaX*invsq2;
	ex  = exp(-arg*arg);
	fac = Ampl*ex*float(2.0)*arg;

	*y += Ampl*ex;
	*y += Offset;

	dy_Sur_dAmpl	= ex;
	dy_Sur_dX0		= fac/DeltaX*invsq2;
	dy_Sur_dDeltaX	= fac*arg/DeltaX;
	dy_Sur_dOffset	= 1;

return;
}

void fUneDoubleGaussienne(float X, float a[], float *y, float dyda[], int na)
{
	float& Ampl_1	= a[1];
	float& X0_1		= a[2];
	float& DeltaX_1	= a[3];
	float& Offset	= a[4];
	float& Ampl_2	= a[5];
	float& X0_2		= a[6];
	float& DeltaX_2	= a[7];

	float& dy_Sur_dAmpl_1	= dyda[1];
	float& dy_Sur_dX0_1		= dyda[2];
	float& dy_Sur_dDeltaX_1	= dyda[3];
	float& dy_Sur_dOffset	= dyda[4];
	float& dy_Sur_dAmpl_2	= dyda[5];
	float& dy_Sur_dX0_2		= dyda[6];
	float& dy_Sur_dDeltaX_2	= dyda[7];

	// on a ici "na=7"
	float fac_1,ex_1,arg_1,fac_2,ex_2,arg_2;
	*y=0.0;

	arg_1 = (X-X0_1)/DeltaX_1*invsq2;
	ex_1  = exp(-arg_1*arg_1);
	fac_1 = Ampl_1*ex_1*float(2.0)*arg_1;
	arg_2 = (X-X0_2)/DeltaX_2*invsq2;
	ex_2  = exp(-arg_2*arg_2);
	fac_2 = Ampl_2*ex_2*float(2.0)*arg_2;

	*y += Ampl_1*ex_1;
	*y += Ampl_2*ex_2;
	*y += Offset;

	dy_Sur_dAmpl_1		= ex_1;
	dy_Sur_dX0_1		= fac_1/DeltaX_1*invsq2;
	dy_Sur_dDeltaX_1	= fac_1*arg_1/DeltaX_1;
	dy_Sur_dOffset		= 1;
	dy_Sur_dAmpl_2		= ex_2;
	dy_Sur_dX0_2		= fac_2/DeltaX_2*invsq2;
	dy_Sur_dDeltaX_2	= fac_2*arg_2/DeltaX_2;

return;
}
void fUneGaussienne2D(float xy, float a[], float *y, float dyda[], int na)
{
	// ASTUCE !  NumPointsX et NumPointsY sont passés en Paramètre par a[0]
	// dans la mesure où  nPoint = NumPointsX * NumPointsY, et que j'ai modifié
	// mrqcof() pour passer ndata en parametre à la fonction, on obtient 
	int NumPointsX = int(a[0]);
	int NumPointsY = na / NumPointsX;

	float& Ampl		= a[1];
	float& X0		= a[2];
	float& DeltaX	= a[3];
	float& Offset	= a[4];
	float& Y0		= a[5];
	float& DeltaY	= a[6];

	// assurons nous de donner a DeltaX & Y une valeur positive, c'est plus joli!
	DeltaX = abs(DeltaX);
	DeltaY = abs(DeltaY);

	float& dy_Sur_dAmpl		= dyda[1];
	float& dy_Sur_dX0		= dyda[2];
	float& dy_Sur_dDeltaX	= dyda[3];
	float& dy_Sur_dOffset	= dyda[4];
	float& dy_Sur_dY0		= dyda[5];
	float& dy_Sur_dDeltaY	= dyda[6];

    // on a ici "na=6"
	float facx1,facx2,ex,argx1,argx2;
	// On definit les coordonnée X et Y et on place l'origine au milieu de la zone à fiter
	// et on redresse Y en -Y pour avoir les + vers la haut de l'ecran
	float X =   float(int(xy)%NumPointsX) - NumPointsX/2;
	float Y = -(float(int(xy)/NumPointsX) - NumPointsY/2);

	*y=0.0;

	argx1 = (X-X0)/DeltaX*invsq2;
	argx2 = (Y-Y0)/DeltaY*invsq2;
	ex    = exp( - argx1*argx1 - argx2*argx2);
	facx1 = Ampl*ex*float(2.0)*argx1;
	facx2 = Ampl*ex*float(2.0)*argx2;

	*y += Ampl*ex;
	*y += Offset;

	dy_Sur_dAmpl	= ex;
	dy_Sur_dX0		= facx1/DeltaX*invsq2;
	dy_Sur_dDeltaX	= facx1*argx1/DeltaX;
	dy_Sur_dOffset	= 1;
	dy_Sur_dY0		= facx2/DeltaY*invsq2;
	dy_Sur_dDeltaY	= facx2*argx2/DeltaY;

return;
}

void fUneGaussienne2DTilt(float xy, float a[], float *y, float dyda[], int na)
{	
	// ASTUCE !  NumPointsX et NumPointsY sont passés en Paramètre par a[0]
	// dans la mesure où  nPoint = NumPointsX * NumPointsY, et que j'ai modifié
	// mrqcof() pour passer ndata en parametre à la fonction, on obtient 
	int NumPointsX = int(a[0]);
	int NumPointsY = na / NumPointsX;

	float& Ampl		= a[1];
	float& X0		= a[2];
	float& DeltaX	= a[3];
	float& Offset	= a[4];
	float& Y0		= a[5];
	float& DeltaY	= a[6];
	float& Theta	= a[7];

	// assurons nous de donner a DeltaX & Y une valeur positive et Theta dans +-180°, c'est plus joli!
	DeltaX = max(abs(DeltaX), 1e-15);
	DeltaY = max(abs(DeltaY), 1e-15);
	Theta = Theta - 180.0 * (int(Theta)/180);

	float& dy_Sur_dAmpl		= dyda[1];
	float& dy_Sur_dX0		= dyda[2];
	float& dy_Sur_dDeltaX	= dyda[3];
	float& dy_Sur_dOffset	= dyda[4];
	float& dy_Sur_dY0		= dyda[5];
	float& dy_Sur_dDeltaY	= dyda[6];
	float& dy_Sur_dTehta	= dyda[7];

	// On definit les coordonnée X et Y et on place l'origine au milieu de la zone à fiter
	// et on redresse Y en -Y pour avoir les + vers la haut de l'ecran
	float X =   float(int(xy)%NumPointsX) - NumPointsX/2;
	float Y = -(float(int(xy)/NumPointsX) - NumPointsY/2);
	float t = -Theta;

	*y=0.0;
	float CosTheta = cos( t * DegreeToRad);
	float SinTheta = sin( t * DegreeToRad);

	double ArgX, ArgY, ex, Facteur1, Facteur2, Facteur3;
	ArgX = ((X-X0)*CosTheta-(Y-Y0)*SinTheta)/DeltaX*invsq2;
	ArgY = ((Y-Y0)*CosTheta+(X-X0)*SinTheta)/DeltaY*invsq2;
	ex		 = exp( - ArgX*ArgX - ArgY*ArgY);
	Facteur1 = 2.0*Ampl*ArgX*ex;
	Facteur2 = 2.0*Ampl*ArgY*ex;
    Facteur3 = Facteur1*Facteur2 / ex;
	if(_isnan(Facteur3))
		Facteur3 = 0.0;

	*y += Ampl*ex;
	*y += Offset;

	dy_Sur_dAmpl	= ex;
	dy_Sur_dX0		= CosTheta*Facteur1/DeltaX*invsq2 + SinTheta*Facteur2/DeltaY*invsq2;
	dy_Sur_dDeltaX	= ArgX*Facteur1/DeltaX;
	dy_Sur_dOffset	= 1.0;
	dy_Sur_dY0		= CosTheta*Facteur2/DeltaY*invsq2 - SinTheta*Facteur1/DeltaX*invsq2;
	dy_Sur_dDeltaY	= ArgY*Facteur2/DeltaY;
	dy_Sur_dTehta	= 0.5*DegreeToRad*Facteur3*(DeltaX-DeltaY)*(DeltaX+DeltaY)/(Ampl*DeltaX*DeltaY);
	
return;
}
void fUneGaussienne2DTigeGauss(float xy, float a[], float *y, float dyda[], int na)
{	
	// cf Fichier Maple : "Fit_2DDoubleGaussienne_Tige.mw"
	int NumPointsX = int(a[0]);
	int NumPointsY = na / NumPointsX;

	float& Ampl_1	= a[1];
	float& Y0_1		= a[2];
	float& DeltaY_1	= a[3];
	float& Offset	= a[4];
	float& Theta	= a[5];

	// assurons nous de donner a DeltaY_1 & 2 une valeur positive, c'est plus joli!
	// assurons nous de donner a DeltaX & Y une valeur positive et Theta dans +-180°, c'est plus joli!
	DeltaY_1 = max(abs(DeltaY_1), 1e-15);
	Theta = Theta - 180.0 * (int(Theta)/180);

	float& dy_Sur_dAmpl_1	= dyda[1];
	float& dy_Sur_dY0_1		= dyda[2];
	float& dy_Sur_dDeltaY_1	= dyda[3];
	float& dy_Sur_dOffset	= dyda[4];
	float& dy_Sur_dTheta	= dyda[5];

	// On definit les coordonnée X et Y et on place l'origine au milieu de la zone à fiter
	// et on redresse Y en -Y pour avoir les + vers la haut de l'ecran
	float X =   float(int(xy)%NumPointsX) - NumPointsX/2;
	float Y = -(float(int(xy)/NumPointsX) - NumPointsY/2);
	float t = -Theta;

	*y=0.0;
	float CosTheta = cos( t * DegreeToRad);
	float SinTheta = sin( t * DegreeToRad);

	float ArgY_1, ex_1,Facteur_1;
	ArgY_1		= (X * SinTheta + CosTheta * Y - Y0_1) / DeltaY_1;
	ex_1		= exp(-ArgY_1 * ArgY_1);
	Facteur_1	= float(2.0) * Ampl_1 * ArgY_1 * ex_1;
	if(_isnan(Facteur_1))
		Facteur_1 = 0.0;
   
	*y += Ampl_1*ex_1;
	*y += Offset;

	dy_Sur_dAmpl_1	= ex_1;
	dy_Sur_dY0_1	= Facteur_1/DeltaY_1;
	dy_Sur_dDeltaY_1= ArgY_1*Facteur_1/DeltaY_1;
	dy_Sur_dOffset	= 1;
	dy_Sur_dTheta	= (X * CosTheta - Y * SinTheta) * Facteur_1 * DegreeToRad / DeltaY_1;
}

void fUneGaussienne2DTigeDoubleGauss(float xy, float a[], float *y, float dyda[], int na)
{	
	// cf Fichier Maple : "Fit_2DDoubleGaussienne_Tige.mw"
	int NumPointsX = int(a[0]);
	int NumPointsY = na / NumPointsX;

	float& Ampl_1	= a[1];
	float& Y0_1		= a[2];
	float& DeltaY_1	= a[3];
	float& Offset	= a[4];
	float& Ampl_2	= a[5];
	float& Y0_2		= a[6];
	float& DeltaY_2	= a[7];
	float& Theta	= a[8];

	// assurons nous de donner a DeltaY_1 & 2 une valeur positive, c'est plus joli!
	// assurons nous de donner a DeltaX & Y une valeur positive et Theta dans +-180°, c'est plus joli!
	DeltaY_1 = max(abs(DeltaY_1), 1e-15);
	DeltaY_2 = max(abs(DeltaY_2), 1e-15);
	Theta = Theta - 180.0 * (int(Theta)/180);

	float& dy_Sur_dAmpl_1	= dyda[1];
	float& dy_Sur_dY0_1		= dyda[2];
	float& dy_Sur_dDeltaY_1	= dyda[3];
	float& dy_Sur_dOffset	= dyda[4];
	float& dy_Sur_dAmpl_2	= dyda[5];
	float& dy_Sur_dY0_2		= dyda[6];
	float& dy_Sur_dDeltaY_2	= dyda[7];
	float& dy_Sur_dTheta	= dyda[8];

	// On definit les coordonnée X et Y et on place l'origine au milieu de la zone à fiter
	// et on redresse Y en -Y pour avoir les + vers la haut de l'ecran
	float X =   float(int(xy)%NumPointsX) - NumPointsX/2;
	float Y = -(float(int(xy)/NumPointsX) - NumPointsY/2);
	float t = -Theta;

	*y=0.0;
	float CosTheta = cos( t * DegreeToRad);
	float SinTheta = sin( t * DegreeToRad);

	float ArgY_1, ArgY_2, ex_1, ex_2, Facteur_1, Facteur_2;
	ArgY_1		= (X * SinTheta + CosTheta * Y - Y0_1) / DeltaY_1;
	ArgY_2		= (X * SinTheta + CosTheta * Y - Y0_2) / DeltaY_2;
	ex_1		= exp(-ArgY_1 * ArgY_1);
	ex_2		= exp(-ArgY_2 * ArgY_2);
	Facteur_1	= float(2.0) * Ampl_1 * ArgY_1 * ex_1;
	Facteur_2	= float(2.0) * Ampl_2 * ArgY_2 * ex_2;
	if(_isnan(Facteur_1))
		Facteur_1 = 0.0;
	if(_isnan(Facteur_2))
		Facteur_2 = 0.0;
    
	*y += Ampl_1*ex_1;
	*y += Ampl_2*ex_2;
	*y += Offset;

	dy_Sur_dAmpl_1	= ex_1;
	dy_Sur_dY0_1	= Facteur_1/DeltaY_1;
	dy_Sur_dDeltaY_1= ArgY_1*Facteur_1/DeltaY_1;
	dy_Sur_dOffset	= 1;
	dy_Sur_dAmpl_2	= ex_2;
	dy_Sur_dY0_2	= Facteur_2/DeltaY_2;
	dy_Sur_dDeltaY_2= ArgY_2*Facteur_2/DeltaY_2;
	dy_Sur_dTheta	= (X * CosTheta - Y * SinTheta) * (DeltaY_1 * Facteur_2 + DeltaY_2 * Facteur_1) * DegreeToRad / (DeltaY_1 * DeltaY_2);
}

void fUne2DCondensat(float xy, float a[], float *y, float dyda[], int na)
{	
	// cf Fichier Maple : "Fit_2DCondensat.mw"
	int NumPointsX = int(a[0]);
	int NumPointsY = na / NumPointsX;

	float& ThqAmp	= a[1];
	float& X0		= a[2];
	float& ThqDX	= a[3];
	float& Offset	= a[4];
	float& Y0		= a[5];
	float& ThqDY	= a[6];
	float& BECAmp	= a[7];
	float& BECDX	= a[8];
	float& BECDY	= a[9];
	float& Theta	= a[10];

	// assurons nous de donner a DeltaX & Y une valeur positive et Theta dans +-180°, c'est plus joli!
	BECDX = max(abs(BECDX), 1e-15);
	BECDY = max(abs(BECDY), 1e-15);
	Theta = Theta - 180.0 * (int(Theta)/180);

	float& dy_Sur_dThqAmp	= dyda[1];
	float& dy_Sur_dX0		= dyda[2];
	float& dy_Sur_dThqDX	= dyda[3];
	float& dy_Sur_dOffset	= dyda[4];
	float& dy_Sur_dY0		= dyda[5];
	float& dy_Sur_dThqDY	= dyda[6];
	float& dy_Sur_dBECAmp	= dyda[7];
	float& dy_Sur_dBECDX	= dyda[8];
	float& dy_Sur_dBECDY	= dyda[9];
	float& dy_Sur_dTheta	= dyda[10];

	// On definit les coordonnée X et Y et on place l'origine au milieu de la zone à fiter
	// et on redresse Y en -Y pour avoir les + vers la haut de l'ecran
	float X =   float(int(xy)%NumPointsX) - NumPointsX/2;
	float Y = -(float(int(xy)/NumPointsX) - NumPointsY/2);
	float t = -Theta;

	*y=0.0;
	float CosTheta = cos( t * DegreeToRad);
	float SinTheta = sin( t * DegreeToRad);

}

/*
Levenberg-Marquardt method, attempting to reduce the value 2 of a t between a set of data
points x[1..ndata], y[1..ndata] with individual standard deviations sig[1..ndata],
and a nonlinear function dependent on ma coecients a[1..ma]. The input array ia[1..ma]
indicates by nonzero entries those components of a that should be tted for, and by zero
entries those components that should be held xed at their input values. The program returns
current best-t values for the parameters a[1..ma], and 2 = chisq. The arrays
covar[1..ma][1..ma], alpha[1..ma][1..ma] are used as working space during most
iterations. Supply a routine fUneGaussienne(x,a,yfit,dyda,ma) that evaluates the tting function
yfit, and its derivatives dyda[1..ma] with respect to the tting parameters a at x. On
the rst call provide an initial guess for the parameters a, and set alamda<0 for initialization
(which then sets alamda=.001). If a step succeeds chisq becomes smaller and alamda decreases
by a factor of 10. If a step fails alamda grows by a factor of 10. You must call this
routine repeatedly until convergence is achieved. Then, make one nal call with alamda=0, so
that covar[1..ma][1..ma] returns the covariance matrix, and alpha the curvature matrix.
(Parameters held xed will return zero covariances.)
//*/
void mrqmin(float x[], float y[], float sig[], int ndata, float a[], int ia[],
			int ma, float **covar, float **alpha, float *chisq,
			void (*fonc)(float, float [], float *, float [], int), float *alamda)

{
	void covsrt(float **covar, int ma, int ia[], int mfit);
	int gaussj(float **a, int n, float **b, int m);
	void mrqcof(float x[], float y[], float sig[], int ndata, float a[],
		int ia[], int ma, float **alpha, float beta[], float *chisq,
		void (*fonc)(float, float [], float *, float [], int));
	int j,k,l;
	static int mfit;
	static float ochisq,*atry,*beta,*da,**oneda;
	if (*alamda < 0.0) 
	{ //Initialization.
		atry=::vector(1,ma);
		beta=::vector(1,ma);
		da=::vector(1,ma);
		for (mfit=0,j=1;j<=ma;j++)	{	if (ia[j]) mfit++;	}
		oneda=matrix(1,mfit,1,1);
		*alamda=float(0.1);
		mrqcof(x,y,sig,ndata,a,ia,ma,alpha,beta,chisq,fonc);
		ochisq=(*chisq);
		for (j=1;j<=ma;j++)	{	atry[j]=a[j];	}
	}

	for (j=1;j<=mfit;j++) 
	{ //Alter linearized tting matrix, by augmenting diagonal elements. 
		for (k=1;k<=mfit;k++)	
		{
		//	if(0 && abs(beta[j]) > 1E5 || abs(alpha[j][k]) > 1E5 ){*alamda = 0.0;return;}
			covar[j][k]=alpha[j][k];	
		}
		covar[j][j]=alpha[j][j]*(float(1.0)+(*alamda));
		oneda[j][1]=beta[j];
	}

	if(gaussj(covar,mfit,oneda,1)) //Matrix solution.
	{	for (j=1;j<=mfit;j++) 
    		for (k=1;k<=mfit;k++)
				alpha[j][k]=10;
	*alamda = 0.0;
	//MessageBeep(1);
	return;
	}
	else
		for (j=1;j<=mfit;j++) 
	{
		da[j]=oneda[j][1];
	}

	if (*alamda == 0.0) 
	{ //Once converged, evaluate covariance matrix.
		covsrt(covar,ma,ia,mfit);
		covsrt(alpha,ma,ia,mfit); //Spread out alpha to its full size too.
		free_matrix(oneda,1,mfit,1,1);
		free_vector(da,1,ma);
		free_vector(beta,1,ma);
		free_vector(atry,1,ma);
		return;
	}
	for (j=0,l=1;l<=ma;l++) 
	{//Did the trial succeed?
		if (ia[l]) atry[l]=a[l]+da[++j];
	}
	atry[0]=a[0];
	mrqcof(x,y,sig,ndata,atry,ia,ma,covar,da,chisq,fonc);
	if (*chisq < ochisq) 
	{ //Success, accept the new solution.
		*alamda *= float(0.1);
		ochisq=(*chisq);
		for (j=1;j<=mfit;j++) {
			for (k=1;k<=mfit;k++) alpha[j][k]=covar[j][k];
			beta[j]=da[j];
		}
		for (l=1;l<=ma;l++) 
		{
			a[l]=atry[l];
		}
	} else 
	{ //Failure, increase alamda and return.
		*alamda *= 10.0;
		*chisq=ochisq;
	}
}
/*
Used by mrqmin to evaluate the linearized tting matrix alpha, and vector beta as in (15.5.8),
and calculate 2.
//*/
void mrqcof(float x[], float y[], float sig[], int ndata, float a[], int ia[],
			int ma, float **alpha, float beta[], float *chisq,
			void (*fonc)(float, float [], float *, float [], int))
{
	int i,j,k,l,m,mfit=0;
	float ymod,wt,sig2i,dy,*dyda;
	dyda=::vector(1,ma);
	for (j = 1; j <= ma; j++)
		if (ia[j]) 
			mfit++;
	for (j = 1; j <= mfit; j++) 
	{ // Initialize (symmetric) alpha, beta.
		for (k = 1; k <= j; k++) 
			alpha[j][k]=0.0;
		beta[j]=0.0;
	}
	*chisq=0.0;
	for (i=1;i<=ndata;i++) 
	{ // Summation loop over all data.
		(*fonc)(x[i],a,&ymod,dyda,ndata);
		sig2i=float(1.0)/(sig[i]*sig[i]);
		dy=y[i]-ymod;
		for (j=0,l=1;l<=ma;l++) 
		{
			if (ia[l]) 
			{
				wt=dyda[l]*sig2i;
				for (j++,k=0,m=1;m<=l;m++)
					if (ia[m]) 
						alpha[j][++k] += wt*dyda[m];
				beta[j] += dy*wt;
			}
		}
		*chisq += dy*dy*sig2i; // And nd 2.
	}
	for (j=2;j<=mfit;j++) // Fill in the symmetric side.
		for (k=1;k<j;k++) alpha[k][j]=alpha[j][k];
	free_vector(dyda,1,ma);
}
//*/
/*
Expand in storage the covariance matrix covar, so as to take into account parameters that are
being held xed. (For the latter, return zero covariances.)
//*/
#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}
void covsrt(float **covar, int ma, int ia[], int mfit)
{
	int i,j,k;
	float swap;
	for (i=mfit+1;i<=ma;i++)
		for (j=1;j<=i;j++) covar[i][j]=covar[j][i]=0.0;
	k=mfit;
	for (j=ma;j>=1;j--) {
		if (ia[j]) {
			for (i=1;i<=ma;i++) SWAP(covar[i][k],covar[i][j])
				for (i=1;i<=ma;i++) SWAP(covar[k][i],covar[j][i])
					k--;
		}
	}
}
/*
Linear equation solution by Gauss-Jordan elimination, equation (2.1.1) above. a[1..n][1..n]
is the input matrix. b[1..n][1..m] is input containing the m right-hand side vectors. On
output, a is replaced by its matrix inverse, and b is replaced by the corresponding set of solution
vectors.
//*/
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}
int gaussj(float **a, int n, float **b, int m)
{
	int *indxc,*indxr,*ipiv;
	int i,icol,irow,j,k,l,ll;
	float big,dum,pivinv,temp;
	indxc=ivector(1,n); // The integer arrays ipiv, indxr, and indxc are
	// used for bookkeeping on the pivoting. 
	indxr=ivector(1,n);
	ipiv=ivector(1,n);
	for (j=1;j<=n;j++) 
		ipiv[j]=0;
	for (i=1;i<=n;i++) 
	{ // This is the main loop over the columns to be reduced. 
		big=0.0;
		for (j=1;j<=n;j++) // This is the outer loop of the search for a pivot element. 
			if (ipiv[j] != 1)
				for (k=1;k<=n;k++) 
				{
					if (ipiv[k] == 0) 
					{
						if (fabs(a[j][k]) >= big) 
						{
							big=fabs(a[j][k]);
							irow=j;
							icol=k;
						}
					} 
					else if (ipiv[k] > 1) 
						nrerror("gaussj: Singular Matrix-1");
				}
				++(ipiv[icol]);
				/*
				We now have the pivot element, so we interchange rows, if needed, to put the pivot
				element on the diagonal. The columns are not physically interchanged, only relabeled:
				indxc[i], the column of the ith pivot element, is the ith column that is reduced, while
				indxr[i] is the row in which that pivot element was originally located. If indxr[i] 6=
				indxc[i] there is an implied column interchange. With this form of bookkeeping, the
				solution b's will end up in the correct order, and the inverse matrix will be scrambled
				by columns.
				//*/
				if (irow != icol) 
				{
					for (l=1;l<=n;l++) 
						SWAP(a[irow][l],a[icol][l])
						for (l=1;l<=m;l++) 
							SWAP(b[irow][l],b[icol][l])
				}
				indxr[i]=irow; // We are now ready to divide the pivot row by the pivot element, located at irow and icol. 
				indxc[i]=icol;
				if (a[icol][icol] == 0.0) 
				{
					nrerror("gaussj: Singular Matrix-2");
					return 1;
				}
				pivinv=float(1.0)/a[icol][icol];
				a[icol][icol]=1.0;
				for (l=1;l<=n;l++) 
					a[icol][l] *= pivinv;
				for (l=1;l<=m;l++) 
					b[icol][l] *= pivinv;
				for (ll=1;ll<=n;ll++) // Next, we reduce the rows...
					if (ll != icol) 
					{ // ...except for the pivot one, of course.
						dum=a[ll][icol];
						a[ll][icol]=0.0;
						for (l=1;l<=n;l++) 
							a[ll][l] -= a[icol][l]*dum;
						for (l=1;l<=m;l++) 
							b[ll][l] -= b[icol][l]*dum;
					}
	}
	/*
	This is the end of the main loop over columns of the reduction. It only remains to unscramble
	the solution in view of the column interchanges. We do this by interchanging pairs of
	columns in the reverse order that the permutation was built up.
	//*/
	for (l=n;l>=1;l--) 
	{
		if (indxr[l] != indxc[l])
			for (k=1;k<=n;k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	} // And we are done.
	free_ivector(ipiv,1,n);
	free_ivector(indxr,1,n);
	free_ivector(indxc,1,n);

	return 0;
}



