using namespace System;
using namespace System::Xml::Linq;

namespace wnxd
{
	namespace Config
	{
		ref class config
		{
		private:
			String^ _path;
			XDocument^ _dom;
			bool _modify;
		public:
			config(String^ path);
			~config();
			String^ Read(String^ key);
			void Write(String^ key, String^ val);
			String^ GetAttr(String^ key, String^ name);
			void SetAttr(String^ key, String^ name, String^ val);
			property String^ default[String^]
			{
				String^ get(String^ key);
				void set(String^ key, String^ value);
			}
		};
		ref class cache
		{
		private:
			String^ _path;
			double _time;
		public:
			cache(String^ name, double time);
			String^ Read(String^ key);
			String^ Read(String^ item, String^ key);
			String^ Read(String^ item, String^ item2, String^ key);
			String^ Read(array<String^>^ items, String^ key);
			void Write(String^ key, String^ val);
			void Write(String^ item, String^ key, String^ val);
			void Write(String^ item, String^ item2, String^ key, String^ val);
			void Write(array<String^>^ items, String^ key, String^ val);
		};
		ref class file
		{
		public:
			static String^ ReadFile(String^ path);
			static void WriteFile(String^ path, String^ contents);
		};
	}
}