// 动态数组模板类（类似UE引擎的TArray）
template<class T>
struct TArray
{
public:
	T* Data;         // 数据存储指针
	int32_t Count;   // 当前元素数量
	int32_t Max;     // 数组最大容量

	// 构造函数：初始化空数组
	TArray()
	{
		Data = nullptr;
		Max = 0;
		Count = 0;
	}

	// 获取当前元素数量
	int Num() const
	{
		return Count;
	}

	// 重载[]运算符：访问指定索引的元素
	T& operator[](int i)
	{
		return Data[i];
	}

	// 重载[]运算符（const版本）
	const T& operator[](int i) const
	{
		return Data[i];
	}

	// 检查索引是否有效
	bool IsValidIndex(int i) const
	{
		return i < Num();
	}

	// 释放数组内存
	void Free()
	{
		delete[] Data;
		Count = 0;
		Max = 0;
		Data = nullptr;
	}

	// 添加元素到数组
	void Add(const T& Element)
	{
		// 当剩余空间不足3时，预留更多空间
		if (GetSlack() <= 3) Reserve(3);

		Data[Count] = Element;
		Count++;
	}

	// 获取剩余可用空间
	int32_t GetSlack() const { return Max - Count; }

	// 预留指定数量的空间
	void Reserve(int32_t Count)
	{
		Max += Count;
		T* NewData = new T[Max];  // 分配新内存

		// 复制旧数据到新内存
		if (Data)
		{
			std::copy(Data, Data + Count, NewData);
			delete[] Data;  // 释放旧内存
		}

		Data = NewData;  // 指向新内存
	}
};

// 三维向量结构体（用于表示3D空间中的位置、方向等）
struct FVector
{
public:
	float X;  // X轴分量
	float Y;  // Y轴分量
	float Z;  // Z轴分量

	// 构造函数：初始化零向量
	FVector() : X(0), Y(0), Z(0) {}

	// 构造函数：通过指定分量初始化
	FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

	// 向量加法
	FVector operator + (const FVector& other) const { return FVector(X + other.X, Y + other.Y, Z + other.Z); }

	// 向量减法
	FVector operator - (const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); }

	// 向量与标量乘法
	FVector operator * (float scalar) const { return FVector(X * scalar, Y * scalar, Z * scalar); }

	// 向量点乘
	FVector operator * (const FVector& other) const { return FVector(X * other.X, Y * other.Y, Z * other.Z); }

	// 向量与标量除法
	FVector operator / (float scalar) const { return FVector(X / scalar, Y / scalar, Z / scalar); }

	// 向量叉乘
	FVector operator^(const FVector& V) const { return FVector(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X); }

	// 向量分量除法
	FVector operator / (const FVector& other) const { return FVector(X / other.X, Y / other.Y, Z / other.Z); }

	// 赋值运算符
	FVector& operator=  (const FVector& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }

	// 加法赋值
	FVector& operator+= (const FVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }

	// 减法赋值
	FVector& operator-= (const FVector& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }

	// 标量乘法赋值
	FVector& operator*= (const float other) { X *= other; Y *= other; Z *= other; return *this; }

	// 计算与另一个向量的点积
	float Dot(const FVector& b) const { return (X * b.X) + (Y * b.Y) + (Z * b.Z); }

	// 计算与另一个向量的距离
	float Distance(FVector v) { return float(sqrt(pow(v.X - X, 2.0) + pow(v.Y - Y, 2.0) + pow(v.Z - Z, 2.0))); }

	// 计算向量长度的平方（避免开方，用于比较）
	float MagnitudeSqr() const { return Dot(*this); }

	// 计算向量长度
	float Magnitude() const { return sqrt(MagnitudeSqr()); }

	// 归一化向量（单位向量）
	FVector Unit() const
	{
		const float fMagnitude = Magnitude();
		return FVector(X / fMagnitude, Y / fMagnitude, Z / fMagnitude);
	}

	// 相等比较
	friend bool operator==(const FVector& first, const FVector& second) { return first.X == second.X && first.Y == second.Y && first.Z == second.Z; }

	// 不等比较
	friend bool operator!=(const FVector& first, const FVector& second) { return !(first == second); }
};

// 二维向量结构体（用于表示2D空间中的位置、坐标等）
struct FVector2D
{
public:
	float X;  // X轴分量
	float Y;  // Y轴分量

	// 构造函数：初始化零向量
	FVector2D() : X(0), Y(0) {}

	// 构造函数：通过指定分量初始化
	FVector2D(float x, float y) : X(x), Y(y) {}

	// 判断是否接近零向量
	bool Zero() const
	{
		return (X > -0.1f && X < 0.1f && Y > -0.1f && Y < 0.1f);
	}

	// 计算与指定点的屏幕距离
	float ScreenDis(float GameX, float GameY) {
		FVector2D Vec;
		Vec.X = GameX - X;
		Vec.Y = GameY - Y;
		float dis = sqrt(Vec.X * Vec.X + Vec.Y * Vec.Y);  // 欧氏距离
		return dis;
	}

	// 向量加法
	FVector2D operator + (const FVector2D& other) const { return FVector2D(X + other.X, Y + other.Y); }

	// 向量减法
	FVector2D operator - (const FVector2D& other) const { return FVector2D(X - other.X, Y - other.Y); }

	// 向量与标量乘法
	FVector2D operator * (float scalar) const { return FVector2D(X * scalar, Y * scalar); }

	// 向量分量乘法
	FVector2D operator * (const FVector2D& other) const { return FVector2D(X * other.X, Y * other.Y); }

	// 向量与标量除法
	FVector2D operator / (float scalar) const { return FVector2D(X / scalar, Y / scalar); }

	// 向量分量除法
	FVector2D operator / (const FVector2D& other) const { return FVector2D(X / other.X, Y / other.Y); }

	// 赋值运算符
	FVector2D& operator=  (const FVector2D& other) { X = other.X; Y = other.Y; return *this; }

	// 加法赋值
	FVector2D& operator+= (const FVector2D& other) { X += other.X; Y += other.Y; return *this; }

	// 减法赋值
	FVector2D& operator-= (const FVector2D& other) { X -= other.X; Y -= other.Y; return *this; }

	// 标量乘法赋值
	FVector2D& operator*= (const float other) { X *= other; Y *= other; return *this; }
};

// 名称结构体（用于UE引擎中的字符串标识，通常关联到全局字符串表）
struct FName {
	int32_t Index;   // 字符串表索引
	int32_t Number;  // 实例编号（用于区分同名不同实例）

	// 获取ANSI字符串名称
	std::string GetName()
	{
		wchar_t Unicode[1024];
		// 调用引擎函数获取宽字符串
		auto _Len = Utils::FastCall<int>(Offsets::GetName, this, Unicode);
		// 转换为ANSI字符串
		std::string name = Utils::WCHAR2String(Unicode);
		// 截取最后一个'/'后的部分（获取短名称）
		auto pos = name.rfind('/');
		if (pos != std::string::npos)
		{
			name = name.substr(pos + 1);
		}
		return name;
	}

	// 获取宽字符串名称
	std::wstring GetNameW()
	{
		wchar_t Unicode[1024];
		auto _Len = Utils::FastCall<int>(Offsets::GetName, this, Unicode);
		auto Name = std::wstring(Unicode);
		// 截取最后一个'/'后的部分
		auto pos = Name.rfind('/');
		if (pos != std::wstring::npos)
		{
			Name = Name.substr(pos + 1);
		}
		return Name;
	}

	// 通过ID获取名称
	static std::string GetName(int id) {
		wchar_t Unicode[1024];
		FName NameID = { id ,0 };  // 构造指定ID的FName
		auto _Len = Utils::FastCall<int>(Offsets::GetName, &NameID, Unicode);
		if (_Len <= 0 || _Len > 1024) {
			return "";
		}
		std::string name = Utils::WCHAR2String(Unicode);
		// 截取最后一个'/'后的部分
		auto pos = name.rfind('/');
		if (pos != std::string::npos)
		{
			name = name.substr(pos + 1);
		}
		return name;
	}
};

// 字段类（UE引擎中反射系统的基础类，用于描述类成员）
class UField
{
public:
	uint8_t Pad_00[0x18];       // 填充字节（0x00-0x17）
	class UField* Next;         // 下一个字段（链表结构）
	uint8_t Pad_20[0x08];       // 填充字节（0x20-0x27）
	FName Name;                 // 字段名称
	uint8_t Pad_30[0x24];       // 填充字节（0x30-0x53）
	uint32_t Offset;            // 字段在类中的偏移量
	uint8_t Pad_58[0x38];       // 填充字节（0x58-0x8F）
};

// 对象基类（UE引擎中所有UObject的基类）
class UObject
{
public:
	void** VFTable;              // 虚函数表指针
	class UClass* ClassPrivate;  // 类信息指针（反射用）
	class UObject* OuterPrivate; // 外部对象指针（所有者）
	int32_t InternalIndex;       // 内部索引
	FName NamePrivate;           // 对象名称

	// 判断当前对象是否是指定类或其子类的实例
	bool IsA(void* cmp)
	{
		// 遍历类继承链
		for (auto super = ClassPrivate; super; super = Mem::Read<UClass*>((ULONG64)super + 0x48))
		{
			if (super == cmp) {
				return true;
			}
		}
		return false;
	}

	// 处理事件（调用蓝图或C++函数）
	void ProcessEvent(void* fn, void* parms)
	{
		auto vtable = *reinterpret_cast<void***>(this);
		auto call = vtable[0x44];  // 0x44是ProcessEvent在虚函数表中的索引
		// 检查是否是有效调用（避免跳转指令）
		if (*(BYTE*)call == 0xE9 || *(BYTE*)call == 0xE8)
			return;
		Utils::FastCall<bool>(call, this, fn, parms);
	}

	// 获取对象的ANSI名称
	std::string GetName()
	{
		return NamePrivate.GetName();
	}

	// 获取对象的宽字符串名称
	std::wstring GetNameW()
	{
		return NamePrivate.GetNameW();
	}

	// 查找指定名称的对象
	static UObject* FindObject(const wchar_t* name)
	{
		return Utils::FastCall<UObject*>(Offsets::StaticFindObject, (uintptr_t)0, (uintptr_t)-1, name, false);
	}
};

// 结构类（UE引擎中结构体的基类，继承自UObject）
class UStruct : public UObject
{
public:
	uint8_t Pad_20[0x28];        // 填充字节（0x20-0x47）
	class UStruct* SuperStruct;  // 父结构（用于继承）
	uint8_t Pad_50[0x20];        // 填充字节（0x50-0x6F）
	class UField* ChildProperties; // 子属性链表（成员变量）
};

// 类信息类（描述UObject的类信息，继承自UStruct）
class UClass : public UStruct
{
public:
	// 类信息相关成员（未完全定义）
};

// 画布类（用于2D渲染，如绘制HUD、UI等）
class UCanvas : public UObject {
public:
	// 画布相关成员和方法（未完全定义）
};

// 演员类（游戏中所有可放置实体的基类）
class AActor : public UObject
{
public:
	// 获取类信息
	static UClass* StaticClass()
	{
		static UClass* Class;
		if (!Class) Class = (UClass*)FindObject((L"Engine.Actor"));
		return Class;
	}

	// 类型转换模板（将AActor转换为指定子类）
	template<typename T>
	inline T As()
	{
		if (!this)
			return nullptr;
		return static_cast<T>(this);
	}
};

//  pawn类（可被玩家或AI控制的实体基类，继承自AActor）
class APawn : public AActor
{
public:
	// Pawn相关成员和方法（未完全定义）
};

// 世界类（包含游戏中所有实体和关卡的根对象）
class UWorld : public UObject
{
public:
	// 世界相关成员和方法（未完全定义）
};

// 游戏视口客户端类（管理游戏渲染窗口和视口）
class UGameViewportClient : public UObject
{
public:
	// 视口相关成员和方法（未完全定义）
};

// 健康数据组件类（管理实体的生命值）
class UGPHealthDataComponent : public AActor
{
public:
	// 获取当前生命值
	float GetHealth() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			// 查找"GetHealth"函数对象
			fn = FindObject((L"GPGameplay.GPHealthDataComponent.GetHealth"));
		// 函数参数结构（包含返回值）
		struct {
			float ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);  // 调用函数
		return Params.ReturnValue;
	}

	// 获取最大生命值
	float GetHealthMax() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPHealthDataComponent.GetHealthMax"));
		struct {
			float ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}
};

// 队伍组件类（管理实体的队伍信息）
class UGPTeamComponent : public AActor
{
public:
	// 获取队伍ID
	int32_t GetTeamID() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPTeamComponent.GetTeamID"));
		struct {
			int32_t ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 获取阵营ID
	int32_t GetCamp() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPTeamComponent.GetCamp"));
		struct {
			int32_t ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}
};

// 角色类（可移动的角色实体基类，继承自APawn）
class ACharacter : public APawn
{
public:
	// 获取类信息
	static UClass* StaticClass()
	{
		static UClass* Class;
		if (!Class) Class = (UClass*)FindObject((L"Engine.CHARACTER"));
		return Class;
	}
};

// 内部角色类（游戏特定的角色基类，继承自ACharacter）
class AIntCharacter : public ACharacter {
public:
	// 获取类信息
	static UClass* StaticClass()
	{
		static UClass* Class;
		if (!Class) Class = (UClass*)FindObject((L"GPGameplay.IntCharacter"));
		return Class;
	}
};

// 角色基础类（继承自AIntCharacter，包含通用角色功能）
class ACharacterBase : public AIntCharacter {
public:
	// 获取类信息
	static UClass* StaticClass()
	{
		static UClass* Class;
		if (!Class) Class = (UClass*)FindObject((L"GPGameplay.CharacterBase"));
		return Class;
	}

	// 判断角色是否死亡
	bool IsDead() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.CharacterBase.IsDead"));
		struct {
			bool ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}
};

// GP角色基础类（继承自ACharacterBase，游戏特定角色基础类）
class AGPCharacterBase : public ACharacterBase {
public:
	// 获取类信息
	static UClass* StaticClass()
	{
		static UClass* Class;
		if (!Class) Class = (UClass*)FindObject((L"GPGameplay.GPCharacterBase"));
		return Class;
	}

	// 判断角色是否存活
	char IsAlive() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPCharacterBase.IsAlive"));
		struct {
			char ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 判断是否为AI角色
	bool IsAI() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPCharacterBase.IsAI"));
		struct {
			bool ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 判断是否为玩家角色
	bool IsPlayer() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPCharacterBase.IsPlayer"));
		struct {
			bool ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 获取队伍组件
	UGPTeamComponent* GetTeamComp() {
		UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPCharacterBase.GetTeamComp"));
		struct {
			UGPTeamComponent* ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 获取健康组件
	UGPHealthDataComponent* GetHealthComp() {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPCharacterBase.GetHealthComp"));
		struct {
			UGPHealthDataComponent* ReturnValue;
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}
};

// GP角色类（继承自AGPCharacterBase，玩家可控制的角色）
class AGPCharacter : public AGPCharacterBase {
public:
	// 获取类信息
	static UClass* StaticClass()
	{
		static UClass* Class;
		if (!Class) Class = (UClass*)FindObject((L"GPGameplay.GPCharacter"));
		return Class;
	}
};

// DFM角色类（继承自AGPCharacter，特定游戏模式的角色）
class ADFMCharacter : public AGPCharacter {
public:
	// 获取类信息
	static UClass* StaticClass()
	{
		static UClass* Class;
		if (!Class) Class = (UClass*)FindObject((L"DFMGameplay.DFMCharacter"));
		return Class;
	}
};

// 蓝图函数库类（提供蓝图可用的静态函数集合）
class UBlueprintFunctionLibrary : public UObject
{
public:
	// 获取指定类的所有实体
	void GetAllActorsOfClass(UObject* WorldContextObject, UClass* ActorClass, TArray<AActor*>* OutActors)
	{
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"Engine.GameplayStatics.GetAllActorsOfClass"));
		// 函数参数结构
		struct
		{
			UObject* WorldContextObject;  // 世界上下文对象
			UClass* ActorClass;           // 要查找的实体类
			TArray<AActor*> OutActors;    // 输出结果数组
		} params{};
		params.WorldContextObject = WorldContextObject;
		params.ActorClass = ActorClass;
		if (fn)
			ProcessEvent(fn, &params);  // 调用函数
		if (OutActors != nullptr)
			*OutActors = params.OutActors;  // 输出结果
	}

	// 判断是否为本地玩家
	bool IsLocalPlayer(UObject* WorldContextObject)
	{
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GameplayBlueprintHelper.IsLocalPlayer"));
		struct {
			UObject* WorldContextObject;  // 世界上下文对象
			bool ReturnValue;             // 返回值
		} Params{};
		Params.WorldContextObject = WorldContextObject;
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 获取GP游戏状态
	uintptr_t GetGPGameState(UObject* WorldContextObject)
	{
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GameplayBlueprintHelper.GetGPGameState"));
		struct {
			UObject* WorldContextObject;  // 世界上下文对象
			uintptr_t ReturnValue;        // 返回值（游戏状态指针）
		} Params{};
		Params.WorldContextObject = WorldContextObject;
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 获取本地GP角色
	AGPCharacter* GetLocalGPCharacter(UObject* WorldContextObject)
	{
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GameplayBlueprintHelper.GetLocalGPCharacter"));
		struct {
			UObject* WorldContextObject;  // 世界上下文对象
			AGPCharacter* ReturnValue;    // 返回值（本地角色指针）
		} Params{};
		Params.WorldContextObject = WorldContextObject;
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 获取主流程状态
	enum EMainFlowState GetMainFlowState()
	{
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPSettings.GPScalabilityBlueprintTools.GetMainFlowState"));
		struct {
			enum EMainFlowState ReturnValue;  // 返回值（主流程状态）
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}

	// 获取游戏模式
	int32_t GetGameMode()
	{
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPSettings.GPScalabilityBlueprintTools.GetGameMode"));
		struct {
			int32_t ReturnValue;  // 返回值（游戏模式）
		} Params{};
		if (fn)
			ProcessEvent(fn, &Params);
		return Params.ReturnValue;
	}
};

// 轮廓效果组件类（管理实体的高亮轮廓效果）
class UGPOutLineEffectComponent : public UObject {
public:
	// 播放指定类型的轮廓效果
	void PlayOutLineEffect(enum class EOutLineEffectType OutLineType) {
		static UObject* fn = nullptr;
		if (fn == nullptr)
			fn = FindObject((L"GPGameplay.GPOutLineEffectComponent.PlayOutLineEffect"));
		struct
		{
			enum class EOutLineEffectType OutLineType;  // 轮廓效果类型
		} params{};
		params.OutLineType = OutLineType;
		if (fn)
			ProcessEvent(fn, &params);  // 调用函数
	}
};