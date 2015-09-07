#include "stdafx.h"
#include "UninstallTypes.h"


UninstallType::UninstallType()
{
}
UninstallType::UninstallType(int ID, String Nome)
{
	this->ID = ID;
	this->Nome = *Nome.Trim();
}

UninstallType::~UninstallType()
{
	Nome.Clear();
	ListaFileDaCancellare.clear();
	ListaCartelleDaCancellare.clear();
	ListaRegKeyDaCancellare.clear();
	ListaRegValueDaCancellare.clear();
}

UninstallType* UninstallType::Merge(UninstallType* ut)
{

	Nome = ut->Nome;
	for each (String s in ut->ListaFileDaCancellare)
		ListaFileDaCancellare.push_back(s);
	for each (String s in ut->ListaCartelleDaCancellare)
		ListaCartelleDaCancellare.push_back(s);
	
	for each (tuple<String, String> s in ut->ListaRegKeyDaCancellare)
		ListaRegKeyDaCancellare.push_back(s);

	for each (tuple<String, String> s in ut->ListaRegValueDaCancellare)
		ListaRegValueDaCancellare.push_back(s);



	return this;
}
String  UninstallType::ToString()
{
	return to_wstring(ID) + " - " + Nome;
}		
int UninstallType::GetID()
{
	return ID;
}
String UninstallType::GetNome()
{
	return Nome;
}




















UninstallTypes::UninstallTypes()
{
}


UninstallTypes::~UninstallTypes()
{
	Clear();
}

list<String> UninstallTypes::GetAllName()
{
	list<String> t;
	for each (UninstallType* ut in listaUninstallType)
		t.push_back(ut->Nome);
	return t;
}


list<int> UninstallTypes::GetAllID()
{
	list<int> t;
	for each (UninstallType* ut in listaUninstallType)
		t.push_back(ut->ID);
	return t;
}


list<tuple<int, String>> UninstallTypes::GetAllIDName()
{
	list<tuple<int, String>> t;
	for each (UninstallType* ut in listaUninstallType)
		t.push_back(make_tuple(ut->ID, ut->Nome));
	return t;
}


UninstallType* UninstallTypes::Find(int ID)
{
	for each (UninstallType* ut in listaUninstallType)
		if (ut->ID == ID)
			return ut;
}

UninstallType* UninstallTypes::Find(String Nome)
{
	for each (UninstallType* ut in listaUninstallType)
		if (ut->Nome == Nome)
			return ut;
}

UninstallType* UninstallTypes::AddNewUninstallType(UninstallType* ut)
{
	UninstallType* t = Find(ut->ID);
	if (t)
	{
		t->Merge(ut);
		return t;
	}
	else
	{
		listaUninstallType.push_back(ut);
		return ut;
	}
}

UninstallType* UninstallTypes::AddNewUninstallType(int ID, String Nome)
{
	UninstallType* t = Find(ID);
	if (t)
	{
		t->Nome = Nome;
		return t;
	}
	else
	{
		t = new UninstallType(ID, Nome);
		listaUninstallType.push_back(t);
		return t;
	}
}

void UninstallTypes::Clear()
{
	for each (UninstallType* ut in listaUninstallType)
		delete ut;
	listaUninstallType.clear();
}