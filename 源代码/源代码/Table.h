//
// Created by 问天 on 2023/5/5.
//

#ifndef C___TABLE_H
#define C___TABLE_H
#include <vector>
#include <string>
#include "bplus_tree.h"
#include "select_map.h"
using namespace std;

struct index
{
    int num=-1;//副索引表个数
    pair<string ,int> member_name[10];//与主键建立的索引表 成员名 与 参数
    bool exist_map[10]={0};//是否生成map
    multimap<string, int> find[10];
    BPlus_tree<int,int> primary_key_tree;//与隐藏主键的树
    bool if_tree=0;
};
class Table
{
public:
    Table(){};
    Table(string a) : name(a){};
    index Index;//索引
    vector<string> member;//成员名称
    string name;//table 名字
    pair<string, int> primary_key;//string:主键名称 int:位于member的第几位
    int key =1;//隐藏主键值
    BPlus_tree<int,int> primary_key_tree;
    bool if_primarykey=0;//是否有主键
    bool if_tree=0;//是否有主键树
    bool exist=1;//是否被删除
    bool if_read =0;//是否被读取

    string member_name()
    {
        string member_name="";
        for(int i=0;i<member.size();i++)
        {
            string temp="";
            for(int j=0;j<member[i].size();j++)
            {
                if(member[i][j]==' ')
                {
                    member_name+=temp;
                    break;
                }
                else temp+=member[i][j];
            }
            member_name+=' ';
        }
        return member_name;
    }
    string member_type()//把type合并为一个字符串，不同类型以空格隔开
    {
        string member_type="";
        for(int i=0;i<member.size();i++)
        {
            string temp="";
            bool b=0;
            for(int j=0;j<member[i].size();j++)
            {
                if(member[i][j]==' ')
                {
                    b=1;
                }
                else
                    if(b) temp+=member[i][j];
            }
            member_type+=temp;
            member_type+=' ';
        }
        member_type+='\0';
        return member_type;
    }
    vector<string> type(string member_type)
    {
        vector<string> type;
        string temp="";
        for(int i=0;i<member_type.size();i++)
        {
            if(member_type[i]==' ')
            {
                type.push_back(temp);
                temp="";
            }
            else temp+=member_type[i];
        }
        return type;
    }
    bool if_primary(vector<string> type,vector<string> name1)
    {
        for(int i=0;i<type.size();i++)
        {
            if(type[i].find("primarykey")!=string::npos)
            {
                primary_key=make_pair(name1[i],i);
                return 1;
            }
        }
        return 0;
    }
};

#endif //C___TABLE_H
