// SpeakWithOriginDlg.h : header file
//

#if !defined(AFX_SpeakWithOriginDlg_H__BF1AE429_10D8_4B57_BB14_0046D53038A2__INCLUDED_)
#define AFX_SpeakWithOriginDlg_H__BF1AE429_10D8_4B57_BB14_0046D53038A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Forward declaration of OriginClient class
class COriginClient;

/////////////////////////////////////////////////////////////////////////////
// CSpeakWithOriginDlg dialog

class CSpeakWithOriginDlg : public CDialog
{
// Construction
public:
	CSpeakWithOriginDlg(CWnd* pParent = NULL);	// standard constructor
	~CSpeakWithOriginDlg();	// standard destructor

// Dialog Data
	//{{AFX_DATA(CSpeakWithOriginDlg)
	enum { IDD = IDD_DIALOG_SPEAK_WITH_ORIGIN };
	CEdit	m_editWksName;
	CEdit	m_editFileName;
	CButton	m_btnAppendData;
	CButton	m_btnSaveProject;
	CButton	m_btnSendData;
	CButton	m_btnClearWorksheet;
	CButton	m_btnDisconnect;
	CButton	m_btnConnect;
	BOOL	m_bShowOriginWindow;
	BOOL	m_bExitOriginOnDisconnect;
	BOOL	m_bAppendData;
	CString	m_strWksName;
	BOOL	m_bSaveProjectOnExit;
	CString	m_strFileName;
	UINT	m_nColumns;
	UINT	m_nRows;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpeakWithOriginDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSpeakWithOriginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAlwaysCreateNew();
	afx_msg void OnConnectExistingFirst();
	afx_msg void OnConnectOrigin();
	afx_msg void OnDisconnectOrigin();
	afx_msg void OnShowOriginWindow();
	afx_msg void OnSendData();
	afx_msg void OnClearWorksheet();
	afx_msg void OnSaveProjectOnExit();
	afx_msg void OnExitOriginOnDisconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void ExecuteOriginCFunction(CString FuncName, CString Opt1, int Optnum1, int Optnum2=-1156, int Optnum3=-1156, int Optnum4=-1156, int Optnum5=-1156, int Optnum6=-1156, int Optnum7=-1156, int Optnum8=-1156, int Optnum9=-1156, int Optnum10=-1156);
	void ExecuteOriginCFunction(CString FuncName, CString Opt1="-1156", CString Opt2="-1156", CString Opt3="-1156", CString Opt4="-1156", CString Opt5="-1156", CString Opt6="-1156", CString Opt7="-1156", CString Opt8="-1156", CString Opt9="-1156", CString Opt10="-1156", CString Opt11="-1156");
	void SetNameFileSave(CString PathSave);
	void Save(CString PathSave = "");
private:
	COriginClient*	m_pClient;
	int				m_nConnOption;

private:
	BOOL	ExecuteOriginCMD(LPCSTR lpszCMD);
	CString GetSaveAsFile();
	void	EnableControls(BOOL bEnable);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SpeakWithOriginDlg_H__BF1AE429_10D8_4B57_BB14_0046D53038A2__INCLUDED_)
