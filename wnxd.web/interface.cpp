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
		json^ param = gcnew json();
		param["Name"] = function;
		param["Param"] = args;
		WebRequest^ request = WebRequest::Create(this->interface_url);
		request->Method = "POST";
		request->ContentType = "application/x-www-form-urlencoded";
		array<Byte>^ data = Encoding::UTF8->GetBytes(interface_enter::EncryptString(param->ToString(), "wnxd: interface_data"));
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
		if (!String::IsNullOrEmpty(responseData)) return gcnew json(interface_enter::DecryptString(responseData, "wnxd: interface_data"));
	}
	catch (...)
	{

	}
	return gcnew json();
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
	this->interface_url = this->_domain + "wnxd.aspx?wnxd_interface=" + HttpUtility::UrlEncode(interface_enter::EncryptString(name, "wnxd: interface_name"));
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
	List<Type^>^ l = gcnew List<Type^>();
	array<Assembly^>^ list = Init::GetAllAssembly();
	for (int i = 0; i < list->Length; i++)
	{
		try
		{
			array<Type^>^ tlist = list[i]->GetTypes();
			for (int n = 0; n < tlist->Length; n++) if (Interface::typeid->IsAssignableFrom(tlist[n]) && tlist[n] != Interface::typeid) l->Add(tlist[n]);
		}
		catch (...)
		{

		}
	}
	this->ilist = l->ToArray();
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
				name = DecryptString(HttpUtility::UrlDecode(name), "wnxd: interface_name");
				param = DecryptString(param, "wnxd: interface_data");
				json^ info = gcnew json(param);
				String^ fn = (String^)((json^)info["Name"])->TryConvert(String::typeid);
				if (name == "$query$")
				{
					json^ r = gcnew json();
					if (fn == "interface_name") for (int i = 0; i < this->ilist->Length; i++) r->push(this->ilist[i]->FullName);
					else if (fn == "interface_info")
					{
						Array^ EmptyArray = gcnew array<json^>(0);
						json^ list = (json^)info["List"];
						for (int i = 0; i < list->length.Value; i++)
						{
							name = (String^)((json^)list[i])->TryConvert(String::typeid);
							for (int n = 0; n < this->ilist->Length; n++)
							{
								if (this->ilist[n]->FullName == name)
								{
									json^ d = gcnew json();
									d["Namespace"] = this->ilist[n]->Namespace;
									d["ClassName"] = this->ilist[n]->Name;
									json^ Methods = gcnew json(EmptyArray);
									array<MethodInfo^>^ mis = this->ilist[n]->GetMethods(BindingFlags::Instance | BindingFlags::Public | BindingFlags::DeclaredOnly);
									for (int x = 0; x < mis->Length; x++)
									{
										MethodInfo^ mi = mis[x];
										json^ t = gcnew json();
										t["MethodName"] = mi->Name;
										Type^ T = mi->ReturnType;
										t["ReturnType"] = T->IsGenericType ? this->GetGenericName(T) : T->FullName;
										json^ Parameters = gcnew json(EmptyArray);
										array<ParameterInfo^>^ pis = mi->GetParameters();
										for (int y = 0; y < pis->Length; y++)
										{
											ParameterInfo^ pi = pis[y];
											json^ tt = gcnew json();
											tt["ParameterName"] = pi->Name;
											if (pi->IsRetval) tt["Type"] = 2;
											else if (pi->IsOut) tt["Type"] = 1;
											else tt["Type"] = 0;
											tt["IsOptional"] = pi->IsOptional;
											if (pi->IsOptional) tt["DefaultValue"] = pi->DefaultValue;
											T = pi->ParameterType;
											tt["ParameterType"] = T->IsGenericType ? this->GetGenericName(T) : T->FullName;
											Parameters->push(tt);
										}
										t["Parameters"] = Parameters;
										array<Interface::MethodAttribute^>^ Attributes = (array<Interface::MethodAttribute^>^)mi->GetCustomAttributes(Interface::MethodAttribute::typeid, true);
										if (Attributes->Length>0) t["summary"] = Attributes[0]->summary;
										Methods->push(t);
									}
									d["Methods"] = Methods;
									r->push(d);
								}
							}
						}
					}
					this->Response->Write(EncryptString(r->ToString(), "wnxd: interface_data"));
					this->Response->End();
				}
				else
				{
					if (!String::IsNullOrEmpty(fn))
					{
						for (int i = 0; this->ilist->Length; i++)
						{
							Type^ T = this->ilist[i];
							if (T->FullName == name || T->Name == name)
							{
								Interface^ obj = (Interface^)Activator::CreateInstance(T);
								MethodInfo^ mi = T->GetMethod(fn);
								if (mi != nullptr)
								{
									json^ fp = (json^)info["Param"];
									array<ParameterInfo^>^ pis = mi->GetParameters();
									List<Object^>^ args = gcnew List<Object^>();
									for (int n = 0; n < pis->Length; n++)
									{
										Object^ o = ((json^)fp[n])->TryConvert(pis[n]->ParameterType);
										args->Add(o);
									}
									json^ r = gcnew json(mi->Invoke(obj, args->ToArray()));
									this->Response->Write(EncryptString(r->ToString(), "wnxd: interface_data"));
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

			}
		}
	}
}