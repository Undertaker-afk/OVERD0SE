#include "security.h"
#include "xorstr.h"
#include "../core/sdk.h"

#ifndef _DEBUG
// dynamic resolved functions
typedef NTSTATUS(__stdcall* _NtQueryInformationProcess)(_In_ HANDLE, _In_  unsigned int, _Out_ PVOID, _In_ ULONG, _Out_ PULONG);
typedef NTSTATUS(__stdcall* _NtSetInformationThread)(_In_ HANDLE, _In_ THREAD_INFORMATION_CLASS, _In_ PVOID, _In_ ULONG);

bool RemoteDebuggerPresent() // checks if a debugger is running (in another system/process)
{
	HANDLE h_process = INVALID_HANDLE_VALUE;
	BOOL found = FALSE;

	// set the process handle to the current process
	h_process = GetCurrentProcess();

	// check if a remote debugger is present
	CheckRemoteDebuggerPresent(h_process, &found);

	// if found is true, we return the right code.
	return found;
}

bool BeingDebuggedPEB() // checks the process environment block (peb) for a "being debugged" field ( gets set if process is launched in a debugger )
{
	return (BOOL)__readgsbyte(0x60 + 0x02); // PEB is at GS:[0x60]
}

bool NTSetInformationThread() // hides the thread from any debugger, any attempt to control the process after this call ends the debugging session
{
	// get a handle to ntdll.dll so we can use NtQueryInformationProcess
	HMODULE h_ntdll = LoadLibraryA(CXOR("ntdll.dll"));

	// if we cant get the handle for some reason, we return none
	if (h_ntdll == INVALID_HANDLE_VALUE || h_ntdll == NULL)
		return true;

	_NtQueryInformationProcess NtQueryInformationProcess = NULL;
	NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(h_ntdll, CXOR("NtQueryInformationProcess"));

	// if we cant get access for some reason, we return none
	if (NtQueryInformationProcess == NULL)
		return true;

	// make call to detach a debugger :moyai:
	(_NtSetInformationThread)(GetCurrentThread(), /*thread_hide_from_debugger*/0x11, 0, 0, 0);

	return true;
}
#endif

bool security::checker()
{
#ifndef _DEBUG
	// no comments ...
	std::thread([&]()
		{
			while (true)
			{
				if (IsDebuggerPresent())
				{
					cheat->unload = true;
					return false;
				}

				if (RemoteDebuggerPresent()) 
				{
					cheat->unload = true;
					return false;
				}

				// don't put too much stress on the cpu.
				Sleep(1);
			}
		}).detach();

	// check
	if (!BeingDebuggedPEB())
	{
		cheat->unload = true;
		return false;
	}

	// check
	if (!NTSetInformationThread()) 
	{
		cheat->unload = true;
		return false;
	}

#endif
	return true;
}