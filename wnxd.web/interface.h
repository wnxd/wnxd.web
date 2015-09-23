#include "init.h"

using namespace System;
using namespace System::Collections::Generic;

namespace wnxd
{
	namespace Web
	{
		public interface class Interface
		{

		};
		private ref class interface_enter : Enter
		{
		private:
			array<Type^>^ ilist;
			String^ EncryptString(String^ sInputString, String^ sKey);
			String^ DecryptString(String^ sInputString, String^ sKey);
		protected:
			void Initialize() override;
			void Application_BeginRequest() override;
		};
	}
}