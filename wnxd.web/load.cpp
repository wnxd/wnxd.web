#include "load.h"
#include "config.h"
#include "Resource.h"

using namespace wnxd::Web;
using namespace wnxd::Config;
using namespace System::Xml;
using namespace System::Text;
using namespace System::IO;
//class Load
//private
bool Load::_SaveHtml()
{
	bool b = true;
	String^ path = this->MapPathSecure("~/wnxd/wnxd_load.tmp");
	XmlDocument^ dom = gcnew XmlDocument();
	dom->LoadXml(file::ReadFile(path));
	XmlElement^ node = (XmlElement^)dom->SelectSingleNode("/wnxd/load[@id='" + this->ClientID + "' and @url='" + this->Page->Request->Url->PathAndQuery + "']");
	if (node == nullptr)
	{
		node = dom->CreateElement("load");
		node->SetAttribute("id", this->ClientID);
		node->SetAttribute("url", this->Page->Request->Url->PathAndQuery);
		dom->DocumentElement->AppendChild(node);
	}
	else
	{
		if (node->Attributes["time"] != nullptr)
		{
			Int64 ticks;
			if (Int64::TryParse(node->Attributes["time"]->Value, ticks) && ticks > DateTime::Now.Ticks) b = false;
		}
	}
	if (b)
	{
		if (node->Attributes["time"] == nullptr) node->SetAttribute("time", DateTime::Now.AddDays(1).Ticks.ToString());
		else node->Attributes["time"]->Value = DateTime::Now.AddHours(this->_cache).Ticks.ToString();
		StringBuilder^ html = gcnew StringBuilder();
		StringWriter^ sw = gcnew StringWriter(html);
		HtmlTextWriter^ htw = gcnew HtmlTextWriter(sw);
		this->Control::Render(htw);
		delete htw;
		delete sw;
		node->InnerText = HttpUtility::UrlEncode(html->ToString());
		html = gcnew StringBuilder();
		sw = gcnew StringWriter(html);
		dom->Save(sw);
		delete sw;
		file::WriteFile(path, html->ToString());
	}
	if (this->_refresh)
	{
		this->Page->Response->Clear();
		this->Page->Response->Write(HttpUtility::UrlDecode(node->InnerText));
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
	String^ path = this->MapPathSecure("~/wnxd/");
	if (!Directory::Exists(path)) Directory::CreateDirectory(path);
	if (!File::Exists(path + "wnxd_load.tmp")) file::WriteFile(path + "wnxd_load.tmp", "<wnxd></wnxd>");
	String^ id = HttpContext::Current->Request["wnxd_load"];
	if (!String::IsNullOrEmpty(id))
	{
		XmlDocument^ dom = gcnew XmlDocument();
		dom->LoadXml(file::ReadFile(path + "wnxd_load.tmp"));
		XmlNode^ node = dom->SelectSingleNode("/wnxd/load[@id='" + id + "' and @url='" + HttpContext::Current->Request->Url->PathAndQuery + "']");
		if (node != nullptr)
		{
			Int64 ticks;
			if (Int64::TryParse(node->Attributes["time"]->Value, ticks) && ticks > DateTime::Now.Ticks)
			{
				HttpContext::Current->Response->Write(HttpUtility::UrlDecode(node->InnerText));
				HttpContext::Current->Response->End();
			}
		}
	}
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
		String^ path = this->Server->MapPath("~/wnxd/");
		if (!Directory::Exists(path)) Directory::CreateDirectory(path);
		if (!File::Exists(path + "wnxd_load.tmp")) file::WriteFile(path + "wnxd_load.tmp", "<wnxd></wnxd>");
		XmlDocument^ dom = gcnew XmlDocument();
		dom->LoadXml(file::ReadFile(path + "wnxd_load.tmp"));
		XmlNode^ node = dom->SelectSingleNode("/wnxd/load[@id='" + id + "' and @url='" + this->Request->Url->PathAndQuery + "']");
		if (node != nullptr)
		{
			Int64 ticks;
			if (Int64::TryParse(node->Attributes["time"]->Value, ticks) && ticks > DateTime::Now.Ticks)
			{
				this->Response->Write(HttpUtility::UrlDecode(node->InnerText));
				this->Response->End();
			}
		}
	}
}