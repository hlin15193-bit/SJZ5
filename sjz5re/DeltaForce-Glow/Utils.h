#pragma once

// 判断指针是否为无效指针（宏定义）
#define IsBadPtr(_x_) (Mem::ValidPtr((void*)(_x_)))

namespace Mem {  // 内存操作命名空间

	// 验证指针是否有效
	// 返回值：true表示指针无效，false表示指针可能有效
	bool ValidPtr(PVOID Ptr)
	{
		auto v1 = (ULONG64)Ptr;  // 将指针转换为64位无符号整数
		// 检查指针是否超出合理内存范围，或不是8字节对齐
		return (bool)(v1 < 0x1000000 || v1 > 0x7FFFFFF00000 || v1 % sizeof(uint64_t));
	}

	// 检查指针指向的内存是否可读取
	// 参数：lp-待检查指针，ucb-需要读取的字节数
	// 返回值：TRUE表示不可读，FALSE表示可读
	BOOL IsBadReadPtr_(void* lp, UINT_PTR ucb)
	{
		// 先通过ValidPtr快速判断指针是否无效
		if (ValidPtr(lp))
			return TRUE;

		MEMORY_BASIC_INFORMATION mbi;  // 内存基本信息结构体

		// 查询内存区域信息
		if (VirtualQuery(lp, &mbi, sizeof(mbi)) == 0)
		{
			// 查询失败，指针无效
			return TRUE;
		}

		// 检查内存保护属性：不可访问或仅执行内存不可读
		if (mbi.Protect == PAGE_NOACCESS || mbi.Protect == PAGE_EXECUTE)
		{
			return TRUE;
		}

		// 检查内存状态：空闲或保留状态的内存不可读
		if (mbi.State == MEM_FREE || mbi.State == MEM_RESERVE) {
			return TRUE;
		}

		// 检查需要读取的字节数是否超出内存区域大小
		if (ucb > mbi.RegionSize)
		{
			return TRUE;
		}

		// 所有检查通过，内存可正常读取
		return FALSE;
	}

	// 从指定地址读取数据（模板函数）
	// 模板参数T：读取的数据类型
	// 参数Address：内存地址
	// 返回值：读取到的数据
	template <typename T>
	T Read(uintptr_t Address)
	{
		return *(T*)Address;
	}

	// 向指定地址写入数据（模板函数）
	// 模板参数T：写入的数据类型
	// 参数dwAddress：内存地址，Values：要写入的数据
	template<typename T>
	void Write(uintptr_t dwAddress, T Values)
	{
		*(T*)dwAddress = Values;
	}

	// 从指定地址读取指定长度的字节
	// 参数address：内存地址，buffer：接收数据的缓冲区，size：读取的字节数
	// 返回值：true表示读取成功，false表示失败
	bool ReadBytes(uintptr_t address, void* buffer, size_t size)
	{
		if (!address || !buffer || size == 0) return false;  // 检查参数有效性

		auto src = reinterpret_cast<const uint8_t*>(address);  // 源地址转换为字节指针
		auto dst = reinterpret_cast<uint8_t*>(buffer);        // 目标缓冲区转换为字节指针

		// 逐字节复制
		for (size_t i = 0; i < size; i++)
		{
			dst[i] = src[i];
		}
		return true;
	}

	// 向指定地址写入指定长度的字节（带内存保护修改）
	// 参数address：内存地址，buffer：要写入的数据，size：写入的字节数
	// 返回值：true表示写入成功，false表示失败
	bool WriteBytes(uintptr_t address, void* buffer, size_t size)
	{
		if (!address || !buffer || size == 0) return false;  // 检查参数有效性

		auto src = reinterpret_cast<uint8_t*>(address);  // 目标地址转换为字节指针
		auto dst = reinterpret_cast<uint8_t*>(buffer);   // 源数据缓冲区转换为字节指针

		DWORD OldProtect = NULL;  // 保存原始内存保护属性
		// 修改内存保护为可读写执行
		if (VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &OldProtect)) {
			// 逐字节写入
			for (size_t i = 0; i < size; i++)
			{
				src[i] = dst[i];
			}
			// 恢复原始内存保护属性
			VirtualProtect(src, size, OldProtect, &OldProtect);
		}
		return true;
	}

	// 计算绝对地址（通常用于解析相对偏移的地址）
	// 参数address：基准地址，offset：偏移量，size：指令长度
	// 返回值：计算得到的绝对地址
	uintptr_t ABS(uintptr_t address, int offset, int size)
	{
		if (address)  // 检查基准地址有效性
		{
			// 公式：绝对地址 = 基准地址 + 指令长度 + 相对偏移值
			uintptr_t addrs = address + size + Read<int>(address + offset);
			return addrs;
		}
		return NULL;  // 基准地址无效时返回空
	}
}

namespace Utils {  // 工具函数命名空间

	// 快速调用__fastcall调用约定的函数（函数指针版本）
	// 模板参数Ret：返回值类型，Args：参数类型列表
	// 参数pFunc：函数指针，args：传递给函数的参数
	template<typename Ret = void, typename... Args>
	__forceinline Ret FastCall(void* pFunc, Args... args) {
		typedef Ret(__fastcall* FuncType)(Args...);  // 定义函数指针类型
		return reinterpret_cast<FuncType>(pFunc)(args...);  // 转换并调用
	}

	// 快速调用__fastcall调用约定的函数（地址版本）
	// 模板参数Ret：返回值类型，Args：参数类型列表
	// 参数pFunc：函数地址，args：传递给函数的参数
	template<typename Ret = void, typename... Args>
	__forceinline Ret FastCall(uint64_t pFunc, Args... args) {
		typedef Ret(__fastcall* FuncType)(Args...);  // 定义函数指针类型
		return FastCall<Ret>((void*)pFunc, args...);  // 转换为指针后调用
	}

	// 将宽字符串（wstring）转换为多字节字符串（string）
	// 参数wstrSrc：源宽字符串
	// 返回值：转换后的多字节字符串
	std::string WCHAR2String(const std::wstring& wstrSrc)
	{
		if (wstrSrc.empty()) return "";  // 空字符串直接返回

		// 计算转换所需的缓冲区大小
		int nLen = WideCharToMultiByte(CP_ACP, 0,
			wstrSrc.c_str(), -1,
			nullptr, 0, nullptr, nullptr);
		if (nLen <= 0) return "";  // 计算失败返回空

		// 分配缓冲区并进行转换（-1是为了去掉自动添加的'\0'）
		std::string strDest(nLen - 1, '\0');
		WideCharToMultiByte(CP_ACP, 0,
			wstrSrc.c_str(), -1,
			strDest.data(), nLen, nullptr, nullptr);

		return strDest;
	}
}