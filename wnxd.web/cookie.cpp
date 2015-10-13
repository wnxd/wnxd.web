#include "cookie.h"
#include "Resource.h"
#include "html.h"

using namespace wnxd::Web;
using namespace wnxd::javascript;
using namespace System::Collections::Specialized;
using namespace System::Reflection;
//class Cookie
//public
void Cookie::Set(HttpCookie^ cookie)
{
	Set(cookie, String::Empty);
}
void Cookie::Set(HttpCookie^ cookie, String^ domain)
{
	HttpContext::Current->Response->AppendCookie(cookie);
	if (!String::IsNullOrEmpty(domain)) Sync(domain, cookie);
}
void Cookie::Set(HttpCookie^ cookie, IList<String^>^ domains)
{
	HttpContext::Current->Response->AppendCookie(cookie);
	Sync(domains, cookie);
}
void Cookie::Sync(String^ domain, HttpCookie^ cookie)
{
	if (domain->Substring(0, 4) != "http") domain = "http://" + domain;
	if (domain[domain->Length - 1] != '/') domain += "/";
	Object^ session = HttpContext::Current->Session["wnxd_cookie"];
	json^ list;
	if (session == nullptr)
	{
		list = gcnew json();
		HttpContext::Current->Session->Add("wnxd_cookie", list);
	}
	else list = (json^)session;
	json^ d = (json^)list[domain];
	if (json::operator==(d, js::undefined)) d = gcnew json();
	d->push(cookie);
	list[domain] = d;
}
void Cookie::Sync(IList<String^>^ domains, HttpCookie^ cookie)
{
	for (int i = 0; i < domains->Count; i++) Sync(domains[i], cookie);
}
//class cookie_enter
//private
String^ Cookie::cookie_enter::HttpWriterRead(TextWriter^ hw, Encoding^ encoding)
{
	String^ str = String::Empty;
	if (dynamic_cast<HttpWriter^>(hw))
	{
		Type^ T = HttpWriter::typeid;
		BindingFlags all = BindingFlags::Instance | BindingFlags::Static | BindingFlags::Public | BindingFlags::NonPublic;
		System::Collections::ArrayList^ arr = (System::Collections::ArrayList^)T->GetField("_buffers", all)->GetValue(hw);
		if (arr->Count > 0)
		{
			MethodInfo^ mi = T->Assembly->GetType("System.Web.IHttpResponseElement")->GetMethod("GetBytes", all);
			for (int i = 0; i < arr->Count; i++) str += encoding->GetString((array<Byte>^)mi->Invoke(arr[i], nullptr));
		}
		array<wchar_t>^ charBuffer = (array<wchar_t>^)T->GetField("_charBuffer", all)->GetValue(hw);
		int charBufferLength = (int)T->GetField("_charBufferLength", all)->GetValue(hw);
		int charBufferFree = (int)T->GetField("_charBufferFree", all)->GetValue(hw);
		int len = charBufferLength - charBufferFree;
		if (len > 0) str += gcnew String(charBuffer, 0, len);
	}
	return str;
}
//public
void Cookie::cookie_enter::Application_BeginRequest()
{
	if (this->Request->QueryString["wnxd_cookie"] == "sync")
	{
		json^ list = gcnew json(HttpUtility::UrlDecode(this->Request->QueryString["list"]));
		if (json::operator!=(list, js::undefined))
		{
			String^ Name = (String^)((json^)list["Name"])->TryConvert(String::typeid);
			String^ Value = (String^)((json^)list["Value"])->TryConvert(String::typeid);
			HttpCookie^ cc = gcnew HttpCookie(Name, Value);
			cc->Domain = (String^)((json^)list["Domain"])->TryConvert(String::typeid);
			cc->Expires = (DateTime)((json^)list["Expires"])->TryConvert(DateTime::typeid);
			cc->HttpOnly = (bool)((json^)list["HttpOnly"])->TryConvert(Boolean::typeid);
			cc->Path = (String^)((json^)list["Path"])->TryConvert(String::typeid);
			cc->Secure = (bool)((json^)list["Secure"])->TryConvert(Boolean::typeid);
			NameValueCollection^ nvc = (NameValueCollection^)((json^)list["Values"])->TryConvert(NameValueCollection::typeid);
			if (nvc != nullptr) cc->Values->Add(nvc);
			this->Response->AppendCookie(cc);
			this->Response->AddHeader("P3P", "CP=NON DSP COR CURa ADMa DEVa TAIa PSAa PSDa IVAa IVDa CONa HISa TELa OTPa OUR UNRa IND UNI COM NAV INT DEM CNT PRE LOC");
		}
		this->Response->Write("wnxd.SyncCookieCallbak();");
		this->Response->End();
	}
}
void Cookie::cookie_enter::Application_PostRequestHandlerExecute()
{
	Object^ session = HttpContext::Current->Session["wnxd_cookie"];
	if (session != nullptr)
	{
		if (this->Response->StatusCode == 200)
		{
			String^ html = this->HttpWriterRead(this->Response->Output, this->Response->ContentEncoding);
			String^ script = String::Empty;
			int count = 0;
			for each (KeyValuePair<String^, json^> kv in (json^)session) for (int i = 0; i < kv.Value->length.Value; i++, count++) script += String::Format("<script type=\"text/javascript\" wnxd_cookie=\"{0}wnxd.aspx?wnxd_cookie=sync&list={1}\"></script>", kv.Key, HttpUtility::UrlEncode(kv.Value[i]->ToString()));
			script += String::Format("<script type=\"text/javascript\">{0}</script>", Resource::cookie->Replace("$$$", count.ToString()));
			Html^ col = gcnew Html();
			col->innerHTML = html;
			Html^ div = Html::FindControl(col, "head");
			if (div == nullptr)
			{
				div = Html::FindControl(col, "html");
				if (div == nullptr) div = col;
			}
			div->Controls->AddAt(0, gcnew LiteralControl(script));
			html = col->innerHTML;
			this->Response->ClearContent();
			this->Response->Write(html);
			HttpContext::Current->Session->Remove("wnxd_cookie");
		}
	}
}