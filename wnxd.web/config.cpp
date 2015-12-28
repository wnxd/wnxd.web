#include "config.h"

using namespace wnxd::Config;
using namespace System::IO;
using namespace System::Text;
using namespace System::Web::Security;
//class config
//public
config::config(String^ path)
{
	this->_path = path;
	String^ dir = Path::GetDirectoryName(path);
	if (!Directory::Exists(dir)) Directory::CreateDirectory(dir);
	if (!File::Exists(path)) file::WriteFile(path, "<wnxd></wnxd>");
	this->_dom = gcnew XmlDocument();
	this->_dom->LoadXml(file::ReadFile(this->_path));
}
config::~config()
{
	StringBuilder^ sb = gcnew StringBuilder();
	StringWriter^ sw = gcnew StringWriter(sb);
	this->_dom->Save(sw);
	delete sw;
	file::WriteFile(this->_path, sb->ToString());
}
String^ config::Read(String^ key)
{
	XmlNode^ node = this->_dom->SelectSingleNode("/wnxd/" + key);
	return node == nullptr ? nullptr : node->InnerText;
}
void config::Write(String^ key, String^ val)
{
	XmlNode^ node = this->_dom->SelectSingleNode("/wnxd/" + key);
	if (node == nullptr)
	{
		node = this->_dom->CreateElement(key);
		node->InnerText = val;
		this->_dom->DocumentElement->AppendChild(node);
	}
	else node->InnerText = val;
}
String^ config::GetAttr(String^ key, String^ name)
{
	XmlNode^ node = this->_dom->SelectSingleNode("/wnxd/" + key);
	if (node != nullptr)
	{
		XmlAttribute^ attr = node->Attributes[name];
		return attr == nullptr ? nullptr : attr->Value;
	}
	return nullptr;
}
void config::SetAttr(String^ key, String^ name, String^ val)
{
	XmlNode^ node = this->_dom->SelectSingleNode("/wnxd/" + key);
	if (node == nullptr)
	{
		node = this->_dom->CreateElement(key);
		this->_dom->DocumentElement->AppendChild(node);
	}
	XmlAttribute^ attr = node->Attributes[name];
	if (attr == nullptr)
	{
		attr = this->_dom->CreateAttribute(name);
		node->Attributes->Append(attr);
	}
	attr->Value = val;
}
String^ config::default::get(String^ key)
{
	return this->Read(key);
}
void config::default::set(String^ key, String^ value)
{
	this->Write(key, value);
}
//class cache
//public
cache::cache(String^ name, double time)
{
	this->_path = AppDomain::CurrentDomain->BaseDirectory + "\\wnxd\\" + name + "\\";
	this->_time = time;
}
String^ cache::Read(String^ key)
{
	String^ path = this->_path + FormsAuthentication::HashPasswordForStoringInConfigFile(key, "md5");
	if (File::Exists(path))
	{
		config^ c = gcnew config(path);
		TimeSpan t = DateTime::Now - File::GetLastWriteTime(path);
		if (this->_time == 0)
		{
			String^ v = c->GetAttr("cache", "time");
			if (v == nullptr || !double::TryParse(v, this->_time)) return nullptr;
		}
		if (t.TotalSeconds <= this->_time) return c->Read("cache");
	}
	return nullptr;
}
String^ cache::Read(String^ item, String^ key)
{
	String^ path = this->_path + FormsAuthentication::HashPasswordForStoringInConfigFile(item, "md5") + "\\" + FormsAuthentication::HashPasswordForStoringInConfigFile(key, "md5");
	if (File::Exists(path))
	{
		config^ c = gcnew config(path);
		TimeSpan t = DateTime::Now - File::GetLastWriteTime(path);
		if (this->_time == 0)
		{
			String^ v = c->GetAttr("cache", "time");
			if (v == nullptr || !double::TryParse(v, this->_time)) return nullptr;
		}
		if (t.TotalSeconds <= this->_time) return c->Read("cache");
	}
	return nullptr;
}
void cache::Write(String^ key, String^ val)
{
	String^ path = this->_path;
	if (!Directory::Exists(path)) Directory::CreateDirectory(path);
	path += FormsAuthentication::HashPasswordForStoringInConfigFile(key, "md5");
	config^ c = gcnew config(path);
	c->Write("cache", val);
	c->SetAttr("cache", "time", this->_time.ToString());
	delete c;
}
void cache::Write(String^ item, String^ key, String^ val)
{
	String^ path = this->_path + FormsAuthentication::HashPasswordForStoringInConfigFile(item, "md5") + "\\";
	if (!Directory::Exists(path)) Directory::CreateDirectory(path);
	path += FormsAuthentication::HashPasswordForStoringInConfigFile(key, "md5");
	config^ c = gcnew config(path);
	c->Write("cache", val);
	c->SetAttr("cache", "time", this->_time.ToString());
	delete c;
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
		delete sr;
		delete fs;
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
		delete sw;
		delete fs;
	}
	catch (...)
	{
		WriteFile(path, contents);
	}
}