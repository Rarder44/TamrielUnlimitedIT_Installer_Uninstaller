#include "stdafx.h"
#include "ServiceSystem.h"
#include "ServicePath.h"
#include "ServiceString.h"
#include "dirent.h"
#include <tuple>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include "GlobVar.h"
#include <fstream>
#include "StringPath.h"
#include "ServiceExecuteCommand.h"

ServiceSystem::ServiceSystem()
{
}


ServiceSystem::~ServiceSystem()
{
}



bool ServiceSystem::SecureCopyFile(String from, String to)
{
	from.Trim();
	to.Trim();
	from = ServicePath::RemoveDuplicateSlash(from);
	to = ServicePath::RemoveDuplicateSlash(to);
	if (PathFileExists(from.lpcwstr()))
	{
		if (!PathIsDirectory(from.lpcwstr()))
		{
			String p = ServicePath::GetParentDirectory(to);
			if (!PathFileExists(p.lpcwstr()))
				CreateDirectory(p.lpcwstr(),NULL);
			return CopyFile(from.lpcwstr(), to.lpcwstr(), false);

		}
		return false;
	}
	return false;
}
bool ServiceSystem::CreateLink(String OriginFile, String DestFolder, String DestNameLink)
{
	OriginFile.Trim();
	DestFolder.Trim();
	DestNameLink.Trim();

	CoInitialize(NULL);
	IShellLink* pShellLink = NULL;
	HRESULT hres;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL,IID_IShellLink, (void**)&pShellLink);

	if (SUCCEEDED(hres))
	{
		pShellLink->SetPath(OriginFile.lpcwstr());  // Path to the object we are referring to
		pShellLink->SetDescription(_T(""));
		pShellLink->SetIconLocation(OriginFile.lpcwstr(), 0);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres))
		{
			hres = pPersistFile->Save(ServicePath::ComprimePath(DestFolder.lpcwstr(), ServicePath::AddExtension(DestNameLink, _T(".lnk")).lpcwstr()).lpcwstr(), TRUE);
			pPersistFile->Release();
		}
		else
		{
			return false;
		}
		pShellLink->Release();
	}
	else
	{
		return false;
	}

	return true;
	
}

bool ServiceSystem::SecureDeleteFile(String path)
{
	if (PathFileExists(path.lpcwstr()) && !PathIsDirectory(path.lpcwstr()))
	{
		return DeleteFile(path.lpcwstr());
	}
	return false;

}
void ServiceSystem::SecureDeleteFileForce(String path)
{
	if (PathFileExists(path.lpcwstr()) &&  !PathIsDirectory(path.lpcwstr()))
	{
		while (!DeleteFile(path.lpcwstr()));
	}
}

FileFolderGroup ServiceSystem::SecureRemoveDirectory(String path)
{
	FileFolderGroup local;
	if (PathIsDirectory(path.lpcwstr()))
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(path.lpcstr())) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				String s = ent->d_name;
				if (s != "." && s != "..")
				{
					String t = ServicePath::ComprimePath(path, s);
					if (PathIsDirectory(t.lpcwstr()))
					{
						local.Merge(SecureRemoveDirectory(t));
					}
					else
						if (!DeleteFile(t.lpcwstr()))
							local.AddFile(t);
				}
			}
			closedir(dir);
			if (!RemoveDirectory(path.lpcwstr()))
				local.AddDir(path);
		}
	}
	return local;
}
void ServiceSystem::SecureRemoveDirectoryForce(String path)
{

	if (PathIsDirectory(path.lpcwstr()))
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(path.lpcstr())) != NULL) 
		{
			while ((ent = readdir(dir)) != NULL) 
			{
				String s = ent->d_name;
				if (s != "." && s != "..")
				{
					String t = ServicePath::ComprimePath(path, s);
					if (PathIsDirectory(t.lpcwstr()))
						SecureRemoveDirectory(t);

					else
						while (!DeleteFile(t.lpcwstr()));
				}
			}
			closedir(dir);
			while (!RemoveDirectory(path.lpcwstr()));
		}
	}

}



void ServiceSystem::killProcessByName(String filename)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);

	BOOL hRes = Process32First(hSnapShot, &pEntry);
	while (hRes)
	{

		if (pEntry.szExeFile == filename)
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				WaitForSingleObject(hProcess, INFINITE);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}
void ServiceSystem::SecureRemoveFileDirectoryWait(FileFolderGroup path)
{
	if (path.File.size() == 0 || path.Dir.size() == 0)
		return;


	TCHAR s[MAX_PATH];
	String TempPath;
	String TempFileName = "dl.bat";
	

	if (GetTempPath(MAX_PATH, s)) {
		TempPath = s;
		TempPath = ServicePath::ComprimePath(TempPath, TempFileName);


		ofstream * out = new ofstream(TempPath.lpcwstr());

		String Del = "";
		String Check = "";

		for each (StringPath ss in path.File)
		{
			ss.ResolveEnviromentPath()->RemoveDuplicateSlash();
			if (PathFileExists(ss.lpcwstr()))
			{
				Del += "del \"" + ss + "\"\r\n";
				Check += "if exist \"" + ss + "\" goto Repeat\r\n";
			}
		}
		for each (StringPath ss in path.Dir)
		{
			ss.ResolveEnviromentPath()->RemoveDuplicateSlash();
			if (PathFileExists(ss.lpcwstr()))
			{
				Del += "rmdir \"" + ss + "\" /s /q\r\n";
				Check += "if exist \"" + ss + "\" goto Repeat\r\n";
			}
		}

		*out << "cd \\ \r\n";
		*out << "timeout 2\r\n";
		*out << Del.lpcstr();
		*out << Check.lpcstr();



		//*out << ("del \"" + TempPath + "\"\r\n").lpcstr();
		*out << "exit\r\n"<<"\r\n";
		*out << ":Repeat" << "\r\n";
		*out << ("start  \"\" /B \"" + TempPath + "\"\r\n").lpcstr();
		*out << "exit\r\n" << "\r\n";
		out->close();



		ServiceExecuteCommand::executeCommandLineAsync(TempPath);
	
	
	}
		

}


StringPath ServiceSystem::GetCurrentExecutiblePath()
{
	wchar_t pBuf[MAX_PATH];
	DWORD bytes = GetModuleFileName(NULL, pBuf, MAX_PATH);
	return pBuf;
}


