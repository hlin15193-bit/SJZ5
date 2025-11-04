#pragma once

// 蓝图函数库类型枚举（用于索引不同功能的蓝图类）
enum class BlueprintType : int32_t {
	GameplayStatics,               // 游戏性静态函数库
	KismetTextLibrary,             // Kismet文本处理函数库
	KismetStringLibrary,           // Kismet字符串处理函数库
	GameplayBlueprintHelper,       // 游戏性蓝图辅助函数库
	GPScalabilityBlueprintTools,   // GP scalability蓝图工具（图形/性能设置相关）
	KismetSystemLibrary,           // Kismet系统函数库
	BlueprintFunctionLibrary,      // 基础蓝图函数库
	Max                            // 枚举最大值（用于边界检查）
};

// DFM游戏模式枚举（定义不同的游戏玩法模式）
enum class EDFMGamePlayMode : uint8_t  // 大小为9（包含所有枚举值）
{
	None = 0,                          // 无模式
	GamePlayMode_SOL = 1,              // SOL模式（可能是单人或特定玩法）
	GamePlayMode_Raid = 2,             // 突袭模式
	GamePlayMode_IrisDiscovery = 3,    // 虹膜探索模式
	GamePlayMode_Conquest = 4,         // 征服模式
	GamePlayMode_Breakthrough = 5,     // 突破模式
	GamePlayMode_SafeHouse = 6,        // 安全屋模式
	GamePlayMode_Intro = 7,            // 介绍/开场模式
	EDFMGamePlayMode_MAX = 8           // 枚举最大值
};

// 轮廓效果类型枚举（定义实体周围的高亮轮廓效果）
enum class EOutLineEffectType : int32_t {
	OutLineType_None = 0,                          // 无轮廓
	OutLineType_ProxSensor = 1,                    //  proximity传感器轮廓（接近感应）
	OutLineType_ArrowMark = 2,                     // 箭头标记轮廓
	OutLineType_MedicGun = 4,                      // 医疗枪相关轮廓
	OutLineType_MedicGunHit = 8,                   // 医疗枪命中轮廓
	OutLineType_SupportEffect = 16,                // 支援效果轮廓
	OutLineType_DyingLockEnemy = 32,               // 濒死状态锁定敌人轮廓
	OutLineType_Breakthrough_Defender = 64,        // 突破模式防御方轮廓
	OutLineType_DyingShowTeammateCanRescueSelf = 128,  // 濒死时显示可救援自己的队友轮廓
	OutLineType_DyingShowCampMedicCanRescueSelf = 256, // 濒死时显示营地医疗兵可救援自己的轮廓
	OutLineType_Locking = 512,                     // 锁定中轮廓
	OutLineType_LockComplete = 1024,               // 锁定完成轮廓
	OutLineType_LockFire = 2048,                   // 锁定开火轮廓
	OutLineType_OnlyMarker = 4096,                 // 仅标记轮廓
	OutLineType_SOL_OB_Teammate = 8192,            // SOL模式观察员视角队友轮廓
	OutLineType_SOL_OB_Enemy = 16384,              // SOL模式观察员视角敌人轮廓
	OutLineType_SOL_OB_AI = 32768,                 // SOL模式观察员视角AI轮廓
	OutLineType_SOL_OB_FreeCamera = 65536,         // SOL模式自由相机视角轮廓
	OutLineType_Breakthrough_OB_Attacker = 131072, // 突破模式观察员视角进攻方轮廓
	OutLineType_OBorReplay = 262144,               // 观察员或回放模式轮廓
	OutLineType_MAX = 262145                       // 枚举最大值
};

// 主流程状态枚举（定义游戏的主要流程阶段）
enum class EMainFlowState : uint8_t {
	Default = 0,              // 默认状态
	Lobby = 1,                // 大厅状态
	Loading = 2,              // 加载状态
	SafeHouse = 3,            // 安全屋状态
	InGame = 4,               // 游戏中状态
	LobbyBHD = 5,             // BHD大厅状态（可能是特定模式的大厅）
	EMainFlowState_MAX = 6    // 枚举最大值
};