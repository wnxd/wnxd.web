#include "init.h"

using namespace wnxd::javascript;
using namespace System;
using namespace System::Collections::Generic;

namespace wnxd
{
	namespace Web
	{
		public interface class Interface
		{

		};
		public ref class InterfaceBase
		{
		private:
			String^ interface_url, ^_namespace, ^_classname;
		protected:
			property String^ Namespace
			{
				String^ get();
				void set(String^ value);
			}
			property String^ ClassName
			{
				String^ get();
				void set(String^ value);
			}
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