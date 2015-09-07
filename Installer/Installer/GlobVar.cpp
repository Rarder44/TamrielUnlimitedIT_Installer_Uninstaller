#pragma once
#include "stdafx.h"
#include "GlobVar.h"



GlobVar::GlobVar()
{
}


GlobVar::~GlobVar()
{
}
wstring GlobVar::TempDotNetInstallName = _T("InstDotNet4_5.exe");
wstring GlobVar::TempConfFileName = _T("InstallConf.ini");


wstring GlobVar::DefaultInstallFolder = _T("%ProgramFiles%");
wstring GlobVar::InstallFolderName = _T("Tamriel Unlimited IT");

wstring GlobVar::DefaultStartMenuFolder = _T("%ALLUSERSPROFILE%\\Microsoft\\Windows\\Start Menu\\Programs\\");

wstring GlobVar::DefaultDesktopFolder = _T("%ALLUSERSPROFILE%\\Desktop\\");




void GlobVar::Init()
{
	//IDR_DA_MOD

	//END_IDR_DA_MOD
}