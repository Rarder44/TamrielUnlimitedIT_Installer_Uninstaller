
// test.h : file di intestazione principale per l'applicazione PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "includere 'stdafx.h' prima di includere questo file per PCH"
#endif

#include "resource.h"		// simboli principali
#include "InstallerDlg.h"


// CtestApp:
// Vedere test.cpp per l'implementazione di questa classe
//

class CtestApp : public CWinApp
{
public:
	CtestApp();

// Override
public:
	virtual BOOL InitInstance();

// Implementazione

	DECLARE_MESSAGE_MAP()
};

extern CtestApp theApp;