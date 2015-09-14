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
		/// ���������ύ��web�ؼ�
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
			/// �ڿؼ�����λ�ü��ص�js
			/// </summary>
			property String^ Script
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// �ڿؼ�����λ�ü��ص�html����
			/// </summary>
			property String^ Html
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// �ύʱ���ڱ�ʶ�ı�־
			/// </summary>
			property String^ Prefix
			{
				String^ get();
				void set(String^ value);
			}
			/// <summary>
			/// �ύ���ݵ�Ŀ��url
			/// </summary>
			property String^ TargetUrl
			{
				String^ get();
				void set(String^ value);
			}
			void OnLoad(EventArgs^ e) override;
			void Render(HtmlTextWriter^ writer) override;
			/// <summary>
			/// �ڿؼ�����λ�����һ��js
			/// </summary>
			/// <param name="script"></param>
			void AddScript(String^ script);
			/// <summary>
			/// �ڿؼ�����λ�����һ��html����
			/// </summary>
			/// <param name="html"></param>
			void AddHtml(String^ html);
			/// <summary>
			/// �ӵ�ǰҳ�������Ѱ�ҵ�һ��web����
			/// </summary>
			/// <returns></returns>
			static Web^ FindFirst();
			/// <summary>
			/// �ӵ�ǰҳ������а�ָ��idѰ��web����
			/// </summary>
			/// <param name="id"></param>
			/// <returns></returns>
			static Web^ Find(String^ id);
			/// <summary>
			/// �ӵ�ǰҳ�������Ѱ�����е�web����
			/// </summary>
			/// <returns></returns>
			static array<Web^>^ Find();
		};
	}
}