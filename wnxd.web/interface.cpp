#include "interface.h"

using namespace wnxd::Web;
using namespace System::Reflection;
using namespace System::Text;
using namespace System::Web::Security;
using namespace System::Security::Cryptography;
using namespace System::Globalization;
using namespace System::Web::Configuration;
using namespace System::Net;
using namespace System::IO;
using namespace System::Text::RegularExpressions;
using namespace System::Diagnostics;
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
		array<Byte>^ data = Encoding::UTF8->GetBytes(interface_enter::EncryptString(param->ToString(), interface_enter::interface_data));
		request->ContentLength = data->Length;
		Stream^ dataStream = request->GetRequestStream();
		dataStream->Write(data, 0, data->Length);
		dataStream->Flush();
		dataStream->Close();
		WebResponse^ response = request->GetResponse();
		dataStream = response->GetResponseStream();
		StreamReader^ reader = gcnew StreamReader(dataStream);
		String^ responseData = reader->ReadToEnd();
		reader->Close();
		dataStream->Close();
		response->Close();
		if (!String::IsNullOrEmpty(responseData)) return gcnew json(interface_enter::DecryptString(responseData, interface_enter::interface_data));
	}
	catch (...)
	{

	}
	return gcnew json();
}
json^ InterfaceBase::GetCache(int time, String^ function, ...array<Object^>^ args)
{
	String^ path = AppDomain::CurrentDomain->BaseDirectory + "wnxd\\interface\\" + FormsAuthentication::HashPasswordForStoringInConfigFile(this->interface_url, "md5") + "\\" + FormsAuthentication::HashPasswordForStoringInConfigFile(function, "md5") + "\\";
	if (!Directory::Exists(path)) Directory::CreateDirectory(path);
	path += FormsAuthentication::HashPasswordForStoringInConfigFile((gcnew json(args))->ToString(), "md5") + ".tmp";
	json^ r;
	FileStream^ fs;
	if (File::Exists(path))
	{
		do
		{
			try
			{
				fs = File::Open(path, FileMode::Open, FileAccess::Read, FileShare::Read);
				break;
			}
			catch (FileNotFoundException^ ex)
			{
				goto open;
			}
			catch (...)
			{

			}
		} while (true);
		TimeSpan t = DateTime::Now - File::GetLastWriteTime(path);
		if (t.TotalSeconds > time) goto open;
		StreamReader^ sr = gcnew StreamReader(fs);
		r = gcnew json(sr->ReadToEnd());
		delete sr;
	}
	else
	{
	open:
		do
		{
			try
			{
				fs = File::Open(path, FileMode::Create, FileAccess::Write, FileShare::None);
				break;
			}
			catch (...)
			{

			}
		} while (true);
		r = this->Run(function, args);
		if (json::operator==(r, js::undefined))
		{
			delete fs;
			File::Delete(path);
		}
		else
		{
			StreamWriter^ sw = gcnew StreamWriter(fs);
			sw->Write(r->ToString());
			delete sw;
		}
	}
	delete fs;
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
	String^ name = this->_classname;
	if (!String::IsNullOrEmpty(this->_namespace)) name = this->_namespace + "." + name;
	this->interface_url = this->_domain + "wnxd.aspx?wnxd_interface=" + HttpUtility::UrlEncode(interface_enter::EncryptString(name, interface_enter::interface_name));
}
//public
InterfaceBase::InterfaceBase()
{
	if (String::IsNullOrEmpty(interface_enter::interface_name))
	{
		interface_enter::interface_name = WebConfigurationManager::AppSettings["Interface_Name_Key"];
		interface_enter::interface_data = WebConfigurationManager::AppSettings["Interface_Data_Key"];
		if (String::IsNullOrEmpty(interface_enter::interface_name)) interface_enter::interface_name = Interface_Name_Key;
		if (String::IsNullOrEmpty(interface_enter::interface_data)) interface_enter::interface_data = Interface_Data_Key;
	}
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
//internal
String^ interface_enter::EncryptString(String^ sInputString, String^ sKey)
{
	array<Byte>^ data = Encoding::UTF8->GetBytes(sInputString);
	DESCryptoServiceProvider^ DES = gcnew DESCryptoServiceProvider();
	DES->Key = ASCIIEncoding::ASCII->GetBytes(FormsAuthentication::HashPasswordForStoringInConfigFile(sKey, "md5")->Substring(0, 8));
	DES->IV = ASCIIEncoding::ASCII->GetBytes(FormsAuthentication::HashPasswordForStoringInConfigFile(sKey, "md5")->Substring(0, 8));
	ICryptoTransform^ desencrypt = DES->CreateEncryptor();
	array<Byte>^ result = desencrypt->TransformFinalBlock(data, 0, data->Length);
	return BitConverter::ToString(result);
}
String^ interface_enter::DecryptString(String^ sInputString, String^ sKey)
{
	array<String^>^ sInput = sInputString->Split('-');
	array<Byte>^ data = gcnew array<Byte>(sInput->Length);
	for (int i = 0; i < sInput->Length; i++) data[i] = Byte::Parse(sInput[i], NumberStyles::HexNumber);
	DESCryptoServiceProvider^ DES = gcnew DESCryptoServiceProvider();
	DES->Key = ASCIIEncoding::ASCII->GetBytes(FormsAuthentication::HashPasswordForStoringInConfigFile(sKey, "md5")->Substring(0, 8));
	DES->IV = ASCIIEncoding::ASCII->GetBytes(FormsAuthentication::HashPasswordForStoringInConfigFile(sKey, "md5")->Substring(0, 8));
	ICryptoTransform^ desencrypt = DES->CreateDecryptor();
	array<Byte>^ result = desencrypt->TransformFinalBlock(data, 0, data->Length);
	return Encoding::UTF8->GetString(result);
}
//protected
void interface_enter::Initialize()
{
	interface_enter::interface_name = WebConfigurationManager::AppSettings["Interface_Name_Key"];
	interface_enter::interface_data = WebConfigurationManager::AppSettings["Interface_Data_Key"];
	if (String::IsNullOrEmpty(interface_enter::interface_name)) interface_enter::interface_name = Interface_Name_Key;
	if (String::IsNullOrEmpty(interface_enter::interface_data)) interface_enter::interface_data = Interface_Data_Key;
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
					for each (MethodInfo^ method in type->GetMethods(BindingFlags::Instance | BindingFlags::Public | BindingFlags::DeclaredOnly)) dic->Add(FormsAuthentication::HashPasswordForStoringInConfigFile(method->ToString(), "md5"), method);
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
				name = DecryptString(HttpUtility::UrlDecode(name), interface_enter::interface_name);
				param = DecryptString(param, interface_enter::interface_data);
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
							for each (KeyValuePair<Type^, IDictionary<String^, MethodInfo^>^>^ item in this->ilist)
							{
								if (item->Key->FullName == name)
								{
									_ClassInfo^ d = gcnew _ClassInfo();
									d->Namespace = item->Key->Namespace;
									d->ClassName = item->Key->Name;
									IList<_MethodInfo^>^ Methods = gcnew List<_MethodInfo^>();
									for each (KeyValuePair<String^, MethodInfo^>^ item2 in item->Value)
									{
										MethodInfo^ mi = item2->Value;
										_MethodInfo^ t = gcnew _MethodInfo();
										t->MethodToken = item2->Key;
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
					this->Response->Write(EncryptString(r->ToString(), interface_enter::interface_data));
					this->Response->End();
				}
				else
				{
					if (!String::IsNullOrEmpty(fn))
					{
						for each (KeyValuePair<Type^, IDictionary<String^, MethodInfo^>^>^ item in this->ilist)
						{
							Type^ type = item->Key;
							if (type->FullName == name || type->Name == name)
							{
								Interface^ obj = (Interface^)Activator::CreateInstance(type);
								MethodInfo^ mi;
								if (item->Value->ContainsKey(fn)) mi = item->Value[fn];
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
									this->Response->Write(EncryptString(r->ToString(), interface_enter::interface_data));
									break;
								}
							}
						}
						this->Response->End();
					}
				}
			}
			catch (...)
			{
				this->Response->End();
			}
		}
	}
}