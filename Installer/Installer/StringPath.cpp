#pragma once
#include "stdafx.h"
#include "StringPath.h"
#include "ServiceString.h"
#include "ServicePath.h"

StringPath::StringPath() : String()
{

}
StringPath::StringPath(string s) : String(s)
{

}
StringPath::StringPath(wstring s):String(s)
{

}
StringPath::StringPath(LPCWSTR s):String(s)
{

}
StringPath::StringPath(LPCSTR s):String(s)
{

}
StringPath::StringPath(const String & s):String(s)
{

}
StringPath::StringPath(int null) : String(null)
{

}



StringPath::~StringPath()
{
}


StringPath* StringPath::TrimRSlashes()
{
	RTrim("\\/");
	return this;
}
StringPath* StringPath::TrimLSlashes()
{
	LTrim("\\/");
	return this;
}
StringPath* StringPath::RemoveDuplicateSlash()
{
	wstring st, ss;
	st = str;
	bool precedent = false;
	for (int i = 0; i < st.length(); i++)
		if (st[i] == '\\' || st[i] == '/')
		{
			if (!precedent)
			{
				precedent = true;
				ss += '\\';
			}
		}
		else
		{
			precedent = false;
			ss += st[i];
		}
	str = ss;
	return this;
}
StringPath* StringPath::ResolveEnviromentPath()
{
	
	bool scambio = true;
	while (scambio)
	{
		scambio = false;
		int iniz = str.find(_T("%"));
		if (iniz != -1)
		{
			iniz++;
			int fin = str.find(_T("%"), iniz);
			if (fin != -1)
			{
				string st = getenv(ServiceString::ws2s(str.substr(iniz, fin - iniz)).c_str());
				iniz--;
				str = str.replace(iniz, (fin - iniz) + 1, ServiceString::s2ws(st));
				scambio = true;
			}
		}
	}

	return this;
}
StringPath* StringPath::AddExtension(String ext)
{
	if (!ToUpper().EndWith(ext.ToUpper()))
	{
		if (!ext.StartWith("."))
			ext.Prepend(".");

		Append(ext);
	}
	return this;
}
StringPath* StringPath::AddPath(String path)
{
	SetString(ServicePath::ComprimePath(*this, path));
	return this;
}


StringPath StringPath::GetParentDirectory()
{
	size_t last_slash_idx = str.rfind('\\');
	if (std::string::npos != last_slash_idx)
		return str.substr(0, last_slash_idx);
	else
		return "";
}


StringPath StringPath::GetDrive()
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t Ext[MAX_PATH];
	_wsplitpath(lpcwstr(), Drive, Dir, FileName, Ext);

	return Drive;
}
StringPath StringPath::GetDirectory()
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t Ext[MAX_PATH];
	_wsplitpath(lpcwstr(), Drive, Dir, FileName, Ext);

	return ServicePath::ComprimePath(Drive, Dir);
}
StringPath StringPath::GetFileName()
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t Ext[MAX_PATH];
	_wsplitpath(lpcwstr(), Drive, Dir, FileName, Ext);

	return FileName;
}
StringPath StringPath::GetFileNameExt()
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t Ext[MAX_PATH];
	_wsplitpath(lpcwstr(), Drive, Dir, FileName, Ext);

	StringPath t = FileName;
	
	return *t.AddExtension(Ext);
}
StringPath StringPath::GetExt()
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t Ext[MAX_PATH];
	_wsplitpath(lpcwstr(), Drive, Dir, FileName, Ext);

	return Ext;
}





