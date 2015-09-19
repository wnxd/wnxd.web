#include "html.h"

using namespace wnxd::Web;
//class Html
Html::~Html()
{
	if (List_hooklist->ContainsKey(this)) List_hooklist->Remove(this);
	if (Dictionary_hooklist->ContainsKey(this)) Dictionary_hooklist->Remove(this);
}
//private
void Html::List_Add(List<Object^>^ target, Object^ item)
{
	Hook::Source->Run(target, item);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
void Html::List_AddRange(List<Object^>^ target, IEnumerable<Object^>^ collection)
{
	Hook::Source->Run(target, collection);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
void Html::List_Item_set(List<Object^>^ target, int index, Object^ item)
{
	Hook::Source->Run(target, index, item);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
void Html::List_Insert(List<Object^>^ target, int index, Object^ item)
{
	Hook::Source->Run(target, index, item);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
void Html::List_InsertRange(List<Object^>^ target, int index, IEnumerable<Object^>^ collection)
{
	Hook::Source->Run(target, index, collection);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
bool Html::List_Remove(List<Object^>^ target, Object^ item)
{
	bool r = Hook::Source->Run<bool>(target, item);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
	return r;
}
int Html::List_RemoveAll(List<Object^>^ target, Predicate<Object^>^ match)
{
	int r = Hook::Source->Run<int>(target, match);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
	return r;
}
void Html::List_RemoveAt(List<Object^>^ target, int index)
{
	Hook::Source->Run(target, index);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
void Html::List_RemoveRange(List<Object^>^ target, int index, int count)
{
	Hook::Source->Run(target, index, count);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
void Html::List_Clear(List<Object^>^ target)
{
	Hook::Source->Run(target);
	if (target->GetType()->GetGenericArguments()[0] == String::typeid){
		for each(KeyValuePair<Html^, IList<String^>^> kv in List_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->classList = (List<String^>^)target;
				break;
			}
		}
	}
}
void Html::Dictionary_Add(Dictionary<Object^, Object^>^ target, Object^ key, Object^ value)
{
	Hook::Source->Run(target, key, value);
	array<Type^>^ ga = target->GetType()->GetGenericArguments();
	if (ga[0] == String::typeid && ga[1] == String::typeid)
	{
		for each(KeyValuePair<Html^, IDictionary<String^, String^>^> kv in Dictionary_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->Refresh();
				break;
			}
		}
	}
}
void Html::Dictionary_Item_set(Dictionary<Object^, Object^>^ target, Object^ key, Object^ value)
{
	Hook::Source->Run(target, key, value);
	array<Type^>^ ga = target->GetType()->GetGenericArguments();
	if (ga[0] == String::typeid && ga[1] == String::typeid)
	{
		for each(KeyValuePair<Html^, IDictionary<String^, String^>^> kv in Dictionary_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->Refresh();
				break;
			}
		}
	}
}
void Html::Dictionary_Remove(Dictionary<Object^, Object^>^ target, Object^ key)
{
	Hook::Source->Run(target, key);
	array<Type^>^ ga = target->GetType()->GetGenericArguments();
	if (ga[0] == String::typeid && ga[1] == String::typeid)
	{
		for each(KeyValuePair<Html^, IDictionary<String^, String^>^> kv in Dictionary_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->Refresh();
				break;
			}
		}
	}
}
void Html::Dictionary_Clear(Dictionary<Object^, Object^>^ target)
{
	Hook::Source->Run(target);
	array<Type^>^ ga = target->GetType()->GetGenericArguments();
	if (ga[0] == String::typeid && ga[1] == String::typeid)
	{
		for each(KeyValuePair<Html^, IDictionary<String^, String^>^> kv in Dictionary_hooklist)
		{
			if (kv.Value->Equals(target))
			{
				kv.Key->Refresh();
				break;
			}
		}
	}
}
void Html::AddText(Control^ control, String^ text)
{
	if (control->Controls->Count > 0 && control->Controls[control->Controls->Count - 1]->GetType() == LiteralControl::typeid) ((LiteralControl^)control->Controls[control->Controls->Count - 1])->Text += text;
	else control->Controls->Add(gcnew LiteralControl(text));
}
int Html::Contains(MatchCollection^ MatchList, int Index)
{
	if (MatchList != nullptr && MatchList->Count > 0)
	{
		for (int i = 0; i < MatchList->Count; i++)
		{
			Group^ g = MatchList[i]->Groups[1];
			if (!g->Success) g = MatchList[i]->Groups[2];
			if (g->Index < Index && Index < g->Index + g->Length) return g->Index + g->Length;
		}
	}
	return -1;
}
String^ Html::Render(System::Web::UI::Control^ control)
{
	StringWriter^ sw = gcnew StringWriter();
	HtmlTextWriter^ htw = gcnew HtmlTextWriter(sw);
	Type^ T = control->GetType();
	MethodInfo^ mi = T->GetMethod("Render", BindingFlags::Instance | BindingFlags::Public | BindingFlags::NonPublic);
	array<Object^>^ args = { htw };
	if (mi != nullptr) mi->Invoke(control, args);
	String^ r = sw->ToString();
	delete htw;
	delete sw;
	return r;
}
Control^ Html::_querySelector(Control^ control, String^ TagName, IList<String^>^ ClassNameList, String^ ID, IDictionary<String^, String^>^ Attr)
{
	TagName = TagName->ToLower();
	for (int i = 0; i < control->Controls->Count; i++)
	{
		if (Html::typeid->IsAssignableFrom(control->Controls[i]->GetType()))
		{
			Html^ child = (Html^)control->Controls[i];
			if (!String::IsNullOrEmpty(TagName) && child->tagName != TagName) goto FindChildren;
			if (ClassNameList != nullptr && ClassNameList->Count > 0)
			{
				IList<String^>^ classList = child->classList;
				if (classList->Count == 0) goto FindChildren;
				bool b = false;
				for (int n = 0; n < ClassNameList->Count; n++)
				{
					if (!classList->Contains(ClassNameList[n]))
					{
						b = true;
						break;
					}
				}
				if (b) goto FindChildren;
			}
			if (!String::IsNullOrEmpty(ID) && child->id != ID) goto FindChildren;
			if (Attr != nullptr && Attr->Count > 0)
			{
				IDictionary<String^, String^>^ attributes = child->attributes;
				if (attributes->Count == 0) goto FindChildren;
				bool b = false;
				for each(KeyValuePair<String^, String^> kv in Attr)
				{
					if (!attributes->Contains(kv))
					{
						b = true;
						break;
					}
				}
				if (b) goto FindChildren;
			}
			return child;
		FindChildren:
			{
				Control^ col = _querySelector(child, TagName, ClassNameList, ID, Attr);
				if (col != nullptr) return col;
			}
		}
	}
	return nullptr;
}
Control^ Html::_queryRegExp(Control^ control, Regex^ regex)
{
	for (int i = 0; i < control->Controls->Count; i++) if (regex->IsMatch(GetInnerHTML(control->Controls[i]))) return _queryRegExp(control->Controls[i], regex);
	return control;
}
//internal
void Html::EnsureChildControls(System::Web::UI::Control^ control)
{
	Type^ T = control->GetType();
	MethodInfo^ mi = T->GetMethod("EnsureChildControls", BindingFlags::Instance | BindingFlags::Public | BindingFlags::NonPublic);
	if (mi != nullptr) mi->Invoke(control, nullptr);
	for (int i = 0; i < control->Controls->Count; i++) EnsureChildControls(control->Controls[i]);
}
//protected
void Html::Refresh()
{
	String^ val = "<" + this->_tagName;
	if (this->_attributes != nullptr)
	{
		for each(KeyValuePair<String^, String^> kv in this->_attributes)
		{
			val += " " + kv.Key;
			if (kv.Value != nullptr) val += "=\"" + kv.Value + "\"";
		}
	}
	if (this->_tagName == "meta" || this->_tagName == "link" || this->_tagName == "input" || this->_tagName == "img" || this->_tagName == "br")
	{
		this->_head = val + " />";
		this->_foot = String::Empty;
	}
	else
	{
		this->_head = val + ">";
		this->_foot = "</" + this->_tagName + ">";
	}
}
//public
String^ Html::tagName::get()
{
	if (this->_tagName == nullptr)
	{
		Match^ mc = (gcnew Regex("^<(\\w+) ?"))->Match(this->_head);
		if (mc != nullptr && mc->Success) this->_tagName = mc->Groups[1]->Value->ToLower();
	}
	return this->_tagName;
}
void Html::tagName::set(String^ value)
{
	this->_tagName = value;
	this->Refresh();
}
String^ Html::className::get()
{
	return this->getAttribute("class");
}
void Html::className::set(String^ value)
{
	this->setAttribute("class", value);
}
IList<String^>^ Html::classList::get()
{
	List<String^>^ list = gcnew List<String^>();
	String^ className = this->className;
	if (className != nullptr) if (className != String::Empty) list->AddRange(className->Split(' '));
	if (List_hooklist->ContainsKey(this)) List_hooklist[this] = list;
	else List_hooklist->Add(this, list);
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "Add"), Hook::GetMethod(Html::typeid, "List_Add"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "AddRange"), Hook::GetMethod(Html::typeid, "List_AddRange"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "set_Item"), Hook::GetMethod(Html::typeid, "List_Item_set"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "Insert"), Hook::GetMethod(Html::typeid, "List_Insert"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "InsertRange"), Hook::GetMethod(Html::typeid, "List_InsertRange"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "Remove"), Hook::GetMethod(Html::typeid, "List_Remove"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "RemoveAll"), Hook::GetMethod(Html::typeid, "List_RemoveAll"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "RemoveAt"), Hook::GetMethod(Html::typeid, "List_RemoveAt"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "RemoveRange"), Hook::GetMethod(Html::typeid, "List_RemoveRange"));
	Hook::Register(Hook::GetMethod(List<String^>::typeid, "Clear"), Hook::GetMethod(Html::typeid, "List_Clear"));
	return list;
}
void Html::classList::set(IList<String^>^ value)
{
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "Add"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "AddRange"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "set_Item"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "Insert"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "InsertRange"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "Remove"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "RemoveAll"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "RemoveAt"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "RemoveRange"));
	Hook::Unregister(Hook::GetMethod(List<String^>::typeid, "Clear"));
	IDictionary<String^, String^>^ attr = this->attributes;
	if (value == nullptr) { if (attr != nullptr) attr->Remove("class"); }
	else
	{
		String^ val = String::Empty;
		if (value->Count > 0)
		{
			array<String^>^ list = gcnew array<String^>(value->Count);
			value->CopyTo(list, 0);
			val = String::Join(" ", list);
		}
		if (attr->ContainsKey("class")) attr["class"] = val;
		else attr->Add("class", val);
	}
	this->Refresh();
}
String^ Html::id::get()
{
	return this->getAttribute("id");
}
void Html::id::set(String^ value)
{
	this->setAttribute("id", value);
}
String^ Html::style::get()
{
	return this->getAttribute("style");
}
void Html::style::set(String^ value)
{
	this->setAttribute("style", value);
}
String^ Html::title::get()
{
	return this->getAttribute("title");
}
void Html::title::set(String^ value)
{
	this->setAttribute("title", value);
}
IDictionary<String^, String^>^ Html::attributes::get()
{
	if (this->_attributes == nullptr)
	{
		this->_attributes = gcnew Dictionary<String^, String^>(StringComparer::OrdinalIgnoreCase);
		MatchCollection^ mc = (gcnew Regex("(\\w+)(?:=['\"]([\\s\\S]*?)['\"])?"))->Matches(this->_head);
		if (mc != nullptr && mc->Count > 0)
		{
			for (int i = 0; i < mc->Count; i++)
			{
				GroupCollection^ gc = mc[i]->Groups;
				String^ key = gc[1]->Value->ToLower();
				if (key != this->tagName)
				{
					String^ val = gc[2]->Success ? gc[2]->Value : nullptr;
					this->_attributes->Add(key, val);
				}
			}
		}
		if (Dictionary_hooklist->ContainsKey(this)) Dictionary_hooklist[this] = this->_attributes;
		else Dictionary_hooklist->Add(this, this->_attributes);
	}
	return this->_attributes;
}
void Html::attributes::set(IDictionary<String^, String^>^ value)
{
	this->_attributes = gcnew Dictionary<String^, String^>(value);
	if (Dictionary_hooklist->ContainsKey(this)) Dictionary_hooklist[this] = this->_attributes;
	else Dictionary_hooklist->Add(this, this->_attributes);
	this->Refresh();
}
String^ Html::innerHTML::get()
{
	StringWriter^ sw = gcnew StringWriter();
	HtmlTextWriter^ htw = gcnew HtmlTextWriter(sw);
	this->Control::Render(htw);
	String^ r = sw->ToString();
	delete htw;
	delete sw;
	return r;
}
void Html::innerHTML::set(String^ value)
{
	SetInnerHTML(this, value);
}
String^ Html::outerHTML::get()
{
	return this->_head + this->innerHTML + this->_foot;
}
String^ Html::getAttribute(String^ attributename)
{
	String^ val;
	this->attributes->TryGetValue(attributename, val);
	return val;
}
void Html::setAttribute(String^ attributename, String^ attributevalue)
{
	IDictionary<String^, String^>^ attr = this->attributes;
	if (attributevalue == nullptr) { if (attr->ContainsKey(attributename)) attr->Remove(attributename); }
	else
	{
		if (attr->ContainsKey(attributename)) attr[attributename] = attributevalue;
		else attr->Add(attributename, attributevalue);
	}
}
void Html::Render(HtmlTextWriter^ writer)
{
	writer->Write(this->outerHTML);
}
String^ Html::ToString()
{
	return this->outerHTML;
}
String^ Html::GetType()
{
	return this->tagName;
}
void Html::SetInnerHTML(Control^ control, String^ html)
{
	control->Controls->Clear();
	if (!String::IsNullOrEmpty(html))
	{
		MatchCollection^ mc = (gcnew Regex("(?:['\"]([\\s\\S]*?)['\"])|(?:<script.*?>([\\s\\S]*?)</script>)"))->Matches(html);
		System::Web::UI::Control^ $this = control;
		int offset = 0, start = 0, m;
		do
		{
			offset = html->IndexOf('<', offset);
			if (offset != -1)
			{
				m = Contains(mc, offset);
				if (m == -1)
				{
					if (start != offset) AddText($this, html->Substring(start, offset - start));
					start = offset;
					do
					{
						offset = html->IndexOf('>', offset);
						if (offset != -1)
						{
							m = Contains(mc, offset);
							if (m == -1)
							{
								int t = html->LastIndexOf('<', offset);
								if (Contains(mc, t) == -1 && t > start)
								{
									AddText($this, html->Substring(start, t - start));
									start = t;
								}
								offset++;
								String^ head = html->Substring(start, offset - start);
								switch (head[1])
								{
								case '/':
								{
									if (Html::typeid->IsAssignableFrom($this->GetType()))
									{
										Match^ val = (gcnew Regex("^</(\\w+) ?"))->Match(head);
										if (val != nullptr && val->Success)
										{
											Html^ child = (Html^)$this;
											if (val->Groups[1]->Value->ToLower() == child->tagName)
											{
												child->_foot = head;
												$this = $this->Parent;
											}
											else AddText($this, head);
										}
										else AddText($this, head);
									}
									else AddText($this, head);
									break;
								}
								case '!':
								{
									AddText($this, head);
									break;
								}
								default:
								{
									Match^ mt = (gcnew Regex("^<(\\w+) ?"))->Match(head);
									if (mt != nullptr && mt->Success)
									{
										String^ tagName = mt->Groups[1]->Value->ToLower();
										Html^ child;
										if (tagName == "img") child = gcnew Img();
										else if (tagName == "input") child = gcnew Input();
										else if (tagName == "script") child = gcnew Script();
										else if (tagName == "a") child = gcnew A();
										else child = gcnew Html();
										child->_head = head;
										$this->Controls->Add(child);
										if (!(tagName == "meta" || tagName == "link" || tagName == "input" || tagName == "img" || tagName == "br")) $this = child;
									}
									else AddText($this, head);
									break;
								}
								}
								start = offset;
								break;
							}
							else offset = m;
						}
						else AddText($this, html->Substring(start, html->Length - start));
					} while (offset != -1 && offset < html->Length);
				}
				else offset = m;
			}
			else AddText($this, html->Substring(start, html->Length - start));
		} while (offset != -1 && offset < html->Length);
	}
}
String^ Html::GetInnerHTML(Control^ control)
{
	EnsureChildControls(control);
	return Render(control);
}
void Html::Reconstruction(Control^ control)
{
	if (control->HasControls())
	{
		for (int i = 0; i < control->Controls->Count; i++) if (Html::typeid->IsAssignableFrom(control->Controls[i]->GetType())) return;
		String^ html = GetInnerHTML(control);
		control->Controls->Clear();
		if (html != String::Empty) SetInnerHTML(control, html);
	}
}
Html^ Html::FindControl(Control^ control, String^ query)
{
	return FindControl(control, query, QueryType::Css_Path);
}
Html^ Html::FindControl(Control^ control, String^ query, QueryType querytype)
{
	if (control->HasControls() && !String::IsNullOrEmpty(query))
	{
		switch (querytype)
		{
		case QueryType::Css_Path: return querySelector(control, query);
		case QueryType::Reg_Exp: return queryRegExp(control, query);
		}
	}
	return nullptr;
}
enum querySelector_Status
{
	TagName,
	ClassName,
	ID,
	Attr_Key,
	Attr_Apart,
	Attr_Val,
	Attr_End
};
Html^ Html::querySelector(Control^ control, String^ selectors)
{
	if (control->HasControls())
	{
		selectors = selectors->Trim();
		if (selectors != String::Empty)
		{
			String^ TagName = String::Empty, ^ClassName = String::Empty, ^ID = String::Empty, ^Attr_Key = String::Empty, ^Attr_Val = String::Empty;
			IList<String^>^ ClassNameList = gcnew List<String^>();
			bool _ID = false;
			IDictionary<String^, String^>^ Attr = gcnew Dictionary<String^, String^>();
			querySelector_Status flag = querySelector_Status::TagName;
			for (int i = 0; i < selectors->Length; i++)
			{
				switch (selectors[i])
				{
				case '.':
				{
					if (flag == querySelector_Status::ClassName)
					{
						if (ClassName == String::Empty) throw gcnew Exception("类名不能为空");
						ClassNameList->Add(ClassName);
						ClassName = String::Empty;
					}
					else if (flag == querySelector_Status::Attr_Key || flag == querySelector_Status::Attr_Apart) throw gcnew Exception("格式错误");
					else if (flag == querySelector_Status::Attr_Val)
					{
						if (Attr_Key == String::Empty) throw gcnew Exception("格式错误");
						Attr_Val += selectors[i];
					}
					else flag = querySelector_Status::ClassName;
					break;
				}
				case '#':
				{
					if (_ID) throw gcnew Exception("控件不能拥有多个ID");
					_ID = true;
					if (flag == querySelector_Status::ClassName)
					{
						if (ClassName == String::Empty) throw gcnew Exception("类名不能为空");
						ClassNameList->Add(ClassName);
						ClassName = String::Empty;
					}
					else if (flag == querySelector_Status::Attr_Key || flag == querySelector_Status::Attr_Apart) throw gcnew Exception("格式错误");
					else if (flag == querySelector_Status::Attr_Val)
					{
						if (Attr_Key == String::Empty) throw gcnew Exception("格式错误");
						Attr_Val += selectors[i];
					}
					else flag = querySelector_Status::ID;
					break;
				}
				case '[':
				{

					if (flag == querySelector_Status::ClassName)
					{
						if (ClassName == String::Empty) throw gcnew Exception("类名不能为空");
						ClassNameList->Add(ClassName);
						ClassName = String::Empty;
					}
					else if (flag == querySelector_Status::Attr_Key || flag == querySelector_Status::Attr_Apart) throw gcnew Exception("格式错误");
					else if (flag == querySelector_Status::Attr_Val)
					{
						if (Attr_Key == String::Empty) throw gcnew Exception("格式错误");
						Attr_Val += selectors[i];
					}
					else flag = querySelector_Status::Attr_Key;
					break;
				}
				case ']':
				{
					if (flag == querySelector_Status::Attr_Key)
					{
						if (Attr_Key == String::Empty) throw gcnew Exception("属性名不能为空");
						Attr->Add(Attr_Key, nullptr);
						Attr_Key = String::Empty;
						flag = querySelector_Status::Attr_End;
					}
					else if (flag == querySelector_Status::Attr_Val)
					{
						if (Attr_Key == String::Empty) flag = querySelector_Status::Attr_End;
						else Attr_Val += selectors[i];
					}
					else throw gcnew Exception("格式错误");
					break;
				}
				case '=':
				{
					if (flag == querySelector_Status::Attr_Key)
					{
						if (Attr_Key == String::Empty) throw gcnew Exception("属性名不能为空");
						else flag = querySelector_Status::Attr_Apart;
					}
					else if (flag == querySelector_Status::Attr_Val && Attr_Key != String::Empty) Attr_Val += selectors[i];
					else throw gcnew Exception("格式错误");
					break;
				}
				case '\'':
				{
					if (flag == querySelector_Status::Attr_Val)
					{
						Attr->Add(Attr_Key, Attr_Val);
						Attr_Key = String::Empty;
						Attr_Val = String::Empty;
					}
					else if (flag != querySelector_Status::Attr_Apart) throw gcnew Exception("格式错误");
					else flag = querySelector_Status::Attr_Val;
					break;
				}
				case ' ':
				{
					if (flag == querySelector_Status::Attr_Val && Attr_Key != String::Empty) Attr_Val += selectors[i];
					else throw gcnew Exception("格式错误");
					break;
				}
				default:
				{
					switch (flag)
					{
					case querySelector_Status::TagName:
					{
						TagName += selectors[i];
						break;
					}
					case querySelector_Status::ClassName:
					{
						ClassName += selectors[i];
						break;
					}
					case querySelector_Status::ID:
					{
						ID += selectors[i];
						break;
					}
					case querySelector_Status::Attr_Key:
					{
						Attr_Key += selectors[i];
						break;
					}
					case querySelector_Status::Attr_Val:
					{
						if (Attr_Key == String::Empty) throw gcnew Exception("属性名不能为空");
						Attr_Val += selectors[i];
						break;
					}
					case querySelector_Status::Attr_Apart:
					case querySelector_Status::Attr_End: throw gcnew Exception("格式错误");
					}
					break;
				}
				}
			}
			if (ClassName != String::Empty) ClassNameList->Add(ClassName);
			if (TagName == String::Empty && ClassNameList->Count == 0 && ID == String::Empty && Attr->Count == 0) throw gcnew Exception("没有查询条件");
			Reconstruction(control);
			return (Html^)_querySelector(control, TagName, ClassNameList, ID, Attr);
		}
	}
	return nullptr;
}
Html^ Html::queryRegExp(Control^ control, String^ pattern)
{
	return queryRegExp(control, gcnew Regex(pattern, RegexOptions::IgnoreCase));
}
Html^ Html::queryRegExp(Control^ control, Regex^ regex)
{
	if (control->HasControls() && regex != nullptr)
	{
		Reconstruction(control);
		Control^ col = _queryRegExp(control, regex);
		if (col->GetType() == LiteralControl::typeid) col = col->Parent;
		if (col != control) return (Html^)col;
	}
	return nullptr;
}
//class Img
//public
Img::Img()
{
	this->Html::tagName = "img";
}
String^ Img::tagName::get()
{
	return this->Html::tagName;
}
String^ Img::src::get()
{
	return this->getAttribute("src");
}
void Img::src::set(String^ value)
{
	this->setAttribute("src", value);
}
String^ Img::alt::get()
{
	return this->getAttribute("alt");
}
void Img::alt::set(String^ value)
{
	this->setAttribute("alt", value);
}
String^ Img::width::get()
{
	return this->getAttribute("width");
}
void Img::width::set(String^ value)
{
	this->setAttribute("width", value);
}
String^ Img::height::get()
{
	return this->getAttribute("height");
}
void Img::height::set(String^ value)
{
	this->setAttribute("height", value);
}
//class Input
//public
Input::Input()
{
	this->Html::tagName = "input";
}
String^ Input::tagName::get()
{
	return this->Html::tagName;
}
bool Input::disabled::get()
{
	String^ a = this->getAttribute("disabled");
	return a != nullptr && a == "disabled";
}
void Input::disabled::set(bool value)
{
	String^ v;
	if (value) v = "disabled";
	this->setAttribute("src", v);
}
InputType Input::type::get()
{
	String^ a = this->getAttribute("type");
	InputType r;
	if (a == nullptr) r = InputType::none;
	else
	{
		try
		{
			r = (InputType)System::Enum::Parse(InputType::typeid, a);
		}
		catch (...)
		{
			r = InputType::unknown;
		}
	}
	return r;
}
void Input::type::set(InputType value)
{
	String^ v;
	if (value != InputType::none) v = System::Enum::GetName(InputType::typeid, (int)value);;
	this->setAttribute("type", v);
}
String^ Input::name::get()
{
	return this->getAttribute("name");
}
void Input::name::set(String^ value)
{
	this->setAttribute("name", value);
}
String^ Input::value::get()
{
	return this->getAttribute("value");
}
void Input::value::set(String^ value)
{
	this->setAttribute("value", value);
}
//class Script
//public
Script::Script()
{
	this->Html::tagName = "script";
}
String^ Script::tagName::get()
{
	return this->Html::tagName;
}
String^ Script::type::get()
{
	return this->getAttribute("type");
}
void Script::type::set(String^ value)
{
	this->setAttribute("type", value);
}
String^ Script::src::get()
{
	return this->getAttribute("src");
}
void Script::src::set(String^ value)
{
	this->setAttribute("src", value);
}
//class A
//public
A::A()
{
	this->Html::tagName = "a";
}
String^ A::tagName::get()
{
	return this->Html::tagName;
}
String^ A::href::get()
{
	return this->getAttribute("href");
}
void A::href::set(String^ value)
{
	this->setAttribute("href", value);
}
String^ A::rel::get()
{
	return this->getAttribute("rel");
}
void A::rel::set(String^ value)
{
	this->setAttribute("rel", value);
}
String^ A::target::get()
{
	return this->getAttribute("target");
}
void A::target::set(String^ value)
{
	this->setAttribute("target", value);
}