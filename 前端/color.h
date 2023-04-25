//
// Created by 问天 on 2023/4/19.
//

#ifndef BIN_COLOR_H
#define BIN_COLOR_H
#include <iostream>

const char ESC = '\033';

const int ATTR_NORMAL = 0; // 传入此常量可将当前颜色设置恢复为预设值

//前景色
enum ConsoleForeground : int
{
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,
    FG_DEFAULT = 39,
    FG_LIGHT_BLACK = 90,
    FG_LIGHT_RED = 91,
    FG_LIGHT_GREEN = 92,
    FG_LIGHT_YELLOW = 93,
    FG_LIGHT_BLUE = 94,
    FG_LIGHT_MAGENTA = 95,
    FG_LIGHT_CYAN = 96,
    FG_LIGHT_WHITE = 97
};

// 背景色
enum ConsoleBackground : int
{
    BG_BLACK = 40,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_YELLOW = 43,
    BG_BLUE = 44,
    BG_MAGENTA = 45,
    BG_CYAN = 46,
    BG_WHITE = 47,
    BG_DEFAULT = 49,
    BG_LIGHT_BLACK = 100,
    BG_LIGHT_RED = 101,
    BG_LIGHT_GREEN = 102,
    BG_LIGHT_YELLOW = 103,
    BG_LIGHT_BLUE = 104,
    BG_LIGHT_MAGENTA = 105,
    BG_LIGHT_CYAN = 106,
    BG_LIGHT_WHITE = 107
};

void setColorAttr(int attr)
{
    std::cout << ESC << "[" << attr << "m";
}
#endif //BIN_COLOR_H
