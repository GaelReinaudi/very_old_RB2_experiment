#pragma once

class CAlgorithmeGenetique
{
public:
	CAlgorithmeGenetique(void);
	CAlgorithmeGenetique(CString NewIp);
	~CAlgorithmeGenetique(void);
	// une eventuelle EditBox pour afficher les resultats des fonction de l'API
	CEdit *m_pEditXPSResult, *m_pEditXPSEvent, *m_pEditXPSAction;
	// une eventuelle Combo pour afficher tous les fichier PVT du repertoire
	CComboBox* m_pComboPVT;
	// une eventuelle Combo pour afficher tous les GroupName
	CComboBox* m_pComboGroupName;

};
