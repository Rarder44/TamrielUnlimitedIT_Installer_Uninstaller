#pragma once
#include "String.h"
#include <list>



class IniFile
{
public:
	class IniSection
	{
		friend class IniFile;

	public:
		class IniKeyValue
		{
			friend class IniSection;
		private:
			~IniKeyValue();
		public:
			IniKeyValue(String key, String value);
			String key;
			String value;
		};



	private:
		list<IniKeyValue*> lkv;
		String SectionName = "";
		~IniSection();

	public:
		IniSection(String SectionName);

		void SetSectionName(String SectionName);
		IniKeyValue* AddIniKeyValue(String key, String value);
		IniKeyValue* AddIniKeyValue(IniKeyValue* keyvalue);
		String GetName();
		IniKeyValue* FindKey(String key);

		String GetKeyValue(String key);

		void MergeWith(IniSection* is2);
		void RemoveAllKeys();

	};

	

private:
	list<IniSection*> ls;
	bool isLoad = false;
public:
	IniFile();

	~IniFile();
	void Load(String fileName);

	IniSection* AddIniSection(String SectionName);
	IniSection* AddIniSection(IniSection* is);
	void RemoveAllSections();
	IniSection* FindIniSection(String SectionName);
	void MergeIniSection(IniSection* is1, IniSection* is2);


};



typedef IniFile::IniSection IniSection;
typedef IniFile::IniSection::IniKeyValue IniKeyValue;
