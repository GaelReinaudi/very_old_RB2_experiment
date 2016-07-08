// SpeakWithOriginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SpeakWithOriginDlg.h"

//Included for Origin automation server access 
#include "OriginClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSpeakWithOriginDlg::CSpeakWithOriginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpeakWithOriginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpeakWithOriginDlg)
	m_bShowOriginWindow = TRUE;
	m_bExitOriginOnDisconnect = FALSE;
	m_bAppendData = FALSE;
	m_strWksName = _T("Data1");
	m_bSaveProjectOnExit = FALSE;
	m_strFileName = _T("c:\\aze753");
	m_nColumns = 2;
	m_nRows = 5;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//Initialize member variables
	m_nConnOption = USE_EXIST_FIRST;
	m_pClient	  = NULL;

	OnAlwaysCreateNew();
}
CSpeakWithOriginDlg::~CSpeakWithOriginDlg()
{
	OnDisconnectOrigin();
}

void CSpeakWithOriginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeakWithOriginDlg)
	//DDX_Control(pDX, IDC_WKS_NAME, m_editWksName);
	//DDX_Control(pDX, IDC_SAVE_AS_FILE_NAME, m_editFileName);
	//DDX_Control(pDX, IDC_APPEND_DATA, m_btnAppendData);
	//DDX_Control(pDX, IDC_SAVE_PROJECT_ON_EXIT, m_btnSaveProject);
	//DDX_Control(pDX, IDC_SEND_DATA, m_btnSendData);
	//DDX_Control(pDX, IDC_CLEAR_WORKSHEET, m_btnClearWorksheet);
	//DDX_Control(pDX, IDC_DISCONNECT_ORIGIN, m_btnDisconnect);
	//DDX_Control(pDX, IDC_CONNECT_ORIGIN, m_btnConnect);
	//DDX_Check(pDX, IDC_SHOW_ORIGIN_WINDOW, m_bShowOriginWindow);
	//DDX_Check(pDX, IDC_EXIT_ORIGIN_ON_DISCONNECT, m_bExitOriginOnDisconnect);
	//DDX_Check(pDX, IDC_APPEND_DATA, m_bAppendData);
	//DDX_Text(pDX, IDC_WKS_NAME, m_strWksName);
	//DDX_Check(pDX, IDC_SAVE_PROJECT_ON_EXIT, m_bSaveProjectOnExit);
	//DDX_Text(pDX, IDC_SAVE_AS_FILE_NAME, m_strFileName);
	//DDX_Text(pDX, IDC_WKS_COLUMNS, m_nColumns);
	//DDX_Text(pDX, IDC_WKS_ROWS, m_nRows);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpeakWithOriginDlg, CDialog)
	//{{AFX_MSG_MAP(CSpeakWithOriginDlg)
	//ON_WM_SYSCOMMAND()
	//ON_WM_PAINT()
	//ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_ALWAYS_CREATE_NEW, OnAlwaysCreateNew)
	//ON_BN_CLICKED(IDC_CONNECT_EXISTING_FIRST, OnConnectExistingFirst)
	//ON_BN_CLICKED(IDC_CONNECT_ORIGIN, OnConnectOrigin)
	//ON_BN_CLICKED(IDC_DISCONNECT_ORIGIN, OnDisconnectOrigin)
	//ON_BN_CLICKED(IDC_SHOW_ORIGIN_WINDOW, OnShowOriginWindow)
	//ON_BN_CLICKED(IDC_SEND_DATA, OnSendData)
	//ON_BN_CLICKED(IDC_CLEAR_WORKSHEET, OnClearWorksheet)
	//ON_BN_CLICKED(IDC_SAVE_PROJECT_ON_EXIT, OnSaveProjectOnExit)
	//ON_BN_CLICKED(IDC_EXIT_ORIGIN_ON_DISCONNECT, OnExitOriginOnDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeakWithOriginDlg message handlers

BOOL CSpeakWithOriginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
//	EnableControls(FALSE);
	
	//CButton* pButton = static_cast<CButton*>( GetDlgItem(IDC_CONNECT_EXISTING_FIRST) );
	//if( pButton != NULL )
	//	pButton->SetCheck(BST_CHECKED);	 	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpeakWithOriginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpeakWithOriginDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpeakWithOriginDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////
//Override CDialog methods
/////////////////////////////////////////////////////////////////////////
BOOL CSpeakWithOriginDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_pClient != NULL )
	{
		delete m_pClient;
		m_pClient = NULL;
	}

	return CDialog::DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////
//Private utility functions
/////////////////////////////////////////////////////////////////////////

void CSpeakWithOriginDlg::EnableControls(BOOL bEnable)
{
	m_btnDisconnect.EnableWindow(bEnable);
	m_btnClearWorksheet.EnableWindow(bEnable);
	m_btnSendData.EnableWindow(bEnable);
	m_btnAppendData.EnableWindow(bEnable);
	m_btnSaveProject.EnableWindow(bEnable);

	m_btnConnect.EnableWindow(!bEnable); 
}



CString CSpeakWithOriginDlg::GetSaveAsFile()
{
	CString str = "";
	if( !IS_VALID_ORIGIN_PTR(m_pClient) )
		return str;

	_bstr_t bstr;
	bstr =  m_pClient->operator ->()->GetLTStr(_bstr_t("%Y"));
	str = _com_util::ConvertBSTRToString(bstr);

	bstr = 	m_pClient->operator ->()->GetLTStr(_bstr_t("%G"));
	str += _com_util::ConvertBSTRToString(bstr); 

	return str;
}


BOOL CSpeakWithOriginDlg::ExecuteOriginCMD(LPCSTR lpszCMD)
{
	if( !IS_VALID_ORIGIN_PTR(m_pClient) )
		return FALSE;

	return m_pClient->operator ->()->Execute( _bstr_t(lpszCMD) );
}


/////////////////////////////////////////////////////////////////////////
//Event handler functions for buttons
/////////////////////////////////////////////////////////////////////////	 

void CSpeakWithOriginDlg::OnConnectOrigin() 
{
	// TODO: Add your control notification handler code here
	OnDisconnectOrigin();
	//Create a new COriginClient object, which will hold the reference to 
	//an Origin automation server instance 
	m_pClient = new COriginClient(USE_EXIST_FIRST+0*m_nConnOption);
	if( !IS_VALID_ORIGIN_PTR(m_pClient) )
	{
		MessageBox("Failure to access Origin instance", "", MB_OK);

		if( m_pClient != NULL )
		{
			delete m_pClient;
			m_pClient = NULL;
		}
		return;
	}

	//apply the show Origin window option
	OnShowOriginWindow();
	
	//Enable controls after connected to Origin
//	EnableControls(TRUE);

	//Origin save as file name
//	m_editFileName.SetWindowText( GetSaveAsFile() );

	//Turn on session saving asking block
	ExecuteOriginCMD("doc -ss 1");

	//Seed the random-number generator with current time so that
    //the numbers will be different every time we run.
    srand( (unsigned)time( NULL ) );  
}	  

void CSpeakWithOriginDlg::OnDisconnectOrigin()
{
	// TODO: Add your control notification handler code here
//	UpdateData();

	if( IS_VALID_ORIGIN_PTR(m_pClient) )
	{
		try
		{
			//Exit Origin if the "Exit Origin when Disconnect" option was checked 
			if( m_bExitOriginOnDisconnect )
				ExecuteOriginCMD("exit");
			//Save project if "Save Origin project on exit" option was checked
			if( m_bSaveProjectOnExit )
				m_pClient->operator ->()->Save(_bstr_t(m_strFileName));
		}
		catch(int i)
		{i;}
		delete m_pClient;
		m_pClient = NULL;
	}
	
	//Enable controls after Disconnected from Origin	
//	EnableControls(FALSE);
}	
void CSpeakWithOriginDlg::Save(CString PathSave)
{
	if(PathSave == "")
		PathSave = m_strFileName;
	else
		m_strFileName = PathSave;

	ExecuteOriginCFunction("SaveProject", m_strFileName);
//	m_pClient->operator ->()->Run();
//	m_pClient->operator ->()->Save(_bstr_t(PathSave));
//	m_pClient->operator ->()->Run();
}

void CSpeakWithOriginDlg::OnSendData() 
{
	// TODO: Add your control notification handler code here
	if( !IS_VALID_ORIGIN_PTR(m_pClient) )
		return;
	 
//	UpdateData();

	//Validate worksheet name
	if( m_strWksName.IsEmpty() )
	{
		MessageBox("Please specify a worksheet name first.", "", MB_OK);
		return;	  
	}

	//Validate m_nColumns and m_nRows 
	if( m_nRows == 0 || m_nColumns == 0 )
	{
		MessageBox("Columns or Rows' value can not be 0.", "", MB_OK);
		return;	  
	}  

	//Check if specified worksheet exists
    CString str = "d=exist(" + m_strWksName + ")";
	int nRet = 0;
    if( ExecuteOriginCMD(str) )
	{
		double fStat = m_pClient->operator ->()->GetLTVar(_bstr_t("d")) ;
        nRet = static_cast<int>( fStat );
	}
    
    //If specified worksheet does not exist then create it
    if( nRet != 2 )
	{
        //If another window has the requested name then our name will be
        //enumerated and the enumerated name will be returned.  So we
        //will put the return value into the edit box in case it is changed.
		_bstr_t bstr = m_pClient->operator ->()->CreatePage(ORIGIN_WINTYPE_WKS, _bstr_t(m_strWksName), _bstr_t("w"));
        m_strWksName = _com_util::ConvertBSTRToString(bstr); 
		
		m_editWksName.SetWindowText(m_strWksName);
	}

	//Get the first target row based on the Append check box
	UINT nRowStart = 0;
	if( m_bAppendData )
	{
		_bstr_t bstr;
		str = m_strWksName + "!wks.col1.name$";
        bstr = m_pClient->operator ->()->GetLTStr(_bstr_t(str));
		str = _com_util::ConvertBSTRToString(bstr); 
        
		CString strDataset = m_strWksName + "_" + str;
        str = "get " + strDataset + " -e i";
        
		if( ExecuteOriginCMD(str) )
		{
			double fRow = m_pClient->operator ->()->GetLTVar(_bstr_t("i")) ;
			nRowStart = static_cast<UINT>(fRow); 		
		}
	}  

	//Make source data
    COleSafeArray csa;
	DWORD bounds[2] = {m_nRows, m_nColumns};
	csa.Create(VT_R8, 2, bounds);

	long index[2];
	UINT nLowerBound = nRowStart;
	UINT nUpperBound = m_nRows + nRowStart;

	for( UINT ii=0; ii < m_nColumns; ii++ )
	{ 	
		for( UINT jj=0; jj < m_nRows; jj++ )
		{					
			index[0]=jj;
			index[1]=ii;

			double fVal = 0.0;
			if( ii == 0 )
				fVal = nLowerBound + jj + 1;
			else
				fVal = ((nLowerBound + jj + 1) * 10 + ii) * rand()/RAND_MAX;
			
			csa.PutElement(index, &fVal);			
		}
	} 	

	//Put generated numeric data into Origin
	m_pClient->SetWorksheet(m_strWksName, csa, nRowStart);

}

void CSpeakWithOriginDlg::OnClearWorksheet() 
{
	// TODO: Add your control notification handler code here
	if( !IS_VALID_ORIGIN_PTR(m_pClient) )
		return;

//	UpdateData();

	CString str = "clearworksheet " + m_strWksName;	      
	ExecuteOriginCMD(str);
}


/////////////////////////////////////////////////////////////////////////
//Event handler functions for check boxes or radio buttons
/////////////////////////////////////////////////////////////////////////  
void CSpeakWithOriginDlg::OnAlwaysCreateNew() 
{
	// TODO: Add your control notification handler code here
	m_nConnOption = ALWAYS_CREATE_NEW;

}

void CSpeakWithOriginDlg::OnConnectExistingFirst() 
{
	// TODO: Add your control notification handler code here
	m_nConnOption = USE_EXIST_FIRST;
}	 


void CSpeakWithOriginDlg::OnShowOriginWindow() 
{
	// TODO: Add your control notification handler code here
	if( !IS_VALID_ORIGIN_PTR(m_pClient) )
		return;

//	UpdateData();
	
	CString strCMD = "doc -mk ";
	strCMD += m_bShowOriginWindow ? "1" : "0";	
	
	ExecuteOriginCMD(strCMD);
}


void CSpeakWithOriginDlg::OnSaveProjectOnExit() 
{
	// TODO: Add your control notification handler code here
//	UpdateData();

	m_editFileName.EnableWindow(m_bSaveProjectOnExit);	
}

void CSpeakWithOriginDlg::OnExitOriginOnDisconnect() 
{
	// TODO: Add your control notification handler code here
//	UpdateData();
}

void CSpeakWithOriginDlg::ExecuteOriginCFunction(CString FuncName, CString Opt1, int Optnum1, int Optnum2, int Optnum3, int Optnum4, int Optnum5, int Optnum6, int Optnum7, int Optnum8, int Optnum9, int Optnum10)
{
	CString strNum[10];
	strNum[0 ].Format("%d", Optnum1 );
	strNum[1 ].Format("%d", Optnum2 );
	strNum[2 ].Format("%d", Optnum3 );
	strNum[3 ].Format("%d", Optnum4 );
	strNum[4 ].Format("%d", Optnum5 );
	strNum[5 ].Format("%d", Optnum6 );
	strNum[6 ].Format("%d", Optnum7 );
	strNum[7 ].Format("%d", Optnum8 );
	strNum[8 ].Format("%d", Optnum9 );
	strNum[9 ].Format("%d", Optnum10);
	ExecuteOriginCFunction(FuncName, Opt1, strNum[0 ], strNum[1 ], strNum[2 ], strNum[3 ], strNum[4 ], strNum[5 ], strNum[6 ], strNum[7 ], strNum[8 ], strNum[9 ]);
}		   

void CSpeakWithOriginDlg::ExecuteOriginCFunction(CString FuncName, CString Opt1, CString Opt2, CString Opt3, CString Opt4, CString Opt5, CString Opt6, CString Opt7, CString Opt8, CString Opt9, CString Opt10, CString Opt11)
{
	CString strCMD;
	strCMD.Format("%s(%s%s%s%s%s%s%s%s%s%s);"
				  , FuncName
				  , Opt1 == "-1156" ? "" : CString("") + Opt1
				  , Opt2 == "-1156" ? "" : CString(", ") + Opt2
				  , Opt3 == "-1156" ? "" : CString(", ") + Opt3
				  , Opt4 == "-1156" ? "" : CString(", ") + Opt4
				  , Opt5 == "-1156" ? "" : CString(", ") + Opt5
				  , Opt6 == "-1156" ? "" : CString(", ") + Opt6
				  , Opt7 == "-1156" ? "" : CString(", ") + Opt7
				  , Opt8 == "-1156" ? "" : CString(", ") + Opt8
				  , Opt9 == "-1156" ? "" : CString(", ") + Opt9
				  , Opt10 == "-1156" ? "" : CString(", ") + Opt10
				  , Opt11 == "-1156" ? "" : CString(", ") + Opt11);
	ExecuteOriginCMD(strCMD);
}
void CSpeakWithOriginDlg::SetNameFileSave(CString PathSave)
{
	m_strFileName = PathSave;
}