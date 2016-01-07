#include <Windows.h>

extern void CSharp_DllMain(HMODULE module);

DWORD WINAPI thread(void* module)
{
	CSharp_DllMain((HMODULE)module);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE h, DWORD reasonForCall, void* resv)
{
	if (reasonForCall == DLL_PROCESS_ATTACH) CloseHandle(CreateThread(0, 0, thread, h, 0, 0));
	return TRUE;
}