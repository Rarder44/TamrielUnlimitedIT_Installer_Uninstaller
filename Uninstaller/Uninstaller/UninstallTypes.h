#pragma once
#include "String.h"
#include <list>
#include <tuple>




class UninstallTypes
{

public:
	class UninstallType
	{
		friend class UninstallTypes;
	private:
		int ID = -1;
		String Nome = "";
		
	public:
		list<String> ListaFileDaCancellare;
		list<String> ListaCartelleDaCancellare;
		list<tuple<String, String>> ListaRegKeyDaCancellare;
		list<tuple<String, String>> ListaRegValueDaCancellare;

		int GetID();
		String GetNome();
		String ToString();
		UninstallType* Merge(UninstallType* ut);
		UninstallType();
		UninstallType(int ID,String Nome);
		~UninstallType();
	};

	
private:

	list<UninstallType*> listaUninstallType;

public:

	list<String> GetAllName();
	list<int> GetAllID();
	list<tuple<int, String>> GetAllIDName();

	UninstallType* Find(int ID);
	UninstallType* Find(String Nome);

	UninstallType* AddNewUninstallType(UninstallType* ut);
	UninstallType* AddNewUninstallType(int ID, String Nome);

	void Clear();

	UninstallTypes();
	~UninstallTypes();
};

typedef UninstallTypes::UninstallType UninstallType;
