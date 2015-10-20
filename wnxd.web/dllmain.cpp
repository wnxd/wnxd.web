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
	return 0;
}

extern "C" BOOL APIENTRY DllMain(HMODULE h, DWORD reasonForCall, void* resv)
{
	if (reasonForCall == DLL_PROCESS_ATTACH)
	{
		Info info;
		info.h = h;
		info.reasonForCall = reasonForCall;
		info.resv = resv;
		CreateThread(0, 0, thread, &info, 0, 0);
	}
	return TRUE;
}