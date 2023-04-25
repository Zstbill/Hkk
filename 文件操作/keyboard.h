//
// Created by 问天 on 2023/4/19.
//

#ifndef BIN_KEYBOARD_H
#define BIN_KEYBOARD_H
#include <conio.h>
#include <Windows.h>
#include "cmd.h"

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
void tickInput()
{
    while (_kbhit())
    {
        int key = _getch();
        if (key == 0xE0 || key == 0)
        {
            // 处理功能键或方向键
            key = _getch();
            switch (key)
            {
                case 72:
                    SetFontSize(5,10);
                    // Up arrow
                    break;
                case 80:
                    SetFontSize(10,20);

                    // Down arrow
                    break;
                case 75:
                    // Left arrow
                    break;
                case 77:
                    // Right arrow
                    break;
                    // 如果需要处理更多的功能键,可在此处添加
                default:
                    break;
            }
        }
        else
        {
            // 在此处处理按键输入,输入按键即为对应字符
            // 特别的, Enter键key为0x0D('\r'), Backspace键为0x08, Esc键为0x1B, Tab键为0x09
        }
    }
}
#endif //BIN_KEYBOARD_H
