#include "interface.h"
#include "config.h"
#include "common.h"

using namespace wnxd::Web;
using namespace wnxd::Config;
using namespace System::Reflection;
using namespace System::Text;
using namespace System::Web::Configuration;
using namespace System::IO;
using namespace System::Text::RegularExpressions;
using namespace System::Runtime::Caching;
using namespace System::Net;
//class MethodAttribute
//public
String^ Interface::MethodAttribute::summary::get()
{
	return this->_summary;
}
void Interface::MethodAttribute::summary::set(String^ value)
{
	this->_summary = value;
}
//class MethodCacheAttribute
//public
int Interface::MethodCacheAttribute::time::get()
{
	return this->_time;
}
void Interface::MethodCacheAttribute::time::set(int value)
{
	this->_time = value;
}
//class InterfaceBase
//protected
String^ InterfaceBase::Domain::get()
{
	return this->_domain;
}
void InterfaceBase::Domain::set(String^ value)
{
	this->_domain = value;
	this->init();
}
String^ InterfaceBase::Namespace::get()
{
	return this->_namespace;
}
void InterfaceBase::Namespace::set(String^ value)
{
	this->_namespace = value;
	this->init();
}
String^ InterfaceBase::ClassName::get()
{
	return this->_classname;
}
void InterfaceBase::ClassName::set(String^ value)
{
	this->_classname = value;
	this->init();
}
json^ InterfaceBase::Run(String^ function, ...array<Object^>^ args)
{
	try
	{
		_CallInfo^ CallInfo = gcnew _CallInfo();
		CallInfo->Name = function;
		CallInfo->Param = gcnew json(args);
		json^ param = gcnew json(CallInfo);
		WebRequest^ request = WebRequest::Create(this->interface_url);
		request->Method = "POST";
		request->ContentType = "application/x-www-form-urlencoded";
		array<Byte>^ data = Encoding::UTF8->GetBytes(DESEncrypt(param->ToString(), interface_enter::interface_data));
		request->ContentLength = data->Length;
		Stream^ dataStream = request->GetRequestStream();
		dataStream->Write(data, 0, data->Length);
		dataStream->Flush();
		delete dataStream;
		WebResponse^ response = request->GetResponse();
		dataStream = response->GetResponseStream();
		StreamReader^ reader = gcnew StreamReader(dataStream);
		String^ responseData = reader->ReadToEnd();
		delete reader;
		delete dataStream;
		delete response;
		if (!String::IsNullOrEmpty(responseData)) return gcnew json(DESDecrypt(responseData, interface_enter::interface_data));
	}
	catch (...)
	{

	}
	return gcnew json();
}
json^ InterfaceBase::GetCache(int time, String^ function, ...array<Object^>^ args)
{
	MemoryCache^ cache = MemoryCache::Default;
	String^ key = "interface:" + MD5Encrypt(this->_domain + "-" + this->_fullname + function + (gcnew json(args))->ToString());
	json^ r = dynamic_cast<json^>(cache->Get(key, nullptr));
	if (r == nullptr || json::operator==(r, js::undefined))
	{
		r = this->Run(function, args);
		if (json::operator!=(r, js::undefined))
		{
			CacheItemPolicy^ policy = gcnew CacheItemPolicy();
			policy->AbsoluteExpiration = DateTime::Now.AddSeconds(time);
			cache->Add(key, r, policy, nullptr);
		}
	}
	return r;
}
void InterfaceBase::init()
{
	if (String::IsNullOrEmpty(this->_domain))
	{
		try
		{
			String^ domain = WebConfigurationManager::AppSettings["wnxd_interface_domain"];
			if (String::IsNullOrEmpty(domain)) domain = HttpContext::Current->Request->Url->Authority;
			this->_domain = domain;
		}
		catch (...)
		{
			this->_domain = "127.0.0.1";
		}
	}
	if (this->_domain->Substring(0, 4) != "http") this->_domain = "http://" + this->_domain;
	if (this->_domain[this->_domain->Length - 1] != '/') this->_domain += "/";
	if (String::IsNullOrEmpty(this->_classname)) this->_classname = this->GetType()->Name;
	this->_fullname = this->_classname;
	if (!String::IsNullOrEmpty(this->_namespace)) this->_fullname = this->_namespace + "." + this->_fullname;
	this->interface_url = this->_domain + "wnxd.aspx?wnxd_interface=" + HttpUtility::UrlEncode(DESEncrypt(this->_fullname, interface_enter::interface_name));
}
//public
InterfaceBase::InterfaceBase()
{
	init();
}
//class interface_enter
//private
String^ interface_enter::GetGenericName(Type^ gt)
{
	String^ name = gt->FullName;
	array<Type^>^ types = gt->GetGenericArguments();
	Regex^ regex = gcnew Regex("^(.+?)`" + types->Length + "\\[.+\\]$");
	Match^ mc = regex->Match(name);
	if (mc->Success)
	{
		name = mc->Groups[1]->Value + "<";
		for (int i = 0; i < types->Length; i++)
		{
			if (i > 0) name += ", ";
			Type^ T = types[i];
			name += T->IsGenericType ? this->GetGenericName(T) : T->FullName;
		}
		name += ">";
	}
	return name;
}
//protected
void interface_enter::Initialize()
{
	IDictionary<Type^, IDictionary<String^, MethodInfo^>^>^ l = gcnew Dictionary<Type^, IDictionary<String^, MethodInfo^>^>();
	array<Assembly^>^ list = Init::GetAllAssembly();
	for each (Assembly^ assembly in list)
	{
		try
		{
			array<Type^>^ tlist = assembly->GetTypes();
			for each (Type^ type in tlist)
			{
				if (Interface::typeid->IsAssignableFrom(type) && type != Interface::typeid)
				{
					IDictionary<String^, MethodInfo^>^ dic = gcnew Dictionary<String^, MethodInfo^>();
					for each (MethodInfo^ method in type->GetMethods(BindingFlags::Instance | BindingFlags::Public | BindingFlags::DeclaredOnly)) dic->Add(MD5Encrypt(method->ToString()), method);
					l->Add(type, dic);
				}
			}
		}
		catch (...)
		{

		}
	}
	this->ilist = l;
}
void interface_enter::Application_Start()
{
	interface_enter::interface_name = WebConfigurationManager::AppSettings["Interface_Name_Key"];
	interface_enter::interface_data = WebConfigurationManager::AppSettings["Interface_Data_Key"];
	if (String::IsNullOrEmpty(interface_enter::interface_name)) interface_enter::interface_name = Interface_Name_Key;
	if (String::IsNullOrEmpty(interface_enter::interface_data)) interface_enter::interface_data = Interface_Data_Key;
}
void interface_enter::Application_BeginRequest()
{
	String^ name = this->Request->QueryString["wnxd_interface"];
	if (!String::IsNullOrEmpty(name))
	{
		String^ param = this->Request->Form->ToString();
		if (!String::IsNullOrEmpty(param))
		{
			try
			{
				name = DESDecrypt(HttpUtility::UrlDecode(name), interface_enter::interface_name);
				param = DESDecrypt(param, interface_enter::interface_data);
				json^ info = gcnew json(param);
				_CallInfo^ CallInfo = (_CallInfo^)info->TryConvert(_CallInfo::typeid);
				String^ fn = CallInfo->Name;
				if (name == "$query$")
				{
					json^ r = gcnew json();
					if (fn == "interface_name") for each (Type^ type in this->ilist->Keys) r->push(type->FullName);
					else if (fn == "interface_info")
					{
						Array^ EmptyArray = gcnew array<json^>(0);
						json^ list = (json^)info["List"];
						for (int i = 0; i < list->length.Value; i++)
						{
							name = (String^)((json^)list[i])->TryConvert(String::typeid);
							for each (KeyValuePair<Type^, IDictionary<String^, MethodInfo^>^> item in this->ilist)
							{
								if (item.Key->FullName == name)
								{
									_ClassInfo^ d = gcnew _ClassInfo();
									d->Namespace = item.Key->Namespace;
									d->ClassName = item.Key->Name;
									IList<_MethodInfo^>^ Methods = gcnew List<_MethodInfo^>();
									for each (KeyValuePair<String^, MethodInfo^> item2 in item.Value)
									{
										MethodInfo^ mi = item2.Value;
										_MethodInfo^ t = gcnew _MethodInfo();
										t->MethodToken = item2.Key;
										t->MethodName = mi->Name;
										Type^ T = mi->ReturnType;
										t->ReturnType = T->IsGenericType ? this->GetGenericName(T) : T->FullName;
										IList<_ParameterInfo^>^ Parameters = gcnew List<_ParameterInfo^>();
										array<ParameterInfo^>^ pis = mi->GetParameters();
										for (int y = 0; y < pis->Length; y++)
										{
											ParameterInfo^ pi = pis[y];
											_ParameterInfo^ tt = gcnew _ParameterInfo();
											tt->ParameterName = pi->Name;
											T = pi->ParameterType;
											if (pi->IsOut) tt->Type = _ParameterType::Out;
											else if (pi->IsRetval || T->IsByRef) tt->Type = _ParameterType::Retval;
											else tt->Type = _ParameterType::In;
											tt->IsOptional = pi->IsOptional;
											if (pi->IsOptional) tt->DefaultValue = pi->DefaultValue;
											if (T->IsByRef) T = T->GetElementType();
											tt->ParameterType = T->IsGenericType ? this->GetGenericName(T) : T->FullName;
											Parameters->Add(tt);
										}
										t->Parameters = Parameters;
										array<Interface::MethodAttribute^>^ Summary = (array<Interface::MethodAttribute^>^)mi->GetCustomAttributes(Interface::MethodAttribute::typeid, true);
										if (Summary->Length > 0) t->Summary = Summary[0]->summary;
										array<Interface::MethodCacheAttribute^>^ Time = (array<Interface::MethodCacheAttribute^>^)mi->GetCustomAttributes(Interface::MethodCacheAttribute::typeid, true);
										if (Time->Length > 0) t->CacheTime = Time[0]->time;
										Methods->Add(t);
									}
									d->Methods = Methods;
									r->push(d);
									break;
								}
							}
						}
					}
					this->Response->Write(DESEncrypt(r->ToString(), interface_enter::interface_data));
				}
				else
				{
					if (!String::IsNullOrEmpty(fn))
					{
						for each (KeyValuePair<Type^, IDictionary<String^, MethodInfo^>^> item in this->ilist)
						{
							Type^ type = item.Key;
							if (type->FullName == name || type->Name == name)
							{
								Interface^ obj = (Interface^)Activator::CreateInstance(type);
								MethodInfo^ mi;
								if (item.Value->ContainsKey(fn)) mi = item.Value[fn];
								else mi = type->GetMethod(fn);
								if (mi != nullptr)
								{
									json^ fp = CallInfo->Param;
									array<ParameterInfo^>^ pis = mi->GetParameters();
									array<Object^>^ args = gcnew array<Object^>(pis->Length);
									IList<int>^ outparams = gcnew List<int>();
									for (int n = 0; n < pis->Length; n++)
									{
										ParameterInfo^ pi = pis[n];
										Object^ o = ((json^)fp[n])->TryConvert(pi->ParameterType);
										args[n] = o;
										if (pi->IsOut || pi->IsRetval || pi->ParameterType->IsByRef) outparams->Add(n);
									}
									json^ r = gcnew json(mi->Invoke(obj, args));
									if (outparams->Count > 0)
									{
										_OutData^ out = gcnew _OutData();
										out->OutParams = gcnew List<Object^>();
										for each (int n in outparams) out->OutParams->Add(args[n]);
										out->Data = r;
										r = gcnew json(out);
									}
									this->Response->Write(DESEncrypt(r->ToString(), interface_enter::interface_data));
									break;
								}
							}
						}
					}
				}
				this->Response->End();
			}
			catch (...)
			{
				this->Response->End();
			}
		}
	}
}