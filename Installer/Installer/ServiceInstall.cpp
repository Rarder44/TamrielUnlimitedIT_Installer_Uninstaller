#pragma once

#include "stdafx.h"
#include "ServiceInstall.h"


#include "ServiceRegistry.h"

#include "Enum.h"
#include "ReturnValue.h"
#include "unzip.h"

#include "ServiceCheckVersion.h"
#include "ServicePath.h"
#include "ServiceResource.h"
#include "ServiceExecuteCommand.h"
#include "ServiceSystem.h"

#include "resource.h"






ServiceInstall::ServiceInstall() {}
ServiceInstall::~ServiceInstall() {}

void ServiceInstall::CheckAndInstall()
{
	TCHAR s[MAX_PATH];

	if (TempPath == _T("") || !PathFileExists(TempPath.lpcwstr()))
	{
		if (GetTempPath(MAX_PATH, s)) {
			TempPath = ServicePath::ComprimePath(s, GlobVar::InstallFolderName);
			CreateDirectory(TempPath.lpcwstr(), NULL);
		}
		else
		{
			if (dialog)
			{
				MessageBox(dialog->m_hWnd, _T("Errore nel recupero del path temporaneo"), NULL, NULL);
				dialog->EndDialog(-1);

			}
			return;
		}
	}


	CheckAndRepairDotNet();
	ExtractResource();
	CheckUpdateOrInstall();
}


void ServiceInstall::CheckAndRepairDotNet()
{
	if (dialog)
	{
		DotNetScene();
		SetDlgItemText(dialog->m_hWnd, IDC_STATIC_dotNet, _T("Controllo versione requisiti..."));
		SetDlgItemText(dialog->m_hWnd, IDC_STATIC_titolo, _T("Installer Requisiti"));
		SetWindowText(dialog->m_hWnd, _T("Installer Requisiti"));
	}


	VersionDotNet v = ServiceCheckVersion::CheckVersion();
	if (v < v4_5)
	{
		if (dialog)
			SetDlgItemText(dialog->m_hWnd, IDC_STATIC_dotNet, _T("Installazione requisiti..."));

		String TempDotNetFullPath = ServicePath::ComprimePath(TempPath, GlobVar::TempDotNetInstallName);
		ServiceResource::ExtractBinResource("BIN", IDR_BIN_DOTNET, TempDotNetFullPath);


		TempDotNetFullPath += " /q /norestart";


		ReturnValue rv = ServiceExecuteCommand::executeCommandLineSync(TempDotNetFullPath);
		if (rv.Status)
		{
			if (rv.exitCode != 0)
			{
				CString msg;
				switch (rv.exitCode)
				{
				case 1602:
					msg = "Installazione annullata dall'utente.";
					break;
				case 1603:
					msg = "Errore irreversibile durante l'installazione.";
					break;
				case 1614:
				case 3010:
					msg = "Riavvio necessario per completare l'installazione.";
					break;
				case 5100:
					msg = "Il computer dell'utente non soddisfa i requisiti di sistema.";
					break;
				case 5101:
					msg = "Errore di stato interno.";
					break;
				default:
					msg = "Exit code non riconosciuto.";
				}
				msg.Append(_T("\n\nIl programma verrà chiuso."));
				dialog->EndDialog(-1);
			}
		}
		else
		{
			
			if (dialog)
			{
				String t = rv.msg;
				t += "\n\nIl programma verrà chiuso.";
				MessageBox(dialog->m_hWnd, t.lpcwstr(), NULL, NULL);
				dialog->EndDialog(-1);
			}
			return;

		}
		if (dialog)
			SetDlgItemText(dialog->m_hWnd, IDC_STATIC_dotNet, _T("Pulizia file temporanei..."));
		DeleteFile(TempDotNetFullPath.lpcwstr());


	}
	if (dialog)
	{
		DotNetScene();
		SetDlgItemText(dialog->m_hWnd, IDC_STATIC_dotNet, _T("Requisiti: OK"));
	}
}

void ServiceInstall::ExtractResource()
{
	ServiceResource::ExtractBinResource("BIN", IDR_BIN_ZIP, ServicePath::ComprimePath(TempPath, TempZipFileName));
}

void ServiceInstall::CheckUpdateOrInstall()
{
	HKEY key;

	//Unizippo il file estratto
	String TempZipFilePath = ServicePath::ComprimePath(TempPath, TempZipFileName);
	HZIP hz = OpenZip(TempZipFilePath.lpcwstr(), 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	for (int zi = 0; zi<numitems; zi++)
	{
		ZIPENTRY ze;
		GetZipItem(hz, zi, &ze);
		String ss = ServicePath::ComprimePath(TempPath, ze.name);
		UnzipItem(hz, zi, ss.lpwstr());
	}
	CloseZip(hz);


	//controllo se nel file c'è il file INI di conf dell'install
	String TempConfFilePath = ServicePath::ComprimePath(TempPath, TempConfFileName);
	ip = new IniParser();
	ip->Parse(TempConfFilePath);

	if (ip->Error != 0)
	{
		if (dialog)
		{
			String s = "Errore durante il parse del file INI: "+ ip->Error;
			MessageBox(dialog->m_hWnd, s.lpcwstr(), _T("INI Parse Error"), 0);
			dialog->EndDialog(-1);
		}
		return;
	}


	if (dialog)
	{
		SetDlgItemText(dialog->m_hWnd, IDC_STATIC_titolo, (ip->AppName + " - Installer " + ip->NewVersion.getVersion()).lpcwstr());
		SetWindowText(dialog->m_hWnd, (ip->AppName + " - Installer " + ip->NewVersion.getVersion()).lpcwstr());

	}




	if (ip->OldVersion.getVersion() == "")
	{
		if (dialog)
			InstallGUI();
		else
			Install();
	}
	else if (ip->NewVersion > ip->OldVersion)
	{
		if (dialog)
		{
			String s = "Procedere con l'aggiornamento alla versione "+ ip->NewVersion.getVersion()+"?";

			if (MessageBox(dialog->m_hWnd, s.lpcwstr(), _T("Procedere con l'aggiornamento?"), MB_YESNO) == IDYES)
			{
				UpdateGUI();
			}
			else
			{
				dialog->EndDialog(-1);
				return;
			}
		}
		else
			Update();
	}
	else
	{
		if (dialog)
		{
			String s = "Il programma è già aggiornato alla versione " + ip->OldVersion.getVersion();
			MessageBox(dialog->m_hWnd, s.lpcwstr(), _T("Programma già aggiornato"), 0);
			dialog->EndDialog(-1);
			return;
		}
	}



}


void ServiceInstall::InstallGUI()
{
	if (dialog)
		PreInstallScene();
}

void ServiceInstall::Install()
{
	String t = ip->OldInstallFolder;
	t.Trim();
	if (t == "")
	{
		if (InstallFolder == "")
		{
			if (dialog)
			{
				MessageBox(dialog->m_hWnd, _T("Impossibile ottenere la folder di installazione"), _T("Errore recupero folder di installazione"), 0);
				dialog->EndDialog(-1);
				return;
			}
		}
	}
	else
		InstallFolder = t;



	if (!PathFileExists(InstallFolder.lpcwstr()))
	{
		CreateDirectory(InstallFolder.lpcwstr(), NULL);
	}

	for each (String f in ip->ListaFileDaCancellare)
	{
		StringPath file = *ReplaceVariable(f).Trim();
		DeleteFile(file.RemoveDuplicateSlash()->lpwstr());
	}
	for each (tuple<String, String> f in ip->ListaFileDaCopiare)
	{
		String from = ReplaceVariable(get<0>(f));

		String to = ReplaceVariable(get<1>(f));

		ServiceSystem::SecureCopyFile(from, to);
	}


	for each (tuple<String, String> f in ip->ListaRegValueDaCancellare)
	{
		ServiceRegistry::RemoveValue(*String(get<0>(f)).Trim(), *String(get<1>(f)).Trim());
	}
	for each (tuple<String, String> f in ip->ListaRegKeyDaCancellare)
	{
		ServiceRegistry::RemoveKey(*String(get<0>(f)).Trim(), *String(get<1>(f)).Trim());
	}

	HKEY out;
	for each (tuple<String, String> f in ip->ListaRegKeyDaCreare)
	{
		ServiceRegistry::CreateKey(*String(get<0>(f)).Trim(), *String(get<1>(f)).Trim(), out);
	}

	for each (tuple<String, String, String, String> f in ip->ListaRegValueDaSettare)
	{
		String type = get<3>(f);
		type.Trim();
		if (type == "S")
		{
			String data = *ReplaceVariable(get<2>(f)).Trim();
			ServiceRegistry::SetStringValue(*String(get<0>(f)).Trim(), *String(get<1>(f)).Trim(), data);
		}
		else if (type == _T("D"))
		{
			try
			{
				int v = _wtoi(ReplaceVariable(get<2>(f)).Trim()->lpcwstr());
				ServiceRegistry::SetDWORDValue(*String(get<0>(f)).Trim(), *String(get<1>(f)).Trim(), v);
			}
			catch (...)
			{

			}
		}

	}

	for each (tuple<TypeRun, String > f in ip->ListaProgrammiDaAvviareDopoInstallazione)
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



	if (ip->DesktopLink == 2)
	{
		for each (tuple<String, String > f in ip->DesktopLinks)
		{
			String origin = ReplaceVariable(get<0>(f));
			ServiceSystem::CreateLink(origin, ServicePath::ResolveEnviromentPath(GlobVar::DefaultDesktopFolder), get<1>(f));
		}
	}

	if (ip->StartMenuLink == 2)
	{
		StringPath p = ServicePath::ComprimePath(GlobVar::DefaultStartMenuFolder, ip->StartMenuFolderName);
		p.ResolveEnviromentPath();
		p = ReplaceVariable(p);


		if (!PathFileExists(p.lpcwstr()))
			CreateDirectory(p.lpcwstr(), NULL);


		for each (tuple<String, String > f in ip->StartMenuLinks)
		{
			String origin = ReplaceVariable(get<0>(f));
			ServiceSystem::CreateLink(origin, p, get<1>(f));
		}
	}


	if (ip->DesktopLink == 1)
		ip->DesktopLink = 2;

	if (ip->StartMenuLink == 1)
		ip->StartMenuLink = 2;


	if (dialog)
		FineScene();

	ServiceSystem::SecureRemoveDirectory(TempPath);
}

void ServiceInstall::UpdateGUI()
{
	if (dialog)
		InstallingScene();
	Update();
}

void ServiceInstall::Update()
{
	if (ip->UninstallBeforeInstall)
	{
		String t = ip->OldInstallFolder;
		t.Trim();
		if (t == "")
		{
			if (dialog)
			{
				MessageBox(dialog->m_hWnd, _T("Impossibile ottenere la folder di installazione"), _T("Errore recupero folder di installazione"), 0);
				dialog->EndDialog(-1);
				return;
			}
		}
		

		ip->UninstallCommand.ReplaceAll("[InstallFolder]", t);
		ip->UninstallCommand.RemoveDuplicateSlash();
	
		ReturnValue r = ServiceExecuteCommand::executeCommandLineSync(ip->UninstallCommand + " " + ip->UninstallArgument);

		while (PathFileExists(ip->OldInstallFolder.lpcwstr()));
		CreateDirectory(ip->OldInstallFolder.lpcwstr(), NULL);
	}

	if (ip->DesktopLink == 1)
		ip->DesktopLink = 2;

	if (ip->StartMenuLink == 1)
		ip->StartMenuLink = 2;


	Install();

}


void ServiceInstall::Clean()
{
	//hide dot net
	dialog->GetDlgItem(IDC_STATIC_dotNet)->ShowWindow(SW_HIDE);

	//hide preinstall
	dialog->GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);

	//hide installing
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_STATIC_inst_in_corso)->ShowWindow(SW_HIDE);

	CurrentScene = 0;
}
void ServiceInstall::DotNetScene()
{
	//show dot net
	dialog->GetDlgItem(IDC_STATIC_dotNet)->ShowWindow(SW_SHOW);

	//hide preinstall
	dialog->GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);

	//hide installing
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_STATIC_inst_in_corso)->ShowWindow(SW_HIDE);


	CurrentScene = 1;
}
void ServiceInstall::PreInstallScene()
{
	//show pre install
	dialog->GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_CHECK1)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_CHECK2)->ShowWindow(SW_SHOW);
	//hide dot net
	dialog->GetDlgItem(IDC_STATIC_dotNet)->ShowWindow(SW_HIDE);

	//hide installing
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_STATIC_inst_in_corso)->ShowWindow(SW_HIDE);


	CurrentScene = 2;
}
void ServiceInstall::InstallingScene()
{
	//show installing
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
	dialog->GetDlgItem(IDC_STATIC_inst_in_corso)->ShowWindow(SW_SHOW);


	//hide dot net
	dialog->GetDlgItem(IDC_STATIC_dotNet)->ShowWindow(SW_HIDE);


	//hide preinstall
	dialog->GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);
	CurrentScene = 3;
}
void ServiceInstall::FineScene()
{
	SetDlgItemText(dialog->m_hWnd, IDC_STATIC_titolo, _T("Installazione Terminata"));

	SetDlgItemText(dialog->m_hWnd, IDC_BUTTON1, _T("Fine"));

	dialog->GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);


	//hide dot net
	dialog->GetDlgItem(IDC_STATIC_dotNet)->ShowWindow(SW_HIDE);

	//hide preinstall
	dialog->GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);

	dialog->GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);

	//hide installing
	dialog->GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	dialog->GetDlgItem(IDC_STATIC_inst_in_corso)->ShowWindow(SW_HIDE);


	CurrentScene = 4;
}



String ServiceInstall::ReplaceVariable(String s)
{
	s.ReplaceAll("[NewInstallVersion]", ip->NewVersion.getVersion());
	s.ReplaceAll("[InstallFolder]", InstallFolder);
	s.ReplaceAll("[DesktopLink]", to_wstring(ip->DesktopLink));
	s.ReplaceAll("[StartMenuLink]", to_wstring(ip->StartMenuLink));
	s.ReplaceAll("[TempFolder]", TempPath);
	return s;
}


