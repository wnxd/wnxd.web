#include "load.h"
#include "config.h"
#include "Resource.h"

using namespace wnxd::Web;
using namespace wnxd::Config;
using namespace System::Text;
using namespace System::IO;
//class Load
//private
bool Load::_SaveHtml()
{
	cache^ c = gcnew cache("load", this->_cache * 3600);
	String^ ret = c->Read(this->Page->Request->Url->PathAndQuery, this->ClientID);
	if (String::IsNullOrWhiteSpace(ret))
	{
		StringBuilder^ html = gcnew StringBuilder();
		StringWriter^ sw = gcnew StringWriter(html);
		HtmlTextWriter^ htw = gcnew HtmlTextWriter(sw);
		this->Control::Render(htw);
		delete htw;
		delete sw;
		ret = html->ToString();
		c->Write(this->Page->Request->Url->PathAndQuery, this->ClientID, HttpUtility::UrlEncode(ret));
	}
	else ret = HttpUtility::UrlDecode(ret);
	if (this->_refresh)
	{
		this->Page->Response->Clear();
		this->Page->Response->Write(ret);
		this->Page->Response->End();
		return false;
	}
	return true;
}
Load^ Load::_FindFirst(Control^ control)
{
	if (!control->HasControls()) return nullptr;
	for each(Control^ col in control->Controls) if (col->GetType() == Load::typeid) return (Load^)col;
	for each(Control^ col in control->Controls)
	{
		Load^ load = _FindFirst(col);
		if (load != nullptr) return load;
	}
	return nullptr;
}
Load^ Load::_Find(Control^ control, String^ id)
{
	if (!control->HasControls()) return nullptr;
	Control^ col1 = control->FindControl(id);
	if (col1 != nullptr && col1->GetType() == Load::typeid) return (Load^)col1;
	for each(Control^ col2 in control->Controls)
	{
		Load^ load = _Find(col2, id);
		if (load != nullptr) return load;
	}
	return nullptr;
}
List<Load^>^ Load::_Find(Control^ control)
{
	List<Load^>^ list = gcnew List<Load^>();
	if (control->HasControls())
	{
		for each(Control^ col in control->Controls)
		{
			if (col->GetType() == Load::typeid) list->Add((Load^)col);
			list->AddRange(_Find(col));
		}
	}
	return list;
}
//public
int Load::Threshold::get()
{
	return this->_threshold;
}
void Load::Threshold::set(int value)
{
	this->_threshold = value;
}
int Load::Cache::get()
{
	return this->_cache;
}
void Load::Cache::set(int value)
{
	this->_cache = value;
}
Load::Load()
{
	this->_threshold = 200;
	this->_cache = 24;
}
void Load::OnInit(EventArgs^ e)
{
	String^ id = HttpContext::Current->Request["wnxd_load"];
	if (!String::IsNullOrEmpty(id))
	{
		cache^ c = gcnew cache("load", this->_cache * 3600);
		String^ html = c->Read(HttpContext::Current->Request->Url->PathAndQuery, id);
		if (!String::IsNullOrWhiteSpace(html))
		{
			HttpContext::Current->Response->Write(HttpUtility::UrlDecode(html));
			HttpContext::Current->Response->End();
		}
		this->_jump = true;
		this->_refresh = id == this->ClientID;
	}
}
void Load::Render(HtmlTextWriter^ writer)
{
	if (this->_jump && !this->_refresh) return;
	if (this->_SaveHtml())
	{
		StringBuilder^ sb = gcnew StringBuilder();
		Load^ load = FindFirst();
		if (!load->Insert) sb->AppendFormat("<script type=\"text/javascript\">{0}</script>", Resource::load->Replace("$url$", this->Page->Request->Url->PathAndQuery));
		load->Insert = true;
		String^ img_html = String::Empty;
		if (!String::IsNullOrEmpty(this->Img)) img_html = String::Format("<div style=\"position: relative; left: 50%; top: 50%; display: inline-block;\"><img src=\"{0}\" style=\"position: relative; left: -50%; top: -50%;\" /></div>", this->Img);
		sb->AppendFormat("<div id=\"{0}\" wnxd-load=\"{1}\" style=\"width: 100%; height: {1}px; background: #fff;\">{2}</div>", this->ClientID, this->_threshold, img_html);
		writer->Write(sb->ToString());
	}
}
Load^ Load::FindFirst()
{
	return _FindFirst((Control^)HttpContext::Current->CurrentHandler);
}
Load^ Load::Find(String^ id)
{
	return _Find((Control^)HttpContext::Current->CurrentHandler, id);
}
array<Load^>^ Load::Find()
{
	return _Find((Control^)HttpContext::Current->CurrentHandler)->ToArray();
}
//load_enter
//protected
void Load::load_enter::Application_BeginRequest()
{
	String^ id = this->Request["wnxd_load"];
	if (!String::IsNullOrEmpty(id))
	{
		cache^ c = gcnew cache("load", 0);
		String^ html = c->Read(HttpContext::Current->Request->Url->PathAndQuery, id);
		if (!String::IsNullOrWhiteSpace(html))
		{
			this->Response->Write(HttpUtility::UrlDecode(html));
			this->Response->End();
		}
	}
}