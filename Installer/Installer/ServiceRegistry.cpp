#include "stdafx.h"
#include "ServiceRegistry.h"
#include "ServicePath.h"




LONG ServiceRegistry::GetDWORDRegKey(HKEY hKey, String strValueName, DWORD &nValue, DWORD nDefaultValue)
{
	nValue = nDefaultValue;
	DWORD dwBufferSize(sizeof(DWORD));
	DWORD nResult(0);
	LONG nError = ::RegQueryValueExW(hKey,strValueName.lpcwstr(),0,	NULL,reinterpret_cast<LPBYTE>(&nResult),&dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		nValue = nResult;
	}

	return nError;
}
LONG ServiceRegistry::GetDWORDRegKey(String hKey, String strValueName, DWORD &nValue, DWORD nDefaultValue)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return GetDWORDRegKey(key, strValueName,  nValue, nDefaultValue);
	else
		nValue = nDefaultValue;
	return l;
}

LONG ServiceRegistry::GetBoolRegKey(HKEY hKey, String strValueName, bool &bValue, bool bDefaultValue)
{
	DWORD nDefValue((bDefaultValue) ? 1 : 0);
	DWORD nResult(nDefValue);
	LONG nError = GetDWORDRegKey(hKey, strValueName.lpcwstr(), nResult, nDefValue);
	if (ERROR_SUCCESS == nError)
	{
		bValue = (nResult != 0) ? true : false;
	}
	return nError;
}
LONG ServiceRegistry::GetBoolRegKey(String hKey, String strValueName, bool &bValue, bool bDefaultValue)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return GetBoolRegKey(key, strValueName, bValue, bDefaultValue);
	else
		bValue = bDefaultValue;
	return l;
}

LONG ServiceRegistry::GetStringRegKey(HKEY hKey, String strValueName, String &strValue, String strDefaultValue)
{
	strValue = strDefaultValue;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hKey, strValueName.lpcwstr(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		strValue = szBuffer;
	}
	return nError;
}
LONG ServiceRegistry::GetStringRegKey(String hKey, String strValueName, String &strValue, String strDefaultValue)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return GetStringRegKey(key, strValueName, strValue, strDefaultValue);
	else
		strValue = strDefaultValue;
	return l;
}




LONG ServiceRegistry::SetDWORDValue(HKEY hKey, String ValueName, DWORD data)
{
	return RegSetValueEx(hKey, ValueName.lpcwstr(), 0, REG_DWORD, (const BYTE*)&data, sizeof(data));
}
LONG ServiceRegistry::SetDWORDValue(String hKey, String ValueName, DWORD data)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return SetDWORDValue(key, ValueName, data);
	return l;
}


LONG ServiceRegistry::SetBoolValue(HKEY hKey, String ValueName, bool data)
{
	return SetDWORDValue(hKey, ValueName, data ? 1 : 0);
}
LONG ServiceRegistry::SetBoolValue(String hKey, String ValueName, bool data)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return SetBoolValue(key, ValueName, data);
	return l;
}

LONG ServiceRegistry::SetStringValue(HKEY hKey, String ValueName, String data)
{
	return RegSetValueEx(hKey, ValueName.lpcwstr(), 0, REG_SZ, (const BYTE*)data.lpcwstr(), (data.Length() + 1) * sizeof(wchar_t));
}
LONG ServiceRegistry::SetStringValue(String hKey, String ValueName, String data)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return SetStringValue(key, ValueName, data);
	return l;
}





bool ServiceRegistry::KeyExist(String hKey)
{
	HKEY key;
	if (GetKey(hKey, key) == ERROR_SUCCESS)
		return true;
	return false;
}
LONG ServiceRegistry::GetKey(String hKey, HKEY &hKeyOut)
{
	if (hKey.StartWith("HKLM"))
	{
		hKey.RemoveStart("HKLM");
		return RegOpenKeyEx(HKEY_LOCAL_MACHINE, ServicePath::TrimLSlashes(hKey).lpcwstr(), 0, KEY_ALL_ACCESS, &hKeyOut);

	}
	else if (hKey.StartWith("HKEY_LOCAL_MACHINE"))
	{
		hKey.RemoveStart("HKEY_LOCAL_MACHINE");
		return RegOpenKeyEx(HKEY_LOCAL_MACHINE, ServicePath::TrimLSlashes(hKey).lpcwstr(), 0, KEY_ALL_ACCESS, &hKeyOut);

	}
	else if (hKey.StartWith("HKCU"))
	{
		hKey.RemoveStart("HKCU");
		return RegOpenKeyEx(HKEY_CURRENT_USER, ServicePath::TrimLSlashes(hKey).lpcwstr(), 0, KEY_ALL_ACCESS, &hKeyOut);
	
	}
	else if (hKey.StartWith("HKEY_CURRENT_USER"))
	{
		hKey.RemoveStart("HKEY_CURRENT_USER");
		return RegOpenKeyEx(HKEY_CURRENT_USER, ServicePath::TrimLSlashes(hKey).lpcwstr(), 0, KEY_ALL_ACCESS, &hKeyOut);
	
	}
	return -1;
}

LONG ServiceRegistry::CreateKey(HKEY hKey, String s, HKEY &Newkey)
{
	return RegCreateKey(hKey, s.lpcwstr(), &Newkey);
}
LONG ServiceRegistry::CreateKey(String hKey, String s, HKEY &Newkey)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return CreateKey(key, s, Newkey);
	return l;
}



LONG ServiceRegistry::RemoveValue(HKEY hKey, String ValueName)
{
	return RegDeleteValue(hKey, ValueName.lpcwstr());
}
LONG ServiceRegistry::RemoveValue(String hKey, String ValueName)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return RemoveValue(key, ValueName);
	return l;
}

LONG ServiceRegistry::RemoveKey(HKEY hKey, String subkey)
{
	return RegDeleteKey(hKey, subkey.lpcwstr());
}
LONG ServiceRegistry::RemoveKey(String hKey, String subkey)
{
	HKEY key;
	LONG l = GetKey(hKey, key);
	if (l == ERROR_SUCCESS)
		return RemoveKey(key, subkey);
	return l;
}


ServiceRegistry::ServiceRegistry() {}
ServiceRegistry::~ServiceRegistry() {}