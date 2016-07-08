#pragma once
#include "Parametre.h"
#include "FolderUtils.h"

#define DEFAULT_CONFIG_FILE_NAME  "Configuration.Rb2"
///////////////////////////////////////////////////////////////////////////////////////////////////
/////// 
/////// Je construit une class � partir de CTextFile pour faciliter le "save" des settings
/////// 
///////////////////////////////////////////////////////////////////////////////////////////////////

# define FOR_ALL_PARAMETERS_IN_THIS_SAVE_do(pSave)		POSITION posInSave = pSave->m_List_pParam.GetHeadPosition();	\
														int indexParamInThisSave = -1;									\
														while(posInSave)												\
														{	indexParamInThisSave++;										\
														CParametre* pLeParam;											\
														pLeParam = pSave->m_List_pParam.GetNext(posInSave);				\


# define FOR_ALL_SAVE_SETTING_DEFINED_do				POSITION posSave = CSettingSave::GetList_Static_AllSetting().GetHeadPosition();	\
														while(posSave)																	\
														{																				\
														CSettingSave* pLeSave;															\
														pLeSave = CSettingSave::GetList_Static_AllSetting().GetNext(posSave);			\

#define PARAM_INT_NON_TROUVE -271828
#define PARAM_FLOAT_NON_TROUVE -2.71828
#define PARAM_STRING_NON_TROUVE "PasTrouv�"

typedef CList <CSettingSave*,CSettingSave*> CList_pSave;
enum{StopAnnee, StopMois, StopJour, StopHeure, StopMinute, StopSeconde};

class CSettingSave : public CTextFile
{
public:
	CSettingSave(void);
	CSettingSave(CString Name, CString Extension, bool ContientTrucPouvantVarier = true);
	~CSettingSave(void);

private:
	// Une CListe (de pointeurs) de tous les SettingSave d�clar�s
	static CList_pSave Liste_Static_AllSettingSave;
	// le r�pertoire du .exe !
	static char m_DirExe[1000]; 
	//// dit qu'on a deja initalis� les parametre avec SetAllSettings pour ne pas le faire deux fois
	//bool DejaInit;
	//// dit si les param�tres ont �t� modifi�s pour �ventuellement pr�venir l'utilisateur qu'il peut les sauver
	//bool Modified;
	// contient le FileContent courant utilis�. ca �vite de lire plein de fois dans le fichier.Rb2
	CString m_AdresseFichierCourant;
	CString m_ContenuFichierCourant;

	CString m_Name;
	CString m_ExtensionFile;
//	CString m_Descritpion;
	
//	// Le numero de version de cette SettingSave, utile pour interpr�ter les donn�es d'un fichier de sauvegarde
//	m_Version

public:
	// definit le repertoire courant comme celui du .exe, ou il doit y avoir les fichier de Sauvegarde
	static void DefineCurrentDirectory_SaveDirectory()
	{::GetCurrentDirectory(sizeof(m_DirExe) - 1, m_DirExe);};
	static CString GetDirExe(){return m_DirExe;};
	static CList_pSave& GetList_Static_AllSetting(){return Liste_Static_AllSettingSave;};

	bool m_ContientTrucPouvantVarier; // dit s'il y a des parametre int�r�ssant a faire varier, pour un algo g�n�tique par exemple

	CString GetName(){return m_Name;};

	// Liste des parametres sp�cifiques a une instance de SettingSave
	// par exemple, pour sauver les parametres propres a une sequence seulement, dans un .seq
	CList_ParametrePointeur m_List_pParam;

	// sauve tous les Settings dans un fichier "filename"(=le chemin d'acc�s)
	void SaveSettings(CString& filename, CString extension = "");
	// cherche (dans le fichier sp�cifi�, dans le dossier du .exe) la valeur correpondant 
	// � "parametre" puis la retourne sous forme d'une CString
	// si on donne le dernier param, on peut aller chercher des valeurs associ�es au 
	// "parametre", apres la premiere valeur, et s�par�s par des virgules
	CString GetStringSettings(CString& filename, CString parametre, int iemeValeurVirgule = 0);
	// cherche (dans le fichier sp�cifi�, dans le dossier du .exe) la valeur correpondant 
	// � "parametre" puis la retourne sous forme d'un int
	// si on donne le dernier param, on peut aller chercher des valeurs associ�es au 
	// "parametre", apres la premiere valeur, et s�par�s par des virgules
	int GetIntSettings(CString& filename, CString parametre, int iemeValeurVirgule = 0);
	// pareil... mais avec Float
	double GetDoubleSettings(CString& filename, CString parametre, int iemeValeurVirgule = 0);
	// sauve tous les Settings dans le fichier sp�cifi�, dans le dossier OldSettings +DateHeure
	void SaveSettingsDate(CString& filename);
	// Restore tous les Settings a partir de filname. si on sp�cifie deux entier, on ne charge que les parametres dont le num�ro est entre ces 2 entiers inclus.
	CString SetAllSettings(bool TellWhichParamMiss = true);
	void SetAllSettings(CString& filename, bool TellWhichParamMiss = true);
	void SaveWinSettings(CString& filename);
	// restore la taille de fenetre stock�e dans le fichier � utiliser dans CMainFrame::PreCreateWindow
	// et renvoye l'�tat de la fenetre : "mini", "maxi".
	int SetWinSettings(CREATESTRUCT &TheCs , CString& filename);
	// d�truit tout les CParametre du SettingSave
	void ClearAllCParametre();
};

// renvoye la CString date
CString LaDate(int AfterWhatToStop = StopMinute);

