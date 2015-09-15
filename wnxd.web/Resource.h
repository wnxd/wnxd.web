using namespace System;

namespace wnxd
{
	namespace Web
	{
		ref class Resource
		{
		private:
			static System::Resources::ResourceManager^ resourceMan;
			static System::Globalization::CultureInfo^ resourceCulture;
		internal:
			property static System::Resources::ResourceManager^ ResourceManager
			{
				System::Resources::ResourceManager^ get();
			}
			property static System::Globalization::CultureInfo^ Culture
			{
				System::Globalization::CultureInfo^ get();
				void set(System::Globalization::CultureInfo^ value);
			}
			property static String^ cookie
			{
				String^ get();
			}
			property static String^ load
			{
				String^ get();
			}
			property static String^ web
			{
				String^ get();
			}
		};
	}
}