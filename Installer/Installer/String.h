#include <string>
#include <list>

using namespace std;
#pragma once
class String
{
protected:
	wstring str;
	bool Null = false;
public:

	~String();

	String();
	String(string s);
	String(wstring s);
	String(LPCWSTR s);
	String(LPCSTR s);
	String(char s);
	String(wchar_t s);

	String(int null);
	
	String(const String & s);



	string s();
	wstring w();
	LPCWSTR lpcwstr();
	LPWSTR lpwstr();
	LPCSTR lpcstr();


	String* SetString(String s);
	

	size_t Length();

	String* RTrim(String chars = " \t");
	String* LTrim(String chars = " \t");
	String* Trim(String chars = " \t");

	String* ReplaceFirst(String find, String replace);
	String* ReplaceAll(String find, String replace);

	String substr(int start=0 , int count= INT_MAX);
	String* substrModify(int start = 0, int count = INT_MAX);


	size_t find_first_of(String s);



	list<String> SplitString(String delimiter);



	bool StartWith(String find);
	bool EndWith(String find);


	String ToUpper();
	String ToLower();
	String* ToUpperModify();
	String* ToLowerModify();


	
	String* RemoveStart(String find);


	String* Append(String s);
	String* Prepend(String s);


	String* Clear();


	size_t Find(String s);

	bool Empty();
	bool IsNull();
	bool NullOrEmpty();


	bool IsInt();

	int ParseInt();





	friend bool operator==(const String& lhs, const String& rhs);
	friend bool operator==(const String& lhs, int rhs);
	friend bool operator==(int lhs, const String& rhs);
	friend bool operator!=(const String& lhs, const String& rhs);
	friend String operator+(const String& lhs, const String& rhs);
	friend String operator+(const String& lhs, int rhs);
	friend String operator+(int lhs, const String& rhs);
	String& operator+=(const String& rhs);
	String& operator+=(int rhs);
	char operator[](std::size_t idx);

	/*operator LPCWSTR() const {
		return str.c_str();
	}
	operator string() const {
		return ServiceString::ws2s(str);
	}
	operator wstring() const {
		return str;
	}
	operator LPWSTR() const {
		return (wchar_t*)str.c_str();;
	}
	operator LPCSTR() const {
		string* s = new string(ServiceString::ws2s(str));
		return s->c_str();
	}
	*/

};

