#pragma once

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unordered_map> 
#include <string>
#include <utility>  
#include <mutex>
#include <thread>
#include <vector>
#include <format>


//#define DEBUG_ENABLE   // 取消注释以启用调试
#ifdef DEBUG_ENABLE
#define DeBug(...)                                                \
        do {                                                          \
            if (GetConsoleWindow() == NULL) {                         \
                AllocConsole();                                       \
                freopen_s((FILE**)stdin, "CONIN$", "r", stdin);       \
                freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);    \
            }                                                         \
            printf(__VA_ARGS__);                                      \
        } while (0)
#else
#define DeBug(...) ((void)0)
#endif

#include "Enum.h"       // 枚举定义头文件
#include "Utils.h"      // 工具函数头文件
#include "Hooks.h"      // 钩子相关头文件
#include "Offsets.h"    // 内存偏移量头文件
#include "Engine.h"     // 引擎相关头文件
#include "Game.h"       // 游戏相关头文件
#include "Entity.h"     // 实体相关头文件
#include "Hack.h"       // 外挂功能头文件
#include "Hidedll.h"    // DLL隐藏功能头文件