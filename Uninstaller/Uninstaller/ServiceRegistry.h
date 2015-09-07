
#pragma once
#include <windows.h>
#include "String.h"


using namespace std;
class ServiceRegistry
{
public:


	static LONG GetDWORDRegKey(HKEY hKey, String strValueName, DWORD &nValue, DWORD nDefaultValue);
	static LONG GetDWORDRegKey(String hKey, String strValueName, DWORD &nValue, DWORD nDefaultValue);

	static LONG GetBoolRegKey(HKEY hKey, String strValueName, bool &bValue, bool bDefaultValue);
	static LONG GetBoolRegKey(String hKey, String strValueName, bool &bValue, bool bDefaultValue);

	static LONG GetStringRegKey(HKEY hKey, String strValueName, String &strValue, String strDefaultValue);
	static LONG GetStringRegKey(String hKey, String strValueName, String &strValue, String strDefaultValue);




	static LONG SetDWORDValue(HKEY hKey, String ValueName, DWORD data);
	static LONG SetDWORDValue(String hKey, String ValueName, DWORD data);


	static LONG SetBoolValue(HKEY hKey, String ValueName, bool data);
	static LONG SetBoolValue(String hKey, String ValueName, bool data);

	static LONG SetStringValue(HKEY hKey, String ValueName, String data);
	static LONG SetStringValue(String hKey, String ValueName, String data);





	static bool KeyExist(String hKey);
	static LONG GetKey(String hKey, HKEY &hKeyOut);

	static LONG CreateKey(HKEY hKey, String s, HKEY &Newkey);
	static LONG CreateKey(String hKey, String s, HKEY &Newkey);



	static LONG RemoveValue(HKEY hKey, String ValueName);
	static LONG RemoveValue(String hKey, String ValueName);

	static LONG RemoveKey(HKEY hKey, String subkey);
	static LONG RemoveKey(String hKey, String subkey);

	ServiceRegistry();
	~ServiceRegistry();
};

