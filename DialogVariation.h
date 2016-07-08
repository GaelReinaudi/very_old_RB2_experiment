#pragma once
#include "resource.h"
#include "Parametre.h"
#include "Variation.h"
//#include "c:\program files\national instruments\measurementstudiovs2003\vcnet\include\ninumedit.h"

// Bo�te de dialogue CDialogVariation

class CDialogVariation : public CDialog
{
	DECLARE_DYNAMIC(CDialogVariation)

public: 
	// constructeur standard
	CDialogVariation(CWnd* pParent = NULL);  
//	// constructeur 
//	CDialogVariation(CVariation* pLaVar, CString NomVar, int* pLeParametre, int ValDebut, int ValFin, int LePas, CWnd* pParent = NULL);  
	// constructeur 
	CDialogVariation(CVariation* pLaVar, CParametre* LeParam, CWnd* pParent = NULL);
	// pointeur vers la variation associ�e � DialogVariation.
	CVariation* m_pLaVar;

	virtual ~CDialogVariation();
	
	virtual BOOL OnInitDialog();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_DIALOG_VARIATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	// �ventuellement un pointeur vers le CParametre auquel la boite de dialog se rapporte
	CParametre* m_pLeParam;
//	// Le nom qui d�crit le param
//	CString m_NomParametre;
//	// Le pointeur vers sa valeur
//	void* m_pValeur;
	// les donn�es de la boite
	double m_VarEditFrom;
	double m_VarEditTo;
	double m_VarEditStep;
	// donne � quelle position dans l'ordre des variations le CParam�tre est ajout�
	int m_Var_Varie;

public:
	DECLARE_EVENTSINK_MAP()
	void ValueChangedVariationNumeditFrom( VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedVariationNumeditTo(   VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
	void ValueChangedVariationNumeditStep( VARIANT* Value, VARIANT* PreviousValue, BOOL OutOfRange);
public:	
	// Remplit les champs From To Step du CParametre
	CParametre* ActualiseCParametreValues(void);
	// Update Check boxes
	void UpdateCheckBoxesVarie(int OrdreSuivantVariant = 0 );
	// Update Static Text
	void UpdateStaticText();
	// verifie les donn�es pour �viter des truc absurde comme de 10 � 0 par pas de 1
	// et corrige si possible ---->>>> par pas de  -1  !! 
	void Verifie();

	DECLARE_MESSAGE_MAP()
	void ValueChangedVarDialogCheck01(BOOL Value);
	void ValueChangedVarDialogCheck02(BOOL Value);
	void ValueChangedVarDialogCheck03(BOOL Value);
	void ValueChangedVarDialogCheck04(BOOL Value);
	void ValueChangedVarDialogCheck05(BOOL Value);
	CNiNumEdit m_EditFrom;
	CNiNumEdit m_EditTo;
	CNiNumEdit m_EditStep;
};
