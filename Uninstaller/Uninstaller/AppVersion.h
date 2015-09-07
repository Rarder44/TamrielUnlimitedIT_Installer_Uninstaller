#pragma once
#include "String.h"


using namespace std;

class AppVersion
{
private:
	String version = "";
	bool CorrectStringVersion(String s);
	static int Confronto(AppVersion av1, AppVersion av2);

public:
	AppVersion();
	AppVersion(String v);


	~AppVersion();
	 

	String getVersion();
	void setVersion(String v);


	friend  bool operator> (const AppVersion lhs, const AppVersion rhs);
	friend  bool operator< (const AppVersion lhs, const AppVersion rhs);
	friend  bool operator== (const AppVersion lhs, const AppVersion rhs);
	friend  bool operator>= (const AppVersion lhs, const AppVersion rhs);
	friend  bool operator<= (const AppVersion lhs, const AppVersion rhs);

};

