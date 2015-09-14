#include "init.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::Specialized;
using namespace System::Linq;
using namespace System::Text;
using namespace System::Web;
using namespace wnxd::javascript;

namespace wnxd
{
	namespace Web
	{
		public ref class Cookie
		{
			static Cookie()
			{
				Enter::Register(cookie_enter::typeid, false);
			}
		internal:
			static json^ list = gcnew json();
		public:
			static void Init();
			static void Set(HttpCookie^ cookie);
			static void Set(HttpCookie^ cookie, String^ domain);
			static void Set(HttpCookie^ cookie, IList<String^>^ domains);
			static void Sync(String^ domain, HttpCookie^ cookie);
			static void Sync(IList<String^>^ domains, HttpCookie^ cookie);
		};
		ref class cookie_enter : Enter
		{
		public:
			void Application_BeginRequest() override;
			void Application_EndRequest() override;
		};
	}
}