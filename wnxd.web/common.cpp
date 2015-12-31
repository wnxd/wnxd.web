#include "common.h"

using namespace System::Text;
using namespace System::Globalization;
using namespace System::Security::Cryptography;

String^ MD5Encrypt(String^ strText)
{
	MD5^ md5 = gcnew MD5CryptoServiceProvider();
	array<Byte>^ result = md5->ComputeHash(Encoding::Default->GetBytes(strText));
	return BitConverter::ToString(result)->Replace("-", "");
}
String^ DESEncrypt(String^ sInputString, String^ sKey)
{
	array<Byte>^ data = Encoding::UTF8->GetBytes(sInputString);
	DESCryptoServiceProvider^ DES = gcnew DESCryptoServiceProvider();
	DES->Key = ASCIIEncoding::ASCII->GetBytes(MD5Encrypt(sKey)->Substring(0, 8));
	DES->IV = ASCIIEncoding::ASCII->GetBytes(MD5Encrypt(sKey)->Substring(0, 8));
	ICryptoTransform^ desencrypt = DES->CreateEncryptor();
	array<Byte>^ result = desencrypt->TransformFinalBlock(data, 0, data->Length);
	return BitConverter::ToString(result);
}
String^ DESDecrypt(String^ sInputString, String^ sKey)
{
	array<String^>^ sInput = sInputString->Split('-');
	array<Byte>^ data = gcnew array<Byte>(sInput->Length);
	for (int i = 0; i < sInput->Length; i++) data[i] = Byte::Parse(sInput[i], NumberStyles::HexNumber);
	DESCryptoServiceProvider^ DES = gcnew DESCryptoServiceProvider();
	DES->Key = ASCIIEncoding::ASCII->GetBytes(MD5Encrypt(sKey)->Substring(0, 8));
	DES->IV = ASCIIEncoding::ASCII->GetBytes(MD5Encrypt(sKey)->Substring(0, 8));
	ICryptoTransform^ desencrypt = DES->CreateDecryptor();
	array<Byte>^ result = desencrypt->TransformFinalBlock(data, 0, data->Length);
	return Encoding::UTF8->GetString(result);
}