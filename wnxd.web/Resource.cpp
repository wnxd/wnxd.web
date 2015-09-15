#include "Resource.h"

using namespace wnxd::Web;
//internal
System::Resources::ResourceManager^ Resource::ResourceManager::get()
{
	if (Object::ReferenceEquals(resourceMan, nullptr))
	{
		System::Resources::ResourceManager^ temp = gcnew System::Resources::ResourceManager("wnxd.Web.Resource", Resource::typeid->Assembly);
		resourceMan = temp;
	}
	return resourceMan;
}
System::Globalization::CultureInfo^ Resource::Culture::get()
{
	return resourceCulture;
}
void Resource::Culture::set(System::Globalization::CultureInfo^ value)
{
	resourceCulture = value;
}
String^ Resource::cookie::get()
{
	return ResourceManager->GetString("cookie", resourceCulture);
}
String^ Resource::load::get()
{
	return ResourceManager->GetString("load", resourceCulture);
}
String^ Resource::web::get()
{
	return ResourceManager->GetString("web", resourceCulture);
}