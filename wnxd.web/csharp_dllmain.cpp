#include <Windows.h>
#include "init.h"

using namespace wnxd::Web;
using namespace System::IO;
using namespace System::Text;

void CSharp_DllMain(HMODULE h, DWORD reasonForCall, void* resv)
{
	String^ path = Path::GetDirectoryName(Assembly::GetExecutingAssembly()->Location);
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