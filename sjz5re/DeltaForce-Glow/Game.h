#pragma once

namespace GameData {  // 游戏数据命名空间（存储全局游戏状态和对象指针）

	UWorld* GWorld = nullptr;  // 全局世界对象指针（游戏世界的核心对象）
	UObject* Engine;           // 引擎对象指针
	// 蓝图函数库数组（存储各种蓝图功能类的指针，最多10个）
	UBlueprintFunctionLibrary* BlueprintFunctionLibrary[10] = { nullptr };
	AGPCharacter* MyCharacter = nullptr;  // 本地玩家角色对象指针

	bool bIsInGame = false;    // 是否在游戏中（非菜单/大厅状态）
	bool bIsCampMode = false;  // 是否处于营地模式
	uint32_t TeamIndex = NULL; // 玩家的队伍索引
}