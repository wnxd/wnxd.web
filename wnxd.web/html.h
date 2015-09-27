#include "enum.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text::RegularExpressions;
using namespace System::Web::UI;
using namespace wnxd::Enum;

namespace wnxd
{
	namespace Web
	{
		public ref class Html : Control
		{
		private:
			ref class List : System::Collections::Generic::List < String^ >
			{
			internal:
				event Action<IList<String^>^>^ Changed;
			public:
				virtual void Add(String^ item) new = IList<String^>::Add;
				virtual void Insert(int index, String^ item) new = IList<String^>::Insert;
				virtual bool Remove(String^ item) new = IList<String^>::Remove;
				virtual void RemoveAt(int index) new = IList<String^>::RemoveAt;
				virtual void Clear() new = IList<String^>::Clear;
				property String^ default[int]
				{
					virtual void set(int index, String^ value) new = IList<String^>::default::set;
				}

			};
			ref class Dictionary : System::Collections::Generic::Dictionary < String^, String^ >
			{
			internal:
				event Action^ Changed;
			public:
				Dictionary(IDictionary<String^, String^>^ dictionary);
				Dictionary(IEqualityComparer<String^>^ comparer);
				virtual void Add(String^ key, String^ value) new = IDictionary<String^, String^>::Add;
				virtual bool Remove(String^ key) new = IDictionary<String^, String^>::Remove;
				virtual void Clear() new = IDictionary<String^, String^>::Clear;
				property String^ default[String^]
				{
					virtual void set(String^ key, String^ value) new = IDictionary<String^, String^>::default::set;
				}
			};
			static void AddText(Control^ control, String^ text);
			static int Contains(MatchCollection^ MatchList, int Index);
			static String^ Render(Control^ control);
			static Control^ _querySelector(Control^ control, String^ TagName, IList<String^>^ ClassNameList, String^ ID, IDictionary<String^, String^>^ Attr);
			static Control^ _queryRegExp(Control^ control, Regex^ regex);
		internal:
			String^ _head, ^_foot, ^_tagName;
			Dictionary^ _attributes;
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
			/// <summary>
			/// ���õ�ǰ�ؼ���html����
			/// </summary>
			/// <param name="control"></param>
			/// <param name="html"></param>
			static void SetInnerHTML(Control^ control, String^ html);
			/// <summary>
			/// ��ȡ��ǰ�ؼ���html����
			/// </summary>
			/// <param name="control"></param>
			/// <returns></returns>
			static String^ GetInnerHTML(Control^ control);
			/// <summary>
			/// �ѵ�ǰ�ؼ����ӿؼ���html��ǩ����ʽ�ع�
			/// </summary>
			/// <param name="control"></param>
			static void Reconstruction(Control^ control);
			/// <summary>
			/// �ڵ�ǰ�ؼ��а�CSSѡ���﷨����ѯ�ӿؼ�,�÷����ᴥ��Reconstruction�����ع�
			/// </summary>
			/// <param name="control"></param>
			/// <param name="query">��ѯ���</param>
			/// <returns></returns>
			static Html^ FindControl(Control^ control, String^ query);
			/// <summary>
			/// �ڵ�ǰ�ؼ��а�ָ���Ĳ�ѯ��ʽ��ѯ�ӿؼ�,�÷����ᴥ��Reconstruction�����ع�
			/// </summary>
			/// <param name="control"></param>
			/// <param name="query">��ѯ���</param>
			/// <param name="querytype">��ѯ��ʽ</param>
			/// <returns></returns>
			static Html^ FindControl(Control^ control, String^ query, QueryType querytype);
			/// <summary>
			/// �ڵ�ǰ�ؼ��а�CSSѡ���﷨����ѯ�ӿؼ�,�÷����ᴥ��Reconstruction�����ع�
			/// </summary>
			/// <param name="control"></param>
			/// <param name="selectors">��ѯ���</param>
			/// <returns></returns>
			static Html^ querySelector(Control^ control, String^ selectors);
			/// <summary>
			/// �ڵ�ǰ�ؼ�����������ʽ����ѯ�ӿؼ�,�÷����ᴥ��Reconstruction�����ع�
			/// </summary>
			/// <param name="control"></param>
			/// <param name="pattern">������ʽ���</param>
			/// <returns></returns>
			static Html^ queryRegExp(Control^ control, String^ pattern);
			/// <summary>
			/// �ڵ�ǰ�ؼ�����������ʽ����ѯ�ӿؼ�,�÷����ᴥ��Reconstruction�����ع�
			/// </summary>
			/// <param name="control"></param>
			/// <param name="regex">������ʽ</param>
			/// <returns></returns>
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
			property String^ src
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