#pragma once
#include "stdafx.h"
#include "IniFile.h"
#include <fstream>



IniKeyValue::~IniKeyValue()
{
	key.Clear();
	value.Clear();
}		
IniKeyValue::IniKeyValue(String key, String value)
{
	this->key = key;
	this->value = value;
}






IniSection::~IniSection()
{
	SectionName.Clear();
	RemoveAllKeys();
}
IniSection::IniSection(String SectionName)
{
	SetSectionName(SectionName);
}
void IniSection::SetSectionName(String SectionName)
{
	this->SectionName = SectionName;
}
IniKeyValue* IniSection::AddIniKeyValue(String key, String value)
{
	IniKeyValue* ikv = FindKey(*key.Trim());
	if (ikv == NULL)
	{
		ikv = new IniKeyValue(key, value);
		lkv.push_back(ikv);
	}
	else
	{
		ikv->value = value;
	}
	return ikv;

}
IniKeyValue* IniSection::AddIniKeyValue(IniKeyValue* keyvalue)
{
	IniKeyValue* ikv = FindKey(keyvalue->key);
	if (ikv == NULL)
	{
		lkv.push_back(keyvalue);
		return keyvalue;
	}
	else
	{
		ikv->value = keyvalue->value;
		return ikv;
	}
}
String IniSection::GetName()
{
	return SectionName;
}
IniKeyValue* IniSection::FindKey(String key)
{
	
	String t = *key.Trim();
	for each (IniKeyValue* var in lkv)
		if (var->key == t)
			return var;
	return NULL;
}
String IniSection::GetKeyValue(String key)
{
	IniKeyValue* t = FindKey(key);
	if (t == NULL)
		return "";
	else
		return t->value;
}
void IniSection::MergeWith(IniSection* is2)
{
	for each (IniKeyValue* var in is2->lkv)
	{
		this->AddIniKeyValue(var);
	}
}
void IniSection::RemoveAllKeys()
{
	for each (IniKeyValue* ikv in lkv)
		delete ikv;
	lkv.clear();
}






IniFile::IniFile()
{

}
IniFile::~IniFile()
{
	RemoveAllSections();
}
void IniFile::Load(String fileName)
{
	std::ifstream input;

	input.open(fileName.lpcwstr(), std::ios::binary);

	if (!input.is_open())
	{
		isLoad = false;
		return;
	}

	IniSection* pSection = NULL;
	//std::wstring sReadw;
	String sReadw;
	std::string sRead;
	enum { KEY, SECTION, COMMENT, OTHER };

	while (std::getline(input, sRead))
	{
		sReadw = sRead;

		sReadw.LTrim();
		sReadw.RTrim("\n\r");


		if (!sReadw.Empty())
		{
			unsigned int nType = (sRead.find_first_of("[") == 0 && (sRead[sRead.find_last_not_of(" \t\r\n")] == ']')) ? SECTION : OTHER;
			nType = ((nType == OTHER) && (sRead.find_first_of("=") != std::string::npos && sRead.find_first_of("=") > 0)) ? KEY : nType;
			nType = ((nType == OTHER) && (sRead.find_first_of("#") == 0)) ? COMMENT : nType;

			switch (nType)
			{
			case SECTION:

				pSection = AddIniSection(sReadw.substr(1, sReadw.Length() - 2));
				break;

			case KEY:
			{

				// Check to ensure valid section... or drop the keys listed
				if (pSection)
				{
					size_t iFind = sReadw.find_first_of(_T("="));
					String sKey = sReadw.substr(0, iFind);
					String sValue = sReadw.substr(iFind + 1);
					pSection->AddIniKeyValue(sKey, sValue);
				}
			}
			break;
			case COMMENT:

				break;
			case OTHER:

				break;
			}
		}
	}



	input.close();
	isLoad = true;
	return;
}
IniSection* IniFile::AddIniSection(String SectionName)
{
	IniSection* ist = FindIniSection(SectionName);
	if (ist == NULL)
	{
		ist = new IniSection(*SectionName.Trim());
		ls.push_back(ist);
	}
	return ist;
}
IniSection* IniFile::AddIniSection(IniSection* is)
{
	IniSection* ist = FindIniSection(is->GetName());
	if (ist == NULL)
	{
		ls.push_back(is);
		return is;
	}
	else
	{
		MergeIniSection(ist, is);
		return ist;
	}
}
void IniFile::RemoveAllSections()
{
	for each (IniSection* is in ls)
		delete is;
	ls.clear();
}
IniSection* IniFile::FindIniSection(String SectionName)
{
	SectionName.Trim();
	for each (IniSection* is in ls)
	{
		if (is->GetName() == SectionName)
		{
			return is;
		}
	}
	return NULL;
}
void IniFile::MergeIniSection(IniSection* is1, IniSection* is2)
{
	is1->MergeWith(is2);
}



