#include <Windows.h>

extern void CSharp_DllMain(HMODULE h, DWORD reasonForCall, void* resv);

struct Info {
	HMODULE h;
	DWORD reasonForCall;
	void* resv;
};

DWORD WINAPI thread(void* info)
{
	Info* i = (Info*)info;
	CSharp_DllMain(i->h, i->reasonForCall, i->resv);
	free(i);
	return 0;
}

extern "C" BOOL APIENTRY DllMain(HMODULE h, DWORD reasonForCall, void* resv)
{
	if (reasonForCall == DLL_PROCESS_ATTACH)
	{
		Info *info = (Info*)malloc(sizeof(Info));
		info->h = h;
		info->reasonForCall = reasonForCall;
		info->resv = resv;
		CloseHandle(CreateThread(0, 0, thread, info, 0, 0));
	}
	return TRUE;
}