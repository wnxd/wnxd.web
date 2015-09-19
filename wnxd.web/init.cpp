#include "init.h"
#include "config.h"

using namespace wnxd::Web;
using namespace wnxd::Config;
using namespace System::Text::RegularExpressions;
using namespace System::Reflection;
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
//public
void Enter::Register()
{
	this->Register(true);
}
void Enter::Register(bool assembly)
{
	Register(this->GetType(), assembly);
}
void Enter::Register(Enter^ obj)
{
	Register(obj, true);
}
void Enter::Register(Enter^ obj, bool assembly)
{
	Register(obj->GetType(), assembly);
}
void Enter::Register(Type^ enter)
{
	Register(enter, true);
}
void Enter::Register(Type^ enter, bool assembly)
{
	if (Enter::typeid->IsAssignableFrom(enter))
	{
		config^ config = gcnew wnxd::Config::config(AppDomain::CurrentDomain->BaseDirectory + "wnxd/wnxd_config.tmp");
		String^ s = config["enter_list"], ^aqn = assembly ? enter->AssemblyQualifiedName : enter->FullName;
		if (String::IsNullOrEmpty(s)) s = aqn;
		else
		{
			if (s->Contains(aqn)) goto next;
			s += "||" + aqn;
		}
		config["enter_list"] = s;
		if (Init::_enter_list == nullptr) Init::_enter_list = gcnew List<Enter^>();
		Init::_enter_list->Add((Enter^)Activator::CreateInstance(enter));
	next:
		String^ path = AppDomain::CurrentDomain->BaseDirectory + "Global.asax";
		if (File::Exists(path))
		{
			String^ str = file::ReadFile(path);
			Match^ mc = (gcnew Regex("Inherits=\"(.*?)\""))->Match(str);
			if (mc != nullptr)
			{
				if (mc->Groups[1]->Value == "wnxd.Web.Init") return;
				config["old_Global"] = mc->Groups[1]->Value;
			}
			File::SetAttributes(path, FileAttributes::Normal);
		}
		file::WriteFile(path, "<%@ Application Inherits=\"wnxd.Web.Init\" Language=\"C#\" %>");
	}
}
void Enter::Unregister(Enter^ obj)
{
	Unregister(obj->GetType());
}
void Enter::Unregister(Type^ enter)
{
	if (Enter::typeid->IsAssignableFrom(enter))
	{
		if (Init::_enter_list != nullptr)
		{
			for (int i = 0; i < Init::_enter_list->Count; i++)
			{
				if (Init::_enter_list[i]->GetType() == enter)
				{
					Init::_enter_list->RemoveAt(i);
					config^ config = gcnew wnxd::Config::config(AppDomain::CurrentDomain->BaseDirectory + "wnxd/wnxd_config.tmp");
					config["enter_list"] = String::Join("||", Init::_enter_list);
					break;
				}
			}
		}
	}
}
//class Init
//private
void Init::_init()
{
	config^ config = gcnew wnxd::Config::config(AppDomain::CurrentDomain->BaseDirectory + "wnxd/wnxd_config.tmp");
	String^ className = config["old_Global"];
	if (!String::IsNullOrEmpty(className))
	{
		array<Assembly^>^ list = AppDomain::CurrentDomain->GetAssemblies();
		BindingFlags all = BindingFlags::Instance | BindingFlags::Static | BindingFlags::Public | BindingFlags::NonPublic;
		for (int i = 0; i < list->Length; i++)
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
				if (this->_old_app != nullptr)
				{
					this->_old_type = T;
					break;
				}
			}
		}
	}
	String^ enter_list = config["enter_list"];
	if (!String::IsNullOrEmpty(enter_list))
	{
		array<String^>^ list = enter_list->Split('|', '|');
		_enter_list = gcnew List<Enter^>();
		for (int i = 0; i < list->Length; i++)
		{
			Type^ T = Type::GetType(list[i]);
			if (T != nullptr)
			{
				try
				{
					Enter^ e = (Enter^)Activator::CreateInstance(T);
					if (e != nullptr) _enter_list->Add(e);
				}
				catch (...)
				{

				}
			}

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
	if (_enter_list != nullptr)
	{
		MethodInfo^ mi = Enter::typeid->GetMethod(method, all, nullptr, Type::EmptyTypes, nullptr);
		for (int i = 0; i < _enter_list->Count; i++) mi->Invoke(_enter_list[i], nullptr);
	}
}
void Init::Application_Start(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_Start", sender, e);
}
void Init::Application_End(Object^ sender, EventArgs^ e)
{
	this->_callback("Application_End", sender, e);
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
//protected
Init::Init()
{
	if (this->GetType()->FullName != "ASP.global_asax") throw gcnew TypeLoadException("<wnxd.web.init>是特殊类无法被继承");
	this->_init();
	this->_callback("Initialize");
}