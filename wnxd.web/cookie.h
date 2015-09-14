namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// 实现跨域cookie同步
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
			protected:
				void Application_BeginRequest() override;
				void Application_EndRequest() override;
			};
		internal:
			static json^ list = gcnew json();
		public:
			/// <summary>
			/// 用于触发cookie的入口类注册
			/// </summary>
			static void Init();
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