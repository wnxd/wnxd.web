using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Text::RegularExpressions;
using namespace System::Web::UI;
using namespace wnxd::Asm;
using namespace wnxd::Enum;

namespace wnxd
{
	namespace Web
	{
		public ref class Html : Control
		{
			static Html()
			{
				Hook::Register(Hook::GetMethod(Dictionary<String^, String^>::typeid, "Add"), Hook::GetMethod(Html::typeid, "Dictionary_Add"));
				Hook::Register(Hook::GetMethod(Dictionary<String^, String^>::typeid, "set_Item"), Hook::GetMethod(Html::typeid, "Dictionary_Item_set"));
				Hook::Register(Hook::GetMethod(Dictionary<String^, String^>::typeid, "Remove"), Hook::GetMethod(Html::typeid, "Dictionary_Remove"));
				Hook::Register(Hook::GetMethod(Dictionary<String^, String^>::typeid, "Clear"), Hook::GetMethod(Html::typeid, "Dictionary_Clear"));
			}
			~Html();
		private:
			static IDictionary<Html^, IList<String^>^>^ List_hooklist = gcnew Dictionary<Html^, IList<String^>^>();
			static IDictionary<Html^, IDictionary<String^, String^>^>^ Dictionary_hooklist = gcnew Dictionary<Html^, IDictionary<String^, String^>^>();
			static void List_Add(List<Object^>^ target, Object^ item);
			static void List_AddRange(List<Object^>^ target, IEnumerable<Object^>^ collection);
			static void List_Item_set(List<Object^>^ target, int index, Object^ item);
			static void List_Insert(List<Object^>^ target, int index, Object^ item);
			static void List_InsertRange(List<Object^>^ target, int index, IEnumerable<Object^>^ collection);
			static bool List_Remove(List<Object^>^ target, Object^ item);
			static int List_RemoveAll(List<Object^>^ target, Predicate<Object^>^ match);
			static void List_RemoveAt(List<Object^>^ target, int index);
			static void List_RemoveRange(List<Object^>^ target, int index, int count);
			static void List_Clear(List<Object^>^ target);
			static void Dictionary_Add(Dictionary<Object^, Object^>^ target, Object^ key, Object^ value);
			static void Dictionary_Item_set(Dictionary<Object^, Object^>^ target, Object^ key, Object^ value);
			static void Dictionary_Remove(Dictionary<Object^, Object^>^ target, Object^ key);
			static void Dictionary_Clear(Dictionary<Object^, Object^>^ target);
			static void AddText(Control^ control, String^ text);
			static int Contains(MatchCollection^ MatchList, int Index);
			static String^ Render(Control^ control);
			static Control^ _querySelector(Control^ control, String^ TagName, IList<String^>^ ClassNameList, String^ ID, IDictionary<String^, String^>^ Attr);
			static Control^ _queryRegExp(Control^ control, Regex^ regex);
		internal:
			String^ _head, ^_foot, ^_tagName;
			Dictionary<String^, String^>^ _attributes;
			static void EnsureChildControls(Control^ control);
		protected:
			void Refresh();
		public:
			property String^ tagName
			{
				String^ get();
				void set(String^ value);
			}
			property String^ className
			{
				String^ get();
				void set(String^ value);
			}
			property IList<String^>^ classList
			{
				IList<String^>^ get();
				void set(IList<String^>^ value);
			}
			property String^ id
			{
				String^ get();
				void set(String^ value);
			}
			property String^ style
			{
				String^ get();
				void set(String^ value);
			}
			property String^ title
			{
				String^ get();
				void set(String^ value);
			}
			property IDictionary<String^, String^>^ attributes
			{
				IDictionary<String^, String^>^ get();
				void set(IDictionary<String^, String^>^ value);
			}
			property String^ innerHTML
			{
				String^ get();
				void set(String^ value);
			}
			property String^ outerHTML
			{
				String^ get();
			}
			String^ getAttribute(String^ attributename);
			void setAttribute(String^ attributename, String^ attributevalue);
			void Render(HtmlTextWriter^ writer) override;
			String^ ToString() override;
			String^ GetType() new;
			static void SetInnerHTML(Control^ control, String^ html);
			static String^ GetInnerHTML(Control^ control);
			static void Reconstruction(Control^ control);
			static Html^ FindControl(Control^ control, String^ query);
			static Html^ FindControl(Control^ control, String^ query, QueryType querytype);
			static Html^ querySelector(Control^ control, String^ selectors);
			static Html^ queryRegExp(Control^ control, String^ pattern);
			static Html^ queryRegExp(Control^ control, Regex^ regex);
		};
		public ref class Img : Html
		{
		public:
			Img();
			property String^ tagName
			{
				String^ get() new;
			}
			property String^ src
			{
				String^ get();
				void set(String^ value);
			}
			property String^ alt
			{
				String^ get();
				void set(String^ value);
			}
			property String^ width
			{
				String^ get();
				void set(String^ value);
			}
			property String^ height
			{
				String^ get();
				void set(String^ value);
			}
		};
		public ref class Input : Html
		{
		public:
			Input();
			property String^ tagName
			{
				String^ get() new;
			}
			property bool disabled
			{
				bool get();
				void set(bool value);
			}
			property InputType type
			{
				InputType get();
				void set(InputType value);
			}
			property String^ name
			{
				String^ get();
				void set(String^ value);
			}
			property String^ value
			{
				String^ get();
				void set(String^ value);
			}
		};
		public ref class Script : Html
		{
		public:
			Script();
			property String^ tagName
			{
				String^ get() new;
			}
			property String^ type
			{
				String^ get();
				void set(String^ value);
			}
		};
		public ref class A : Html
		{
		public:
			A();
			property String^ tagName
			{
				String^ get() new;
			}
			property String^ href
			{
				String^ get();
				void set(String^ value);
			}
			property String^ rel
			{
				String^ get();
				void set(String^ value);
			}
			property String^ target
			{
				String^ get();
				void set(String^ value);
			}
		};
	}
}