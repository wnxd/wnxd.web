#include "load.h"
#include "common.h"
#include "Resource.h"

using namespace wnxd::Web;
using namespace System::Text;
using namespace System::IO;
using namespace System::Runtime::Caching;
//class Load
//private
bool Load::_SaveHtml()
{
	MemoryCache^ cache = MemoryCache::Default;
	String^ key = "load:" + MD5Encrypt(this->Page->Request->Url->PathAndQuery + "/" + this->ClientID);
	String^ ret = dynamic_cast<String^>(cache->Get(key, nullptr));
	if (String::IsNullOrWhiteSpace(ret))
	{
		StringBuilder^ html = gcnew StringBuilder();
		StringWriter^ sw = gcnew StringWriter(html);
		HtmlTextWriter^ htw = gcnew HtmlTextWriter(sw);
		this->Control::Render(htw);
		delete htw;
		delete sw;
		ret = html->ToString();
		CacheItemPolicy^ policy = gcnew CacheItemPolicy();
		policy->AbsoluteExpiration = DateTime::Now.AddHours(this->_cache);
		cache->Add(key, ret, policy, nullptr);
	}
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
		if (!load->Insert) sb->AppendFormat("<script type=\"text/javascript\" src=\"/wnxd.aspx?wnxd_js=load&url={0}\"></script>", HttpUtility::UrlEncode(this->Page->Request->Url->PathAndQuery));
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
//class load_enter
//protected
void Load::load_enter::Application_BeginRequest()
{
	if (this->Request->QueryString["wnxd_js"] == "load")
	{
		this->Response->Write(Resource::load->Replace("$url$", HttpUtility::UrlDecode(this->Request->QueryString["url"])));
		this->Response->End();
	}
	else
	{
		String^ id = this->Request["wnxd_load"];
		if (!String::IsNullOrEmpty(id))
		{
			MemoryCache^ cache = MemoryCache::Default;
			String^ key = "load:" + MD5Encrypt(this->Request->Url->PathAndQuery + "/" + id);
			String^ html = dynamic_cast<String^>(cache->Get(key, nullptr));
			if (!String::IsNullOrWhiteSpace(html))
			{
				this->Response->Write(html);
				this->Response->End();
			}
		}
	}
}