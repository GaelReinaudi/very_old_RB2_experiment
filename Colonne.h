#pragma once

#include "WaveForme.h"

#define NUMBER_CARDS_DIG_OUTPUT 1
#define NUMBER_CARDS_ANA_OUTPUT 2
#define NUMBER_DIG_OUTPUT_CARD_DIG_1 32
//2011
#define NUMBER_ANA_OUTPUT_CARD_ANA_1_REAL 8
#define NUMBER_ANA_OUTPUT_CARD_ANA_1 8
#define NUMBER_ANA_OUTPUT_CARD_ANA_2 8

#define NUMBER_DIG_OUTPUT NUMBER_DIG_OUTPUT_CARD_DIG_1
#define NUMBER_ANA_OUTPUT (NUMBER_ANA_OUTPUT_CARD_ANA_1 + NUMBER_ANA_OUTPUT_CARD_ANA_2)

#define DEFAULT_VALUE		-654321
#define PRECEDENT_VALUE		-456123
#define CURRENT_VALUE		-666123
class Sequence;

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////        Colonne        ////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
#define WhichDebFin_32_ParamH int Which1=-1, double Deb1=-1, double Fin1=-1, int Which2=-1, double Deb2=-1, double Fin2=-1, int Which3=-1, double Deb3=-1, double Fin3=-1, int Which4=-1, double Deb4=-1, double Fin4=-1, int Which5=-1, double Deb5=-1, double Fin5=-1, int Which6=-1, double Deb6=-1, double Fin6=-1, int Which7=-1, double Deb7=-1, double Fin7=-1, int Which8=-1, double Deb8=-1, double Fin8=-1, int Which9=-1, double Deb9=-1, double Fin9=-1, int Which10=-1, double Deb10=-1, double Fin10=-1, int Which11=-1, double Deb11=-1, double Fin11=-1, int Which12=-1, double Deb12=-1, double Fin12=-1, int Which13=-1, double Deb13=-1, double Fin13=-1, int Which14=-1, double Deb14=-1, double Fin14=-1, int Which15=-1, double Deb15=-1, double Fin15=-1, int Which16=-1, double Deb16=-1, double Fin16=-1, int Which17=-1, double Deb17=-1, double Fin17=-1, int Which18=-1, double Deb18=-1, double Fin18=-1, int Which19=-1, double Deb19=-1, double Fin19=-1, int Which20=-1, double Deb20=-1, double Fin20=-1, int Which21=-1, double Deb21=-1, double Fin21=-1, int Which22=-1, double Deb22=-1, double Fin22=-1, int Which23=-1, double Deb23=-1, double Fin23=-1, int Which24=-1, double Deb24=-1, double Fin24=-1, int Which25=-1, double Deb25=-1, double Fin25=-1, int Which26=-1, double Deb26=-1, double Fin26=-1, int Which27=-1, double Deb27=-1, double Fin27=-1, int Which28=-1, double Deb28=-1, double Fin28=-1, int Which29=-1, double Deb29=-1, double Fin29=-1, int Which30=-1, double Deb30=-1, double Fin30=-1, int Which31=-1, double Deb31=-1, double Fin31=-1, int Which32=-1, double Deb32=-1, double Fin32=-1
#define WhichDebFin_32_Param int Which1, double Deb1, double Fin1, int Which2, double Deb2, double Fin2, int Which3, double Deb3, double Fin3, int Which4, double Deb4, double Fin4, int Which5, double Deb5, double Fin5, int Which6, double Deb6, double Fin6, int Which7, double Deb7, double Fin7, int Which8, double Deb8, double Fin8, int Which9, double Deb9, double Fin9, int Which10, double Deb10, double Fin10, int Which11, double Deb11, double Fin11, int Which12, double Deb12, double Fin12, int Which13, double Deb13, double Fin13, int Which14, double Deb14, double Fin14, int Which15, double Deb15, double Fin15, int Which16, double Deb16, double Fin16, int Which17, double Deb17, double Fin17, int Which18, double Deb18, double Fin18, int Which19, double Deb19, double Fin19, int Which20, double Deb20, double Fin20, int Which21, double Deb21, double Fin21, int Which22, double Deb22, double Fin22, int Which23, double Deb23, double Fin23, int Which24, double Deb24, double Fin24, int Which25, double Deb25, double Fin25, int Which26, double Deb26, double Fin26, int Which27, double Deb27, double Fin27, int Which28, double Deb28, double Fin28, int Which29, double Deb29, double Fin29, int Which30, double Deb30, double Fin30, int Which31, double Deb31, double Fin31, int Which32, double Deb32, double Fin32
#define WhichDebFin_32_ParamF Which1, Deb1, Fin1, Which2, Deb2, Fin2, Which3, Deb3, Fin3, Which4, Deb4, Fin4, Which5, Deb5, Fin5, Which6, Deb6, Fin6, Which7, Deb7, Fin7, Which8, Deb8, Fin8, Which9, Deb9, Fin9, Which10, Deb10, Fin10, Which11, Deb11, Fin11, Which12, Deb12, Fin12, Which13, Deb13, Fin13, Which14, Deb14, Fin14, Which15, Deb15, Fin15, Which16, Deb16, Fin16, Which17, Deb17, Fin17, Which18, Deb18, Fin18, Which19, Deb19, Fin19, Which20, Deb20, Fin20, Which21, Deb21, Fin21, Which22, Deb22, Fin22, Which23, Deb23, Fin23, Which24, Deb24, Fin24, Which25, Deb25, Fin25, Which26, Deb26, Fin26, Which27, Deb27, Fin27, Which28, Deb28, Fin28, Which29, Deb29, Fin29, Which30, Deb30, Fin30, Which31, Deb31, Fin31, Which32, Deb32, Fin32
#define Set32DigParamH int d00=0 ,int d01=0 ,int d02=0 ,int d03=0 ,int d04=0 ,int d05=0 ,int d06=0 ,int d07=0 ,int d08=0 ,int d09=0 ,int d10=0,int d11=0,int d12=0,int d13=0,int d14=0,int d15=0 ,int d16=0,int d17=0,int d18=0,int d19=0,int d20=0,int d21=0,int d22=0,int d23=0 ,int d24=0,int d25=0,int d26=0,int d27=0,int d28=0,int d29=0,int d30=0,int d31=0 
#define Set32DigParam int d00,int d01,int d02,int d03,int d04,int d05,int d06,int d07,int d08,int d09,int d10,int d11,int d12,int d13,int d14,int d15,int d16,int d17,int d18,int d19,int d20,int d21,int d22,int d23,int d24,int d25,int d26,int d27,int d28,int d29,int d30,int d31
#define Set32DigParamF d00,d01,d02,d03,d04,d05,d06,d07,d08,d09,d10,d11,d12,d13,d14,d15,d16,d17,d18,d19,d20,d21,d22,d23,d24,d25,d26,d27,d28,d29,d30,d31
#define WhichDigValH int d1 =-1, int v1 =-1, int d2 =-1, int v2 =-1, int d3 =-1, int v3 =-1, int d4 =-1, int v4 =-1, int d5 =-1, int v5 =-1, int d6 =-1, int v6 =-1, int d7 =-1, int v7 =-1, int d8 =-1, int v8 =-1, int d9 =-1, int v9 =-1, int d10=-1, int v10=-1, int d11=-1, int v11=-1, int d12=-1, int v12=-1, int d13=-1, int v13=-1, int d14=-1, int v14=-1, int d15=-1, int v15=-1, int d16=-1, int v16=-1, int d17=-1, int v17=-1, int d18=-1, int v18=-1, int d19=-1, int v19=-1, int d20=-1, int v20=-1, int d21=-1, int v21=-1, int d22=-1, int v22=-1, int d23=-1, int v23=-1, int d24=-1, int v24=-1, int d25=-1, int v25=-1, int d26=-1, int v26=-1, int d27=-1, int v27=-1, int d28=-1, int v28=-1, int d29=-1, int v29=-1, int d30=-1, int v30=-1, int d31=-1, int v31=-1, int d32=-1, int v32=-1
#define WhichDigVal int d1, int v1, int d2, int v2, int d3, int v3, int d4, int v4, int d5, int v5, int d6, int v6, int d7, int v7, int d8, int v8, int d9, int v9, int d10, int v10, int d11, int v11, int d12, int v12, int d13, int v13, int d14, int v14, int d15, int v15, int d16, int v16, int d17, int v17, int d18, int v18, int d19, int v19, int d20, int v20, int d21, int v21, int d22, int v22, int d23, int v23, int d24, int v24, int d25, int v25, int d26, int v26, int d27, int v27, int d28, int v28, int d29, int v29, int d30, int v30, int d31, int v31, int d32, int v32
#define WhichDigValF d1, v1, d2, v2, d3, v3, d4, v4, d5, v5, d6, v6, d7, v7, d8, v8, d9, v9, d10, v10, d11, v11, d12, v12, d13, v13, d14, v14, d15, v15, d16, v16, d17, v17, d18, v18, d19, v19, d20, v20, d21, v21, d22, v22, d23, v23, d24, v24, d25, v25, d26, v26, d27, v27, d28, v28, d29, v29, d30, v30, d31, v31, d32, v32

class Colonne
{
public:
	Colonne(void);							// constructeurs
	Colonne(CString UnNom, unsigned int UnNombrePeriode); // constructeurs
	Colonne(const Colonne& UneCol);			// constructeurs
	Colonne(const Colonne& UneCol, CString UnNom, unsigned int UnNombrePeriode = 0);// constructeurs avec nouveau nom et duartion
	~Colonne(void);

	static unsigned int NumberColonne;		// membre statique :le nombre de colonnes instanciées

	CString m_NameCol;						// une colonne peut avoir un nom
	Sequence* m_pSequenceOwning;			// éventuellement, la séquence qui contient cette colonne
private:
	// elle dure un certain temps
	int m_Duree;
	// la colonne est-elle activée? ie: sur quel duree est elle "jouée"
	int m_ActiveDuree;
	// précise quand cela a un sens, que la colonne débute un groupe de colonne dans une séquence
	bool m_DebutDunGroupe;				
	// précise quand cela a un sens, que la limite gauche de la colonne est un Split dans une séquence
	bool m_LimiteGaucheEstUnSplit;				
	// précise quand cela a un sens, que la limite droite de la colonne est un Split dans une séquence
	bool m_LimiteDroiteEstUnSplit;				

//	// chaque AnaOut peut éventuellement etre définie par une autre !
//	CRelationAnalog m_pRelation[NUMBER_ANA_OUTPUT];

public:
	// valeurs prisent par les sorties Digitales
	bool m_DigChannel[NUMBER_DIG_OUTPUT];
	// pointeurs vers les wave-formes qui doivent etre executées sur chaque AnaChannel 
	CWaveForme* m_AnaChannel[NUMBER_ANA_OUTPUT];
	// Pointeur vers des parametres qui seront créés pour définir une colonne dans une séquence
	CParametre* m_CParamDuration;
	CParametre* m_CParamDig32b;
	CParametre* m_CParamValDeb[NUMBER_ANA_OUTPUT];
	CParametre* m_CParamValFin[NUMBER_ANA_OUTPUT];

public:
	// Set la durée de la colonne , ainsi que la durée jouée si on la précise. Par défault toute la colonne est activée
	// renvoye la duree effectivement active
	int SetDuree(int NewDuree, int NewActiveDuree = -1)
	{	m_Duree = NewDuree; 
		int actives = NewActiveDuree == -1 ? m_Duree : min(m_Duree, NewActiveDuree);
		m_ActiveDuree = (actives); return actives;	};
	int SetActiveDuree(int NewActiveDuree = -1)
	{	int actives = NewActiveDuree == -1 ? m_Duree : min(m_Duree, NewActiveDuree);
		m_ActiveDuree = (actives); return actives;	};
	int GetDuree(){return m_Duree;};
	int GetActiveDuree(){return m_ActiveDuree;};
	// reset toutes les sorties digitales à 0 et les waveformes des analogiques à numero 0
	void ResetCol(); 
	// met la sortie digitale indexDigOut à la valeur TheState, ou 0 par defaut. Renvoye la somme de tous les bit : compris entre 0 et 32
	unsigned char Set1Digital(unsigned char indexDigOut, int TheState);
	// Assigne a la colonne les chanel 0 à 31 avec un apriori de 0 pour les valeurs non données et Renvoye le nombre corespondant à la représentation 32 bit
	unsigned long Set32Digital(Set32DigParamH);
	// Assigne a la indexAna la pente definie par AnaDebut et AnaFin et Renvoye le nombre de WaveForme Créées pour l'occasion
	unsigned int Set1Analog(unsigned char  indexAnaOut, double AnaDebut, double AnaFin);
	// Assigne a la colonne les pentes definies par Debut et Fin  de la voie analogique Which	// on peut passer jusquèà 32 parametres
	// les Analogiques dont on ne spécifie pas continuent la colonne précédente
	// Renvoye le nombre de WaveForme Créées pour l'occasion
	unsigned int SetAnalogs_Which_Debut_Fin(WhichDebFin_32_ParamH);
	// Assigne a la colonne les valeurs on peut passer jusqu'à 32 parametres
	// les Digitales dont on ne spécifie pas la valeur continuent la colonne précédente
	unsigned int SetDigitals_DefaultPrecedentColonne(WhichDigValH);
	// Assigne a la colonne les valeurs on peut passer jusqu'à 32 parametres
	// les Analogiques qu'on ne spécifie pas gardent leur valeurs
	unsigned int SetDigitals(WhichDigValH);

	// Assigne une Sortie Analogique suivant une fonction de ses coordonnées x : y = F(x)
	// on précise FromX et ToX, ainsi que le nombre de points de défintion de cette fonction (plus il y a de points, plus une courbe sera bien définie !)
	void Set1AnalogFromFunction_ofX(unsigned char  indexAnaOut, double Fy_ofx(double x), double FromX, double ToX, int NbrPoints, double WarpFirstPointTo = 0);
	void Set1AnalogFromFunction_ofX(unsigned char  indexAnaOut, CString Formula_Fofx, double FromX, double ToX, int NbrPoints, double WarpFirstPointTo = 0);
	// Assigne une Sortie Analogique suivant une fonction des valeur d'une autre sortie : y = F(y_autre)
	// on précise le nombre de points de défintion de cette fonction (plus il y a de points, plus une courbe sera bien définie !)
	void Set1AnalogFromFunction_ofOtherAnalog(unsigned char indexAnaSlave, unsigned char  indexAnaMaster, double Fy_ofy(double y_other), int NbrPoints);
	void Set1AnalogFromFunction_ofOtherAnalog(unsigned char indexAnaSlave, unsigned char  indexAnaMaster, CString Formula_Fofx, int NbrPoints);

	unsigned long GetDigInt32bit();			// Renvoye le nombre dont le représentation binaire est "les sorties 0 à 31"
	// Renvoye le réel qui correspond à la indexAnaOut et la iemePerCol periode de la colonne.
	double GetAnaValue_InterpolatedLinear(unsigned char indexAnaOut, int iemePerCol);
	CPointDbl GetLastPointWf(unsigned char indexAnaOut); // Renvoye le dernier point de la colonne 
	CPointDbl GetFirstPointWf(unsigned char indexAnaOut); // Renvoye le premier point de la colonne 

	// Précise quelque notion relative à l'insertion de la colonne dans une séquence
	// précise dans une séquence, que la colonne débute un groupe de colonne 
	void Set_DebutDunGroupe(bool tobeornottobe){m_DebutDunGroupe = tobeornottobe;};
	// précise dans une séquence, que la limite gauche de la colonne est un Split 
	void Set_LimiteGaucheEstUnSplit(bool estSpliteeAGauche){m_LimiteGaucheEstUnSplit = estSpliteeAGauche;};
	bool Is_LimiteGaucheUnSplit(){return m_LimiteGaucheEstUnSplit;};
	// précise dans une séquence, que la limite droite de la colonne est un Split 
	void Set_LimiteDroiteEstUnSplit(bool estSpliteeADroite){m_LimiteDroiteEstUnSplit = estSpliteeADroite;};
	bool Is_LimiteDroiteUnSplit(){return m_LimiteDroiteEstUnSplit;};

//private:
	void AssignWaveFormeToAnalog(unsigned char  indexAnaOut, CWaveForme* p_wf);// associe la sortie indexAnaOut à la WaveForme p_wf

};