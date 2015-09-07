#pragma once
#include <string>

using namespace std;

class GlobVar
{
public:
	static wstring TempDotNetInstallName;
	static wstring TempConfFileName;

	static wstring DefaultInstallFolder;
	static wstring InstallFolderName;

	static wstring DefaultStartMenuFolder;
	static wstring DefaultDesktopFolder;

	static float CurrentVersion;


	static void Init();
	GlobVar();
	~GlobVar();
};


