using namespace System;

#include "enum.h"

//class Enum<T>
//public
generic <typename T> String^ wnxd::Enum::Enum<T>::Format(Object^ value, String^ format)
{
	return System::Enum::Format(_T, value, format);
}
generic <typename T> String^ wnxd::Enum::Enum<T>::GetName(Object^ value)
{
	return System::Enum::GetName(_T, value);
}
generic <typename T> array<String^>^ wnxd::Enum::Enum<T>::GetNames()
{
	return System::Enum::GetNames(_T);
}
generic <typename T> Type^ wnxd::Enum::Enum<T>::GetUnderlyingType()
{
	return System::Enum::GetUnderlyingType(_T);
}
generic <typename T> Array^ wnxd::Enum::Enum<T>::GetValues()
{
	return System::Enum::GetValues(_T);
}
generic <typename T> bool wnxd::Enum::Enum<T>::IsDefined(Object^ value)
{
	return System::Enum::IsDefined(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::Parse(String^ value)
{
	return (T)System::Enum::Parse(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::Parse(String^ value, bool ignoreCase)
{
	return (T)System::Enum::Parse(_T, value, ignoreCase);
}
generic <typename T> bool wnxd::Enum::Enum<T>::TryParse(String^ value, T% result)
{
	try
	{
		result = Parse(value);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
generic <typename T> bool wnxd::Enum::Enum<T>::TryParse(String^ value, bool ignoreCase, T% result)
{
	try
	{
		result = Parse(value, ignoreCase);
		return true;
	}
	catch (...)
	{
		return false;
	}
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(unsigned char value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(int value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(long long value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(Object^ value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(signed char value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(short value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(unsigned int value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(unsigned long long value)
{
	return (T)System::Enum::ToObject(_T, value);
}
generic <typename T> T wnxd::Enum::Enum<T>::ToObject(unsigned short value)
{
	return (T)System::Enum::ToObject(_T, value);
}