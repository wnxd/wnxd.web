#include "init.h"

using namespace wnxd::javascript;
using namespace System;
using namespace System::Collections::Generic;

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// web�ӿ�,�̳иýӿڼ���ʵ��web�ӿ�
		/// </summary>
		public interface class Interface
		{

		};
		/// <summary>
		/// web�ӿڵ��û���,����web.config��<appSettings>�����ýӿ�����,����<add key="wnxd_interface_domain" value="�ӿ�����" />
		/// </summary>
		public ref class InterfaceBase
		{
		private:
			String^ interface_url, ^_namespace, ^_classname;
		protected:
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
		internal:
			static String^ EncryptString(String^ sInputString, String^ sKey);
			static String^ DecryptString(String^ sInputString, String^ sKey);
		protected:
			void Initialize() override;
			void Application_BeginRequest() override;
		};
	}
}