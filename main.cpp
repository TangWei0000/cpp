#include "Value.h"
#include "Write.h"
#include "Reader.h"
#include <iostream>
#include <ctime>
/***********************************
1000 * 500  50万 个xml节点对象
测试结果如下:

不用移动语义构造 unMove run 0.991s      占用内存500M左右

使用move移动语义构造 move run 0.511s    占用内存200M左右

使用无格式写入文件 fasetWriteFile run 0.655s 占用磁盘19.1M

使用有格式写入文件 styleWriteFile run 0.702s 占用磁盘21.0M

读取xml文件并解析字符串 parse run 0.577s

单独解析字符串 parse run 0.074s

*************************************/

void testFileRead(const char* filePath) {
    std::clock_t starTime,endTime1,endTime2;
    Xml::Reader read;
    starTime = std::clock();
    if(read.loadFile(filePath)){
       endTime1 = std::clock();
       std::cout << "---------------文件读取完成----------------------" << std::endl;
       Xml::Value root = read.parse();
       endTime2 = std::clock();
       std::cout << "open file parse run time is :" << (double)(endTime2 - starTime) / CLOCKS_PER_SEC << "s"<< std::endl;
       std::cout << "parse run time is :" << (double)(endTime1 - starTime) / CLOCKS_PER_SEC << "s"<< std::endl;
       std::cout << "---------------文件解析完成----------------------" << std::endl;
    }

}

void testFileWrite(Xml::Value& root,const char* filePath1,const char* filePath2){
    std::clock_t starTime,endTime;
    starTime = std::clock();                            // out 覆盖 app 追加
    if(Xml::Write().fasetWirteFile(root,filePath1,std::ios_base::out)) {
        endTime = std::clock();
        std::cout << "---------------文件写入成功----------------------" << std::endl;
        std::cout << "fasetWriteFile run time is :" << (double)(endTime - starTime) / CLOCKS_PER_SEC << "s"<< std::endl;
    } else {
        std::cout << "---------------文件写入失败----------------------" << std::endl;
    }

    starTime = std::clock();                        // out 覆盖 app 追加
    if(Xml::Write().styleWirteFile(root,filePath2,std::ios_base::out)) {
        endTime = std::clock();
        std::cout << "---------------文件写入成功----------------------" << std::endl;
        std::cout << "styleWriteFile run time is :" << (double)(endTime - starTime) / CLOCKS_PER_SEC << "s"<< std::endl;
    } else {
        std::cout << "---------------文件写入失败----------------------" << std::endl;
    }
}


void test_move() {
    std::clock_t starTime,endTime;
    starTime = std::clock();
    Xml::Value root("action");
    for(int i = 0;i < 1000;i++) {
        Xml::Value v1("class");
        v1.attr("name","student");
        v1.attr("id",i);
        for(int j = 0;j < 500; j++) {
            Xml::Value v2("student");
            v2.attr("id",i + j);
            v2.attr("off",i < j);
            v1.append(std::move(v2));
        }
        root.append(std::move(v1));
    }
    endTime = std::clock();

    std::cout << "move run time is :" << (double)(endTime - starTime) / CLOCKS_PER_SEC << "s"<< std::endl;
    std::cout << "------------------------------------------" << std::endl;


    // 测试文件 路径 自己选择
    testFileWrite(root,"F:/student1.xml","F:/student2.xml");
    testFileRead("F:/student2.xml");
}

void test_unMove() {
    std::clock_t starTime,endTime;
    starTime = std::clock();
    Xml::Value root("action");
    for(int i = 0;i < 1000;i++) {
        Xml::Value v1("class");
        v1.attr("name","student");
        v1.attr("id",i);
        for(int j = 0;j < 500; j++) {
            Xml::Value v2("student");
            v2.attr("id",i + j);
            v2.attr("off",i < j);
            v1.append(v2);
        }
        root.append(v1);
    }
    endTime = std::clock();
    std::cout << "unMove run time is :" << (double)(endTime - starTime) / CLOCKS_PER_SEC << "s"<< std::endl;
    std::cout << "------------------------------------------" << std::endl;
}


/** 移动语义测试 */
void testMove(){
    class A {
    public:
        A() {}
        A(const A& ) {
            std::cout << "拷贝构造" << std::endl;
        }
        A(A&& ){
            std::cout << "移动构造" << std::endl;
        }
        A& operator=(const A& ) {
            std::cout << "拷贝赋值" << std::endl;
            return *this;
        }
        A& operator=(A&& ) {
            std::cout << "移动赋值" << std::endl;
            return *this;
        }
    };

    std::list<A> ls;
    A a,b,c;
    std::cout << "---------c++11 新特性 移动语义测试----------" << std::endl;
    ls.push_back(a);
    ls.push_back(a);
    ls.push_back(a);
    std::cout << "-----------------------" << std::endl;
    ls.push_back(std::move(a));
    ls.push_back(std::move(a));
    ls.push_back(std::move(a));
    std::cout << "-----------------------" << std::endl;
    a = b;
    a = c;
    std::cout << "-----------------------" << std::endl;
    a = std::move(b);
    a = std::move(c);
}


int main(int , char **)
{
    test_unMove();
    test_move();

    std::cout << "------END------" << std::endl;
}
