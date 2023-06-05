//
// Created by 问天 on 2023/4/19.
//

#ifndef BIN_KEYBOARD_H
#define BIN_KEYBOARD_H
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include "cmd.h"

using namespace std;

void SetFontSize(int x,int y) {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    cfi.dwFontSize.X = x; // 字体宽度
    cfi.dwFontSize.Y = y; // 字体高度

    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
void GetFontSize(int& width, int& height) {
    CONSOLE_FONT_INFOEX cfi = { 0 };
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    width = cfi.dwFontSize.X;
    height = cfi.dwFontSize.Y;
}
string Password()
{
    string password;
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b' && password.length() > 0) // 处理退格键
        {
            password.pop_back();
            putchar('\b');
            putchar(' ');
            putchar('\b');
        }
        else if (ch != '\b') // 处理其他键
        {
            password.push_back(ch);
            putchar('*');
        }
    }
    std::cout<<std::endl;
    return password;
}

#endif //BIN_KEYBOARD_H
