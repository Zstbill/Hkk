#include <iostream>
#include <Windows.h>
#include <codecvt>
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
    setColorAttr(100);
    setColorAttr(31);
    cout<<"Hello world"<<endl;
    setColorAttr(0);
    COORD c=getCursorPos();
    cout<<"Hello world"<<endl;
    cout<<"调整字体大小";
    while(true)
    {
        tickInput();
        int x,y;
        GetFontSize(x,y);
        if(y==10)
        {
            break;
        }
    }
        tickInput();
    system("pause");
    return 0;
}
