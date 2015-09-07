
#include "stdafx.h"
#include "ReturnValue.h"


ReturnValue::ReturnValue(bool Status, String msg, DWORD exitCode )
{
	this->Status = Status;
	this->msg = msg;
	this->exitCode = exitCode;
}
ReturnValue::~ReturnValue()
{

}