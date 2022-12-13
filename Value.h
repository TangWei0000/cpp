#ifndef XML_H
#define XML_H
/**********************************

  xml 节点对象 Value

***********************************/

#include <string>
#include <list>
#include <map>

#ifndef STD
#define STD ::std
#endif

namespace Xml {

/** @brief 实现基本类型转(bool int double const char*)换到 string类型  低层使用 stringstream实现 性能比boost库的lexical_cast低*/
class String {
public:
    String();
    String(const bool& value);
    String(const int& value);
    String(const double& value);
    String(const char* value);
    String(const STD::string& value);
    String(const String& other);

    String& operator=(const bool& value);
    String& operator=(const int& value);
    String& operator=(const double& value);
    String& operator=(const char* value);
    String& operator=(const STD::string& value);
    String& operator=(const String& other);

    operator bool();
    operator int();
    operator double();
    operator STD::string();
    operator const char*();

    bool operator!=(const String& other);
    bool operator==(const String& other);

private:
    STD::string m_str;
};

class Value
{
public:
    using iterator = STD::list<Value>::iterator;
    using const_iterator = STD::list<Value>::const_iterator;
    using Attr = STD::map<STD::string,String>;

    Value();
    Value(const char* name);
    Value(const STD::string& name);
    Value(const Value& other);
    Value(Value&& other);
    ~Value();

    STD::string name() const;
    STD::string text() const;
    Attr *attr();
    const Attr *attr() const;
    STD::string attr(const STD::string& key) const;

    void name(const STD::string& name);
    void text(const STD::string& text);
    void attr(const STD::string& key,const String value);
    void append(const Value& child);
    void append(Value&& child);

    void remove(int index);
    void remove(const STD::string& name);

    void clear();
    Value& operator[](int index);
    Value& operator[](const STD::string& name);
    Value& operator=(const Value& other);
    Value& operator=(Value&& other);

    iterator begin();
    iterator end();
    iterator erase(iterator it);
    const_iterator begin() const;
    const_iterator end() const;

    int childCount() const;
    int attrConunt() const;
    void swap(Value &other);

private:
    STD::string*        m_name{nullptr};     //名称
    STD::string*        m_text{nullptr};     //内容
    STD::list<Value>*   m_child{nullptr};    //子成员
    Attr*               m_attrs{nullptr};    //属性
    friend class Write;
};




}


#endif // XML_H
