#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Psapi.h>
#include <iostream>

void printError(const TCHAR* msg);

BOOL getProcessList() {

	HANDLE ProcessHandle;
	HANDLE ProcessHandleSnap;
	PROCESSENTRY32 hProcessSnap;


	ProcessHandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (ProcessHandleSnap == INVALID_HANDLE_VALUE) {
		printError(TEXT("CreateToolhelp32Snapshot failed to create Snapshot with flag TH32CS_SNAPPROCESS"));
		return FALSE;
	}

	hProcessSnap.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(ProcessHandleSnap, &hProcessSnap)) {
		printError(TEXT("Failed to grab the first process from the list."));
		CloseHandle(ProcessHandleSnap);
		return FALSE;
	}

	do {
		_tprintf(TEXT("\n\n======================================================="));
		_tprintf("\nProcess Name: %s", hProcessSnap.szExeFile);
		_tprintf(TEXT("\n======================================================="));
		_tprintf(TEXT("\n  Process ID        = 0x%08X"), hProcessSnap.th32ProcessID);
		_tprintf(TEXT("\n  Thread count      = %d"), hProcessSnap.cntThreads);
		_tprintf(TEXT("\n  Parent process ID = 0x%08X"), hProcessSnap.th32ParentProcessID);
		_tprintf(TEXT("\n  Priority base     = %d"), hProcessSnap.pcPriClassBase);
		
	} while (Process32Next(ProcessHandleSnap, &hProcessSnap));
	CloseHandle(ProcessHandleSnap);
	return TRUE;
}

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

int main() {
	getProcessList();
	return 0;
}