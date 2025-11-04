#pragma once

namespace Offsets {  // 偏移量命名空间（存储各种内存偏移地址）

	uint64_t StaticFindObject;             // 静态查找对象的偏移量
	uint64_t GetName;                      // 获取名称的偏移量
	uint64_t FreeFunc;                     // 释放函数的偏移量
	uint64_t GEngine;                      // 全局引擎对象的偏移量

	uint64_t GameViewport;                 // 游戏视口的偏移量
	uint64_t World;                        // 世界对象的偏移量
	uint64_t Levels;                       // 关卡列表的偏移量
	uint64_t GameState;                    // 游戏状态的偏移量
	uint64_t DFMGamePlayerMode;            // DFM游戏玩家模式的偏移量
	uint64_t PlayerController;             // 玩家控制器的偏移量
	uint64_t PlayerCameraManager;          // 玩家相机管理器的偏移量
	uint64_t PlayerState;                  // 玩家状态的偏移量
	uint64_t PlayerNamePrivate;            // 私有玩家名称的偏移量
	uint64_t RootComponent;                // 根组件的偏移量
	uint64_t DFMOutLineEffectComponent;    // DFM轮廓效果组件的偏移量
}