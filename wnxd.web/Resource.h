#define rc_cookie  101
#define rc_load  102
#define rc_web  103

namespace wnxd
{
	namespace Web
	{
		ref class Resource
		{
		private:
			static System::String^ _cookie, ^_load, ^_web;
			static System::String^ GetResource(int id);
		public:
			static property System::String^ cookie
			{
				System::String^ get();
			}
			static property System::String^ load
			{
				System::String^ get();
			}
			static property System::String^ web
			{
				System::String^ get();
			}
		};
	}
}