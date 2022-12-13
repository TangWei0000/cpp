#ifndef READER_H
#define READER_H

/**********************************

  读取xml文件 解析成 Value 节点对象

  构建Value节点对象 采用的move特性 避免了不必要的拷贝赋值
***********************************/

#include <Value.h>
namespace Xml {
class Reader
{
public:
    Reader();
    bool loadFile(const STD::string& filePath);
    void loadString(const STD::string& str);
    Value parse();
    Value parse(const STD::string& str);
private:
    void skip_white_space();    //跳过空白区域
    bool parse_declaration();   //解析声明
    bool parse_comment();       //解析注释

    Value parse_element();
    STD::string parse_element_name();
    STD::string parse_element_text();
    STD::string parse_element_attr_key();
    STD::string parse_element_attr_val();
private:
    STD::string m_str;
    STD::size_t m_idx;
};


}
#endif // READER_H
