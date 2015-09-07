#include "stdafx.h"
#include "String.h"
#include "ServiceString.h"

String::~String()
{

}


String::String()
{
	str = _T("");
}
String::String(string s)
{
	str = ServiceString::s2ws(s);
}
String::String(wstring s)
{
	str = s;
}
String::String(LPCWSTR s)
{
	str = s;
}
String::String(LPCSTR s)
{
	string ss = s;
	str = ServiceString::s2ws(ss);
}
String::String(char s)
{
	string ss = "";
	ss.push_back(s);
	str = ServiceString::s2ws(ss);
}
String::String(wchar_t s)
{
	wstring ss = _T("");
	ss.push_back(s);
	str =ss;
}
String::String(const String & s)
{
	str = s.str;
}
String::String(int null)
{
	str = _T("");
	if (null == 0)
		Null = true;

}



string String::s()
{
	return ServiceString::ws2s(str);
}
wstring String::w()
{
	return str;
}
LPCWSTR String::lpcwstr()
{
	return str.c_str();
}
LPWSTR String::lpwstr()
{
	return (wchar_t*)str.c_str();
}
LPCSTR String::lpcstr()
{
	string* s = new string(ServiceString::ws2s(str));
	return s->c_str();
}



size_t String::Length()
{
	return str.size();
}

String* String::RTrim(String chars )
{
	str.erase(str.find_last_not_of(chars.w()) + 1);
	return this;
}
String* String::LTrim(String chars)
{
	str.erase(0, str.find_first_not_of(chars.w()));
	return this;
}


String* String::Trim(String chars)
{
	str.erase(str.find_last_not_of(chars.w()) + 1);
	str.erase(0, str.find_first_not_of(chars.w()));
	return this;
}


String* String::ReplaceFirst(String find, String replace)
{
	int i = str.find(find.w());
	if (i != -1)
		str = str.replace(i, find.Length(), replace.w());
	return this;
}
String* String::ReplaceAll(String find, String replace)
{
	int i = -1;
	while ((i = str.find(find.w())) != -1)
		str = str.replace(i, find.Length(), replace.w());
	return this;
}


String String::substr(int start , int count )
{
	String t = str;
	return *t.substrModify(start, count);
}
String* String::substrModify(int start , int count )
{
	str = str.substr(start, count);
	return this;
}

size_t  String::find_first_of(String s)
{
	return str.find_first_of(s.w());
}




list<String> String::SplitString(String delimiter)
{
	wstring strt = str;
	list<String> s;
	int p = 0;
	while ((p = strt.find(delimiter.w(), 0)) && p != -1)
	{
		s.push_back(strt.substr(0, p));
		strt = strt.substr(p + 1);
	}
	s.push_back(strt);
	return s;
}


bool String::StartWith(String find)
{
	if (str.find(find.w()) == 0)
		return true;
	return false;
}

bool String::EndWith(String find)
{
	if (str.find(find.w()) == (str.length() - find.Length()))
		return true;
	return false;
}


String String::ToUpper()
{
	String t = str;
	t.ToUpperModify();
	return t;
}
String String::ToLower()
{
	String t = str;
	t.ToLowerModify();
	return t;
}
String* String::ToUpperModify()
{
	for (int i = 0; i < str.length(); i++)
		str[i] = toupper(str[i]);
	return this;
}
String* String::ToLowerModify()
{
	for (int i = 0; i < str.length(); i++)
		str[i] = tolower(str[i]);
	return this;
}


String* String::RemoveStart(String find)
{
	if (str.find(find.w()) == 0)
		str = str.substr(find.Length());
	return this;
}


String* String::Append(String s)
{
	str.append(s.w());
	return this;
}
String* String::Prepend(String s)
{
	str = s.w() + str;
	return this;
}


String* String::Clear()
{
	str.clear();
	return this;
}


size_t String::Find(String s)
{
	return str.find(s.w());
}


String* String::SetString(String s)
{
	str = s.str;
	return this;
}


bool String::Empty()
{
	return str == _T("");
}
bool String::IsNull()
{
	return Null;
}
bool String::NullOrEmpty()
{
	return Empty() || IsNull();
}






bool String::IsInt()
{
	std::wstring::const_iterator it = str.begin();
	while (it != str.end() && isdigit(*it))
		++it;
	return !str.empty() && it == str.end();
}

int String::ParseInt()
{
	return _wtoi(str.c_str());
}

bool operator==(const String& lhs, const String& rhs)
{
	return lhs.str == rhs.str;
}
bool operator==(const String& lhs, int rhs)
{
	return ((rhs == 0 && lhs.Null) || (rhs != 0 && !lhs.Null));
	
}
bool operator==(int lhs, const String& rhs)
{
	return rhs == lhs;
}
bool operator!=(const String& lhs, const String& rhs)
{
	return lhs.str != rhs.str;
}
String operator+(const String& lhs, const String& rhs)
{
	return lhs.str + rhs.str;
}
String& String::operator+=(const String& rhs)
{
	str += rhs.str;
	return *this;
}

String operator+(int lhs, const String& rhs)
{
	return to_wstring(lhs) + rhs.str;
}
String operator+(const String& lhs, int rhs)
{
	return lhs.str + to_wstring(rhs);
}
String& String::operator+=(int rhs)
{
	str += to_wstring(rhs);
	return *this;
}
char String::operator[](std::size_t idx)
{
	
	string s = lpcstr();
	if (idx >= s.length())
		return ' ';
	return s[idx];
}