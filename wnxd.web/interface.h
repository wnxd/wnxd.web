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
		/// web�ӿ�,�̳иýӿڿ������ⲿ����ͨ���ӿڵķ�ʽ���ô���
		/// </summary>
		public interface class Interface
		{
		public:
			/// <summary>
			/// �����Կ���Ϊweb�ӿڵķ��������������
			/// </summary>
			[AttributeUsage(AttributeTargets::Method)]
			ref class MethodAttribute sealed : Attribute
			{
			private:
				String^ _summary;
			public:
				/// <summary>
				/// �ӿڷ����ı�ע��Ϣ
				/// </summary>
				property String^ summary
				{
					String^ get();
					void set(String^ value);
				}
			};
			/// <summary>
			/// �����Կ���Ϊweb�ӿڵķ�����ӻ��湦��,Ĭ�ϻ���ʱ��Ϊ1800��
			/// </summary>
			[AttributeUsage(AttributeTargets::Method)]
			ref class MethodCacheAttribute sealed : Attribute
			{
			private:
				int _time = 1800;
			public:
				/// <summary>
				/// �ӿڷ��������ݻ���ʱ��,��λΪ��
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
		/// web�ӿڵ��û���,ִ�л����run�������ɵ���ָ�������µĽӿ�
		/// ������web.config��&lt;appSettings&gt;�����ýӿ�����,����&lt;add key=&quot;wnxd_interface_domain&quot; value=&quot;�ӿ�����&quot; /&gt;
		/// </summary>
		public ref class InterfaceBase
		{
		private:
			String^ _domain, ^_namespace, ^_classname, ^_fullname, ^interface_url;
			void init();
		protected:
			/// <summary>
			/// �ӿ���������,�����web.config��&lt;appSettings&gt;�����ýӿ�����,��Ĭ��ֵΪ�����õ�����,����ӿ�����Ϊ��������
			/// </summary>
			property String^ Domain
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// ���ýӿڵ������ռ�
			/// </summary>
			property String^ Namespace
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// ���ýӿڵ�����,Ĭ��Ϊ��ǰ�����������
			/// </summary>
			property String^ ClassName
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// ���ýӿڵ�ָ������
			/// </summary>
			/// <param name="function">������</param>
			/// <param name="args">����</param>
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