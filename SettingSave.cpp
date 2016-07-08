/*
Tout ca, c'est des fonctions qui visent à sauver les settings:
	-enregistrer les settings dans un fichier dans le repertoires du .exe
	-""""""""""""""""""""""""""""""""""""""""""""" un repertoires OldSettings, avec date heure...
	-trouver la valeur associée à une variable dans le fichier si il existe, sinon, valeur defaut
	-
*/
#include "stdafx.h"
#include "SettingSave.h"

enum{valVarie=-1, Val0, valFrom, valTo, valStep};
char CSettingSave::m_DirExe[1000];
CList_pSave CSettingSave::Liste_Static_AllSettingSave;

/*
constructeur
*/
CSettingSave::CSettingSave(void)
{	
	m_AdresseFichierCourant = "";
	m_ContenuFichierCourant = "";
	m_Name = "";
	m_ExtensionFile = "";
//	m_Descritpion = "";
	m_ContientTrucPouvantVarier = false;
//	AfxMessageBox("A \"SettingSave\" should be instanciated like this\nCSettingSave(CString Name, CString Extension, bool ContientTrucPouvantVarier = true)");
//	Liste_Static_AllSettingSave.AddTail(this);
}
CSettingSave::CSettingSave(CString Name, CString Extension, bool ContientTrucPouvantVarier /* = true */)
{	
	m_AdresseFichierCourant = "";
	m_ContenuFichierCourant = "";
	m_Name = Name;
	m_ExtensionFile = Extension;
	m_ContientTrucPouvantVarier = ContientTrucPouvantVarier;
//	m_Descritpion = Description == "" ? "No Description" : Description;
	Liste_Static_AllSettingSave.AddTail(this);
}
CSettingSave::~CSettingSave(void)
{
	POSITION pos = Liste_Static_AllSettingSave.Find(this);
	if(pos)
		Liste_Static_AllSettingSave.RemoveAt(pos);
}
/*
renvoye la CString date
*/
CString LaDate(int AfterWhatToStop)
{	
	char annee[128];
	char* jepointe=NULL;		//qui pointera vers un endroit de la chaine "date"
	time_t t; 		time(&t);	//la date en seconde depuis 1970
	char* ladate=ctime(&t);		//puis en adresse de debut d'une String
	jepointe=ladate; if(ladate!=NULL) {*(ladate+24)='\0';}
	int ind = sprintf(annee,"");
	strcat(annee,(jepointe+20));//prend l'annee
	ladate=ladate+3;//enleve le jour de la semaine
	//en déplacant le '\0', on peut selectionner là ou on arrete la precision de la date!
	jepointe=strchr(ladate,' ');  if(jepointe!=NULL) *jepointe = AfterWhatToStop == StopAnnee	? '\0' : '_';
	jepointe=strchr(ladate,' ');  if(jepointe!=NULL) *jepointe = AfterWhatToStop == StopMois	? '\0' : '_';
	jepointe=strchr(ladate,' ');  if(jepointe!=NULL) *jepointe = AfterWhatToStop == StopJour	? '\0' : '@';
	jepointe=strchr(ladate,':');  if(jepointe!=NULL) *jepointe = AfterWhatToStop == StopHeure	? '\0' : 'h';
	jepointe=strchr(ladate,':');  if(jepointe!=NULL) *jepointe = AfterWhatToStop == StopMinute	? '\0' : '_';
	jepointe=strrchr(ladate,' '); if(jepointe!=NULL) *jepointe = '\0';
	strcat(annee,ladate);//
	CString CStrDate(annee);
	if(0)
	{
		CStrDate.Replace("Jan","01Jan");	CStrDate.Replace("Feb","02Feb");	CStrDate.Replace("Mar","03Mar");
		CStrDate.Replace("Apr","04Apr");	CStrDate.Replace("May","05May");	CStrDate.Replace("Jun","06Jun");
		CStrDate.Replace("Jul","07Jul");	CStrDate.Replace("Aug","08Aug");	CStrDate.Replace("Sep","09Sep");
		CStrDate.Replace("Oct","10Oct");	CStrDate.Replace("Nov","11Nov");	CStrDate.Replace("Dec","12Dec");
		CStrDate.Replace("@","th@");
	}
	else
	{
		CStrDate.Replace("Jan","01");	CStrDate.Replace("Feb","02");	CStrDate.Replace("Mar","03");
		CStrDate.Replace("Apr","04");	CStrDate.Replace("May","05");	CStrDate.Replace("Jun","06");
		CStrDate.Replace("Jul","07");	CStrDate.Replace("Aug","08");	CStrDate.Replace("Sep","09");
		CStrDate.Replace("Oct","10");	CStrDate.Replace("Nov","11");	CStrDate.Replace("Dec","12");
	}
	return CStrDate;
}

/*
	-enregistre les settings dans un fichier dans le repertoire du .exe
*/
void CSettingSave::SaveSettings(CString& filename, CString extension /* = "" */)
{
	// Create the folder if it does not exist
	int finfoldername = filename.ReverseFind('\\');
	CString foldername = filename.Left(finfoldername);
	CFolderUtils::CreateFolder(foldername);

	// PS : Si filename = "" alors une boite de dialogue CFileDialog doit s'ouvrir...
	CString exttt = extension == "" ? m_ExtensionFile : extension;
    CTextFile tf(exttt,"\r\n");

	// si il n'y a pas ".ext" a la fin du filename, on l'ajoute
	if(filename.Find(CString(".") + m_ExtensionFile) != filename.GetLength() - m_ExtensionFile.GetLength() - 1)
		filename += CString(".") + m_ExtensionFile;

	CString ToFile, aLigne, aTemp;

	// en tête du fichier texte, la date /////////////////////////////
	ToFile.Format("\r\n\tsauvé le : %s\r\n\r\n"	,	LaDate());		//
										//////////////////////////////
	{aTemp.Format("*********************************************************************************\r\n", 0); ToFile += aTemp;}
	FOR_ALL_PARAMETERS_IN_THIS_SAVE_do(this)
		if( pLeParam && pLeParam->m_MustBeSaved )
		{
			aLigne = "";
			switch(pLeParam->m_Type)
			{
			case Entier:
				{aTemp.Format("%s =\t%d\t,%d\t"	,	pLeParam->m_NomSave	,	pLeParam->Get_Val_Int()		,	pLeParam->Varie	); 
				aLigne += aTemp;
				}
				break;
			case Double:
				{aTemp.Format("%s =\t%g\t,%d\t"	,	pLeParam->m_NomSave	,	pLeParam->Get_Val_Double()	,	pLeParam->Varie	); 
				aTemp.Replace("e+00","e"); // enlève les extra 00 de l'exposant
				aTemp.Replace("e-00","e-");
				aLigne += aTemp;
				}
				break;
			case String:
				{aTemp.Format("%s =\t%s\t,%d\t"	,	pLeParam->m_NomSave	,	pLeParam->Get_Val_String()	,	pLeParam->Varie	); 
				aLigne += aTemp;
				}
				break;
			}
			// si Varie alors, on sauve les trois parametres From To Step
			if(pLeParam->Varie)
				{
					if(pLeParam->m_Type == Entier)
					{
						{aTemp.Format(",%g,%g,%g\t"	,	pLeParam->m_From,	pLeParam->m_To, pLeParam->m_Step); aLigne += aTemp;}
					}
					if(pLeParam->m_Type == Double)
					{
						{aTemp.Format(",%g,%g,%g\t"	,	pLeParam->m_From,	pLeParam->m_To, pLeParam->m_Step); aLigne += aTemp;}
					}
				}
			// ou si m_MustBeSaved > 1 
			// Alors, on sauve les m_MustBeSaved-1 Valeurs qui suivent *pVal dans la mémoire 
			// ie: pVal pointe vers le début d'un tableau de taille m_MustBeSaved
			else if( pLeParam->m_MustBeSaved > 1 )
			{
				for(int iemeValeurTableau = 1; iemeValeurTableau < pLeParam->m_MustBeSaved; iemeValeurTableau++)
				{
					switch(pLeParam->m_Type)
					{
					case Entier:
						{aTemp.Format(",%d"		,	pLeParam->Get_Val_Int(iemeValeurTableau)); aLigne += aTemp;}
						break;
					case Double:
						{aTemp.Format(",%g"		,	pLeParam->Get_Val_Double(iemeValeurTableau)); aLigne += aTemp;}
						break;
					case String:
						{aTemp.Format(",%s"		,	pLeParam->Get_Val_String(iemeValeurTableau)); aLigne += aTemp;}
						break;
					}
				}
				{aTemp.Format("\t",0); aLigne += aTemp;}
			}
			// si on veut mettre les truc cote a cote et pas a la ligne  dans certain cas:
			if(pLeParam->m_NomSave.Find("S_", 0) != -1 && pLeParam->m_NomSave.Find("Deb", 0) != -1)
				{aTemp.Format("||\t",0); aLigne += aTemp;}
			else
				{aTemp.Format("\r\n",0); aLigne += aTemp;}	
			ToFile += aLigne;
		}
	}

	tf.WriteTextFile(filename, ToFile);
return;
}

/*
	-enregistre les settings dans un fichier dans le repertoire OldSettings 
	-crée ce répertoire s'il n'existe pas
	-ajoute la Date-Heure au nom du fichier
*/
void CSettingSave::SaveSettingsDate(CString& filename)
{
	char NewFichier[MAX_PATH];

	if(filename =="")
	{
		sprintf(NewFichier,"%s\\Rb2Manip_OldSettings\\config_","C:\\");
	}
	else
	{
		sprintf(NewFichier,"%s_config_",filename);
	}
    
	strcat(NewFichier,LaDate());
	strcat(NewFichier,".Rb2");
	SaveSettings(CString(NewFichier));
return;
}

/*
	-trouve la valeur associée à une variable dans le fichier si il existe et renvoye la CString
	 si on donne le dernier param, on peut aller chercher des valeurs associées au 
	 "parametre", apres la premiere valeur, et séparés par des virgules
*/
CString CSettingSave::GetStringSettings(CString& filename, CString parametre, int iemeValeurVirgule /* = 0 */)
{
	// si il n'y a pas ".ext" a la fin du filename, on l'ajoute
	if((filename.Find(CString(".") + m_ExtensionFile) != filename.GetLength() - m_ExtensionFile.GetLength() - 1)
		&& filename != "")
		filename += CString(".") + m_ExtensionFile;

	// si on a déja ouvert ce fichier, on lira directement le *m_ContenuFichierCourant déjà en mémoire
	// sinon, on le charge : ca devrait servir à accélérer le processus de "fouille" du .txt
	if(filename != m_AdresseFichierCourant || filename == "" || m_ContenuFichierCourant == "")
	{
		CTextFile tf(m_ExtensionFile,"\r\n");
		m_ContenuFichierCourant = "";
		// PS : Si filename = "" alors une boite de dialogue CFileDialog doit s'ouvrir...
		tf.ReadTextFile(filename, m_ContenuFichierCourant);
		m_AdresseFichierCourant = filename;
	}
	CString paramSansSpace(parametre);
	paramSansSpace.Replace(CString(" "),CString(""));
	paramSansSpace.Replace(CString("\t"),CString(""));
	CString resToken;
	// trouve la CString "paprametre    " sans les espaces
	int curPos = m_ContenuFichierCourant.Find(paramSansSpace);	
	if(curPos == -1)
		return PARAM_STRING_NON_TROUVE;

	if(curPos >= 0)
	{
        // tokenize prend les CString qui se trouve ENTRE les symboles qu'on lui précise
		resToken = m_ContenuFichierCourant.Tokenize(" \t\r\n",curPos); // trouve le resToken = "parametre"
		resToken = m_ContenuFichierCourant.Tokenize("\t\r\n",curPos); // trouve le resToken = "="
		resToken = m_ContenuFichierCourant.Tokenize(",\t\r\n",curPos); // trouve la valeur 

		if(iemeValeurVirgule)
		{
			resToken = m_ContenuFichierCourant.Tokenize(",\t\r\n",curPos); // trouve la valeur de "Varie", qu'on saute
			if(iemeValeurVirgule == valVarie)
				return resToken; // si on a demandé la Varie, on return resToken ici
			for(int i = 1; i <= iemeValeurVirgule; i++)
			{
				resToken = m_ContenuFichierCourant.Tokenize(",\t\r\n",curPos); // trouve la valeur suivante
			}
		}
	}
return resToken;
}
/*
	-trouve la valeur associée à une variable dans le fichier si il existe et renvoye un int
	 si on donne le dernier param, on peut aller chercher des valeurs associées au 
	 "parametre", apres la premiere valeur, et séparés par des virgules
*/
int CSettingSave::GetIntSettings(CString& filename, CString parametre, int iemeValeurVirgule /* = 0 */)
{
	char  *stopstring;
	CString resultat = GetStringSettings(filename, parametre, iemeValeurVirgule);
	if(resultat == PARAM_STRING_NON_TROUVE)
		return PARAM_INT_NON_TROUVE;
	int lavaleur=strtol(resultat, &stopstring,10);
return lavaleur;
}/*
	pareil... mais avec Float
*/
double CSettingSave::GetDoubleSettings(CString& filename, CString parametre, int iemeValeurVirgule /* = 0 */)
{
	char  *stopstring;
	CString resultat = GetStringSettings(filename, parametre, iemeValeurVirgule);
	if(resultat == PARAM_STRING_NON_TROUVE)
		return PARAM_FLOAT_NON_TROUVE;
	double lavaleur=strtod(resultat, &stopstring);
return lavaleur;
}
/*
	-restore tous les Settings à partir du fichier
	si on spécifie deux entier, on ne charge que les parametres dont le numéro est entre
	ces 2 entiers inclus.

*/
CString CSettingSave::SetAllSettings(bool TellWhichParamMiss /* = true */)
{
	CString UneStr("");
	SetAllSettings(UneStr, TellWhichParamMiss);
	return UneStr;
}
void CSettingSave::SetAllSettings(CString& filename, bool TellWhichParamMiss /* = true */)
{
	CString ParamNotFounded = "";

	FOR_ALL_PARAMETERS_IN_THIS_SAVE_do(this)
		if( pLeParam && pLeParam->m_MustBeSaved )
		{
			// si un nom de fichier n'est toujours pas donné au deuxieme parametre, on peut sortir: ca ne sert à rien de demander 1000 fois un nom de fichier
			if(indexParamInThisSave > 0 && filename == "")
				break;
			int val_INT;double val_DBL;CString val_STR;
			CString NomParam = pLeParam->m_NomSave;
			switch(pLeParam->m_Type)
			{
			case Entier:
				val_INT = GetIntSettings(filename	, NomParam);
				if(val_INT == PARAM_INT_NON_TROUVE)
				{	pLeParam->Varie = 0;	
					if(pLeParam->m_MightComplainIfMissing)
						ParamNotFounded += NomParam + "\n"; 
					break;	}
				pLeParam->Set_Val(   val_INT 	)	;
				// si Varie alors, on restore les trois parametres From To Step
				pLeParam->Varie	= GetIntSettings(filename	,NomParam, valVarie)	;
				break;
			case Double:
				val_DBL = GetDoubleSettings(filename, NomParam);
				if(val_DBL == PARAM_FLOAT_NON_TROUVE)
				{	pLeParam->Varie = 0;	
					if(pLeParam->m_MightComplainIfMissing)
					ParamNotFounded += NomParam + "\n"; 
					break;	}
				pLeParam->Set_Val(   val_DBL 	)	;
				// si Varie alors, on restore les trois parametres From To Step
				pLeParam->Varie	= GetIntSettings(filename	,NomParam, valVarie)	;
				break;
			case String:
				val_STR = GetStringSettings(filename, NomParam);
				if(val_STR == PARAM_STRING_NON_TROUVE)
				{	pLeParam->Varie = 0;	
					if(pLeParam->m_MightComplainIfMissing)
					ParamNotFounded += NomParam + "\n"; 
					break;	}
				pLeParam->Set_Val(   val_STR 	)	;
				// si Varie alors, on restore les trois parametres From To Step
				pLeParam->Varie	= GetIntSettings(filename	,NomParam, valVarie)	;
				break;
			}
			if(pLeParam->Varie)
			{
				if(pLeParam->m_Type == Entier)
				{
					pLeParam->m_From	= double(GetIntSettings(filename	,NomParam, valFrom))	;
					pLeParam->m_To		= double(GetIntSettings(filename	,NomParam, valTo))	;
					pLeParam->m_Step	= double(GetIntSettings(filename	,NomParam, valStep))	;
				}
				if(pLeParam->m_Type == Double)
				{
					pLeParam->m_From	= GetDoubleSettings(filename	,NomParam, valFrom)	;
					pLeParam->m_To		= GetDoubleSettings(filename	,NomParam, valTo)	;
					pLeParam->m_Step	= GetDoubleSettings(filename	,NomParam, valStep)	;
				}
			}
			// ou si m_MustBeSaved > 1 
			// Alors, on restore les m_MustBeSaved-1 Valeurs qui suivent *pVal dans la mémoire 
			// ie: pVal pointe vers le début d'un tableau de taille m_MustBeSaved
			else if( pLeParam->m_MustBeSaved > 1)
			{
				for(int iemeValeurTableau = 1; iemeValeurTableau < pLeParam->m_MustBeSaved; iemeValeurTableau++)
				{
					switch(pLeParam->m_Type)
					{
					case Entier:
						{int IntVal		   = GetIntSettings(filename, NomParam, iemeValeurTableau);
						pLeParam->Set_Val( IntVal, iemeValeurTableau );
						break;}
					case Double:
						{double FloatVal   = GetDoubleSettings(filename, NomParam, iemeValeurTableau);
						pLeParam->Set_Val( FloatVal, iemeValeurTableau );
						break;}
					case String:
						{CString StringVal = GetStringSettings(filename, NomParam, iemeValeurTableau);
						pLeParam->Set_Val( StringVal, iemeValeurTableau );
						break;}
					}
				}
			}
		}
	}
	if(TellWhichParamMiss && ParamNotFounded != "")
		MessageBox(*((CFrameWnd*)AfxGetMainWnd()), ParamNotFounded, "Parameters Not Founded in File", MB_ICONEXCLAMATION);
return;
}
void CSettingSave::SaveWinSettings(CString& filename)
{
	// Create the folder if it does not exist
	int finfoldername = filename.ReverseFind('\\');
	CString foldername = filename.Left(finfoldername);
	CFolderUtils::CreateFolder(foldername);

	// PS : Si filename = "" alors une boite de dialogue CFileDialog doit s'ouvrir...
	CString exttt = "Rb2";
    CTextFile tf(exttt,"\r\n");

	CString ToFile, aLigne, aTemp;

	// en tête du fichier texte, la date /////////////////////////////
	ToFile.Format("\r\n\tsauvé le : %s\r\n\r\n"	,	LaDate());		//
										//////////////////////////////
	{aTemp.Format("*********************************************************************************\r\n", 0); ToFile += aTemp;}
	WINDOWPLACEMENT wp;
	(*((CFrameWnd*)AfxGetMainWnd())).GetWindowPlacement(&wp);
	CRect r(wp.rcNormalPosition);
	
	{aTemp.Format("WINDOWS_STATE\t=\t%d\r\n"	,	wp.showCmd)	; ToFile += aTemp;}	
	{aTemp.Format("WINDOWS_POS_X\t=\t%d\r\n"	,	r.left)		; ToFile += aTemp;}	
	{aTemp.Format("WINDOWS_POS_Y\t=\t%d\r\n"	,	r.top)		; ToFile += aTemp;}	
	{aTemp.Format("WINDOWS_HEIGHT\t=\t%d\r\n"	,	r.Height())	; ToFile += aTemp;}	
	{aTemp.Format("WINDOWS_WIDHT\t=\t%d\r\n"	,	r.Width())	; ToFile += aTemp;}	

	tf.WriteTextFile(filename, ToFile);
}
/*
	-restore la taille de fenetre stockée dans le fichier à utiliser dans CMainFrame::PreCreateWindow
*/
int CSettingSave::SetWinSettings(CREATESTRUCT& TheCs , CString& filename)
{
  // gère la fenetre dans CMainFrame::PreCreateWindow
	TheCs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	TheCs.lpszClass = AfxRegisterWndClass(0);

  // on restore la taille de fenetre stocké dans le fichier "filename"
	if(GetIntSettings(filename,"WINDOWS_POS_X") == PARAM_INT_NON_TROUVE)	{		TheCs.x=0;	}
	else{	TheCs.x  = GetIntSettings(filename,"WINDOWS_POS_X");	}
	if(GetIntSettings(filename,"WINDOWS_POS_Y") == PARAM_INT_NON_TROUVE)	{		TheCs.y=0;	}
	else{	TheCs.y  = GetIntSettings(filename,"WINDOWS_POS_Y");	}
	if(GetIntSettings(filename,"WINDOWS_HEIGHT") == PARAM_INT_NON_TROUVE)	{		TheCs.cx=800;	}
	else{	TheCs.cx = GetIntSettings(filename,"WINDOWS_WIDHT");	}
	if(GetIntSettings(filename,"WINDOWS_HEIGHT") == PARAM_INT_NON_TROUVE)	{		TheCs.cy=600;	}
	else{	TheCs.cy = GetIntSettings(filename,"WINDOWS_HEIGHT");	}

	if(GetIntSettings(filename,"WINDOWS_STATE") == PARAM_INT_NON_TROUVE)	{	return SW_MAXIMIZE;	}
	else	{	return GetIntSettings(filename,"WINDOWS_STATE");	}
}

void CSettingSave::ClearAllCParametre()
{
	while (!m_List_pParam.IsEmpty())
	{
		CParametre* pTheParam = m_List_pParam.GetHead();
		delete pTheParam;
		pTheParam = NULL;
	}
}
