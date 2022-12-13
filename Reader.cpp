#include "Reader.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Xml {

Reader::Reader() :m_idx(0) { }

bool Reader::loadFile(const std::string &filePath) {
    STD::ifstream stream(filePath);
    if(!stream)
        return false;

    STD::stringstream ss;
    ss << stream.rdbuf();
    m_str = ss.str();
    m_idx = 0;
    return true;
}

void Reader::loadString(const std::string &str) {
    m_str = str;
    m_idx = 0;
}

Value Reader::parse() {
    // 解析 xml 头部
    skip_white_space();
    if(m_str.compare(m_idx,5,"<?xml") == 0) {
        if(!parse_declaration()) throw STD::logic_error("parse declaration error : end tag not found '?>' ");
    } else {
        STD::cout << "[warn] xml No declaration " << STD::endl;
    }

    // 跳过注释
    skip_white_space();
    while (m_str.compare(m_idx,4,"<!--") == 0) {
        if(!parse_comment()) throw STD::logic_error("parse comment error : end tag not found '-->' ");
        skip_white_space();
    }

    //解析成员 element
    if(m_str[m_idx] != '\0' && m_str[m_idx] == '<' && (STD::isalpha(m_str[m_idx + 1]) || m_str[m_idx + 1] == '_')){
        return parse_element();
    }
    throw STD::logic_error("parse element error");
}

Value Reader::parse(const std::string &str) {
    loadString(str);
    return parse();
}

void Reader::skip_white_space() {
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t') m_idx++;
//    switch (m_str[m_idx]) {
//    case ' ' :
//    case '\r':
//    case '\n':
//    case '\t':
//        m_idx++;
//        return;
//    default:
//        return;
    //    }
}

bool Reader::parse_declaration() {
    m_idx += 5;
    STD::size_t pos = m_str.find("?>",m_idx); //在500字符内查找是否存在结束标志 没有则返回false
    if(pos == STD::string::npos) return false;
    m_idx = pos + 2;
    return true;
}

bool Reader::parse_comment() {
    m_idx += 4;
    STD::size_t pos = m_str.find("-->",m_idx);
    if(pos == STD::string::npos) {
        return false;
    }
    m_idx = pos + 3;
    return true;
}

Value Reader::parse_element() {
    Value root;
    ++m_idx;
    skip_white_space();
    root.name(parse_element_name());

    while (m_idx != '\0') {
        skip_white_space();
        //解析单标签  <aaa />
        if(m_str[m_idx] == '/') {
            if(m_str[m_idx + 1] == '>') {
                m_idx += 2;
                break;
            } else {
                throw STD::logic_error("[ERROR] parse element error : " + root.name() + " end tag not found '>' ");
            }
        }

        //解析内容 text <aaa>text<aaa/>
        else if(m_str[m_idx] == '>') {
            m_idx++;
            skip_white_space();
            STD::string text = parse_element_text();
            if(!text.empty()) root.text(text);
        }

        //解析结束标志 or 注释 or 子元素
        else if(m_str[m_idx] == '<'){
            //结束标记
            if(m_str[m_idx + 1] == '/') {
                STD::string endTag = "</" + root.name() + ">";
                if(m_str.compare(m_idx,endTag.size(),endTag.c_str()) != 0){
                    throw STD::logic_error("[ERROR] parse element error : end tag not found '" + endTag + "'");
                }
                m_idx += endTag.length();
                break;
            }
            // 跳过注释
            else if(m_str.compare(m_idx,4,"<!--") == 0) {
                if(!parse_comment()) throw STD::logic_error("parse comment error : end tag not found '-->' ");
            }
            // 解析子元素
            else {
                root.append(parse_element());
            }
        }

        //解析属性
        else {
            STD::string key = parse_element_attr_key();
            skip_white_space();
            if(m_str[m_idx] != '=') throw STD::logic_error("parse attr error : " + key + " not found '=' ");
            m_idx++;
            skip_white_space();
            root.attr(key,parse_element_attr_val());
        }
    }
    return root;
}


std::string Reader::parse_element_name() {
    STD::size_t pos = m_idx;
    if(STD::isalpha(m_str[m_idx]) || m_str[m_idx] == '_') { // 判断 name 是否是已字母或者'_'开头
        m_idx++;
        while((STD::isalnum(m_str[m_idx]) || m_str[m_idx] == '_' ||
              m_str[m_idx] == '-' || m_str[m_idx] == '.' ) && m_str[m_idx] != '\0') m_idx++;
    } else {
        throw STD::logic_error("[ERROR] parse name : element invalid field " + m_str.substr(m_idx,1));
    }
    return m_str.substr(pos,m_idx - pos);
}

std::string Reader::parse_element_text() {
    STD::size_t pos = m_idx;
    m_idx = m_str.find('<',m_idx);
    if(m_idx == STD::string::npos) throw STD::logic_error("[ERROR] parse element error : end tag not found '<' ");
    return m_str.substr(pos,m_idx - pos);
}

std::string Reader::parse_element_attr_key() {
    STD::size_t pos = m_idx;
    if(STD::isalpha(m_str[m_idx]) || m_str[m_idx] == '_') { // 判断 name 是否是已字母或者'_'开头
        m_idx++;
        while((STD::isalnum(m_str[m_idx]) || m_str[m_idx] == '_' ||
              m_str[m_idx] == '-' || m_str[m_idx] == '.' ) && m_str[m_idx] != '\0') m_idx++;
    } else {
        throw STD::logic_error("[ERROR] parse attr key : element invalid field " + m_str.substr(m_idx,1));
    }
    return m_str.substr(pos,m_idx - pos);
}

std::string Reader::parse_element_attr_val() {
    if(m_str[m_idx] != '"') throw std::logic_error("[ERROR] parse element error : attr value should be in double quotes");
    STD::size_t pos = ++m_idx;
    while (m_str[m_idx] != '\0' && m_str[m_idx] != '"') m_idx++;
    m_idx++;    //跳过 '"' 符号
    return m_str.substr(pos,m_idx - pos - 1);
}





}
