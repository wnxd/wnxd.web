#include "Resource.h"
#include <Windows.h>

using namespace wnxd::Web;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;
//class Resource
//private
System::String^ Resource::GetResource(int id)
{
	void* ptr = (void*)Marshal::StringToHGlobalAnsi(Assembly::GetExecutingAssembly()->ManifestModule->Name);
	HMODULE hExe = GetModuleHandleA((LPCSTR)ptr);
	HRSRC hRes = FindResource(hExe, MAKEINTRESOURCE(id), TEXT("JAVASCRIPT"));
	if (hRes == NULL) return nullptr;
	HGLOBAL hgRes = LoadResource(hExe, hRes);
	LPVOID pRes = LockResource(hgRes);
	DWORD nResSize = SizeofResource(hExe, hRes);
	System::String^ s = Marshal::PtrToStringAnsi((System::IntPtr)pRes, nResSize);
	FreeResource(hgRes);
	return s;
}
//public
System::String^ Resource::cookie::get()
{
	if (_cookie == nullptr) _cookie = GetResource(rc_cookie);
	return _cookie;
}
System::String^ Resource::load::get()
{
	if (_load == nullptr) _load = GetResource(rc_load);
	return _load;
}
System::String^ Resource::web::get()
{
	if (_web == nullptr) _web = GetResource(rc_web);
	return _web;
}