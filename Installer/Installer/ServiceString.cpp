#include "stdafx.h"
#include "ServiceString.h"

ServiceString::ServiceString() {}
ServiceString::~ServiceString() {}

wstring ServiceString::s2ws(string str)
{
	CString s(str.c_str());
	wstring ss(s);
	return ss;
}
string ServiceString::ws2s(wstring wstr)
{
	char* temp = new char[wstr.length()];
	wcstombs_s(NULL, temp, wstr.length() + 1, wstr.c_str(), wstr.length());
	string ss(temp);
	return ss;
}


wstring ServiceString::RemoveTailChars(wstring s, char c)
{
	while (s[s.size() - 1] == c)
		s = s.substr(0, s.size() - 1);
	return s;
}
wstring ServiceString::RemoveHeadChars(wstring s, char c)
{
	while (s.size() != 0 && (s[0] == c))
		return s.substr(1, s.size());
	return s;
}



bool ServiceString::StartWith(String str, String find)
{
	if (str.Find(find) == 0)
		return true;
	return false;
}
bool ServiceString::EndWith(String str, String find)
{
	if (str.Find(find) == (str.Length() - find.Length()))
		return true;
	return false;
}


wstring ServiceString::RemoveStart(wstring str, wstring find)
{
	if (str.find(find) == 0)
		return str.substr(find.length());
	return str;
}
string ServiceString::RemoveStart(string str, string find)
{
	if (str.find(find) == 0)
		return str.substr(find.length());
	return str;
}

string ServiceString::ToUpper(string str)
{
	for (int i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return str;
}
wstring ServiceString::ToUpper(wstring str)
{
	for (int i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return str;
}


list<string> ServiceString::SplitString(string str, string delimiter)
{
	list<string> s;
	int p = 0;
	while ((p = str.find(delimiter, 0)) && p != -1)
	{
		s.push_back(str.substr(0, p));
		str = str.substr(p + 1);
	}
	s.push_back(str);
	return s;
}
list<wstring> ServiceString::SplitString(wstring str, wstring delimiter)
{
	list<wstring> s;
	int p = 0;
	while ((p = str.find(delimiter, 0)) && p != -1)
	{
		s.push_back(str.substr(0, p));
		str = str.substr(p + 1);
	}
	s.push_back(str);
	return s;
}

string ServiceString::RTrim(string str, string chars)
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}
string ServiceString::LTrim(string str, string chars)
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}
string ServiceString::Trim(string str, string chars)
{
	str.erase(str.find_last_not_of(chars) + 1);
	str.erase(0, str.find_first_not_of(chars));
	return str;
}

wstring ServiceString::RTrim(wstring str, wstring chars )
{
	wstring s;
	s = str.erase(str.find_last_not_of(chars) + 1);
	return s;
}
wstring ServiceString::LTrim(wstring str, wstring chars )
{
	wstring s;
	s = str.erase(0, str.find_first_not_of(chars));
	return s;
}
wstring ServiceString::Trim(wstring str, wstring chars)
{
	str.erase(str.find_last_not_of(chars) + 1);
	str.erase(0, str.find_first_not_of(chars));
	return str;
}


wstring ServiceString::ReplaceFirst(wstring str, wstring find, wstring replace)
{
	int i = str.find(find);
	if (i != -1)
		return  str.replace(i, find.size(), replace);
	return str;
}
wstring ServiceString::ReplaceAll(wstring str, wstring find, wstring replace)
{
	int i = -1;
	while ((i = str.find(find)) != -1)
		str = str.replace(i, find.size(), replace);
	return str;
}

