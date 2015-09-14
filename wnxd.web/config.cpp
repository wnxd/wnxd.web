using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Linq;
using namespace System::Text;
using namespace System::Xml;

#include "config.h"

using namespace wnxd::Config;
//class config
//public
config::config(String^ path)
{
	this->_path = path;
	String^ dir = Path::GetDirectoryName(path);
	if (!Directory::Exists(dir)) Directory::CreateDirectory(dir);
	if (!File::Exists(path)) file::WriteFile(path, "<wnxd></wnxd>");
}
String^ config::Read(String^ key)
{
	XmlDocument^ dom = gcnew XmlDocument();
	dom->LoadXml(file::ReadFile(this->_path));
	XmlNode^ node = dom->SelectSingleNode("/wnxd/load[@key='" + key + "']");
	return node == nullptr ? nullptr : node->InnerText;
}
void config::Write(String^ key, String^ val)
{
	XmlDocument^ dom = gcnew XmlDocument();
	dom->LoadXml(file::ReadFile(this->_path));
	XmlNode^ node = dom->SelectSingleNode("/wnxd/load[@key='" + key + "']");
	if (node == nullptr)
	{
		XmlElement^ element = dom->CreateElement("load");
		element->SetAttribute("key", key);
		element->InnerText = val;
		dom->DocumentElement->AppendChild(element);
	}
	else node->InnerText = val;
	StringBuilder^ sb = gcnew StringBuilder();
	StringWriter^ sw = gcnew StringWriter(sb);
	dom->Save(sw);
	sw->Close();
	file::WriteFile(this->_path, sb->ToString());
}
String^ config::default::get(String^ key)
{
	return this->Read(key);
}
void config::default::set(String^ key, String^ value)
{
	this->Write(key, value);
}
//class file
//public
String^ file::ReadFile(String^ path)
{
	String^ res;
	try
	{
		if (!File::Exists(path)) return nullptr;
		FileStream^ fs = File::Open(path, FileMode::OpenOrCreate, FileAccess::Read, FileShare::Read);
		StreamReader^ sr = gcnew StreamReader(fs);
		res = sr->ReadToEnd();
		sr->Close();
		fs->Close();
	}
	catch (...)
	{
		res = ReadFile(path);
	}
	return res;
}
void file::WriteFile(String^ path, String^ contents)
{
	try
	{
		FileStream^ fs = File::Open(path, FileMode::Create, FileAccess::Write, FileShare::None);
		StreamWriter^ sw = gcnew StreamWriter(fs);
		sw->Write(contents);
		sw->Flush();
		sw->Close();
		fs->Close();
	}
	catch (...)
	{
		WriteFile(path, contents);
	}
}