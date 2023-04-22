#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Psapi.h>
#include <iostream>
#include "ForkProc.h"
//#pragma comment(lib, "ForkProcess.dll")
void printError(const TCHAR* msg);


// Function to enumerate the processes
BOOL getProcessList() {

	HANDLE ProcessHandle;
	HANDLE ProcessHandleSnap;
	PROCESSENTRY32 hProcessSnap;


	ProcessHandleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (ProcessHandleSnap == INVALID_HANDLE_VALUE) {
		printError(TEXT("CreateToolhelp32Snapshot "));
		return FALSE;
	}

	hProcessSnap.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(ProcessHandleSnap, &hProcessSnap)) {
		printError(TEXT("Process32First "));
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

int main() {
	getProcessList();
	return 0;
}