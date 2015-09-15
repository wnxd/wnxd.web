using namespace System;

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
		public enum class InputType
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
	}
}