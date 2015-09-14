#include "config.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Text::RegularExpressions;
using namespace System::Web;
using namespace System::Web::SessionState;
using namespace wnxd::Config;
using namespace std;

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// 入口类,继承该类并调用Register进行注册即可监听应用程序相关事件
		/// </summary>
		public ref class Enter abstract
		{
		public:
			void Register();
			void Register(bool assembly);
			static void Register(Enter^ obj);
			static void Register(Enter^ obj, bool assembly);
			static void Register(Type^ enter);
			static void Register(Type^ enter, bool assembly);
			static void Unregister(Enter^ obj);
			static void Unregister(Type^ enter);

		};
		public ref class Init abstract : HttpApplication
		{
		private:
			Object^ _old_app;
			Type^ _old_type;
			void _init();
			void _callback(String^ method, array<Object^>^ parameters);
			void Application_Start(Object^ sender, EventArgs^ e);
			void Application_End(Object^ sender, EventArgs^ e);
			void Application_Error(Object^ sender, EventArgs^ e);
			void Application_BeginRequest(Object^ sender, EventArgs^ e);
			void Application_AuthenticateRequest(Object^ sender, EventArgs^ e);
			void Application_AuthorizeRequest(Object^ sender, EventArgs^ e);
			void Application_ResolveRequestCache(Object^ sender, EventArgs^ e);
			void Application_AcquireRequestState(Object^ sender, EventArgs^ e);
			void Application_PreRequestHandlerExecute(Object^ sender, EventArgs^ e);
			void Application_PostRequestHandlerExecute(Object^ sender, EventArgs^ e);
			void Application_ReleaseRequestState(Object^ sender, EventArgs^ e);
			void Application_UpdateRequestCache(Object^ sender, EventArgs^ e);
			void Application_EndRequest(Object^ sender, EventArgs^ e);
			void Application_PreSendRequestHeaders(Object^ sender, EventArgs^ e);
			void Application_PreSendRequestContent(Object^ sender, EventArgs^ e);
			void Session_Start(Object^ sender, EventArgs^ e);
			void Session_End(Object^ sender, EventArgs^ e);
			void Application_Disposed(Object^ sender, EventArgs^ e);
		internal:
			static IList<Enter^>^ _enter_list;
		protected:
			Init();
		};
	}
}