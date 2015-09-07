#pragma once
#include "String.h"
#include "ReturnValue.h"

using namespace std;
class  ServiceExecuteCommand
{
public:
	ServiceExecuteCommand();
	 ~ServiceExecuteCommand();

	 static ReturnValue executeCommandLineSync(String cmdLine);

	 static ReturnValue executeCommandLineAsync(String cmdLine);

};
