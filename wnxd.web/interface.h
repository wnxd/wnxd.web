#include "init.h"

using namespace wnxd::javascript;
using namespace System;
using namespace System::Collections::Generic;

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
		public:
			InterfaceBase();
		};
		private ref class interface_enter : Enter
		{
		private:
			array<Type^>^ ilist;
			String^ GetGenericName(Type^ gt);
		internal:
			static String^ EncryptString(String^ sInputString, String^ sKey);
			static String^ DecryptString(String^ sInputString, String^ sKey);
		protected:
			virtual void Initialize() override;
			virtual void Application_BeginRequest() override;
		};
	}
}