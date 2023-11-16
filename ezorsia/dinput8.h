#pragma once
class dinput8
{
public:
	static void CreateHook();
};

//old ijl15.h
//#pragma once
//class ijl15
//{
//public:
//	static void CreateHook();
//};

//old ijl15.cpp
//#include "stdafx.h"
//#include "ijl15.h"
//
//FARPROC ijlErrorStr_Proc;
//FARPROC ijlFree_Proc;
//FARPROC ijlGetLibVersion_Proc;
//FARPROC ijlInit_Proc;
//FARPROC ijlRead_Proc;
//FARPROC ijlWrite_Proc;
//
//void ijl15::CreateHook() {
//	HMODULE hModule = LoadLibraryA("2ijl15.dll");
//	if (hModule) {
//		ijlErrorStr_Proc = GetProcAddress(hModule, "ijlErrorStr");
//		ijlFree_Proc = GetProcAddress(hModule, "ijlFree");
//		ijlGetLibVersion_Proc = GetProcAddress(hModule, "ijlGetLibVersion");
//		ijlInit_Proc = GetProcAddress(hModule, "ijlInit");
//		ijlRead_Proc = GetProcAddress(hModule, "ijlRead");
//		ijlWrite_Proc = GetProcAddress(hModule, "ijlWrite");
//	}
//	else { MessageBox(NULL, L"Failed to find 2ijl15.dll, please follow Ezorsia v2 installation instructions and remember to rename the original file to 2ijl15.dll", L"Missing file", 0); ExitProcess(0); }
//}
//extern "C" __declspec(dllexport) __declspec(naked) void ijlGetLibVersion()
//{
//	__asm	jmp dword ptr[ijlGetLibVersion_Proc] // make sure you're compiling in x86
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void ijlInit()
//{
//	__asm	jmp dword ptr[ijlInit_Proc]
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void ijlFree()
//{
//	__asm	jmp dword ptr[ijlFree_Proc]
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void ijlRead()
//{
//	__asm	jmp dword ptr[ijlRead_Proc]
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void ijlWrite()
//{
//	__asm	jmp dword ptr[ijlWrite_Proc]
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void ijlErrorStr()
//{
//	__asm	jmp dword ptr[ijlErrorStr_Proc]
//}

//old NMCO.h
//#pragma once
//class NMCO
//{
//public:
//	static void CreateHook();
//};

//old MNCO.cpp
//#include "stdafx.h"
//#include "NMCO.h"
//
//FARPROC dwNMCOCallFunc;
//FARPROC dwNMCOCallFunc2;
//FARPROC dwNMCOMemoryFree;
//
//void NMCO::CreateHook() {
//	HMODULE hModule = LoadLibraryA("nmconew2.dll");
//	if (hModule == nullptr) {
//		MessageBox(NULL, L"Failed to find nmconew2.dll file", L"Missing file", 0);
//		return;
//	}
//	dwNMCOCallFunc = GetProcAddress(hModule, "NMCO_CallNMFunc");
//	dwNMCOCallFunc2 = GetProcAddress(hModule, "NMCO_CallNMFunc2");
//	dwNMCOMemoryFree = GetProcAddress(hModule, "NMCO_MemoryFree");
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void NMCO_CallNMFunc() {
//	__asm jmp dwNMCOCallFunc
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void NMCO_CallNMFunc2() {
//	__asm jmp dwNMCOCallFunc2
//}
//
//extern "C" __declspec(dllexport) __declspec(naked) void NMCO_MemoryFree() {
//	__asm jmp dwNMCOMemoryFree
//}