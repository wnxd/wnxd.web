#include "interface.h"

using namespace wnxd::Web;
using namespace wnxd::javascript;
using namespace System::Reflection;
using namespace System::Text;
using namespace System::Security::Cryptography;
using namespace System::Globalization;
//class interface_enter
//private
String^ interface_enter::EncryptString(String^ sInputString, String^ sKey)
{
	array<Byte>^ data = Encoding::UTF8->GetBytes(sInputString);
	DESCryptoServiceProvider^ DES = gcnew DESCryptoServiceProvider();
	DES->Key = ASCIIEncoding::ASCII->GetBytes(sKey);
	DES->IV = ASCIIEncoding::ASCII->GetBytes(sKey);
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
	DES->Key = ASCIIEncoding::ASCII->GetBytes(sKey);
	DES->IV = ASCIIEncoding::ASCII->GetBytes(sKey);
	ICryptoTransform^ desencrypt = DES->CreateDecryptor();
	array<Byte>^ result = desencrypt->TransformFinalBlock(data, 0, data->Length);
	return Encoding::UTF8->GetString(result);
}
//protected
void interface_enter::Initialize()
{
	List<Type^>^ l = gcnew List<Type^>();
	array<Assembly^>^ list = AppDomain::CurrentDomain->GetAssemblies();
	for (int i = 0; i < list->Length; i++)
	{
		array<Type^>^ tlist = list[i]->GetTypes();
		for (int n = 0; n < tlist->Length; n++) if (Interface::typeid->IsAssignableFrom(tlist[n]) && tlist[n] != Interface::typeid) l->Add(tlist[n]);
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
				name = this->DecryptString(HttpUtility::UrlDecode(name), "wnxdname");
				param = this->DecryptString(param, "wnxddata");
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
								this->Response->Write(this->EncryptString(r->ToString(), "wnxddata"));
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