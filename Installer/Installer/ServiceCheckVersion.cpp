#include "stdafx.h"

#include "ServiceCheckVersion.h"
#include "ServiceRegistry.h"




ServiceCheckVersion::ServiceCheckVersion()
{

}
ServiceCheckVersion::~ServiceCheckVersion()
{

}


VersionDotNet  ServiceCheckVersion::CheckVersion()
{
	HKEY key;

	if (ServiceRegistry::GetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full\\", key) == ERROR_SUCCESS || ServiceRegistry::GetKey("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Client\\", key) == ERROR_SUCCESS)
	{
		DWORD Version;
		if (ServiceRegistry::GetDWORDRegKey(key, L"Release", Version, -1) == ERROR_SUCCESS)
		{
			switch (Version)
			{
			case 393297:
			case 393295:
				return v4_6;
			case 379893:
				return v4_5_2;
			case 378758:
			case 378675:
				return v4_5_1;
			case 378389:
				return v4_5;
			default:
				break;
			}
			if (Version > 393297)
				return v4_6Later;
		}
		return v4_5;
	}
	else if (ServiceRegistry::KeyExist("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4.0\\"))
	{
		return v4_0;
	}
	else if (ServiceRegistry::KeyExist("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v3.5\\"))
	{
		return v3_5;
	}
	else if (ServiceRegistry::KeyExist("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v3.0\\"))
	{
		return v3_0;
	}
	else if (ServiceRegistry::KeyExist("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v2.0.50727\\"))
	{
		return v2_0;
	}
	else if (ServiceRegistry::KeyExist("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v1.1.4322\\"))
	{
		return v1_1;
	}

	return vNotFound;
}
