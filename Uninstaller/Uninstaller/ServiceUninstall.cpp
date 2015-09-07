#include "stdafx.h"
#include "ServiceUninstall.h"
#include "resource.h"
#include "ServiceSystem.h"
#include "StringPath.h"
#include "ServiceRegistry.h"
#include "ServiceExecuteCommand.h"
#include "FileFolderGroup.h"
#include "ServicePath.h"

ServiceUninstall::ServiceUninstall()
{
}


ServiceUninstall::~ServiceUninstall()
{
}



void ServiceUninstall::PreUninstall()
{
	ip = new IniParser();
	ip->Parse(UninstallIniPath);
	if (ip->Error != 0)
	{
		if (dialog)
		{
			String s = "Errore durante il parse del file INI: " + to_wstring(ip->Error);
			MessageBox(dialog->m_hWnd, s.lpcwstr(), _T("INI Parse Error"), 0);
			dialog->EndDialog(-1);
		}
		return;
	}

	String s = "Uninstall "+ ip->AppName + " - "+ ip->AppVersion.getVersion();

	if (dialog)
	{
		SetDlgItemText(dialog->m_hWnd, IDC_STATIC_TITOLO, s.lpcwstr());
		SetWindowText(dialog->m_hWnd, s.lpcwstr());
	}
	



	if (dialog)
		UninstallGUI();
	else
		Uninstall();

}
void ServiceUninstall::UninstallGUI()
{
	if (dialog)
	{
		PreUninstallScene();
		CComboBox* ccb = ((CComboBox*)dialog->GetDlgItem(IDC_COMBO1));
		list<int> l=ip->TipiDiDisistallazione.GetAllID();
		if (l.size() == 0)
		{
			String s = "Nessuna informazione trovata per procedere con la disistallazione";
			MessageBox(dialog->m_hWnd, s.lpcwstr(), _T("Errore info per disistallazione"), 0);
			dialog->EndDialog(-1);
			return;
		}
		for each (int i in l)
		{
			UninstallType* ut = nullptr;
			ut = ip->TipiDiDisistallazione.Find(i);
			if(ut)
				ccb->AddString(ut->ToString().lpcwstr());	
		}
		ccb->SetCurSel(0);
	}
		
}
void ServiceUninstall::Uninstall()
{
	if (dialog)
		UninstallingScene();

	for each (String f in ip->ListaKillProcess)
	{
		ServiceSystem::killProcessByName(f);
	}

 	for each (tuple<TypeRun, String > f in ip->ListaProgrammiDaAvviarePrimaUninstall)
	{
		String data = ReplaceVariable(get<1>(f));

		if (get<0>(f) == Async)
		{
			ServiceExecuteCommand::executeCommandLineAsync(data);
		}
		else if (get<0>(f) == Sync)
		{
			ServiceExecuteCommand::executeCommandLineSync(data);
		}
	}

	UninstallType* ut = nullptr;
	 ut = ip->TipiDiDisistallazione.Find(SelectedUninstallType);
	if (ut)
	{

		for each (String f in ut->ListaFileDaCancellare)
		{	

			StringPath file = *ReplaceVariable(f).Trim();
			file.ResolveEnviromentPath();
			file.RemoveDuplicateSlash();
			ServiceSystem::SecureDeleteFileForce(file);
		}
		for each (String f in ut->ListaCartelleDaCancellare)
		{
			StringPath file = *ReplaceVariable(f).Trim();
			file.ResolveEnviromentPath();
			file.RemoveDuplicateSlash();
			ServiceSystem::SecureRemoveDirectoryForce(file);
		}
	

		for each (tuple<String, String> f in ut->ListaRegValueDaCancellare)
		{
			ServiceRegistry::RemoveValue(*get<0>(f).Trim(), *get<1>(f).Trim());
		}
		for each (tuple<String, String> f in ut->ListaRegKeyDaCancellare)
		{
			ServiceRegistry::RemoveKey(*get<0>(f).Trim(), *get<1>(f).Trim());
		}



	}
	else
	{
		if (dialog)
		{
			String s = "Impossibile recuperare le informazioni per la disistallazione";
			MessageBox(dialog->m_hWnd, s.lpcwstr(), _T("Errore info per disistallazione"), 0);
			dialog->EndDialog(-1);
		}
		return;
	}

	RemoveTempFileFolder();
	if (dialog)
		FineScene();
}





void ServiceUninstall::CleanScene()
{
	//PreUninstall
	dialog->GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	SetDlgItemText(dialog->m_hWnd, IDC_BUTTON1, _T(""));

	//Uninstalling
	dialog->GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);


	//FineScene
	


	CurrentScene = 0;
}
void ServiceUninstall::PreUninstallScene()
{
	//PreUninstall
	dialog->GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	SetDlgItemText(dialog->m_hWnd, IDC_BUTTON1, _T("Disistalla"));

	//Uninstalling
	dialog->GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);


	//FineScene



	CurrentScene = 1;
}
void ServiceUninstall::UninstallingScene()
{
	//PreUninstall
	dialog->GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	SetDlgItemText(dialog->m_hWnd, IDC_BUTTON1, _T("Annulla"));
	
	//Uninstalling
	dialog->GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);


	//FineScene



	CurrentScene = 2;
}
void ServiceUninstall::FineScene()
{
	//PreUninstall
	dialog->GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	SetDlgItemText(dialog->m_hWnd, IDC_BUTTON1, _T("Fine"));

	//Uninstalling
	dialog->GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	SetDlgItemText(dialog->m_hWnd, IDC_STATIC2, _T("Disitallazione Terminata"));
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);


	//FineScene



	CurrentScene = 3;
}


String ServiceUninstall::ReplaceVariable(String s)
{
	s.ReplaceAll("[InstallVersion]", ip->AppVersion.getVersion());
	s.ReplaceAll("[InstallFolder]", ip->InstallDir);
	return s;
}





void ServiceUninstall::RemoveTempFileFolder()
{
	ServiceSystem::SecureRemoveFileDirectoryWait(ServiceSystem::SecureRemoveDirectory(ServiceSystem::GetCurrentExecutiblePath().GetDirectory()));
}
