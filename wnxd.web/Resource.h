//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 wnxd.web.rc 使用
//
#define rc_cookie                 101
#define rc_load                 102
#define rc_web                 103

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        105
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

namespace wnxd
{
	namespace Web
	{
		ref class Resource
		{
		private:
			static System::String^ _cookie, ^_load, ^_web;
			static System::String^ GetResource(int id);
		public:
			static property System::String^ cookie
			{
				System::String^ get();
			}
			static property System::String^ load
			{
				System::String^ get();
			}
			static property System::String^ web
			{
				System::String^ get();
			}
		};
	}
}