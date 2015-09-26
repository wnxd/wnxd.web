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
		/// �����,�̳и��ಢ����Register����ע�ἴ�ɼ���Ӧ�ó�������¼�
		/// </summary>
		public ref class Enter abstract
		{
		protected:
			/// <summary>
			/// ��ASP.NET������ʼ��ʱ������
			/// </summary>
			virtual void Initialize();
			/// <summary>
			/// ��Ӧ�ó�������ʱ������
			/// </summary>
			virtual void Application_Start();
			/// <summary>
			/// Ӧ�ó������ʱ������
			/// </summary>
			virtual void Application_End();
			/// <summary>
			/// ������Ӧ�ó����к�ʱ�δ�����δ����Ĵ��󶼽�������
			/// </summary>
			virtual void Application_Error();
			/// <summary>
			/// ��ASP.NET��ʼ����ÿ������ʱ������
			/// </summary>
			virtual void Application_BeginRequest();
			/// <summary>
			/// ����֤����֮ǰ������
			/// </summary>
			virtual void Application_AuthenticateRequest();
			/// <summary>
			/// ��Ϊ������Ȩ֮ǰ������
			/// </summary>
			virtual void Application_AuthorizeRequest();
			/// <summary>
			/// ��ASP.NETȷ���Ƿ�Ӧ�������µ�����������ɻ������ǰ������
			/// </summary>
			virtual void Application_ResolveRequestCache();
			/// <summary>
			/// �ڻ�ȡ�Ự״̬֮ǰִ�С�
			/// </summary>
			virtual void Application_AcquireRequestState();
			/// <summary>
			/// �ڽ������͵�����������Ĵ���������֮ǰ������
			/// </summary>
			virtual void Application_PreRequestHandlerExecute();
			/// <summary>
			/// ��HTTP���������ҳ������һ�����ʱ������
			/// </summary>
			virtual void Application_PostRequestHandlerExecute();
			/// <summary>
			/// ���ͷź͸�����ͼ״̬ʱ������
			/// </summary>
			virtual void Application_ReleaseRequestState();
			/// <summary>
			/// �����������棬��ô�������ʱ��������
			/// </summary>
			virtual void Application_UpdateRequestCache();
			/// <summary>
			/// ���������ʱִ�С�
			/// </summary>
			virtual void Application_EndRequest();
			/// <summary>
			/// ����ͻ��˷���HTTPͷ֮ǰ������
			/// </summary>
			virtual void Application_PreSendRequestHeaders();
			/// <summary>
			/// ��ͻ��˷���HTTP����֮ǰ������
			/// </summary>
			virtual void Application_PreSendRequestContent();
			/// <summary>
			/// ÿ���Ự��ʼʱ������
			/// </summary>
			virtual void Session_Start();
			/// <summary>
			/// �Ự����ʱ������
			/// </summary>
			virtual void Session_End();
			/// <summary>
			/// ��CLR���ڴ����Ƴ�Ӧ�ó���ʱ������
			/// </summary>
			virtual void Application_Disposed();
			/// <summary>
			/// Ϊ��ǰ HTTP �����ȡ System.Web.HttpRequest ����
			/// </summary>
			property HttpRequest^ Request
			{
				HttpRequest^ get();
			}
			/// <summary>
			/// Ϊ��ǰ HTTP ��Ӧ��ȡ System.Web.HttpResponse ����
			/// </summary>
			property HttpResponse^ Response
			{
				HttpResponse^ get();
			}
			/// <summary>
			/// ��ȡ�ṩ���ڴ��� Web ����ķ����� System.Web.HttpServerUtility ����
			/// </summary>
			property HttpServerUtility^ Server
			{
				HttpServerUtility^ get();
			}
			/// <summary>
			/// Ϊ��ǰ HTTP �����ȡ System.Web.SessionState.HttpSessionState ����
			/// </summary>
			property HttpSessionState^ Session
			{
				HttpSessionState^ get();
			}
		public:
			/// <summary>
			/// ��ʼ��������������
			/// </summary>
			static void Init();
		};
		/// <summary>
		/// ������,�����޷�������������
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