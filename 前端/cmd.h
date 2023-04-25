//
// Created by 问天 on 2023/4/19.
//

#ifndef BIN_CMD_H
#define BIN_CMD_H
#include <Windows.h>
#include <iostream>

bool EnableVTMode()//VT mode
{
    // Set output mode to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return false;
    }
    return true;
}

void setCursorPos(int x, int y)//指针位置
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = x;
    Position.Y = y;

    SetConsoleCursorPosition(hOut, Position);

}

COORD getCursorPos()//指针位置
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hOut, &csbi);

    Position.X = csbi.dwCursorPosition.X;
    Position.Y = csbi.dwCursorPosition.Y;

    return Position;
}

void moveCursorPos(int vecX, int vecY)//指针位置
{
    auto initPos = getCursorPos();
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = initPos.X + vecX;
    Position.Y = initPos.Y + vecY;

    SetConsoleCursorPosition(hOut, Position);
}
void clearScreen()
{
    std::cout << "\033[H\033[2J\033[3J";
}
bool CheckScreenSize()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    if(rows == 30 && columns == 120) return 1;
    else
    {
        std::cout << "Please set the window size:120 * 30 " <<std::endl;
        return 0;
    }
}
#endif //BIN_CMD_H
