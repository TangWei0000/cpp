#include "Write.h"

namespace Xml {
Write::Write() {}
STD::string Write::fastWrite(const Value &root) {
    m_ss.clear();
    declaration();
    FastWrite(root);
    return m_ss.str();
}

std::string Write::styleWrite(const Value &root) {
    m_ss.clear();
    m_level = 0;
    declaration();
    StyleWrite(root);
    return m_ss.str();
}

bool Write::fasetWirteFile(const Value &root, const std::string &filePath, std::ios_base::openmode mode) {
    STD::ofstream ofs;
    ofs.open(filePath,mode);
    if(ofs) {
        declaration(ofs);
        FastWrite(ofs,root);
        ofs.close();
        return true;
    }
    else
        return false;
}

bool Write::styleWirteFile(const Value &root, const std::string &filePath, std::ios_base::openmode mode) {
    STD::ofstream ofs;
    ofs.open(filePath,mode);
    if(ofs) {
        m_level = 0;
        declaration(ofs);
        StyleWrite(ofs,root);
        ofs.close();
        return true;
    }
    else
        return false;
}

void Write::FastWrite(const Value &root) {
    if(!root.m_name) throw STD::logic_error("element name is empty");
    m_ss << "<" << *root.m_name;
    if(root.m_attrs)
        for(auto &it : *root.m_attrs)
            m_ss << " " << it.first << "=\"" << (const char*)it.second << "\"";
    m_ss << ">";

    if(root.m_text) m_ss << *root.m_text;

    if(root.m_child)
        for(auto &it : *root.m_child) FastWrite(it);

    m_ss << "</" << *root.m_name << '>';
}

void Write::FastWrite(std::ofstream &os, const Value &root) {
    if(!root.m_name) throw STD::logic_error("element name is empty");
    os << "<" << *root.m_name;
    if(root.m_attrs)
        for(auto &it : *root.m_attrs)
            os << ' ' << it.first << "=\"" << (const char*)it.second << "\"";
    os << ">";

    if(root.m_text) m_ss << *root.m_text;

    if(root.m_child)
        for(auto &it : *root.m_child) FastWrite(os,it);

    os << "</" << *root.m_name << ">";
}

void Write::StyleWrite(std::ofstream &os, const Value &root) {
    if(!root.m_name) throw STD::logic_error("element name is empty");
    if(m_level != 0) os << "\n" << space(m_level);
    os << "<" << *root.m_name;
    if(root.m_attrs)
        for(auto &it : *root.m_attrs)          //(const char*)it.second 会调用 operator const char*() 函数
            os << ' ' << it.first << "=\"" << (const char*)it.second << "\"";
    os << ">";

    if(root.m_text) os << *root.m_text;

    if(root.m_child)
        for(auto &it : *root.m_child) {
            m_level++;
            StyleWrite(os,it);
            m_level--;
        }

    if(root.m_child && !root.m_child->empty()) os << "\n" << space(m_level);
    os << "</" << *root.m_name << ">";
}

void Write::declaration() {
    m_ss << "<?xml encoding=\"UTF-8\"?>" << std::endl;
}

void Write::declaration(std::ofstream &os) {
    os << "<?xml encoding=\"UTF-8\"?>" << std::endl;
}

STD::string Write::space(int level) {
    STD::string str;
    for(int i = 0;i < level;i++) str += '\t';
    return str;
}


void Write::StyleWrite(const Value &root) {
    if(!root.m_name) throw STD::logic_error("element name is empty");
    if(m_level != 0) m_ss << '\n' << space(m_level);
    m_ss << "<" << *root.m_name;
    if(root.m_attrs)
        for(auto &it : *root.m_attrs)          //(const char*)it.second 会调用 operator const char*() 函数
            m_ss << " " << it.first << "=\"" << (const char*)it.second << "\"";
    m_ss << ">";

    if(root.m_text) m_ss << *root.m_text;

    if(root.m_child)
        for(auto &it : *root.m_child) {
            m_level++;
            StyleWrite(it);
            m_level--;
        }

    if(root.m_child && !root.m_child->empty()) m_ss << '\n' << space(m_level);
    m_ss << "</" << *root.m_name << ">";
}



}
