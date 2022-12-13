#include "Value.h"
#include <sstream>
namespace Xml {

Value::Value() { }

Value::Value(const char *name) : m_name(new STD::string(name)) {}

Value::Value(const std::string &name) : m_name (new STD::string(name)) { }

Value::Value(const Value &other) {
    if(other.m_name) m_name = new STD::string(*other.m_name);
    if(other.m_text) m_text = new STD::string(*other.m_text);
    if(other.m_attrs) m_attrs = new Attr(*other.m_attrs);
    if(other.m_child) m_child = new STD::list<Value>(*other.m_child);
}
Value::Value(Value &&other) {
    swap(other);
}

Value::~Value() {
    clear();
}

STD::string Value::name() const {
    if(m_name)
        return *m_name;
    else
        return "";
}

STD::string Value::text() const {
    if(m_text)
        return *m_text;
    else
        return "";
}

Value::Attr *Value::attr() {
    return m_attrs;
}

const Value::Attr *Value::attr() const {
    return m_attrs;
}

STD::string Value::attr(const STD::string &key) const {
    if(m_attrs) {
        auto it = m_attrs->find(key);
        if(it != m_attrs->end())
               return it->second;
    }
    return "";
}

void Value::name(const STD::string &name) {
    if(m_name)
        *m_name = name;
    else
        m_name = new STD::string(name);
}

void Value::text(const STD::string &text) {
    if(m_text)
        *m_text = text;
    else
        m_text = new STD::string(text);
}

void Value::attr(const std::string &key, const String value) {
    if(!m_attrs)
        m_attrs = new STD::map<STD::string,String>;
    (*m_attrs)[key] = value;
}

void Value::append(const Value &child) {
    if(!m_child) m_child = new STD::list<Value>;
    m_child->push_back(child);
}

void Value::append(Value &&child) {
    if(!m_child) m_child = new STD::list<Value>;
    m_child->push_back(std::move(child));
}

void Value::remove(int index) {
    if(!m_child || index < 0 || (unsigned)index >= m_child->size()) return;
    auto it = m_child->begin();
    for(int i = 0;i < index;++i) ++it;
    if(it != m_child->end()) m_child->erase(it);    //erase 会调用 移除对象的析构函数释放内存
}

void Value::remove(const std::string &name) {
    if(m_child == nullptr) return;
    for(auto it = m_child->begin();it != m_child->end();) {
        if(it->name() == name)
            it = m_child->erase(it);
        else
            ++it;
    }
}

void Value::clear() {
    if(m_name) delete m_name;
    if(m_text) delete m_text;
    if(m_child) delete m_child;
    if(m_attrs) delete m_attrs;

    m_name = nullptr;
    m_text = nullptr;
    m_attrs = nullptr;
    m_child = nullptr;
}

Value &Value::operator[](int index) {
    if(!m_child) throw STD::logic_error("element child is empty");
    if(index < 0) throw STD::logic_error("index Out of range");
    if((unsigned)index >= m_child->size()) throw STD::logic_error("index Out of range");

    auto it = m_child->begin();
    for(int i = 0;i < index;++i) ++it;
    return *it;
}

Value &Value::operator[](const std::string &name) {
    if(!m_child)
        m_child = new STD::list<Value>;

    for(auto &it : *m_child)
        if(*it.m_name == name) return it;

    m_child->push_back(Value(name));
    return m_child->back();
}

Value &Value::operator=(const Value &other) {
    clear();
    if(other.m_name) m_name = new STD::string(*other.m_name);
    if(other.m_text) m_text = new STD::string(*other.m_text);
    if(other.m_attrs) m_attrs = new Attr(*other.m_attrs);
    if(other.m_child) m_child = new STD::list<Value>(*other.m_child);
    return *this;
}

Value &Value::operator=(Value &&other) {
    swap(other);
    return *this;
}

Value::iterator Value::begin() {
    if(m_child)
        return m_child->begin();
    else
        throw STD::logic_error("xml child element is empty");
}

Value::iterator Value::end() {
    if(m_child)
        return m_child->end();
    else
        throw STD::logic_error("xml child element is empty");
}

Value::iterator Value::erase(Value::iterator it) {
    if(m_child)
        return m_child->erase(it);
    else
        throw STD::logic_error("xml child element is empty");
}

Value::const_iterator Value::begin() const {
    if(m_child)
        return m_child->begin();
    else
        throw STD::logic_error("xml child element is empty");
}

Value::const_iterator Value::end() const {
    if(m_child)
        return m_child->end();
    else
        throw STD::logic_error("xml child element is empty");
}

int Value::childCount() const {
    if(m_child)
        return m_child->size();
    else
        return 0;
}

int Value::attrConunt() const {
    if(m_attrs)
        return m_attrs->size();
    else
        return 0;
}

void Value::swap(Value &other) {
    std::swap(m_name,other.m_name);
    std::swap(m_text,other.m_text);
    std::swap(m_attrs,other.m_attrs);
    std::swap(m_child,other.m_child);
}

String::String() { }

String::String(const bool &value) {
    *this = value;
}

String::String(const int &value) {
    *this = value;
}

String::String(const double &value) {
    *this = value;
}

String::String(const char *value) : m_str(value){ }

String::String(const std::string &value): m_str(value){ }

String::String(const String &other) : m_str(other.m_str){ }

String &String::operator=(const bool &value) {
    m_str = value ? "true" : "false";
    return *this;
}

String &String::operator=(const int &value) {
    STD::stringstream ss;
    ss << value;
    m_str = ss.str();
    return *this;
}

String &String::operator=(const double &value) {
    STD::stringstream ss;
    ss << value;
    m_str = ss.str();
    return *this;
}

String &String::operator=(const char *value) {
    m_str = value;
    return *this;
}

String &String::operator=(const std::string &value) {
    m_str = value;
    return *this;
}

String &String::operator=(const String &other) {
    m_str = other.m_str;
    return *this;
}

String::operator const char *() {
    return m_str.c_str();
}

String::operator STD::string() {
    return m_str;
}

String::operator double() {
    return STD::atof(m_str.c_str());
}

String::operator int() {
    return atoi(m_str.c_str());
}

String::operator bool() {
    return m_str == "true";
}




}
