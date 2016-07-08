// AppliDialogXPS.h : main header file for the PROJECT_NAME application
//

#pragma once

#include "resource.h"		// main symbols


// CAppliDialogXPSApp:
// See AppliDialogXPS.cpp for the implementation of this class
//

class CAppliDialogXPSApp : public CWinApp
{
public:
	CAppliDialogXPSApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CAppliDialogXPSApp theApp;