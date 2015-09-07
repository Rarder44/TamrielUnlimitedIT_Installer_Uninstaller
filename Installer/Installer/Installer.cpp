
// test.cpp : definisce i comportamenti delle classi per l'applicazione.
//
#pragma once
#include "stdafx.h"
#include "Installer.h"
#include "ServicePath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestApp

BEGIN_MESSAGE_MAP(CtestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// costruzione di CtestApp

CtestApp::CtestApp()
{
	// supporto Gestione riavvio
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: aggiungere qui il codice di costruzione.
	// Inserire l'inizializzazione significativa in InitInstance.
}


// L'unico e solo oggetto CtestApp

CtestApp theApp;


// Inizializzazione di CtestApp

BOOL CtestApp::InitInstance()
{
	// InitCommonControlsEx() è necessario in Windows XP se nel manifesto
	// di un'applicazione è specificato l'utilizzo di ComCtl32.dll versione 6 o successiva per abilitare
	// gli stili visuali.  In caso contrario, non sarà possibile creare finestre.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Effettuare questa impostazione in modo da includere tutte le classi di controlli comuni da utilizzare
	// nell'applicazione.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Creare il gestore della shell se la finestra di dialogo contiene
	// controlli della visualizzazione elenco o struttura ad albero della shell.
	CShellManager *pShellManager = new CShellManager;

	// Attiva gestore visualizzazione "Windows nativo" per abilitare i temi nei controlli MFC
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Inizializzazione standard
	// Se non si utilizzano queste funzionalità e si desidera ridurre la dimensione
	// dell'eseguibile finale, è necessario rimuovere dal seguente codice
	// le specifiche routine di inizializzazione che non sono necessarie.
	// Modificare la chiave del Registro di sistema in cui sono memorizzate le impostazioni
	// TODO: è necessario modificare questa stringa in modo appropriato,
	// inserendo ad esempio il nome della società o dell'organizzazione.
	SetRegistryKey(_T("Applicazioni locali generate tramite la Creazione guidata applicazioni"));

	

	GlobVar::Init();

	std::wstring str(m_lpCmdLine);
	std::wstring ToFind(_T("\\NoGUI"));
	std::size_t found = str.find(ToFind);
	if (found != std::string::npos) //trovato
	{
		ServiceInstall *si = new ServiceInstall();
		si->InstallFolder = ServicePath::ComprimePath(GlobVar::DefaultInstallFolder, GlobVar::InstallFolderName);
		si->InstallFolder=ServicePath::ResolveEnviromentPath(si->InstallFolder);
		si->CheckAndInstall();


		//CtestDlg dlg(NULL,true);
		//dlg.CheckAndRepairDotNet();
	}
	else
	{
		CtestDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: inserire qui il codice per gestire la chiusura della finestra di dialogo 
			//  tramite il pulsante OK.
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: inserire qui il codice per gestire la chiusura della finestra di dialogo 
			//  tramite il pulsante Annulla.
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "Avviso: creazione della finestra di dialogo non riuscita. L'applicazione verrà terminata in modo imprevisto.\n");
			TRACE(traceAppMsg, 0, "Avviso: se si utilizzano i controlli MFC nella finestra di dialogo, non è possibile utilizzare #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
		}

		
	}

	// Eliminare il gestore della shell creato in precedenza.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	
	

	// Poiché la finestra di dialogo è stata chiusa, restituisce FALSE in modo che l'applicazione
	//  venga terminata, anziché avviare il message pump dell'applicazione.
	return FALSE;
}

