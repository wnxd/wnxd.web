#include "config.h"
#include <initializer_list>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Text::RegularExpressions;
using namespace System::Web;
using namespace System::Web::SessionState;
using namespace wnxd::Config;
using namespace std;

namespace wnxd
{
	namespace Web
	{
		/// <summary>
		/// 入口类,继承该类并调用Register进行注册即可监听应用程序相关事件
		/// </summary>
		public ref class Enter abstract
		{
		public:
			void Register()
			{
				this->Register(true);
			}
			void Register(bool assembly)
			{
				Register(this->GetType(), assembly);
			}
			static void Register(Enter^ obj)
			{
				Register(obj, true);
			}
			static void Register(Enter^ obj, bool assembly)
			{
				Register(obj->GetType(), assembly);
			}
			static void Register(Type^ enter)
			{
				Register(enter, true);
			}
			static void Register(Type^ enter, bool assembly)
			{
				if (Enter::typeid->IsAssignableFrom(enter))
				{
					config^ config = gcnew wnxd::Config::config(AppDomain::CurrentDomain->BaseDirectory + "wnxd/wnxd_config.tmp");
					String^ s = config["enter_list"], ^aqn = assembly ? enter->AssemblyQualifiedName : enter->FullName;
					if (String::IsNullOrEmpty(s)) s = aqn;
					else
					{
						if (s->Contains(aqn)) goto next;
						s += "||" + aqn;
					}
					config["enter_list"] = s;
					if (Init::_enter_list == nullptr) Init::_enter_list = gcnew List<Enter^>();
					Init::_enter_list->Add((Enter^)Activator::CreateInstance(enter));
				next:
					String^ path = AppDomain::CurrentDomain->BaseDirectory + "Global.asax";
					if (File::Exists(path))
					{
						String^ str = file::ReadFile(path);
						Match^ mc = (gcnew Regex("Inherits=\"(.*?)\""))->Match(str);
						if (mc != nullptr)
						{
							if (mc->Groups[1]->Value == "wnxd.web.init") return;
							config["old_Global"] = mc->Groups[1]->Value;
						}
						File::SetAttributes(path, FileAttributes::Normal);
					}
					file::WriteFile(path, "<%@ Application Inherits=\"wnxd.web.init\" Language=\"C#\" %>");
				}
			}

		};
		public ref class Init abstract : HttpApplication
		{
		private:
			Object^ _old_app;
			Type^ _old_type;
			void _init()
			{
				config^ config = gcnew wnxd::Config::config(AppDomain::CurrentDomain->BaseDirectory + "wnxd/wnxd_config.tmp");
				String^ className = config["old_Global"];
				if (!String::IsNullOrEmpty(className))
				{
					array<Assembly^>^ list = AppDomain::CurrentDomain->GetAssemblies();
					BindingFlags all = BindingFlags::Instance | BindingFlags::Static | BindingFlags::Public | BindingFlags::NonPublic;
					for (int i = 0; i < list->Length; i++)
					{
						Type^ T = list[i]->GetType(className);
						if (T != nullptr)
						{
							try
							{
								this->_old_app = Activator::CreateInstance(T);
							}
							catch (...)
							{

							}
							if (this->_old_app != nullptr)
							{
								this->_old_type = T;
								T = HttpApplication::typeid;
								try
								{
									FieldInfo^ fi = T->GetField("_context", all);
									fi->SetValue(this->_old_app, this->Context);
									fi = T->GetField("_state", all);
									fi->SetValue(this->_old_app, this->Application);
								}
								catch (...)
								{

								}
								break;
							}
						}
					}
				}
				String^ enter_list = config["enter_list"];
				if (!String::IsNullOrEmpty(enter_list))
				{
					array<String^>^ list = enter_list->Split('|', '|');
					_enter_list = gcnew List<Enter^>();
					for (int i = 0; i < list->Length; i++)
					{
						Type^ T = Type::GetType(list[i]);
						if (T != nullptr)
						{
							try
							{
								Enter^ e = (Enter^)Activator::CreateInstance(T);
								if (e != nullptr) _enter_list->Add(e);
							}
							catch (...)
							{

							}
						}

					}
				}
			}
			void _callback(String^ method, array<Object^>^ parameters)
			{
				if (this->_old_app != nullptr)
				{
					MethodInfo^ mi = this->_old_type->GetMethod(method, BindingFlags::Instance | BindingFlags::Static | BindingFlags::Public | BindingFlags::NonPublic);
					if (mi != nullptr)
					{
						int len = mi->GetParameters()->Length;
						if (len == 0) mi->Invoke(this->_old_app, nullptr);
						else if (len == 2)
						{
							mi->Invoke(this->_old_app, parameters);
						}
					}
				}
				if (_enter_list != nullptr)
				{
					MethodInfo^ mi = Enter::typeid->GetMethod(method, Type::EmptyTypes);
					for (int i = 0; i < _enter_list->Count; i++) mi->Invoke(_enter_list[i], nullptr);
				}
			}
		internal:
			static IList<Enter^>^ _enter_list;
		protected:
			Init()
			{
				if (this->GetType()->FullName != "ASP.global_asax") throw gcnew TypeLoadException("<wnxd.web.init>是特殊类无法被继承");
				this->_init();
				this->_callback("Initialize", {});
			}
		public:

		};
	}
}