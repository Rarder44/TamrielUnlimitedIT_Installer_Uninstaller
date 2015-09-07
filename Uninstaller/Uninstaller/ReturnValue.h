#include "String.h"
class ReturnValue
{
public:
	String msg;
	bool Status;
	DWORD exitCode;

	ReturnValue(bool Status, String msg, DWORD exitCode = -1);
	~ReturnValue();
};