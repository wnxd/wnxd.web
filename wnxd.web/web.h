#include "Resource.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Reflection;
using namespace System::Web;
using namespace System::Web::UI;

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// 用于数据提交的web控件
		/// </summary>
		[ParseChildren(true)]
		[PersistChildren(false)]
		public ref class Web : Control
		{
		private:
			String^ _prefix = "wnxd_ajax", ^_script, ^_html, ^_targeturl;
			static Web^ _FindFirst(Control^ control);
			static Web^ _Find(Control^ control, String^ id);
			static List<Web^>^ _Find(Control^ control);
		internal:
			bool Insert = false;
		public:
			/// <summary>
			/// 在控件所在位置加载的js
			/// </summary>
			property String^ Script
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// 在控件所在位置加载的html代码
			/// </summary>
			property String^ Html
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// 提交时用于辨识的标志
			/// </summary>
			property String^ Prefix
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// 提交数据的目标url
			/// </summary>
			property String^ TargetUrl
			{
				String^ get();
				void set(String^ value);
			}
			void OnLoad(EventArgs^ e) override;
			void Render(HtmlTextWriter^ writer) override;
			/// <summary>
			/// 在控件所在位置添加一个js
			/// </summary>
			/// <param name="script"></param>
			void AddScript(String^ script);
			/// <summary>
			/// 在控件所在位置添加一个html代码
			/// </summary>
			/// <param name="html"></param>
			void AddHtml(String^ html);
			/// <summary>
			/// 从当前页面对象中寻找第一个web对象
			/// </summary>
			/// <returns></returns>
			static Web^ FindFirst();
			/// <summary>
			/// 从当前页面对象中按指定id寻找web对象
			/// </summary>
			/// <param name="id"></param>
			/// <returns></returns>
			static Web^ Find(String^ id);
			/// <summary>
			/// 从当前页面对象中寻找所有的web对象
			/// </summary>
			/// <returns></returns>
			static array<Web^>^ Find();
		};
	}
}