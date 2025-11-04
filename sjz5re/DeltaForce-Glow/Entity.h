#pragma once
class C_Entity  // 实体管理类（处理游戏实体相关逻辑）
{
public:

	// 判断当前是否为营地模式
	// 参数DFMGamePlayerMode：当前游戏模式
	// 返回值：true表示是营地模式，false表示不是
	static bool GetIsCamp(EDFMGamePlayMode DFMGamePlayerMode) {
		bool bIsCamp = false;
		switch (DFMGamePlayerMode)
		{
		case EDFMGamePlayMode::None:
			break;
		case EDFMGamePlayMode::GamePlayMode_SOL:
			bIsCamp = false;  // SOL模式不属于营地模式
			break;
		case EDFMGamePlayMode::GamePlayMode_Raid:
			break;
		case EDFMGamePlayMode::GamePlayMode_IrisDiscovery:
			break;
		case EDFMGamePlayMode::GamePlayMode_Conquest:
			bIsCamp = false;  // 征服模式不属于营地模式
			break;
		case EDFMGamePlayMode::GamePlayMode_Breakthrough:
			bIsCamp = true;   // 突破模式属于营地模式
			break;
		case EDFMGamePlayMode::GamePlayMode_SafeHouse:
			break;
		case EDFMGamePlayMode::GamePlayMode_Intro:
			break;
		case EDFMGamePlayMode::EDFMGamePlayMode_MAX:
			break;
		default:
			break;
		}
		return bIsCamp;
	}

	// 更新实体数据（主逻辑函数，通常每帧调用）
	static void UpData() {
		// 获取当前主流程状态（如大厅、加载中、游戏中）
		auto MainFlowState = GameData::BlueprintFunctionLibrary[(int)BlueprintType::GPScalabilityBlueprintTools]->GetMainFlowState();
		// 判断是否处于游戏中或安全屋状态
		GameData::bIsInGame = MainFlowState == EMainFlowState::InGame || MainFlowState == EMainFlowState::SafeHouse;
		if (!GameData::bIsInGame) return;  // 不在游戏中则退出更新

		// 获取游戏视口客户端对象
		auto GameViewportClient = Mem::Read<UGameViewportClient*>((uintptr_t)GameData::Engine + Offsets::GameViewport);
		if (IsBadPtr(GameViewportClient)) return;  // 视口对象无效则退出

		// 从视口客户端中读取世界对象
		GameData::GWorld = Mem::Read<UWorld*>((uintptr_t)GameViewportClient + Offsets::World);
		if (IsBadPtr(GameData::GWorld)) return;  // 世界对象无效则退出

		// 获取游戏状态对象
		auto GameState = GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayBlueprintHelper]->GetGPGameState(GameData::GWorld);
		if (IsBadPtr(GameState)) return;  // 游戏状态无效则退出

		// 读取当前游戏模式
		auto DFMGamePlayerMode = Mem::Read<EDFMGamePlayMode>(GameState + Offsets::DFMGamePlayerMode);

		// 更新营地模式状态
		GameData::bIsCampMode = GetIsCamp(DFMGamePlayerMode);

		// 获取本地玩家角色
		GameData::MyCharacter = GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayBlueprintHelper]->GetLocalGPCharacter(GameData::GWorld);
		if (IsBadPtr(GameData::MyCharacter)) return;  // 本地角色无效则退出

		// 检查是否为本地玩家
		if (GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayBlueprintHelper]->IsLocalPlayer(GameData::MyCharacter)) {

			// 获取生命值组件
			auto HealthDataComponent = GameData::MyCharacter->GetHealthComp();
			if (IsBadPtr(HealthDataComponent)) return;  // 生命值组件无效则退出

			// 仅当角色存活时更新队伍索引
			if (HealthDataComponent->GetHealth() > 0 && !GameData::MyCharacter->IsDead()) {

				// 获取队伍组件
				auto TeamComp = GameData::MyCharacter->GetTeamComp();
				if (IsBadPtr(TeamComp)) return;  // 队伍组件无效则退出

				// 根据模式获取队伍索引（营地模式用Camp，其他用TeamID）
				if (GameData::bIsCampMode)
					GameData::TeamIndex = TeamComp->GetCamp();
				else
					GameData::TeamIndex = TeamComp->GetTeamID();
			}
		}

		// 获取所有DFM角色实体
		TArray<AActor*> ACharacters = {};
		GameData::BlueprintFunctionLibrary[(int)BlueprintType::GameplayStatics]->GetAllActorsOfClass(GameData::GWorld, ADFMCharacter::StaticClass(), &ACharacters);

		// 遍历所有角色实体
		for (auto i = 0; i < ACharacters.Num(); i++) {

			// 转换为DFM角色类型
			const auto& pCharacter = ACharacters[i]->As<ADFMCharacter*>();
			// 跳过本地玩家和已死亡的角色
			if (pCharacter == GameData::MyCharacter || !pCharacter->IsAlive()) continue;

			// 获取目标角色的队伍组件
			auto TeamComp = pCharacter->GetTeamComp();
			if (IsBadPtr(TeamComp)) continue;  // 队伍组件无效则跳过

			// 获取目标角色的队伍索引
			auto TeamIndex = GameData::bIsCampMode ? TeamComp->GetCamp() : TeamComp->GetTeamID();
			// 跳过同队伍的角色
			if (TeamIndex == GameData::TeamIndex) continue;

			// 获取目标角色的轮廓效果组件
			auto DFMOutLineEffectComponent = Mem::Read<UGPOutLineEffectComponent*>(
				reinterpret_cast<uintptr_t>(pCharacter) + Offsets::DFMOutLineEffectComponent);
			if (IsBadPtr(DFMOutLineEffectComponent)) continue;  // 轮廓组件无效则跳过

			// 根据目标类型设置所需的轮廓效果
			auto desiredEffect = EOutLineEffectType::OutLineType_None;
			if (pCharacter->IsAI()) {
				// AI角色使用特定轮廓（濒死时显示可救援的队友样式）
				desiredEffect = EOutLineEffectType::OutLineType_DyingShowTeammateCanRescueSelf;
			}
			else {
				// 玩家角色使用接近传感器样式轮廓
				desiredEffect = EOutLineEffectType::OutLineType_ProxSensor;
			}

			// 读取当前轮廓效果（偏移0x1D8为轮廓效果状态）
			auto currentEffect = Mem::Read<EOutLineEffectType>(
				reinterpret_cast<uintptr_t>(DFMOutLineEffectComponent) + 0x1D8);
			// 如果当前效果与所需效果不同，则更新
			if (currentEffect != desiredEffect) {
				DFMOutLineEffectComponent->PlayOutLineEffect(desiredEffect);
			}
		}

		// 释放角色数组内存（防止内存泄漏）
		if (ACharacters.Data)
			Utils::FastCall(Offsets::FreeFunc, ACharacters.Data);
	}
};