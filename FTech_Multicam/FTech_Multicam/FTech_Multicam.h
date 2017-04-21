
// FTech_Multicam.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFTech_MulticamApp:
// See FTech_Multicam.cpp for the implementation of this class
//

class CFTech_MulticamApp : public CWinApp
{
public:
	CFTech_MulticamApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFTech_MulticamApp theApp;