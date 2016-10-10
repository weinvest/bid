
// MyBid.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMyBidApp:
// See MyBid.cpp for the implementation of this class
//

class CMyBidApp : public CWinApp
{
public:
	CMyBidApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);

	HACCEL mHacclTable;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMyBidApp theApp;