//
// Created by 问天 on 2023/4/23.
//

#ifndef C___FILE_H
#define C___FILE_H

#include <string>
#include <io.h>
#include <direct.h>
#include <Windows.h>
#include <utility>
#include <filesystem>
#include "Table.h"
#include "typetree.h"
#include "Lexer.h"
#include "select_one.h"

using namespace std;

void Line(int x)
{
    cout<<"->";
    int xx=12+11*(x-1);
    for(int i=1;i<=xx;i++) cout<<"-";
    cout<<endl;
}

bool is_number(const std::string& str)
{
    try
    {
        double d = std::stod(str);
        return true;
    }
    catch (const std::invalid_argument&)
    {
        return false;
    }
}

vector<string> type(string member_type)
{
    vector<string> type;
    string temp = "";
    for (int i = 0; i < member_type.size(); i++)
    {
        if (member_type[i] == ' ')
        {
            type.push_back(temp);
            temp = "";
        } else temp += member_type[i];
    }
    return type;
}

void ChangeDatabase(string Mkdir, vector<Table> &table,char* buf)//改变路径并使用数据库
{
    chdir(buf);
    fstream database("Databases.dat", ios::in | ios::app | ios::binary);
    int size = 21;
    database.seekg(0, ios::end);
    int data_size = database.tellg();
    database.seekg(0, ios::beg);
    bool exist,if1=0;
    char name[20];
    while (data_size >= size - 1)
    {
        int point = database.tellg();
        database.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        if (!exist)
        {
            database.seekg(point + size);
            data_size -= size;
            continue;
        }
        database.read(name, 20);
        if(name==Mkdir)
        {
            if1=1;
            break;
        }
        data_size -= size;
    }
    database.close();
    if(!if1)
    {
        cout<<"->Not Find Such Database!"<<endl;
        return;
    }
    string LoadTestPath = buf;
    LoadTestPath += "\\";
    LoadTestPath += Mkdir;
    chdir(LoadTestPath.c_str());
    table.clear();
    ifstream in(Mkdir + ".dat", ios::in | ios::binary);
    int size1 = 21;
    in.seekg(0, ios::end);
    int data_size1 = in.tellg();
    in.seekg(0, ios::beg);
    while (data_size1 >= size1 - 1)
    {
        int point = in.tellg();
        in.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        in.read(name, 20);
        if (exist)
        {
            Table t(name);
            table.push_back(t);
        } else
        {
            in.seekg(point + size1);
            data_size1 -= size1;
            continue;
        }
        data_size1 -= size1;
    }
    in.close();
}

void CreateMkdir(string Mkdir)//创建数据库
{
    fstream database("Databases.dat", ios::in | ios::app | ios::binary);
    char buf[1000];
    GetCurrentDirectory(1000, buf);
    string LoadTestPath = buf;
    LoadTestPath += "\\";
    LoadTestPath += Mkdir;
    if (_access(LoadTestPath.c_str(), 0) == -1)
    {
        _mkdir(LoadTestPath.c_str());
    }
    bool exist = 1;
    char name[20];
    database.seekg(0, ios::end);
    strcpy(name, Mkdir.c_str());
    database.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
    database.write(name, 20);
    database.close();
    ofstream out(Mkdir + ".dat", ios::out | ios::binary);
    out.close();
}

void ShowDatabase(char *buff)
{
    Line(1);
    cout << "->|" << left << setw(10) << "Databases" << "|" << endl;
    Line(1);
    char buf[1000];
    GetCurrentDirectory(1000, buf);
    chdir(buff);
    fstream database("Databases.dat", ios::in | ios::app | ios::binary);
    int size = 21;
    database.seekg(0, ios::end);
    int data_size = database.tellg();
    database.seekg(0, ios::beg);
    bool exist;
    while (data_size >= size - 1)
    {
        int point = database.tellg();
        database.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        if (!exist)
        {
            database.seekg(point + size);
            data_size -= size;
            continue;
        }
        char name[20];
        database.read(name, 20);
        cout << "->|" << left << setw(10) << name << "|" << endl;
        data_size -= size;
    }
    Line(1);
    database.close();
    chdir(buf);
}//展示数据库

void DescribeTable(Table t)
{
    string a;
    ifstream in1(t.name + ".sql", ios::in);
    Line(3);
    cout << "->|" << left << setw(10) << "Field";
    cout << "|" << left << setw(10) << "Type";
    cout << "|" << left << setw(10) << "Key" << "|" << endl;
    Line(3);
    getline(in1, a);
    while (getline(in1, a))
    {
        cout << "->|";
        int b;
        bool v = 0;
        string temp = "";
        for (int i = 0; i < a.size(); i++)
            if (a[i] == ' ')
            {
                b = i;
                break;
            }
        for (int i = 0; i < b; i++)
        {
            temp += a[i];
        }
        cout << left << setw(10) << temp;
        temp = "";
        for (int i = b + 1; i < a.size(); i++)
        {
            if (a[i] == 'p' && a[i + 1] == 'r')
            {
                v = 1;
                break;
            }
            temp += a[i];
        }
        cout << "|" << left << setw(10) << temp;
        if (v) cout << "|" << left << setw(10) << "PRI" << "|";
        else cout << "|" << left << setw(10) << "" << "|";
        cout << endl;
    }
    Line(3);
}//表细节

Table CreateTable(string table_name, int num, vector<string> member, string database,bool if_database)//建立新表
{
    Table t;
    if(!if_database)
    {
        cout<<"->Please Use a Database first!\n";
        return t;
    }
    bool if1=0;
    for(auto i: member)
    {
        vector<string> a= type(i);
        if(a[1]=="int"||a[1]=="intprimarykey")
        {
            if1=1;
            break;
        }
    }
    if(!if1)
    {
        cout<<"->Must Create a Table with a int Data!\n";
        return t;
    }
    t.name = table_name;
    char name[20];
    strcpy(name, table_name.c_str());
    for (int i = 0; i < num; i++)
        t.member.push_back(member[i]);
    ofstream out(t.name + ".sql", ios::out);//顺序文件
    ofstream out_data(t.name + "_data.sql", ios::out | ios::binary);
    ofstream out_index(t.name + "_index.sql", ios::out | ios::binary);
    ofstream out_database(database + ".dat", ios::app | ios::binary);
    out << t.name << endl;
    for (int i = 0; i < num; i++)
    {
        out << t.member[i] << endl;
    }
    bool b = 1;
    out_database.write(reinterpret_cast<char *>( &b ), sizeof(bool));
    out_database.write(name, 20);
    b = 0;
    out.close();
    out_index.close();
    out_data.close();
    out_database.close();
    t.if_primarykey = t.if_primary(t.type(t.member_type()), t.type(t.member_name()));
    if (!t.if_primarykey)
    {
        vector<string> type = t.type(t.member_type());
        for (int j = 0; j < type.size(); j++)
        {
            if (type[j] == "int")
            {
                t.Index.num++;
                t.Index.member_name[t.Index.num] = make_pair("int", j);
                ofstream out_index2(t.name + "_index_0" + ".sql", ios::out | ios::binary);
                int key = t.key;
                out_index2.write(reinterpret_cast<char *>( &key ), sizeof(int));
                out_index2.close();
                break;
            }
        }
    }
    t.if_read = 1;
    return t;
}

void ShowTable(string database)
{
    Line(1);
    cout << "->|" << left << setw(10) << "Tables" << "|" << endl;
    Line(1);
    char name[20];
    bool exist;
    ifstream in(database + ".dat", ios::in | ios::binary);
    int size = 21;
    in.seekg(0, ios::end);
    int data_size = in.tellg();
    in.seekg(0, ios::beg);
    while (data_size >= size - 1)
    {
        int point = in.tellg();
        in.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        in.read(name, 20);
        if (exist)
        {
            cout << "->|" << left << setw(10) << name << "|" << endl;
        } else
        {
            in.seekg(point + size);
            data_size -= size;
            continue;
        }
        data_size -= size;
    }
    in.close();
    Line(1);
}//展示表

pair<string, vector<string>> SelectFile(Table &t, string member_name, string range1, string range2)
{
    ifstream in1_data(t.name + "_data.sql", ios::in | ios::binary);
    ifstream in1_index(t.name + "_index.sql", ios::in | ios::binary);
    vector<string> name1 = t.type(t.member_name());//成员名称
    vector<string> type = t.type(t.member_type());//成员类型
    vector<string> Value;
    string value;
    bool exist = 0;
    pair<string, vector<string>> back;
    //cout << "->";
    in1_data.seekg(0, ios::end);
    int data_size = in1_data.tellg();
    in1_data.seekg(0, ios::beg);
    int size = 1;
    for (auto i: type)
    {
        if (i == "int" || i == "intprimarykey") size += sizeof(int);
        else if (i == "double") size += sizeof(double);
        else size += 20;
    }
    if (member_name == "" && range1 == "" && range2 == "")//select *
    {

        //语法树（数据类型）
        //现在默认(int, char[20], double)
        while (data_size >= size - 1)
        {
            //cout << "->";
            exist = 0;
            int point = in1_data.tellg();
            in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
            if (!exist)
            {
                in1_data.seekg(point + size);
                data_size -= size;
                continue;
            }
            value = out_data(type, in1_data);
            data_size -= size;
            //cout << endl;
            Value.push_back(value);
        }
        back = make_pair(t.member_name(), Value);
        in1_data.close();
        in1_index.close();
        return back;
    } else//有附加条件:id=..
    {
        int type_num;
        bool if1=0;
        for (int i = 0; i <= name1.size(); i++)
        {
            if (name1[i] == member_name)
            {
                type_num = i;
                if1=1;
                break;
            }
        }
        if(!if1)
        {
            cout<<"->Not Find Such List!\n";
            string a;
            vector<string> b;
            return make_pair(a,b);
        }
        if (t.if_primarykey)//主键索引
        {
            if (type_num == t.primary_key.second)//附加条件是主键
            {
                int select_value = stoi(range1);
                if (t.if_tree)
                {
                    vector<int> point;
                    if (range2 == "up")
                    {
                        bool b = 1;
                        point = t.primary_key_tree.search_range(select_value, b);
                    } else if (range2 == "down")
                    {
                        bool b = 0;
                        point = t.primary_key_tree.search_range(select_value, b);
                    } else
                    {
                        int select_value2 = stoi(range2);
                        point = t.primary_key_tree.search_range(select_value, select_value2);
                    }
                    for (auto i: point)
                    {
                        in1_data.seekg(i);
                        //cout << "->";
                        exist = 0;
                        in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                        if (!exist)
                        {
                            //cout << "Not Found" << endl;
                            continue;
                        }
                        value = out_data(type, in1_data);
                        //cout << endl;
                        Value.push_back(value);
                    }
                    back = make_pair(t.member_name(), Value);
                    in1_data.close();
                    in1_index.close();
                    return back;
                } else//没有树
                {
                    int size_index = 9;
                    in1_index.seekg(0, ios::end);
                    int index_size = in1_index.tellg();
                    in1_index.seekg(0, ios::beg);
                    exist = 0;
                    while (index_size >= size_index - 1)
                    {
                        in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                        int point = in1_index.tellg();
                        if (!exist)
                        {
                            index_size -= size_index;
                            in1_index.seekg(point + 8);
                            //cout << "Not Found" << endl;
                            continue;
                        }
                        int key, va;
                        in1_index.read(reinterpret_cast<char *>( &key ), sizeof(int));
                        in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                        t.primary_key_tree.insert_value(key, va);
                        index_size -= size_index;
                    }
                    t.if_tree = 1;
                    vector<int> point;
                    if (range2 == "up")
                    {
                        bool b = 1;
                        point = t.primary_key_tree.search_range(select_value, b);
                    } else if (range2 == "down")
                    {
                        bool b = 0;
                        point = t.primary_key_tree.search_range(select_value, b);
                    } else
                    {
                        int select_value2 = stoi(range2);
                        point = t.primary_key_tree.search_range(select_value, select_value2);
                    }
                    for (auto i: point)
                    {
                        in1_data.seekg(i);
                        //cout << "->";
                        exist = 0;
                        in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                        if (!exist)
                        {
                            //cout << "Not Found" << endl;
                            continue;
                        }
                        value = out_data(type, in1_data);
                        //cout << endl;
                        Value.push_back(value);
                    }
                    back = make_pair(t.member_name(), Value);
                    in1_data.close();
                    in1_index.close();
                    return back;
                }
            } else//select非主键
            {
                for (int i = 0; i <= t.Index.num; i++)//查找是否生成副索引
                {
                    if (type_num == t.Index.member_name[i].second)//找到副索引
                    {
                        if (t.Index.exist_map[i])
                        {
                            vector<int> point;
                            int x=t.Index.member_name[i].second;
                            vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                            for (auto item: range)
                            {
                                point.push_back(t.primary_key_tree.search_value(item));
                            }
                            for (auto item: point)
                            {
                                in1_data.seekg(item);
                                //cout << "->";
                                exist = 0;
                                in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                                if (!exist)
                                {
                                    //cout << "Not Found" << endl;
                                    continue;
                                }
                                value = out_data(type, in1_data);
                                //cout << endl;
                                Value.push_back(value);
                            }
                            back = make_pair(t.member_name(), Value);
                            in1_data.close();
                            in1_index.close();
                            return back;
                        } else//不存在map
                        {
                            Create_map(t.Index.find[t.Index.num], type_num, t.primary_key.second,
                                       in1_data, size, data_size, type);
                            t.Index.exist_map[t.Index.num] = 1;
                            vector<int> point;
                            int x=t.Index.member_name[i].second;
                            vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                            for (auto item: range)
                            {
                                point.push_back(t.primary_key_tree.search_value(item));
                            }
                            for (auto item: point)
                            {
                                in1_data.seekg(item);
                                //cout << "->";
                                exist = 0;
                                in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                                if (!exist)
                                {
                                    //cout << "Not Found" << endl;
                                    continue;
                                }
                                value = out_data(type, in1_data);
                                //cout << endl;
                                Value.push_back(value);
                            }
                            back = make_pair(t.member_name(), Value);
                            in1_data.close();
                            in1_index.close();
                            return back;
                        }
                    }
                }
                //没找到副表
                t.Index.num++;
                t.Index.member_name[t.Index.num] = make_pair(member_name, type_num);
                Create_map(t.Index.find[t.Index.num], type_num, t.primary_key.second,
                           in1_data, size, data_size, type);
                t.Index.exist_map[t.Index.num] = 1;
                vector<int> point;
                int x=t.Index.member_name[t.Index.num].second;
                vector<int> range = Find_primary_key(t.Index.find[t.Index.num], range1, range2,type[x]);
                for (auto item: range)
                {
                    if (t.if_tree)
                    {
                        point.push_back(t.primary_key_tree.search_value(item));
                    } else
                    {
                        int size_index = 9;
                        in1_index.seekg(0, ios::end);
                        int index_size = in1_index.tellg();
                        in1_index.seekg(0, ios::beg);
                        exist = 0;
                        while (index_size >= size_index - 1)
                        {
                            in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                            int point1 = in1_index.tellg();
                            if (!exist)
                            {
                                index_size -= size_index;
                                in1_index.seekg(point1 + 8);
                                //cout << "Not Found" << endl;
                                continue;
                            }
                            int key, va;
                            in1_index.read(reinterpret_cast<char *>( &key ), sizeof(int));
                            in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                            t.primary_key_tree.insert_value(key, va);
                            index_size -= size_index;
                        }
                        t.if_tree = 1;
                        point.push_back(t.primary_key_tree.search_value(item));
                    }
                }
                for (auto item: point)
                {
                    in1_data.seekg(item);
                    //cout << "->";
                    exist = 0;
                    in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    if (!exist)
                    {
                        //cout << "Not Found" << endl;
                        continue;
                    }
                    value = out_data(type, in1_data);
                    //cout << endl;
                    Value.push_back(value);
                }
                back = make_pair(t.member_name(), Value);
                in1_data.close();
                in1_index.close();
                return back;
            }
        } else//没有主键
        {
            if (t.Index.if_tree)
            {
                for (int i = 1; i <= t.Index.num; i++)//查找是否生成副索引
                {
                    if (type_num == t.Index.member_name[i].second)//找到副索引
                    {
                        if (t.Index.exist_map[i])
                        {
                            vector<int> point;
                            int x=t.Index.member_name[i].second;
                            vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                            for (auto item: range)
                            {
                                point.push_back(t.primary_key_tree.search_value(item));
                            }
                            for (auto item: point)
                            {
                                in1_data.seekg(item);
                                //cout << "->";
                                exist = 0;
                                in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                                if (!exist)
                                {
                                    //cout << "Not Found" << endl;
                                    continue;
                                }
                                value = out_data(type, in1_data);
                                //cout << endl;
                                Value.push_back(value);
                            }
                            back = make_pair(t.member_name(), Value);
                            in1_data.close();
                            in1_index.close();
                            return back;
                        } else//不存在map
                        {
                            Create_map_without_primary_key(t.Index.find[t.Index.num], type_num,
                                                           t.Index.member_name[0].second,
                                                           in1_data, t.Index.primary_key_tree, size, data_size, type);
                            t.Index.exist_map[t.Index.num] = 1;
                            vector<int> point;
                            int x=t.Index.member_name[i].second;
                            vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                            for (auto item: range)
                            {
                                point.push_back(t.primary_key_tree.search_value(item));
                            }
                            for (auto item: point)
                            {
                                in1_data.seekg(item);
                                //cout << "->";
                                exist = 0;
                                in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                                if (!exist)
                                {
                                    //cout << "Not Found" << endl;
                                    continue;
                                }
                                value = out_data(type, in1_data);
                                //cout << endl;
                                Value.push_back(value);
                            }
                            back = make_pair(t.member_name(), Value);
                            in1_data.close();
                            in1_index.close();
                            return back;
                        }
                    }
                }
                //没找到副表
                t.Index.num++;
                t.Index.member_name[t.Index.num] = make_pair(member_name, type_num);
                Create_map_without_primary_key(t.Index.find[t.Index.num], type_num, t.Index.member_name[0].second,
                                               in1_data, t.Index.primary_key_tree, size, data_size, type);
                t.Index.exist_map[t.Index.num] = 1;
                vector<int> point;
                int x=t.Index.member_name[t.Index.num].second;
                vector<int> range = Find_primary_key(t.Index.find[t.Index.num], range1, range2,type[x]);
                for (auto item: range)
                {
                    if (t.if_tree)
                    {
                        point.push_back(t.primary_key_tree.search_value(item));
                    } else
                    {
                        int size_index = 9;
                        in1_index.seekg(0, ios::end);
                        int index_size = in1_index.tellg();
                        in1_index.seekg(0, ios::beg);
                        exist = 0;
                        while (index_size >= size_index - 1)
                        {
                            in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                            int point1 = in1_index.tellg();
                            if (!exist)
                            {
                                index_size -= size_index;
                                in1_index.seekg(point1 + 8);
                                //cout << "Not Found" << endl;
                                continue;
                            }
                            int key, va;
                            in1_index.read(reinterpret_cast<char *>( &key ), sizeof(int));
                            in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                            t.primary_key_tree.insert_value(key, va);
                            index_size -= size_index;
                        }
                        t.if_tree = 1;
                        point.push_back(t.primary_key_tree.search_value(item));
                    }
                }
                for (auto item: point)
                {
                    in1_data.seekg(item);
                    //cout << "->";
                    exist = 0;
                    in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    if (!exist)
                    {
                        //cout << "Not Found" << endl;
                        continue;
                    }
                    value = out_data(type, in1_data);
                    //cout << endl;
                    Value.push_back(value);
                }
                back = make_pair(t.member_name(), Value);
                in1_data.close();
                in1_index.close();
                return back;
            } else//隐藏主键未生成树
            {
                ifstream in1_index2(t.name + "_index_0" + ".sql", ios::in | ios::binary);
                int size_index1 = 9;
                in1_index2.seekg(0, ios::end);
                int index_size1 = in1_index2.tellg();
                in1_index2.seekg(0, ios::beg);
                index_size1 -= 4;
                in1_index2.read(reinterpret_cast<char *>( &t.key ), sizeof(int));
                while (index_size1 >= size_index1 - 1)
                {
                    in1_index2.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    int point = in1_index2.tellg();
                    if (!exist)
                    {
                        index_size1 -= size_index1;
                        in1_index.seekg(point + 8);
                        //cout << "Not Found" << endl;
                        continue;
                    }
                    int key, va;
                    in1_index2.read(reinterpret_cast<char *>( &key ), sizeof(int));
                    in1_index2.read(reinterpret_cast<char *>( &va ), sizeof(int));
                    t.Index.primary_key_tree.insert_value(key, va);
                    index_size1 -= size_index1;
                }
                in1_index2.close();
                t.Index.if_tree = 1;
                for (int i = 1; i <= t.Index.num; i++)//查找是否生成副索引
                {
                    if (type_num == t.Index.member_name[i].second)//找到副索引
                    {
                        if (t.Index.exist_map[i])
                        {
                            vector<int> point;
                            int x=t.Index.member_name[i].second;
                            vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                            for (auto item: range)
                            {
                                point.push_back(t.primary_key_tree.search_value(item));
                            }
                            for (auto item: point)
                            {
                                in1_data.seekg(item);
                                //cout << "->";
                                exist = 0;
                                in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                                if (!exist)
                                {
                                    //cout << "Not Found" << endl;
                                    continue;
                                }
                                value = out_data(type, in1_data);
                                //cout << endl;
                                Value.push_back(value);
                            }
                            back = make_pair(t.member_name(), Value);
                            in1_data.close();
                            in1_index.close();
                            return back;
                        } else//不存在map
                        {
                            Create_map_without_primary_key(t.Index.find[t.Index.num], type_num,
                                                           t.Index.member_name[0].second,
                                                           in1_data, t.Index.primary_key_tree, size, data_size, type);
                            t.Index.exist_map[t.Index.num] = 1;
                            vector<int> point;
                            int x=t.Index.member_name[i].second;
                            vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                            for (auto item: range)
                            {
                                point.push_back(t.primary_key_tree.search_value(item));
                            }
                            for (auto item: point)
                            {
                                in1_data.seekg(item);
                                //cout << "->";
                                exist = 0;
                                in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                                if (!exist)
                                {
                                    //cout << "Not Found" << endl;
                                    continue;
                                }
                                value = out_data(type, in1_data);
                                //cout << endl;
                                Value.push_back(value);
                            }
                            back = make_pair(t.member_name(), Value);
                            in1_data.close();
                            in1_index.close();
                            return back;
                        }
                    }
                }
                //没找到副表
                t.Index.num++;
                t.Index.member_name[t.Index.num] = make_pair(member_name, type_num);
                Create_map_without_primary_key(t.Index.find[t.Index.num], type_num, t.Index.member_name[0].second,
                                               in1_data, t.Index.primary_key_tree, size, data_size, type);
                t.Index.exist_map[t.Index.num] = 1;
                vector<int> point;
                int x=t.Index.member_name[t.Index.num].second;
                vector<int> range = Find_primary_key(t.Index.find[t.Index.num], range1, range2,type[x]);
                for (auto item: range)
                {
                    if (t.if_tree)
                    {
                        point.push_back(t.primary_key_tree.search_value(item));
                    } else
                    {
                        int size_index = 9;
                        in1_index.seekg(0, ios::end);
                        int index_size = in1_index.tellg();
                        in1_index.seekg(0, ios::beg);
                        exist = 0;
                        while (index_size >= size_index - 1)
                        {
                            in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                            int point1 = in1_index.tellg();
                            if (!exist)
                            {
                                index_size -= size_index;
                                in1_index.seekg(point1 + 8);
                                //cout << "Not Found" << endl;
                                continue;
                            }
                            int key, va;
                            in1_index.read(reinterpret_cast<char *>( &key ), sizeof(int));
                            in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                            t.primary_key_tree.insert_value(key, va);
                            index_size -= size_index;
                        }
                        t.if_tree = 1;
                        point.push_back(t.primary_key_tree.search_value(item));
                    }
                }
                for (auto item: point)
                {
                    in1_data.seekg(item);
                    //cout << "->";
                    exist = 0;
                    in1_data.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    if (!exist)
                    {
                        //cout << "Not Found" << endl;
                        continue;
                    }
                    value = out_data(type, in1_data);
                    //cout << endl;
                    Value.push_back(value);
                }
                back = make_pair(t.member_name(), Value);
                in1_data.close();
                in1_index.close();
                return back;
            }
        }
    }

}//查找数据

pair<vector<int>, vector<int>> SelectPoint(Table &t, string member_name, string range1, string range2)
{
    ifstream in1_data(t.name + "_data.sql", ios::in | ios::binary);
    ifstream in1_index(t.name + "_index.sql", ios::in | ios::binary);
    vector<string> name1 = t.type(t.member_name());//成员名称
    vector<string> type = t.type(t.member_type());//成员类型
    vector<string> Value;
    string value;
    bool exist = 0;
    in1_data.seekg(0, ios::end);
    int data_size = in1_data.tellg();
    in1_data.seekg(0, ios::beg);
    int size = 1;
    for (auto i: type)
    {
        if (i == "int" || i == "intprimarykey") size += sizeof(int);
        else if (i == "double") size += sizeof(double);
        else size += 20;
    }
    int type_num;
    bool if1=0;
    for (int i = 0; i <= name1.size(); i++)
    {
        if (name1[i] == member_name)
        {
            type_num = i;
            if1=1;
            break;
        }
    }
    if(!if1)
    {
        cout<<"->Not Find Such List!\n";
        vector<int> a,b;
        return make_pair(a, b);
    }
    if (t.if_primarykey)//主键索引
    {
        if (type_num == t.primary_key.second)//附加条件是主键
        {
            int select_value = stoi(range1);
            if (t.if_tree)
            {
                vector<int> point;
                vector<int> key;
                int select_value2 = stoi(range2);
                key.push_back(select_value);
                point = t.primary_key_tree.search_range(select_value, select_value2);
                in1_data.close();
                in1_index.close();
                return make_pair(key, point);
            } else//没有树
            {
                int size_index = 9;
                in1_index.seekg(0, ios::end);
                int index_size = in1_index.tellg();
                in1_index.seekg(0, ios::beg);
                exist = 0;
                while (index_size >= size_index - 1)
                {
                    in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    int point = in1_index.tellg();
                    if (!exist)
                    {
                        index_size -= size_index;
                        in1_index.seekg(point + 8);
                        //cout << "Not Found" << endl;
                        continue;
                    }
                    int key, va;
                    in1_index.read(reinterpret_cast<char *>( &key ), sizeof(int));
                    in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                    t.primary_key_tree.insert_value(key, va);
                    index_size -= size_index;
                }
                t.if_tree = 1;
                vector<int> point;
                vector<int> key;
                int select_value2 = stoi(range2);
                key.push_back(select_value);
                point = t.primary_key_tree.search_range(select_value, select_value2);
                in1_data.close();
                in1_index.close();
                return make_pair(key, point);
            }
        } else//select非主键
        {
            for (int i = 0; i <= t.Index.num; i++)//查找是否生成副索引
            {
                if (type_num == t.Index.member_name[i].second)//找到副索引
                {
                    if (t.Index.exist_map[i])
                    {
                        vector<int> point;
                        vector<int> key;
                        int x=t.Index.member_name[i].second;
                        vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                        for (auto item: range)
                        {
                            point.push_back(t.primary_key_tree.search_value(item));
                            key.push_back(item);
                        }
                        in1_data.close();
                        in1_index.close();
                        return make_pair(key, point);
                    } else//不存在map
                    {
                        Create_map(t.Index.find[t.Index.num], type_num, t.primary_key.second,
                                   in1_data, size, data_size, type);
                        t.Index.exist_map[t.Index.num] = 1;
                        vector<int> point;
                        vector<int> key;
                        int x=t.Index.member_name[i].second;
                        vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                        for (auto item: range)
                        {
                            point.push_back(t.primary_key_tree.search_value(item));
                            key.push_back(item);
                        }
                        in1_data.close();
                        in1_index.close();
                        return make_pair(key, point);
                    }
                }
            }
//没找到副表
            t.Index.num++;
            t.Index.member_name[t.Index.num] = make_pair(member_name, type_num);
            Create_map(t.Index.find[t.Index.num], type_num, t.primary_key.second,
                       in1_data, size, data_size, type);
            t.Index.exist_map[t.Index.num] = 1;
            vector<int> point;
            vector<int> key;
            int x=t.Index.member_name[t.Index.num].second;
            vector<int> range = Find_primary_key(t.Index.find[t.Index.num], range1, range2,type[x]);
            for (auto item: range)
            {
                if (t.if_tree)
                {
                    point.push_back(t.primary_key_tree.search_value(item));
                    key.push_back(item);
                } else
                {
                    int size_index = 9;
                    in1_index.seekg(0, ios::end);
                    int index_size = in1_index.tellg();
                    in1_index.seekg(0, ios::beg);
                    exist = 0;
                    while (index_size >= size_index - 1)
                    {
                        in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                        int point1 = in1_index.tellg();
                        if (!exist)
                        {
                            index_size -= size_index;
                            in1_index.seekg(point1 + 8);
                            //cout << "Not Found" << endl;
                            continue;
                        }
                        int key1, va;
                        in1_index.read(reinterpret_cast<char *>( &key1 ), sizeof(int));
                        in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                        t.primary_key_tree.insert_value(key1, va);
                        index_size -= size_index;
                    }
                    t.if_tree = 1;
                    point.push_back(t.primary_key_tree.search_value(item));
                    key.push_back(item);
                }
            }

            in1_data.close();
            in1_index.close();
            return make_pair(key, point);
        }
    } else//没有主键
    {
        if (t.Index.if_tree)
        {
            for (int i = 1; i <= t.Index.num; i++)//查找是否生成副索引
            {
                if (type_num == t.Index.member_name[i].second)//找到副索引
                {
                    if (t.Index.exist_map[i])
                    {
                        vector<int> point;
                        vector<int> key;
                        int x=t.Index.member_name[i].second;
                        vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                        for (auto item: range)
                        {
                            point.push_back(t.primary_key_tree.search_value(item));
                            key.push_back(item);
                        }
                        in1_data.close();
                        in1_index.close();
                        return make_pair(key, point);
                    } else//不存在map
                    {
                        Create_map_without_primary_key(t.Index.find[t.Index.num], type_num,
                                                       t.Index.member_name[0].second,
                                                       in1_data, t.Index.primary_key_tree, size, data_size, type);
                        t.Index.exist_map[t.Index.num] = 1;
                        vector<int> point;
                        vector<int> key;
                        int x=t.Index.member_name[i].second;
                        vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                        for (auto item: range)
                        {
                            point.push_back(t.primary_key_tree.search_value(item));
                            key.push_back(item);
                        }
                        in1_data.close();
                        in1_index.close();
                        return make_pair(key, point);
                    }
                }
            }
//没找到副表
            t.Index.num++;
            t.Index.member_name[t.Index.num] = make_pair(member_name, type_num);
            Create_map_without_primary_key(t.Index.find[t.Index.num], type_num, t.Index.member_name[0].second,
                                           in1_data, t.Index.primary_key_tree, size, data_size, type);
            t.Index.exist_map[t.Index.num] = 1;
            vector<int> point;
            vector<int> key;
            int x=t.Index.member_name[t.Index.num].second;
            vector<int> range = Find_primary_key(t.Index.find[t.Index.num], range1, range2,type[x]);
            for (auto item: range)
            {
                if (t.if_tree)
                {
                    point.push_back(t.primary_key_tree.search_value(item));
                    key.push_back(item);
                } else
                {
                    int size_index = 9;
                    in1_index.seekg(0, ios::end);
                    int index_size = in1_index.tellg();
                    in1_index.seekg(0, ios::beg);
                    exist = 0;
                    while (index_size >= size_index - 1)
                    {
                        in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                        int point1 = in1_index.tellg();
                        if (!exist)
                        {
                            index_size -= size_index;
                            in1_index.seekg(point1 + 8);
                            //cout << "Not Found" << endl;
                            continue;
                        }
                        int key1, va;
                        in1_index.read(reinterpret_cast<char *>( &key1 ), sizeof(int));
                        in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                        t.primary_key_tree.insert_value(key1, va);
                        index_size -= size_index;
                    }
                    t.if_tree = 1;
                    point.push_back(t.primary_key_tree.search_value(item));
                    key.push_back(item);
                }
            }

            in1_data.close();
            in1_index.close();
            return make_pair(key, point);
        } else//隐藏主键未生成树
        {
            ifstream in1_index2(t.name + "_index_0" + ".sql", ios::in | ios::binary);
            int size_index1 = 9;
            in1_index2.seekg(0, ios::end);
            int index_size1 = in1_index2.tellg();
            in1_index2.seekg(0, ios::beg);
            index_size1 -= 4;
            in1_index2.read(reinterpret_cast<char *>( &t.key ), sizeof(int));
            while (index_size1 >= size_index1 - 1)
            {
                in1_index2.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                int point = in1_index2.tellg();
                if (!exist)
                {
                    index_size1 -= size_index1;
                    in1_index.seekg(point + 8);
                    //cout << "Not Found" << endl;
                    continue;
                }
                int key, va;
                in1_index2.read(reinterpret_cast<char *>( &key ), sizeof(int));
                in1_index2.read(reinterpret_cast<char *>( &va ), sizeof(int));
                t.Index.primary_key_tree.insert_value(key, va);
                index_size1 -= size_index1;
            }
            in1_index2.close();
            t.Index.if_tree = 1;
            for (int i = 1; i <= t.Index.num; i++)//查找是否生成副索引
            {
                if (type_num == t.Index.member_name[i].second)//找到副索引
                {
                    if (t.Index.exist_map[i])
                    {
                        vector<int> point;
                        vector<int> key;
                        int x=t.Index.member_name[i].second;
                        vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                        for (auto item: range)
                        {
                            point.push_back(t.primary_key_tree.search_value(item));
                            key.push_back(item);
                        }
                        in1_data.close();
                        in1_index.close();
                        return make_pair(key, point);
                    } else//不存在map
                    {
                        Create_map_without_primary_key(t.Index.find[t.Index.num], type_num,
                                                       t.Index.member_name[0].second,
                                                       in1_data, t.Index.primary_key_tree, size, data_size, type);
                        t.Index.exist_map[t.Index.num] = 1;
                        vector<int> point;
                        vector<int> key;
                        int x=t.Index.member_name[i].second;
                        vector<int> range = Find_primary_key(t.Index.find[i], range1, range2,type[x]);
                        for (auto item: range)
                        {
                            point.push_back(t.primary_key_tree.search_value(item));
                            key.push_back(item);
                        }
                        in1_data.close();
                        in1_index.close();
                        return make_pair(key, point);
                    }
                }
            }
//没找到副表

            t.Index.num++;
            t.Index.member_name[t.Index.num] = make_pair(member_name, type_num);
            Create_map_without_primary_key(t.Index.find[t.Index.num], type_num, t.Index.member_name[0].second,
                                           in1_data, t.Index.primary_key_tree, size, data_size, type);
            t.Index.exist_map[t.Index.num] = 1;
            vector<int> point;
            vector<int> key;
            int x=t.Index.member_name[t.Index.num].second;
            vector<int> range = Find_primary_key(t.Index.find[t.Index.num], range1, range2,type[x]);
            for (auto item: range)
            {
                if (t.if_tree)
                {
                    point.push_back(t.primary_key_tree.search_value(item));
                    key.push_back(item);
                } else
                {
                    int size_index = 9;
                    in1_index.seekg(0, ios::end);
                    int index_size = in1_index.tellg();
                    in1_index.seekg(0, ios::beg);
                    exist = 0;
                    while (index_size >= size_index - 1)
                    {
                        in1_index.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                        int point1 = in1_index.tellg();
                        if (!exist)
                        {
                            index_size -= size_index;
                            in1_index.seekg(point1 + 8);
                            //cout << "Not Found" << endl;
                            continue;
                        }
                        int key1, va;
                        in1_index.read(reinterpret_cast<char *>( &key1 ), sizeof(int));
                        in1_index.read(reinterpret_cast<char *>( &va ), sizeof(int));
                        t.primary_key_tree.insert_value(key1, va);
                        index_size -= size_index;
                    }
                    t.if_tree = 1;
                    point.push_back(t.primary_key_tree.search_value(item));
                    key.push_back(item);
                }
            }

            in1_data.close();
            in1_index.close();
            return make_pair(key, point);
        }
    }
}//查找数据位置

pair<string, vector<string>> SelectData(pair<string, vector<string>> data, string name)
{
    if(data.first.empty())
    {
        string b;
        vector<string> a;
        return make_pair(b,a);
    }
    string member_name = data.first;
    int a;
    pair<string, vector<string>> pair1;
    vector<string> vec = type(member_name);
    vector<string> value;
    bool if1=0;
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == name)
        {
            if1=1;
            a = i;
            break;
        }
    }
    if(!if1)
    {
        cout<<"->Not Find Such List!\n";
        string A;
        vector<string> b;
        return make_pair(A,b);
    }
    for (int i = 0; i < data.second.size(); i++)
    {
        vector<string> v = type(data.second[i]);
        value.push_back(v[a]);
    }
    pair1 = make_pair(vec[a], value);
    return pair1;
}

void InsertFile(Table &t, vector<string> s, int num)
{
    vector<string> type = t.type(t.member_type());//数据类型
    if (num != type.size())
    {
        cout << "->Data num error!" << endl;
        return;
    }
    ofstream out_data(t.name + "_data.sql", ios::app | ios::binary);
    ofstream out_index(t.name + "_index.sql", ios::app | ios::binary);
    out_data.seekp(0, ios::end);
    int point = out_data.tellp();

    bool exist = 1;
    out_data.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
    for (int i = 0; i < type.size(); i++)
    {
        if (type[i] == "int" || type[i] == "intprimarykey" || type[i] == "double")
        {
            if (!is_number(s[i]))
            {
                cout << "->Please insert Correct Data!\n";
                return;
            }
        }
    }
    for (int i = 0; i < type.size(); i++)
    {
        if (type[i] == "int" || type[i] == "intprimarykey")
        {
            int a = stoi(s[i]);
            out_data.write(reinterpret_cast<char *>( &a ), sizeof(int));
        } else if (type[i] == "double")
        {
            double a = stod(s[i]);
            out_data.write(reinterpret_cast<char *>( &a ), sizeof(double));
        } else
        {
            char name[20];
            strcpy(name, s[i].c_str());
            out_data.write(name, 20);
        }
    }
    out_data.close();
    //主键索引
    if (t.if_primarykey)
    {
        int a;
        string type_name = type[t.primary_key.second];
        if (type_name == "int" || type_name == "intprimarykey")//目前只考虑主键为int
        {
            out_index.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
            a = stoi(s[t.primary_key.second]);
            out_index.write(reinterpret_cast<char *>( &a ), sizeof(int));
        }
        out_index.write(reinterpret_cast<char *>( &point ), sizeof(int));
        if (t.if_tree)
        {
            t.primary_key_tree.insert_value(a, point);
        }
        for (int i = 0; i <= t.Index.num; i++)
        {
            if (t.Index.exist_map[i])
            {
                t.Index.find[i].insert(pair<string, int>(s[t.Index.member_name[i].second], a));
            }
        }
    } else//没有主键
    {
        ofstream out_index2(t.name + "_index_0" + ".sql", ios::app | ios::binary);
        out_index2.seekp(0, ios::end);
        out_index2.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
        int a = stoi(s[t.Index.member_name[0].second]);
        out_index2.write(reinterpret_cast<char *>( &a ), sizeof(int));
        out_index2.write(reinterpret_cast<char *>( &t.key ), sizeof(int));
        out_index2.close();
        out_index.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
        out_index.write(reinterpret_cast<char *>( &t.key ), sizeof(int));
        t.key++;
        out_index.write(reinterpret_cast<char *>( &point ), sizeof(int));
        fstream out_index3(t.name + "_index_0" + ".sql", ios::in | ios::out | ios::binary);
        out_index3.seekp(0, ios::beg);
        out_index3.write(reinterpret_cast<char *>( &t.key ), sizeof(int));
        out_index3.close();
        if (t.if_tree)
        {
            t.primary_key_tree.insert_value(t.key, point);
        }
        if (t.Index.if_tree)
        {
            t.Index.primary_key_tree.insert_value(a, t.key);
        }
        for (int i = 1; i <= t.Index.num; i++)
        {
            if (t.Index.exist_map[i])
            {
                t.Index.find[i].insert(pair<string, int>(s[t.Index.member_name[i].second], t.key));
            }
        }
    }
    out_index.close();
}//插入数据

void UpdateFile(Table &t, string member_name1, string value1, string member_name2, string value2)//更新数据不能修改主键值
{
    vector<string> name1 = t.type(t.member_name());
    vector<string> type1 = t.type(t.member_type());
    pair<string, vector<string>> pair1 = SelectFile(t, member_name1, value1, value1);
    if(pair1.second.empty())
    {
        return;
    }
    vector<int> point;
    vector<int> key;
    pair<vector<int>, vector<int>> pair2;
    pair2 = SelectPoint(t, member_name1, value1, value1);
    if(pair2.first.empty())
    {
        return;
    }
    point = pair2.second;
    key = pair2.first;
    int type_num;
    bool if1=0;
    for (int i = 0; i < name1.size(); i++)
    {
        if (name1[i] == member_name2)
        {
            type_num = i;
            if1=1;
            break;
        }
    }
    if(!if1)
    {
        cout<<"->Not Find Such List!\n";
        return;
    }
    for (int j = 0; j < point.size(); j++)
    {
        if (t.if_primarykey)
        {
            fstream out_data(t.name + "_data.sql", ios::in | ios::out | ios::binary);
            out_data.seekg(point[j]);
            vector<string> data = type(pair1.second[j]);
            string old_data = data[type_num];
            data[type_num] = value2;
            bool exist = 1;
            out_data.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
            for (int i = 0; i < type1.size(); i++)
            {
                if (type1[i] == "int" || type1[i] == "intprimarykey")
                {
                    int a = stoi(data[i]);
                    out_data.write(reinterpret_cast<char *>( &a ), sizeof(int));
                } else if (type1[i] == "double")
                {
                    double a = stod(data[i]);
                    out_data.write(reinterpret_cast<char *>( &a ), sizeof(double));
                } else
                {
                    char name[20];
                    strcpy(name, data[i].c_str());
                    out_data.write(name, 20);
                }
            }
            out_data.close();
            for (int i = 0; i <= t.Index.num; i++)
            {
                if (t.Index.member_name[i].first == member_name2)
                {
                    auto it = t.Index.find[i].lower_bound(old_data);
                    while (it != t.Index.find[i].end() && it->first == old_data)
                    {
                        if (it->second == key[j])
                        {
                            t.Index.find[i].erase(it);
                            t.Index.find[i].insert({data[type_num], key[j]});
                        }
                        it++;
                    }
                }
            }
        } else//无主键
        {
            fstream out_data(t.name + "_data.sql", ios::out | ios::binary);
            out_data.seekg(point[j]);
            vector<string> data = type(pair1.second[j]);
            string old_data = data[type_num];
            data[type_num] = value2;
            bool exist = 1;
            out_data.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
            for (int i = 0; i < type1.size(); i++)
            {
                if (type1[i] == "int" || type1[i] == "intprimarykey")
                {
                    int a = stoi(data[i]);
                    out_data.write(reinterpret_cast<char *>( &a ), sizeof(int));
                } else if (type1[i] == "double")
                {
                    double a = stod(data[i]);
                    out_data.write(reinterpret_cast<char *>( &a ), sizeof(double));
                } else
                {
                    char name[20];
                    strcpy(name, data[i].c_str());
                    out_data.write(name, 20);
                }
            }
            out_data.close();

            for (int i = 1; i <= t.Index.num; i++)
            {
                if (t.Index.member_name[i].first == member_name2)
                {
                    auto it = t.Index.find[i].lower_bound(old_data);
                    while (it != t.Index.find[i].end() && it->first == old_data)
                    {
                        if (it->second == key[j])
                        {
                            t.Index.find[i].erase(it);
                            t.Index.find[i].insert({data[type_num], key[j]});
                        }
                        it++;
                    }
                }
            }
        }
    }
}

void DeleteFile(Table &t, string member_name, string value)
{
    vector<string> name1 = t.type(t.member_name());
    pair<string, vector<string>> pair1 = SelectFile(t, member_name, value, value);
    if(pair1.second.empty())
    {
        return;
    }
    vector<int> point;
    vector<int> key;
    pair<vector<int>, vector<int>> pair2;
    pair2 = SelectPoint(t, member_name, value, value);
    point = pair2.second;
    key = pair2.first;
    for (int j = 0; j < point.size(); j++)
    {
        if (t.if_primarykey)
        {
            fstream out_data(t.name + "_data.sql", ios::in | ios::out | ios::binary);
            out_data.seekg(point[j]);
            vector<string> data = type(pair1.second[j]);

            bool exist = 0;
            out_data.write(reinterpret_cast<char *>( &exist ), sizeof(bool));

            out_data.close();
            for (int i = 0; i <= t.Index.num; i++)//delete sub 索引
            {
                for (int k = 0; k < name1.size(); k++)
                {
                    if (t.Index.member_name[i].first == name1[k])
                    {
                        auto it = t.Index.find[i].lower_bound(data[k]);
                        while (it != t.Index.find[i].end() && it->first == data[k])
                        {
                            if (it->second == key[j])
                            {
                                t.Index.find[i].erase(it);
                            }
                            it++;
                        }
                    }
                }
            }
            //删除主键树与index文件
            fstream out_data2(t.name + "_index.sql", ios::in | ios::out | ios::binary);
            int size_index = 9;
            out_data2.seekg(0, ios::end);
            int index_size = out_data2.tellg();
            out_data2.seekg(0, ios::beg);
            exist = 0;
            while (index_size >= size_index - 1)
            {
                int point1 = out_data2.tellg();
                out_data2.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                if (!exist)
                {
                    index_size -= size_index;
                    out_data2.seekg(point1 + 9);
                    //cout << "Not Found" << endl;
                    continue;
                }
                int key1, va;
                out_data2.read(reinterpret_cast<char *>( &key1 ), sizeof(int));
                out_data2.read(reinterpret_cast<char *>( &va ), sizeof(int));
                int point2 = out_data2.tellg();
                if (key1 == key[j])
                {
                    exist = 0;
                    out_data2.seekg(point1);
                    out_data2.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    out_data2.seekg(point2);
                    break;
                }
                index_size -= size_index;
            }
            out_data2.close();
            t.primary_key_tree.drop_value(key[j]);
        } else//无主键
        {
            fstream out_data(t.name + "_data.sql", ios::in | ios::out | ios::binary);
            out_data.seekg(point[j]);
            vector<string> data = type(pair1.second[j]);

            bool exist = 0;
            out_data.write(reinterpret_cast<char *>( &exist ), sizeof(bool));

            out_data.close();
            for (int i = 1; i <= t.Index.num; i++)//delete sub 索引
            {
                for (int k = 0; k < name1.size(); k++)
                {
                    if (t.Index.member_name[i].first == name1[k])
                    {
                        auto it = t.Index.find[i].lower_bound(data[k]);
                        while (it != t.Index.find[i].end() && it->first == data[k])
                        {
                            if (it->second == key[j])
                            {
                                t.Index.find[i].erase(it);
                            }
                            it++;
                        }
                    }
                }
            }
            fstream out_data2(t.name + "_index.sql", ios::in | ios::out | ios::binary);
            int size_index = 9;
            out_data2.seekg(0, ios::end);
            int index_size = out_data2.tellg();
            out_data2.seekg(0, ios::beg);
            exist = 0;
            while (index_size >= size_index - 1)
            {
                int point1 = out_data2.tellg();
                out_data2.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                if (!exist)
                {
                    index_size -= size_index;
                    out_data2.seekg(point1 + 9);
                    //cout << "Not Found" << endl;
                    continue;
                }
                int key1, va;
                out_data2.read(reinterpret_cast<char *>( &key1 ), sizeof(int));
                out_data2.read(reinterpret_cast<char *>( &va ), sizeof(int));
                int point2 = out_data2.tellg();
                if (key1 == key[j])
                {
                    exist = 0;
                    out_data2.seekg(point1);
                    out_data2.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    out_data2.seekg(point2);
                    break;
                }
                index_size -= size_index;
            }
            out_data2.close();
            t.primary_key_tree.drop_value(key[j]);
            if (t.Index.if_tree)
            {
                string a = data[t.Index.member_name[0].second];
                t.Index.primary_key_tree.drop_value(stoi(a));
            }
            fstream out_data3(t.name + "_index_0.sql", ios::in | ios::out | ios::binary);
            out_data3.seekg(0, ios::end);
            index_size = out_data3.tellg();
            out_data3.seekg(0, ios::beg);
            while (index_size >= size_index - 1)
            {
                int point1 = out_data3.tellg();
                out_data3.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
                if (!exist)
                {
                    index_size -= size_index;
                    out_data3.seekg(point1 + 9);
                    //cout << "Not Found" << endl;
                    continue;
                }
                int key1, va;
                out_data3.read(reinterpret_cast<char *>( &key1 ), sizeof(int));
                out_data3.read(reinterpret_cast<char *>( &va ), sizeof(int));
                int point2 = out_data3.tellg();
                if (va == key[j])
                {
                    exist = 0;
                    out_data3.seekg(point1);
                    out_data3.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
                    out_data3.seekg(point2);
                    break;
                }
                index_size -= size_index;
            }
            out_data3.close();
        }
    }
}//删除数据

void DeleteTable(Table &t, string database, vector<Table> &table)
{
    char name[20];
    bool exist;
    fstream in(database + ".dat", ios::in | ios::out | ios::binary);
    int size = 21;
    in.seekg(0, ios::end);
    int data_size = in.tellg();
    in.seekg(0, ios::beg);
    while (data_size >= size - 1)
    {
        int point = in.tellg();
        in.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        in.read(name, 20);
        if (name == t.name)
        {
            in.seekg(point);
            exist = 0;
            in.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
            break;
        }
        if (!exist)
        {
            in.seekg(point + size);
            data_size -= size;
            continue;
        }
        data_size -= size;
    }
    in.close();
    string d[4];
    d[0] = (t.name + ".sql");
    d[1] = (t.name + "_data.sql");
    d[2] = (t.name + "_index.sql");
    d[3] = (t.name + "_index_0.sql");
    if (t.if_primarykey)
    {
        remove(d[0].c_str());
        remove(d[1].c_str());
        remove(d[2].c_str());
    } else
    {
        remove(d[0].c_str());
        remove(d[1].c_str());
        remove(d[2].c_str());
        remove(d[3].c_str());
    }
    for (int i = 0; i < table.size(); i++)
    {
        if (table[i].name == t.name)
        {
            table.erase(table.begin() + i);
            break;
        }
    }
}//删除表

void DeleteDatabase(string database_name, vector<Table> &t,char* buf)//删除数据库后回到主路径
{
    string LoadTestPath = buf;
    chdir(LoadTestPath.c_str());
    std::filesystem::remove_all(database_name.c_str());
    t.clear();
    fstream database("Databases.dat", ios::in | ios::out | ios::binary);
    int size = 21;
    database.seekg(0, ios::end);
    int data_size = database.tellg();
    database.seekg(0, ios::beg);
    bool exist,if1=0;
    char name[20];
    while (data_size >= size - 1)
    {
        int point = database.tellg();
        database.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        if (!exist)
        {
            database.seekg(point + size);
            data_size -= size;
            continue;
        }
        database.read(name, 20);
        if(name==database_name)
        {
            if1=1;
            break;
        }
        data_size -= size;
    }
    if(!if1)
    {
        cout<<"->Not Find Such Database!"<<endl;
        return;
    }
    database.seekg(0, ios::end);
    data_size = database.tellg();
    database.seekg(0, ios::beg);
    while (data_size >= size - 1)
    {
        int point = database.tellg();
        database.read(reinterpret_cast<char *>( &exist ), sizeof(bool));
        if (!exist)
        {
            database.seekg(point + size);
            data_size -= size;
            continue;
        }
        char name1[20];
        database.read(name1, 20);
        string a=name1;
        if (a == database_name)
        {
            database.seekg(point);
            exist = 0;
            database.write(reinterpret_cast<char *>( &exist ), sizeof(bool));
            break;
        }
        data_size -= size;
    }
    database.close();
}

void if_or_and(bool if_or,bool if_and,vector<vector<Token>> operation,Table *t,
               vector<pair<string,vector<string>>> &vec,vector<select_one> select_list)
{
    vector<string> name = t->type(t->member_name());
    if(if_or)
    {
        pair<string, vector<string>> pair1_1;
        pair<string, vector<string>> pair1_1_id;
        pair<string, vector<string>> pair1_2;
        pair<string, vector<string>> pair1_2_id;
        pair<string, vector<string>> pair3;//最终
        vector<string> data;
        if(operation[0][1].getValue()=="=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),operation[0][2].getValue());
        }
        else if(operation[0][1].getValue()==">=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"up");
        }
        else if(operation[0][1].getValue()=="<=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[0][2].getValue());
            b=stod(operation[0][4].getValue());
            if(a>b)
            {
                swap(operation[0][2],operation[0][4]);
            }
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),operation[0][4].getValue());

        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[0];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[0][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_1= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error!\n";
        }
        //处理第二个operation
        if(operation[1][1].getValue()=="=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),
                                 operation[1][2].getValue());
        }
        else if(operation[1][1].getValue()==">=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"up");
        }
        else if(operation[1][1].getValue()=="<=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[1][2].getValue());
            b=stod(operation[1][4].getValue());
            if(a>b)
            {
                swap(operation[1][2],operation[1][4]);
            }
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),operation[1][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[1];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[1][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_2= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error!\n";
        }
        //判断pair是否为空
        if(pair1_1.second.empty() || pair1_2.second.empty())
        {
            return;
        }
        if(t->if_primarykey)
        {
            pair1_1_id=SelectData(pair1_1,t->primary_key.first);
            pair1_2_id=SelectData(pair1_2,t->primary_key.first);
            
            for(int i=0;i<pair1_1.second.size();i++)
            {
                bool b=1;
                for(auto it : pair1_2_id.second)
                {
                    if(pair1_1_id.second[i]==it)
                    {
                        b=0;
                        break;
                    }
                }
                if(b) data.push_back(pair1_1.second[i]);
            }
            for(int i=0;i<pair1_2.second.size();i++)
            {
                data.push_back(pair1_2.second[i]);
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        else
        {
            pair1_1_id=SelectData(pair1_1,t->Index.member_name[0].first);
            pair1_2_id=SelectData(pair1_2,t->Index.member_name[0].first);
            for(int i=0;i<pair1_1.second.size();i++)
            {
                bool b=1;
                for(auto it : pair1_2_id.second)
                {
                    if(pair1_1_id.second[i]==it)
                    {
                        b=0;
                        break;
                    }
                }
                if(b) data.push_back(pair1_1.second[i]);
            }
            for(int i=0;i<pair1_2.second.size();i++)
            {
                data.push_back(pair1_2.second[i]);
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        if(select_list.empty())
        {
            for (auto i: name)
            {
                pair<string, vector<string>> pair2 = SelectData(pair3, i);
                vec.push_back(pair2);
            }
        }
        else
        {
            for (auto i: select_list)
            {
                pair<string, vector<string>> pair2 = SelectData(pair3, i.list_name);
                if(pair2.second.empty())
                {
                    continue;
                }
                vec.push_back(pair2);
            }
        }
        if(vec.empty())
        {
            return;
        }
        Line(vec.size());
        cout<<"->";
        for(int i=0;i<vec.size();i++)
        {
            cout<<"|"<<left<<setw(10)<<vec[i].first;
        }
        cout<<"|"<<endl;
        Line(vec.size());
        for(int i=0;i<pair3.second.size();i++)
        {
            cout<<"->";
            for(auto j :vec)
            {
                cout<<"|"<<left<<setw(10)<<j.second[i];
            }
            cout<<"|"<<endl;
        }
        Line(vec.size());
        vec.clear();
    }
    else if(if_and)
    {
        pair<string, vector<string>> pair1_1;
        pair<string, vector<string>> pair1_1_id;
        pair<string, vector<string>> pair1_2;
        pair<string, vector<string>> pair1_2_id;
        pair<string, vector<string>> pair3;//最终
        vector<string> data;
        if(operation[0][1].getValue()=="=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),
                                 operation[0][2].getValue());
        }
        else if(operation[0][1].getValue()==">=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"up");
        }
        else if(operation[0][1].getValue()=="<=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[0][2].getValue());
            b=stod(operation[0][4].getValue());
            if(a>b)
            {
                swap(operation[0][2],operation[0][4]);
            }
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),operation[0][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[0];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[0][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_1= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error!\n";
        }
        if(operation[1][1].getValue()=="=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),
                                 operation[1][2].getValue());
        }
        else if(operation[1][1].getValue()==">=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"up");
        }
        else if(operation[1][1].getValue()=="<=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"down");
        }
        else if(operation[1][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[1][2].getValue());
            b=stod(operation[1][4].getValue());
            if(a>b)
            {
                swap(operation[1][2],operation[1][4]);
            }
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),operation[1][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[1];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[1][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_2= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error!\n";
        }
        if(pair1_1.second.empty() || pair1_2.second.empty())
        {
            return;
        }
        if(t->if_primarykey)
        {
            pair1_1_id=SelectData(pair1_1,t->primary_key.first);
            pair1_2_id=SelectData(pair1_2,t->primary_key.first);
            for(int i=0;i<pair1_1.second.size();i++)
            {
                for(int it=0;it<pair1_2_id.second.size();it++)
                {
                    if(pair1_1_id.second[i]==pair1_2_id.second[it])
                    {
                        pair1_2_id.second.erase(pair1_2_id.second.begin()+it);
                        data.push_back(pair1_1.second[i]);
                        break;
                    }
                }
                
            }
            for(int i=0;i<pair1_2_id.second.size();i++)
            {
                for(auto it : pair1_1_id.second)
                {
                    if(pair1_2_id.second[i]==it)
                    {
                        data.push_back(pair1_2.second[i]);
                        break;
                    }
                }
                
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        else
        {
            pair1_1_id=SelectData(pair1_1,t->Index.member_name[0].first);
            pair1_2_id=SelectData(pair1_2,t->Index.member_name[0].first);
            for(int i=0;i<pair1_1.second.size();i++)
            {
                for(int it=0;it<pair1_2_id.second.size();it++)
                {
                    if(pair1_1_id.second[i]==pair1_2_id.second[it])
                    {
                        pair1_2_id.second.erase(pair1_2_id.second.begin()+it);
                        data.push_back(pair1_1.second[i]);
                        break;
                    }
                }
            }
            for(int i=0;i<pair1_2_id.second.size();i++)
            {
                for(auto it : pair1_1_id.second)
                {
                    if(pair1_2_id.second[i]==it)
                    {
                        data.push_back(pair1_2.second[i]);
                        break;
                    }
                }
                
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        if(select_list.empty())
        {
            for (auto i: name)
            {
                pair<string, vector<string>> pair2 = SelectData(pair3, i);
                vec.push_back(pair2);
            }
        }
        else
        {
            for (auto i: select_list)
            {
                pair<string, vector<string>> pair2 = SelectData(pair3, i.list_name);
                if(pair2.second.empty())
                {
                    continue;
                }
                vec.push_back(pair2);
            }
        }
        if(vec.empty())
        {
            return;
        }
        Line(vec.size());
        cout<<"->";
        for(int i=0;i<vec.size();i++)
        {
            cout<<"|"<<left<<setw(10)<<vec[i].first;
        }
        cout<<"|"<<endl;
        Line(vec.size());
        for(int i=0;i<pair3.second.size();i++)
        {
            cout<<"->";
            for(auto j :vec)
            {
                cout<<"|"<<left<<setw(10)<<j.second[i];
            }
            cout<<"|"<<endl;
        }
        Line(vec.size());
        vec.clear();
    }
    else//二者都无
    {
        pair<string, vector<string>> pair1;
        if(operation[0][1].getValue()=="=")
        {
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                               operation[0][2].getValue(),
                               operation[0][2].getValue());
        }
        else if(operation[0][1].getValue()==">=")
        {
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                               operation[0][2].getValue(),"up");
        }
        else if(operation[0][1].getValue()=="<=")
        {
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                               operation[0][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[0][2].getValue());
            b=stod(operation[0][4].getValue());
            if(a>b)
            {
                swap(operation[0][2],operation[0][4]);
            }
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),operation[0][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[0];
            vector<string> total;
            vector<string> data;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data.push_back(i.getValue());
            }
            for (auto& i : data)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[0][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error!\n";
        }
        if(pair1.second.empty())
        {
            return;
        }
        if(select_list.empty())
        {
            for (auto i: name)
            {
                pair<string, vector<string>> pair2 = SelectData(pair1, i);
                vec.push_back(pair2);
            }
        }
        else
        {
            for (auto i: select_list)
            {
                pair<string, vector<string>> pair2 = SelectData(pair1, i.list_name);
                if(pair2.second.empty())
                {
                    continue;
                }
                vec.push_back(pair2);
            }
        }
        if(vec.empty())
        {
            return;
        }
        Line(vec.size());
        cout<<"->";
        for(int i=0;i<vec.size();i++)
        {
            cout<<"|"<<left<<setw(10)<<vec[i].first;
        }
        cout<<"|"<<endl;
        Line(vec.size());
        for(int i=0;i<pair1.second.size();i++)
        {
            cout<<"->";
            for(auto j :vec)
            {
                cout<<"|"<<left<<setw(10)<<j.second[i];
            }
            cout<<"|"<<endl;
        }
        Line(vec.size());
        vec.clear();
    }
}

void UpdateUse(bool if_or,bool if_and,vector<vector<Token>> operation,Table *t,
               vector<pair<string,vector<string>>> &vec)
{
    vector<string> name = t->type(t->member_name());
    if(if_or)
    {
        pair<string, vector<string>> pair1_1;
        pair<string, vector<string>> pair1_1_id;
        pair<string, vector<string>> pair1_2;
        pair<string, vector<string>> pair1_2_id;
        pair<string, vector<string>> pair3;//最终
        vector<string> data;
        if(operation[0][1].getValue()=="=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),operation[0][2].getValue());
        }
        else if(operation[0][1].getValue()==">=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"up");
        }
        else if(operation[0][1].getValue()=="<=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[0][2].getValue());
            b=stod(operation[0][4].getValue());
            if(a>b)
            {
                swap(operation[0][2],operation[0][4]);
            }
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),operation[0][4].getValue());

        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[0];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[0][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_1= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error\n";
            return;
        }
        //处理第二个operation
        if(operation[1][1].getValue()=="=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),
                                 operation[1][2].getValue());
        }
        else if(operation[1][1].getValue()==">=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"up");
        }
        else if(operation[1][1].getValue()=="<=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[1][2].getValue());
            b=stod(operation[1][4].getValue());
            if(a>b)
            {
                swap(operation[1][2],operation[1][4]);
            }
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),operation[1][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[1];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[1][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_2= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error\n";
            return;
        }
        if(pair1_1.second.empty() || pair1_2.second.empty())
        {
            return;
        }
        if(t->if_primarykey)
        {
            pair1_1_id=SelectData(pair1_1,t->primary_key.first);
            pair1_2_id=SelectData(pair1_2,t->primary_key.first);
            
            for(int i=0;i<pair1_1.second.size();i++)
            {
                bool b=1;
                for(auto it : pair1_2_id.second)
                {
                    if(pair1_1_id.second[i]==it)
                    {
                        b=0;
                        break;
                    }
                }
                if(b) data.push_back(pair1_1.second[i]);
            }
            for(int i=0;i<pair1_2.second.size();i++)
            {
                data.push_back(pair1_2.second[i]);
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        else
        {
            pair1_1_id=SelectData(pair1_1,t->Index.member_name[0].first);
            pair1_2_id=SelectData(pair1_2,t->Index.member_name[0].first);
            for(int i=0;i<pair1_1.second.size();i++)
            {
                bool b=1;
                for(auto it : pair1_2_id.second)
                {
                    if(pair1_1_id.second[i]==it)
                    {
                        b=0;
                        break;
                    }
                }
                if(b) data.push_back(pair1_1.second[i]);
            }
            for(int i=0;i<pair1_2.second.size();i++)
            {
                data.push_back(pair1_2.second[i]);
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        
        for (auto i: name)
        {
            pair<string, vector<string>> pair2 = SelectData(pair3, i);
            vec.push_back(pair2);
        }
    }
    else if(if_and)
    {
        pair<string, vector<string>> pair1_1;
        pair<string, vector<string>> pair1_1_id;
        pair<string, vector<string>> pair1_2;
        pair<string, vector<string>> pair1_2_id;
        pair<string, vector<string>> pair3;//最终
        vector<string> data;
        if(operation[0][1].getValue()=="=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),
                                 operation[0][2].getValue());
        }
        else if(operation[0][1].getValue()==">=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"up");
        }
        else if(operation[0][1].getValue()=="<=")
        {
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[0][2].getValue());
            b=stod(operation[0][4].getValue());
            if(a>b)
            {
                swap(operation[0][2],operation[0][4]);
            }
            pair1_1 = SelectFile(*t, operation[0][0].getValue(),
                                 operation[0][2].getValue(),operation[0][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[0];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[0][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_1= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error\n";
            return;
        }
        if(operation[1][1].getValue()=="=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),
                                 operation[1][2].getValue());
        }
        else if(operation[1][1].getValue()==">=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"up");
        }
        else if(operation[1][1].getValue()=="<=")
        {
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),"down");
        }
        else if(operation[1][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[1][2].getValue());
            b=stod(operation[1][4].getValue());
            if(a>b)
            {
                swap(operation[1][2],operation[1][4]);
            }
            pair1_2 = SelectFile(*t, operation[1][0].getValue(),
                                 operation[1][2].getValue(),operation[1][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[1];
            vector<string> total;
            vector<string> data1;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data1.push_back(i.getValue());
            }
            for (auto& i : data1)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[1][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1_2= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error\n";
            return;
        }
        if(pair1_1.second.empty() || pair1_2.second.empty())
        {
            return;
        }
        if(t->if_primarykey)
        {
            pair1_1_id=SelectData(pair1_1,t->primary_key.first);
            pair1_2_id=SelectData(pair1_2,t->primary_key.first);
            for(int i=0;i<pair1_1.second.size();i++)
            {
                for(int it=0;it<pair1_2_id.second.size();it++)
                {
                    if(pair1_1_id.second[i]==pair1_2_id.second[it])
                    {
                        pair1_2_id.second.erase(pair1_2_id.second.begin()+it);
                        data.push_back(pair1_1.second[i]);
                        break;
                    }
                }
                
            }
            for(int i=0;i<pair1_2_id.second.size();i++)
            {
                for(auto it : pair1_1_id.second)
                {
                    if(pair1_2_id.second[i]==it)
                    {
                        data.push_back(pair1_2.second[i]);
                        break;
                    }
                }
                
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        else
        {
            pair1_1_id=SelectData(pair1_1,t->Index.member_name[0].first);
            pair1_2_id=SelectData(pair1_2,t->Index.member_name[0].first);
            for(int i=0;i<pair1_1.second.size();i++)
            {
                for(int it=0;it<pair1_2_id.second.size();it++)
                {
                    if(pair1_1_id.second[i]==pair1_2_id.second[it])
                    {
                        pair1_2_id.second.erase(pair1_2_id.second.begin()+it);
                        data.push_back(pair1_1.second[i]);
                        break;
                    }
                }
            }
            for(int i=0;i<pair1_2_id.second.size();i++)
            {
                for(auto it : pair1_1_id.second)
                {
                    if(pair1_2_id.second[i]==it)
                    {
                        data.push_back(pair1_2.second[i]);
                        break;
                    }
                }
                
            }
            pair3 = make_pair(pair1_2.first,data);
        }
        for (auto i: name)
        {
            pair<string, vector<string>> pair2 = SelectData(pair3, i);
            vec.push_back(pair2);
        }
    }
    else//二者都无
    {
        pair<string, vector<string>> pair1;
        if(operation[0][1].getValue()=="=")
        {
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                               operation[0][2].getValue(),
                               operation[0][2].getValue());
        }
        else if(operation[0][1].getValue()==">=")
        {
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                               operation[0][2].getValue(),"up");
        }
        else if(operation[0][1].getValue()=="<=")
        {
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                               operation[0][2].getValue(),"down");
        }
        else if(operation[0][1].getValue()=="BETWEEN")
        {
            double a,b;
            a=stod(operation[0][2].getValue());
            b=stod(operation[0][4].getValue());
            if(a>b)
            {
                swap(operation[0][2],operation[0][4]);
            }
            pair1 = SelectFile(*t, operation[0][0].getValue(),
                               operation[0][2].getValue(),operation[0][4].getValue());
        }
        else if(operation[0][1].getValue()=="IN")
        {
            vector<Token> temp = operation[0];
            vector<string> total;
            vector<string> data;
            for (auto& i : temp)
            {
                if (i.getType() == TokenType::LITERAL)
                    data.push_back(i.getValue());
            }
            for (auto& i : data)
            {
                pair<string, vector<string>> p = SelectFile(*t,operation[0][0].getValue(),
                                                            i,i);
                for(auto j : p.second)
                    total.push_back(j);
            }
            pair1= make_pair(t->member_name(),total);
        }
        else
        {
            cout<<"->Operator Error\n";
            return;
        }
        if(pair1.second.empty() )
        {
            return;
        }
        for (auto i: name)
        {
            pair<string, vector<string>> pair2 = SelectData(pair1, i);
            vec.push_back(pair2);
        }
    }
}
#endif //C___FILE_H
