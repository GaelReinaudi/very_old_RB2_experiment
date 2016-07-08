#pragma once

// Classes wrapper IDispatch générées par l'ordinateur créées par Microsoft Visual C++

// REMARQUE : Ne modifiez pas le contenu de ce fichier. Si cette classe est régénérée par
//  Microsoft Visual C++, vos modifications seront remplacées.

/////////////////////////////////////////////////////////////////////////////
// Classe wrapper CBcamTopologyViewCtl

class CBcamTopologyViewCtl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBcamTopologyViewCtl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x26551652, 0x9D46, 0x4D5D, { 0x8D, 0xCA, 0x13, 0x1C, 0x56, 0x14, 0x11, 0x28 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributs
public:


// Opérations
public:

// IBcamTopologyViewCtl

// Functions
//

	void Update()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ResetBus()
	{
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_Details()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Details(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

// Properties
//



};

