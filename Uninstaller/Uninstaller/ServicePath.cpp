#pragma once
#include "stdafx.h"
#include "ServicePath.h"
#include "ServiceString.h"
#include <stdarg.h>



ServicePath::ServicePath() {}
ServicePath::~ServicePath() {}

String ServicePath::ComprimePath(StringPath arg1, StringPath arg2, StringPath arg3, StringPath arg4, StringPath arg5, StringPath arg6, StringPath arg7, StringPath arg8)
{

	arg1.Trim();
	arg1.TrimRSlashes();

	if (!arg2.NullOrEmpty())
	{
		arg2.Trim(" \t\\/");
		arg1.Append("\\");
		arg1.Append(arg2);
	}
	
	if (!arg3.NullOrEmpty())
	{
		arg3.Trim(" \t\\/");
		arg1.Append("\\");
		arg1.Append(arg3);
	}

	if (!arg4.NullOrEmpty())
	{
		arg4.Trim(" \t\\/");
		arg1.Append("\\");
		arg1.Append(arg4);
	}

	if (!arg5.NullOrEmpty())
	{
		arg5.Trim(" \t\\/");
		arg1.Append("\\");
		arg1.Append(arg5);
	}

	if (!arg6.NullOrEmpty())
	{
		arg6.Trim(" \t\\/");
		arg1.Append("\\");
		arg1.Append(arg6);
	}

	if (!arg7.NullOrEmpty())
	{
		arg7.Trim(" \t\\/");
		arg1.Append("\\");
		arg1.Append(arg7);
	}

	if (!arg8.NullOrEmpty())
	{
		arg8.Trim(" \t\\/");
		arg1.Append("\\");
		arg1.Append(arg8);
	}

	
	return arg1;
}



String ServicePath::TrimRSlashes(String s)
{
	return ServiceString::RTrim(s.w(), _T("\\/"));
}
String ServicePath::TrimLSlashes(String s)
{
	return ServiceString::LTrim(s.w(), _T("\\/"));
}




String ServicePath::RemoveDuplicateSlash(String s)
{
	wstring ss;
	wstring st = s.w();
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
	return ss;
}

String ServicePath::GetParentDirectory(String s)
{
	s = TrimRSlashes(s);
	wstring t = s.w();
	size_t last_slash_idx = t.rfind('\\');
	if (std::string::npos != last_slash_idx)
		return t.substr(0, last_slash_idx);
	else
		return "";
}

String ServicePath::ResolveEnviromentPath(String s)
{
	wstring t = s.w();
	bool scambio = true;
	while (scambio)
	{
		scambio = false;
		int iniz = t.find(_T("%"));
		if (iniz != -1)
		{
			iniz++;
			int fin = t.find(_T("%"), iniz);
			if (fin != -1)
			{
				string st = getenv(ServiceString::ws2s(t.substr(iniz, fin - iniz)).c_str());
				iniz--;
				t = t.replace(iniz, (fin - iniz) + 1, ServiceString::s2ws(st));
				scambio = true;
			}
		}
	}

	return t;
}

String ServicePath::AddExtension(String FileName, String ext)
{
	if (!FileName.ToUpper().EndWith(ext.ToUpper()))
	{
		if (!ext.StartWith("."))
			ext.Prepend(".");

		FileName.Append(ext);
	}
	return FileName;
}

String ServicePath::GetFileName(String Path)
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t Ext[MAX_PATH];
	_wsplitpath(Path.lpcwstr(), Drive, Dir, FileName, Ext);

	return FileName;
}

String ServicePath::GetFileNameExt(String Path)
{
	wchar_t Drive[MAX_PATH];
	wchar_t Dir[MAX_PATH];
	wchar_t FileName[MAX_PATH];
	wchar_t Ext[MAX_PATH];
	_wsplitpath(Path.lpcwstr(), Drive, Dir, FileName, Ext);

	StringPath sp = FileName;
	return *sp.AddExtension(Ext);;
}


