#include "Includes.h"

// DLL 主函数，当 DLL 被加载、线程创建/销毁、进程退出时调用
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {  // 当 DLL 被进程加载时
        DisableThreadLibraryCalls(hModule);  // 禁用线程附加/分离时的 DLL 通知
        HideDll DLL(hModule);  // 创建隐藏 DLL 的对象
        DLL.RemoveLDR();       // 从 LDR 链表中移除 DLL 信息
        DLL.RemoveMAP();       // 从内存映射中移除 DLL 信息
        DLL.RemovePEH();       // 从 PE 头相关结构中移除 DLL 信息
        // 创建线程执行 C_Hack::Init 函数（已注释）
        // CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C_Hack::Init, hModule, NULL, NULL);
        // 创建线程执行 C_Hack::Init2 函数
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C_Hack::Init2, hModule, NULL, NULL);

        break;
    }
    case DLL_THREAD_ATTACH:    // 当线程被创建时
    case DLL_THREAD_DETACH:    // 当线程退出时
    case DLL_PROCESS_DETACH:   // 当 DLL 被进程卸载时
        break;
    }
    return TRUE;
}

// 备用的 DLL 主函数（已注释）
// BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
// {
// 	switch (ul_reason_for_call)
// 	{
// 	case DLL_PROCESS_ATTACH: {  // 当 DLL 被进程加载时
// 		DisableThreadLibraryCalls(hModule);  // 禁用线程附加/分离时的 DLL 通知
// 		HideDll DLL(hModule);  // 创建隐藏 DLL 的对象
// 		// DLL.RemoveLDR();    // （已注释）从 LDR 链表中移除 DLL 信息
// 		// DLL.RemoveMAP();    // （已注释）从内存映射中移除 DLL 信息
// 		// DLL.RemovePEH();    // （已注释）从 PE 头相关结构中移除 DLL 信息
// 		// 创建线程执行 C_Hack::Init 函数
// 		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C_Hack::Init, hModule, NULL, NULL);
// 		break;
// 	}
// 	case DLL_THREAD_ATTACH:    // 当线程被创建时
// 	case DLL_THREAD_DETACH:    // 当线程退出时
// 	case DLL_PROCESS_DETACH:   // 当 DLL 被进程卸载时
// 		break;
// 	}
// 	return TRUE;
// }