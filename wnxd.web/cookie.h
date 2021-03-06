#include "init.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;
using namespace System::IO;

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// 实现跨域cookie同步
		/// </summary>
		public ref class Cookie
		{
		private:
			ref class cookie_enter : Enter
			{
			private:
				String^ HttpWriterRead(TextWriter^ hw, Encoding^ encoding);
			protected:
				virtual void Application_BeginRequest() override;
				virtual void Application_PostRequestHandlerExecute() override;
			};
		public:
			/// <summary>
			/// 设置并同步cookie给指定域名
			/// </summary>
			/// <param name="cookie">设置的cookie</param>
			static void Set(HttpCookie^ cookie);
			/// <summary>
			/// 设置并同步cookie给指定域名
			/// </summary>
			/// <param name="cookie">设置的cookie</param>
			/// <param name="domain">同步的域名</param>
			static void Set(HttpCookie^ cookie, String^ domain);
			/// <summary>
			/// 设置并同步cookie给指定域名列表
			/// </summary>
			/// <param name="cookie">设置的cookie</param>
			/// <param name="domains">同步的域名列表</param>
			static void Set(HttpCookie^ cookie, IList<String^>^ domains);
			/// <summary>
			/// 同步cookie给指定域名
			/// </summary>
			/// <param name="domain">同步的域名</param>
			/// <param name="cookie"></param>
			static void Sync(String^ domain, HttpCookie^ cookie);
			/// <summary>
			/// 同步cookie给指定域名列表
			/// </summary>
			/// <param name="domains">同步的域名列表</param>
			/// <param name="cookie"></param>
			static void Sync(IList<String^>^ domains, HttpCookie^ cookie);
		};
	}
}