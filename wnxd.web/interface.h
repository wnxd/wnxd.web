#include "init.h"

using namespace wnxd::javascript;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Net;
using namespace System::Net::Sockets;

#define Interface_Name_Key "wnxd: interface_name"
#define Interface_Data_Key "wnxd: interface_data"

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// web接口,继承该接口可以让外部程序通过接口的方式调用此类
		/// </summary>
		public interface class Interface
		{
		public:
			/// <summary>
			/// 此属性可以为web接口的方法添加描述内容
			/// </summary>
			[AttributeUsage(AttributeTargets::Method)]
			ref class MethodAttribute sealed : Attribute
			{
			private:
				String^ _summary;
			public:
				/// <summary>
				/// 接口方法的备注信息
				/// </summary>
				property String^ summary
				{
					String^ get();
					void set(String^ value);
				}
			};
			/// <summary>
			/// 此属性可以为web接口的方法添加缓存功能,默认缓存时间为1800秒
			/// </summary>
			[AttributeUsage(AttributeTargets::Method)]
			ref class MethodCacheAttribute sealed : Attribute
			{
			private:
				int _time = 1800;
			public:
				/// <summary>
				/// 接口方法的数据缓存时间,单位为秒
				/// </summary>
				property int time
				{
					int get();
					void set(int value);
				}
			};
		};
		private ref struct _CallInfo
		{
			property String^ Name;
			property json^ Param;
		};
		/// <summary>
		/// web接口调用基类,执行基类的run方法即可调用指定域名下的接口
		/// 可以在web.config的&lt;appSettings&gt;中配置接口域名,例如&lt;add key=&quot;wnxd_interface_domain&quot; value=&quot;接口域名&quot; /&gt;
		/// </summary>
		public ref class InterfaceBase
		{
		private:
			String^ _domain, ^_namespace, ^_classname, ^_fullname, ^interface_url;
			void init();
		protected:
			/// <summary>
			/// 接口所在域名,如果在web.config的&lt;appSettings&gt;中配置接口域名,则默认值为所配置的域名,否则接口域名为自身域名
			/// </summary>
			property String^ Domain
			{
				String^ get();
				void set(String^ value);
			}
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
			json^ GetCache(int time, String^ function, ...array<Object^>^ args);
		public:
			InterfaceBase();
		};
		private ref class interface_enter : Enter
		{
		private:
			IDictionary<Type^, IDictionary<String^, MethodInfo^>^>^ ilist;
			String^ GetGenericName(Type^ gt);
			enum class _ParameterType
			{
				In,
				Out,
				Retval
			};
			ref struct _ParameterInfo
			{
				property String^ ParameterName;
				property _ParameterType Type;
				property bool IsOptional;
				property Object^ DefaultValue;
				property String^ ParameterType;
			};
			ref struct _MethodInfo
			{
				property String^ MethodToken;
				property String^ MethodName;
				property String^ ReturnType;
				property IList<_ParameterInfo^>^ Parameters;
				property String^ Summary;
				property Nullable<int> CacheTime;
			};
			ref struct _ClassInfo
			{
				property String^ Namespace;
				property String^ ClassName;
				property IList<_MethodInfo^>^ Methods;
			};
			ref struct _OutData
			{
				property IList<Object^>^ OutParams;
				property json^ Data;
			};
		internal:
			static String^ interface_name;
			static String^ interface_data;
		protected:
			virtual void Initialize() override;
			virtual void Application_Start() override;
			virtual void Application_BeginRequest() override;
		};
	}
}