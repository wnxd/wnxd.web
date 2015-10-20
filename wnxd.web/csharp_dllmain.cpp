#include <Windows.h>
#include "init.h"

using namespace wnxd::Web;
using namespace System::IO;
using namespace System::Text;
using namespace System::Runtime::InteropServices;

#define LPSTR2String(lpstr) Marshal::PtrToStringUni((IntPtr)lpstr)

void CSharp_DllMain(HMODULE h, DWORD reasonForCall, void* resv)
{
	WCHAR pDllFileName[MAX_PATH];
	GetModuleFileName(h, pDllFileName, MAX_PATH);
	String^ path = Path::GetDirectoryName(LPSTR2String(&pDllFileName));
	path += "\\__AssemblyInfo__.ini";
	if (File::Exists(path))
	{
		String^ str = File::ReadAllText(path, Encoding::Unicode);
		if (!String::IsNullOrEmpty(str))
		{
			int start = str->IndexOf("file:///") + 8;
			str = str->Substring(start)->Replace("\0", "");
			path = Path::GetDirectoryName(str);
			str = path->Substring(path->Length - 3);
			if (str->ToLower() == "bin") path = path->Substring(0, path->Length - 3);
			Enter::_Init(path);
		}
	}
}