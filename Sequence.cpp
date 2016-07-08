#include "StdAfx.h"
#include "DialogSetValue.h"
#include ".\Sequence.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////        Sequence        ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
double Sequence::m_SampleRate = 10000;

CParametre* Sequence::m_ParamDefaultDigOut[NUMBER_DIG_OUTPUT] = {NULL};
CParametre* Sequence::m_ParamDefaultAnaOut[NUMBER_ANA_OUTPUT] = {NULL};

CListCtrl* Sequence::p_ListCtrlColonnes = NULL;
CListCtrl* Sequence::p_ListCtrlLignesDig = NULL;
CListCtrl* Sequence::p_ListCtrlLignesAna = NULL;
	
COLORREF Rouge =		(	RGB( 255,000,000 ));
COLORREF Vert =			(	RGB( 000,255,000 ));
COLORREF Bleu =			(	RGB( 000,000,255 ));
COLORREF Jaune =		(	RGB( 205,205,000 ));
COLORREF BleuClaire =	(	RGB( 000,255,255 ));
COLORREF Violet =		(	RGB( 255,000,255 ));
COLORREF Blanc =		(	RGB( 255,255,255 ));
COLORREF DigGrisee =	(	RGB( 135,135,135 ));	COLORREF DigLigneGrisee =		(	RGB( 205,205,205 ));
COLORREF DigNoir =		(	RGB(   0,  0,  0 ));	COLORREF DigGriseeNoir =		(	RGB( 185,185,185 ));
COLORREF DigRouge =		(	RGB( 255,  0,  0 ));	COLORREF DigGriseeRouge =		(	RGB( 185,  0,  0 ));
COLORREF DigVert =		(	RGB(   0,255,  0 ));	COLORREF DigGriseeVert =		(	RGB(   0,185,  0 ));
COLORREF DigBleu =		(	RGB(   0,  0,255 ));	COLORREF DigGriseeBleu =		(	RGB(   0,  0,185 ));
COLORREF DigJaune =		(	RGB( 255,255,  0 ));	COLORREF DigGriseeJaune =		(	RGB( 185,185,  0 ));
COLORREF DigBleuClaire =(	RGB(   0,255,255 ));	COLORREF DigGriseeBleuClaire =	(	RGB(   0,185,185 ));
COLORREF DigViolet =	(	RGB( 255,  0,255 ));	COLORREF DigGriseeViolet =		(	RGB( 185,  0,185 ));
COLORREF DigBlanc =		(	RGB( 255,255,255 ));	COLORREF DigGriseeBlanc =		(	RGB( 185,185,185 ));



int Sequence::m_ColWidth = 50; 
int Sequence::m_DigHeight = 12; 
int Sequence::m_AnaHeight = 35; 
int Sequence::m_EcartHoriz = 0;
int Sequence::m_EcartVert = 0;
CPoint Sequence::m_FirstDigTopLeft = CPoint(50, 20);
CPoint Sequence::m_FirstAnaTopLeft = CPoint(50, 0);
int Sequence::m_AnaMax_mV = 10000; 
int Sequence::m_AnaMin_mV = 0;
int Sequence::m_NbrDisplayDigChan = NUMBER_DIG_OUTPUT;
int Sequence::m_NbrDisplayAnaChan = NUMBER_ANA_OUTPUT;

Sequence::Sequence(void)
: m_SeqSave("Une Séquence", "Seq", false)
{
	// mise à NULL de tous les m_ParamDefault (si la première est à NULL)
	if(m_ParamDefaultDigOut[0] == NULL)
		for(int i = 0; i < NUMBER_DIG_OUTPUT; i++)
			m_ParamDefaultDigOut[i] = NULL;
	if(m_ParamDefaultAnaOut[0] == NULL)
		for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
			m_ParamDefaultAnaOut[i] = NULL;
	m_NameSeq = DEFAULT_SEQ_NAME;
//	m_FileSave = DEFAULT_SEQ_FILE_NAME;
	m_nPeriodes = 0;
	m_nRepetition = 0;
	ClearColonneList();
	m_nColonnes = 0;
	p_DigBuffer = NULL;
	for(int i = 0; i < NUMBER_CARDS_DIG_OUTPUT; i++)
		m_DigBufferNiMatrix[i].SetSize(1,1);
	for(unsigned char indexChan = 0; indexChan < NUMBER_ANA_OUTPUT; indexChan++)
		p_AnaBuffer[indexChan] = NULL;

	m_Constr_Name					= NULL;
	m_Constr_Duration				= NULL;
	m_Constr_Duration_µs			= NULL;
	m_Constr_Dig32bit				= NULL;
	m_Constr_ColStartingGroupe		= NULL;
	m_Constr_Dig32ContiWithPrevious = NULL;
	for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)
	{
		m_Constr_AnaDebut[j]			= NULL;
		m_Constr_AnaFin[j]				= NULL;
		m_Constr_AnaContiWithPrevious[j]= NULL;
	}
}
Sequence::Sequence(Sequence &UneSequence)
: m_SeqSave("", "Seq", false)
{
	// mise à NULL de tous les m_ParamDefaultDigOut (si la première est à NULL)
	if(m_ParamDefaultDigOut[0] == NULL)
		for(int i = 0; i < NUMBER_DIG_OUTPUT; i++)
			m_ParamDefaultDigOut[i] = NULL;
	if(m_ParamDefaultAnaOut[0] == NULL)
		for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
			m_ParamDefaultAnaOut[i] = NULL;
	m_NameSeq = UneSequence.m_NameSeq;
//	m_FileSave = UneSequence.m_FileSave;
	m_nPeriodes = 0;
	m_nRepetition = UneSequence.m_nRepetition;
	m_nColonnes = 0;
	p_DigBuffer = NULL;
	for(unsigned char indexChan = 0; indexChan < NUMBER_ANA_OUTPUT; indexChan++)
		p_AnaBuffer[indexChan] = NULL;
	m_Constr_Name					= NULL;
	m_Constr_Duration				= NULL;
	m_Constr_Duration_µs			= NULL;
	m_Constr_Dig32bit				= NULL;
	m_Constr_ColStartingGroupe		= NULL;
	m_Constr_Dig32ContiWithPrevious = NULL;
	for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)
	{
		m_Constr_AnaDebut[j]			= NULL;
		m_Constr_AnaFin[j]				= NULL;
		m_Constr_AnaContiWithPrevious[j]= NULL;
	}
	for(int iemeCol = 1; iemeCol <= UneSequence.m_nColonnes; iemeCol++)
	{
		Colonne* pCol = UneSequence.GetColonne(iemeCol);
		Colonne* pNewCol = InsertNewColonne_CopiedFromCol( AfterLastColonne, pCol);
		m_nPeriodes += pNewCol->GetActiveDuree();
	}

	for(int i = 0; i <= m_nColonnes; i++)
	{	m_Constr_Name[i]						= (i == 0?"":UneSequence.m_Constr_Name[i]					);
		m_Constr_Duration[i]					= (i == 0?0:UneSequence.m_Constr_Duration[i]				);
		m_Constr_Duration_µs[i]					= (i == 0?0:UneSequence.m_Constr_Duration_µs[i]				);
		m_Constr_Dig32bit[i]					= (i == 0?0:UneSequence.m_Constr_Dig32bit[i]				);
		m_Constr_ColStartingGroupe[i]			= (i == 0?0:UneSequence.m_Constr_ColStartingGroupe[i]		);
		m_Constr_Dig32ContiWithPrevious[i]		= (i == 0?0:UneSequence.m_Constr_Dig32ContiWithPrevious[i]	);
		for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)
		{	m_Constr_AnaDebut[j][i]				= (i == 0?0:UneSequence.m_Constr_AnaDebut[j][i]				);
			m_Constr_AnaFin[j][i]				= (i == 0?0:UneSequence.m_Constr_AnaFin[j][i]				);	
			m_Constr_AnaContiWithPrevious[j][i]	= (i == 0?0:UneSequence.m_Constr_AnaContiWithPrevious[j][i]	);
	}	}
									  
	Make_Constr_Param();
//	MakeCParametrantFrom_Sequence_AndConstrParam();
}

Sequence::~Sequence(void)
{
	if(m_Constr_Name)								delete[] m_Constr_Name;
	if(m_Constr_Duration)							delete[] m_Constr_Duration;
	if(m_Constr_Duration_µs)						delete[] m_Constr_Duration_µs;
	if(m_Constr_Dig32bit)							delete[] m_Constr_Dig32bit;
	if(m_Constr_ColStartingGroupe)					delete[] m_Constr_ColStartingGroupe;
	if(m_Constr_Dig32ContiWithPrevious)				delete[] m_Constr_Dig32ContiWithPrevious;
	for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)	
	{	if(m_Constr_AnaDebut[j])					delete[] (m_Constr_AnaDebut[j]);
       	if(m_Constr_AnaFin[j])						delete[] (m_Constr_AnaFin[j]);
		if(m_Constr_AnaContiWithPrevious[j])		delete[] (m_Constr_AnaContiWithPrevious[j]);} 
	ClearDigBuffer();
	ClearAnaBuffer();
	ClearColonneList();
}
void Sequence::operator =(Sequence& UneSequence)
{
	ClearDigBuffer();
	ClearAnaBuffer();
	ClearColonneList();
	m_NameSeq = UneSequence.m_NameSeq;
//	m_FileSave = UneSequence.m_FileSave;
	m_nPeriodes = 0;
	m_nRepetition = UneSequence.m_nRepetition;
	m_nColonnes = 0;
	for(int iemeCol = 1; iemeCol <= UneSequence.m_nColonnes; iemeCol++)
	{
		Colonne* pCol = UneSequence.GetColonne(iemeCol);
		Colonne* pNewCol = InsertNewColonne_CopiedFromCol( AfterLastColonne, pCol);
		m_nPeriodes += pNewCol->GetActiveDuree();
	}
	p_DigBuffer = NULL;
	for(unsigned char indexChan = 0; indexChan < NUMBER_ANA_OUTPUT; indexChan++)
		p_AnaBuffer[indexChan] = NULL;

	if(m_nColonnes == 0)
		return;

	for(int i = 0; i <= m_nColonnes; i++)
	{	m_Constr_Name[i]						= (i == 0?"":UneSequence.m_Constr_Name[i]					);
		m_Constr_Duration[i]					= (i == 0?0:UneSequence.m_Constr_Duration[i]				);
		m_Constr_Duration_µs[i]					= (i == 0?0:UneSequence.m_Constr_Duration_µs[i]				);
		m_Constr_Dig32bit[i]					= (i == 0?0:UneSequence.m_Constr_Dig32bit[i]				);
		m_Constr_ColStartingGroupe[i]			= (i == 0?0:UneSequence.m_Constr_ColStartingGroupe[i]		);
		m_Constr_Dig32ContiWithPrevious[i]		= (i == 0?0:UneSequence.m_Constr_Dig32ContiWithPrevious[i]	);
		for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)
		{	m_Constr_AnaDebut[j][i]				= (i == 0?0:UneSequence.m_Constr_AnaDebut[j][i]				);
			m_Constr_AnaFin[j][i]				= (i == 0?0:UneSequence.m_Constr_AnaFin[j][i]				);	
			m_Constr_AnaContiWithPrevious[j][i]	= (i == 0?0:UneSequence.m_Constr_AnaContiWithPrevious[j][i]	);
	}	}

	Make_Constr_Param();
	MakeCParametrantFrom_Sequence_AndConstrParam();
}
void Sequence::LoadNewSequenceFromFile(CString filename /* = "" */)
{
	// on oublie l'ancienne sequence 
	Sequence UneSeqVierge;//???????????????????????????????????????????
	operator =(UneSeqVierge);
	// On sort d'abord le nombre de colonne. Si filename == "", Dialog, et la Sring correspondante est alors mise dans filename
	int NumColonne = m_SeqSave.GetIntSettings(filename, "NumCol");
	// Ajoutons les colonnes une par une
	for(int iemeCol = 1; iemeCol <= NumColonne; iemeCol++)
	{
		InsertNewColonne(-1, "None", 1000);
		SetCol_32Digitals( LastColonne,   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
		SetCol_Analogs_Continue( LastColonne);
	}
	// fabrication des CParametres sensé etre trouvés par la suite dans le filename
	MakeCParametrantFrom_Sequence_AndConstrParam();
	// affectation des valeur a ces CParametres
	// on demande a ne pas etre mis au courant des Parametres non trouvés, car c'est normale, ils vont etre détruits dans MakeNotRedondant_Constr_param()
	m_SeqSave.SetAllSettings(filename, false);
	// Actualise les CParametrant
	MakeNotRedondant_Constr_param();
	ActualiseSequenceFromCParametrant();
}
void Sequence::SaveSequenceToFile(CString filename /* = "" */)
{
	m_SeqSave.SaveSettings(filename, "Seq");
}

unsigned int Sequence::CalculateNbrPeriodes()
{
	m_nPeriodes = 0;
	m_nColonnes = 0;
	POSITION pos = m_Liste_p_Colonnes.GetHeadPosition();
	while(pos)
	{
		m_nPeriodes += m_Liste_p_Colonnes.GetNext(pos)->GetActiveDuree();
		m_nColonnes++;
	}
return m_nPeriodes;
}

unsigned long* Sequence::GetDigBuffer()
{
	return p_DigBuffer;
}

double** Sequence::GetAnaBuffer()
{
return p_AnaBuffer;
}

void Sequence::ClearColonneList(bool DestroyAllWF /* = false */ )
{
	// un peu de netoyage !
	if(m_nColonnes)
	{
		while (!m_Liste_p_Colonnes.IsEmpty())
		{
			Colonne* pTheCol = m_Liste_p_Colonnes.GetHead();
			delete m_Liste_p_Colonnes.RemoveHead();
		}
	}
	// si bool DestroyAllWF
	// on détruit aussi les WF, qui ont bien été répertoriées dans CWaveForme::Liste_WaveFormes
	while (DestroyAllWF && !CWaveForme::Liste_WaveFormes.IsEmpty())
	{
		delete CWaveForme::Liste_WaveFormes.RemoveHead();
	}

	m_nPeriodes = 0;
	m_nColonnes = 0;
return;
}

Colonne* Sequence::InsertNewColonne_CopiedFromCol(int Where_0Debut_m1Fin, Colonne* pUneColCopie, CString NewNom /* = "" */, unsigned int UnNewNombrePeriode /* = 0 */)
{
	Colonne* pLaNouvelleColonne;
	CString LeNouveauNom = NewNom;
	// si on fournit une colonne a copier
	if(pUneColCopie)
	{
		LeNouveauNom = (NewNom == "")? pUneColCopie->m_NameCol : NewNom;
		pLaNouvelleColonne = new Colonne(*pUneColCopie, LeNouveauNom, UnNewNombrePeriode);
	}
	else
		pLaNouvelleColonne = new Colonne(LeNouveauNom, UnNewNombrePeriode);

	int where = Where_0Debut_m1Fin;
	// PS: m_nColonnes est 1 based, est where est 0 based, 
	// : ce qui fait que cela marche
	// de 0 à m_nColonnes, on insert la colonne avant where
	if(where >= 0 && where < m_nColonnes)
	{
		POSITION pos = m_Liste_p_Colonnes.FindIndex(where);
		if(pos == NULL)
			return NULL;
		m_Liste_p_Colonnes.InsertBefore(pos, pLaNouvelleColonne);
	}
	// si where = m_nColonnes, ou si il n'y a pas de colonne, on insert à la fin
	else if(where == m_nColonnes || m_nColonnes == 0)
	{
		m_Liste_p_Colonnes.AddTail(pLaNouvelleColonne);
	}
	// -1 --> à la fin, -2 avant la derniere, etc...
	else if(where < 0 && where >= -(int)m_nColonnes)
	{
		where += m_nColonnes;
		POSITION pos = m_Liste_p_Colonnes.FindIndex(where);
		if(pos == NULL)
			return NULL;
		m_Liste_p_Colonnes.InsertAfter(pos, pLaNouvelleColonne);
		where++;// juste pour que le parametre passé un peu plus bas
	}
	else
		return NULL;
	// on dit bien que c'est cette sequence qui possède la colonne
	pLaNouvelleColonne->m_pSequenceOwning = this;
	// actualise m_nColonne
	m_nColonnes++;
	// Ajoute une colonne au m_Constr
	AddColonne_Constr_Param(where);
return pLaNouvelleColonne;
}

Colonne* Sequence::InsertNewColonne(int Where_0Debut_m1Fin, CString UnNewNom, unsigned int UnNewNombrePeriode /* = 0 */)
{
	return InsertNewColonne_CopiedFromCol(Where_0Debut_m1Fin, NULL, UnNewNom, UnNewNombrePeriode);
}

//void Sequence::SetFromNowOn_RelationBetween_2Analogs(unsigned char indexAnaSlave, unsigned char indexAnaMaster, CString Formula_Fofx, int NbrPoints)
//{
//}

void Sequence::SplitColonne(int iemeCol, int NbrPeriodeFirstPart)
{
	if(iemeCol < 0 && iemeCol >= -(int)m_nColonnes)
		iemeCol += m_nColonnes + 1;

	Colonne* ColASpliter = GetColonne(iemeCol);
	if(ColASpliter == NULL)
		return;
	int nPeriodeColASpliter = ColASpliter->GetDuree();
	if(NbrPeriodeFirstPart >= nPeriodeColASpliter || NbrPeriodeFirstPart <= 0)
		return;

	Colonne* DeuxiemeColonne = InsertNewColonne_CopiedFromCol(iemeCol, ColASpliter);

	ColASpliter->Set_LimiteDroiteEstUnSplit(true);
	DeuxiemeColonne->Set_LimiteGaucheEstUnSplit(true);

	// Duree active de la colonne a spliter
	int ActiveDuree = GetColActiveDuration(iemeCol);
	// durées
	SetColonneDuration(iemeCol, NbrPeriodeFirstPart);
	SetColonneDuration(iemeCol + 1, nPeriodeColASpliter - NbrPeriodeFirstPart);
	// on ajuste aussi les duree active pour etre cohérent avec ce qu'on avait avant le splitting
	int activePremiere = ActivateColonne(iemeCol, ActiveDuree);
	if(activePremiere <= ActiveDuree)
		ActivateColonne(iemeCol + 1, ActiveDuree - activePremiere);
	// proportion
	double AtProportion = double(NbrPeriodeFirstPart ) / double(nPeriodeColASpliter);
	// pour chaque Analogique, on split
	for(int indexAna = 0; indexAna < NUMBER_ANA_OUTPUT; indexAna++)
	{
		CWaveForme* DeuxiemeWf = ColASpliter->m_AnaChannel[indexAna]->SplitReturn_new_SecondePart(AtProportion);
		if(DeuxiemeWf)
			DeuxiemeColonne->AssignWaveFormeToAnalog(indexAna, DeuxiemeWf);
		// précisons aussi pour le dessin par exemple, que la deuxième colonne est une SlopeContinueAGauche
		// c'est juste une info rajouté dans la séquence
		m_Constr_AnaContiWithPrevious[indexAna][iemeCol + 1] = SlopeContinueAGauche;
	}

return;
}

double Sequence::SplitSequence_FromEndOfCol_NbrPeriodeBeforeEndCol_SetDigitals(int iemeCol, int NbrPeriodeBefore, WhichDigVal)
{
	double ARetourner = -1.0;
	if(iemeCol < 0 && iemeCol >= -(int)m_nColonnes)
		iemeCol += m_nColonnes + 1;

	iemeCol++;

	// On va remonter à la colonne qu'il faut splitter en deux pour ajouter le ClicClac
	// on compte pour ca le nombre de periode active de chaque colonne avant
	int NumeroColSplit = iemeCol;
	int dureeActiveSomColPrec = 0;
	while((dureeActiveSomColPrec < NbrPeriodeBefore) && GetColonne(NumeroColSplit - 1))
	{
		NumeroColSplit--;
		GetColDuration(NumeroColSplit);
		GetColActiveDuration(NumeroColSplit);
		dureeActiveSomColPrec += GetColActiveDuration(NumeroColSplit);
	}
	// si on est revenu au debut de la sequence ...
	if((dureeActiveSomColPrec < NbrPeriodeBefore) && NumeroColSplit == 1)
		NumeroColSplit--;

	// si ca match un début de colonne, on ne fait rien
	if(dureeActiveSomColPrec == NbrPeriodeBefore)
	{
		NumeroColSplit--;
		ARetourner = double(NumeroColSplit);
	}
	// sinon, La colonne a spliter, on la coupe en deux
	else if(NbrPeriodeBefore && (dureeActiveSomColPrec > NbrPeriodeBefore))
	{
		iemeCol++;
		int dureeFirstPart = dureeActiveSomColPrec - NbrPeriodeBefore;
		int dureeColASpliter = GetColDuration(NumeroColSplit);
		SplitColonne(NumeroColSplit, dureeFirstPart);
		ARetourner = double(NumeroColSplit - 1) + double(dureeFirstPart) / double(dureeColASpliter);
	}

	// on assigne les sorties digitales depuis la deuxieme partie de la colonne créée, j'usqu'à la notre non incluse
	for(int i = NumeroColSplit + 1; i < iemeCol; i++)
		GetColonne(i)->SetDigitals(WhichDigValF);

return ARetourner;
}
int Sequence::DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToEndOfCol(int FromBeginOfCol, int PlusNbrPeriodeAfter, int UpToEndOfCol /* = -1 */)
{
	int Aretourner = 0;
	if(FromBeginOfCol < 0 && FromBeginOfCol >= -(int)m_nColonnes)
		FromBeginOfCol += m_nColonnes + 1;
	if(UpToEndOfCol < 0 && UpToEndOfCol >= -(int)m_nColonnes)
		UpToEndOfCol += m_nColonnes + 1;

	int periodeActiveApresDebutCol = 0;
	for(int i = FromBeginOfCol; i <= UpToEndOfCol; i++)
	{	// on active tant qu'on a pas activer assez de periode pour avoir periodeActiveApresDebutCol == PlusNbrPeriodeAfter
		int periodeAactiverSurLaCol = PlusNbrPeriodeAfter - periodeActiveApresDebutCol;
		if(periodeAactiverSurLaCol < 0)
		{	AfxMessageBox("Bizar 125645 : ca devrait jamais etre negatif!", MB_ICONERROR); break; }
		periodeActiveApresDebutCol += ActivateColonne(i, periodeAactiverSurLaCol);
	}

	return Aretourner;
}
int Sequence::DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToEndOfCol(CString PartNameFromCol, int PlusNbrPeriodeAfter, CString PartNameUpToCol /* = "" */)
{
	int Aretourner = 0;
	int FromCol = GetColNumero(PartNameFromCol);
	int ToCol = PartNameUpToCol == "" ? -1 : GetColNumero(PartNameUpToCol);
	if(FromCol != 0 && ToCol != 0)
		Aretourner = DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToEndOfCol(FromCol, PlusNbrPeriodeAfter, ToCol);
	return Aretourner;
}
int Sequence::DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToBeginOfCol(CString PartNameFromCol, int PlusNbrPeriodeAfter, CString PartNameUpToCol /* = "" */)
{
	int Aretourner = 0;
	int FromCol = GetColNumero(PartNameFromCol);
	int ToCol = PartNameUpToCol == "" ? -1 : GetColNumero(PartNameUpToCol);
	ToCol--;
	if(FromCol != 0 && ToCol != 0)
		Aretourner = DesactivateSequence_FromBeginOfCol_PlusNbrPeriodeAfter_UpToEndOfCol(FromCol, PlusNbrPeriodeAfter, ToCol);
	return Aretourner;
}
void Sequence::ActualiseCtrlListCol(CDC& dc)
{
	int epaisseurBordListe = 7;

	if(p_ListCtrlColonnes == NULL)
		return;
	// la liste des colonne
	p_ListCtrlColonnes->DeleteAllItems();
	while(p_ListCtrlColonnes->DeleteColumn(0))
	{;}

	// Dans quel zone est dessinée la séquence, et la CtrlList
	CRect rectView, rectDialog;
	dc.GetWindow()->GetWindowRect(rectView);
	p_ListCtrlColonnes->GetParent()->GetWindowRect(rectDialog);
	// coordonnée de la CListCtrl
	int HauteurList = 80;
	int top = rectDialog.bottom - rectDialog.top - HauteurList;
	// la droite de la liste
	int left = rectView.left - rectDialog.left - epaisseurBordListe;
	int droite = GetRectDigOut(m_nColonnes, 0).right + epaisseurBordListe;
	// mais doit etre dans la fenetre quand meme:
	droite = min(droite, rectView.right - rectDialog.left - 22);
	// on repositionne
	p_ListCtrlColonnes->SetWindowPos(0, left, top , droite - left, HauteurList, 0);
	//la premiere colonne (vide) entre left et m_FirstDigTopLeft.x
	p_ListCtrlColonnes->InsertColumn(0, "", LVCFMT_CENTER, m_FirstDigTopLeft.x - left - 4);

	CString str1,str2;
	p_ListCtrlColonnes->InsertItem(LVIF_TEXT,0,0,0,0,0,0);
	p_ListCtrlColonnes->InsertItem(LVIF_TEXT,0,0,0,0,0,0);
	// Pour chaque Colonne, on fait une colonne dans la CtrListCol
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		int LargeurColonne = GetRectDigOut(iemeCol, 0).Width();
		Colonne& LaCol = *GetColonne(iemeCol);
		p_ListCtrlColonnes->InsertColumn(iemeCol, LaCol.m_NameCol, LVCFMT_CENTER, LargeurColonne);
		unsigned int duree_µs = unsigned int((double)LaCol.GetDuree() * 1e6 / m_SampleRate);
		double duree_ms = (double)duree_µs /1e3;
		double duree_s = duree_ms /1e3;
		if(duree_s >= 1)
		{
			str1.Format("%g", duree_s);
			str2.Format("%s", "s");
		}
		else if(duree_ms >= 1)
		{
			str1.Format("%g", duree_ms);
			str2.Format("%s", "ms");
		}
		else if(duree_µs >= 1)
		{
			str1.Format("%d", duree_µs);
			str2.Format("%s", "µs");
		}
		p_ListCtrlColonnes->SetItemText(0, iemeCol, str1);
		p_ListCtrlColonnes->SetItemText(1, iemeCol, str2);
	}
return;
}

void Sequence::ActualiseCtrlListLigneDig(CDC& dc)
{
	int epaisseurBordListe = 7;
	int LargeurColonne = 128;

	if(p_ListCtrlLignesDig == NULL)
		return;
	// la liste des lignes Digitale
	p_ListCtrlLignesDig->DeleteAllItems();
	while(p_ListCtrlLignesDig->DeleteColumn(0))
	{;}
	// pour avoir des checkbox
	// 		p_ListCtrlLignesDig->SetExtendedStyle(p_ListCtrlLignesDig->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	// Dans quel zone est dessinée la séquence, et la CtrlList
	CRect rectView, rectDialog;
	dc.GetWindow()->GetWindowRect(rectView);
	p_ListCtrlLignesDig->GetParent()->GetWindowRect(rectDialog);
	// on repositionne
	int top = rectView.top - rectDialog.top - epaisseurBordListe;
	p_ListCtrlLignesDig->SetWindowPos(0, 0, top , LargeurColonne, rectView.Height(), 0);
	//la premiere colonne (vide) entre left et m_FirstDigTopLeft.x
	p_ListCtrlLignesDig->InsertColumn(0, "Name", LVCFMT_LEFT, LargeurColonne - epaisseurBordListe);

	CString str1;
	// Pour chaque Ligne digitales
	for(unsigned int indexDig = 0; indexDig < NUMBER_DIG_OUTPUT; indexDig++)
	{
		if(Sequence::m_ParamDefaultDigOut[indexDig])
		{	
			str1.Format("%s", Sequence::m_ParamDefaultDigOut[indexDig]->m_NomTexte);
			p_ListCtrlLignesDig->InsertItem(LVIF_TEXT,indexDig,str1,0,0,0,0);
		}
	}
return;
}

void Sequence::ActualiseCtrlListLigneAna(CDC& dc)
{
	int epaisseurBordListe = 7;
	int LargeurColonne = 128;
	if(p_ListCtrlLignesAna == NULL)
		return;
	// la liste des lignes Analogiques
	p_ListCtrlLignesAna->DeleteAllItems();
	while(p_ListCtrlLignesAna->DeleteColumn(0))
	{;}
	// pour avoir des checkbox
	// 		p_ListCtrlLignesDig->SetExtendedStyle(p_ListCtrlLignesDig->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);

	// Dans quel zone est dessinée la séquence, et la CtrlList
	CRect rectView, rectDialog;
	dc.GetWindow()->GetWindowRect(rectView);
	p_ListCtrlLignesAna->GetParent()->GetWindowRect(rectDialog);
	// on repositionne
	int top = rectView.top - rectDialog.top - epaisseurBordListe;
	p_ListCtrlLignesAna->SetWindowPos(0, 0, top, LargeurColonne, rectView.Height(), 0);
	//la premiere colonne (vide) entre left et m_FirstDigTopLeft.x
	p_ListCtrlLignesAna->InsertColumn(0, "Name", LVCFMT_LEFT, LargeurColonne - epaisseurBordListe);

	CString str1;
	// Pour chaque Ligne digitale,
	for(unsigned int indexAna = 0; indexAna < NUMBER_ANA_OUTPUT; indexAna++)
	{
		if(Sequence::m_ParamDefaultAnaOut[indexAna])
		{
			str1.Format("%s", Sequence::m_ParamDefaultAnaOut[indexAna]->m_NomTexte);
			p_ListCtrlLignesAna->InsertItem(LVIF_TEXT,indexAna,str1,0,0,0,0);
		}
	}
return;
}

void Sequence::RemoveColonne(int iemeCol)
{
	int where = iemeCol;
	if(where < 0 && where >= -(int)m_nColonnes)
		where += m_nColonnes;
	else if(where > 0 && where <= m_nColonnes)
		where -= 1;
	else
		return;

	POSITION pos = m_Liste_p_Colonnes.FindIndex(where);
	if(pos == NULL)
		return;
	Colonne* pLaCol = m_Liste_p_Colonnes.GetAt(pos);
	pLaCol->m_pSequenceOwning = NULL;
	m_Liste_p_Colonnes.RemoveAt(pos);
		
	delete pLaCol;

	m_nColonnes--;
return;
}


int Sequence::SetColonneDuration(int iemeCol, int nPeriodesCol)
{
	GetColonne(iemeCol)->SetDuree(nPeriodesCol);
	CalculateNbrPeriodes();
	return m_nPeriodes;
}
int Sequence::GetColDuration(int iemeCol)
{
	Colonne* pLaCol = GetColonne(iemeCol); 
	if(!pLaCol)
		return 0; 
	return pLaCol->GetDuree();
}
int Sequence::GetColActiveDuration(int iemeCol)
{
	Colonne* pLaCol = GetColonne(iemeCol); 
	if(!pLaCol)
		return 0; 
	return pLaCol->GetActiveDuree();
}

int Sequence::ActivateColonne(int iemeCol, int Aactiver /* = -1 */)
{
	Colonne* pLaCol = GetColonne(iemeCol); 
	if(!pLaCol)
		return 0; 
	pLaCol->SetActiveDuree(Aactiver);
	return pLaCol->GetActiveDuree();
}
int Sequence::GetColNumero(Colonne* pLaCol)
{
	POSITION pos = m_Liste_p_Colonnes.GetHeadPosition();
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		if(m_Liste_p_Colonnes.GetNext(pos) == pLaCol)
			return iemeCol;
	}
	return 0;
}
int Sequence::GetColNumero(CString & PartNameCol)
{
	POSITION pos = m_Liste_p_Colonnes.GetHeadPosition();
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		Colonne* pLaCol = m_Liste_p_Colonnes.GetNext(pos);
		if(pLaCol)
		{
			CString IemeColName = pLaCol->m_NameCol;
			if(IemeColName.Find(PartNameCol) != -1)
			{
				PartNameCol = IemeColName;
				return iemeCol;
			}
		}
	}
	return 0;
}

Colonne* Sequence::GetColonne(int iemeCol)
{
	Colonne* pLaCol = NULL;
	int where = iemeCol;
	if(where < 0 && where >= -(int)m_nColonnes)
		where += m_nColonnes;
	else if(where > 0 && where <= m_nColonnes)
		where -= 1;
	else
		return NULL;

	POSITION pos = m_Liste_p_Colonnes.FindIndex(where);
	if(pos == NULL)
		return NULL;

	pLaCol = m_Liste_p_Colonnes.GetAt(pos);
		
	return pLaCol;
}

int Sequence::MakeDigBuffer(void)
{
	ClearDigBuffer();	// commencons par nettoyer l'ancien buffer si il existe
	CalculateNbrPeriodes();	
//	p_DigBuffer = new unsigned long[m_nPeriodes];	// allouons la mémoire
	m_DigBufferNiMatrix[0].SetSize(1, m_nPeriodes, CNiMatrix::NiMatrixDoNotInit);

	Colonne* pLaColonne = NULL;
	unsigned int indexZero = 0;

	unsigned int DurActCol = 0;
	unsigned int indexPerSeq = 0;

	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++) 
	{
		pLaColonne = GetColonne(iemeCol);
		DurActCol = pLaColonne->GetActiveDuree();
		for(unsigned int indexPerCol = 0; indexPerCol < DurActCol; indexPerCol++) 
		{
//			p_DigBuffer[indexPerSeq] = LaColonne->GetDigInt32bit();
			m_DigBufferNiMatrix[0](indexZero, indexPerSeq) = pLaColonne->GetDigInt32bit();
			indexPerSeq++;
		}
	}
return indexPerSeq;
}

int Sequence::MakeWholeSeqWaveForme()
{
	int nbrPoints = 0;
	// pour chaque sortie analogique on fait une longue wavforme qui représente toute la séquence
	for(int indexChan = 0; indexChan < NUMBER_ANA_OUTPUT; indexChan++)
	{
		m_WholeSeqWaveForme[indexChan].ClearPointList();
		for(int iemeColSeq = 1; iemeColSeq <= m_nColonnes; iemeColSeq++)
		{
			Colonne & LaCol = *GetColonne(iemeColSeq);
			nbrPoints += m_WholeSeqWaveForme[indexChan].AddWaveFormeAtEnd(*LaCol.m_AnaChannel[indexChan], double(LaCol.GetDuree()) / m_SampleRate, 0.001);
		}
	}
return nbrPoints;
}

int Sequence::MakeAnaBuffer(void)
{
	// commencons par nettoyer les anciens buffers si ils existent
	ClearAnaBuffer();	
	CalculateNbrPeriodes();	

	m_AnaBufferNiMatrix[0].SetSize(NUMBER_ANA_OUTPUT_CARD_ANA_1, m_nPeriodes, CNiMatrix::NiMatrixDoNotInit);
	m_AnaBufferNiMatrix[1].SetSize(NUMBER_ANA_OUTPUT_CARD_ANA_2, m_nPeriodes, CNiMatrix::NiMatrixDoNotInit);

	Colonne* pLaColonne = NULL;
	unsigned int indexZero = 0;

	unsigned int iemePerSeq = 0;
	int DurActCol = 0;

bool useOld = false;//true;
if(useOld)
{
	// un buffer par sortie analogique
	for(unsigned char indexChan = 0; indexChan < NUMBER_ANA_OUTPUT; indexChan++)
	{
//		p_AnaBuffer[indexChan] = new double[m_nPeriodes];	// allouons la mémoire
		
		iemePerSeq = 0;
		DurActCol = 0;
		// quelle carte ?
		int indexCard = indexChan < NUMBER_ANA_OUTPUT_CARD_ANA_1 ? 0 : 1;
		// quelle chanel de la carte
		unsigned int indexChanCard = indexCard < 1 ? indexChan : indexChan - NUMBER_ANA_OUTPUT_CARD_ANA_1;
		
		for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++) 
		{
			pLaColonne = GetColonne(iemeCol);
			DurActCol = pLaColonne->GetActiveDuree();
			for(int indPerCol = 0; indPerCol < DurActCol; indPerCol++)
			{
//				p_AnaBuffer[indexChan][iemePerSeq] = pLaColonne->GetAnaValue_InterpolatedLinear(indexChan, indPerCol);
				m_AnaBufferNiMatrix[indexCard](indexChanCard, iemePerSeq) = pLaColonne->GetAnaValue_InterpolatedLinear(indexChan, indPerCol);
				iemePerSeq++;
			}
		}
	}
}
// TO DO
// il faut empecher CwaveForme d'aller au dessus de +- 10V , 
// en mettant un menbre MAxVal et MinVal dans CWaveForme, qui est fixé 
// au moment de la déclaration par exemple.
// ces valeures permettent d'ajouter un point dans la Wae forme de maniere 
// a faire saturer la valeur...
if(!useOld)
{
	MakeWholeSeqWaveForme();
	// un buffer par sortie analogique
	for(unsigned char indexChan = 0; indexChan < NUMBER_ANA_OUTPUT; indexChan++)
	{
		// quelle carte ?
		int indexCard = indexChan < NUMBER_ANA_OUTPUT_CARD_ANA_1 ? 0 : 1;
		// quelle chanel de la carte
		unsigned int indexChanCard = indexCard < 1 ? indexChan : indexChan - NUMBER_ANA_OUTPUT_CARD_ANA_1;
		m_WholeSeqWaveForme[indexChan].InterpolateLinearFillNiMatrix(m_AnaBufferNiMatrix[indexCard], 1/m_SampleRate, indexChanCard);
	}
}
return m_nPeriodes;
}
void Sequence::ClearDigBuffer(void)
{
	if(p_DigBuffer)
		delete p_DigBuffer;
return;
}

void Sequence::ClearAnaBuffer(void)
{
	// un buffer par sortie analogique
	for(unsigned char indexChan = 0; indexChan < NUMBER_ANA_OUTPUT; indexChan++)
		if(p_AnaBuffer[indexChan])
			delete (p_AnaBuffer[indexChan]);
return;
}

double Sequence::GetRatioDureeColonneDansUnSplit(int iemeCol)
{
	double ratio = 1.0;
	int LaColGauche = iemeCol;
	int LaColDroite = iemeCol;
	Colonne* pCol = GetColonne(iemeCol);
	int DureeTotalSplit = pCol->GetDuree();
	// d'abord, trouvons la colonne la plus a gauche, qui n'a pas de m_LimiteGaucheEstUnSplit
	// si une colonne fut splitée une fois, c'est celle de gauche, mais nous, on cherche dans le cas général d'une colonne qui fut splitée mainte fois
	while(LaColGauche > 1 && pCol->Is_LimiteGaucheUnSplit())
	{
		LaColGauche--;
		pCol = GetColonne(LaColGauche);
		DureeTotalSplit += pCol->GetDuree();
	}
	// puis celle de droite
	pCol = GetColonne(LaColDroite);
	while(LaColDroite < GetnColonnes() && pCol->Is_LimiteDroiteUnSplit())
	{
		LaColDroite++;
		pCol = GetColonne(LaColDroite);
		DureeTotalSplit += pCol->GetDuree();
	}
	ratio = double(GetColonne(iemeCol)->GetDuree()) / double(DureeTotalSplit);
return ratio;
}
CRect Sequence::GetRectDigOut(int iemeCol, int indexDig)
{
//	m_ColWidth = -100;
	// on definit pour la premiere chan de la premiere Colonne
	// puis on definira les autres par réurence en appelant cette meme fonction
	// point topleft de la premiere chan de la premiere Colonne
	CPoint topleft = m_FirstDigTopLeft;
	CSize size(m_ColWidth, m_DigHeight);
	// si m_ColWidth est négative, on ajuste la taille à ColonneDuration[µs??] / abs(m_ColWidth)
	if(m_ColWidth < 0)
		size.cx = GetColonne(iemeCol)->GetDuree() / abs(m_ColWidth);

	if(GetColonne(iemeCol)->Is_LimiteGaucheUnSplit() || GetColonne(iemeCol)->Is_LimiteDroiteUnSplit())
		size.cx *= GetRatioDureeColonneDansUnSplit(iemeCol);

	// recurence sur les lignes & Colonnes : case au dessus a gauche
	if(iemeCol > 1 && indexDig > 0)
	{
		topleft = GetRectDigOut(iemeCol - 1, indexDig - 1).BottomRight() + CPoint(m_EcartHoriz, m_EcartVert);
		return CRect(topleft, size);
	}
	// recurence sur les lignes : ligne précédente plus un ecart ....
	if(iemeCol == 1 && indexDig > 0)
	{
		topleft = GetRectDigOut(iemeCol, indexDig - 1).BottomRight() + CPoint(-size.cx, m_EcartVert);
		return CRect(topleft, size);
	}
	// recurence sur les colonnes : colonne précédente plus un ecart ....
	if(iemeCol > 1 && indexDig == 0)
	{
		topleft = GetRectDigOut(iemeCol - 1, indexDig).BottomRight() + CPoint(m_EcartHoriz, -size.cy);
		return CRect(topleft, size);
	}
	if(iemeCol == 1 && indexDig == 0)
		return CRect(topleft, size);
return CRect(topleft, size);
}
CRect Sequence::GetRectAnaOut(int iemeCol, int indexAna)
{
	// on definit pour la premiere chan de la premiere Colonne
	// puis on definira les autre par réurence en appelant cette meme fonction
	// point topleft de la premiere chan de la premiere Colonne
	CPoint topleft = m_FirstAnaTopLeft;
	CSize size(m_ColWidth, m_AnaHeight);
	// si m_ColWidth est négative, on ajuste la taille à ColonneDuration[µs??] / n
	if(m_ColWidth < 0)
		size.cx = GetColonne(iemeCol)->GetDuree() / abs(m_ColWidth);

	if(GetColonne(iemeCol)->Is_LimiteGaucheUnSplit() || GetColonne(iemeCol)->Is_LimiteDroiteUnSplit())
		size.cx *= GetRatioDureeColonneDansUnSplit(iemeCol);

	// recurence sur les lignes & Colonnes : case au dessus a gauche
	if(iemeCol > 1 && indexAna > 0)
	{
		topleft = GetRectAnaOut(iemeCol - 1, indexAna - 1).BottomRight() + CPoint(m_EcartHoriz, m_EcartVert);
		return CRect(topleft, size);
	}
	// recurence sur les lignes : ligne précédente plus un ecart ....
	if(iemeCol == 1 && indexAna > 0)
	{
		topleft = GetRectAnaOut(iemeCol, indexAna - 1).BottomRight() + CPoint(-size.cx, m_EcartVert);
		return CRect(topleft, size);
	}
	// recurence sur les colonnes : colonne précédente plus un ecart ....
	if(iemeCol > 1 && indexAna == 0)
	{
		topleft = GetRectAnaOut(iemeCol - 1, indexAna).BottomRight() + CPoint(m_EcartHoriz, -size.cy);
		return CRect(topleft, size);
	}
	if(iemeCol == 1 && indexAna == 0)
		return CRect(topleft, size);
return CRect(topleft, size);
}
CRect Sequence::Dessine_1_Digital(CDC& dc, int iemeCol, int indexDig, int IndexColor /* = -1 */)
{
	Colonne* pLaCol = GetColonne(iemeCol);
	if(pLaCol == NULL)
		return CRect(0,0,0,0);
	CRect Circonscrit = GetRectDigOut(iemeCol, indexDig);
//	CRect PourFillRect = Circonscrit;
//	PourFillRect.InflateRect(0,0,1, 1);

	COLORREF Color = IndexColor == -1 ? (DigNoir)  : IndexColor%6==0? (DigRouge )	: IndexColor%6==1? (DigVert )	: IndexColor%6==2? (DigBleu )	: IndexColor%6==3? (DigJaune )	: IndexColor%6==4? (DigBleuClaire	)	: (DigViolet )	;
	if(pLaCol->m_DigChannel[indexDig] == false)
		Color = DigBlanc;
	CBrush brush(Color);
	dc.FillRect( Circonscrit, &brush);

	// si la colonne en question n'est que partielement active, on grise la partie inactive
	double FractionActive = double(pLaCol->GetActiveDuree()) / double(pLaCol->GetDuree());
	if(FractionActive != 1)
	{	
		CRect RectGrise = Circonscrit;
		RectGrise.left += long(double(Circonscrit.Width()) * FractionActive);
		CBrush brushGrisee(pLaCol->m_DigChannel[indexDig] == false ? DigGrisee : IndexColor == -1 ? (DigGriseeNoir)  : IndexColor%6==0? (DigGriseeRouge )	: IndexColor%6==1? (DigGriseeVert )	: IndexColor%6==2? (DigGriseeBleu )	: IndexColor%6==3? (DigGriseeJaune )	: IndexColor%6==4? (DigGriseeBleuClaire	)	: (DigGriseeViolet ));
		dc.FillRect( RectGrise, &brushGrisee);
	}
	// Les lignes de contour
	CPen ligneFine(PS_SOLID, 1, DigNoir);
	CPen ligneFineGrisee(PS_SOLID, 1, DigLigneGrisee);
	CPen ligneTransparente(PS_SOLID, 1, Color);
	dc.SelectObject(ligneFine);	
	// la ligne du bas de la cellule, noire
	dc.MoveTo(Circonscrit.right , Circonscrit.bottom - 1);
	dc.LineTo(Circonscrit.left - 1	, Circonscrit.bottom - 1);
	// la ligne de gauche, dépend de sa m_Constr_DigContiWithPrevious
	bool conti = GetIemeBitFromInt32Bit(indexDig, m_Constr_Dig32ContiWithPrevious[iemeCol]);
	if(conti)
		dc.SelectObject(ligneTransparente);
	else if(pLaCol->Is_LimiteGaucheUnSplit())
		dc.SelectObject(ligneFineGrisee);
	dc.MoveTo(Circonscrit.TopLeft());
	dc.LineTo(Circonscrit.left 	, Circonscrit.bottom - 1);
return Circonscrit;
}
CRect Sequence::Dessine_1_Analog(CDC& dc, int iemeCol, int indexAna, int IndexColor /* = -1 */, int grisee /* = 0 */)
{
	Colonne* pLaCol = GetColonne(iemeCol);
	if(pLaCol == NULL)
		return CRect(0,0,0,0);
	CRect& circonci = GetRectAnaOut(iemeCol, indexAna);
	CWaveForme* pLaWf = pLaCol->m_AnaChannel[indexAna];

	// si la colonne en question n'est que partielement active, on grise la partie inactive
	double FractionActive = double(pLaCol->GetActiveDuree()) / double(pLaCol->GetDuree());
	if(FractionActive != 1)
	{	
		CRect RectGrise = circonci;
		RectGrise.left += long(double(circonci.Width()) * FractionActive);
		CBrush brushGrisee(DigGrisee);
		dc.FillRect( RectGrise, &brushGrisee);
	}

	if(pLaWf)
	{
		int OptionDessinWF = 0;
		if(m_Constr_AnaContiWithPrevious[indexAna])
			switch(m_Constr_AnaContiWithPrevious[indexAna][iemeCol])
			{
			case NoConti:
				OptionDessinWF |= WF_TRAITGAUCHE_VISIBLE | WF_VALGAUCHE_VISIBLE;
				break;
			case ContinueAGauche:
				OptionDessinWF |= WF_TRAITGAUCHE_GRISEE | WF_VALGAUCHE_VISIBLE;
				break;
			case SlopeContinueAGauche:
				OptionDessinWF |= WF_TRAITGAUCHE_GRISEE | WF_VALGAUCHE_GRISEE;
				break;
			}
		if(m_Constr_AnaContiWithPrevious[indexAna] && iemeCol < m_nColonnes)
			switch(m_Constr_AnaContiWithPrevious[indexAna][iemeCol + 1])
			{
			case NoConti:
				OptionDessinWF |= WF_VALDROITE_VISIBLE;
				break;
			case ContinueAGauche:
				OptionDessinWF |= 0;
				break;
			case SlopeContinueAGauche:
				OptionDessinWF |= 0;
				break;
			}

			pLaWf->DessineWF(dc, circonci, OptionDessinWF, IndexColor, CPointDbl(m_AnaMin_mV, m_AnaMax_mV));
	}
	return circonci;
}

CRect Sequence::DessineSeqView1(CDC& dc)
{
//	Make_Constr_Param();
	CRect LeRectTout(0,0,0,0);

	// Pour chaque colonne
	CString texte;
	// pour chaque colonne
	for(int iemeColSeq = 1; iemeColSeq <= m_nColonnes; iemeColSeq++) 
	{	
		Colonne* pLaCol = GetColonne( iemeColSeq );
		if(pLaCol == NULL)
			return CRect(0,0,0,0);
	
		CRect rect = GetRectDigOut(iemeColSeq, 0) - CPoint(0, m_FirstDigTopLeft.y);
		rect.bottom = m_FirstDigTopLeft.y;
		double duree = (pLaCol->GetDuree() / (MILLI_SECONDE(1.0)));
		texte.Format("%gms", duree);
		// pour les colonnes dont le temps doit etre afficher en µs:
		if(pLaCol->m_NameCol.Find("µs") != -1)
		{
			texte.Format(_T("%dµs"), unsigned int(double(pLaCol->GetDuree()) / (MICRO_SECONDE(1))));
		}
		dc.DrawText(texte,&rect,DT_LEFT);
	
		// pour chaque Channel Dig Out
		// boucle indexChan de 0 à NUMBER_DIG_OUTPUT - 1
		for(int indexChan = 0; indexChan < m_NbrDisplayDigChan; indexChan++) 
		{
			CRect& FillRect = Dessine_1_Digital(dc, iemeColSeq, indexChan, indexChan);
			LeRectTout.UnionRect(LeRectTout, FillRect);

			// pour la colonne des noms
			if(iemeColSeq == 1)
			{
				if(Sequence::m_ParamDefaultDigOut[indexChan])
				{	
					rect = GetRectDigOut(1, indexChan) - CPoint(m_FirstDigTopLeft.x, 0);
					texte.Format("%s", Sequence::m_ParamDefaultDigOut[indexChan]->m_NomTexte );				dc.DrawText(texte,&rect,DT_LEFT);
					texte.Format("%d", indexChan );
					dc.DrawText(texte,&rect,DT_RIGHT);
				}
			}
		}
	}

	ActualiseCtrlListCol(dc);
	ActualiseCtrlListLigneDig(dc);

return LeRectTout;
}

CRect Sequence::DessineSeqView2(CDC& dc)
{
//	Make_Constr_Param();
	CRect LeRectTout(0,0,0,0);

	// Pour chaque colonne
	CString texte;
	// pour chaque colonne
	for(int iemeColSeq = 1; iemeColSeq <= m_nColonnes; iemeColSeq++) 
	{	
		// pour chaque Channel Ana Out
		// boucle indexChan de 0 à NUMBER_ANA_OUTPUT - 1
		for(int indexChan = 0; indexChan < m_NbrDisplayAnaChan; indexChan++) 
		{	
			CRect& Circon = Dessine_1_Analog(dc, iemeColSeq, indexChan, indexChan);
			LeRectTout.UnionRect(LeRectTout, Circon);

			// pour la colonne des noms
			if(iemeColSeq == 1 && Sequence::m_ParamDefaultAnaOut[indexChan])
			{
				CRect rect = GetRectAnaOut(1, indexChan) - CPoint(m_FirstDigTopLeft.x, 0);

				texte.Format(_T("%s"), Sequence::m_ParamDefaultAnaOut[indexChan]->m_NomTexte );
				dc.DrawText(texte,&rect,DT_LEFT);
				texte.Format(_T("%d"), indexChan );
				dc.DrawText(texte,&rect,DT_RIGHT);
			}
		}
	}
	ActualiseCtrlListLigneAna(dc);

return LeRectTout;
}
CRect Sequence::ClickedDigitalToSwitch(CPoint point)
{
	CRect rectaredessiner = CRect(0,0,0,0);
	int iemeCol = PointedIemeColonne(point);
	Colonne* pLaCol = GetColonne(iemeCol);
	unsigned int IndexDigChan = Pointed_DigIndexY_IemeColX(point).y;
	if(IndexDigChan == ERREUR)
		return rectaredessiner;
	if(pLaCol)
	{
		// si on a defini des CParametrant qui implique des changement dans d'autre colonne, 
		// on les utilise, ca devrait tj etre le cas, 
		// mais on se laisse la possibilité de ne changer qu'une digital avec le else
		bool contiGauche = GetIemeBitFromInt32Bit(IndexDigChan, m_Constr_Dig32ContiWithPrevious[iemeCol]);
		bool contiDroite = (iemeCol < m_nColonnes) ? GetIemeBitFromInt32Bit(IndexDigChan, m_Constr_Dig32ContiWithPrevious[iemeCol + 1]) : false;
		if(contiGauche || contiDroite)
		{
			// tant qu'on trouve des conti, on revient vers la gauche
			while(GetIemeBitFromInt32Bit(IndexDigChan, m_Constr_Dig32ContiWithPrevious[iemeCol]))
				iemeCol--;
			// puis on change la valeur qui est celle du CParametrant qui dirige celles de droite
			bool oldVal = GetColonne(iemeCol)->m_DigChannel[IndexDigChan];
			GetColonne(iemeCol)->Set1Digital(IndexDigChan, !oldVal);
			rectaredessiner = GetRectDigOut(iemeCol, IndexDigChan);
			// puis on calcul juste le bon rectaredessiner allant vers la droite
			while(GetIemeBitFromInt32Bit(IndexDigChan, m_Constr_Dig32ContiWithPrevious[iemeCol + 1]))
				iemeCol++;
			rectaredessiner.UnionRect(rectaredessiner, GetRectDigOut(iemeCol + 1, IndexDigChan));
		}
		else
		{
			bool oldVal = pLaCol->m_DigChannel[IndexDigChan];
			pLaCol->Set1Digital(IndexDigChan, !oldVal);
			rectaredessiner = GetRectDigOut(iemeCol, IndexDigChan);
		}

		Make_Constr_Param();
//		ActualiseSequenceFromCParametrant();
	}
	return rectaredessiner;
}

CRect Sequence::ClickedAnalogToSet(CPoint point)
{
	CRect rectaretourner = CRect(0,0,0,0);
	int iemeCol = PointedIemeColonne(point);
	Colonne* pLaCol = GetColonne(iemeCol);
	// si on a cliqué a droite, on extrait l'info qui est 
	CPoint IemeAna_PosWf = Pointed_AnaIndexY_PosWfX(point);
	unsigned int LaAnaChan = IemeAna_PosWf.y;
	int GaucheMilieuDroite = IemeAna_PosWf.x;
	if(LaAnaChan == ERREUR)
		return rectaretourner;

	if(pLaCol)				 
	{
		CWaveForme& LaWaveForme = *(pLaCol->m_AnaChannel[LaAnaChan]);
		double valueDeb = LaWaveForme.GetPoint(0).y;
		double valueFin = LaWaveForme.GetPoint(-1).y;

		double value = 0.0;
		switch(GaucheMilieuDroite)
		{
		case WfGriseeGauche:
			value = valueDeb;			break;
		case WfGriseeMilieu:
			value = valueDeb;			break;
		case WfGriseeDroite:
			value = valueFin;			break;
		}

		CDialogSetValue SetValDlg(value);
		if(SetValDlg.DoModal() != IDOK) {return rectaretourner;}

		switch(GaucheMilieuDroite)
		{
		case WfGriseeGauche:
			pLaCol->Set1Analog(LaAnaChan, SetValDlg.m_Value, valueFin);			break;
		case WfGriseeMilieu:
			pLaCol->Set1Analog(LaAnaChan, SetValDlg.m_Value, SetValDlg.m_Value);break;
		case WfGriseeDroite:
			pLaCol->Set1Analog(LaAnaChan, valueDeb, SetValDlg.m_Value);			break;
		}

		Make_Constr_Param();
		rectaretourner = GetRectAnaOut(iemeCol, LaAnaChan);
		return rectaretourner;
	}
	return rectaretourner;
}

int Sequence::PointedIemeColonne(CPoint point)
{
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++) 
	{
		CRect& rectCol0 = GetRectDigOut(iemeCol, 0);
		if(rectCol0.right > point.x && rectCol0.left < point.x)
			return iemeCol;
	}
return ERREUR;
}

Colonne* Sequence::PointedColonne(CPoint point)
{
return GetColonne(PointedIemeColonne(point));
}
CPoint Sequence::Pointed_DigIndexY_IemeColX(CPoint point)
{
	CPoint DigCol(ERREUR, ERREUR);
	for(int indexDig = 0; indexDig < m_NbrDisplayDigChan; indexDig++) 
	{
		CRect& rectDig = GetRectDigOut(1, indexDig);
		if(rectDig.bottom > point.y && rectDig.top < point.y)
		{
			DigCol.y = indexDig;
			break;
		}
	}
	DigCol.x = PointedIemeColonne(point);
	return DigCol;
}

CPoint Sequence::Pointed_AnaIndexY_IemeColX(CPoint point)
{
	CPoint AnaCol(ERREUR, ERREUR);
	int indexAna = 0;
	CRect rectAna;
	int iemeCol = PointedIemeColonne(point);
	if(iemeCol == ERREUR)
		return ERREUR;
	for(indexAna = 0; indexAna < m_NbrDisplayAnaChan; indexAna++) 
	{
		rectAna = GetRectAnaOut(iemeCol, indexAna);
		if(rectAna.bottom > point.y && rectAna.top < point.y)
		{
			AnaCol.y = indexAna;
			break;
		}
		// si aucun n'a marché
		if(indexAna == m_NbrDisplayAnaChan - 1)
			return ERREUR;
	}
	AnaCol.x = PointedIemeColonne(point);
return AnaCol;
}

CPoint Sequence::Pointed_AnaIndexY_PosWfX(CPoint point)
{
	CPoint AnaCol(ERREUR, ERREUR);
	int indexAna = 0;
	CRect rectAna;
	int iemeCol = PointedIemeColonne(point);
	if(iemeCol == ERREUR)
		return ERREUR;
	for(indexAna = 0; indexAna < m_NbrDisplayAnaChan; indexAna++) 
	{
		rectAna = GetRectAnaOut(iemeCol, indexAna);
		if(rectAna.bottom > point.y && rectAna.top < point.y)
		{
			AnaCol.y = indexAna;
			break;
		}
		// si aucun n'a marché
		if(indexAna == m_NbrDisplayAnaChan - 1)
			return ERREUR;
	}
	// à quel niveau d'avencement de la WF avons nous cliqué
	long PercentInWF = long(100.0 * double(point.x - rectAna.left) / double(rectAna.Width()));

	unsigned int GaucheOuDroite = 0;
	if(PercentInWF <= 25.0)
		GaucheOuDroite = WfGriseeGauche;
	else if(PercentInWF >= 75.0)
		GaucheOuDroite = WfGriseeDroite;
	else
		GaucheOuDroite = WfGriseeMilieu;
	
	AnaCol.x = GaucheOuDroite;
return AnaCol;
}

void* Sequence::PointedParamSequenceView1(CPoint point)
{
	Colonne* pLaCol = PointedColonne(point);
	if(!pLaCol)
		return NULL;
	unsigned int indexDigChan = Pointed_DigIndexY_IemeColX(point).y;
	unsigned int indexAnaChan = Pointed_AnaIndexY_IemeColX(point).y;
    if(indexDigChan != ERREUR)
	{
		return &(pLaCol->m_DigChannel[indexDigChan]);
	}
    if(indexAnaChan != ERREUR)
	{
		return &(pLaCol->m_AnaChannel[indexAnaChan]);
	}
return NULL;
}
void* Sequence::PointedParamSequenceView2(CPoint point)
{
	Colonne* pLaCol = PointedColonne(point);
	if(!pLaCol)
		return NULL;
	unsigned int indexDigChan = Pointed_DigIndexY_IemeColX(point).y;
	unsigned int indexAnaChan = Pointed_AnaIndexY_IemeColX(point).y;
    if(indexDigChan != ERREUR)
	{
		return &(pLaCol->m_DigChannel[indexDigChan]);
	}
    if(indexAnaChan != ERREUR)
	{
		return &(pLaCol->m_AnaChannel[indexAnaChan]);
	}
return NULL;
}
int Sequence::MouseMoveOnSeq(UINT nFlags, CPoint point)
{
return 0;
}
int Sequence::LButtonDownOnSeq(UINT nFlags, CPoint point)
{
return 0;
}
int Sequence::RButtonDownOnSeq(UINT nFlags, CPoint point)
{
return 0;
}
int Sequence::RButtonUpOnSeq(UINT nFlags, CPoint point)
{
return 0;
}
int Sequence::MouseWheelOnSeq(UINT nFlags, short zDelta, CPoint pt)
{
	// si on scroll dans la region des digitale
	if(Pointed_DigIndexY_IemeColX(pt).y != ERREUR)
	{
		m_NbrDisplayDigChan += zDelta / WHEEL_DELTA;
		m_NbrDisplayDigChan = min(NUMBER_DIG_OUTPUT, max(m_NbrDisplayDigChan, 1));
		return 1;
	}
	// si on scroll dans la region des analogiques
	if(Pointed_AnaIndexY_IemeColX(pt).y != ERREUR)
	{
		m_NbrDisplayAnaChan += zDelta / WHEEL_DELTA;
		m_NbrDisplayAnaChan = min(NUMBER_ANA_OUTPUT, max(m_NbrDisplayAnaChan, 1));
		return 2;
	}
return 0;
}










void Sequence::AddColonne_Constr_Param(int AfterWhichCol_0Debut)
{
	// on se fait des nouveaux ** temporaire avec des new
	CString*		tempo_Name					= new CString[m_nColonnes + 1];
	int*			tempo_Duration				= new int[m_nColonnes + 1];
	int*			tempo_Duration_µs			= new int[m_nColonnes + 1];
	unsigned long*	tempo_Dig32bit				= new unsigned long[m_nColonnes + 1];
	int*			tempo_ColStartingGroupe		= new int[m_nColonnes + 1];
	unsigned long*	tempo_Dig32ContiWithPrevious = new unsigned long[m_nColonnes + 1];
	double*			tempo_AnaDebut[NUMBER_ANA_OUTPUT];
	double*			tempo_AnaFin[NUMBER_ANA_OUTPUT];
	int*			tempo_AnaContiWithPrevious[NUMBER_ANA_OUTPUT];
	for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)
	{	tempo_AnaDebut[j]				= new double[m_nColonnes + 1];
		tempo_AnaFin[j]					= new double[m_nColonnes + 1];
		tempo_AnaContiWithPrevious[j]	= new int[m_nColonnes + 1] ;	}
	// on rempli, en oubliant pas la case en plus en position AfterWhichCol_0Debut, sachant que debut c'est la premiere colonne, donc  1
	for(int i = 0; i <= m_nColonnes; i++)
	{	int iii = (i <= AfterWhichCol_0Debut)? i : i - 1;
		if(i == AfterWhichCol_0Debut + 1)
			iii = -456;
							// pour 0, on ne s'en sert pas		// si c'est LA Col rajoutée		sinon, on recopie le reste
		tempo_Name[i]							= (iii == 0)?""	:(iii == -456) ? ""		:	m_Constr_Name[iii]						;
		tempo_Duration[i]						= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_Duration[iii]					;
		tempo_Duration_µs[i]					= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_Duration_µs[iii]				;
		tempo_Dig32bit[i]						= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_Dig32bit[iii]					;
		tempo_ColStartingGroupe[i]				= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_ColStartingGroupe[iii]			;
		tempo_Dig32ContiWithPrevious[i]			= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_Dig32ContiWithPrevious[iii]	;
		for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)
		{	tempo_AnaDebut[j][i]				= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_AnaDebut[j][iii]				;
			tempo_AnaFin[j][i]					= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_AnaFin[j][iii]					;	
			tempo_AnaContiWithPrevious[j][i]	= (iii == 0)?0	:(iii == -456) ? 0		:	m_Constr_AnaContiWithPrevious[j][iii]	;
	}	}
	// on fait la passation de tableau
	if(m_Constr_Name)								delete[] m_Constr_Name;
	if(m_Constr_Duration)							delete[] m_Constr_Duration;
	if(m_Constr_Duration_µs)						delete[] m_Constr_Duration_µs;
	if(m_Constr_Dig32bit)							delete[] m_Constr_Dig32bit;
	if(m_Constr_ColStartingGroupe)					delete[] m_Constr_ColStartingGroupe;
	if(m_Constr_Dig32ContiWithPrevious)				delete[] m_Constr_Dig32ContiWithPrevious;
	for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)	
	{	if(m_Constr_AnaDebut[j])					delete[] (m_Constr_AnaDebut[j]);
       	if(m_Constr_AnaFin[j])						delete[] (m_Constr_AnaFin[j]);
		if(m_Constr_AnaContiWithPrevious[j])		delete[] (m_Constr_AnaContiWithPrevious[j]);} 
	m_Constr_Name					=	tempo_Name							;
	m_Constr_Duration				=	tempo_Duration						;
	m_Constr_Duration_µs			=	tempo_Duration_µs					;
	m_Constr_Dig32bit				=	tempo_Dig32bit						;
	m_Constr_ColStartingGroupe		=	tempo_ColStartingGroupe				;
	m_Constr_Dig32ContiWithPrevious	=	tempo_Dig32ContiWithPrevious		;
	for(int j = 0; j < NUMBER_ANA_OUTPUT; j++)
	{	m_Constr_AnaDebut[j]				=	tempo_AnaDebut[j]			;
		m_Constr_AnaFin[j]					=	tempo_AnaFin[j]				;
		m_Constr_AnaContiWithPrevious[j]	=	tempo_AnaContiWithPrevious[j];	}

}

void Sequence::Make_Constr_Param()
{
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		Colonne* pLaCol = GetColonne(iemeCol);
		if(pLaCol)
		{
			m_Constr_Name[iemeCol] = pLaCol->m_NameCol;
			int i=0; //juste pour passer en parametre a tokenize
			if(m_Constr_Name[iemeCol].Tokenize(" \t", i) == "")
				m_Constr_Name[iemeCol] = 	"NoName";
			m_Constr_Duration[iemeCol] = pLaCol->GetDuree();
			//////////// si la colonne n'est pas activée, on mais sa durée en négatif.
			//////////if(!pLaCol->m_ActiveDuree)
			//////////{
			//////////	m_Constr_Duration[iemeCol] *= -1;
			//////////}
			m_Constr_Dig32bit[iemeCol] = pLaCol->GetDigInt32bit();
			for(char indexAna = 0; indexAna < NUMBER_ANA_OUTPUT; indexAna++)
			{
				CWaveForme* pLaWave = pLaCol->m_AnaChannel[indexAna];
				if(pLaWave)
				{
					m_Constr_AnaDebut[indexAna][iemeCol] = pLaWave->GetPoint(0).y;
					m_Constr_AnaFin[indexAna][iemeCol]	= pLaWave->GetPoint(-1).y;
				}
			}
		}
	}
}
void Sequence::MakeSequenceFrom_Constr_Param()
{

}
int Sequence::MakeCParametrantFrom_Sequence_AndConstrParam()
{
	CString Str; 
	// il faut que la séquence existe avant d'y attcher les CParametre !
	if(GetColonne(m_nColonnes) == NULL)
	{
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "Il faut d'abord faire la séquence\nà partir des m_Constr_Paramètres\nEt en suite créer les CParametre avec :\n\n\"MakeCParametrantFrom_Sequence_AndConstrParam()\"", "MakeCParametrantFrom_Sequence_AndConstrParam()", 1);
		return 0;
	}
	m_SeqSave.ClearAllCParametre();

Make_Constr_Param();

	// les parametres de constructions doivent etre CParametrés pour pouvoir etre sauvés
	Str.Format("NumCol");										//    "" -> pas de valeur par default pour ne pas remplacer la valeur de m_nColonne
	new CParametre(&m_SeqSave,  Entier,	Str, Str, Auto, &m_nColonnes, "", CanParticipateToNothing);
	Str.Format("D32Cont");
	new CParametre(&m_SeqSave,  Entier,	Str, Str, Auto, m_Constr_Dig32ContiWithPrevious, "0", CanParticipateVariation | CanParticipateGeneticAlgo, m_nColonnes + 1);
	for(int indexAna = 0; indexAna < NUMBER_ANA_OUTPUT; indexAna++)
	{
		Str.Format(_T("A%dCont"), indexAna);
		new CParametre(&m_SeqSave,  Entier,	Str, Str, Auto, m_Constr_AnaContiWithPrevious[indexAna], "0", CanParticipateToNothing, m_nColonnes + 1);
	}
	Str.Format("ColNames");
	new CParametre(&m_SeqSave,  String,	Str, Str, Auto, m_Constr_Name, "None", CanParticipateToNothing, m_nColonnes + 1);

	// On va d'abord faire tout les parametres définnissant une séquence colonne par colonne, valeur par valeur
	// Apres seulement, on détruira certain Cparametre redondant
	// pour chaque colonne
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		Colonne& LaIemeCol = *GetColonne(iemeCol);
		// durée
		Str.Format(_T("C%dDur"), iemeCol);
		LaIemeCol.m_CParamDuration = 
		new CParametre(&m_SeqSave,  Entier,	Str, Str, Auto, &(m_Constr_Duration[iemeCol]), "0", CanParticipateVariation | CanParticipateGeneticAlgo);
		LaIemeCol.m_CParamDuration->Set_Val(int(LaIemeCol.GetDuree()));
		// 32BitDigitale
		Str.Format(_T("C%dDig32"), iemeCol);
		LaIemeCol.m_CParamDig32b = 
		new CParametre(&m_SeqSave,  Entier,	Str, Str, Auto, &(m_Constr_Dig32bit[iemeCol]), "0", CanParticipateVariation | CanParticipateGeneticAlgo);
		LaIemeCol.m_CParamDig32b->Set_Val(int(LaIemeCol.GetDigInt32bit()));
		// pour chaque analogique
		for(int indexAna = 0; indexAna < NUMBER_ANA_OUTPUT; indexAna++)
		{
			// valeur de début
			Str.Format(_T("C%dA%dDeb"), iemeCol, indexAna);
			LaIemeCol.m_CParamValDeb[indexAna] = 
			new CParametre(&m_SeqSave,  Double,	Str, Str, Auto, &(m_Constr_AnaDebut[indexAna][iemeCol]), "0", CanParticipateVariation | CanParticipateGeneticAlgo);
			LaIemeCol.m_CParamValDeb[indexAna]->Set_Val(LaIemeCol.m_AnaChannel[indexAna]->GetPoint(0).y);
			// valeur de fin
			Str.Format(_T("C%dA%dFin"), iemeCol, indexAna);
			LaIemeCol.m_CParamValFin[indexAna] = 
			new CParametre(&m_SeqSave,  Double,	Str, Str, Auto, &(m_Constr_AnaFin[indexAna][iemeCol]), "0", CanParticipateVariation | CanParticipateGeneticAlgo);
			LaIemeCol.m_CParamValFin[indexAna]->Set_Val(LaIemeCol.m_AnaChannel[indexAna]->GetPoint(-1).y);
		}
	}

	MakeNotRedondant_Constr_param();
	ActualiseSequenceFromCParametrant();
return 0;
}
int Sequence::MakeNotRedondant_Constr_param()
{
	// il faut que la séquence et que les parametres soient fait avant d'en supprimer !
	if(GetColonne(m_nColonnes) == NULL)
	{
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "Il faut d'abord faire la séquence\nà partir des m_Constr_Paramètres\nEt en suite créer les CParametre avec :\n\n\"MakeCParametrantFrom_Sequence_AndConstrParam()\"", "MakeCParametrantFrom_Sequence_AndConstrParam()", 1);
		return 0;
	}

	// Pour chaque ligne Analogique
	// on regarde les cluster de continuité, ou de slope continuité, puis de Constance
	// puis on supprime les Cparmetre redondant
	int iemeCluster = 0;
	for(int indexAna = 0; indexAna < NUMBER_ANA_OUTPUT; indexAna++)
	{
		for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
		{
			Colonne* pLaCol = GetColonne(iemeCol);
			Colonne* pLaColPrecedente = GetColonne(iemeCol - 1);
			// Pour SlopeContinueAGauche:
			// on détruit le CParametre Debut de la colonne et le Fin de la colonne précendente
			// left devrais etre au moins egale a 2!!
			if(m_Constr_AnaContiWithPrevious[indexAna][iemeCol] & SlopeContinueAGauche)
			{
				if(pLaCol->m_CParamValDeb[indexAna])
				{	
					delete pLaCol->m_CParamValDeb[indexAna];
					pLaCol->m_CParamValDeb[indexAna] = NULL;	
				}
				if(pLaColPrecedente->m_CParamValFin[indexAna])
				{
					delete pLaColPrecedente->m_CParamValFin[indexAna];
					pLaColPrecedente->m_CParamValFin[indexAna] = NULL;	
				}
			}
			if(m_Constr_AnaContiWithPrevious[indexAna][iemeCol] & ContinueAGauche)
			{
				if(pLaCol->m_CParamValDeb[indexAna])
				{
					delete pLaCol->m_CParamValDeb[indexAna];
					pLaCol->m_CParamValDeb[indexAna] = NULL;	
				}
			}
			if(m_Constr_AnaContiWithPrevious[indexAna][iemeCol] & Constance)
			{
				if(pLaCol->m_CParamValDeb[indexAna])
				{	
					delete pLaCol->m_CParamValFin[indexAna];
					pLaCol->m_CParamValFin[indexAna] = NULL;
				}
			}
		}
	}
return 0;
}

CParametre* Sequence::GetCParamMostLeftThatDefineThisValue(int iemeCol, int indexAna, int Debut0_Fin1)
{
	CParametre* pParRetourner = NULL;
	// propriété de la colonne, et eventullement de la suivante
	int typePropriete = m_Constr_AnaContiWithPrevious[indexAna][iemeCol];
	int typeProprieteSuiv = 0;
	if(iemeCol < m_nColonnes)
		typeProprieteSuiv = m_Constr_AnaContiWithPrevious[indexAna][iemeCol + 1];
	Colonne & LaCol = *GetColonne(iemeCol);

	// la valeur de debut est CParametrante si il n'y a pas Conti, ni SlopeConti
	bool ContiOuSlopConti = bool(typePropriete & (ContinueAGauche + SlopeContinueAGauche));
	if( Debut0_Fin1 == 0 && !ContiOuSlopConti )
		pParRetourner = LaCol.m_CParamValDeb[indexAna];
	// la valeur de fin est CParametrante si il n'y a pas Constance ni SlopeContinu à la colonne suivante
	else if( Debut0_Fin1 == 1
		&& !(typePropriete & Constance) 
		&& !(typeProprieteSuiv & SlopeContinueAGauche) )
		pParRetourner = LaCol.m_CParamValFin[indexAna];
	// sinon, on se déplace d'un demi cran vers la gauche avec la meme fonction
	else if(Debut0_Fin1 == 0)
		pParRetourner = GetCParamMostLeftThatDefineThisValue(iemeCol - 1, indexAna, 1);
	else if(Debut0_Fin1 == 1)
		pParRetourner = GetCParamMostLeftThatDefineThisValue(iemeCol, indexAna, 0);

	if(pParRetourner == NULL)
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "Probleme de Cparametrage\nUn pCParametre est NULL\nalors qu'il devrait definir la séquence !", "GetCParamMostLeftThatDefineThisValue", 1);
return pParRetourner;
}

int Sequence::ActualiseSequenceFromCParametrant()
{
	// Les Timings et noms des colonnes
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		SetColonneDuration(iemeCol, m_Constr_Duration[iemeCol]);
		GetColonne(iemeCol)->m_NameCol = m_Constr_Name[iemeCol];
	}
	// Des valeurs sont mises a l'identique en fontion des ContinueAGauche, ou Constance
	for(int iemeCol = 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		Colonne & LaCol = *GetColonne(iemeCol);
		for(int indexAnaOut = 0; indexAnaOut < NUMBER_ANA_OUTPUT; indexAnaOut++)
		{
			double valDeb = ERREUR;
			double valFin = ERREUR;
			CParametre* mosteLeft = GetCParamMostLeftThatDefineThisValue(iemeCol, indexAnaOut, 0);
			CParametre* mosteRight = GetCParamMostLeftThatDefineThisValue(iemeCol, indexAnaOut, 1);
			if(mosteLeft == NULL || mosteRight == NULL)
				return 0;
			valDeb = mosteLeft->Get_Val_Double();
			valFin = mosteRight->Get_Val_Double();
			LaCol.Set1Analog(indexAnaOut, valDeb, valFin);
		}
		for(int indexDig = 0; indexDig < NUMBER_DIG_OUTPUT; indexDig++)
		{
			bool indexDigconti = GetIemeBitFromInt32Bit(indexDig, m_Constr_Dig32ContiWithPrevious[iemeCol]);
			// si conti, on prend la valeur de la colonne précendente
			if(indexDigconti && iemeCol > 1)
			{
				Colonne & LaColPrec = *GetColonne(iemeCol - 1);
				bool stateColPrec = GetIemeBitFromInt32Bit(indexDig, LaColPrec.GetDigInt32bit());
				LaCol.Set1Digital(indexDig, stateColPrec);
			}
			else // sinon, on met la valeur stockée dans m_CParamDig32b
			{
				LaCol.Set1Digital(indexDig, GetIemeBitFromInt32Bit(indexDig, LaCol.m_CParamDig32b->Get_Val_Int()));
			}
		}
	}
	// enfin, on fait les SlopeConti...
	ActualiseSequenceFromLinkedSlopeConti();
	return 0;
}

int Sequence::ActualiseSequenceFromLinkedSlopeConti()
{	
	// Pour chaque ligne Analogique
	// on regarde les cluster de SlopeContiAGauche, 
	// puis on assigne les bonne valeurs aux parametres de la Sequence

	for(int indexAnaOut = 0; indexAnaOut < NUMBER_ANA_OUTPUT; indexAnaOut++)
	{
		int iemeCluster = 0;
		bool IlYEnAEncore = true;
		while(IlYEnAEncore)// tant qu'il y en a on continue
		{
			// trouvons le premier cluster
			iemeCluster++;
			CRect Cluster = FindCluster_IemeAna(indexAnaOut, iemeCluster);
			if(Cluster.left == 0)
				break;
			IlYEnAEncore = bool(Cluster.bottom);
			long left = Cluster.left;
			long right = Cluster.right;
			long type = Cluster.top;
			long Encore = Cluster.bottom;
			if(left >= 2 && (type == SlopeContinueAGauche) )
			{
				// Les valeurs de debut et fin de cluster
				CParametre* pParDebClust = GetCParamMostLeftThatDefineThisValue(left - 1, indexAnaOut, 0);
				CParametre* pParFinClust = GetCParamMostLeftThatDefineThisValue(right, indexAnaOut, 1);
				double valDebutCluster = pParDebClust->Get_Val();
				double valFinCluster = pParFinClust->Get_Val();
				// temps utile pour le calcul des valeurs intermediaires
				int totalTemps = 0;
				int interTemps = 0;
				for(int iemeCol = left - 1; iemeCol <= right; iemeCol++)
					totalTemps += GetColonne(iemeCol)->GetDuree();

				// Mise a une certaine valeur
				for(int iemeCol = left - 1; iemeCol <= right; iemeCol++)
				{
					Colonne& LaCol = *GetColonne(iemeCol);
					double valInterG = IntermediateValue(valDebutCluster, valFinCluster, totalTemps, interTemps);
					interTemps += LaCol.GetDuree();
					double valInterD = IntermediateValue(valDebutCluster, valFinCluster, totalTemps, interTemps);
					LaCol.Set1Analog(indexAnaOut, valInterG, valInterD);
				}
			}
		}
	}


return 0;
}

CRect Sequence::FindCluster_IemeAna(int indexAna, int iemeCluster)
{
	// Renvoye CRect: 
	// left = PremiereColCluster, 
	// right = DerniereColCluster, 
	// top = TypeProprieteEtendue, 
	// bottom = IlExisteEncoreDesCluster
	CRect RectRetourner(0,0,0,0);
	if(iemeCluster <=0)
		return RectRetourner;
	// tout d'abord, definnissont un cluster, if any
	// un cluster, c'est une zone d'une ou plusieur colonne contigue, qui ont la propriété
	// Continue, SlopeContinue, et/ou Constance

	// on initialise la recherche
	int i_PremiereColCluster = 0;
	int i_DerniereColCluster = 0;
	int i_Clust = 0;
	int typeCol = 0;
	int typeColPrec = 0;
	// il peut y en a voir plusieur dans une meme ligne...
	// qui se chevauche eventuellement
	// on ne cherche pas les propriété de type ContiAGauche, ou Constance
	// , qui sont propres a une Colonne donnée
	// mais plutot de type SlopeContiAGauche, ou autre (? SommeTempsConstante ?)
	for(int iemeCol = i_DerniereColCluster + 1; iemeCol <= m_nColonnes; iemeCol++)
	{
		typeCol = m_Constr_AnaContiWithPrevious[indexAna][iemeCol];
		typeColPrec = m_Constr_AnaContiWithPrevious[indexAna][iemeCol - 1];
		// si on trouve un debut d'un cluster:
		if(	(typeCol & SlopeContinueAGauche)
			&& !(typeColPrec & SlopeContinueAGauche) )
		{
			i_Clust++;
			i_PremiereColCluster = iemeCol;
			i_DerniereColCluster = iemeCol;
		}
		// si on trouve une continuation
		if( (typeCol & SlopeContinueAGauche) 
			&& (typeColPrec & SlopeContinueAGauche) )
		{
			i_DerniereColCluster = iemeCol;
		}

		// si c'est ce cluster qu'on cherche
		if(i_Clust == iemeCluster)
		{
			RectRetourner.left = i_PremiereColCluster;
			RectRetourner.right = i_DerniereColCluster;
			RectRetourner.top = SlopeContinueAGauche;
			RectRetourner.bottom = 0;
		}
		// si il y en a en plus
		if(i_Clust > iemeCluster)
			RectRetourner.bottom = 1;
	}
	// si on a trouver le ieme SlopeContinueAGauche, on retourne maintenant
	if(i_Clust >= iemeCluster)
		return RectRetourner;

return RectRetourner;
}

bool Sequence::GetDigContiWithPrevious(int iemeCol, int IndexDig)
{
	if(iemeCol > m_nColonnes || IndexDig >= NUMBER_DIG_OUTPUT)
		return false;
	unsigned long & ContiCol = m_Constr_Dig32ContiWithPrevious[iemeCol];
	return GetIemeBitFromInt32Bit(IndexDig, ContiCol);
}
int Sequence::GetAnaContiWithPrevious(int iemeCol, int IndexAna)
{
	if(iemeCol > m_nColonnes || IndexAna >= NUMBER_ANA_OUTPUT)
		return false;
	int & ContiCol = m_Constr_AnaContiWithPrevious[IndexAna][iemeCol];
	return ContiCol;
}
CRect Sequence::SetDigContiWithPrevious(int iemeCol, int IndexDig, bool state)
{
	CRect rectaredessiner = CRect(0,0,0,0);
	if(iemeCol > m_nColonnes || IndexDig >= NUMBER_DIG_OUTPUT)
		return rectaredessiner;
	unsigned long & ContiCol = m_Constr_Dig32ContiWithPrevious[iemeCol];
	bool oldstate = SetIemeBitInsideInt32Bit(IndexDig, ContiCol, state);
	MakeCParametrantFrom_Sequence_AndConstrParam();

	rectaredessiner = GetRectDigOut(iemeCol, IndexDig);
	// puis on calcul juste le bon rectaredessiner allant vers la droite
	int iemeColConti = iemeCol;
	while(GetIemeBitFromInt32Bit(IndexDig, m_Constr_Dig32ContiWithPrevious[iemeColConti + 1]))
		iemeColConti++;
	rectaredessiner.UnionRect(rectaredessiner, GetRectDigOut(iemeColConti, IndexDig));
	
	return rectaredessiner;
}

CRect Sequence::SetAnaContiWithPrevious(int iemeCol, int IndexAna, int typeConti)
{
	CRect rectaredessiner = CRect(0,0,0,0);
	if(iemeCol > m_nColonnes || IndexAna >= NUMBER_ANA_OUTPUT)
		return rectaredessiner;
	if(typeConti != DoNothing)
	{
		int & LaConti = m_Constr_AnaContiWithPrevious[IndexAna][iemeCol];
		LaConti = typeConti;
		MakeCParametrantFrom_Sequence_AndConstrParam();
	}

	rectaredessiner = GetRectAnaOut(iemeCol, IndexAna);
	// puis on calcul juste le bon rectaredessiner en allant vers la droite
	// tant qu'on trouve des contiSlope ou Conti, puis, on revient vers la gauche
	int iemeColConti = iemeCol;
	while((m_Constr_AnaContiWithPrevious[IndexAna][iemeColConti] == SlopeContinueAGauche
		||m_Constr_AnaContiWithPrevious[IndexAna][iemeColConti] == ContinueAGauche)
		&& iemeColConti > 1)
		iemeColConti--;
	rectaredessiner.UnionRect(rectaredessiner, GetRectAnaOut(iemeColConti - 1, IndexAna));
	while((m_Constr_AnaContiWithPrevious[IndexAna][iemeColConti + 1]) && iemeColConti < GetnColonnes())
		iemeColConti++;
	rectaredessiner.UnionRect(rectaredessiner, GetRectAnaOut(iemeColConti, IndexAna));
	
	return rectaredessiner;
}





