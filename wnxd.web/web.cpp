#include "web.h"
#include "Resource.h"

using namespace System::Reflection;
using namespace System::Web;
//private
wnxd::Web::Web^ wnxd::Web::Web::_FindFirst(Control^ control)
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
wnxd::Web::Web^ wnxd::Web::Web::_Find(Control^ control, String^ id)
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
List<wnxd::Web::Web^>^ wnxd::Web::Web::_Find(Control^ control)
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
//public
String^ wnxd::Web::Web::Script::get()
{
	return this->_script;
}
void wnxd::Web::Web::Script::set(String^ value)
{
	this->_script = value;
}
String^ wnxd::Web::Web::Html::get()
{
	return this->_html;
}
void wnxd::Web::Web::Html::set(String^ value)
{
	this->_html = value;
}
String^ wnxd::Web::Web::Prefix::get()
{
	return this->_prefix;
}
void wnxd::Web::Web::Prefix::set(String^ value)
{
	this->_prefix = value;
}
String^ wnxd::Web::Web::TargetUrl::get()
{
	if (String::IsNullOrEmpty(this->_targeturl)) this->_targeturl = this->Page->Request->Url->PathAndQuery;
	return this->_targeturl;
}
void wnxd::Web::Web::TargetUrl::set(String^ value)
{
	if (!String::IsNullOrEmpty(value)) this->_targeturl = value;
}
wnxd::Web::Web::Web()
{
	this->_prefix = "wnxd_ajax";
}
void wnxd::Web::Web::OnLoad(EventArgs^ e)
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
void wnxd::Web::Web::Render(HtmlTextWriter^ writer)
{
	if (!String::IsNullOrEmpty(this->_html)) writer->Write(this->_html);
	String^ text = this->_script;
	Web^ web = FindFirst();
	if (!web->Insert) writer->Write(String::Format("<script type=\"text/javascript\" src=\"/wnxd.aspx?wnxd_js=web&url={0}\"></script>", HttpUtility::UrlEncode(this->TargetUrl)));
	this->Insert = true;
	if (!String::IsNullOrEmpty(text)) writer->Write("<script type=\"text/javascript\">{0}</script>", text);
}
void wnxd::Web::Web::AddScript(String^ script)
{
	this->_script += script;
}
void wnxd::Web::Web::AddHtml(String^ html)
{
	this->_html += html;
}
wnxd::Web::Web^ wnxd::Web::Web::FindFirst()
{
	return _FindFirst((Control^)HttpContext::Current->CurrentHandler);
}
wnxd::Web::Web^ wnxd::Web::Web::Find(String^ id)
{
	return _Find((Control^)HttpContext::Current->CurrentHandler, id);
}
array<wnxd::Web::Web^>^ wnxd::Web::Web::Find()
{
	return _Find((Control^)HttpContext::Current->CurrentHandler)->ToArray();
}
//class web_enter
//protected
void wnxd::Web::Web::web_enter::Application_BeginRequest()
{
	if (this->Request->QueryString["wnxd_js"] == "web")
	{
		this->Response->Write(Resource::web->Replace("$url$", HttpUtility::UrlDecode(this->Request->QueryString["url"])));
		this->Response->End();
	}
}