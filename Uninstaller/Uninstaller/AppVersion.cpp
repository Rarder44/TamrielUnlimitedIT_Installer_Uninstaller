#include "stdafx.h"
#include "AppVersion.h"


AppVersion::AppVersion() {}
AppVersion::AppVersion(String v)
{
	setVersion(v);
}

AppVersion::~AppVersion() {}



String AppVersion::getVersion()
{
	return version;
}
void AppVersion::setVersion(String v)
{
	if (CorrectStringVersion(v))
		version = v;
}



bool AppVersion::CorrectStringVersion(String s)
{
	wstring ss = s.w();
	for each (TCHAR var in ss)
	{
		if (!((var >= '0' && var <= '9') || var == '.'))
			return false;
	}
}

int AppVersion::Confronto(AppVersion av1, AppVersion av2)
{
	
	list<String> my = av1.version.SplitString(".");
	list<String> other = av2.version.SplitString(".");

	if (my.size() > other.size())
		for (int c = my.size() - other.size(); c > 0; c--)
			other.push_back("0");
	else if (other.size() > my.size())
		for (int c = other.size() - my.size(); c > 0; c--)
			my.push_back("0");

	try
	{
		for (int i = my.size(); i > 0; i--)
		{
			int myI = stoi(my.front().s());
			int otherI = stoi(other.front().s());

			if (myI > otherI)
				return 1;
			else if (otherI > myI)
				return 2;

			my.pop_front();
			other.pop_front();
		}
		return 3;
	}
	catch (...)
	{
		return -1;
	}
}




bool operator> (const AppVersion lhs, const AppVersion rhs)
{
	return AppVersion::Confronto(lhs, rhs) == 1;
}
bool operator< (const AppVersion lhs, const AppVersion rhs)
{
	return AppVersion::Confronto(lhs, rhs) == 2;
}
bool operator== (const AppVersion lhs, const AppVersion rhs)
{
	return AppVersion::Confronto(lhs, rhs) == 3;
}
bool operator>= (const AppVersion lhs, const AppVersion rhs)
{
	int i = AppVersion::Confronto(lhs, rhs);
	return i == 1 || i == 3; 3;
}
bool operator<= (const AppVersion lhs, const AppVersion rhs)
{
	int i = AppVersion::Confronto(lhs, rhs);
	return i == 2 || i == 3;
}
