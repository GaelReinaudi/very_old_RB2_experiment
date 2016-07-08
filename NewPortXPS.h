#pragma once
#include "./XPS_C8_drivers.h"
#include "WaveForme.h"
#include <afxinet.h> // pour la connection CInternetSession suivi de CFtpConnection
#include <io.h>
#include "NiIncludes.h"

#define MAX_POSITIONER 8
#define PVT_FILE_DOESNT_EXIST CPointDbl(1.1, 1.1)
#define PVT_VERIFY_ERROR CPointDbl(2.2, 2.2)
#define PVT_VERIFY_OK CPointDbl(3.3, 3.3)
#define GATHERED_DATA_FILE_NAME "Gathering.dat"
#define PVT_NOT_SAVED_FILE_NAME (CString("_Not_Saved_Traj.trj"))
#define DEFAULT_LOCAL_FOLDER_RECORD_POSITION_VITESSE "C:\\Rb2Manip_XPSRecords\\"
#define For_All_Positioner_In_The_Group for(int iemePositGroup = 1; iemePositGroup <= m_NbrPositionnerInCurrentGroup; iemePositGroup++)

class CNewPortXPS
{
public:
	CNewPortXPS(void);
	CNewPortXPS(CString NewIp);
	~CNewPortXPS(void);
	// une eventuelle EditBox pour afficher les resultats des fonction de l'API
	CEdit *m_pEditXPSResult, *m_pEditXPSEvent, *m_pEditXPSAction;
	// une eventuelle Combo pour afficher tous les fichier PVT du repertoire
	CComboBox* m_pComboPVT;
	// une eventuelle Combo pour afficher tous les GroupName
	CComboBox* m_pComboGroupName, * m_pComboPositionerName;

private:
	// erreur courante
	int m_ErrorXPS;
	// CString courante correspondant a des résulats, ou a l'erreu Courante
	CString m_StrXPS;
	// le SocketID + IP + port si on s'est connecté
	int m_SocketID;
	CString m_IPAdress;
	int m_Port;
	double m_TimeOut;
	int m_EventTriggerForTrigBack, m_EventTriggerForTrajectory, m_EventTriggerForStartPVT, m_EventTriggerForStopPVT, m_EventTriggerForGathering, m_EventForOutputPosition;
	int m_AllEventAndActionIDs[50];
	// groupe, positionneur, trigg...
	CString m_SystemIniFileContent; // contnenu du fichier "system.ini" du XPS
	char m_pCurrentGroupName[250];
	char m_pGroupPositionnerName[MAX_POSITIONER + 1][250];
	int m_NbrGroup;
	int m_NbrPositionnerTotal;
	int m_NbrPositionnerInCurrentGroup;

	char m_pTrigName[250];
	char m_pGroupStatus[250];
	char m_pPositionnerStatus[250];
	// données sur le PVT
	char m_PVTFileName[250];
	// nom sous lequel serait sauver le pvtsi on clique save
	char m_PVTFileNameForSave[250];
	bool m_PVTVerified;
	int m_PVTNbrExecution;
	// les donnée d'un PVT, alouée dynamiquement
	int m_PVT_NumPoints;
	double* m_PVT_dt;
	double* m_PVT_dx[MAX_POSITIONER + 1];
	double* m_PVT_vf[MAX_POSITIONER + 1];
	// pour le transfert ftp
	CInternetSession UneSessionInternet;
	CFtpConnection *pConnectTrajectories, *pConnectSystemIni, *pConnectScript, *pConnectGatheredData;

	// limits
	double m_LimitMinPosition[MAX_POSITIONER + 1], m_LimitMaxPosition[MAX_POSITIONER + 1], m_LimitMaxVelocity[MAX_POSITIONER + 1], m_LimitMaxAcceleration[MAX_POSITIONER + 1];

	enum{NoPort = -1, DefaultPort_1 = 5001}; // Has to be 5001 : cf ProgrammerManual: p.12
	enum{NoSocketID = -1};

	// gere un m_ErrorXPS
	CString XPSError(CString APIName = "", int error = 0);

public:
	// La WaveForme qui d'écrit un graph de vitesse
	CWaveForme m_WfVitesse[MAX_POSITIONER + 1];
	// La position, qui est la surtout pour le display : en effet, on se restreindra souvent a des trajectoire parabolique pour lequelle l'info vitesse en fonction du temps est sufisante. cf XPS User's Manual p.84
	CWaveForme m_WfPosition[MAX_POSITIONER + 1];
	CNiGraph *m_pNiGraphVitesse[MAX_POSITIONER + 1], *m_pNiGraphPosition[MAX_POSITIONER + 1];


public:
	void OpenGraphicInterface();
	bool IsConnected();
	void SetNewIP(CString NewIP){m_IPAdress = NewIP;};
	void SetNewGroupName(CString NewGP);
	CString GetIP(){return m_IPAdress;};
	CString GetGroupName(){return CString(m_pCurrentGroupName);};
	CString GetGroupStateString();
	CString GetPostionnerStatusString();
	bool XPSConnect(CString NewIp = "", bool FindGroupNameAndPVT = true);
	void XPSDisConnect();
	// initialize, et renvoy un CPoint(Min, Max) de la course
	CPointDbl GetPositionerLimits(int iemePositioner);
	void RebootXPS();
	void ResetGroup();
	void InitializeHome();
	double GetPos(int iemePositioner);
	double GetPosTheoric(int iemePositioner);
	void SetPos(int iemePositioner, double NewPos);
	CPointDbl GetMinMaxPosition(int iemePositioner){return CPointDbl(m_LimitMinPosition[iemePositioner], m_LimitMaxPosition[iemePositioner]);};

	/// Pour les PVT **************************************************************
	//*****************************************************************************
	// dit qu'il faut utiliser un PVT file renvoye false si le lie n'existe pas
	bool PVTFileSet(CString pvtfile);
	CString PVTFileGet();
	CString PVTFileNameForSaveGet();
	void PVTFileNameForSaveSet(CString name);
	void CNewPortXPS::ActualiseGraphs();
	void PVTExecute();
	void PVTNbrExecutionSet(int NewNbr);
	int PVTNbrExecutionGet();
	// return CPointDbl(Min, Max) du mouvement
	CPointDbl PVTVerifie(bool DoVerifyPhysicallySeveralExecution = false);
	CPointDbl m_PvtMinMaxAbsolutePosition[MAX_POSITIONER + 1], m_PvtMaxVelocityMaxAcceleration[MAX_POSITIONER + 1];
private:
	// récupère toutes les données d'un fichier pvt du controleur
	CString PVTContentStringRead(); 
	// transfer toutes les données Wf dans les double[] m_PVT_dt, dx, et vf
	void PVT_Wf_to_dt_dx_vf();
	// affecte les données du PVT à UneWfVitesse et UneWfPosition dt dv et dx.   renvoye le m_PVT_NumPoints
	int PVTContentValuesGet(CWaveForme & UneWfVitesse, CWaveForme & UneWfPosition);
	// calcule le jerk et l'accélération initiale d'un segment de PVT comme à la page 85 du XPS User's Manual.
	// renvoye CPointDbl(Jerk, AccIni)
	CPointDbl PVTCalculate_Jerk_AccIni_LikeXPS(double DT, double DX, double VI, double VF);
	CPointDbl PVTCalculate_Jerk_AccIni_LikeXPS(int iemePVT, int iemeSegment_Start1);
public:
	// resize le pvt pour qu'il prenne un temps NewTotalTime
	// les vitesse du pvt sont donc aussi resaclée : prend plus longtemps = plus lentement
	// renvoye true si il y a eut changement d'un point. (il y a un seuil de 0.1% sur le multiplicateur)
	bool PVT_ResizeToTotalTime(double NewTotalTime);
	// idem amplitude vitesse
	bool PVT_ResizeToVelocityAmplitude(int iemePositionner, double NewVelocityAmplitude);
	// idem amplitude position
	bool PVT_ResizeToPositonAmplitude(int iemePositionner, double NewPositonAmplitude);
	// calcule, en fonction de m_WfVitesse, les positions nécessaires au PVT de manière a ce que le jerk (dérivée de l'accélération) soit nul rempli le tableau m_PVT_dx[]
	void PVTCalculatePositionToHaveJerk_0();
	// Ecrit les données de la Wf dans le PVT file du XPS
	void PVTContentStringWrite(bool ThenReOpen = true, CString FileName = PVT_NOT_SAVED_FILE_NAME);
	// trouve tous les fichier PVT, rempli une eventuelle Combo
	CString PVTFindAllFiles();
	// trouve tous les GroupName, rempli une eventuelle Combo
	CString GroupNameFindAll();
	// calcul l'interpolation PVT du XPS de la page 85 du XPS User's Manual.
	// renvoye CpointDbl(x(t) , v(t))
	CPointDbl PVT_XPS_Interpolate_Page85(int iemePVT, double WhichTime);
	// fin de: /// Pour les PVT **************************************************************
	//*****************************************************************************
	
	// dit qu'il faut suivre en POSITION l'entrée analogique WhichAI_GPIO2,
	void TrackingPosition(int iemePositioner, int WhichAI_GPIO2);
	// dit qu'il faut suivre en VITESSE l'entrée analogique WhichAI_GPIO2,
	void TrackingVelocity(int iemePositioner, int WhichAI_GPIO2);
	void TrackingDisable();
	// met les valeur de AquiringFreq et AquiringDuration utilisées en pratique
	// on aquérit les positions et vitesses à partir du WhichDI_GPIO1_ForAquiring, avec AquiringFreq pendant AquiringDuration
	void Record_PositionVelocity(int WhichDI_GPIO1_ForAquiring, double& AquiringFreq, double& AquiringDuration);
	void Record_AnalogTrack1And2AndPosition1And2(int WhichDI_GPIO1_ForAquiring, double& AquiringFreq, double& AquiringDuration);
	void Record_Trigger_Remove();
	// rapatrie dans chemin complet: NewLocalFileName, renvoye le pas de temps de l'aquistion, et rempli la CNiReal64Matrix des donnée 
	double Record_SaveAndRapatrie(CString NewLocalFileName, CNiReal64Matrix * pMatrix = NULL);
	// rapatrie dans DEFAULT_LOCAL_FOLDER_RECORD_POSITION_VITESSE NewLocalFileName et renvoye la CNiReal64Matrix des donnée
	// ajoute _%d_%d_%d_%d_%d_%d à la suite du nom .dat
	double Record_SaveAndRapatrieInDefaultFolder(CString NewLocalFileName, int chiffre1 = -1, int chiffre2 = -1, int chiffre3 = -1, int chiffre4 = -1, int chiffre5 = -1, int chiffre6 = -1);
	CString GetEventAndAction(int EventId);
	CString GetAllEventsAndActions();
	void RemoveAllEventAndAction();
	// utlise un trigger
	void SetTriggerForTrajectoryViaScript(int WhichTTL_1_8, int WhichGPIO_1_4 = 1);
	// renvoye CPoint(WhichTTL_1_8, WhichGPIO_1_4), (0,0) si pas de trigg
	CPoint GetTriggerToUseForTrajectoryViaScript();
};
