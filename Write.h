#ifndef WRITE_H
#define WRITE_H
/**********************************
  解析Value对象 输出Xml格式字符串
  FastWirte  无格式 快速输出
  StyleWrite 有格式 输出较慢
***********************************/
#include "Value.h"
#include <sstream>
#include <fstream>

namespace Xml {
class Write
{
public:
    Write();
    /** @brief 无格式快速写入*/
    STD::string fastWrite(const Value& root);
    /** @brief 格式写入*/
    STD::string styleWrite(const Value& root);
    /** @brief 无格式快速写入 文件*/
    bool fasetWirteFile(const Value& root, const STD::string& filePath,STD::ios_base::openmode mode);
    /** @brief 格式写入文件*/
    bool styleWirteFile(const Value& root,const STD::string& filePath,STD::ios_base::openmode mode);

private:
    void FastWrite(const Value& root);
    void StyleWrite(const Value& root);
    void FastWrite(STD::ofstream& os,const Value& root);
    void StyleWrite(STD::ofstream& os,const Value& root);
    void declaration();
    void declaration(STD::ofstream& os);
    STD::string space(int level);

    STD::stringstream m_ss;
    int m_level;
};

}
#endif // WRITE_H
