#include "StdAfx.h"
#include "WaveForme.h"
#include ".\dialogwaveform.h"
//srybex #include "muParser.h"
//srybex using namespace mu;


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////      CWaveForme       ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Initialisation des variables static
*/
CList_WF CWaveForme::Liste_WaveFormes;

COLORREF wfGrisee =		(	RGB( 205,205,205 ));
COLORREF wfNoir =		(	RGB(   0,  0,  0 ));
COLORREF wfRouge =		(	RGB( 255,  0,  0 ));
COLORREF wfVert =		(	RGB(   0,255,  0 ));
COLORREF wfBleu =		(	RGB(   0,  0,255 ));
COLORREF wfJaune =		(	RGB( 255,255,  100 ));
COLORREF wfBleuClaire =	(	RGB(   0,255,255 ));
COLORREF wfViolet =		(	RGB( 255,  0,255 ));
COLORREF wfBlanc =		(	RGB( 255,255,255 ));
#define WF_FONT_SIZE 12

double time1 ;
double val1  ;
double time2 ;
double val2  ;


CWaveForme::CWaveForme(void)
: m_SaveWf("Une WaveForme(void)", "Wf", false)
{
	m_nPoints = 0;
	m_ConstrWf_X = NULL;
	m_ConstrWf_Y = NULL;
	// TOUJOURS au moins 2 points ! ! ! !	
	AddPoint(0.0, 0.0);
	AddPoint(WF_DEFAULT_MAX_ABSCISSE, 0.0);
	Liste_WaveFormes.AddTail(this);
}

CWaveForme::CWaveForme(double ValDebut, double ValFin)
: m_SaveWf("Une WaveForme(double, double)", "Wf", false)
{
	m_nPoints = 0;
	m_ConstrWf_X = NULL;
	m_ConstrWf_Y = NULL;
    AddPoint(0, ValDebut);
	AddPoint(WF_DEFAULT_MAX_ABSCISSE, ValFin);
	Liste_WaveFormes.AddTail(this);
}

CWaveForme::CWaveForme(double ValConst)
: m_SaveWf("Une WaveForme(double)", "Wf", false)
{
	m_nPoints = 0;
	m_ConstrWf_X = NULL;
	m_ConstrWf_Y = NULL;
    AddPoint(0, ValConst);
	AddPoint(WF_DEFAULT_MAX_ABSCISSE, ValConst);
	Liste_WaveFormes.AddTail(this);
}

CWaveForme::CWaveForme(const CWaveForme &UneWF)
: m_SaveWf("", "Wf", false)
{
	m_nPoints = 0;
	m_ConstrWf_X = NULL;
	m_ConstrWf_Y = NULL;
	POSITION pos = UneWF.m_Liste_Points.GetHeadPosition();
	while(pos)
	{
		CPointDbl pt = UneWF.m_Liste_Points.GetNext(pos);
		AddPoint(pt.x, pt.y);
	}	
	Liste_WaveFormes.AddTail(this);
}

CWaveForme::~CWaveForme(void)
{
	POSITION pos = Liste_WaveFormes.Find(this);
	if(pos)
		Liste_WaveFormes.RemoveAt(pos);
	if(m_ConstrWf_X)
	{	delete[] m_ConstrWf_X;	m_ConstrWf_X = NULL;}
	if(m_ConstrWf_Y)
	{	delete[] m_ConstrWf_Y;	m_ConstrWf_Y = NULL;}
	m_SaveWf.ClearAllCParametre();
}
	
/*
supprime tous les points de la WF
*/
void CWaveForme::ClearPointList()
{
	m_Liste_Points.RemoveAll();
	m_nPoints = 0;
return;
}
bool CWaveForme::AddPoint(double TimeArbUnit, double Val, bool SiDoubletAtEnd_NajoutePas /* = false */)
{
//	if(TimeArbUnit < 0)
//	{
//		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "Add Point", "Attention, ajout d'un point à T < 0 !", 1);
//	}
	CPointDbl point(TimeArbUnit, Val);
	if(m_nPoints >= 2)
	{
		CPointDbl & LastPoint = m_Liste_Points.GetTail();
		if(LastPoint == point)
			return false;
	}

	m_Liste_Points.AddTail(point);
	m_nPoints++;
//	SortListePoints();
return true;
}

int CWaveForme::AddWaveFormeAtEnd(CWaveForme & WaveFormeToAppend, double ScaleDureeToAppend, double ScaleOrdonnee /* = 1.0 */)
{
	int nbrAdded = 0;
	double StartTimeToAppend = 0.0;
	if(GetnPoints() >= 1)
		StartTimeToAppend = m_Liste_Points.GetTail().x;
	double ScalingFactorTime = 1.0;
	if(WaveFormeToAppend.GetnPoints() >= 2)
		ScalingFactorTime = ScaleDureeToAppend / WaveFormeToAppend.m_Liste_Points.GetTail().x;

	FOR_ALL_POINTS_in_Wf_do(WaveFormeToAppend)
		if(AddPoint(StartTimeToAppend + LePoint.x * ScalingFactorTime, LePoint.y * ScaleOrdonnee, true))
			nbrAdded++;
	}
return nbrAdded;
}

void CWaveForme::SortListePoints(int* pIndexPointActualise /* = NULL */)
{
	unsigned int nPWf = GetnPoints();
	if(nPWf < 2)
		return;
	// on fait autant de fois qu'il y a eu un switch
	bool IlYAEuUnSwitch = true;//init
	while(IlYAEuUnSwitch)
	{
		IlYAEuUnSwitch = false;
		// on balaye la liste en recherche de deux Point a switcher en comparant indexP à indexP + 1
		for(unsigned int indexP = 0; indexP < nPWf - 1; indexP++)
		{
			POSITION Pos1 = GetPointPosList(indexP);
			POSITION Pos2 = GetPointPosList(indexP + 1);
			CPointDbl point1 = m_Liste_Points.GetAt(Pos1);
			CPointDbl point2 = m_Liste_Points.GetAt(Pos2);
			if(point1.x > point2.x)
			{
				// On switch en affectant les valeurs de maniere croisées
				m_Liste_Points.SetAt(Pos1, point2);
				m_Liste_Points.SetAt(Pos2, point1);
				IlYAEuUnSwitch = true;
				// On Actualise pIndexPointActualise
				if(pIndexPointActualise)
				{
					if(*pIndexPointActualise == indexP)
						(*pIndexPointActualise) += 1;
					else if(*pIndexPointActualise == indexP + 1)
						(*pIndexPointActualise) -= 1;
				}
			}
		}
	}
return;
}

void CWaveForme::RemovePoint(int indexP)
{
	POSITION PosRem = GetPointPosList(indexP);
	m_Liste_Points.RemoveAt(PosRem);
	m_nPoints--;
}

void CWaveForme::RemovePoint(CPointDbl PointAEnlever)
{
	POSITION PosRem = m_Liste_Points.Find(PointAEnlever);
	m_Liste_Points.RemoveAt(PosRem);
	m_nPoints--;
}

CWaveForme* CWaveForme::SplitReturn_new_SecondePart(double AtProportionAbscisse)
{
	SortListePoints();
	if(this == NULL)
		return NULL;
	CWaveForme* pWfDeuxiemePartie = NULL;
	if(AtProportionAbscisse <= 0.0)
	{	// la deuxieme partie devient cette Wf
//		pWfDeuxiemePartie = new CWaveForme(*this);
//		// la premiere est réduite à NULL
//		delete this;
		return pWfDeuxiemePartie;
	}
	if(AtProportionAbscisse >= 1.0)
	{	// l'inverse
		return NULL;
	}
	if(GetnPoints() > 3)
	{
		int sfsdf = 0;
	}
	// dans les autre cas :
	// on ajoute un point à l'endroit de la coupe, si besoin est:
	double InitialLength = GetLength();
	double AbscisseCut = InitialLength * AtProportionAbscisse + GetPoint(0).x;
	int indexPointBefore = Find_PreceedingPoint(AbscisseCut);
	int indexPointAfter = Find_FolowingPoint(AbscisseCut);
	// if we are on a point :
	bool WeAreOnAPoint = false;
	if(indexPointBefore == indexPointAfter)
		WeAreOnAPoint = true;
	else // sinon, on ajoute un point là
	{
		AddPoint(AbscisseCut, InterpolateLinear(AbscisseCut));
		indexPointBefore++;
	}
	SortListePoints();
	
	pWfDeuxiemePartie = new CWaveForme(*this);
	// on va enlever les points en trop de la premiere partie: les points au dela de celui sur lequel on se trouve
	int nPoint = GetnPoints();
	for(int i = indexPointBefore + 1; i < nPoint; i++)
		RemovePoint(-1);
	// on va enlever les points en trop de la deuxieme partie: les points en deca de celui sur lequel on se trouve
	for(int i = 0; i < indexPointAfter; i++)
		pWfDeuxiemePartie->RemovePoint(0);
	// Ottons l'offset sur tous les points de la deuxième partie !
	CPointDbl FirstPoint = pWfDeuxiemePartie->GetPoint(0);
	for(int i = 0; i < pWfDeuxiemePartie->GetnPoints(); i++)
		pWfDeuxiemePartie->MovePoint(i, pWfDeuxiemePartie->GetPoint(i).x - FirstPoint.x, pWfDeuxiemePartie->GetPoint(i).y);

return pWfDeuxiemePartie;
}

void CWaveForme::MovePoint(int indexP, double NewAbscisse, double NewOrdonnee)
{
	POSITION PosCh = GetPointPosList(indexP);
	m_Liste_Points.SetAt(PosCh, CPointDbl(NewAbscisse, NewOrdonnee));
}

POSITION CWaveForme::GetPointPosList(int indexPoint)
{	
	if(indexPoint < 0)	
		indexPoint += GetnPoints();
	POSITION LaPos = m_Liste_Points.FindIndex(indexPoint);
	if(LaPos == NULL)	
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "GetPointPosList", "Position non trouvée: NULL !", 1);
	return LaPos;	
}

CPointDbl CWaveForme::GetPoint(int indexP)
{	
	if(this == NULL)
		return CPointDbl(0, 0);
	POSITION LaPos = GetPointPosList(indexP);
	if(LaPos == NULL) 
		return CPointDbl(0, 0);
	return m_Liste_Points.GetAt(LaPos);
}

double CWaveForme::InterpolateLinear(double Abscisse)
{
	// trouvons le dernier CPointDbl de la list qui vérifie CPointDbl.x < Abscisse
	unsigned int iemeP = 0;
	// prenons le dernier point précédant strictement Abscisse
	// si on est au dela de la Wf, on prend la valeur du dernier point
	if(Abscisse >= GetPoint(-1).x)
		iemeP = -2;
	else
		while(GetPoint(iemeP+1).x < Abscisse)
			iemeP++;
    
    // entre ce point et le suivant, faisont une interpolation linéaire
	CPointDbl& point1 = GetPoint(iemeP);
	CPointDbl& point2 = GetPoint(iemeP + 1);
	time1 = point1.x;		
	val1  = point1.y;
	time2 = point2.x;	
	val2  = point2.y;    
	return val1 + (val2 - val1) * (Abscisse - time1) / (time2 - time1);
}

void CWaveForme::InterpolateLinearFillNiMatrix(CNiReal64Matrix & LaMatrix, double timeStep, int SurQuelleLigne, int ApartirDeQuelEchantillon /* = 0 */)
{
	int nEchantillon = int(m_Liste_Points.GetTail().x / timeStep + 0.5);
	if(GetnPoints() < 2 || LaMatrix.GetRows() <= SurQuelleLigne || LaMatrix.GetCols() < ApartirDeQuelEchantillon + nEchantillon)
	{
		AfxMessageBox("InterpolateLinearFillNiMatrix:\r\nImpossible", MB_ICONERROR);
		return;
	}
	CPointDbl PGauche;
	CPointDbl PDroit;
	double FacteurLin = 0;
	double OffsetLin = 0;
	double precision = timeStep / 2;
	double LaValeur = 0;

	double CurrentTime = 0;
	POSITION pos = m_Liste_Points.GetHeadPosition();
	PDroit = m_Liste_Points.GetHead();

	while(pos != NULL)
	{
		PGauche = PDroit;
		PDroit = m_Liste_Points.GetNext(pos);
		FacteurLin = (PDroit.y - PGauche.y) / ((PDroit.x - timeStep) - PGauche.x);
		OffsetLin = PGauche.y - PGauche.x * FacteurLin;

		while(CurrentTime + precision < PDroit.x)
		{
			LaValeur = OffsetLin + CurrentTime * FacteurLin;
			if(LaValeur > 10)
			{
			LaValeur = 10;
			}
			if(LaValeur < -10)
			{
			LaValeur = -10;
			}
//			double oldVal = LaMatrix(SurQuelleLigne, ApartirDeQuelEchantillon);
			LaMatrix(SurQuelleLigne, ApartirDeQuelEchantillon) = LaValeur;
//			oldVal = LaMatrix(SurQuelleLigne, ApartirDeQuelEchantillon) + 0* oldVal;
			ApartirDeQuelEchantillon++;
			CurrentTime += timeStep;
		}
	}
}

void CWaveForme::DessineWF(CDC& dc, CRect Circonscrit, int OptionsDessin /* = ... */, int IndexColor /* = -1 */, CPointDbl Rect_MinMax /* = CPointDbl(-10000,10000) */)
{
	// l'interieur du rectangle, définie comme de ca gauche, a sa droite-1, et de haut en bas-1
	CRect Interieur = Circonscrit; 
	Interieur.DeflateRect(0,0,1,1);
	// La couleur en fond
	if(OptionsDessin & (WF_FOND_GRISEE + WF_FONDGAUCHE_GRISEE + WF_FONDMILIEU_GRISEE + WF_FONDDROITE_GRISEE))
	{
		CRect InterieurGauche = Interieur; 
		CRect InterieurMilieu = Interieur; 
		CRect InterieurDroit = Interieur;
		InterieurGauche.DeflateRect(0, 0, (2 * Interieur.Width()) / 3, 0);
		InterieurMilieu.DeflateRect(Interieur.Width() / 3, 0, Interieur.Width() / 3, 0);
		InterieurDroit.DeflateRect((2 * Interieur.Width()) / 3, 0, 0, 0);
		CBrush brusssh(wfGrisee);
		if(OptionsDessin & WF_FONDGAUCHE_GRISEE)
			dc.FillRect(InterieurGauche, &brusssh);
		if(OptionsDessin & WF_FONDMILIEU_GRISEE)
			dc.FillRect(InterieurMilieu, &brusssh);
		if(OptionsDessin & WF_FONDDROITE_GRISEE)
			dc.FillRect(InterieurDroit, &brusssh);
	}
	// Les lignes en bas et a droite
	CPen ligneFine(PS_SOLID, 1, wfNoir);
	CPen ligneGrisee(PS_SOLID, 1, wfGrisee);
	dc.SelectObject(ligneFine);	
	// si on dessine le cadre en noir
	if(OptionsDessin & WF_DESSINNE_CADRE)
	{
		dc.MoveTo(Circonscrit.left - 1	, Circonscrit.bottom - 1	);
		dc.LineTo(Circonscrit.right		, Circonscrit.bottom - 1	);
		dc.LineTo(Circonscrit.right		, Circonscrit.top			);
		dc.LineTo(Circonscrit.left - 1	, Circonscrit.top			);
		dc.LineTo(Circonscrit.left - 1	, Circonscrit.bottom - 1	);
	}
	else
	{
		// la ligne du bas de la cellule, noire
		dc.MoveTo(Circonscrit.right , Circonscrit.bottom - 1);
		dc.LineTo(Circonscrit.left - 1	, Circonscrit.bottom - 1);
		// la ligne de gauche
		dc.SelectObject(ligneFine);	
		if(OptionsDessin & WF_TRAITGAUCHE_GRISEE)
			dc.SelectObject(ligneGrisee);
		if(OptionsDessin & (WF_TRAITGAUCHE_GRISEE + WF_TRAITGAUCHE_VISIBLE))
		{
			dc.MoveTo(Circonscrit.TopLeft());
			dc.LineTo(Circonscrit.left 	, Circonscrit.bottom - 1);
		}
	}

	// puis, les lignes de la WF
	CPen Pen(PS_SOLID, 2,  IndexColor == -1 ? (wfNoir)
						 : IndexColor%6==0? (wfRouge )
						 : IndexColor%6==1? (wfVert )
						 : IndexColor%6==2? (wfBleu )
						 : IndexColor%6==3? (wfJaune )
						 : IndexColor%6==4? (wfBleuClaire	)
						 : (wfViolet )	);
	dc.SelectObject( Pen );	

	CPoint UpLeft = Interieur.TopLeft();

	double WfLength = GetLength();
	if(true)
	{
		unsigned int np = GetnPoints();
		unsigned int TotalPoints = np;
		np--;
		while( np )
		{
			// un segment de la Wf !
			double SegFromY = GetPoint(np-1).y;
			double SegToY	= GetPoint(np).y;
			double SegDeniv = abs(SegToY - SegFromY);
			double SegFromX = GetPoint(np-1).x;
			double SegToX	= GetPoint(np).x;
			double SegLength = (SegToX - SegFromX);
			// position Théorique des bouts du segment: théorique dans le sens ou si ca dépasse, on va modifier les valeur
			CPoint FromTh(UpLeft + CPoint( /*X*/  (long)(Interieur.Width() * SegFromX  / WfLength)
										 /*Y*/, (   (Rect_MinMax.y, (long)((double)Interieur.Height() * (1.0 - (SegFromY - Rect_MinMax.x) / (Rect_MinMax.y - Rect_MinMax.x))))) ));
			CPoint   ToTh(UpLeft + CPoint( /*X*/  (long)(Interieur.Width() * SegToX  / WfLength)
										 /*Y*/, (   (Rect_MinMax.y, (long)((double)Interieur.Height() * (1.0 - (SegToY	- Rect_MinMax.x) / (Rect_MinMax.y - Rect_MinMax.x))))) ));
			
			// Si on dépasse de Rect_MinMax, on peut arréter le trait !
			if(SegFromY > Rect_MinMax.y && SegToY <= Rect_MinMax.y)
			{	SegFromX += (SegToX - SegFromX) * abs(SegFromY - Rect_MinMax.y) / abs(SegToY - SegFromY); 
				SegFromY = Rect_MinMax.y;	}
			if(SegFromY <= Rect_MinMax.y && SegToY > Rect_MinMax.y)
			{	SegToX -= (SegToX - SegFromX) * abs(Rect_MinMax.y - SegToY) / abs(SegToY - SegFromY); 
				SegToY = Rect_MinMax.y;	}
			if(SegFromY < Rect_MinMax.x && SegToY >= Rect_MinMax.x)
			{	SegFromX += (SegToX - SegFromX) * abs(Rect_MinMax.x - SegFromY) / abs(SegToY - SegFromY); 
				SegFromY = Rect_MinMax.x;	}
			if(SegFromY >= Rect_MinMax.x && SegToY < Rect_MinMax.x)
			{	SegToX -= (SegToX - SegFromX) * abs(Rect_MinMax.x - SegToY) / abs(SegToY - SegFromY); 
				SegToY = Rect_MinMax.x;	}

			CPoint From(UpLeft + CPoint( /*X*/  (long)(Interieur.Width() * SegFromX  / WfLength)
										 /*Y*/, (   (Rect_MinMax.y, (long)((double)Interieur.Height() * (1.0 - (SegFromY - Rect_MinMax.x) / (Rect_MinMax.y - Rect_MinMax.x))))) ));
			CPoint   To(UpLeft + CPoint( /*X*/  (long)(Interieur.Width() * SegToX  / WfLength)
										 /*Y*/, (   (Rect_MinMax.y, (long)((double)Interieur.Height() * (1.0 - (SegToY	- Rect_MinMax.x) / (Rect_MinMax.y - Rect_MinMax.x))))) ));
			dc.MoveTo(From);
			// si le trait dépasse tout le temps, on ne trace pas le trait
			if((SegFromY > Rect_MinMax.y && SegToY > Rect_MinMax.y)
			 ||(SegFromY < Rect_MinMax.x && SegToY < Rect_MinMax.x))
			{	;	}
			else
				dc.LineTo(To);

			// Le Texte des valeur numerique 
			dc.SetTextColor(wfNoir);

			// La fonte
			CFont font,*pOldfont;
			LOGFONT* pLF;
			pLF = (LOGFONT*)calloc(1,sizeof(LOGFONT));
			pLF->lfHeight = WF_FONT_SIZE;
			font.CreateFontIndirect(pLF);
			pOldfont = dc.SelectObject(&font);

			// Position du texte qui donne la valeur au début d'un segment
			CString texte;
			texte.Format(" %.4g", GetPoint(np-1).y);
			texte.Replace("e+00", "e");
			texte.Replace("e-00", "e-");
			CRect rect = CRect( FromTh + CPoint(1, -WF_FONT_SIZE/2), CSize(Interieur.Width(), WF_FONT_SIZE));
			// si le rectangle dépasse, on le réajuste dans la case
			if(rect.top < Interieur.top + 1)	rect += CPoint(0, Interieur.top + 1 - rect.top);
			if(rect.left < Interieur.left)		rect += CPoint(0, Interieur.left - rect.left);
			if(rect.bottom > Interieur.bottom)	rect += CPoint(0, Interieur.bottom - rect.bottom);

			if(OptionsDessin & (WF_VALGAUCHE_VISIBLE + WF_VALGAUCHE_GRISEE) && (TotalPoints <= 3 || np == 1))
			{
				if(OptionsDessin & WF_VALGAUCHE_GRISEE)
					dc.SetTextColor(wfGrisee);
				else
					dc.SetTextColor(wfNoir);
				dc.DrawText(texte,&rect,DT_LEFT);
			}
			// on n'écrite pas la meme valeur a nouveau
			// ou, on ecrit pas les valeurs si il y en a trop
			if(    (SegToY != SegFromY && TotalPoints <= 3 )
				|| (np == TotalPoints-1 && TotalPoints > 3 )   )
			{
				texte.Format(_T(" %.4g"), GetPoint(np).y );	
				texte.Replace("e+00", "e");
				texte.Replace("e-00", "e-");
				rect = CRect(ToTh + CPoint(-Interieur.Width()-1, -WF_FONT_SIZE/2), CSize(Interieur.Width(), WF_FONT_SIZE));
				// idem
				if(rect.top < Interieur.top + 1)
					rect += CPoint(0, Interieur.top + 1 - rect.top);
				if(rect.right > Interieur.right)
					rect += CPoint(0, Interieur.right - rect.right);
				if(rect.bottom > Interieur.bottom)
					rect += CPoint(0, Interieur.bottom - rect.bottom);
				if(OptionsDessin & (WF_VALDROITE_VISIBLE + WF_VALDROITE_GRISEE))
				{
					if(OptionsDessin & WF_VALDROITE_GRISEE)
						dc.SetTextColor(wfGrisee);
					else
						dc.SetTextColor(wfNoir);
					dc.DrawText(texte,&rect,DT_RIGHT);
				}
			}
			dc.SelectObject(pOldfont);
			np--;
			free(pLF);
		}
	}
}

void CWaveForme::SetGraphically()
{
	if(this == NULL)
		return;
    CDialogWaveForm WFDlg(this);
	if(WFDlg.DoModal() != IDOK) 
		return;

	SaveToFileWf("LaWfDeMainDialog.Wf");
}
CString CWaveForme::GetFormula_FromFile(CString filename)
{
	CString Formule = "";
	CTextFile pourlire;
	if(!pourlire.ReadTextFile(filename, Formule))
	{;}//		AfxMessageBox(CString("GetFormula_FromFile") + pourlire.GetErrorMessage(), MB_ICONERROR);
	else if(Formule == "")
	{;}//		AfxMessageBox(filename + CString(" : No formula in the file") + CString("\r\n Should be of the type\r\n \" x + 3*cos(x^2)/abs(x)\" \r\nie:  fonction of \"x\" ") , MB_ICONERROR);
	// si on trouve #, on commente tout le reste de la ligne
	int posComment = 0;
	int posEndLigne = 0;
	Formule += "\r\n";
	while((posComment = Formule.Find("#", 0)) != -1)
	{
		posEndLigne = Formule.Find("\r", posComment + 1);
		Formule.Delete(posComment, posEndLigne - posComment);
	}
	// on enlève tout les alaligne
	Formule.Replace("\r", " ");
	Formule.Remove('\n');
	// On enlève les espaces du début et de la fin
	Formule.Trim(" ");
	return Formule;
}
//void CWaveForme::SetFromFunction_ofX(double FromX, double ToX, int NbrPoints, double Fy_ofx(double x), double WarpFirstPointTo /* = 0.0 */)
//{
//	if(this == NULL || NbrPoints < 2 || FromX >= ToX)
//		return;
//	double intervalPoints = (ToX - FromX) / double(NbrPoints - 1);
//	ClearPointList();
//	double xNew = FromX;
//	double yNew = 0;
//	// le warpage de la premiere valeur par multiplication du tout
//	double MultiplicateurWarp = 1.0;
//	double FirstY = Fy_ofx(xNew);
//	if(WarpFirstPointTo != 0.0 && FirstY != 0.0)
//		MultiplicateurWarp = WarpFirstPointTo / FirstY;
//
//	for(int indexPoint = 0; indexPoint < NbrPoints; indexPoint++)
//	{	// pour chaque nouveau point, on prend la fonction yNew = f(xNew)
//		yNew = Fy_ofx(xNew);
//		yNew *= MultiplicateurWarp;
//		AddPoint(xNew, yNew);
//		// puis on incrémente x_here pour l'iteration suivante
//		xNew += intervalPoints;
//	}
//}
//void CWaveForme::SetFromFunction_ofX(double FromX, double ToX, int NbrPoints, CString Formula_Fofx, double WarpFirstPointTo /* = 0.0 */)
//{	
//	// tout d'abord, on check si Formula_Fofx n'est pas en fait une adresse de fichier qui contiendrait une formule
//	if(Formula_Fofx.Find(":\\") != -1) // on trouve toujours :\ dans une adresse de fichier, et jamis dans une formule!
//	{
//		Formula_Fofx = GetFormula_FromFile(Formula_Fofx);
//		if(Formula_Fofx == "")
//			return;
//	}
//
//	if(this == NULL || NbrPoints < 2 || FromX >= ToX)
//		return;
//	double intervalPoints = (ToX - FromX) / double(NbrPoints - 1);
//	ClearPointList();
//	double xNew = FromX;
//	double yNew = 0;
//	double y0New = WarpFirstPointTo;
//	// un parser : prend une string formula, et en fait une fonction
//	mu::Parser parser;
//	// on definit xNew comme étant la variable "x"
//	parser.DefineVar("x", &xNew);  
//	parser.DefineVar("y0", &y0New);  
//	// puis la formule
//	std::string formule = Formula_Fofx;
//	parser.SetExpr(formule);
//	// le warpage de la premiere valeur par multiplication du tout
//	double MultiplicateurWarp = 1.0;
//	double FirstY = 0.0;
//	try{
//		FirstY = parser.Eval();
//	}catch(Parser::exception_type &e){}
////	if(WarpFirstPointTo != 0.0 && FirstY != 0.0)
////		MultiplicateurWarp = WarpFirstPointTo / FirstY;
//
//	for(int indexPoint = 0; indexPoint < NbrPoints; indexPoint++)
//	{	// pour chaque nouveau point, on prend la fonction yNew = f(xNew)
//		try
//		{
//			yNew = parser.Eval();
//			yNew *= MultiplicateurWarp;
//		}
//		catch(Parser::exception_type &e)
//		{
//			ClearPointList();
//			AddPoint(FromX, yNew);
//			AddPoint(ToX, yNew);
//			CString strErr = CString(e.GetMsg().c_str()) + CString("\r\n") + CString(e.GetExpr().c_str() + CString("\r\n Should be of the type\r\n \" x + 3*cos(x^2)/abs(x)\" \r\nie:  fonction of \"x\" ") );
//			AfxMessageBox(strErr, MB_ICONERROR);
//			return;
//		}
//		AddPoint(xNew, yNew);
//		// puis on incrémente x_here pour l'iteration suivante
//		xNew += intervalPoints;
//	}
//}
//
//void CWaveForme::SetFromFunction_ofOtherWfY(CWaveForme* pOtherWf, int NbrPoints, double Fy_ofy(double y_other))
//{
//	if(pOtherWf == NULL || this == NULL || NbrPoints < 2)
//		return;
//	double intervalPoints = pOtherWf->GetLength() / double(NbrPoints - 1);
//	ClearPointList();
//	double x_other = pOtherWf->GetPoint(0).x;
//	double y_other = 0;
//	double yNew = 0;
//	for(int indexPoint = 0; indexPoint < NbrPoints; indexPoint++)
//	{	// pour chaque nouveau point, on prend l'interpolation lineaire de pOtherWf en x_here
//		y_other = pOtherWf->InterpolateLinear(x_other);
//		yNew = Fy_ofy(y_other);
//		AddPoint(x_other, yNew);
//		// puis on incrémente x_here pour l'iteration suivante
//		x_other += intervalPoints;
//	}
//}
//void CWaveForme::SetFromFunction_ofOtherWfY(CWaveForme* pOtherWf, int NbrPoints, CString Formula_Fofx)
//{
//	// tout d'abord, on check si Formula_Fofx n'est pas en fait une adresse de fichier qui contiendrait une formule
//	if(Formula_Fofx.Find(":\\") != -1) // on trouve toujours :\ dans une adresse de fichier, et jamis dans une formule!
//	{
//		Formula_Fofx = GetFormula_FromFile(Formula_Fofx);
//		if(Formula_Fofx == "")
//			return;
//	}
//
//	if(pOtherWf == NULL || this == NULL || NbrPoints < 2)
//		return;
//	double intervalPoints = pOtherWf->GetLength() / double(NbrPoints - 1);
//	ClearPointList();
//	double x_other = pOtherWf->GetPoint(0).x;
//	double y_other = 0;
//	double yNew = 0;
//	// un parser : prend une string formula, et en fait une fonction
//	mu::Parser parser;
//	// on definit xNew comme étant la variable "x"
//	parser.DefineVar("x", &y_other);  
//	// puis la formule
//	std::string formule = Formula_Fofx;
//	parser.SetExpr(formule);
//	for(int indexPoint = 0; indexPoint < NbrPoints; indexPoint++)
//	{	// pour chaque nouveau point, on prend l'interpolation lineaire de pOtherWf en x_here
//		y_other = pOtherWf->InterpolateLinear(x_other);
//		try
//		{
//			yNew = parser.Eval();
//		}
//		catch(Parser::exception_type &e)
//		{
//			ClearPointList();
//			AddPoint(pOtherWf->GetPoint(0).x, yNew);
//			AddPoint(pOtherWf->GetPoint(-1).x, yNew);
//			CString strErr = CString(e.GetMsg().c_str()) + CString("\r\n") + CString(e.GetExpr().c_str() + CString("\r\n Should be of the type\r\n \" x + 3*cos(x^2)/abs(x)\" \r\nie:  fonction of \"x\" ") );
//			AfxMessageBox(strErr, MB_ICONERROR);
//			return;
//		}
//		AddPoint(x_other, yNew);
//		// puis on incrémente x_here pour l'iteration suivante
//		x_other += intervalPoints;
//	}
//}

void CWaveForme::MakeCParametreWf()
{
	m_SaveWf.ClearAllCParametre();
	new CParametre(&m_SaveWf, Entier, "nPoints", "nPoints", Auto, &m_nPoints, "");
	//detruisons les m_ConstrWf_X et Y
	if(m_ConstrWf_X)
	{	delete[] m_ConstrWf_X;	m_ConstrWf_X = NULL;}
	if(m_ConstrWf_Y)
	{	delete[] m_ConstrWf_Y;	m_ConstrWf_Y = NULL;}
	// puis, on en fait de nouveau
	m_ConstrWf_X = new double[m_nPoints];
	m_ConstrWf_Y = new double[m_nPoints];
	// et de nouveau CParametre
	CString str("");
	for(int iemeP = 1; iemeP <= m_nPoints; iemeP++)
	{
		str.Format("P%d_X", iemeP);
		new CParametre(&m_SaveWf, Double, str, str, Auto, &(m_ConstrWf_X[iemeP - 1]), "0", CanParticipateVariation | CanParticipateGeneticAlgo);
		str.Format("P%d_Y", iemeP);
		new CParametre(&m_SaveWf, Double, str, str, Auto, &(m_ConstrWf_Y[iemeP - 1]), "0", CanParticipateVariation | CanParticipateGeneticAlgo);
	}
}
void CWaveForme::LoadFromFileWf(CString filename)
{
	m_SaveWf.ClearAllCParametre();
	// definition du parametre Nombre de points
	new CParametre(&m_SaveWf, Entier, "nPoints", "nPoints", Auto, &m_nPoints, "0");
	// puis on charge sa valeur
	m_SaveWf.SetAllSettings(filename, false);
	// si pas de points, bye
	if(m_nPoints == 0)
		return;
	// Maintenant qu'on a le nombre de points, on fait les CParametre
	MakeCParametreWf();
	// et on load a nouveau
	int NombreDePoint = m_nPoints;
	m_SaveWf.SetAllSettings(filename);
	// puis on fait la Wf en ajoutant les points
	ClearPointList();
	for(int indexeP = 0; indexeP < NombreDePoint; indexeP++)
	{
		AddPoint(m_ConstrWf_X[indexeP], m_ConstrWf_Y[indexeP]);
	}
	SortListePoints();
}
void CWaveForme::SaveToFileWf(CString filename)
{
	// on fait les CParametre
	MakeCParametreWf();
	// on attribue des valeurs
	for(int indexeP = 0; indexeP < m_nPoints; indexeP++)
	{
		m_ConstrWf_X[indexeP] = GetPoint(indexeP).x;
		m_ConstrWf_Y[indexeP] = GetPoint(indexeP).y;
	}
	// et on les sauve
	m_SaveWf.SaveSettings(filename);
}
CString CWaveForme::FindAllWaveFormesInAFolder(CString FolderName, CComboBox* pCombo)
{
	if(pCombo)
		pCombo->ResetContent();
	CString TheFiles("");
	// use a file find object to enumerate files
	CFileFind finder;
	FolderName.TrimRight("\\");
	if(FolderName != "")
		FolderName += "\\";

	// start looping
	BOOL bWorking = finder.FindFile(_T(FolderName + "*.Wf"));
	if(!bWorking)
		return TheFiles;
	bWorking = finder.FindNextFile();
	// 1ere ligne
	CString NewLigne;
	NewLigne.Format("%s", finder.GetFileName());
	TheFiles = NewLigne;
	if(pCombo)
		pCombo->AddString(NewLigne);
	// suite
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		NewLigne.Format("%s", finder.GetFileName());
		TheFiles += CString("\n") + NewLigne;
		if(pCombo)
			pCombo->AddString(NewLigne);
	}
return TheFiles;
}
