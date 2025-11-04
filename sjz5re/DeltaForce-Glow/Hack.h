#pragma once
class C_Hack  // 外挂功能主类
{
public:

	// 渲染后钩子函数（用于绘制外挂界面等）
	// 参数GameViewport：游戏视口对象，Canvas：画布对象
	static void hkPostRender(void* GameViewport, UCanvas* Canvas)
	{
		C_Entity::UpData();  // 更新实体数据

		// 调用原始的PostRender函数（索引95）
		return Utils::FastCall<void>(RenderHook.GetOriginal<void*>(95), GameViewport, Canvas);
	}

	// 查找指定类中变量的偏移量
	// 参数Class：类名（宽字符串），varName：变量名
	// 返回值：找到的偏移量，失败返回NULL
	static int FindOffset(const wchar_t* Class, const char* varName) {

		// 查找指定的类对象
		auto CurrentObject = (UStruct*)UObject::FindObject(Class);
		if (IsBadPtr(CurrentObject)) return NULL;  // 类对象无效则返回

		// 遍历类的所有子属性查找目标变量
		for (auto Property = CurrentObject->ChildProperties; !IsBadPtr(Property); Property = Property->Next) {

			auto Offset = Property->Offset;  // 获取属性偏移量
			// 筛选合理范围内的偏移量，并比对变量名
			if (Offset > 0 && Offset < 0xFFFF) {
				if (strcmp(Property->Name.GetName().c_str(), varName) == 0) {
					return Offset;  // 找到匹配的变量，返回偏移量
				}
			}
		}
		return NULL;  // 未找到变量
	}

	// 初始化所有内存偏移量
	static void InitOffsets() {

		// 手动设置基础偏移量（这些通常需要随游戏更新而更新）
		Offsets::StaticFindObject = 0x14AE8C310;
		DeBug(("StaticFindObject:0x%llX\n"), Offsets::StaticFindObject);

		Offsets::GetName = 0x14AB2CA30;
		DeBug(("GetName:0x%llX\n"), Offsets::GetName);

		Offsets::FreeFunc = 0x14A9F0B90;
		DeBug(("FreeFunc:0x%llX\n"), Offsets::FreeFunc);

		Offsets::GEngine = 0x153A3D2A8;
		DeBug(("GEngine:0x%llX\n"), Offsets::GEngine);

		// 以下偏移量通过FindOffset自动查找（可兼容游戏小更新）
		Offsets::GameViewport = FindOffset((L"Engine.Engine"), ("GameViewport"));
		DeBug(("GameViewport:0x%X\n"), Offsets::GameViewport);

		Offsets::World = FindOffset((L"Engine.GameViewportClient"), ("World"));
		DeBug(("World:0x%X\n"), Offsets::World);

		Offsets::Levels = FindOffset((L"Engine.World"), ("Levels"));
		DeBug(("Levels:0x%X\n"), Offsets::Levels);

		Offsets::GameState = FindOffset((L"Engine.World"), ("GameState"));
		DeBug(("GameState:0x%X\n"), Offsets::GameState);

		Offsets::DFMGamePlayerMode = FindOffset((L"DFMGameplay.DFMGameState"), ("DFMGamePlayerMode"));
		DeBug(("DFMGamePlayerMode:0x%X\n"), Offsets::DFMGamePlayerMode);

		Offsets::PlayerController = FindOffset((L"Engine.Player"), ("PlayerController"));
		DeBug(("PlayerController:0x%X\n"), Offsets::PlayerController);

		Offsets::PlayerCameraManager = FindOffset((L"Engine.PlayerController"), ("PlayerCameraManager"));
		DeBug(("PlayerCameraManager:0x%X\n"), Offsets::PlayerCameraManager);

		Offsets::PlayerState = FindOffset((L"Engine.Pawn"), ("PlayerState"));
		DeBug(("PlayerState:0x%X\n"), Offsets::PlayerState);

		Offsets::PlayerNamePrivate = FindOffset((L"Engine.PlayerState"), ("PlayerNamePrivate"));
		DeBug(("PlayerNamePrivate:0x%X\n"), Offsets::PlayerNamePrivate);

		Offsets::RootComponent = FindOffset((L"Engine.Actor"), ("RootComponent"));
		DeBug(("RootComponent:0x%X\n"), Offsets::RootComponent);

		Offsets::DFMOutLineEffectComponent = FindOffset((L"DFMGameplay.DFMCharacter"), ("DFMOutLineEffectComponent"));
		DeBug(("DFMOutLineEffectComponent:0x%X\n"), Offsets::DFMOutLineEffectComponent);

		// 初始化蓝图函数库指针
		GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayStatics] = (UBlueprintFunctionLibrary*)UObject::FindObject((L"Engine.GameplayStatics"));
		DeBug(("BlueprintFunctionLibrary->%llX\n"), GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayStatics]);

		GameData::BlueprintFunctionLibrary[(int)BlueprintType::KismetTextLibrary] = (UBlueprintFunctionLibrary*)UObject::FindObject((L"Engine.KismetTextLibrary"));
		DeBug(("BlueprintFunctionLibrary->%llX\n"), GameData::BlueprintFunctionLibrary[(int)BlueprintType::KismetTextLibrary]);

		GameData::BlueprintFunctionLibrary[(int)BlueprintType::KismetStringLibrary] = (UBlueprintFunctionLibrary*)UObject::FindObject((L"Engine.KismetStringLibrary"));
		DeBug(("BlueprintFunctionLibrary->%llX\n"), GameData::BlueprintFunctionLibrary[(int)BlueprintType::KismetStringLibrary]);

		GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayBlueprintHelper] = (UBlueprintFunctionLibrary*)UObject::FindObject((L"GPGameplay.GameplayBlueprintHelper"));
		DeBug(("BlueprintFunctionLibrary->%llX\n"), GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayBlueprintHelper]);

		GameData::BlueprintFunctionLibrary[(int)BlueprintType::GPScalabilityBlueprintTools] = (UBlueprintFunctionLibrary*)UObject::FindObject((L"GPSettings.GPScalabilityBlueprintTools"));
		DeBug(("BlueprintFunctionLibrary->%llX\n"), GameData::BlueprintFunctionLibrary[(int)BlueprintType::GPScalabilityBlueprintTools]);

		GameData::BlueprintFunctionLibrary[(int)BlueprintType::KismetSystemLibrary] = (UBlueprintFunctionLibrary*)UObject::FindObject((L"Engine.KismetSystemLibrary"));
		DeBug(("BlueprintFunctionLibrary->%llX\n"), GameData::BlueprintFunctionLibrary[(int)BlueprintType::KismetSystemLibrary]);

		GameData::BlueprintFunctionLibrary[(int)BlueprintType::BlueprintFunctionLibrary] = (UBlueprintFunctionLibrary*)UObject::FindObject((L"Engine.BlueprintFunctionLibrary"));
		DeBug(("BlueprintFunctionLibrary->%llX\n"), GameData::BlueprintFunctionLibrary[(int)BlueprintType::BlueprintFunctionLibrary]);

		// 读取引擎对象
		GameData::Engine = Mem::Read<UObject*>(Offsets::GEngine);
	}

	// 初始化所有钩子
	static void InitHooks() {

		// 检查引擎对象是否有效
		if (!IsBadPtr(GameData::Engine)) {
			// 获取游戏视口客户端对象
			auto GameViewportClient = *(UGameViewportClient**)((uintptr_t)GameData::Engine + Offsets::GameViewport);
			if (!IsBadPtr(GameViewportClient)) {

				// 初始化渲染钩子并绑定PostRender函数（索引95）
				RenderHook.Initialize(GameViewportClient);
				RenderHook.Bind(95, hkPostRender);
			}
		}
	}

	// 初始化函数（版本1）
	// 参数hModule：DLL模块句柄
	static void Init(HMODULE hModule) {

		// 在大厅中按HOME键初始化（会发出提示音）
		while (!(GetAsyncKeyState(VK_HOME) & 0x8000)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		Beep(800, 500);  // 发出提示音

		// 初始化偏移量和钩子
		InitOffsets();
		InitHooks();
	}

	// 初始化函数（版本2，包含范围伤害功能）
	static void Init2(HMODULE hModule) {
		// 范围伤害相关变量
		BYTE 范围伤害[5] = { 0 };  // 存储原始内存字节
		BOOL 范围伤害开关 = FALSE;  // 功能开关状态
		BOOL 范围伤害初始化 = FALSE;  // 初始化状态标记
		BOOL F1键按下 = FALSE;  // 按键状态标记

		// 范围伤害功能的目标内存地址（基地址+偏移）
		DWORD64 地址 = 0x140000000 + 0x199E3A5;

		// 读取内存字节集的Lambda函数
		auto 读字节集 = [](DWORD64 地址, BYTE* 缓冲区, SIZE_T 大小) -> BOOL {
			HANDLE 进程句柄 = GetCurrentProcess();  // 获取当前进程句柄
			SIZE_T 读取字节数;
			// 读取指定地址的内存
			if (ReadProcessMemory(进程句柄, (LPCVOID)地址, 缓冲区, 大小, &读取字节数)) {
				return 读取字节数 == 大小;  // 检查是否读取完整
			}
			return FALSE;
			};

		// 写入内存字节集的Lambda函数（带内存保护修改）
		auto 写字节集 = [](DWORD64 地址, BYTE* 数据, SIZE_T 大小) -> BOOL {
			HANDLE 进程句柄 = GetCurrentProcess();  // 获取当前进程句柄
			DWORD 旧保护;
			// 修改内存保护属性为可读写
			if (VirtualProtectEx(进程句柄, (LPVOID)地址, 大小, PAGE_EXECUTE_READWRITE, &旧保护)) {
				SIZE_T 写入字节数;
				// 写入数据
				BOOL 结果 = WriteProcessMemory(进程句柄, (LPVOID)地址, 数据, 大小, &写入字节数);
				// 恢复原始内存保护属性
				VirtualProtectEx(进程句柄, (LPVOID)地址, 大小, 旧保护, &旧保护);
				return 结果 && (写入字节数 == 大小);  // 检查是否写入完整
			}
			return FALSE;
			};

		// 在大厅中按HOME键初始化（会发出提示音）
		while (!(GetAsyncKeyState(VK_HOME) & 0x8000)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		Beep(800, 500);  // 发出提示音

		// 初始化范围伤害功能（读取原始内存字节）
		if (读字节集(地址, 范围伤害, 5)) {
			范围伤害初始化 = TRUE;  // 标记初始化成功
		}

		// 字节转十六进制字符串的Lambda函数（用于调试）
		auto 字节转十六进制 = [](BYTE* 数据, SIZE_T 大小) -> std::string {
			std::string 结果;
			char 缓冲[3];
			for (SIZE_T i = 0; i < 大小; i++) {
				sprintf_s(缓冲, sizeof(缓冲), "%02X", 数据[i]);
				结果 += 缓冲;
				if (i < 大小 - 1) 结果 += " ";  // 字节间添加空格分隔
			}
			return 结果;
			};

		// 初始化偏移量和钩子
		InitOffsets();
		InitHooks();

		// 主循环（处理按键和功能逻辑）
		while (true) {
			// 检测F1键切换范围伤害功能
			if (范围伤害初始化) {
				if (GetAsyncKeyState(VK_F1) & 0x8000) {  // F1键被按下
					if (!F1键按下) {  // 处理按键按下事件（防止重复触发）
						F1键按下 = TRUE;
						范围伤害开关 = !范围伤害开关;  // 切换开关状态

						if (范围伤害开关) {
							// 开启范围伤害：写入修改后的字节
							BYTE 新字节[] = { 243, 15, 16, 64, 60 };
							写字节集(地址, 新字节, 5);
						}
						else {
							// 关闭范围伤害：恢复原始字节
							写字节集(地址, 范围伤害, 5);
						}
					}
				}
				else {
					F1键按下 = FALSE;  // 按键释放
				}
			}

			// 短暂休眠，降低CPU占用
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	// 初始化函数（版本2的备用实现，仅循环休眠）
	//static void Init2(HMODULE hModule) {
	//
	//	while (true) {
	//
	//		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	//
	//
	//	}
	//	Beep(800, 500);
	//
	//	//InitOffsets();
	//	//InitHooks();
	//}
};