#include "init.h"
#include "cookie.h"
#include "Resource.h"
#include "html.h"

using namespace wnxd::Web;
using namespace System::Web::UI;
//class Cookie
//public
void Cookie::Init()
{

}
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
	Type^ T = HttpWriter::typeid;
	String^ str;
	if (T->IsAssignableFrom(hw->GetType()))
	{
		BindingFlags all = BindingFlags::Instance | BindingFlags::Static | BindingFlags::Public | BindingFlags::NonPublic;
		System::Collections::ArrayList^ arr = (System::Collections::ArrayList^)T->GetField("_buffers", all)->GetValue(hw);
		if (arr->Count > 0)
		{
			MethodInfo^ mi = arr[0]->GetType()->GetMethod("GetBytes", all);
			for (int i = 0; i < arr->Count; i++) str += encoding->GetString((array<Byte>^)mi->Invoke(arr[i], nullptr));
		}
		array<wchar_t>^ charBuffer = (array<wchar_t>^)T->GetField("_charBuffer", all)->GetValue(hw);
		int charBufferLength = (int)T->GetField("_charBufferLength", all)->GetValue(hw);
		int charBufferFree = (int)T->GetField("_charBufferFree", all)->GetValue(hw);
		int len = charBufferLength - charBufferFree;
		if (len == 0) str += gcnew String(charBuffer);
		else str += gcnew String(charBuffer, 0, len);
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
			for (int i = 0; i < list->length.Value; i++)
			{
				json^ c = (json^)list[i];
				HttpCookie^ cc = gcnew HttpCookie((String^)c["Name"], (String^)c["Value"]);
				cc->Domain = (String^)c["Domain"];
				cc->Expires = (DateTime)c["Expires"];
				cc->HttpOnly = (bool)c["HttpOnly"];
				cc->Path = (String^)c["Path"];
				cc->Secure = (bool)c["Secure"];
				NameValueCollection^ nvc = (NameValueCollection^)c["Values"];
				if (nvc != nullptr) cc->Values->Add(nvc);
				this->Response->AppendCookie(cc);
			}
			this->Response->AddHeader("P3P", "CP=NON DSP COR CURa ADMa DEVa TAIa PSAa PSDa IVAa IVDa CONa HISa TELa OTPa OUR UNRa IND UNI COM NAV INT DEM CNT PRE LOC");
		}
		this->Response->End();
	}
}
void Cookie::cookie_enter::Application_PostRequestHandlerExecute()
{
	if (json::operator!=(Cookie::list, js::undefined) && this->Response->StatusCode == 200)
	{
		String^ html = this->HttpWriterRead(this->Response->Output, this->Response->ContentEncoding);
		Html^ col = gcnew Html();
		Script^ script = gcnew Script();
		script->type = "text/javascript";
		script->Controls->Add(gcnew LiteralControl(Resource::cookie->Replace("$$$", Cookie::list->ToString())));
		col->innerHTML = html;
		Html^ div = Html::FindControl(col, "head");
		if (div == nullptr)
		{
			div = Html::FindControl(col, "html");
			if (div == nullptr) div = col;
		}
		div->Controls->AddAt(0, script);
		html = col->innerHTML;
		this->Response->ClearContent();
		this->Response->Write(html);
		Cookie::list = gcnew json();
	}
}