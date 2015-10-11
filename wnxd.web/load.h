#include "init.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Web::UI;

namespace wnxd
{
	namespace Web
	{
		public ref class Load : Control
		{
		private:
			ref class load_enter : Enter
			{
			protected:
				virtual void Application_BeginRequest() override;
			};
			int _threshold;
			bool _jump, _refresh;
			bool _SaveHtml();
			static Load^ _FindFirst(Control^ control);
			static Load^ _Find(Control^ control, String^ id);
			static List<Load^>^ _Find(Control^ control);
		internal:
			bool Insert;
		public:
			property int Threshold
			{
				int get();
				void set(int value);
			}
			property String^ Img;
			Load();
			virtual void OnInit(EventArgs^ e) override;
			virtual void Render(HtmlTextWriter^ writer) override;
			static Load^ FindFirst();
			static Load^ Find(String^ id);
			static array<Load^>^ Find();
		};
	}
}