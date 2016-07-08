#include "StdAfx.h"
#include "NewPortXPS.h"
#include "DialogTranslationNewPort.h"

#define A_LA_LIGNE "\r\n"

CNewPortXPS::CNewPortXPS(void)
: m_IPAdress(""), m_Port(DefaultPort_1), m_SocketID(NoSocketID), m_ErrorXPS(0), m_StrXPS("")
, m_pEditXPSResult(NULL), m_pEditXPSEvent(NULL), m_pEditXPSAction(NULL), m_pComboPVT(NULL), pConnectTrajectories(NULL), m_pComboGroupName(NULL), m_pComboPositionerName(NULL)
, m_PVTVerified(false), m_PVTNbrExecution(1)
, m_EventTriggerForTrajectory(-1), m_EventTriggerForStartPVT(-1), m_EventTriggerForTrigBack(-1), m_EventTriggerForStopPVT(-1), m_EventTriggerForGathering(-1), m_EventForOutputPosition(-1)
{
	pConnectTrajectories = NULL;
	pConnectSystemIni = NULL;
	pConnectScript = NULL;
	pConnectGatheredData = NULL;
	for(int i = 0; i < 50; i++) m_AllEventAndActionIDs[i] = -1;
	m_NbrGroup = 0;
	m_NbrPositionnerTotal = 0;
	sprintf(m_pCurrentGroupName			, ""						);
	for(int i = 1; i <= MAX_POSITIONER; i++)sprintf(m_pGroupPositionnerName[i]		, ""						);
	sprintf(m_PVTFileName			, ""						);
	sprintf(m_PVTFileNameForSave	, ""						);
	sprintf(m_pTrigName				, ""						);
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PvtMinMaxAbsolutePosition[i] = PVT_FILE_DOESNT_EXIST;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PvtMaxVelocityMaxAcceleration[i] = PVT_FILE_DOESNT_EXIST;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMinPosition[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMaxPosition[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMaxVelocity[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMaxAcceleration[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_pNiGraphVitesse[i] = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_pNiGraphPosition[i] = NULL;
	m_PVT_NumPoints = 0;
	m_PVT_dt = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PVT_vf[i] = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PVT_dx[i] = NULL;
}
CNewPortXPS::CNewPortXPS(CString NewIp)
: m_IPAdress(""), m_Port(DefaultPort_1), m_SocketID(NoSocketID), m_ErrorXPS(0), m_StrXPS("")
, m_pEditXPSResult(NULL), m_pEditXPSEvent(NULL), m_pEditXPSAction(NULL), m_pComboPVT(NULL), pConnectTrajectories(NULL), m_pComboGroupName(NULL), m_pComboPositionerName(NULL)
, m_PVTVerified(false), m_PVTNbrExecution(1)
, m_EventTriggerForTrajectory(-1), m_EventTriggerForStartPVT(-1), m_EventTriggerForTrigBack(-1), m_EventTriggerForStopPVT(-1), m_EventTriggerForGathering(-1), m_EventForOutputPosition(-1)
{
	pConnectTrajectories = NULL;
	pConnectSystemIni = NULL;
	pConnectScript = NULL;
	pConnectGatheredData = NULL;
	for(int i = 0; i < 50; i++) m_AllEventAndActionIDs[i] = -1;
	m_NbrGroup = 0;
	m_NbrPositionnerTotal = 0;
	sprintf(m_pCurrentGroupName			, ""						);
	for(int i = 1; i <= MAX_POSITIONER; i++)sprintf(m_pGroupPositionnerName[i]		, ""						);
	sprintf(m_PVTFileName			, ""						);
	sprintf(m_PVTFileNameForSave	, ""						);
	sprintf(m_pTrigName				, ""						);
	SetNewIP(NewIp);
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PvtMinMaxAbsolutePosition[i] = PVT_FILE_DOESNT_EXIST;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PvtMaxVelocityMaxAcceleration[i] = PVT_FILE_DOESNT_EXIST;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMinPosition[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMaxPosition[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMaxVelocity[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_LimitMaxAcceleration[i] = 0.0;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_pNiGraphVitesse[i] = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_pNiGraphPosition[i] = NULL;
	m_PVT_NumPoints = 0;
	m_PVT_dt = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PVT_vf[i] = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)m_PVT_dx[i] = NULL;
	// + CONNECTION !
	XPSConnect();
}
CNewPortXPS::~CNewPortXPS(void)
{
	XPSDisConnect();
	if(m_PVT_dt)	delete [] m_PVT_dt; m_PVT_dt = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)if(m_PVT_dx[i])	delete [] m_PVT_dx[i];
	for(int i = 1; i <= MAX_POSITIONER; i++)if(m_PVT_vf[i])	delete [] m_PVT_vf[i];
}
void CNewPortXPS::XPSDisConnect()
{
	// si on est deja connecté
	if(!IsConnected())
		return;

	/////////////////////////////////////////////////
	// Kill group
	/////////////////////////////////////////////////
//	m_ErrorXPS = GroupKill(m_SocketID, m_pCurrentGroupName);
//	if("OK" != XPSError("GroupKill"))
//		return;

	///////////////////////////////////////////////////
	// Ftp dis-connection
	///////////////////////////////////////////////////
	// if the connection is open, close it
	if(pConnectTrajectories)
	{	pConnectTrajectories->Close();
		delete pConnectTrajectories;
		pConnectTrajectories = NULL;
	}
	if(pConnectSystemIni)
	{	pConnectSystemIni->Close();
		delete pConnectSystemIni;
		pConnectSystemIni = NULL;
	}
//	if(pConnectScript)
//	{	pConnectScript->Close();
//		delete pConnectScript;
//		pConnectScript = NULL;
//	}
	if(pConnectGatheredData)
	{	pConnectGatheredData->Close();
		delete pConnectGatheredData;
		pConnectGatheredData = NULL;
	}

	TCP_CloseSocket(m_SocketID);
	m_SocketID = NoSocketID;
	m_ErrorXPS = 0;
	m_StrXPS = "Disonnected";
}
bool CNewPortXPS::XPSConnect(CString NewIp /* = "" */, bool FindGroupNameAndPVT /* = true */)
{
	m_ErrorXPS = 0;
	// si on est deja connecté
	if(IsConnected())
		XPSDisConnect();

	/////////////////////////////////////////////////
	// TCP / IP connection
	/////////////////////////////////////////////////
	char buffer [256] = {'\0'};
	sprintf(buffer, NewIp == "" ? m_IPAdress : NewIp);

	m_TimeOut = 10;
	m_SocketID = TCP_ConnectToServer(buffer, m_Port, m_TimeOut); // Open a socket
	if (!IsConnected())
	{
		CString strErrorConnect;
		strErrorConnect.Format("Connection to @ %s, port = %ld failed\n", m_IPAdress, m_Port);
		//AfxMessageBox (strErrorConnect, MB_ICONSTOP);
		//m_IPAdress = "";
		return false;
	}
	else
	{
		m_IPAdress = buffer;
		*buffer = '\0';
		/////////////////////////////////////////////////
		// Get controller version
		/////////////////////////////////////////////////
		m_ErrorXPS = FirmwareVersionGet(m_SocketID, buffer); // Get controller version
		if("OK" != XPSError("FirmwareVersionGet"))	
		{;}
		m_StrXPS = GetLibraryVersion(); // Get Library version

		m_StrXPS += CString("\r\n") + buffer;
		if(m_pEditXPSResult)
			m_pEditXPSResult->SetWindowText(m_StrXPS);
	}

	if(FindGroupNameAndPVT)
	{
		PVTFindAllFiles();
		GroupNameFindAll();
	}
	return true;
}
bool CNewPortXPS::IsConnected()
{
	if(m_SocketID != NoSocketID)
		return true; 
	m_StrXPS = "Not Connected";
	if(m_pEditXPSResult)
		m_pEditXPSResult->SetWindowText(m_StrXPS);
	return false;
}
void CNewPortXPS::SetNewGroupName(CString NewGP)
{
	sprintf(m_pCurrentGroupName, NewGP);
	// si il y a un ou des positionneur correspondant, 
	int indexPos = 0;
	for(int i = 1; i <= MAX_POSITIONER; i++)
		sprintf(m_pGroupPositionnerName[i], "");

	if(m_pComboPositionerName)
		m_pComboPositionerName->ResetContent();
	m_NbrPositionnerInCurrentGroup = 0;
	while((indexPos = m_SystemIniFileContent.Find(CString(m_pCurrentGroupName) + ".", indexPos)) != -1)
	{
		m_NbrPositionnerInCurrentGroup++;
		sprintf(m_pGroupPositionnerName[m_NbrPositionnerInCurrentGroup], m_SystemIniFileContent.Tokenize("[]", indexPos));
		// on l'ajoute à la combo box Positioner
		CString PosiName = CString(m_pGroupPositionnerName[m_NbrPositionnerInCurrentGroup]);
		int posPositioner = PosiName.Find(".");
		PosiName.Delete(0, posPositioner + 1);
		if(m_pComboPositionerName)
			m_pComboPositionerName->AddString(PosiName);
	}
	if(indexPos == 0)
		AfxMessageBox("Pas de Positionneur Correspondant !", MB_ICONSTOP);
	else if(m_pComboPositionerName)
		m_pComboPositionerName->SetCurSel(0);
}

CString CNewPortXPS::GetGroupStateString()
{
	if(!IsConnected()) 
		return false; 
	int state; 
	GroupStatusGet(m_SocketID, m_pCurrentGroupName, &state);
	GroupStatusStringGet(m_SocketID, state, m_pGroupStatus);
	CString StrStatus(m_pGroupStatus);
	return StrStatus;
}
CString CNewPortXPS::GetPostionnerStatusString()
{
	if(!IsConnected()) 
		return false; 
	CString StrStatus("");
	For_All_Positioner_In_The_Group
	{
	int state; 
	PositionerErrorGet(m_SocketID, m_pGroupPositionnerName[iemePositGroup], &state);
	PositionerErrorStringGet(m_SocketID, state, m_pPositionnerStatus);
	StrStatus += CString(m_pGroupPositionnerName[iemePositGroup]) 
			   + CString(" : ") 
			   + CString(m_pPositionnerStatus) 
			   + "\r\n";
	}
	return StrStatus;
}
CString CNewPortXPS::XPSError(CString APIName /* = "" */, int Error /* = 0 */)
{	
	if(!IsConnected())
	{
		m_StrXPS = "No Connection : XPSError";
		return m_StrXPS;
	}
	char strError[250];

	// si on ne passe pas d'argument à error, on prend l'erreur membre: m_ErrorXPS
	int errorXPS = Error ? Error : m_ErrorXPS;
	// si c'est toujours 0, c'est a dire si m_ErrorXPS == 0, on quite avec "0"
	if(!errorXPS)
	{
		m_StrXPS = "OK";
		if(m_pEditXPSResult)
			m_pEditXPSResult->SetWindowText(m_StrXPS);
		return m_StrXPS;
	}

	int error2;

	// If error occurred other than Timeout error
	if (errorXPS != -2)
	{
		// Error => Get error description
		error2 = ErrorStringGet(m_SocketID, errorXPS, strError);
		// If error occurred with the API ErrorStringGet
		if(error2)
		{
			// Display API name, error code and ErrorStringGet error code
			m_StrXPS.Format("ErrorStringGet ERROR => %d", error2);
			AfxMessageBox(m_StrXPS, MB_ICONSTOP);
		} 
		else
		{
			// Display API name, number and description of the error
			m_StrXPS.Format("%s ERROR => %d:\r\n%s", APIName, errorXPS, strError);
//			AfxMessageBox(m_StrXPS, MB_ICONSTOP);
		}
	}
	else
	{
		// Timeout error
		m_StrXPS.Format("%s ERROR => %d:\r\nTCP timeout", APIName, errorXPS);
		if(IsConnected())
			AfxMessageBox(m_StrXPS, MB_ICONSTOP);
	}

	if(m_pEditXPSResult)
		m_pEditXPSResult->SetWindowText(m_StrXPS);
	return m_StrXPS;
}

void CNewPortXPS::RebootXPS()
{
	m_ErrorXPS = Reboot(m_SocketID);
	if("OK" != XPSError("Reboot"))
	{;}
}

void CNewPortXPS::ResetGroup()
{
	/////////////////////////////////////////////////
	// Kill group
	/////////////////////////////////////////////////
	m_ErrorXPS = GroupKill(m_SocketID, m_pCurrentGroupName);
	if("OK" != XPSError("GroupKill"))
	{;}
	//m_ErrorXPS = GroupMotionDisable(m_SocketID, m_pCurrentGroupName);
	//if("OK" != XPSError("GroupMotionDisable"))
	//	;//return;
	m_ErrorXPS = GroupMotionEnable(m_SocketID, m_pCurrentGroupName);
	if("OK" != XPSError("GroupMotionEnable"))
	{;}
}
void CNewPortXPS::InitializeHome()
{
	ResetGroup();
	/////////////////////////////////////////////////
	// Initialize group
	/////////////////////////////////////////////////
	m_ErrorXPS = GroupInitialize(m_SocketID, m_pCurrentGroupName);
	if("OK" != XPSError("GroupInitialize"))
		return;
	/////////////////////////////////////////////////
	// Search home group
	/////////////////////////////////////////////////
	m_ErrorXPS = GroupHomeSearch(m_SocketID, m_pCurrentGroupName);
	if("OK" != XPSError("GroupHomeSearch"))
		return;
	For_All_Positioner_In_The_Group
	{
		GetPositionerLimits(iemePositGroup);
	}
	return;
}
CPointDbl CNewPortXPS::GetPositionerLimits(int iemePositioner)
{
	CPointDbl MinMaxTravelARetourner(0.0, 0.0);
	/////////////////////////////////////////////////
	// Search and speed and acceleration Travel Limits
	/////////////////////////////////////////////////
	m_ErrorXPS = PositionerUserTravelLimitsGet(m_SocketID, m_pGroupPositionnerName[iemePositioner], &m_LimitMinPosition[iemePositioner], &m_LimitMaxPosition[iemePositioner]);
	if(iemePositioner > m_NbrPositionnerInCurrentGroup || "OK" != XPSError("PositionerUserTravelLimitsGet"))
		return MinMaxTravelARetourner;
	m_ErrorXPS = PositionerMaximumVelocityAndAccelerationGet(m_SocketID, m_pGroupPositionnerName[iemePositioner], &m_LimitMaxVelocity[iemePositioner], &m_LimitMaxAcceleration[iemePositioner]);
	if("OK" != XPSError("PositionerMaximumVelocityAndAccelerationGet"))
		return MinMaxTravelARetourner;
    
	MinMaxTravelARetourner.x = m_LimitMinPosition[iemePositioner];
	MinMaxTravelARetourner.y = m_LimitMaxPosition[iemePositioner];
	return MinMaxTravelARetourner;
}
void CNewPortXPS::OpenGraphicInterface()
{
	if(this == NULL)
		return;
	CDialogTranslationNewPort XPSDlg(this);
	if(XPSDlg.DoModal() != IDOK) 
		return;
}
double CNewPortXPS::GetPos(int iemePositioner)
{
	if(!IsConnected() || iemePositioner > m_NbrPositionnerInCurrentGroup)
		return 0.0;
	double dresult[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	m_ErrorXPS = GroupPositionCurrentGet(m_SocketID, m_pCurrentGroupName, m_NbrPositionnerInCurrentGroup, dresult);
	if("OK" != XPSError("GetPos"))
		return 0.0;
	return dresult[iemePositioner - 1];
}
double CNewPortXPS::GetPosTheoric(int iemePositioner)
{
	if(!IsConnected() || iemePositioner > m_NbrPositionnerInCurrentGroup)
		return 0.0;
	double dresult[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    m_ErrorXPS = GroupPositionSetpointGet(m_SocketID, m_pCurrentGroupName, m_NbrPositionnerInCurrentGroup, dresult);
	if("OK" != XPSError("GetPos"))
		return 0.0;
	return dresult[iemePositioner - 1];
}
void CNewPortXPS::SetPos(int iemePositioner, double NewPos)
{
	if(!IsConnected() || iemePositioner > m_NbrPositionnerInCurrentGroup)
		return;
	double newpos[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	For_All_Positioner_In_The_Group
	{
		newpos[iemePositGroup - 1] = GetPosTheoric(iemePositGroup);
	}
	newpos[iemePositioner - 1] = NewPos;
    m_ErrorXPS = GroupMoveAbsolute(m_SocketID, m_pCurrentGroupName, m_NbrPositionnerInCurrentGroup, newpos);
	if("OK" != XPSError("SetPos"))
		return;
	return;
}
bool CNewPortXPS::PVTFileSet(CString pvtfile)
{
	sprintf(m_PVTFileName, pvtfile);
	PVTFileNameForSaveSet(m_PVTFileName);
	if(!IsConnected())
		return false;
	bool FileExiste = (PVTVerifie(true) != PVT_FILE_DOESNT_EXIST);
	if(FileExiste)
		PVTContentValuesGet(m_WfVitesse[1], m_WfPosition[1]/*arevoir?*/);

	return m_PVTVerified;
}
CString CNewPortXPS::PVTFileGet()
{
	return CString(m_PVTFileName);
}
CString CNewPortXPS::PVTFileNameForSaveGet()
{
	return CString(m_PVTFileNameForSave);
}
void CNewPortXPS::PVTFileNameForSaveSet(CString name)
{
	if(CString(name) != PVT_NOT_SAVED_FILE_NAME)
		sprintf(m_PVTFileNameForSave, name);
}
void CNewPortXPS::ActualiseGraphs()
{
	For_All_Positioner_In_The_Group
	{
		if(m_pNiGraphVitesse[iemePositGroup] && m_pNiGraphPosition[iemePositGroup])
		{
			m_pNiGraphVitesse[iemePositGroup]->ClearData();
			m_pNiGraphVitesse[iemePositGroup]->GetAnnotations().RemoveAll();
			for(int i = 0; i < m_WfVitesse[iemePositGroup].GetnPoints(); i++)
			{
				CPointDbl& iemePoint = m_WfVitesse[iemePositGroup].GetPoint(i);
				m_pNiGraphVitesse[iemePositGroup]->ChartXY(iemePoint.x, iemePoint.y);
				// ajout des accélérations
				if(i > 0)
				{
					CString textAccele;
					double accele = (iemePoint.y - m_WfVitesse[iemePositGroup].GetPoint(i - 1).y) / (iemePoint.x - m_WfVitesse[iemePositGroup].GetPoint(i - 1).x);
					textAccele.Format("%d mm/s²", int(accele));
					CNiAnnotation Annot = m_pNiGraphVitesse[iemePositGroup]->GetAnnotations().Add();
					Annot.Visible = true;
					Annot.Caption.Text = textAccele;
					Annot.Caption.Alignment = CNiCaption::Center;
					Annot.Caption.SetXCoordinate((iemePoint.x + m_WfVitesse[iemePositGroup].GetPoint(i - 1).x) / 2);
					Annot.Caption.SetYCoordinate((iemePoint.y + m_WfVitesse[iemePositGroup].GetPoint(i - 1).y) / 2);
					Annot.Arrow.Visible = false;
				}
			}
			m_pNiGraphVitesse[iemePositGroup]->GetAxes().Item("XAxis").AutoScaleNow();

			m_pNiGraphPosition[iemePositGroup]->ClearData();
			for(int i = 0; i < m_WfPosition[iemePositGroup].GetnPoints(); i++)
			{
				CPointDbl& iemePoint = m_WfPosition[iemePositGroup].GetPoint(i);
				m_pNiGraphPosition[iemePositGroup]->ChartXY(iemePoint.x, iemePoint.y);
			}
			// l'interpolation XPS : xxx points par exemple
			PVT_Wf_to_dt_dx_vf();
			int npoints = 200;
			double dt = m_WfPosition[iemePositGroup].GetPoint(-1).x / npoints;
			for(int i = 0; i <= npoints; i++)
			{
				CPointDbl Pti = PVT_XPS_Interpolate_Page85(1/*arevoir?*/, i * dt);
				m_pNiGraphPosition[iemePositGroup]->GetPlots().Item(1).ChartXY(i * dt, Pti.x);
			}
			m_pNiGraphPosition[iemePositGroup]->GetAxes().Item("XAxis").AutoScaleNow();
		}
	}
}

CString CNewPortXPS::GetEventAndAction(int EventId)
{	
	char pEvent[256]; // quel Event
	char pAction[256]; // dummy car l'api renvoye la meme chose dans pEvent et pAction à qq erreur près
	//for(int i = 0; i < 255; i++) 
	//	pEvent[i] = '_';
	//for(int i = 0; i < 255; i++) 
	//	pAction[i] = '_';

	m_ErrorXPS = EventExtendedGet(m_SocketID, EventId, pEvent, pAction);
	if("OK" != XPSError("EventExtendedGet"))
		return "Error Getting Event";
	// it seems that on doit remplacer manuellement les '\0' : Erreur de l'API qui renvoye la string !!!
	if(pEvent[1] == '\0')
		pEvent[1] = 'à';
	CString Event(pEvent);
	Event.Replace("à", "\r\n");
return Event;
}
CString CNewPortXPS::GetAllEventsAndActions()
{
	char pEventAndAction[256]; // quel Event
	sprintf(pEventAndAction, "No Event!");
	m_ErrorXPS = EventExtendedAllGet(m_SocketID, pEventAndAction);
	if("OK" != XPSError("EventExtendedAllGet"))
		return pEventAndAction;
	// it seems that on doit remplacer manuellement les '\0' : Erreur de l'API qui renvoye la string !!!
	// on a une liste de chiffres... 0\01,2,3,...EndOfAPI
	char* pEventIDs = pEventAndAction;
	if(pEventAndAction[1] == '\0')
		//pEventIDs += 2;
		 pEventAndAction[1] = ',';
	// on a une liste de chiffres... 0,1,2,3,...EndOfAPI
	CString AllEventIDs(pEventIDs);
	AllEventIDs.Replace(",EndOfAPI", "");
	// on a une liste de chiffres... 0,1,2,3
	// trouvons chaque Event: tant qu'il y a un chiffre
	int pos = 0;
	CString resTok;
	CString TheTokChars = " ,\t\r\n";
	resTok = AllEventIDs.Tokenize(TheTokChars, pos);
	CString AllEventAndAction = "";
	char* stopString;
	int ieme = 0;
	for(int i = 0; i < 50; i++) m_AllEventAndActionIDs[i] = -1;
	while(resTok != "")
	{
		long iemeID = strtol(resTok, & stopString, 10);
		AllEventAndAction += GetEventAndAction(iemeID) + CString("\r\n");
		resTok = AllEventIDs.Tokenize(TheTokChars, pos);
		m_AllEventAndActionIDs[ieme] = iemeID;
		ieme++;
	}

	return AllEventAndAction;
}
void CNewPortXPS::RemoveAllEventAndAction()
{
	GetAllEventsAndActions();
	for(int i = 0; i < 50; i++)
	{
		if(m_AllEventAndActionIDs[i] != -1)
			EventExtendedRemove(m_SocketID, m_AllEventAndActionIDs[i]);
	}
	GetAllEventsAndActions();
}

void CNewPortXPS::SetTriggerForTrajectoryViaScript(int WhichTTL_1_8, int WhichGPIO_1_4 /* = 1 */)
{
	if(m_EventTriggerForTrajectory != -1)
	{
		m_ErrorXPS = EventExtendedRemove(m_SocketID, m_EventTriggerForTrajectory);
		if("OK" != XPSError("EventExtendedRemove"))
		{;}
		m_EventTriggerForTrajectory = -1;
	}
	if(m_EventTriggerForStartPVT != -1)
	{
		m_ErrorXPS = EventExtendedRemove(m_SocketID, m_EventTriggerForStartPVT);
		if("OK" != XPSError("EventExtendedRemove"))
		{;}
		m_EventTriggerForStartPVT = -1;
	}
	if(m_EventTriggerForStopPVT != -1)
	{
		m_ErrorXPS = EventExtendedRemove(m_SocketID, m_EventTriggerForStopPVT);
		if("OK" != XPSError("EventExtendedRemove"))
		{;}
		m_EventTriggerForStopPVT = -1;
	}
	if(m_EventTriggerForTrigBack != -1)
	{
		m_ErrorXPS = EventExtendedRemove(m_SocketID, m_EventTriggerForTrigBack);
		if("OK" != XPSError("EventExtendedRemove"))
		{;}
		m_EventTriggerForTrigBack = -1;
	}
	GetAllEventsAndActions();
	if(WhichGPIO_1_4 == 0 || WhichTTL_1_8 == 0)
	{
		return;
	}

	char* pZero = "0";
	char* pZeroZero = "0 0";
	char* pUn = "1";
	char* pDeuxcinqcinq = "255";
////// EVENT //////
	char pEvent[250]; // quel Event
	char pBit[8]; // index du bit en base 2
	sprintf(pEvent, "GPIO%d.DI.DIHighLow %s", WhichGPIO_1_4, "Always");
	sprintf(pBit, "%d 0", int(pow(2.0, double(WhichTTL_1_8 - 1))) - 1);
////// ACTION //////
	char pAction[250];
	char pActionParam1[250];
	char pActionParam2[250];
	char pActionParam3[250];
	CString strExecuteTCLScript("ExecuteTCLScript");
	CString strDo1_Set("GPIO1.DO.DOSet");
	CString strDo1_Pulse("GPIO1.DO.DOPulse");

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// DOSet au recevoir d'un trig
	sprintf(pAction			, "%s",	strDo1_Set	);
	m_ErrorXPS = EventExtendedConfigurationTriggerSet(m_SocketID, 2, pEvent, pBit, "0 0", "0 0", "0 0");
	if("OK" != XPSError("ExtendedConfigurationTriggerSet"))
		return;
	m_ErrorXPS = EventExtendedConfigurationActionSet(m_SocketID, 1,	pAction, "1", "1", "0", "0");
	if("OK" != XPSError("EventExtendedConfigurationActionSet"))
		return;
	m_ErrorXPS = EventExtendedStart(m_SocketID, &m_EventTriggerForTrigBack);
	if("OK" != XPSError("EventExtendedStart"))
		return;
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Lance script au recevoir d'un trig
	sprintf(pAction			, "%s",	strExecuteTCLScript		);
	sprintf(pActionParam1	, "%s",	"ExecuteTrajectory.tcl"	);
	sprintf(pActionParam2	, "%s",	"ExecuteTrajectory"		);
	sprintf(pActionParam3	, "%s",	m_PVTFileName			);
	m_ErrorXPS = EventExtendedConfigurationTriggerSet(m_SocketID, 2, pEvent, pBit, "0 0", "0 0", "0 0");
	if("OK" != XPSError("ExtendedConfigurationTriggerSet"))
		return;
	m_ErrorXPS = EventExtendedConfigurationActionSet(m_SocketID, 1,	pAction, pActionParam1, pActionParam2, pActionParam3, "0");
	if("OK" != XPSError("EventExtendedConfigurationActionSet"))
		return;
	m_ErrorXPS = EventExtendedStart(m_SocketID, &m_EventTriggerForTrajectory);
	if("OK" != XPSError("EventExtendedStart"))
		return;

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Trig au start d'un PVT
	sprintf(pEvent, "%s.PVT.TrajectoryStart %s", m_pCurrentGroupName, "Always");
	sprintf(pAction, "%s",	strDo1_Set	);
	m_ErrorXPS = EventExtendedConfigurationTriggerSet(m_SocketID, 2, pEvent, pZeroZero, pZeroZero, pZeroZero, pZeroZero);
	if("OK" != XPSError("ExtendedConfigurationTriggerSet"))
		return;
	m_ErrorXPS = EventExtendedConfigurationActionSet(m_SocketID, 1,	pAction, "3", "2", "0", "0");
	if("OK" != XPSError("EventExtendedConfigurationActionSet"))
		return;
	m_ErrorXPS = EventExtendedStart(m_SocketID, &m_EventTriggerForStartPVT);
	if("OK" != XPSError("EventExtendedStart"))
		return;

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Trig au stop d'un PVT
	sprintf(pEvent, "%s.PVT.TrajectoryEnd %s", m_pCurrentGroupName, "Always");
	sprintf(pAction, "%s",	strDo1_Set	);
	m_ErrorXPS = EventExtendedConfigurationTriggerSet(m_SocketID, 2, pEvent, pZeroZero, pZeroZero, pZeroZero, pZeroZero);
	if("OK" != XPSError("ExtendedConfigurationTriggerSet"))
		return;
	m_ErrorXPS = EventExtendedConfigurationActionSet(m_SocketID, 1,	pAction, "2", "0", "0", "0");
	if("OK" != XPSError("EventExtendedConfigurationActionSet"))
		return;
	m_ErrorXPS = EventExtendedStart(m_SocketID, &m_EventTriggerForStopPVT);
	if("OK" != XPSError("EventExtendedStart"))
		return;
}

CPoint CNewPortXPS::GetTriggerToUseForTrajectoryViaScript()
{	
	CPoint pointaRet(0,0);
	char pEvent[256]; // quel Event
	char pAction[256]; // quel Action
	EventExtendedGet(m_SocketID, m_EventTriggerForTrajectory, pEvent, pAction);
	// it seems that on doit remplacer manuellement les '\0'
	for(int i = 0; i < 255; i++)
		if(pEvent[i] == '\0' && pEvent[i + 1] != 'Ì')
			pEvent[i] = '\r';
	// it seems that on doit remplacer manuellement les '\0'
	for(int i = 0; i < 255; i++)
		if(pAction[i] == '\0' && pAction[i + 1] != 'Ì')
			pAction[i] = '\r';
	CString Event(pEvent);
	CString Action(pAction);
	Event.Replace("\r", "\r\n");
	Action.Replace("\r", "\r\n");

	char* stopChar = '\0';
	int whichGPIO = 0;
	int whichTTL = 0;
	// si ca commence par GPIO on chope le chiffre
	if(Event.Find("GPIO") == 0)
		pointaRet.y = strtol(Event.Mid(4, 1), &stopChar, 10);
	/////////////////////////// PAS FINI
	if(m_pTrigName == "")
		return pointaRet;

	return pointaRet;
}
void CNewPortXPS::PVTExecute()
{
	// verifions, avec un true pour verifer plusieur executions éventuelles
	PVTVerifie(true);
	if(!m_PVTVerified)
		return;

	m_ErrorXPS = MultipleAxesPVTExecution(m_SocketID, m_pCurrentGroupName, m_PVTFileName, m_PVTNbrExecution);
	if("OK" != XPSError("PVTExecution"))
		return;

	return;
}
void CNewPortXPS::PVTNbrExecutionSet(int NewNbr)
{
	m_PVTNbrExecution = NewNbr;
}
int CNewPortXPS::PVTNbrExecutionGet()
{
	return m_PVTNbrExecution;
}
CPointDbl CNewPortXPS::PVTVerifie(bool DoVerifyPhysicallySeveralExecution /* = false */)
{
	// first, we tell the program that it is not verified
	m_PVTVerified = false;
	int ErrorLikeVitesseTropGrande = 0;
	m_ErrorXPS = MultipleAxesPVTVerification(m_SocketID, m_pCurrentGroupName, m_PVTFileName);
	if("OK" != XPSError("PVT"))
		if(m_StrXPS.Find("exist")/* en fait : "doesn't exist" */ != -1)
			return PVT_FILE_DOESNT_EXIST;
		else
			ErrorLikeVitesseTropGrande = m_ErrorXPS;
	// Get the result
	double RelatifTrajMinPos, RelatifTrajMaxPos, TrajMaxV, TrajMaxA;

	For_All_Positioner_In_The_Group
	{
		MultipleAxesPVTVerificationResultGet(m_SocketID, m_pGroupPositionnerName[iemePositGroup], m_PVTFileName, &RelatifTrajMinPos, &RelatifTrajMaxPos, &TrajMaxV, &TrajMaxA);
		// Now let's see if we reach an OUTOFRANGE:
		m_PvtMaxVelocityMaxAcceleration[iemePositGroup].x = TrajMaxV;
		m_PvtMaxVelocityMaxAcceleration[iemePositGroup].y = TrajMaxA;
		double InitialAbslutePos = GetPos(iemePositGroup);
		m_PvtMinMaxAbsolutePosition[iemePositGroup].x = InitialAbslutePos + RelatifTrajMinPos;
		m_PvtMinMaxAbsolutePosition[iemePositGroup].y = InitialAbslutePos + RelatifTrajMaxPos;
		m_ErrorXPS = ErrorLikeVitesseTropGrande;
		if("OK" != XPSError("PVTVerification"))
			return PVT_VERIFY_ERROR;
		if(ErrorLikeVitesseTropGrande)
			return PVT_VERIFY_ERROR;
		if(		m_PvtMinMaxAbsolutePosition[iemePositGroup].x < m_LimitMinPosition[iemePositGroup]
		||	m_PvtMinMaxAbsolutePosition[iemePositGroup].y > m_LimitMaxPosition[iemePositGroup]
		||	TrajMaxV > m_LimitMaxVelocity[iemePositGroup]
			||	TrajMaxA > m_LimitMaxAcceleration[iemePositGroup]
			)
		{
			// display a OUTOFRANGE ERROR
			XPSError("Movement Simulation : OUTOFRANGE", -22/*ERR_NOT_ALLOWED_ACTION*/);
			return PVT_VERIFY_OK;
		}
	}
	// if we execute once only, we are done:
//	if(m_PVTNbrExecution <= 1)
	{
		// if you reach this point, it is verified succesfully
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		m_PVTVerified = true;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return PVT_VERIFY_OK;
	}
//	// no négative!
//	if(m_PVTNbrExecution < 0)
//		return PVT_VERIFY_ERROR;
//	// si on ne verify pas un m_PVTNbrExecution > 1, on return
//	if(!DoVerifyPhysicallySeveralExecution)
//		return m_PvtMinMaxAbsolutePosition;
//
//	m_PvtMinMaxAbsolutePosition[iemePositGroup] = PVT_VERIFY_ERROR;
//	// if the PVT is to be executed severaltimes, we have to:
//	// first verifie the first PVT execution : already done
//	// and then we calculate if the trajectory after the Nth execution is out of range by doing
//	// let's ask first
//	CString asking; asking.Format("Do you want to test the %d executions \nby executing one of them\nand then come back to this positon?", m_PVTNbrExecution);
//	if(IDCANCEL == AfxMessageBox(asking))
//		return m_PvtMinMaxAbsolutePosition;
//	// where are we ?
//    m_ErrorXPS = GroupPositionSetpointGet(m_SocketID, m_pCurrentGroupName, 1, &InitialAbslutePos);
//	if("OK" != XPSError("PositionSetpointGet"))
//		return m_PvtMinMaxAbsolutePosition;
//	// let's move over one ! and see the new pos
//	m_ErrorXPS = MultipleAxesPVTExecution(m_SocketID, m_pCurrentGroupName, m_PVTFileName, 1);
//	if("OK" != XPSError("PVTExecution"))
//		return m_PvtMinMaxAbsolutePosition;
// 	// where are we Now?
//	double NexPos_1_Exec;
//    m_ErrorXPS = GroupPositionSetpointGet(m_SocketID, m_pCurrentGroupName, 1, &NexPos_1_Exec);
//	if("OK" != XPSError("PositionSetpointGet"))
//		return m_PvtMinMaxAbsolutePosition;
//   double DeltaPos_1_exec = NexPos_1_Exec - InitialAbslutePos;
//	// let's come back
//    m_ErrorXPS = GroupMoveAbsolute(m_SocketID, m_pCurrentGroupName, 1, &InitialAbslutePos);
//	if("OK" != XPSError("SetPos"))
//		return m_PvtMinMaxAbsolutePosition;
//	// calculons
//	InitialAbslutePos = GetPos();
//	m_PvtMinMaxAbsolutePosition.x = InitialAbslutePos + (m_PVTNbrExecution - 1) * DeltaPos_1_exec + RelatifTrajMinPos;
//	m_PvtMinMaxAbsolutePosition.y = InitialAbslutePos + (m_PVTNbrExecution - 1) * DeltaPos_1_exec + RelatifTrajMaxPos;
//	if(		m_PvtMinMaxAbsolutePosition.x < m_LimitMinPosition
//		||	m_PvtMinMaxAbsolutePosition.y > m_LimitMaxPosition
//	  )
//	{
//		// display a OUTOFRANGE ERROR
//		XPSError("Movement Simulation : OUTOFRANGE", -22/*ERR_NOT_ALLOWED_ACTION*/);
//		return m_PvtMinMaxAbsolutePosition;
//	}
//	// if you reach this point, it is verified succesfully
//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	m_PVTVerified = true;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	return m_PvtMinMaxAbsolutePosition;
}

CString CNewPortXPS::PVTContentStringRead()
{
	if(!IsConnected())
		return "";

	// ouvrons ftp
	if(pConnectTrajectories)
	{	pConnectTrajectories->Close();
		delete pConnectTrajectories;
		pConnectTrajectories = NULL;
	}
	pConnectTrajectories = UneSessionInternet.GetFtpConnection(m_IPAdress, "Administrator", "Administrator", 21);
	pConnectTrajectories->SetCurrentDirectory("//Admin//public//Trajectories//");

	// là ou on mettra le contenu
	CString TheFileContent("");
	// use a file find object to enumerate files
	CFtpFileFind finder(pConnectTrajectories);
	// start looping
	BOOL bWorking = finder.FindFile(m_PVTFileName);
	if(bWorking)
	{
		CInternetFile* pTheFile = pConnectTrajectories->OpenFile(m_PVTFileName);
		// ligne par ligne
		CString OneLigne;
		// premiere
		bool NotFinished = pTheFile->ReadString(TheFileContent)?true:false;
		// suivante
		while(NotFinished)
		{
			NotFinished = pTheFile->ReadString(OneLigne)?true:false;
			// si il n'y a rien d'intéressant sur la ligne (ie: au moins un chiffre), on ne l'ajoute pas
			if(OneLigne.FindOneOf("0123456789") != -1)
				TheFileContent += CString(A_LA_LIGNE) + OneLigne;
		}
		pTheFile->Close();
		delete pTheFile;
	}
	// on ferme
	if(pConnectTrajectories)
	{	pConnectTrajectories->Close();
		delete pConnectTrajectories;
		pConnectTrajectories = NULL;
	}

	return TheFileContent;
}
void CNewPortXPS::PVTContentStringWrite(bool ThenReOpen /* = true */, CString FileName /* = PVT_NOT_SAVED_FILE_NAME */)
{
	if(FileName == "")
		return;
	if(!IsConnected())
		return;
	m_PVT_NumPoints = m_WfVitesse[1].GetnPoints() - 1;
	if(m_PVT_NumPoints < 2)
		return;

	PVT_Wf_to_dt_dx_vf();

	// créons la String ligne par ligne
	CString TheFileContent("");
	CString OneLigne;
	for(int valcour = 1; valcour <= m_PVT_NumPoints; valcour++)
	{
		OneLigne.Format("%f, \t%f, \t%f ", m_PVT_dt[valcour], m_PVT_dx[valcour], m_PVT_vf[valcour]);
		OneLigne += A_LA_LIGNE;
		TheFileContent += OneLigne;
	}

	// ouvrons ftp
	if(pConnectTrajectories)
	{	pConnectTrajectories->Close();
		delete pConnectTrajectories;
		pConnectTrajectories = NULL;
	}
	pConnectTrajectories = UneSessionInternet.GetFtpConnection(m_IPAdress, "Administrator", "Administrator", 21);
	pConnectTrajectories->SetCurrentDirectory("//Admin//public//Trajectories//");
	// on ecrit maintenant
	CInternetFile* pTheFile = pConnectTrajectories->OpenFile(FileName, GENERIC_WRITE);
	pTheFile->Write(TheFileContent, TheFileContent.GetLength());
	pTheFile->Close();
	delete pTheFile;
	// on ferme
	if(pConnectTrajectories)
	{	pConnectTrajectories->Close();
		delete pConnectTrajectories;
		pConnectTrajectories = NULL;
	}

	if(ThenReOpen)
	{
		PVTFileSet(FileName);
	}
}
void CNewPortXPS::PVT_Wf_to_dt_dx_vf()
{
	m_PVT_NumPoints = m_WfVitesse[1].GetnPoints() - 1;
	int PVT_NumPointsP = m_WfPosition[1].GetnPoints() - 1;
	if(m_PVT_NumPoints != PVT_NumPointsP)
		AfxMessageBox("456456", MB_ICONERROR);
	if(m_PVT_NumPoints < 2)
		return;

	if(m_PVT_dt)	delete [] m_PVT_dt; m_PVT_dt = NULL;
	for(int i = 1; i <= MAX_POSITIONER; i++)if(m_PVT_dx[i])	delete [] m_PVT_dx[i];
	for(int i = 1; i <= MAX_POSITIONER; i++)if(m_PVT_vf[i])	delete [] m_PVT_vf[i];
	m_PVT_dt = new double[m_PVT_NumPoints + 1];
	For_All_Positioner_In_The_Group
	{
		// le dernier point a toujours une vitesse nulle
		m_WfVitesse[iemePositGroup].MovePoint(-1, m_WfVitesse[iemePositGroup].GetPoint(-1).x, 0.0);
		// on prend les valeurs de la Wf maintenant
		m_PVT_dx[iemePositGroup] = new double[m_PVT_NumPoints + 1];
		m_PVT_vf[iemePositGroup] = new double[m_PVT_NumPoints + 1];
		// par convention :
		m_PVT_dt[0] = 0.0;	m_PVT_dx[iemePositGroup][0] = 0.0;	m_PVT_vf[iemePositGroup][0] = 0.0;
		double TotalTime = 0.0;
		double TotalDist = 0.0;
		for(int valcour = 1; valcour <= m_PVT_NumPoints; valcour++)
		{	// le delta_t
			CPointDbl TiVFi = m_WfVitesse[iemePositGroup].GetPoint(valcour);
			m_PVT_dt[valcour] = TiVFi.x - TotalTime;
			TotalTime += m_PVT_dt[valcour];
			// le vf
			m_PVT_vf[iemePositGroup][valcour] = TiVFi.y;
			// le dx
//			if(!ShouldWeCalculatePostionsToHaveJerkNull)
			{
				CPointDbl TiXi = m_WfPosition[iemePositGroup].GetPoint(valcour);
				if(TiXi.x != TiVFi.x)
				{
//					ShouldWeCalculatePostionsToHaveJerkNull = true;
					//	AfxMessageBox("Les abscisses de la WaveForm Position ne correpond pas aux abscisses de la WaveForm Vitesse", 1);
					PVTCalculatePositionToHaveJerk_0();
					PVT_Wf_to_dt_dx_vf();
					return;
				}
				m_PVT_dx[iemePositGroup][valcour] = TiXi.y - TotalDist;
				TotalDist += m_PVT_dx[iemePositGroup][valcour];
			}
		}
	}
//	if(ShouldWeCalculatePostionsToHaveJerkNull)
//	{
//		PVTCalculatePositionToHaveJerk_0();
//		PVT_Wf_to_dt_dx_vf();
//	}
}
bool CNewPortXPS::PVT_ResizeToTotalTime(double NewTotalTime)
{
	m_PVT_NumPoints = m_WfVitesse[1].GetnPoints() - 1;
	if(m_PVT_NumPoints < 2)
		return false;
	double Multiplicateur = NewTotalTime / m_WfVitesse[1].GetPoint(-1).x;
	// si ca change trop peu, byebye
	if(abs(Multiplicateur - 1.0) < 0.001)
		return false;
	For_All_Positioner_In_The_Group
	{
		for(int valcour = 1; valcour <= m_PVT_NumPoints; valcour++)
		{	
			m_WfVitesse[iemePositGroup].MovePoint(valcour, m_WfVitesse[iemePositGroup].GetPoint(valcour).x * Multiplicateur, m_WfVitesse[iemePositGroup].GetPoint(valcour).y / Multiplicateur);
			//	m_WfPosition.MovePoint(valcour, m_WfPosition.GetPoint(valcour).x * Multiplicateur, m_WfPosition.GetPoint(valcour).y);
		}
	}
	PVTCalculatePositionToHaveJerk_0();
	PVT_Wf_to_dt_dx_vf();
	return true;
}
bool CNewPortXPS::PVT_ResizeToVelocityAmplitude(int iemePositionner, double NewVelocityAmplitude)
{
	m_PVT_NumPoints = m_WfVitesse[iemePositionner].GetnPoints() - 1;
	if(m_PVT_NumPoints < 2)
		return false;
	double Multiplicateur = NewVelocityAmplitude / m_PvtMaxVelocityMaxAcceleration[iemePositionner].x;
	// si ca change trop peu, byebye
	if(abs(Multiplicateur - 1.0) < 0.001)
		return false;
	for(int valcour = 1; valcour <= m_PVT_NumPoints; valcour++)
	{	
		m_WfVitesse[iemePositionner].MovePoint(valcour, m_WfVitesse[iemePositionner].GetPoint(valcour).x, m_WfVitesse[iemePositionner].GetPoint(valcour).y * Multiplicateur);
	//	m_WfPosition.MovePoint(valcour, m_WfPosition.GetPoint(valcour).x, m_WfPosition.GetPoint(valcour).y * Multiplicateur * Multiplicateur);
	}
	PVTCalculatePositionToHaveJerk_0();
	PVT_Wf_to_dt_dx_vf();
	return true;
}
bool CNewPortXPS::PVT_ResizeToPositonAmplitude(int iemePositionner, double NewPositonAmplitude)
{
	if(m_PvtMinMaxAbsolutePosition[iemePositionner] == PVT_VERIFY_ERROR)
		return false;
	m_PVT_NumPoints = m_WfVitesse[1].GetnPoints() - 1;
	if(m_PVT_NumPoints < 2)
		return false;
	double Multiplicateur = NewPositonAmplitude / (m_PvtMinMaxAbsolutePosition[iemePositionner].y - m_PvtMinMaxAbsolutePosition[iemePositionner].x);
	// si ca change trop peu, byebye
	if(abs(Multiplicateur - 1.0) < 0.001)
		return false;
	for(int valcour = 1; valcour <= m_PVT_NumPoints; valcour++)
	{	
		m_WfVitesse[iemePositionner].MovePoint(valcour, m_WfVitesse[iemePositionner].GetPoint(valcour).x, m_WfVitesse[iemePositionner].GetPoint(valcour).y  * (Multiplicateur));
	}
	PVTCalculatePositionToHaveJerk_0();
	PVT_Wf_to_dt_dx_vf();
	return true;
}
int CNewPortXPS::PVTContentValuesGet(CWaveForme & UneWfVitesse, CWaveForme & UneWfPosition)
{
	CString content = PVTContentStringRead();
	if(content == "")
		return 0;
	// d'abord, on compte le nombre de lignes sachant qu'il n'y a que des lignes avec au moins un chiffre qui sort de PVTContentStringRead
	m_PVT_NumPoints = 0;
	if(content.FindOneOf("0123456789") == -1)
		return 0;
	// si il y a au moins un point
	m_PVT_NumPoints = 1;
	int pos = 0;
	pos = content.Find(A_LA_LIGNE, pos);
	while(-1 != pos)
	{
		m_PVT_NumPoints++;
		pos = content.Find(A_LA_LIGNE, pos + int(strlen(A_LA_LIGNE)));
	}
	// on prend les valeurs maintenant
	UneWfVitesse.ClearPointList();
	UneWfVitesse.AddPoint(0.0,0.0); // point initial : tj 0
	UneWfPosition.ClearPointList();
	UneWfPosition.AddPoint(0.0,0.0); // point initial : tj 0

	pos = 0;
	CString resTok;
	CString TheTokChars = " ,\t";
	resTok = content.Tokenize(TheTokChars, pos);
	// tant qu'on trouve un truc
	double TotalTime = 0.0;
	double TotalDist = 0.0;
	while(resTok != "")
	{
		double DT, DX, VF;
		char* afterDouble;
		DT = strtod(resTok, &afterDouble);
		resTok = content.Tokenize(TheTokChars, pos);
		DX = strtod(resTok, &afterDouble);
		// there should be a A_LA_LIGNE after this value, AND it may be stuck to the value...
		int postemp = pos;
		resTok = content.Tokenize(TheTokChars, postemp);
		if(-1 != resTok.Find(A_LA_LIGNE)) // si on trouve A_LA_LIGNE avec postemp
		{	// on recomence avec pos en éliminant le A_LA_LIGNE grace à Tokenize
			resTok = content.Tokenize(TheTokChars + A_LA_LIGNE, pos);
			VF = strtod(resTok, &afterDouble);
			// puis on sort du if_else -> ajout d'un point
		}
		else // sinon ca veut dire qu'on a la valeur toute seule dans le Token et, the next Tokenize Value should then start with a A_LA_LIGNE
		{
			pos = postemp;
			VF = strtod(resTok, &afterDouble);
			// trouvons le A_LA_LIGNE      
			resTok = content.Tokenize(TheTokChars, postemp);
			if(-1 != resTok.Find(A_LA_LIGNE))
			{	// si on trouve A_LA_LIGNE, on sort du if_else -> ajout d'un point
			}
			else if(resTok == "")
			{
			}
			else // erreur dans le .trj
			{
				AfxMessageBox("Error in The PVT File !\r\nOr the file is for several positionners...", MB_ICONERROR);
				return 0;
			}
		}
		// on change de ligne (on sait qu'il y en a une autre !)
		resTok = content.Tokenize(TheTokChars, pos); // on cherche la 1ere valeur de la ligne suivante
		TotalTime += DT;
		TotalDist += DX;
		UneWfVitesse.AddPoint(TotalTime, VF);
		UneWfPosition.AddPoint(TotalTime, TotalDist);
	}
	PVT_Wf_to_dt_dx_vf();
	return m_PVT_NumPoints;
}
CString CNewPortXPS::PVTFindAllFiles()
{
	if(!IsConnected())
		return "";

	if(m_pComboPVT)
		m_pComboPVT->ResetContent();

	// ouvrons ftp
	if(pConnectTrajectories)
	{	pConnectTrajectories->Close();
		delete pConnectTrajectories;
		pConnectTrajectories = NULL;
	}
	pConnectTrajectories = UneSessionInternet.GetFtpConnection(m_IPAdress, "Administrator", "Administrator", 21);
	pConnectTrajectories->SetCurrentDirectory("//Admin//public//Trajectories//");

	CString TheFiles("");
	// use a file find object to enumerate files
	CFtpFileFind finder(pConnectTrajectories);

	// start looping
	BOOL bWorking = finder.FindFile(_T("*.trj"));
	bWorking = finder.FindNextFile();
	// 1ere ligne
	CString NewLigne;
	NewLigne.Format("%s", finder.GetFileName());
	TheFiles = NewLigne;
	if(m_pComboPVT)
		m_pComboPVT->AddString(NewLigne);
	// suite
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		NewLigne.Format("%s", finder.GetFileName());
		TheFiles += CString("\n") + NewLigne;
		if(m_pComboPVT)
			m_pComboPVT->AddString(NewLigne);
	}
	// on ferme
	if(pConnectTrajectories)
	{	pConnectTrajectories->Close();
		delete pConnectTrajectories;
		pConnectTrajectories = NULL;
	}
//	CWaveForme::FindAllWaveFormesInAFolder("c:\\temp\\", m_pComboPVT);
//	m_WfPosition[2].LoadFromFileWf("");

return TheFiles;
}
CString CNewPortXPS::GroupNameFindAll()
{
	if(!IsConnected())
		return "";

	m_NbrGroup = 0;
	m_NbrPositionnerTotal = 0;
	if(m_pComboGroupName)
		m_pComboGroupName->ResetContent();
	if(m_pComboPositionerName)
		m_pComboPositionerName->ResetContent();

	// ouvrons une connection ftp:
	if(pConnectSystemIni)
	{	pConnectSystemIni->Close();
		delete pConnectSystemIni;
		pConnectSystemIni = NULL;
	}
	pConnectSystemIni = UneSessionInternet.GetFtpConnection(m_IPAdress, "Administrator", "Administrator", 21);
	pConnectSystemIni->SetCurrentDirectory("//Admin//CONFIG//");

	// là ou on mettra le contenu
	m_SystemIniFileContent = "";
	// use a file find object to enumerate files
	CFtpFileFind finder(pConnectSystemIni);
	// start looping
	BOOL bWorking = finder.FindFile("system.ini");
	if(bWorking)
	{
		CInternetFile* pTheFile = pConnectSystemIni->OpenFile("system.ini");
		// ligne par ligne
		CString OneLigne;
		// premiere
		bool NotFinished = pTheFile->ReadString(m_SystemIniFileContent)?true:false;
		// suivante
		while(NotFinished)
		{
			NotFinished = pTheFile->ReadString(OneLigne)?true:false;
			m_SystemIniFileContent += CString(A_LA_LIGNE) + OneLigne;
		}
		pTheFile->Close();
		delete pTheFile;
	}
	// on ferme
	if(pConnectSystemIni)
	{	pConnectSystemIni->Close();
		delete pConnectSystemIni;
		pConnectSystemIni = NULL;
	}

	// Maintenant, on cherche les noms de GroupName
	// recherche de "[]"
	int indexPos = 0;
	CString strName;
	while(m_SystemIniFileContent.Find("[", indexPos) != -1)
	{
		indexPos = m_SystemIniFileContent.Find("[", indexPos);
		strName = m_SystemIniFileContent.Tokenize("[]", indexPos);
		// si le nom est pas [GENERAL], ou [GROUPS]
		if(strName != "GENERAL" && strName != "GROUPS")
		{
			// et si on a pas de nom de positionneur : c'est que c'est un groupname : on l'ajoute
			int posPositioner = 0;
			if((posPositioner = strName.Find(".")) == -1)
			{
				m_NbrGroup++;
				if(m_pComboGroupName)
					m_pComboGroupName->AddString(strName);
			}
			// sinon, on l'ajoute au positonner...
			else
			{
				m_NbrPositionnerTotal++;
//				strName.Delete(0, posPositioner + 1);
//				if(m_pComboPositionerName)
//					m_pComboPositionerName->AddString(strName);
			}
		}
	}
	if(m_pComboGroupName)
		m_pComboGroupName->SetCurSel(0);
	if(m_pComboPositionerName)
		m_pComboPositionerName->SetCurSel(0);
return m_SystemIniFileContent;
}
void CNewPortXPS::PVTCalculatePositionToHaveJerk_0()
{
	int nPoints = m_WfVitesse[1].GetnPoints() - 1;
	For_All_Positioner_In_The_Group
	{
		m_WfPosition[iemePositGroup].ClearPointList();
		m_WfPosition[iemePositGroup].AddPoint(0.0,0.0); // point initile : tj 0, car on parle tj en relatif

		double TotalTime = 0.0;
		double TotalDist = 0.0;
		// le DX est seulement l'intégrale de la vitesse !
		for(int valcour = 1; valcour <= nPoints; valcour++)
		{
			CPointDbl TiVFi = m_WfVitesse[iemePositGroup].GetPoint(valcour);
			CPointDbl TiVFi_moins1 = m_WfVitesse[iemePositGroup].GetPoint(valcour - 1);
			double DT = TiVFi.x - TotalTime;
			TotalTime += DT;
			double VF = TiVFi.y;
			double VI = TiVFi_moins1.y;
			// calcul p85 XPS User's Manual
			double DX = DT * (VI + VF) / 2;
			TotalDist += DX;
			m_WfPosition[iemePositGroup].AddPoint(TotalTime, TotalDist);
		}
	}
	PVT_Wf_to_dt_dx_vf();
}
CPointDbl CNewPortXPS::PVTCalculate_Jerk_AccIni_LikeXPS(double DT, double DX, double VI, double VF)
{
	double JJJEEERRRKKK = 6 * (DT * (VI + VF) - 2 * DX) / DT / DT / DT;
	double AccIni = 2 * (3 * DX - DT * (2 * VI + VF)) / DT / DT;
	double AccFin = 2 * (DT * (VI + 2 * VF) - 3 * DX) / DT / DT;
	return CPointDbl(JJJEEERRRKKK, AccIni);
}
CPointDbl CNewPortXPS::PVTCalculate_Jerk_AccIni_LikeXPS(int iemePVT, int iemeSegment_Start1)
{
	if(!m_PVT_dt || !m_PVT_dx || !m_PVT_vf)
		return CPointDbl(0.0, 0.0);
//	return PVTCalculate_Jerk_AccIni_LikeXPS(m_PVT_dt[iemeSegment_Start1], m_PVT_dx[iemePVT][iemeSegment_Start1], m_PVT_vf[iemePVT][iemeSegment_Start1 - 1], m_PVT_vf[iemePVT][iemeSegment_Start1]);
}
CPointDbl CNewPortXPS::PVT_XPS_Interpolate_Page85(int iemePVT, double WhichTime)
{
//	if(!m_PVT_dt || !m_PVT_dx || !m_PVT_vf)
//		return CPointDbl(0.0, 0.0);
	if(WhichTime < 0 || WhichTime > m_WfPosition[iemePVT].GetPoint(-1).x)
		return CPointDbl(0.0, 0.0);
	if(m_WfVitesse[iemePVT].GetnPoints() < 2)
		return CPointDbl(0.0, 0.0);
	// recherche du segment de Wf:
	int NumeroSegment = 1;
	while(WhichTime > m_WfPosition[iemePVT].GetPoint(NumeroSegment).x)
	{
		NumeroSegment++;
	}
	double TimeUpToSegment = m_WfPosition[iemePVT].GetPoint(NumeroSegment - 1).x;
	double tDansSegment = WhichTime - TimeUpToSegment;
	// Maintenant : p.85 du XPS User's Manual.
	CPointDbl JerkAcc = PVTCalculate_Jerk_AccIni_LikeXPS(iemePVT, NumeroSegment);
	double VI = m_WfVitesse[iemePVT].GetPoint(NumeroSegment - 1).y;
	double XI = m_WfPosition[iemePVT].GetPoint(NumeroSegment - 1).y;

	double Vt = VI + JerkAcc.y * tDansSegment + JerkAcc.x * tDansSegment * tDansSegment / 2;
	double Xt = XI + VI * tDansSegment + JerkAcc.y * tDansSegment * tDansSegment / 2 + JerkAcc.x * tDansSegment * tDansSegment * tDansSegment / 6;

    return CPointDbl(Xt, Vt);
}

void CNewPortXPS::TrackingPosition(int iemePositioner, int WhichAI_GPIO2)
{
	TrackingDisable();
	if(iemePositioner > m_NbrPositionnerInCurrentGroup || AfxMessageBox("This will first Home the translation!\n\
Also, the position of the translation will be contuniously \n\
output on the Corresponding Analog Output!\n\n\nProceed?", MB_OKCANCEL) != IDOK)
		return;

	RemoveAllEventAndAction();
	InitializeHome();
	CString	strAnalog; strAnalog.Format("GPIO2.ADC%d", WhichAI_GPIO2);
	char* pGPIOname(strAnalog.GetBuffer());

//	// Available ranges for the analog intputs are ±10 V, ±5 V, ±2.5 V and ±1.25 V (Gain is 1, 2, 4 and 8 respectively).
//	// which defines the précision you can obtain with the 14bits of coding : 1.25mV, 0.6mV, 0.3mV, 0.15mV
//	// p.167 XPSDocumentation V2.0.0.pdf
//	int gain = 2;
//	m_ErrorXPS = GPIOAnalogGainSet(m_SocketID, 1, pGPIOname, &gain);
//	if("OK" != XPSError(""))
//		return;

	// p.70 XPSDocumentation V2.0.0.pdf
	m_ErrorXPS = PositionerAnalogTrackingPositionParametersSet(m_SocketID, m_pGroupPositionnerName[iemePositioner], pGPIOname
	//	offset		, DistanceUnit/V
		, 0.0		, 10.0				, m_LimitMaxVelocity[iemePositioner], m_LimitMaxAcceleration[iemePositioner]);
	if("OK" != XPSError(""))
		return;
	m_ErrorXPS = GroupAnalogTrackingModeEnable(m_SocketID, m_pCurrentGroupName, "Position");
	if("OK" != XPSError(""))
		return;

	// d'abord, on remove l'event
	if(m_EventForOutputPosition != -1)
	{
		m_ErrorXPS = EventExtendedRemove(m_SocketID, m_EventForOutputPosition);
		if("OK" != XPSError("EventExtendedRemove"))
		{;}
		m_EventForOutputPosition = -1;
	}

	// Pour outputer la postion sur les analog output
	////// EVENT //////
	char pEvent[250]; // quel Event
	sprintf(pEvent, "Always");
	////// ACTION //////
	char pAction[250];
	sprintf(pAction, "GPIO2.DAC%d.DACSet.SetpointPosition", WhichAI_GPIO2);
	// 
	m_ErrorXPS = EventExtendedConfigurationTriggerSet(m_SocketID, 1, pEvent, "0", "0", "0", "0");
	if("OK" != XPSError("ExtendedConfigurationTriggerSet"))
		return;
	m_ErrorXPS = EventExtendedConfigurationActionSet(m_SocketID, 1,	pAction, m_pGroupPositionnerName[iemePositioner], "0.1", "0", "0");
	if("OK" != XPSError("EventExtendedConfigurationActionSet"))
		return;
	m_ErrorXPS = EventExtendedStart(m_SocketID, &m_EventForOutputPosition);
	if("OK" != XPSError("EventForOutputPosition"))
		return;
}
void CNewPortXPS::TrackingVelocity(int iemePositioner, int WhichAI_GPIO2)
{
	TrackingDisable();
	if(iemePositioner > m_NbrPositionnerInCurrentGroup || AfxMessageBox("This will first Home the translation!\n\nProceed?") != IDOK)
		return;
	RemoveAllEventAndAction();
	InitializeHome();
	CString	strAnalog; strAnalog.Format("GPIO2.ADC%d", WhichAI_GPIO2);
	char* pGPIOname(strAnalog.GetBuffer());

	m_ErrorXPS = PositionerAnalogTrackingVelocityParametersSet(m_SocketID, m_pGroupPositionnerName[iemePositioner], pGPIOname, 0.0, 100.0, 0.005, 1, m_LimitMaxVelocity[iemePositioner], m_LimitMaxAcceleration[iemePositioner]);
	if("OK" != XPSError(""))
		return;
}
void CNewPortXPS::TrackingDisable()
{
	m_ErrorXPS = GroupAnalogTrackingModeDisable(m_SocketID, m_pCurrentGroupName);
	if("OK" != XPSError(""))
		return;
}


void CNewPortXPS::Record_PositionVelocity(int WhichDI_GPIO1_ForAquiring, double& AquiringFreq, double& AquiringDuration)
{
	int nbrMesures = 2;
	CString What1, What2;
	What1.Format("%s.CurrentPosition", m_pGroupPositionnerName);
	What2.Format("%s.CurrentVelocity", m_pGroupPositionnerName);
	CString Whattout = What1 + CString(" ") + What2;
	m_ErrorXPS = GatheringConfigurationSet(m_SocketID, nbrMesures, Whattout.GetBuffer());
	if("OK" != XPSError(""))
		return;

	// echantillonnage periode = un multiple de 100µs
	int Divisor = max(1, int(1e4 / AquiringFreq));
	int NbPoints = min(1000000 / nbrMesures, 1 + max(0, int(AquiringDuration / (1e-4 * Divisor))));
	AquiringFreq = 1.0 / (1e-4 * double(Divisor));
	AquiringDuration = 1e-4 * double(Divisor) * double(NbPoints - 1);
	CString TellMe;
	TellMe.Format("Those value will be used:\n\nSampling Frequency = %.4g Hz\nDuration = %.4g s\n\nA pulse on GPIO1.DO2 will be output when the Recording starts."
		, AquiringFreq
		, AquiringDuration);
	AfxMessageBox(TellMe, MB_OK);

////// EVENT //////
	char pEvent[250]; // quel Event
	char pBit[8]; // index du bit en base 2
	sprintf(pEvent, "GPIO1.DI.DIHighLow %s", "Always");
	sprintf(pBit, "%d 0", int(pow(2.0, double(WhichDI_GPIO1_ForAquiring - 1))) - 1);
////// ACTION //////
	char pAction[250] = "GatheringRun GPIO1.DO.DOPulse";
	char pActionParam1[250];
	char pActionParam2[250];
	sprintf(pActionParam1, "%d 2", NbPoints);
	sprintf(pActionParam2, "%d 0", Divisor);
	////////////////////////////////////////////////////////////////////////////
	// D'abord, on enleve un éventuel déjà trig
	Record_Trigger_Remove();
	////////////////////////////////////////////////////////////////////////////
	// DOSet au recevoir d'un trig
	m_ErrorXPS = EventExtendedConfigurationTriggerSet(m_SocketID, 2, pEvent, pBit, "0 0", "0 0", "0 0");
	if("OK" != XPSError("ExtendedConfigurationTriggerSet"))
		return;
	m_ErrorXPS = EventExtendedConfigurationActionSet(m_SocketID, 2,	pAction, pActionParam1, pActionParam2, "0 0", "0 0");
	if("OK" != XPSError("EventExtendedConfigurationActionSet"))
		return;
	m_ErrorXPS = EventExtendedStart(m_SocketID, &m_EventTriggerForGathering);
	if("OK" != XPSError("EventExtendedStart"))
		return;
}
void CNewPortXPS::Record_AnalogTrack1And2AndPosition1And2(int WhichDI_GPIO1_ForAquiring, double& AquiringFreq, double& AquiringDuration)
{
	int nbrMesures = 4;
	CString What1, What2, What3, What4;
	What1.Format("GPIO2.ADC1");
	What2.Format("GPIO2.ADC2");

	// trouvons les positionneurs
	CString GroupName = "";
	int totalgroup = m_pComboGroupName->GetCount();
	int selecCourant = m_pComboGroupName->GetCurSel();
	if(totalgroup >= 1)
	{
		m_pComboGroupName->GetLBText(0, GroupName);
		SetNewGroupName(GroupName);
		What3.Format("%s.CurrentPosition", m_pGroupPositionnerName);
		m_pComboGroupName->SetCurSel(0);
	}
	if(totalgroup >= 2)
	{
		m_pComboGroupName->GetLBText(1, GroupName);
		SetNewGroupName(GroupName);
		What4.Format("%s.CurrentPosition", m_pGroupPositionnerName);
		m_pComboGroupName->SetCurSel(1);
	}
	m_pComboGroupName->SetCurSel(selecCourant);

	CString Whattout = What1 + CString(" ") + What2 + CString(" ") + What3 + CString(" ") + What4;
	m_ErrorXPS = GatheringConfigurationSet(m_SocketID, nbrMesures, Whattout.GetBuffer());
	if("OK" != XPSError(""))
		return;

	// echantillonnage periode = un multiple de 100µs
	int Divisor = max(1, int(1e4 / AquiringFreq));
	int NbPoints = min(1000000 / nbrMesures, 1 + max(0, int(AquiringDuration / (1e-4 * Divisor))));
	AquiringFreq = 1.0 / (1e-4 * double(Divisor));
	AquiringDuration = 1e-4 * double(Divisor) * double(NbPoints - 1);
	CString TellMe;
	TellMe.Format("Those value will be used:\n\nSampling Frequency = %.4g Hz\nDuration = %.4g s\n\nA pulse on GPIO1.DO2 will be output when the Recording starts."
		, AquiringFreq
		, AquiringDuration);
	AfxMessageBox(TellMe, MB_OK);

////// EVENT //////
	char pEvent[250]; // quel Event
	char pBit[8]; // index du bit en base 2
	sprintf(pEvent, "GPIO1.DI.DIHighLow %s", "Always");
	sprintf(pBit, "%d 0", int(pow(2.0, double(WhichDI_GPIO1_ForAquiring - 1))) - 1);
////// ACTION //////
	char pAction[250] = "GatheringRun GPIO1.DO.DOPulse";
	char pActionParam1[250];
	char pActionParam2[250];
	sprintf(pActionParam1, "%d 2", NbPoints);
	sprintf(pActionParam2, "%d 0", Divisor);
	////////////////////////////////////////////////////////////////////////////
	// D'abord, on enleve un éventuel déjà trig
	Record_Trigger_Remove();
	////////////////////////////////////////////////////////////////////////////
	// DOSet au recevoir d'un trig
	m_ErrorXPS = EventExtendedConfigurationTriggerSet(m_SocketID, 2, pEvent, pBit, "0 0", "0 0", "0 0");
	if("OK" != XPSError("ExtendedConfigurationTriggerSet"))
		return;
	m_ErrorXPS = EventExtendedConfigurationActionSet(m_SocketID, 2,	pAction, pActionParam1, pActionParam2, "0 0", "0 0");
	if("OK" != XPSError("EventExtendedConfigurationActionSet"))
		return;
	m_ErrorXPS = EventExtendedStart(m_SocketID, &m_EventTriggerForGathering);
	if("OK" != XPSError("EventExtendedStart"))
		return;
}
void CNewPortXPS::Record_Trigger_Remove()
{
	if(m_EventTriggerForGathering != -1)
	{
		m_ErrorXPS = EventExtendedRemove(m_SocketID, m_EventTriggerForGathering);
		if("OK" != XPSError("EventExtendedRemove"))
		{;}
		m_EventTriggerForGathering = -1;
	}
}
double CNewPortXPS::Record_SaveAndRapatrie(CString NewLocalFileName, CNiReal64Matrix * pMatrix)
{
	m_ErrorXPS = GatheringStopAndSave(m_SocketID);
	if("OK" != XPSError("GatheringStopAndSave"))
		return 0;//CString("Error :\r\n") + m_StrXPS;
	// ouvrons ftp
	if(pConnectGatheredData)
	{	pConnectGatheredData->Close();
		delete pConnectGatheredData;
		pConnectGatheredData = NULL;
	}
	pConnectGatheredData = UneSessionInternet.GetFtpConnection(m_IPAdress, "Administrator", "Administrator", 21);
	pConnectGatheredData->SetCurrentDirectory("//Admin/public//");

	// là ou on mettra le contenu
	CString TheFileContent("");
	// on rapatrie maintenant
	pConnectGatheredData->GetFile(GATHERED_DATA_FILE_NAME, NewLocalFileName, FALSE);

	// on ferme
	if(pConnectGatheredData)
	{	pConnectGatheredData->Close();
		delete pConnectGatheredData;
		pConnectGatheredData = NULL;
	}
    CSettingSave PourLireLeContenu;
	PourLireLeContenu.ReadTextFile(NewLocalFileName, TheFileContent);
	// voila a quoi ressemble un fihier type
	//SamplePeriod 0 0
	//GatheringTypeA GatheringTypeB GatheringTypeC
	//ValueA1 ValueB1 ValueC1
	//ValueA2 ValueB2 ValueC2
	//… … …
	//ValueAN ValueBN ValueCN
	double steptime = 0;
	int nCol = 0;
	int nRow = 0;
	// recherche du nombre de colonne
	int pos =0; char* stop = NULL;
	CString toktok = TheFileContent.Tokenize(" \t", pos);
	if(toktok.FindOneOf("0123456789") != -1)
		steptime = strtod(toktok, &stop);
	if(steptime == 0.0)
		return steptime;
	if(!pMatrix)
		return steptime;
	CNiReal64Matrix & TheMatrix = (*pMatrix);

	nCol++;
	// y a t'il des colonne en plus : recherche de "0" sur la meme ligne
	while((toktok = TheFileContent.Tokenize(" \t\r\n", pos)) == "0")
		nCol++;
	// a ce moment là, le token est le nom de la premiere grandeur regardée : GatheringTypeA
	// alors allons à la 1ere valeur du tableau
	for(int i = 1; i <= nCol; i++)
		toktok = TheFileContent.Tokenize(" \t\r\n", pos);
	// remplissage de la matrice avec les éléments du tableau 
	double val = 0; int indElem = -1; int indCol = 0;
	while(toktok != "")
	{
		indElem++;
		indCol = indElem % nCol;
		val = strtod(toktok, &stop);
		// si on commence une nouvelle ligne
		if(indCol == 0)
		{
			nRow++;
			TheMatrix.Resize(nRow, nCol);
		}
		TheMatrix(nRow - 1, indCol) = val;

		toktok = TheFileContent.Tokenize(" \t\r\n", pos);
	}
return steptime;
}
double CNewPortXPS::Record_SaveAndRapatrieInDefaultFolder(CString NewLocalFileName, int chiffre1, int chiffre2, int chiffre3, int chiffre4, int chiffre5, int chiffre6)
{
	CString LocalFolder = DEFAULT_LOCAL_FOLDER_RECORD_POSITION_VITESSE;
	CString filename = LocalFolder + NewLocalFileName;
	CString str[7];
	for(int iemeChiffre = 1; iemeChiffre <= 6; iemeChiffre++)
	{
		// quel chiffre_i
		int ci = 0;
		switch(iemeChiffre)
		{	case 1: ci = chiffre1; break;	case 2: ci = chiffre2; break;	case 3: ci = chiffre3; break;	case 4: ci = chiffre4; break;	case 5: ci = chiffre5; break;	case 6: ci = chiffre6; break;	}
		// un truc qui vaut 1 si >=10, 2 si >= 100, etc..
		int nbr0 = max(0, int(log10(max(0.1, double(ci)))));
		// si on a effectivement précisé chiffre_i
		if(ci != -1)
			str[iemeChiffre].Format("_%s%s%s%d", nbr0 < 1 ?"0":"", nbr0 < 2 ?"0":"", nbr0 < 3 ?"0":"", ci);
		else
			str[iemeChiffre] = "";
		filename += str[iemeChiffre];
	}
	return 0;//Record_SaveAndRapatrie(filename + ".dat");
}