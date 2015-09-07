
// Uninstaller.cpp : definisce i comportamenti delle classi per l'applicazione.
//

#include "stdafx.h"
#include "Uninstaller.h"
#include "UninstallerDlg.h"
#include <string>
#include "ServiceUninstall.h"
#include "ServicePath.h"
#include "GlobVar.h"
#include "String.h"
#include "ServiceSystem.h"
#include "ServiceExecuteCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUninstallerApp

BEGIN_MESSAGE_MAP(CUninstallerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// costruzione di CUninstallerApp
using namespace std;
CUninstallerApp::CUninstallerApp()
{
	// supporto Gestione riavvio
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: aggiungere qui il codice di costruzione.
	// Inserire l'inizializzazione significativa in InitInstance.
}


// L'unico e solo oggetto CUninstallerApp

CUninstallerApp theApp;


// Inizializzazione di CUninstallerApp

BOOL CUninstallerApp::InitInstance()
{
	// InitCommonControlsEx() � necessario in Windows XP se nel manifesto
	// di un'applicazione � specificato l'utilizzo di ComCtl32.dll versione 6 o successiva per abilitare
	// gli stili visuali.  In caso contrario, non sar� possibile creare finestre.
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
	// Se non si utilizzano queste funzionalit� e si desidera ridurre la dimensione
	// dell'eseguibile finale, � necessario rimuovere dal seguente codice
	// le specifiche routine di inizializzazione che non sono necessarie.
	// Modificare la chiave del Registro di sistema in cui sono memorizzate le impostazioni
	// TODO: � necessario modificare questa stringa in modo appropriato,
	// inserendo ad esempio il nome della societ� o dell'organizzazione.
	SetRegistryKey(_T("Tamriel Unlimited IT"));

	
	if (!InTempFolder())
	{
		StringPath FullPath = ServiceSystem::GetCurrentExecutiblePath();
		StringPath Dir = FullPath.GetDirectory();
		StringPath FileName = FullPath.GetFileNameExt();
		StringPath TempDir = GlobVar::PreTempFolder;
		TempDir.ResolveEnviromentPath();
		TempDir = ServicePath::ComprimePath(TempDir, (GlobVar::postTempFolder + time(NULL)));

		StringPath toExe = ServicePath::ComprimePath(TempDir, FileName);
		ServiceSystem::SecureCopyFile(FullPath, toExe);
		ServiceSystem::SecureCopyFile(ServicePath::ComprimePath(Dir, GlobVar::DefaultNameUninstallIni), ServicePath::ComprimePath(TempDir, GlobVar::DefaultNameUninstallIni));

		ServiceExecuteCommand::executeCommandLineAsync(toExe+" "+ m_lpCmdLine);
		return FALSE;
	}

	SetCurrentDirectory(ServiceSystem::GetCurrentExecutiblePath().GetDirectory().lpcwstr());


	String strs = m_lpCmdLine;
	size_t foundNoGui = strs.Find("\\NoGUI");
	if (foundNoGui != std::string::npos) //trovato
	{
		ServiceUninstall *su = new ServiceUninstall();
		su->SelectedUninstallType = 0;


		String PatternUninstallType = "\\UninstallType_";
		size_t foundUninstallType = strs.Find(PatternUninstallType);
		if (foundUninstallType != std::string::npos) //trovato
		{
			int StartPoint = foundUninstallType + PatternUninstallType.Length();
			String s = "";
			while (true)
			{
				String ss = strs[StartPoint++];
				if (ss.IsInt())
					s += ss;
				else
					break;
			}
			if (s.IsInt())
			{
				ServiceUninstall *su = new ServiceUninstall();
				su->SelectedUninstallType = s.ParseInt();
				StringPath sp = ServicePath::ComprimePath(ServiceSystem::GetCurrentExecutiblePath().GetDirectory(), GlobVar::DefaultNameUninstallIni);
				su->UninstallIniPath = *sp.RemoveDuplicateSlash();
				su->PreUninstall();
			}

		}
		
	

	}
	else
	{
		CUninstallerDlg dlg;
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
			TRACE(traceAppMsg, 0, "Avviso: creazione della finestra di dialogo non riuscita. L'applicazione verr� terminata in modo imprevisto.\n");
			TRACE(traceAppMsg, 0, "Avviso: se si utilizzano i controlli MFC nella finestra di dialogo, non � possibile utilizzare #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
		}


	}

	// Eliminare il gestore della shell creato in precedenza.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Poich� la finestra di dialogo � stata chiusa, restituisce FALSE in modo che l'applicazione
	//  venga terminata, anzich� avviare il message pump dell'applicazione.
	return FALSE;

	
}

bool CUninstallerApp::InTempFolder()
{
	StringPath currentFolder = *ServiceSystem::GetCurrentExecutiblePath().GetDirectory().RemoveDuplicateSlash()->TrimRSlashes()->ToUpperModify();
	StringPath PreTempFolder = GlobVar::PreTempFolder;
	PreTempFolder.ResolveEnviromentPath()->RemoveDuplicateSlash()->TrimRSlashes()->ToUpperModify();
	

	if (currentFolder.StartWith(PreTempFolder))
		return true;
	return false;

}