namespace wnxd
{
	namespace Enum
	{
		/// <summary>
		/// 查询方式
		/// </summary>
		public enum QueryType
		{
			/// <summary>
			/// CSS选择语法
			/// </summary>
			Css_Path,
			/// <summary>
			/// 正则表达式
			/// </summary>
			Reg_Exp
		};
		public enum InputType
		{
			button,
			checkbox,
			color,
			date,
			datetime,
			datetime_local,
			month,
			week,
			time,
			email,
			file,
			hidden,
			image,
			number,
			password,
			radio,
			range,
			reset,
			search,
			submit,
			tel,
			text,
			url,
			none,
			unknown
		};
		generic <typename T> public ref class Enum
		{
		private:
			static Type^ _T = T::typeid;
		public:
			static String^ Format(Object^ value, String^ format);
			static String^ GetName(Object^ value);
			static array<String^>^ GetNames();
			static Type^ GetUnderlyingType();
			static Array^ GetValues();
			static bool IsDefined(Object^ value);
			static T Parse(String^ value);
			static T Parse(String^ value, bool ignoreCase);
			static bool TryParse(String^ value, T% result);
			static bool TryParse(String^ value, bool ignoreCase, T% result);
			static T ToObject(unsigned char value);
			static T ToObject(int value);
			static T ToObject(long long value);
			static T ToObject(Object^ value);
			static T ToObject(signed char value);
			static T ToObject(short value);
			static T ToObject(unsigned int value);
			static T ToObject(unsigned long long value);
			static T ToObject(unsigned short value);
		};
	}
}