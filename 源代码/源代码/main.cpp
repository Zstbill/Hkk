#include <bits/stdc++.h>

#include "keyboard.h"
#include "cmd.h"
#include "color.h"
#include "file.h"
#include "Table.h"
#include "Lexer.h"
#include "select_one.h"
#include "operation.h"
#include "search_error.h"

using namespace std::chrono;
using namespace std;

vector<Table> table;

void setConsoleUTF8()
{
    SetConsoleOutputCP(CP_UTF8);
#pragma warning (disable: 4996)
    std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
    std::wcout.imbue(utf8);
#pragma warning (default: 4996)
}

Table *Find_Table(string name)
{
    for (int i = 0; i < table.size(); i++)
    {
        if (table[i].name == name)
        {
            if (!table[i].if_read)
            {
                string a;
                ifstream in1(table[i].name + ".sql", ios::in);
                getline(in1, a);
                while (getline(in1, a))
                {
                    table[i].member.push_back(a);
                }
                table[i].if_primarykey = table[i].if_primary(table[i].type(table[i].member_type()),
                                                             table[i].type(table[i].member_name()));
                if (!table[i].if_primarykey)
                {
                    vector<string> type = table[i].type(table[i].member_type());
                    vector<string> name1 = table[i].type(table[i].member_name());
                    for (int j = 0; j < type.size(); j++)
                    {
                        if (type[j] == "int")
                        {
                            table[i].Index.num++;
                            table[i].Index.member_name[table[i].Index.num] = make_pair(name1[j], j);
                            ifstream in_index2(table[i].name + "_index_" + "0.sql", ios::in | ios::binary);
                            int key;
                            in_index2.read(reinterpret_cast<char *>( &key ), sizeof(int));
                            table[i].key = key;
                            in_index2.close();
                            break;
                        }
                    }
                }
                table[i].if_read = 1;
            }
            return &table[i];
        }
    }
    cout << "->Not Find Such Table!\n";
    return NULL;
}

int main()
{
    char buf[1000];//当前路径
    GetCurrentDirectory(1000, buf);
    setConsoleUTF8();
    EnableVTMode();//初始化VT
    SetFontSize(10, 20);
    setColorAttr(30);
    setColorAttr(107);
    clearScreen();
    while (!CheckScreenSize())
        system("pause");
    clearScreen();
    string pr;
    setColorAttr(30);
    setColorAttr(107);
    clearScreen();
    ifstream print("Welcome.txt", ios::in);//生成字符画
    while (getline(print, pr))
    {
        cout << pr << endl;
        this_thread::sleep_for(chrono::milliseconds(66));//sleep参数为66
    }
    print.close();
//    处理帐户
    ifstream acc1("account.txt", ios::in);
    if (!acc1)
    {
        ofstream acc("account.txt", ios::out);
        string a, p;
        cout << "For the first open Luki_ST SQL, please set your account name and password:" << endl;
        cout << "name:";
        cin >> a;
        acc << a << endl;
        cout << "password:";
        cin >> p;
        acc << p << endl;
        acc.close();
    } else
    {
        string a[2], p[2];
        acc1 >> a[0] >> p[0];
        cout << "Please enter your account name:";
        while (cin >> a[1])
        {
            if (a[1] == a[0])
            {
                cout << "Please enter your password:";
                while (1)
                {
                    p[1] = Password();
                    if (p[1] != p[0])
                    {
                        cerr << "Please enter the right password:";
                    } else break;
                }
                break;
            } else cerr << "Please enter the right account name:";
        }
    }
    acc1.close();

    setColorAttr(0);
    clearScreen();
    cin.ignore();
    string in, In = "";
    string database_name1;
    bool if_use_database = 0;
    //主循环，需以“;”换行
    vector<Token> tokens;
    while (1)
    {
        cout << "->";
        getline(cin, in);
        if (in[in.size() - 1] == ';')
        {
            auto start = high_resolution_clock::now();
            In += in;
            if (In == "exit;") return 0;
            if (In == "cls;")
            {
                clearScreen();
                In = "";
                continue;
            }
            if (In == "pwd;")
            {
                pwd();
                In = "";
                continue;
            }
            tokens = get_statement_tokens(In);
    
            // 检查语法错误
            string error_box = search_error(tokens);
            {
                if (!error_box.empty())
                {
                    In.clear();
                    cout<<endl;
                    cout<<"-> ----------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"-> "<<error_box<<endl;
                    cout<<endl;
                    cout<<endl;
                    continue;
                }
            }
            
            Token token = tokens[0];

            // todo: 已更新了IN和BETWEEN关键字,下面对where的处理耶也全部包含这两个关键字
            if (token.getType() == TokenType::SELECT)
            {
                // todo: BETWEEN & IN

                vector<select_one> select_list;
                string table_name;
                vector<Token> where_tokens;
                where_tokens = get_where_tokens(tokens);

                // 获取一个select_list
                bool if_from = false;
                bool if_all = false;
                for (int i = 0; i < tokens.size(); i++)
                {
                    auto &token = tokens[i];
                    select_one temp_one;
                    if (token.getType() == TokenType::SELECT)
                    {
                        continue;
                    }
                    if (token.getType() == TokenType::FROM)
                    {
                        if_from = true;
                        continue;
                    }
                    if (token.getType() == TokenType::WHERE)
                        break;

                    // select 和 from 中间
                    if (if_from == false)
                    {
                        if (token.getType() == TokenType::IDENTIFIER)
                        {
                            temp_one.list_name = token.getValue();
                            if (tokens[i + 1].getType() == TokenType::AS)
                            {
                                temp_one.alias = tokens[i + 2].getValue();
                                i = i + 3;
                            }
                            select_list.push_back(temp_one);
                        } else if (token.getType() == TokenType::ALL)
                        {
                            if_all = true;
                            select_list.clear();
                            continue;
                        }
                    }
                        // from 和 where 中间
                    else
                    {
                        if (token.getType() == TokenType::IDENTIFIER)
                        {
                            table_name = token.getValue();
                            for (auto &temp: select_list)
                            {
                                temp.table_name = table_name;
                            }
                        }
                    }
                }
                // 处理where语句
                // 拆分为多个查询
                vector<vector<Token>> operation;
                bool if_or = false, if_and = false;
                bool if_between = false;
                for (auto &i: where_tokens)
                {
                    if (i.getType() == TokenType::BETWEEN)
                    {
                        if_between = true;
                        continue;
                    } else if (i.getType() == TokenType::OR)
                    {
                        operation = break_where_tokens_by_OR(where_tokens);
                        if_or = true;
                        break;
                    } else if (i.getType() == TokenType::AND)
                    {
                        if (if_between == false)
                        {
                            operation = break_where_tokens_by_AND(where_tokens);
                            if_and = true;
                            break;
                        } else
                        {
                            if_between = false;
                            continue;
                        }

                    }
                }
                if (if_or == false && if_and == false)
                {
                    operation.push_back(where_tokens);
                    if (where_tokens.empty() == false)
                    {
                        operation[0].erase(operation[0].begin());
                        operation[0].erase(operation[0].end() - 1);
                    }
                }


                // if_all
                // 现在获得了一个operation用if_or和if_and标记是否有and或者or
                Table *t = Find_Table(table_name);
                if(t==NULL)
                {
                    In="";
                    continue;
                }
                vector<pair<string, vector<string>>> vec;
                if (operation[0].empty())//无WHERE
                {
                    if (if_all)//select *
                    {
                        pair<string, vector<string>> pair1;
                        pair1 = SelectFile(*t, "", "", "");
                        vector<string> name1 = type(pair1.first);
                        Line(name1.size());
                        cout << "->";
                        for (auto i: name1)
                        {
                            cout << "|" << left << setw(10) << i;
                        }
                        cout << "|" << endl;
                        Line(name1.size());
                        for (int i = 0; i < pair1.second.size(); i++)
                        {
                            vector<string> data = type(pair1.second[i]);
                            cout << "->";
                            for (auto j: data)
                            {
                                cout << "|" << left << setw(10) << j;
                            }
                            cout << "|" << endl;
                        }
                        Line(name1.size());
                    } else
                    {
                        pair<string, vector<string>> pair1;
                        pair1 = SelectFile(*t, "", "", "");
                        for (auto i: select_list)
                        {
                            pair<string, vector<string>> pair2 = SelectData(pair1, i.list_name);
                            vec.push_back(pair2);
                        }
                        Line(vec.size());
                        cout << "->";
                        for (int i = 0; i < vec.size(); i++)
                        {
                            cout << "|" << left << setw(10) << vec[i].first;
                        }
                        cout << "|" << endl;
                        Line(vec.size());
                        for (int i = 0; i < pair1.second.size(); i++)
                        {
                            cout << "->";
                            for (auto j: vec)
                            {
                                cout << "|" << left << setw(10) << j.second[i];
                            }
                            cout << "|" << endl;
                        }
                        Line(vec.size());
                    }
                } else //select 有where
                {
                    if_or_and(if_or, if_and, operation, t, vec, select_list);
                }
            }

                // INSERT INTO test (56,79,58,69,"lucy","男");
            else if (token.getType() == TokenType::INSERT)
            {
                string table_name;
                vector<string> data;
                bool if_left_paren = false;
                bool if_right_paren = false;
                for (auto &i: tokens)
                {
                    if (i.getType() == TokenType::INSERT || i.getType() == TokenType::INTO)
                    {
                        continue;
                    }

                    if (i.getType() == TokenType::LEFT_PAREN)
                    {
                        if_left_paren = true;
                        continue;
                    }

                    if (i.getType() == TokenType::RIGHT_PAREN)
                    {
                        if_right_paren = true;
                        continue;
                    }

                    if (i.getType() == TokenType::IDENTIFIER)
                    {
                        table_name = i.getValue();
                    }

                    if (if_left_paren == true && i.getType() == TokenType::LITERAL)
                    {
                        data.push_back(i.getValue());
                    }
                }

                if (if_right_paren == false)
                {
                    cout << "Invalid statement without right_paren!" << endl;
                }
                if (if_left_paren == false)
                {
                    cout << "Invalid statement without left_paren!" << endl;
                }

                // 以上得到了string table_name 和 vector<string> data
                Table *t = Find_Table(table_name);
                if(t==NULL)
                {
                    In="";
                    continue;
                }

                InsertFile(*t, data, data.size());
            }

                // UPDATE test SET id = 88 WHERE grade = 80 OR grade = 70;
            else if (token.getType() == TokenType::UPDATE)
            {
                string table_name;
                vector<string> data;
                vector<Token> where_tokens = get_where_tokens(tokens);
                if (where_tokens.empty())
                {
                    // set_safe_update = 1;
                    // todo: 执行语法报错提示
                    continue;
                }

                bool if_set = false;
                for (int j = 0; j < tokens.size(); j++)
                {
                    Token i = tokens[j];
                    if (i.getType() == TokenType::WHERE)
                    {
                        // 遇到WHERE直接返回
                        break;
                    }
                    if (i.getType() == TokenType::COMMA)
                    {
                        continue;
                    }

                    if (i.getType() == TokenType::SET)
                    {
                        if_set = true;
                        continue;
                    }

                    if (i.getType() == TokenType::IDENTIFIER)
                    {
                        if (if_set == false)
                        {
                            // 第一个IDENTIFIER为表名
                            table_name = i.getValue();
                        } else
                        {
                            string temp;
                            temp += i.getValue();
                            temp += " ";
                            temp += tokens[j + 2].getValue();
                            data.push_back(temp);
                            j = j + 2;
                            continue;
                        }
                    }
                }

                // 处理where语句
                // 拆分为多个查询
                vector<vector<Token>> operation;
                bool if_or = false, if_and = false;
                bool if_between = false;
                for (auto &i: where_tokens)
                {
                    if (i.getType() == TokenType::BETWEEN)
                    {
                        if_between = true;
                        continue;
                    } else if (i.getType() == TokenType::OR)
                    {
                        operation = break_where_tokens_by_OR(where_tokens);
                        if_or = true;
                        break;
                    } else if (i.getType() == TokenType::AND)
                    {
                        if (if_between == false)
                        {
                            operation = break_where_tokens_by_AND(where_tokens);
                            if_and = true;
                            break;
                        } else
                        {
                            if_between = false;
                            continue;
                        }

                    }
                }
                if (if_or == false && if_and == false)
                {
                    operation.push_back(where_tokens);
                    if (where_tokens.empty() == false)
                    {
                        operation[0].erase(operation[0].begin());
                        operation[0].erase(operation[0].end() - 1);
                    }
                }

                // 已知数据：
                // string table_name
                // vector<string> data
                // if_or和if_and
                // operation
                Table *t = Find_Table(table_name);
                if(t==NULL)
                {
                    In="";
                    continue;
                }
                vector<pair<string, vector<string>>> vec;
                for (auto it: data)
                {
                    it += " ";
                    vector<string> a = type(it);
                    UpdateUse(if_or, if_and, operation, t, vec);
                    pair<string, vector<string>> pair1;
                    if(vec.empty())
                    {
                        continue;
                    }
                    if (t->if_primarykey)
                    {
                        pair1 = vec[t->primary_key.second];
                    } else
                    {
                        pair1 = vec[t->Index.member_name[0].second];
                    }
                    for (auto i: pair1.second)
                    {
                        UpdateFile(*t, pair1.first, i, a[0], a[1]);
                    }
                    vec.clear();
                }
            }

                // DELETE FROM 表名称 WHERE 列名称 = 值
            else if (token.getType() == TokenType::Delete)
            {
                string table_name;
                bool if_from = false;
                vector<Token> where_tokens = get_where_tokens(tokens);

                for (auto &i: tokens)
                {
                    if (i.getType() == TokenType::WHERE)
                        break;

                    if (if_from = true && i.getType() == TokenType::IDENTIFIER)
                        table_name = i.getValue();

                    else if (i.getType() == TokenType::FROM)
                        if_from = true;
                }

                // 处理where语句
                // 拆分为多个查询
                vector<vector<Token>> operation;
                bool if_or = false, if_and = false;
                bool if_between = false;
                for (auto &i: where_tokens)
                {
                    if (i.getType() == TokenType::BETWEEN)
                    {
                        if_between = true;
                        continue;
                    } else if (i.getType() == TokenType::OR)
                    {
                        operation = break_where_tokens_by_OR(where_tokens);
                        if_or = true;
                        break;
                    } else if (i.getType() == TokenType::AND)
                    {
                        if (if_between == false)
                        {
                            operation = break_where_tokens_by_AND(where_tokens);
                            if_and = true;
                            break;
                        } else
                        {
                            if_between = false;
                            continue;
                        }

                    }
                }
                if (if_or == false && if_and == false)
                {
                    operation.push_back(where_tokens);
                    if (where_tokens.empty() == false)
                    {
                        operation[0].erase(operation[0].begin());
                        operation[0].erase(operation[0].end() - 1);
                    }
                }

                // 已知数据：
                // string table_name
                // vector<vector<Token>> operation
                // if_or 和 if_and
                Table *t = Find_Table(table_name);
                if(t==NULL)
                {
                    In="";
                    continue;
                }
                vector<pair<string, vector<string>>> vec;
                UpdateUse(if_or, if_and, operation, t, vec);
                if(vec.empty())
                {
                    In="";
                    continue;
                }
                pair<string, vector<string>> pair1 = vec[t->primary_key.second];
                for (auto i: pair1.second)
                {
                    DeleteFile(*t, pair1.first, i);
                }
                vec.clear();
            }

                // DROP TABLE test;
                // DROP DATABASE test;
            else if (token.getType() == TokenType::DROP)
            {
                bool if_database = false;
                bool if_table = false;
                string name;

                for (auto &i: tokens)
                {
                    if (i.getType() == TokenType::DATABASE)
                    {
                        if_database = true;
                        continue;
                    }
                    if (i.getType() == TokenType::TABLE)
                    {
                        if_table = true;
                        continue;
                    }
                    if (i.getType() == TokenType::IDENTIFIER)
                    {
                        name = i.getValue();
                        break;
                    }
                }

                // 已知数据：
                // string name;
                // if_database  (为true时则应删除名为name的database)
                // if_table
                if (if_database)
                {
                    DeleteDatabase(name, table,buf);
                    if_use_database=0;
                }
                if (if_table)
                {
                    Table *t = Find_Table(name);
                    if(t==NULL)
                    {
                        In="";
                        continue;
                    }
                    DeleteTable(*t, database_name1, table);
                }
            }

                // SHOW DATABASES;
                // SHOW TABLES;
            else if (token.getType() == TokenType::SHOW)
            {
                bool if_table = false;
                bool if_database = false;
                for (auto &i: tokens)
                {
                    if (i.getType() == TokenType::DATABASES)
                    {
                        if_database = true;
                        break;
                    } else if (i.getType() == TokenType::TABLES)
                    {
                        if_table = true;
                        break;
                    }
                }

                // 已知数据:
                // if_table 和 if database
                if (if_table == true)
                {
                    ShowTable(database_name1);
                } else if (if_database == true)
                {
                    ShowDatabase(buf);
                }
            }

                // DESCRIBE TABLE test;
            else if (token.getType() == TokenType::DESCRIBE)
            {
                string table_name;
                for (auto &i: tokens)
                {
                    if (i.getType() == TokenType::IDENTIFIER)
                    {
                        table_name = i.getValue();
                        break;
                    }
                }

                // 已知数据:
                // table_name
                Table *t = Find_Table(table_name);
                if(t==NULL)
                {
                    In="";
                    continue;
                }
                DescribeTable(*t);
            }

                // USE DATABASE test;
            else if (token.getType() == TokenType::USE)
            {
                string database_name;
                for (auto &i: tokens)
                {
                    if (i.getType() == TokenType::IDENTIFIER)
                    {
                        database_name = i.getValue();
                        break;
                    }
                }

                // 已知数据:
                // string database_name
                database_name1 = database_name;
                ChangeDatabase(database_name1, table,buf);
                if_use_database=1;
            }

                // CREATE DATABASE test;(如果已存在)
                // CREATE TABLE test (id INT,name STRING,grade DOUBLE);
            else if (token.getType() == TokenType::CREATE)
            {
                bool if_database = false;
                bool if_table = false;
                for (auto &i: tokens)
                {
                    if (i.getType() == TokenType::TABLE)
                    {
                        if_table = true;
                        break;
                    }
                    if (i.getType() == TokenType::DATABASE)
                    {
                        if_database = true;
                        break;
                    }
                }

                // CREATE DATABASE
                if (if_database == true)
                {
                    // CREATE DATABASE test
                    string database_name;
                    for (auto &i: tokens)
                    {
                        if (i.getType() == TokenType::IDENTIFIER)
                        {
                            database_name = i.getValue();
                            break;
                        }
                    }


                    // todo
                    // 已知数据:
                    // string database_name
                    CreateMkdir(database_name);
                }
                    // CREATE TABLE
                else if (if_table == true)
                {
                    string table_name;
                    vector<string> data;

                    for (auto &i: tokens)
                    {
                        if (i.getType() == TokenType::IDENTIFIER)
                        {
                            table_name = i.getValue();
                            break;
                        }
                    }

                    bool if_left_paren = false;
                    bool if_right_paren = false;

                    string temp;
                    for (auto &i: tokens)
                    {
                        if (i.getType() == TokenType::LEFT_PAREN)
                        {
                            if_left_paren = true;
                            continue;
                        } else if (i.getType() == TokenType::RIGHT_PAREN)
                        {
                            data.push_back(temp);
                            temp.clear();
                            if_right_paren = true;
                            break;
                        } else if (if_left_paren == true && if_right_paren == false)
                        {
                            if (i.getType() == TokenType::IDENTIFIER)
                            {
                                temp += i.getValue();
                            } else if (i.getType() == TokenType::PRIMARYKEY)
                            {
                                temp += i.getValue();
                            } else if (i.getType() == TokenType::INT || i.getType() == TokenType::DOUBLE ||
                                       i.getType() == TokenType::STRING)
                            {
                                temp += " ";
                                if (i.getType() == TokenType::INT)
                                {
                                    temp += "int";
                                } else if (i.getType() == TokenType::DOUBLE)
                                {
                                    temp += "double";
                                } else if (i.getType() == TokenType::STRING)
                                {
                                    temp += "string";
                                }
                            } else if (i.getType() == TokenType::COMMA)
                            {
                                data.push_back(temp);
                                temp.clear();
                            }
                        }
                    }

                    // 已知数据:
                    // string table_name
                    // vector<string> data
                    // data内容：[id int ][name string ][grade double ]
                    // 输入的时候要输入int, string, double的大写
                    for (auto &i: data)
                    {
                        i += " ";
                    }
                    Table table1=CreateTable(table_name, data.size(), data,
                                             database_name1,if_use_database);
                    if(table1.name==table_name) table.push_back(table1);
                }
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout<<endl;
            cout<<"-> ----------------------------------------------------------------------------------------------------------"<<endl;
            cout << "->Time taken by program: " << duration.count()/1000000.0 << " seconds" <<endl;
            cout<<endl;
            cout<<endl;
            In.clear();
        } else
        {
            In += in;
            In += " ";
        }
    }
}








