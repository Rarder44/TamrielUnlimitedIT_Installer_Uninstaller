#pragma once
#include "stdafx.h"
#include "ServiceResource.h"
#include <fstream>

ServiceResource::ServiceResource() {

}
ServiceResource::~ServiceResource() {

}

void ServiceResource::ExtractBinResource(String strCustomResName, int nResourceId, String strOutputName)
{
	HGLOBAL hResourceLoaded;  // handle to loaded resource
	HRSRC   hRes;              // handle/ptr to res. info.
	char    *lpResLock;        // pointer to resource data
	DWORD   dwSizeRes;

	hRes = FindResource(NULL, MAKEINTRESOURCE(nResourceId), strCustomResName.lpcwstr());

	hResourceLoaded = LoadResource(NULL, hRes);
	lpResLock = (char *)LockResource(hResourceLoaded);
	dwSizeRes = SizeofResource(NULL, hRes);

	ofstream outputFile(strOutputName.lpcwstr(), std::ios::binary);
	outputFile.write((const char *)lpResLock, dwSizeRes);
	outputFile.close();
}



