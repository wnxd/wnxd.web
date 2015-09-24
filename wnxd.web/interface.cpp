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
//class InterfaceBase
//protected
String^ InterfaceBase::Domain::get()
{
	return this->_domain;
}
void InterfaceBase::Domain::set(String^ value)
{
	this->_domain = value;
	InterfaceBase();
}
String^ InterfaceBase::Namespace::get()
{
	return this->_namespace;
}
void InterfaceBase::Namespace::set(String^ value)
{
	this->_namespace = value;
	InterfaceBase();
}
String^ InterfaceBase::ClassName::get()
{
	return this->_classname;
}
void InterfaceBase::ClassName::set(String^ value)
{
	this->_classname = value;
	InterfaceBase();
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
//public
InterfaceBase::InterfaceBase()
{
	if (String::IsNullOrEmpty(this->_domain))
	{
		String^ domain = WebConfigurationManager::AppSettings["wnxd_interface_domain"];
		if (String::IsNullOrEmpty(domain)) domain = HttpContext::Current->Request->Url->Authority;
		this->_domain = domain;
	}
	if (this->_domain->Substring(0, 4) != "http") this->_domain = "http://" + this->_domain;
	if (this->_domain[this->_domain->Length - 1] != '/') this->_domain += "/";
	if (String::IsNullOrEmpty(this->_classname)) this->_classname = this->GetType()->Name;
	String^ name = this->_classname;
	if (!String::IsNullOrEmpty(this->_namespace)) name = this->_namespace + "." + name;
	this->interface_url = this->_domain + "wnxd.aspx?wnxd_interface=" + HttpUtility::UrlEncode(interface_enter::EncryptString(name, "wnxd: interface_name"));
}
//class interface_enter
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
				}
			}
			catch (...)
			{

			}
		}
		this->Response->End();
	}
}