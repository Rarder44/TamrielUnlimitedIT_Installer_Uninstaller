#pragma once
#include "IniParser.h"
class ServiceUninstall
{
public:
	String UninstallIniPath = "";
	CDialogEx * dialog = NULL;
	IniParser* ip = NULL;
	int SelectedUninstallType = -1;

	int CurrentScene = 0;

	void PreUninstall();
	void UninstallGUI();
	void Uninstall();


	void CleanScene();
	void PreUninstallScene();
	void UninstallingScene();
	void FineScene();

	String ReplaceVariable(String s);


	void RemoveTempFileFolder();


	ServiceUninstall();
	~ServiceUninstall();


};

