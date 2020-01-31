#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;
//.frag .vert文件读取
char* readTheFile(string strSource) {
    std::ifstream myfile(strSource);
    //打开失败
    if (myfile.fail()) {
        std::cout << "Can not open it " << std::endl;
    }
    //转换成字符串
    std::string str((std::istreambuf_iterator<char>(myfile)),
        std::istreambuf_iterator<char>());
    //关闭文件处理器
    myfile.close();
    //str数组长度一定要 +1,
    int len = str.length();
    char* result = new char[len+1];
    strcpy(result, str.c_str());
    return result;
}