#include "init.h"
#include "config.h"

using namespace wnxd::Web;
using namespace wnxd::Config;
using namespace System::Text::RegularExpressions;
using namespace System::IO;
//class Enter
//protected
void Enter::Initialize()
{

}
void Enter::Application_Start()
{

}
void Enter::Application_End()
{

}
void Enter::Application_Error()
{

}
void Enter::Application_BeginRequest()
{

}
void Enter::Application_AuthenticateRequest()
{

}
void Enter::Application_AuthorizeRequest()
{

}
void Enter::Application_ResolveRequestCache()
{

}
void Enter::Application_AcquireRequestState()
{

}
void Enter::Application_PreRequestHandlerExecute()
{

}
void Enter::Application_PostRequestHandlerExecute()
{

}
void Enter::Application_ReleaseRequestState()
{

}
void Enter::Application_UpdateRequestCache()
{

}
void Enter::Application_EndRequest()
{

}
void Enter::Application_PreSendRequestHeaders()
{

}
void Enter::Application_PreSendRequestContent()
{

}
void Enter::Session_Start()
{

}
void Enter::Session_End()
{

}
void Enter::Application_Disposed()
{

}
HttpRequest^ Enter::Request::get()
{
	return HttpContext::Current->Request;
}
HttpResponse^ Enter::Response::get()
{
	return HttpContext::Current->Response;
}
HttpServerUtility^ Enter::Server::get()
{
	return HttpContext::Current->Server;
}
HttpSessionState^ Enter::Session::get()
{
	return HttpContext::Current->Session;
}
String^ Enter::HttpHandlerPath::get()
{
	return this->_HttpHandlerPath;
}
void Enter::HttpHandlerPath::set(String^ value)
{
	this->_HttpHandlerPath = value;
}
//internal
void Enter::_Init(String^ dir)
{
	if (String::IsNullOrEmpty(dir)) return;
	Char c = dir[dir->Length - 1];
	if (c != '\\' && c != '/') dir += "\\";
	String^ path = dir + "Global.asax";
	String^ InitClassName = Init::typeid->FullName;
	if (File::Exists(path))
	{
		String^ str = file::ReadFile(path);
		Match^ mc = (gcnew Regex("Inherits=\"(.*?)\""))->Match(str);
		if (mc != nullptr)
		{
			if (mc->Groups[1]->Value == InitClassName) return;
			config^ cf = gcnew config(dir + "wnxd\\wnxd_config.tmp");
			cf["old_Global"] = mc->Groups[1]->Value;
			delete cf;
		}
		File::SetAttributes(path, FileAttributes::Normal);
	}
	file::WriteFile(path, "<%@ Application Inherits=\"" + InitClassName + "\" Language=\"C#\" %>");
}
//public
void Enter::Init()
{
	_Init(AppDomain::CurrentDomain->BaseDirectory);
}
//class Init
//private
void Init::_init()
{
	array<Assembly^>^ list = GetAllAssembly();
	config^ cf = gcnew config(AppDomain::CurrentDomain->BaseDirectory + "wnxd/wnxd_config.tmp");
	String^ className = cf["old_Global"];
	Init::_enter_list = gcnew Dictionary<Type^, Enter^>();
	Init::_handler_list = gcnew Dictionary<Type^, IHttpHandler^>();
	Init::_module_list = gcnew Dictionary<Type^, IHttpModule^>();
	for (int i = 0; i < list->Length; i++)
	{
		if (this->_old_app == nullptr && !String::IsNullOrEmpty(className))
		{
			Type^ T = list[i]->GetType(className);
			if (T != nullptr)
			{
				try
				{
					this->_old_app = Activator::CreateInstance(T);
				}
				catch (...)
				{

				}
				if (this->_old_app != nullptr) this->_old_type = T;
			}
		}
		try
		{
			array<Type^>^ tlist = list[i]->GetTypes();
			for (int n = 0; n < tlist->Length; n++)
			{
				Type^ T = tlist[n];
				if (Enter::typeid->IsAssignableFrom(T) && T != Enter::typeid)
				{
					Enter^ e = (Enter^)Activator::CreateInstance(T);
					if (e != nullptr)
					{
						Init::_enter_list->Add(T, e);
						IHttpHandler^ HttpHandler = dynamic_cast<IHttpHandler^>(e);
						if (HttpHandler != nullptr) Init::_handler_list->Add(T, HttpHandler);
						IHttpModule^ HttpModule = dynamic_cast<IHttpModule^>(e);
						if (HttpModule != nullptr) Init::_module_list->Add(T, HttpModule);
					}
				}
			}
		}
		catch (...)
		{

		}
	}
}
void Init::_callback(String^ method, ...array<Object^>^ parameters)
{
	BindingFlags all = BindingFlags::Instance | BindingFlags::Static | BindingFlags::Public | BindingFlags::NonPublic;
	if (this->_old_app != nullptr)
	{
		MethodInfo^ mi = this->_old_type->GetMethod(method, all);
		if (mi != nullptr)
		{
			Type^ T = HttpApplication::typeid;
			try
			{
				FieldInfo^ fi = T->GetField("_context", all);
				fi->SetValue(this->_old_app, this->Context);
				fi = T->GetField("_state", all);
				fi->SetValue(this->_old_app, this->Application);
			}
			catch (...)
			{

			}
			int len = mi->GetParameters()->Length;
			if (len == 0) mi->Invoke(this->_old_app, nullptr);
			else if (len == 2) mi->Invoke(this->_old_app, parameters);
		}
	}
	if (Init::_enter_list != nullptr)
	{
		MethodInfo^ mi = Enter::typeid->GetMethod(method, all, nullptr, Type::EmptyTypes, nullptr);
		for each (KeyValuePair<Type^, Enter^> kv in Init::_enter_list)
		{
			Enter^ obj = kv.Value;
			if (obj == nullptr)
			{
				obj = (Enter^)Activator::CreateInstance(kv.Key);
				if (obj == nullptr) continue;
				else Init::_enter_list[kv.Key] = obj;
			}
			mi->Invoke(obj, nullptr);
		}
	}
}
void Init::_HttpModule_Init()
{
	if (Init::_module_list != nullptr) for each (KeyValuePair<Type^, IHttpModule^> kv in Init::_module_list) if (kv.Value != nullptr) kv.Value->Init(this);
}
void Init::_HttpModule_Dispose()
{
	if (Init::_module_list != nullptr)
	{
		MethodInfo^ Dispose = IHttpModule::typeid->GetMethod("Dispose");
		for each (KeyValuePair<Type^, IHttpModule^> kv in Init::_module_list) if (kv.Value != nullptr) Dispose->Invoke(kv.Value, nullptr);
	}
}
void Init::_HttpHandler()
{
	if (Init::_handler_list != nullptr)
	{
		for each (KeyValuePair<Type^, IHttpHandler^> kv in Init::_handler_list)
		{
			Enter^ enter = dynamic_cast<Enter^>(kv.Value);
			if (enter != nullptr && kv.Value->IsReusable)
			{
				String^ HttpHandlerPath = enter->_HttpHandlerPath;
				if (!String::IsNullOrEmpty(HttpHandlerPath))
				{
					HttpHandlerPath = HttpHandlerPath->Replace(".", "\\.")->Replace("*", ".*")->Replace("?", ".?") + "$";
					Regex^ regex = gcnew Regex(HttpHandlerPath, RegexOptions::IgnoreCase);
					if (regex->IsMatch(this->Request->Url->AbsolutePath))
					{
						kv.Value->ProcessRequest(this->Context);
						this->Response->End();
					}
				}
			}
		}
	}
}
void Init::Application_Start(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_Start", sender, e);
	this->_HttpModule_Init();
}
void Init::Application_End(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_End", sender, e);
	this->_HttpModule_Dispose();
}
void Init::Application_Error(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_Error", sender, e);
}
void Init::Application_BeginRequest(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_BeginRequest", sender, e);
}
void Init::Application_AuthenticateRequest(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_AuthenticateRequest", sender, e);
}
void Init::Application_AuthorizeRequest(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_AuthorizeRequest", sender, e);
}
void Init::Application_ResolveRequestCache(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_ResolveRequestCache", sender, e);
	this->_HttpHandler();
}
void Init::Application_AcquireRequestState(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_AcquireRequestState", sender, e);
}
void Init::Application_PreRequestHandlerExecute(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_PreRequestHandlerExecute", sender, e);
}
void Init::Application_PostRequestHandlerExecute(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_PostRequestHandlerExecute", sender, e);
}
void Init::Application_ReleaseRequestState(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_ReleaseRequestState", sender, e);
}
void Init::Application_UpdateRequestCache(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_UpdateRequestCache", sender, e);
}
void Init::Application_EndRequest(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_EndRequest", sender, e);
}
void Init::Application_PreSendRequestHeaders(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_PreSendRequestHeaders", sender, e);
}
void Init::Application_PreSendRequestContent(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_PreSendRequestContent", sender, e);
}
void Init::Session_Start(Object^ sender, EventArgs^ e)
{
	this->_callback("Session_Start", sender, e);
}
void Init::Session_End(Object^ sender, EventArgs^ e)
{
	this->_callback("Session_End", sender, e);
}
void Init::Application_Disposed(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_Disposed", sender, e);
}
//internal
array<Assembly^>^ Init::GetAllAssembly()
{
	String^ dir = AppDomain::CurrentDomain->RelativeSearchPath;
	if (String::IsNullOrEmpty(dir)) dir = AppDomain::CurrentDomain->BaseDirectory;
	else dir += "\\";
	DirectoryInfo^ di = gcnew DirectoryInfo(dir);
	array<FileInfo^>^ fis = di->GetFiles("*.dll");
	List<Assembly^>^ list = gcnew List<Assembly^>();
	for (int i = 0; i < fis->Length; i++)
	{
		try
		{
			Assembly^ assembly = Assembly::LoadFrom(fis[i]->FullName);
			if (assembly != nullptr) list->Add(assembly);
		}
		catch (...)
		{

		}
	}
	return list->ToArray();
}
//protected
Init::Init()
{
	if (this->GetType()->FullName != "ASP.global_asax") throw gcnew TypeLoadException("<wnxd.Web.Init>是特殊类无法被继承");
	this->_init();
	this->_callback("Initialize");
}