#pragma once
#include "String.h"
class GlobVar
{
public:

	static String DefaultNameUninstallIni;
	static String CurrentPath;

	static String PreTempFolder;
	static String postTempFolder;


	GlobVar();
	~GlobVar();
};

