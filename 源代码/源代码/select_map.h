//
// Created by 问天 on 2023/5/14.
//

#ifndef C___SELECT_MAP_H
#define C___SELECT_MAP_H
#include <map>
#include "Table.h"
using namespace std;

bool cmp( pair<string, int>& s1,  pair<string, int>& s2)
{
    double num1,num2;
    // 找到 s1 中第一个数字的位置
    size_t pos1 = s1.first.find_first_of("0123456789");
    // 将 s1 中的数字部分转换为整数
    if(s1.first.find_first_of("0123456789")!=string::npos) num1 = std::stod(s1.first.substr(pos1));

    // 找到 s2 中第一个数字的位置
    size_t pos2 = s2.first.find_first_of("0123456789");
    // 将 s2 中的数字部分转换为整数
    if(s2.first.find_first_of("0123456789")!=string::npos) num2 = std::stod(s2.first.substr(pos2));

    // 比较转换后的数字大小
    return num1 < num2;
}
multimap<double, int> Sort(multimap<string, int> &find)
{
    vector<pair<string, int>> vec;
    vector<pair<double, int>> v;
    multimap<double, int>f;
    for (auto it = find.begin(); it != find.end(); ++it)
        vec.push_back(*it);

    // 对 vector 中的元素按 value 进行排序
    sort(vec.begin(), vec.end(), cmp);
    for (auto it : vec)
        v.push_back(make_pair(stod(it.first),it.second));
    // 将 vector 中的元素重新插入到 multimap 容器中
    find.clear();
    for (auto it = v.begin(); it != v.end(); ++it)
        f.insert(*it);

    // 输出排序后的 multimap 中的元素
    return f;
}
vector<int> Find_primary_key(multimap<string, int>find,string range1 ,string range2,string type)
{
    if(type=="int"||type=="intprimarykey"||type=="double")
    {
        multimap<double, int>find1=Sort(find);
        vector<int> back;
        if (range2 == "up")
        {
            auto it_start = find1.lower_bound(stod(range1));
            auto it_end = find1.end();
            for (auto it = it_start; it != it_end; ++it)
            {
                back.push_back(it->second);
            }
        } else if (range2 == "down")
        {
            auto it_start = find1.begin();
            auto it_end = find1.upper_bound(stod(range1));
            for (auto it = it_start; it != it_end; ++it)
            {
                back.push_back(it->second);
            }
        } else
        {
            auto it_start = find1.lower_bound(stod(range1));
            auto it_end = find1.upper_bound(stod(range2));
            for (auto it = it_start; it != it_end; ++it)
            {
                back.push_back(it->second);
            }
        }
        return back;
    }
    else
    {
        vector<int> back;
        auto it_start = find.lower_bound(range1);
        auto it_end = find.upper_bound(range2);
        for (auto it = it_start; it != it_end; ++it)
        {
            back.push_back(it->second);
        }
        return back;
    }
}
void Create_map(multimap<string, int>& find,int type_num,int key_num,
                ifstream& in ,int size ,int data_size,vector<string> type)
{
    while(data_size>=size-1)
    {
        bool exist = 0;
        int key;
        string v;
        int point =in.tellg();
        in.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        if (!exist)
        {
            in.seekg(point+size);
            data_size -= size;
            continue;
        }
        for (int i = 0; i < type.size(); i++)
        {
            if (type[i] == "int" || type[i] == "intprimarykey")
            {
                int j;
                in.read(reinterpret_cast<char *>( &j ), sizeof(int));
                if(i==key_num)
                    key=j;
                if(i==type_num)
                {
                    v=to_string(j);
                }

            } else if (type[i] == "double")
            {
                double j;
                in.read(reinterpret_cast<char *>( &j ), sizeof(double));
                if(i==type_num)
                {
                    v=to_string(j);
                }
            } else
            {
                char name[20];
                in.read(name, 20);
                if(i==type_num)
                {
                    v=name;
                }
            }
        }
        find.insert(pair<string,int>(v,key));
        data_size-=size;
    }
}
void Create_map_without_primary_key(multimap<string, int>& find,int type_num,int key_num,ifstream& in ,
                                    BPlus_tree<int,int> primary_key_tree ,int size ,int data_size,vector<string> type)
{
    while(data_size>=size-1)
    {
        bool exist = 0;
        int key;
        string v;
        int point =in.tellg();
        in.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        if (!exist)
        {
            in.seekg(point+size);
            data_size -= size;
            continue;
        }
        for (int i = 0; i < type.size(); i++)
        {
            if (type[i] == "int" )
            {
                int j;
                in.read(reinterpret_cast<char *>( &j ), sizeof(int));
                if(i==key_num)
                {
                    key=primary_key_tree.search_value(j);
                }
                if(i==type_num)
                {
                    v=to_string(j);
                }

            } else if (type[i] == "double")
            {
                double j;
                in.read(reinterpret_cast<char *>( &j ), sizeof(double));
                if(i==type_num)
                {
                    v=to_string(j);
                }
            } else
            {
                char name[20];
                in.read(name, 20);
                if(i==type_num)
                {
                    v=name;
                }
            }
        }
        find.insert(pair<string,int>(v,key));
        data_size-=size;
    }
}

#endif //C___SELECT_MAP_H
