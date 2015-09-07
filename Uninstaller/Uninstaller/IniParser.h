#pragma once
#include "String.h"
#include "AppVersion.h"
#include "UninstallTypes.h"
#include "Enum.h"

class IniParser
{
public:
	IniParser(){}
	~IniParser(){}


	String AppName = "";

	AppVersion AppVersion=String("");
	
	String InstallDir = "";

	UninstallTypes TipiDiDisistallazione;

	list<tuple<TypeRun, String>> ListaProgrammiDaAvviarePrimaUninstall;
	list<String> ListaKillProcess;


	int Error = 0;
	/*
		1		=	File non trovato

		2		=	Errore generico get GLOBAL

		3		=	Errore generico get FILE/KEY

		4		=	Errore generico get File da Cancellare

		5		=	Errore generico get Cartelle da Cancellare

		6		=	stringa valori registro da cancellare non valida
		7		=	Errore generico get valori registro da cancellare

		8		=	stringa key registro da cancellare non valida
		9		=	Errore generico get key registro da cancellare
		
		10		=	stringa prog da avviare prima uninstall non valida
		11		=	Errore generico get lista prog da avviare prima uninstall 
	*/


	void Parse(String Path);
	
};

