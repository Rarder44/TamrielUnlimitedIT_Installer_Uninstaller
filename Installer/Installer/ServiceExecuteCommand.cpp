#include "stdafx.h"
#include "ServiceExecuteCommand.h"


ServiceExecuteCommand::ServiceExecuteCommand(){}
ServiceExecuteCommand::~ServiceExecuteCommand(){}

ReturnValue ServiceExecuteCommand::executeCommandLineSync(String cmdLine)
{
	DWORD exitCode;
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	int nStrBuffer = cmdLine.Length() + 50;

	// Create the process
	BOOL result = CreateProcess(NULL, cmdLine.lpwstr(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInformation);


	if (!result)
	{
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		// Display the error
		String err = "Error: %s";
		err+=((LPTSTR)lpMsgBuf);



		return ReturnValue(false, err);
	}
	else
	{
		// Successfully created the process.  Wait for it to finish.
		WaitForSingleObject(processInformation.hProcess, INFINITE);

		// Get the exit code.
		result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

		// Close the handles.
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		if (!result)
		{
			// Could not get exit code.
			return ReturnValue(false, _T("Couldn't get exit code"), exitCode);
		}


		// We succeeded.
		return ReturnValue(true, "", exitCode);
	}
}

ReturnValue ServiceExecuteCommand::executeCommandLineAsync(String cmdLine)
{
	DWORD exitCode;
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFO startupInfo = { 0 };
	startupInfo.cb = sizeof(startupInfo);
	int nStrBuffer = cmdLine.Length() + 50;


	// Create the process
	BOOL result = CreateProcess(NULL, cmdLine.lpwstr(),
		NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW,
		NULL, NULL, &startupInfo, &processInformation);


	if (!result)
	{
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		// Display the error
		String err = "Error: %s";
		err += ((LPTSTR)lpMsgBuf);


		return ReturnValue(false, err);
	}
	else
	{
		return ReturnValue(true, "");
	}
}