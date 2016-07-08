/*------------------------------------------------------------------------------*
 * File Name: OriginClient.h													*
 * Creation:  LY 4/7/2004														*
 * Purpose: Utility functions to access Origin Automation server				*
 * Copyright (C) OriginLab Corp.												*
 * All Rights Reserved															*
 *------------------------------------------------------------------------------*/

#include "comdef.h"
#include "comutil.h"

#pragma once

#import ".\Origin.tlb" rename_namespace("OrgApplication")
using namespace OrgApplication;
	   
#define IS_VALID_ORIGIN_PTR(_p)		( _p != NULL && _p->IsValid() )
#define ORIGIN_WINTYPE_WKS		2

enum {
	ALWAYS_CREATE_NEW = 0,
	USE_EXIST_FIRST
};

class COriginClient
{
public:
	COriginClient(int nOption = USE_EXIST_FIRST);
	~COriginClient(void);

public:
	// This demonstrates how to wrap automation methods and provide
	// user specified calling arguments
	BOOL GetWorksheet(LPCSTR lpszWks, _variant_t& val);
	BOOL SetWorksheet(LPCSTR lpszWks, _variant_t val, int nRowStart = 0);
	
	//Method to assert the reference to Origin automation server is valid
	BOOL IsValid();

	// This demonstrates how to allow this class to be used 
	// as the IOApplicationPtr itself.
	IOApplicationPtr operator->() const 
	{ 
		if( m_pOApp == NULL ) 
		{
			_com_issue_error(E_POINTER);
		}

		return m_pOApp; 
	}

private:
	IOApplicationPtr m_pOApp;
};
