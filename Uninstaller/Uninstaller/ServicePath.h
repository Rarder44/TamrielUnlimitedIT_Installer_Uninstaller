#pragma once
#include "stdafx.h"
#include <string>
#include "String.h"
#include "StringPath.h"

using namespace std;


class ServicePath
{

public:
	ServicePath();
	~ServicePath();



	static String ComprimePath(StringPath arg1, StringPath arg2, StringPath arg3 = NULL, StringPath arg4 = NULL, StringPath arg5 = NULL, StringPath arg6 = NULL, StringPath arg7 = NULL, StringPath arg8 = NULL);


	static String TrimRSlashes(String s);
	static String TrimLSlashes(String s);

	static String RemoveDuplicateSlash(String s);

	static String GetParentDirectory(String s);

	static String ResolveEnviromentPath(String s);

	static String AddExtension(String FileName, String ext);


	static String  GetFileName(String Path);
	static String  GetFileNameExt(String Path);

};

