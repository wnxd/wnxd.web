namespace wnxd
{
	namespace Web
	{
		public ref class Load : Control
		{
		private:
			int _threshold = 200;
			bool _jump = false, _refresh = false;
			bool _SaveHtml();
			ref class load_enter : Enter
			{
			protected:
				void Application_BeginRequest() override;
			};
		internal:
			bool Insert = false;
		public:
			property int Threshold
			{
				int get();
				void set(int value);
			}
			property String^ Img;
			Load();
			void OnInit(EventArgs^ e) override;
			void Render(HtmlTextWriter^ writer) override;
		};
	}
}