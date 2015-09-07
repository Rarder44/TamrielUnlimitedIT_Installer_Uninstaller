#pragma once
#include "String.h"
class StringPath : public String
{

public:
	~StringPath();

	StringPath();
	StringPath(string s);
	StringPath(wstring s);
	StringPath(LPCWSTR s);
	StringPath(LPCSTR s);
	StringPath(int null);
	StringPath(const String & s);
	

	StringPath* TrimRSlashes();
	StringPath* TrimLSlashes();
	StringPath* RemoveDuplicateSlash();
	StringPath* ResolveEnviromentPath();

	StringPath* AddExtension(String ext);
	StringPath* AddPath(String path);

	StringPath GetParentDirectory();

	StringPath GetDrive();
	StringPath GetDirectory();
	StringPath GetFileName();
	StringPath GetFileNameExt();
	StringPath GetExt();

};

