using namespace System;
using namespace System::Net;

String^ MD5Encrypt(String^ strText);
String^ DESEncrypt(String^ sInputString, String^ sKey);
String^ DESDecrypt(String^ sInputString, String^ sKey);
IPAddress^ Host2IP(String^ host);
bool IsSelfIP(IPAddress^ ip);