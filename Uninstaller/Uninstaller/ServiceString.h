#include "stdafx.h"
#include <string>
#include <list>
#include "String.h"

using namespace std;

class ServiceString
{
public:
	ServiceString();
	~ServiceString();

	static wstring s2ws(string str);
	static string ws2s(wstring wstr);


	static wstring RemoveTailChars(wstring s, char c);
	static wstring RemoveHeadChars(wstring s, char c);



	static bool StartWith(String str, String find);
	static bool EndWith(String str, String find);

	
	static wstring RemoveStart(wstring str, wstring find);
	static string RemoveStart(string str, string find);

	static string ToUpper(string str);
	static wstring ToUpper(wstring str);


	static list<string> SplitString(string str, string delimiter);
	static list<wstring> SplitString(wstring str, wstring delimiter);

	static string RTrim(string str, string chars = " \t");
	static string LTrim(string str, string chars = " \t");
	static string Trim(string str, string chars = " \t");

	static wstring RTrim(wstring str, wstring chars = _T(" \t"));
	static wstring LTrim(wstring str, wstring chars = _T(" \t"));
	static wstring Trim(wstring str, wstring chars = _T(" \t"));


	static wstring ReplaceFirst(wstring str, wstring find, wstring replace);
	static wstring ReplaceAll(wstring str, wstring find, wstring replace);



};

