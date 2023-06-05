//
// Created by 问天 on 2023/5/14.
//

#ifndef C___TYPETREE_H
#define C___TYPETREE_H
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

string out_data(vector<string> type,ifstream &in1_data)//语法树（数据类型）
{
    string value;
    for(int i=0;i<type.size();i++)
    {
        if(type[i]=="int" || type[i]=="intprimarykey")
        {
            int j;
            in1_data.read(reinterpret_cast<char *>( &j ), sizeof( int ));
            value+= to_string(j)+" ";
        }
        else if(type[i]=="double")
        {
            double j;
            in1_data.read(reinterpret_cast<char *>( &j ), sizeof( double ));
            value+= to_string(j)+" ";
        }
        else
        {
            char name[20];
            in1_data.read(name,20);
            string x=name;
            value+= x+" ";
        }
    }
    return value;
}

#endif //C___TYPETREE_H
