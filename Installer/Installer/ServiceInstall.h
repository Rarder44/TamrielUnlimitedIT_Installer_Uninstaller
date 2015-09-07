#include "stdafx.h"
#include "String.h"
#include "GlobVar.h"
#include "IniParser.h"



using namespace std;

class ServiceInstall
{
public:
	String InstallFolder=_T("");
	String TempPath = _T("");
	String TempZipFileName = _T("test.zip");
	String TempConfFileName = GlobVar::TempConfFileName;
	String TempDotNetInstallName = GlobVar::TempDotNetInstallName;

	CDialogEx * dialog = NULL;
	IniParser* ip = NULL;

	int CurrentScene = -1;



	ServiceInstall();
	~ServiceInstall();

	void CheckAndInstall();
	
	
	void CheckAndRepairDotNet();

	void ExtractResource();

	void CheckUpdateOrInstall();


	void InstallGUI();

	void Install();

	void UpdateGUI();

	void Update();

	void CleanTempFile(wstring folder);


	

	void Clean();
	void DotNetScene();
	void PreInstallScene();
	void InstallingScene();
	void FineScene();



	String ReplaceVariable(String s);
};

