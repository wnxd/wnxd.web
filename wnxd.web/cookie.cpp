#include "init.h"
#include "cookie.h"
#include "Resource.h"

using namespace wnxd::Web;
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
void Cookie::cookie_enter::Application_EndRequest()
{
	if (json::operator!=(Cookie::list, js::undefined) && this->Response->StatusCode == 200)
	{
		this->Response->Write(String::Format("<script type=\"text/javascript\">{0}</script>", Resource::cookie->Replace("$$$", Cookie::list->ToString())));
		Cookie::list = gcnew json();
	}
}