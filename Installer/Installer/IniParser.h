#pragma once
#include "StringPath.h"
#include <tuple>
#include "AppVersion.h"
#include "Enum.h"



class IniParser
{
public:
	IniParser();
	~IniParser();


	String AppName = "";

	AppVersion NewVersion= String("");
	


	AppVersion OldVersion = String("");
	String OldInstallFolder = "";



	StringPath UnistallCommand = "";
	String UnistallArgument = "";
	bool UnistallBeforeInstall = true;



	list<tuple<StringPath, StringPath>> ListaFileDaCopiare;
	list<StringPath> ListaFileDaCancellare;

	list<tuple<String, String>> ListaRegKeyDaCreare;
	list<tuple<String, String, String, String>> ListaRegValueDaSettare;
	list<tuple<String, String>> ListaRegKeyDaCancellare;
	list<tuple<String, String>> ListaRegValueDaCancellare;


	list<tuple<TypeRun, StringPath>> ListaProgrammiDaAvviareDopoInstallazione;
	list<tuple<TypeRun, StringPath>> ListaProgrammiDaAvviareDopoAbort;



	// 0 = NO , 1 = chiedi, 2 = forza
	int DesktopLink = 0;
	list<tuple<StringPath, StringPath>> DesktopLinks;

	// 0 = NO , 1 = chiedi, 2 = forza
	int StartMenuLink = 0;
	String StartMenuFolderName = "";
	list<tuple<StringPath, StringPath>> StartMenuLinks;


	int Error = 0;
	/*
		1		=	File non trovato

		2		=	stringa RegKey x get della version non valido
		3		=	Errore generico get versioni

		4		=	stringa RegKey x get della InstallFolder non valida
		5		=	Errore generico get unistall command

		6		=	stringa file da copiare non valida
		7		=	Errore generico get copy file list

		8		=	Errore generico get delete file list

		9		=	stringa lista key da creare non valida
		10		=	Errore generico get lista key da creare

		11		=	stringa lista value da settare non valida
		12		=	Errore generico get lista value da settare

		13		=	stringa lista key da cancellare non valida
		14		=	Errore generico get lista key da cancellare

		15		=	stringa lista value da cancellare non valida
		16		=	Errore generico get lista value da cancellare

		17		=	stringa prog da avviare dopo l'install non valida
		18		=	Errore generico get lista prog da avviare dopo l'install

		19		=	stringa prog da avviare dopo l'abort non valida
		20		=	Errore generico get lista prog da avviare dopo l'abort

		21		=	Riga DesktopLink errata
		22		=	Riga StartMenuLink errata
		23		=	Parametri x i link non validi

		24		=	stringa start menu link non valida
		25		=	Errore generico get lista start menu link

		26		=	stringa desktop link non valida
		27		=	Errore generico get lista desktop link

	*/


	void Parse(String Path);
};

