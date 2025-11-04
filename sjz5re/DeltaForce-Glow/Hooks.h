#pragma once

class VTableHook  // 虚函数表钩子类（用于Hook类的虚函数）
{
public:

	// 初始化钩子，针对目标对象设置VTableHook
	// 参数pTarget：要Hook的目标对象指针
	void Initialize(void* pTarget)
	{
		Target = pTarget;
		// 获取目标对象的原始虚函数表（对象前4/8字节存储VTable指针）
		oVTable = *(void***)pTarget;

		// 计算虚函数表大小并创建新的虚函数表
		uint32_t Size = CalcVTableSize();
		VTable = new void* [Size];

		// 复制原始虚函数表内容到新表，并替换目标对象的VTable指针
		memcpy(VTable, oVTable, Size * sizeof(void*));
		*(void***)pTarget = VTable;
	}

	// 将指定索引的虚函数绑定到新函数（实现Hook）
	// 参数Index：虚函数在表中的索引，Function：替换后的函数指针
	void Bind(uint32_t Index, void* Function)
	{
		VTable[Index] = Function;
	}

	// 解除指定索引的虚函数Hook（恢复原始函数）
	// 参数Index：虚函数在表中的索引
	void UnBind(uint32_t Index)
	{
		VTable[Index] = oVTable[Index];
	}

	// 解除所有虚函数的Hook（恢复原始虚函数表）
	void UnAllBind()
	{
		delete[] VTable;  // 释放新创建的虚函数表
		*(void***)Target = oVTable;  // 恢复目标对象的原始VTable指针
	}

	// 获取指定索引的原始虚函数（模板函数，自动转换为指定类型）
	// 模板参数T：原始函数的类型（通常是函数指针类型）
	// 参数Index：虚函数在表中的索引
	template <typename T>
	T GetOriginal(uint32_t Index)
	{
		return (T)oVTable[Index];
	}

private:
	// 计算虚函数表的大小（通过遍历直到遇到空指针）
	uint32_t CalcVTableSize()
	{
		uint32_t i = 0;
		while (oVTable[i] != NULL) {
			i++;
		}
		return i;
	}

	void* Target = NULL;       // 目标对象指针
	void** VTable = NULL;      // 新的虚函数表指针
	void** oVTable = NULL;     // 原始的虚函数表指针
};

// 内联命名空间，用于存放全局钩子实例
inline namespace Hook {
	VTableHook RenderHook;  // 渲染相关的虚函数表钩子实例
};