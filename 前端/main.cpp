#include <iostream>
#include <Windows.h>
#include <codecvt>
#include <fstream>
#include <chrono>
#include <thread>
#include "keyboard.h"
#include "cmd.h"
#include "color.h"
#include "file.h"
using namespace std;

void setConsoleUTF8()
{
    SetConsoleOutputCP(CP_UTF8);
#pragma warning (disable: 4996)
    std::locale utf8(std::locale(), new std::codecvt_utf8_utf16<wchar_t>);
    std::wcout.imbue(utf8);
#pragma warning (default: 4996)
}
int main()
{
    setConsoleUTF8();
    EnableVTMode();//初始化VT
    SetFontSize(10,20);
    setColorAttr(30);
    setColorAttr(107);
    while(!CheckScreenSize())
        system("pause");
    clearScreen();
    string pr;
    string name;
    string password;
    ifstream print("Welcome.txt", ios::in);//生成字符画
    while(getline(print,pr))
    {
        cout<<pr<<endl;
        this_thread::sleep_for(chrono::milliseconds(66));
    }
    print.close();

    ifstream acc1("account.txt", ios::in);//处理帐户
    if(!acc1)
    {
        ofstream acc("account.txt", ios::out);
        string a,p;
        cout<<"For the first open C++SQL, please set your account name and password:"<<endl;
        cout<<"name:";
        cin>>a;
        acc<<a<<endl;
        cout<<"password:";
        cin>>p;
        acc<<a<<endl;
        acc.close();
    }
    else
    {
        string a[2],p[2];
        acc1>>a[0]>>p[0];
        cout<<"Please enter your account name:";
        while(cin>>a[1])
        {
            if(a[1]==a[0])
            {
                cout<<"Please enter your password:";
                while(1)
                {
                    p[1]=Password();
                    if(p[1]!=p[0])
                    {
                        cerr<<"Please enter the right password:";
                    }
                    else break;
                }
                break;
            }
            else cerr<<"Please enter the right account name:";
        }
    }
    acc1.close();
    setColorAttr(0);
    clearScreen();
//语法树

    system("pause");
    return 0;
}
