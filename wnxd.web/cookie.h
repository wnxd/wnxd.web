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
		/// <summary>
		/// ʵ�ֿ���cookieͬ��
		/// </summary>
		public ref class Cookie
		{
			static Cookie()
			{
				Enter::Register(cookie_enter::typeid, false);
			}
		private:
			ref class cookie_enter : Enter
			{
			private:
				String^ HttpWriterRead(TextWriter^ hw, Encoding^ encoding);
			protected:
				void Application_BeginRequest() override;
				void Application_PostRequestHandlerExecute() override;
			};
		internal:
			static json^ list = gcnew json();
		public:
			/// <summary>
			/// ���ڴ���cookie�������ע��
			/// </summary>
			static void Init();
			/// <summary>
			/// ���ò�ͬ��cookie��ָ������
			/// </summary>
			/// <param name="cookie">���õ�cookie</param>
			static void Set(HttpCookie^ cookie);
			/// <summary>
			/// ���ò�ͬ��cookie��ָ������
			/// </summary>
			/// <param name="cookie">���õ�cookie</param>
			/// <param name="domain">ͬ��������</param>
			static void Set(HttpCookie^ cookie, String^ domain);
			/// <summary>
			/// ���ò�ͬ��cookie��ָ�������б�
			/// </summary>
			/// <param name="cookie">���õ�cookie</param>
			/// <param name="domains">ͬ���������б�</param>
			static void Set(HttpCookie^ cookie, IList<String^>^ domains);
			/// <summary>
			/// ͬ��cookie��ָ������
			/// </summary>
			/// <param name="domain">ͬ��������</param>
			/// <param name="cookie"></param>
			static void Sync(String^ domain, HttpCookie^ cookie);
			/// <summary>
			/// ͬ��cookie��ָ�������б�
			/// </summary>
			/// <param name="domains">ͬ���������б�</param>
			/// <param name="cookie"></param>
			static void Sync(IList<String^>^ domains, HttpCookie^ cookie);
		};
	}
}