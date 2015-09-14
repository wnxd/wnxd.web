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
		/// �����,�̳и��ಢ����Register����ע�ἴ�ɼ���Ӧ�ó�������¼�
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
			virtual void Initialize();
			virtual void Application_Start();
			virtual void Application_End();
			virtual void Application_Error();
			virtual void Application_BeginRequest();
			virtual void Application_AuthenticateRequest();
			virtual void Application_AuthorizeRequest();
			virtual void Application_ResolveRequestCache();
			virtual void Application_AcquireRequestState();
			virtual void Application_PreRequestHandlerExecute();
			virtual void Application_PostRequestHandlerExecute();
			virtual void Application_ReleaseRequestState();
			virtual void Application_UpdateRequestCache();
			virtual void Application_EndRequest();
			virtual void Application_PreSendRequestHeaders();
			virtual void Application_PreSendRequestContent();
			virtual void Session_Start();
			virtual void Session_End();
			virtual void Application_Disposed();
			property HttpRequest^ Request
			{
				HttpRequest^ get();
			}
			property HttpResponse^ Response
			{
				HttpResponse^ get();
			}
			property HttpServerUtility^ Server
			{
				HttpServerUtility^ get();
			}
			property HttpSessionState^ Session
			{
				HttpSessionState^ get();
			}
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