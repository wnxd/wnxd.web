namespace wnxd
{
	namespace Config
	{
		ref class config
		{
		private:
			String^ _path;
		public:
			config(String^ path);
			String^ Read(String^ key);
			void Write(String^ key, String^ val);
			property String^ default[String^]
			{
				String^ get(String^ key);
				void set(String^ key, String^ value);
			}
		};
		ref class file
		{
		public:
			static String^ ReadFile(String^ path);
			static void WriteFile(String^ path, String^ contents);
		};
	}
}