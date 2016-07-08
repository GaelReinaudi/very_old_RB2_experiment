#include "StdAfx.h"
#include "Colonne.h"
#include "Sequence.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////        Colonne        ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
/*
Initialisation de la variable static NumberColonne
*/
unsigned int Colonne::NumberColonne = 0;

/*
constructeur : affecte des valeurs par defaut au membres de la colonne
*/
Colonne::Colonne(void):m_DebutDunGroupe(false),m_LimiteGaucheEstUnSplit(false),m_LimiteDroiteEstUnSplit(false)
{
	m_NameCol = "un-named";
	SetDuree(100);
	ResetCol();
	NumberColonne++;
	m_pSequenceOwning = NULL;
	MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "Creation Colonne", "C'est pas bon comme ca", 1);
}

/*
constructeur : affecte des valeurs par defaut au membres de la colonne
*/
Colonne::Colonne(CString UnNom, unsigned int UnNombrePeriode):m_DebutDunGroupe(false),m_LimiteGaucheEstUnSplit(false),m_LimiteDroiteEstUnSplit(false)
{
	m_CParamDuration = NULL;
	m_CParamDig32b = NULL;
	m_pSequenceOwning = NULL;
	for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
	{
		m_CParamValDeb[i] = NULL;
		m_CParamValFin[i] = NULL;
	}

	m_NameCol = UnNom;
	SetDuree(UnNombrePeriode);
	ResetCol();
	NumberColonne++;
}

/*
constructeur dit "de copie"
*/
Colonne::Colonne(const Colonne& UneCol):m_DebutDunGroupe(false),m_LimiteGaucheEstUnSplit(false),m_LimiteDroiteEstUnSplit(false)
{
	m_CParamDuration = NULL;
	m_CParamDig32b = NULL;
	m_pSequenceOwning = NULL;
	for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
	{
		m_CParamValDeb[i] = NULL;
		m_CParamValFin[i] = NULL;
	}

	m_NameCol = UneCol.m_NameCol;
	SetDuree(UneCol.m_Duree, UneCol.m_ActiveDuree);
	for(int i = 0; i < NUMBER_DIG_OUTPUT; i++)
		m_DigChannel[i] = UneCol.m_DigChannel[i];
	for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
	{
		if(UneCol.m_AnaChannel[i])
			m_AnaChannel[i] = new CWaveForme(*UneCol.m_AnaChannel[i]);
		else
			m_AnaChannel[i] = NULL;
	}
	NumberColonne++;
}
/*
constructeur dit "de copie" avec nouveau nom et duration
*/
Colonne::Colonne(const Colonne& UneCol, CString UnNewNom, unsigned int UnNewNombrePeriode /* = 0 */):m_DebutDunGroupe(false),m_LimiteGaucheEstUnSplit(false),m_LimiteDroiteEstUnSplit(false)
{
	m_CParamDuration = NULL;
	m_CParamDig32b = NULL;
	m_pSequenceOwning = NULL;
	for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
	{
		m_CParamValDeb[i] = NULL;
		m_CParamValFin[i] = NULL;
	}

	m_NameCol = UnNewNom;
	if(UnNewNombrePeriode)
		SetDuree(UnNewNombrePeriode); // active toute la colonne par defaut
	else
		SetDuree(UneCol.m_Duree); // active toute la colonne par defaut

	for(int i = 0; i < NUMBER_DIG_OUTPUT; i++)
		m_DigChannel[i] = UneCol.m_DigChannel[i];
	for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
	{
		if(UneCol.m_AnaChannel[i])
			m_AnaChannel[i] = new CWaveForme(*UneCol.m_AnaChannel[i]);
		else
			m_AnaChannel[i] = NULL;
	}
	m_LimiteDroiteEstUnSplit = UneCol.m_LimiteDroiteEstUnSplit;
	m_LimiteGaucheEstUnSplit = UneCol.m_LimiteGaucheEstUnSplit;
	NumberColonne++;
}

Colonne::~Colonne(void)
{
	// On detruit toute les WF associées a cette Colonne
	for(int ieme = 0; ieme < NUMBER_ANA_OUTPUT; ieme++)
		if(m_AnaChannel[ieme])
		{
			POSITION pos = CWaveForme::Liste_WaveFormes.Find(m_AnaChannel[ieme]);
			if(pos)
			{
				CWaveForme* pLaWF = CWaveForme::Liste_WaveFormes.GetAt(pos);
				CWaveForme::Liste_WaveFormes.RemoveAt(pos);
				delete (pLaWF);
			}
		}
	NumberColonne--;
}

/*
reset toutes les sorties digitales à 0 
et les waveformes des analogiques à numero 0
*/
void Colonne::ResetCol(void)
{
	for(int i = 0; i < NUMBER_DIG_OUTPUT; i++)
		m_DigChannel[i] = 0;
	for(int i = 0; i < NUMBER_ANA_OUTPUT; i++)
		m_AnaChannel[i] = NULL;
return;
}

/*
met la sortie digitale nDigOut à la valeur state, ou 0 par defaut 
indexDigOut commence à 0 pour la première sortie
Renvoye la somme de tous les bit : compris entre 0 et 32
*/
unsigned char Colonne::Set1Digital(unsigned char indexDigOut, int TheState)
{
	int state = TheState;
	if(this == NULL)
	{
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "Set_1_Digital", "This Column Does NOT Exist !", 1);
		return 0;
	}
	m_DigChannel[indexDigOut] = state?true:false;
	// doit on retrouver la valeur précédente ? //////////////////////////////////////
	if(state == PRECEDENT_VALUE)													//
	{																				//
		if(m_pSequenceOwning) // si la colonne fait partie d'une séquence			//
		{																			//
			// qu'elle est la colonne précédente									//
			Colonne* pLaColPrec = NULL;												//
			int numeroThisCol = m_pSequenceOwning->GetColNumero(this);				//
			if(numeroThisCol <= 1)													//
				pLaColPrec = NULL;													//
			else																	//
				pLaColPrec = m_pSequenceOwning->GetColonne(numeroThisCol - 1);		//
			// s'il y a bien une colonne avant										//
			if(pLaColPrec)															//
			{																		//
				m_DigChannel[indexDigOut] = pLaColPrec->m_DigChannel[indexDigOut];	//
			}																		//
			else // sinon, valeur par défault										//
				state = DEFAULT_VALUE;												//
		}																			//
		else // sinon, valeur par défault											//
			state = DEFAULT_VALUE;													//
	}/////////////////////////////////////////////////////////////////////////////////
	if(state == DEFAULT_VALUE)
		m_DigChannel[indexDigOut] = bool(Sequence::GetDefaultParamDigOut(indexDigOut)->Get_Val_Int());
	unsigned char som = 0;
	for(unsigned char iDigOut = 0; iDigOut < NUMBER_DIG_OUTPUT; iDigOut++)
		som += m_DigChannel[iDigOut]?1:0;
return som;
}

unsigned int Colonne::SetDigitals_DefaultPrecedentColonne(WhichDigVal)
{	if(this == NULL)
	{
		AfxMessageBox("SetDigitals_DefaultPrecedentColonne : This Column Does NOT Exist !", MB_ICONERROR);
		return 0;
	}
	int which[32] = {d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31, d32};
	int val[32] = {v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32};
	// pour les digitales spécifiées, on Assigne grace à Set1Digital(...)
	int NombreDigSpecifiee = 0;
	for(int iemeParam = 0; iemeParam < 32; iemeParam++)
	{	// non spécifiée (-1 valeur par défaut): break;
		if(which[iemeParam] < 0)
			break;
		// probleme : break
		if(which[iemeParam] >= NUMBER_DIG_OUTPUT)
		{	AfxMessageBox("Index de sortie Digitale >= NUMBER_DIG_OUTPUT !", MB_ICONEXCLAMATION);
			break;	}
		// sinon:
		NombreDigSpecifiee++;
		Set1Digital(which[iemeParam], val[iemeParam]);
	}
	// pour les non-spécifiée, on continue la valeur de la colonne précédente, si cette colonne fait partie d'une sequence
	// ou DEFAULT_VALUE si il n'y a pas de colonne avant
	for(int indexDig = 0; indexDig < NUMBER_DIG_OUTPUT; indexDig++)
	{	// on ne refait pas les spécifiées:///////////////////////////////////////
		bool AlreadyDone = false;												//
		for(int iemeParam = 0; iemeParam < NombreDigSpecifiee; iemeParam++)		//
			if(indexDig == which[iemeParam])									//
				AlreadyDone = true;												//
		if(AlreadyDone)															//
			continue;															//
		//////////////////////////////////////////////////////////////////////////
		Set1Digital(indexDig, PRECEDENT_VALUE);
	}

	unsigned int som = 0;
	for(unsigned int iDigOut = 0; iDigOut < NUMBER_DIG_OUTPUT; iDigOut++)
		som += m_DigChannel[iDigOut]?1:0;
return som;
}

unsigned int Colonne::SetDigitals(WhichDigVal)
{	if(this == NULL)
	{
		AfxMessageBox("SetDigitals : This Column Does NOT Exist !", MB_ICONERROR);
		return 0;
	}
	int which[32] = {d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d14, d15, d16, d17, d18, d19, d20, d21, d22, d23, d24, d25, d26, d27, d28, d29, d30, d31, d32};
	int val[32] = {v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32};
	// pour les digitales spécifiées, on Assigne grace à Set1Digital(...)
	int NombreDigSpecifiee = 0;
	for(int iemeParam = 0; iemeParam < 32; iemeParam++)
	{	// non spécifiée (-1 valeur par défaut): break;
		if(which[iemeParam] < 0)
			break;
		// probleme : break
		if(which[iemeParam] >= NUMBER_DIG_OUTPUT)
		{	AfxMessageBox("Index de sortie Digitale >= NUMBER_DIG_OUTPUT !", MB_ICONEXCLAMATION);
			break;	}
		// sinon:
		NombreDigSpecifiee++;
		Set1Digital(which[iemeParam], val[iemeParam]);
	}
	unsigned int som = 0;
	for(unsigned int iDigOut = 0; iDigOut < NUMBER_DIG_OUTPUT; iDigOut++)
		som += m_DigChannel[iDigOut]?1:0;
return som;
}

/*
Assigne a la colonne les chanel 0 à 31 
avec un apriori de 0 pour les valeurs digitales non données 
Renvoye le nombre corespondant à la représentation 32 bit
*/
unsigned long Colonne::Set32Digital(Set32DigParam)
{
	if(this == NULL)
	{
		AfxMessageBox("Set_32_Digital : This Column Does NOT Exist !", MB_ICONERROR);
		return 0;
	}
	Set1Digital(0, d00);	Set1Digital(1, d01);	Set1Digital(2, d02);	Set1Digital(3, d03);
	Set1Digital(4, d04);	Set1Digital(5, d05);	Set1Digital(6, d06);	Set1Digital(7, d07);
	Set1Digital(8, d08);	Set1Digital(9, d09);	Set1Digital(10, d10);	Set1Digital(11, d11);
	Set1Digital(12, d12);	Set1Digital(13, d13);	Set1Digital(14, d14);	Set1Digital(15, d15);
	Set1Digital(16, d16);	Set1Digital(17, d17);	Set1Digital(18, d18);	Set1Digital(19, d19);
	Set1Digital(20, d20);	Set1Digital(21, d21);	Set1Digital(22, d22);	Set1Digital(23, d23);
	Set1Digital(24, d24);	Set1Digital(25, d25);	Set1Digital(26, d26);	Set1Digital(27, d27);
	Set1Digital(28, d28);	Set1Digital(29, d29);	Set1Digital(30, d30);	Set1Digital(31, d31);
return GetDigInt32bit();
}

void Colonne::Set1AnalogFromFunction_ofX(unsigned char  indexAnaOut, double Fy_ofx(double x), double FromX, double ToX, int NbrPoints, double WarpFirstPointTo /* = 0 */)
{
	// d'abord, on assigne une Wf
	if(Set1Analog(indexAnaOut, 0, 0) == 0)
		return;
	// puis:
//srybex 	m_AnaChannel[indexAnaOut]->SetFromFunction_ofX(FromX, ToX, NbrPoints, Fy_ofx, WarpFirstPointTo);
}
void Colonne::Set1AnalogFromFunction_ofX(unsigned char  indexAnaOut, CString Formula_Fofx, double FromX, double ToX, int NbrPoints, double WarpFirstPointTo /* = 0 */)
{
	// d'abord, on assigne une Wf
	if(Set1Analog(indexAnaOut, 0, 0) == 0)
		return;
	// puis:
//srybex 	m_AnaChannel[indexAnaOut]->SetFromFunction_ofX(FromX, ToX, NbrPoints, Formula_Fofx, WarpFirstPointTo);
}
void Colonne::Set1AnalogFromFunction_ofOtherAnalog(unsigned char indexAnaSlave, unsigned char indexAnaMaster, double Fy_ofy(double y_other), int NbrPoints)
{
	if(m_AnaChannel[indexAnaMaster] == NULL)
	{
		AfxMessageBox("Error 54164 :\r\n Set1AnalogFromFunction_ofOtherAnalog \r\n Master AnalogOut does not exist!", MB_ICONERROR);
		return;
	}
	// d'abord, on assigne une Wf
	if(Set1Analog(indexAnaSlave, 0, 0) == 0)
		return;
	// puis:
//srybex 	m_AnaChannel[indexAnaSlave]->SetFromFunction_ofOtherWfY(m_AnaChannel[indexAnaMaster], NbrPoints, Fy_ofy);
}
void Colonne::Set1AnalogFromFunction_ofOtherAnalog(unsigned char indexAnaSlave, unsigned char indexAnaMaster, CString Formula_Fofx, int NbrPoints)
{
	if(m_AnaChannel[indexAnaMaster] == NULL)
	{
		AfxMessageBox("Error 54164 :\r\n Set1AnalogFromFunction_ofOtherAnalog \r\n Master AnalogOut does not exist!", MB_ICONERROR);
		return;
	}
	// d'abord, on assigne une Wf
	if(Set1Analog(indexAnaSlave, 0, 0) == 0)
		return;
	// puis:
//srybex 	m_AnaChannel[indexAnaSlave]->SetFromFunction_ofOtherWfY(m_AnaChannel[indexAnaMaster], NbrPoints, Formula_Fofx);
}

/*
associe la sortie nAnaOut à la WaveForme wf
*/
void Colonne::AssignWaveFormeToAnalog(unsigned char  indexAnaOut, CWaveForme* p_wf)
{
	if(this == NULL)
	{
		AfxMessageBox("Set_Analog : This Column Does NOT Exist !", MB_ICONERROR);
		return;
	}
	if(p_wf == NULL)
	{
		AfxMessageBox("Set_Analog : This WaveForme Does NOT Exist !", MB_ICONERROR);
		return;
	}
	if(indexAnaOut >= NUMBER_ANA_OUTPUT)
	{
		Beep(100,500);
		return;
	}
	if(m_AnaChannel[indexAnaOut])
		delete m_AnaChannel[indexAnaOut];
	m_AnaChannel[indexAnaOut] = p_wf;
return;
}
/*
Assigne a la colonne les pentes definies par AnaDebut et AnaFin 
et Renvoye le nombre de WaveForme Créées pour l'occasion
si on passe le parametre DEFAULT_VALUE, on prend les valeurs du Port
*/
unsigned int Colonne::Set1Analog(unsigned char indexAnaOut, double AnaDebut, double AnaFin)
{	
	if(this == NULL)
	{
		AfxMessageBox("Set_1_Analog : This Column Does NOT Exist !", MB_ICONERROR);
		return 0;
	}
	unsigned int NombreWfCree = 0;
	if(indexAnaOut >= NUMBER_ANA_OUTPUT)
		return 0;																	 
	double debut = AnaDebut; 
	double fin = AnaFin;
	// doit on retrouver la valeur précédente ? //////////////////////////////////////////
	double valPrecedente = 0.0;															//
	if(debut == PRECEDENT_VALUE || fin == PRECEDENT_VALUE)								//
	{																					//
		if(m_pSequenceOwning) // si la colonne fait partie d'une séquence				//
		{																				//
			// qu'elle est la colonne précédente										//
			Colonne* pLaColPrec = NULL;													//
			int numeroThisCol = m_pSequenceOwning->GetColNumero(this);					//
			if(numeroThisCol <= 1)														//
				pLaColPrec = NULL;														//
			else																		//
				pLaColPrec = m_pSequenceOwning->GetColonne(numeroThisCol - 1);			//
			// s'il y a bien une colonne avant et qu'elle a une WaveForme[indexAna]		//
			if(pLaColPrec && pLaColPrec->m_AnaChannel[indexAnaOut])						//
			{																			//
				valPrecedente = pLaColPrec->GetLastPointWf(indexAnaOut).y;				//
				NombreWfCree += Set1Analog(indexAnaOut, valPrecedente, valPrecedente);	//
			}																			//
			else // sinon, valeur par défault											//
				valPrecedente = DEFAULT_VALUE;											//
		}																				//
		else // sinon, valeur par défault												//
			valPrecedente = DEFAULT_VALUE;												//
	}/////////////////////////////////////////////////////////////////////////////////////

	if(debut == PRECEDENT_VALUE) // d'abord ca, sachant que valPrecedente peut etre devenu = DEFAULT_VALUE
		debut = valPrecedente;
	if(debut == DEFAULT_VALUE) // puis
		debut = Sequence::GetDefaultParamAnaOut(indexAnaOut)->Get_Val();
	if(fin == PRECEDENT_VALUE) // d'abord ca
		fin = valPrecedente;
	if(fin == DEFAULT_VALUE) // puis
		fin = Sequence::GetDefaultParamAnaOut(indexAnaOut)->Get_Val();

	CWaveForme* pUneNouvelleWF = new CWaveForme(debut, fin); 
	AssignWaveFormeToAnalog( indexAnaOut , pUneNouvelleWF);	
	NombreWfCree++;
return NombreWfCree;
}

unsigned int Colonne::SetAnalogs_Which_Debut_Fin(WhichDebFin_32_Param)
{	if(this == NULL)
	{
		AfxMessageBox("SetAnalogs_Which_Debut_Fin : This Column Does NOT Exist !", MB_ICONERROR);
		return 0;
	}
	int which[32] = {Which1, Which2, Which3, Which4, Which5, Which6, Which7, Which8, Which9, Which10, Which11, Which12, Which13, Which14, Which15, Which16, Which17, Which18, Which19, Which20, Which21, Which22, Which23, Which24, Which25, Which26, Which27, Which28, Which29, Which30, Which31, Which32};
	double deb[32] = {Deb1, Deb2, Deb3, Deb4, Deb5, Deb6, Deb7, Deb8, Deb9, Deb10, Deb11, Deb12, Deb13, Deb14, Deb15, Deb16, Deb17, Deb18, Deb19, Deb20, Deb21, Deb22, Deb23, Deb24, Deb25, Deb26, Deb27, Deb28, Deb29, Deb30, Deb31, Deb32};
	double fin[32] = {Fin1, Fin2, Fin3, Fin4, Fin5, Fin6, Fin7, Fin8, Fin9, Fin10, Fin11, Fin12, Fin13, Fin14, Fin15, Fin16, Fin17, Fin18, Fin19, Fin20, Fin21, Fin22, Fin23, Fin24, Fin25, Fin26, Fin27, Fin28, Fin29, Fin30, Fin31, Fin32};
	unsigned int NombreWfCree = 0;
	// pour les analogiques spécifiées, on Assigne grace à Set1Analog(which, debut, fin)...
	int NombreAnaSpecifiee = 0;
	for(int iemeParam = 0; iemeParam < 32; iemeParam++)
	{
		if(which[iemeParam] < 0)
			break;
		if(which[iemeParam] >= NUMBER_ANA_OUTPUT)
		{	AfxMessageBox("Index de sortie Analogique >= NUMBER_ANA_OUTPUT !", MB_ICONEXCLAMATION);
			break;	}
		NombreAnaSpecifiee++;
		NombreWfCree += Set1Analog(which[iemeParam], deb[iemeParam], fin[iemeParam]);
	}
	// pour les non-spécifiée, on continue la valeur de la colonne précédente, si cette colonne fait partie d'une sequence
	// ou DEFAULT_VALUE si il n'y a pas de colonne avant
	for(int indexAna = 0; indexAna < NUMBER_ANA_OUTPUT; indexAna++)
	{	// on ne refait pas les spécifiées:///////////////////////////////////////
		bool AlreadyDone = false;												//
		for(int iemeParam = 0; iemeParam < NombreAnaSpecifiee; iemeParam++)		//
			if(indexAna == which[iemeParam])									//
				AlreadyDone = true;												//
		if(AlreadyDone)															//
			continue;															//
		//////////////////////////////////////////////////////////////////////////
		NombreWfCree += Set1Analog(indexAna, PRECEDENT_VALUE, PRECEDENT_VALUE);
	}

return NombreWfCree;
}

/*
Donne le nombre dont le représentation binaire est "les sorties 0 à 31"
*/
unsigned long Colonne::GetDigInt32bit()
{
	if(this == NULL)
	{
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "GetDigInt32bit", "This Column Does NOT Exist !", 1);
		return 0;
	}
	unsigned long hexa = 0;
	unsigned long puissanceDe2 = 1;
	for(unsigned char iChan = 0; iChan < NUMBER_DIG_OUTPUT_CARD_DIG_1; iChan++)
	{
		hexa += m_DigChannel[iChan] * puissanceDe2;
		puissanceDe2 *= 2;
	}
return hexa;
}

/*
Renvoye le réel qui correspond à la indexAnaOut et la iemePerCol periode de la colonne
Si il n'y a pas de WaveForme définie : renvoye le parametre correspondant à la sortie du "Port"
*/
double Colonne::GetAnaValue_InterpolatedLinear(unsigned char indexAnaOut, int iemePerCol)
{
	if(this == NULL)
	{	MessageBox(*((CFrameWnd*)AfxGetMainWnd()), "GetAnaValue_InterpolatedLinear", "This Column Does NOT Exist !", 1);
		return 0;	}
	if(m_AnaChannel[indexAnaOut] == NULL)
		return Sequence::GetDefaultParamAnaOut(indexAnaOut)->Get_Val() MILLI_VOLT;
	CWaveForme* p_LaWF = m_AnaChannel[indexAnaOut];

	// abscisse ou l'on va interpoller
	double ProportionWf = double(iemePerCol) / double(GetDuree() - 1);
	double AbscisseWf = p_LaWF->GetLength() * ProportionWf;
	
	double val = p_LaWF->InterpolateLinear(AbscisseWf);
	val *= 0.001;
	val = min(max(val, -10.0), 10.0);
return val;
}

CPointDbl Colonne::GetLastPointWf(unsigned char indexAnaOut)
{
	if(this && m_AnaChannel[indexAnaOut])
		return m_AnaChannel[indexAnaOut]->GetPoint(-1);
	else
		return CPointDbl(0.0,0.0);
}
CPointDbl Colonne::GetFirstPointWf(unsigned char indexAnaOut)
{
	if(this && m_AnaChannel[indexAnaOut])
		return m_AnaChannel[indexAnaOut]->GetPoint(0);
	else
		return CPointDbl(0.0,0.0);
}

/*
Donne le ieme (de 0 à 31) bit d'un unsigned long
*/
bool GetIemeBitFromInt32Bit(unsigned char iemeBit, unsigned long Lint32Bit)
{
	unsigned long hexa;
	unsigned long puissanceDe2 = 2;
	hexa = Lint32Bit;		 

	for(unsigned char iChan = 0; iChan < 32; iChan++)
	{
		unsigned long Bit = 0;
		if(puissanceDe2)
			// Bit est le reste de la division par puissanceDe2
			Bit = hexa % puissanceDe2;
		else // il faut faire gaffe à ca car 2^32 = 0, et oui.
			Bit = hexa;

		if(iChan == iemeBit)
			return (Bit ? true : false);
		
		hexa -= Bit;
		puissanceDe2 *= 2;
	}
return false;
}

bool SetIemeBitInsideInt32Bit(unsigned char iemeBit, unsigned long & Lint32Bit, bool NewState)
{
	unsigned long hexa;
	unsigned long puissanceDe2 = 2;
	hexa = Lint32Bit;		 

	for(unsigned char iChan = 0; iChan < 32; iChan++)
	{
		unsigned long Bit = 0;
		if(puissanceDe2)
			// Bit est le reste de la division par puissanceDe2
			Bit = hexa % puissanceDe2;
		else // il faut faire gaffe à ca car 2^32 = 0, et oui.
			Bit = hexa;

		if(iChan == iemeBit)
		{
			bool state = Bit ? true : false;
			// si l'état est different du NewEtat, on switch
			if((state && !NewState) || (!state && NewState))
				Lint32Bit = Lint32Bit ^ (puissanceDe2 / 2);
			return state;
		}
		hexa -= Bit;
		puissanceDe2 *= 2;
	}
return false;
}
/* 
Utile pour calculer des points intermediaires dans la séquence, comme pour la compression
Calcul la valeur sur une pente (qui va de From à To, sur un longueur Length)
à une distance Cut du debut de la pente.
*/
double IntermediateValue(double From, double To, double Length, double Cut)
{
	if(Length == 0)
		return To;
	if(Cut <= 0)
		return From;
	if(Cut > Length)
		return To;
	double inter = From + ((To - From) * Cut) / Length;
return inter;
}

/* 
Utile pour calculer Le moment auquel la pente de From à To (en Length) devient égale à ValeurInter
Peut renvoyer des temps négatif ou superieur à Length!
*/
double IntermediateTime(double From, double To, double Length, double ValeurInter)
{
	if(From == To)
		return 0.0;
	double Time = (From - ValeurInter) / (From - To) * Length;
return Time;
}

/*
Donne les parametre nécessaire à faire marcher le Monster aec 2 Photodode d'asservissement
Durée Colonne 1 et 2								, double& duree_Col1, double& duree_Col2
From To de la photodiode 1 , premiere colonne		, double& Col1_pHd1_From, double& Col1_pHd1_To
From To de la photodiode 1 , deuxieme colonne		, double& Col2_pHd1_From, double& Col2_pHd1_To
From To de la photodiode 2 , premiere colonne		, double& Col1_pHd2_From, double& Col1_pHd2_To
From To de la photodiode 2 , deuxieme colonne		, double& Col2_pHd2_From, double& Col2_pHd2_To)

*/
void SpecialMonster_With_2_PhotoDiode(double LaserFrom, double LaserTo, double LaserTimeTotal, double LeSeuil_Ph1_Ph2, double LeMultipl_Ph2
									  , bool& IlFaut2Colonnes_Ph1_Ph2, bool& LaPremiereColonneEstEnPh2
									  , double& duree_Col1, double& duree_Col2
									  , double& Col1_pHd1_From, double& Col1_pHd1_To
									  , double& Col2_pHd1_From, double& Col2_pHd1_To
									  , double& Col1_pHd2_From, double& Col1_pHd2_To
									  , double& Col2_pHd2_From, double& Col2_pHd2_To)
{
	// doit on couper en deux ? ? ?
	duree_Col1 = IntermediateTime(LaserFrom, LaserTo, LaserTimeTotal, LeSeuil_Ph1_Ph2);
	IlFaut2Colonnes_Ph1_Ph2 = duree_Col1 > 0 && duree_Col1 < LaserTimeTotal;
	// Est-ce que on commence par la premiere colonne en Ph2?
	LaPremiereColonneEstEnPh2 = (LaserFrom < LeSeuil_Ph1_Ph2) || (LaserFrom == LeSeuil_Ph1_Ph2 && LaserTo < LeSeuil_Ph1_Ph2);
	// duree colonne 1 et 2 si besoin est
	if(!IlFaut2Colonnes_Ph1_Ph2)
		duree_Col1 = LaserTimeTotal;
	duree_Col1 = max(min(duree_Col1, LaserTimeTotal), 0);
	duree_Col2 = max(min(LaserTimeTotal - duree_Col1, LaserTimeTotal), 0);
	// Valeur intermediaire en convention Ph1
	double LaserValInter = IlFaut2Colonnes_Ph1_Ph2 ? LeSeuil_Ph1_Ph2 : LaserTo;;
	// Les valeurs de consigne du laser 
	Col1_pHd1_From = LaserFrom	;
	Col1_pHd1_To   = LaserValInter;
	Col1_pHd2_From = Col1_pHd1_From	* LeMultipl_Ph2;
	Col1_pHd2_To   = Col1_pHd1_To	* LeMultipl_Ph2;
	Col2_pHd1_From = LaserValInter;
	Col2_pHd1_To   = LaserTo		;
	Col2_pHd2_From = Col2_pHd1_From	* LeMultipl_Ph2;
	Col2_pHd2_To   = Col2_pHd1_To	* LeMultipl_Ph2;
}














