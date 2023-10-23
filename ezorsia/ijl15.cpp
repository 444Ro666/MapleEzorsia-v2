#include "stdafx.h"
#include "ijl15.h"

FARPROC ijlErrorStr_Proc;
FARPROC ijlFree_Proc;
FARPROC ijlGetLibVersion_Proc;
FARPROC ijlInit_Proc;
FARPROC ijlRead_Proc;
FARPROC ijlWrite_Proc;

void ijl15::CreateHook() {
	HMODULE hModule = LoadLibraryA("2ijl15.dll");
	if (hModule == nullptr) {
		MessageBox(NULL, L"Failed to find 2ijl15.dll file", L"Missing file", 0);
		return;
	}
	ijlErrorStr_Proc = GetProcAddress(hModule, "ijlErrorStr");
	ijlFree_Proc = GetProcAddress(hModule, "ijlFree");
	ijlGetLibVersion_Proc = GetProcAddress(hModule, "ijlGetLibVersion");
	ijlInit_Proc = GetProcAddress(hModule, "ijlInit");
	ijlRead_Proc = GetProcAddress(hModule, "ijlRead");
	ijlWrite_Proc = GetProcAddress(hModule, "ijlWrite");
}

extern "C" __declspec(dllexport) __declspec(naked) void ijlGetLibVersion()
{
	__asm	jmp dword ptr[ijlGetLibVersion_Proc] // make sure you're compiling in x86
}

extern "C" __declspec(dllexport) __declspec(naked) void ijlInit()
{
	__asm	jmp dword ptr[ijlInit_Proc]
}

extern "C" __declspec(dllexport) __declspec(naked) void ijlFree()
{
	__asm	jmp dword ptr[ijlFree_Proc]
}

extern "C" __declspec(dllexport) __declspec(naked) void ijlRead()
{
	__asm	jmp dword ptr[ijlRead_Proc]
}

extern "C" __declspec(dllexport) __declspec(naked) void ijlWrite()
{
	__asm	jmp dword ptr[ijlWrite_Proc]
}

extern "C" __declspec(dllexport) __declspec(naked) void ijlErrorStr()
{
	__asm	jmp dword ptr[ijlErrorStr_Proc]
}