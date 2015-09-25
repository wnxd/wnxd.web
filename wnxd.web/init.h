using namespace System;
using namespace System::Collections::Generic;
using namespace System::Web;
using namespace System::Web::SessionState;
using namespace System::Reflection;

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// 入口类,继承该类并调用Register进行注册即可监听应用程序相关事件
		/// </summary>
		public ref class Enter abstract
		{
		protected:
			/// <summary>
			/// 当ASP.NET环境初始化时触发。
			/// </summary>
			virtual void Initialize();
			/// <summary>
			/// 当应用程序启动时触发。
			/// </summary>
			virtual void Application_Start();
			/// <summary>
			/// 应用程序结束时触发。
			/// </summary>
			virtual void Application_End();
			/// <summary>
			/// 无论在应用程序中何时何处发生未处理的错误都将触发。
			/// </summary>
			virtual void Application_Error();
			/// <summary>
			/// 当ASP.NET开始处理每个请求时触发。
			/// </summary>
			virtual void Application_BeginRequest();
			/// <summary>
			/// 在验证请求之前触发。
			/// </summary>
			virtual void Application_AuthenticateRequest();
			/// <summary>
			/// 在为请求授权之前触发。
			/// </summary>
			virtual void Application_AuthorizeRequest();
			/// <summary>
			/// 在ASP.NET确定是否应该生成新的输出，或者由缓存填充前触发。
			/// </summary>
			virtual void Application_ResolveRequestCache();
			/// <summary>
			/// 在获取会话状态之前执行。
			/// </summary>
			virtual void Application_AcquireRequestState();
			/// <summary>
			/// 在将请求发送到服务于请求的处理程序对象之前触发。
			/// </summary>
			virtual void Application_PreRequestHandlerExecute();
			/// <summary>
			/// 当HTTP处理程序与页面请求一起完成时触发。
			/// </summary>
			virtual void Application_PostRequestHandlerExecute();
			/// <summary>
			/// 当释放和更新试图状态时触发。
			/// </summary>
			virtual void Application_ReleaseRequestState();
			/// <summary>
			/// 如果输出被缓存，那么缓存更新时将触发。
			/// </summary>
			virtual void Application_UpdateRequestCache();
			/// <summary>
			/// 当请求结束时执行。
			/// </summary>
			virtual void Application_EndRequest();
			/// <summary>
			/// 在向客户端发送HTTP头之前触发。
			/// </summary>
			virtual void Application_PreSendRequestHeaders();
			/// <summary>
			/// 向客户端发送HTTP内容之前触发。
			/// </summary>
			virtual void Application_PreSendRequestContent();
			/// <summary>
			/// 每个会话开始时触发。
			/// </summary>
			virtual void Session_Start();
			/// <summary>
			/// 会话结束时触发。
			/// </summary>
			virtual void Session_End();
			/// <summary>
			/// 当CLR从内存中移除应用程序时触发。
			/// </summary>
			virtual void Application_Disposed();
			/// <summary>
			/// 为当前 HTTP 请求获取 System.Web.HttpRequest 对象。
			/// </summary>
			property HttpRequest^ Request
			{
				HttpRequest^ get();
			}
			/// <summary>
			/// 为当前 HTTP 响应获取 System.Web.HttpResponse 对象。
			/// </summary>
			property HttpResponse^ Response
			{
				HttpResponse^ get();
			}
			/// <summary>
			/// 获取提供用于处理 Web 请求的方法的 System.Web.HttpServerUtility 对象。
			/// </summary>
			property HttpServerUtility^ Server
			{
				HttpServerUtility^ get();
			}
			/// <summary>
			/// 为当前 HTTP 请求获取 System.Web.SessionState.HttpSessionState 对象。
			/// </summary>
			property HttpSessionState^ Session
			{
				HttpSessionState^ get();
			}
		public:
			/// <summary>
			/// 初始化入口类相关配置
			/// </summary>
			static void Init();
		};
		/// <summary>
		/// 特殊类,此类无法被创建与派生
		/// </summary>
		public ref class Init abstract : HttpApplication
		{
		private:
			Object^ _old_app;
			Type^ _old_type;
			void _init();
			void _callback(String^ method, ...array<Object^>^ parameters);
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
			static array<Assembly^>^ GetAllAssembly();
		protected:
			Init();
		};
	}
}