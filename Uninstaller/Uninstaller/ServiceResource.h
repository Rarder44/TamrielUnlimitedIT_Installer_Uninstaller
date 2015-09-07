#pragma once
#include "stdafx.h"
#include "String.h"

class ServiceResource
{
public:
	ServiceResource();
	~ServiceResource();

	static void ExtractBinResource(String strCustomResName, int nResourceId, String strOutputName);


};

