#pragma once
#include <winternl.h>

class ForkProcess {
	HANDLE ProcessHandle;
	HANDLE ForkedChildProcHandle;

public:
	ForkProcess(HANDLE hProcess);
	ForkProcess(DWORD ProcessID);
	~ForkProcess(VOID);
	HANDLE Fork(VOID);
	BOOL CleanFork(VOID);
};

void printError(const TCHAR* msg);

extern "C"
{
    NTSYSCALLAPI
        NTSTATUS
        NTAPI
        NtCreateProcessEx(
            _Out_ PHANDLE ProcessHandle,
            _In_ ACCESS_MASK DesiredAccess,
            _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
            _In_ HANDLE ParentProcess,
            _In_ ULONG Flags,
            _In_opt_ HANDLE SectionHandle,
            _In_opt_ HANDLE DebugPort,
            _In_opt_ HANDLE ExceptionPort,
            _In_ ULONG JobMemberLevel
        );
}