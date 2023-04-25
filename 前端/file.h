//
// Created by 问天 on 2023/4/23.
//

#ifndef C___FILE_H
#define C___FILE_H
#include <string>
#include <io.h>
#include <direct.h>
#include <Windows.h>
using namespace std;
void CreatMkdir(string Mkdir)
{
    char  buf[1000];
    GetCurrentDirectory(1000, buf);
    string LoadTestPath =buf;
    LoadTestPath+=Mkdir;
    if (_access(LoadTestPath.c_str(), 0) == -1)
    {
        _mkdir(LoadTestPath.c_str());
    }
}
#endif //C___FILE_H
