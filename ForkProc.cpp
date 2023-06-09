#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "ForkProc.h"

/*
Credit goes to D4stiny's ForkPlayground, a POC on process forking.
Blog Post explaining the process: https://billdemirkapi.me/abusing-windows-implementation-of-fork-for-stealthy-memory-operations/
*/

ForkProcess::ForkProcess(HANDLE hProcess) {
	this->ProcessHandle = hProcess;
	this->ForkedChildProcHandle = NULL;
}

ForkProcess::ForkProcess(DWORD ProcessID) {
	this->ForkedChildProcHandle = NULL;
	this->ProcessHandle = OpenProcess(PROCESS_CREATE_PROCESS, FALSE, ProcessID);
	if (this->ProcessHandle == INVALID_HANDLE_VALUE) {
		printError(TEXT("OpenProcess "));
	}
}

ForkProcess::~ForkProcess(VOID) {
	if (this->ForkedChildProcHandle != NULL) {
		this->CleanFork();
	}
}

HANDLE ForkProcess::Fork(VOID) {

	NTSTATUS status;

	if (this->ForkedChildProcHandle != NULL) {
		if (!this->CleanFork()) {
			printError(TEXT("CleanFork() "));
		}
	}
	
	status = NtCreateProcessEx(&this->ForkedChildProcHandle, PROCESS_ALL_ACCESS, NULL, this->ProcessHandle, 0, NULL, NULL, NULL, 0);
	
	if (NT_SUCCESS(status) == FALSE) {
		printError(TEXT("NtCreateProcessEx "));
		return NULL;
	}

	return this->ForkedChildProcHandle;

}

BOOL ForkProcess::CleanFork(VOID) {

	BOOL successFlag;
	successFlag = TRUE;

	if (this->ForkedChildProcHandle) {
		successFlag = TerminateProcess(this->ForkedChildProcHandle, 0);
		CloseHandle(this->ForkedChildProcHandle);
		if (!successFlag) {
			printError(TEXT("TerminateProcess "));
		}
		this->ForkedChildProcHandle = NULL;
	}
	return successFlag;
}


// Directly taken from the msdn docs
void printError(const TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}