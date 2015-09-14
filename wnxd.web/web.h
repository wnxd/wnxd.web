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
			static Web^ _FindFirst(Control^ control)
			{
				if (!control->HasControls()) return nullptr;
				for each (Control^ col in control->Controls) if (col->GetType() == Web::typeid) return (Web^)col;
				for each(Control^ col in control->Controls)
				{
					Web^ web = _FindFirst(col);
					if (web != nullptr) return web;
				}
				return nullptr;
			}
			static Web^ _Find(Control^ control, String^ id)
			{
				if (!control->HasControls()) return nullptr;
				Control^ col1 = control->FindControl(id);
				if (col1 != nullptr && col1->GetType() == Web::typeid) return (Web^)col1;
				for each(Control^ col2 in control->Controls)
				{
					Web^ web = _Find(col2, id);
					if (web != nullptr) return web;
				}
				return nullptr;
			}
			static List<Web^>^ _Find(Control^ control)
			{
				List<Web^>^ list = gcnew List<Web^>();
				if (control->HasControls())
				{
					for each(Control^ col in control->Controls)
					{
						if (col->GetType() == Web::typeid) list->Add((Web^)col);
						list->AddRange(_Find(col));
					}
				}
				return list;
			}
		internal:
			bool Insert = false;
		public:
			/// <summary>
			/// 在控件所在位置加载的js
			/// </summary>
			property String^ Script
			{
				String^ get()
				{
					return this->_script;
				}
				void set(String^ value)
				{
					this->_script = value;
				}
			}
			/// <summary>
			/// 在控件所在位置加载的html代码
			/// </summary>
			property String^ Html
			{
				String^ get()
				{
					return this->_html;
				}
				void set(String^ value)
				{
					this->_html = value;
				}
			}
			/// <summary>
			/// 提交时用于辨识的标志
			/// </summary>
			property String^ Prefix
			{
				String^ get()
				{
					return this->_prefix;
				}
				void set(String^ value)
				{
					this->_prefix = value;
				}
			}
			/// <summary>
			/// 提交数据的目标url
			/// </summary>
			property String^ TargetUrl
			{
				String^ get()
				{
					if (String::IsNullOrEmpty(this->_targeturl)) this->_targeturl = this->Page->Request->Url->PathAndQuery;
					return this->_targeturl;
				}
				void set(String^ value)
				{
					if (!String::IsNullOrEmpty(value)) this->_targeturl = value;
				}
			}
			void OnLoad(EventArgs^ e) override
			{
				class Page^ page = this->Page;
				String^ text = page->Request[this->_prefix];
				if (String::IsNullOrEmpty(text)) return;
				MethodInfo^ method;
				Control^ $this = page;
				Type^ type = page->GetType();
				String^ prefixtest = this->_prefix + "_" + text;
				BindingFlags all = BindingFlags::Instance | BindingFlags::Static | BindingFlags::Public | BindingFlags::NonPublic;
				array<Type^>^ types = { System::Web::UI::Page::typeid };
				method = (method = type->GetMethod(prefixtest, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(prefixtest, all, nullptr, Type::EmptyTypes, nullptr);
				if (method == nullptr)
				{
					method = (method = type->GetMethod(text, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(text, all, nullptr, Type::EmptyTypes, nullptr);
					if (method == nullptr)
					{
						type = type->BaseType;
						method = (method = type->GetMethod(prefixtest, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(prefixtest, all, nullptr, Type::EmptyTypes, nullptr);
						if (method == nullptr) method = (method = type->GetMethod(text, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(text, all, nullptr, Type::EmptyTypes, nullptr);
					}
				}
				if (method == nullptr)
				{
					$this = this;
					do
					{
						$this = $this->Parent;
						if ($this == nullptr) break;
						type = $this->GetType();
						method = (method = type->GetMethod(prefixtest, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(prefixtest, all, nullptr, Type::EmptyTypes, nullptr);
						if (method == nullptr)
						{
							method = (method = type->GetMethod(text, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(text, all, nullptr, Type::EmptyTypes, nullptr);
							if (method == nullptr)
							{
								type = type->BaseType;
								method = (method = type->GetMethod(prefixtest, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(prefixtest, all, nullptr, Type::EmptyTypes, nullptr);
								if (method == nullptr)
								{
									method = (method = type->GetMethod(text, all, nullptr, types, nullptr)) != nullptr ? method : type->GetMethod(text, all, nullptr, Type::EmptyTypes, nullptr);
									if (method != nullptr) break;
								}
								else break;
							}
							else break;
						}
						else break;
					} while (true);
				}
				if (method != nullptr)
				{
					Object^ obj = method->IsStatic ? nullptr : $this;
					array<Object^>^ args = { page };
					if (method->GetParameters()->Length == 1) obj = method->Invoke(obj, args);
					else obj = method->Invoke(obj, nullptr);
					if (method->ReturnType == bool::typeid)
					{
						if (!(bool)obj) page->Response->End();
					}
					else if (method->ReturnType == int::typeid)
					{
						if ((int)obj == 0) page->Response->End();
					}
					else if (method->ReturnType != void::typeid)
					{
						if (obj != nullptr)
						{
							if (page->IsPostBack) this->_html += obj->ToString();
							else page->Response->Write(obj->ToString());
						}
						if (!this->Page->IsPostBack) page->Response->End();
					}
				}
			}
			void Render(HtmlTextWriter^ writer) override
			{
				if (!String::IsNullOrEmpty(this->_html)) writer->Write(this->_html);
				String^ text = this->_script;
				Web^ web = FindFirst();
				if (!web->Insert) text = Resource::web->Replace("$url$", this->TargetUrl) + text;
				this->Insert = true;
				if (!String::IsNullOrEmpty(text)) writer->Write("<script type=\"text/javascript\">{0}</script>", text);
			}
			/// <summary>
			/// 在控件所在位置添加一个js
			/// </summary>
			/// <param name="script"></param>
			void AddScript(String^ script)
			{
				this->_script += script;
			}
			/// <summary>
			/// 在控件所在位置添加一个html代码
			/// </summary>
			/// <param name="html"></param>
			void AddHtml(String^ html)
			{
				this->_html += html;
			}
			/// <summary>
			/// 从当前页面对象中寻找第一个web对象
			/// </summary>
			/// <returns></returns>
			static Web^ FindFirst()
			{
				return _FindFirst((Control^)HttpContext::Current->CurrentHandler);
			}
			/// <summary>
			/// 从当前页面对象中按指定id寻找web对象
			/// </summary>
			/// <param name="id"></param>
			/// <returns></returns>
			static Web^ Find(String^ id)
			{
				return _Find((Control^)HttpContext::Current->CurrentHandler, id);
			}
			/// <summary>
			/// 从当前页面对象中寻找所有的web对象
			/// </summary>
			/// <returns></returns>
			static array<Web^>^ Find()
			{
				return _Find((Control^)HttpContext::Current->CurrentHandler)->ToArray();
			}
		};
	}
}