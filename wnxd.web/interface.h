#include "init.h"

using namespace wnxd::javascript;
using namespace System;
using namespace System::Collections::Generic;

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
				property String^ summary
				{
					String^ get();
					void set(String^ value);
				}
			};
		};
		private ref struct _InterfaceInfo
		{
			property String^ Name;
		};
		private ref struct _CallInfo : _InterfaceInfo
		{
			property int Token;
			property json^ Param;
		};
		/// <summary>
		/// web�ӿڵ��û���,ִ�л����run�������ɵ���ָ�������µĽӿ�
		/// ������web.config��&lt;appSettings&gt;�����ýӿ�����,����&lt;add key=&quot;wnxd_interface_domain&quot; value=&quot;�ӿ�����&quot; /&gt;
		/// </summary>
		public ref class InterfaceBase
		{
		private:
			String^ interface_url, ^_domain, ^_namespace, ^_classname;
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
			/// <summary>
			/// ���ýӿڵ�ָ������
			/// </summary>
			/// <param name="function">������ʶ</param>
			/// <param name="args">����</param>
			/// <returns></returns>
			json^ Run(int function, ...array<Object^>^ args);
		public:
			InterfaceBase();
		};
		private ref class interface_enter : Enter
		{
		private:
			array<Type^>^ ilist;
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
				property int MethodToken;
				property String^ MethodName;
				property String^ ReturnType;
				property IList<_ParameterInfo^>^ Parameters;
				property String^ Summary;
			};
			ref struct _ClassInfo
			{
				property String^ Namespace;
				property String^ ClassName;
				property IList<_MethodInfo^>^ Methods;
			};
		internal:
			static String^ interface_name;
			static String^ interface_data;
			static String^ EncryptString(String^ sInputString, String^ sKey);
			static String^ DecryptString(String^ sInputString, String^ sKey);
		protected:
			virtual void Initialize() override;
			virtual void Application_BeginRequest() override;
		};
	}
}