#include "init.h"

using namespace wnxd::javascript;
using namespace System;
using namespace System::Collections::Generic;

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// web接口,继承该接口即可实现web接口
		/// </summary>
		public interface class Interface
		{

		};
		/// <summary>
		/// web接口调用基类,需在web.config的<appSettings>中配置接口域名,例如<add key="wnxd_interface_domain" value="接口域名" />
		/// </summary>
		public ref class InterfaceBase
		{
		private:
			String^ interface_url, ^_namespace, ^_classname;
		protected:
			/// <summary>
			/// 调用接口的命名空间
			/// </summary>
			property String^ Namespace
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// 调用接口的类名,默认为当前派生类的类名
			/// </summary>
			property String^ ClassName
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// 调用接口的指定方法
			/// </summary>
			/// <param name="function">方法名</param>
			/// <param name="args">参数</param>
			/// <returns></returns>
			json^ Run(String^ function, ...array<Object^>^ args);
		public:
			InterfaceBase();
		};
		private ref class interface_enter : Enter
		{
		private:
			array<Type^>^ ilist;
		internal:
			static String^ EncryptString(String^ sInputString, String^ sKey);
			static String^ DecryptString(String^ sInputString, String^ sKey);
		protected:
			void Initialize() override;
			void Application_BeginRequest() override;
		};
	}
}