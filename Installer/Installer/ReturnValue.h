#include "String.h"

#ifndef _ReturnValue
#define _ReturnValue

class ReturnValue
{
public:
	String msg;
	bool Status;
	DWORD exitCode;

	ReturnValue(bool Status, String msg, DWORD exitCode = -1);
	~ReturnValue();
};

#endif