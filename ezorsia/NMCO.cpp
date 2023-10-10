#include "stdafx.h"
#include "NMCO.h"

FARPROC dwNMCOCallFunc;
FARPROC dwNMCOCallFunc2;
FARPROC dwNMCOMemoryFree;

void NMCO::CreateHook() {
	HMODULE hModule = LoadLibraryA("nmconew2.dll");
	if (hModule == nullptr) {
		MessageBox(NULL, L"Failed to find nmconew2.dll file", L"Missing file", 0);
		return;
	}
	dwNMCOCallFunc = GetProcAddress(hModule, "NMCO_CallNMFunc");
	dwNMCOCallFunc2 = GetProcAddress(hModule, "NMCO_CallNMFunc2");
	dwNMCOMemoryFree = GetProcAddress(hModule, "NMCO_MemoryFree");
}

extern "C" __declspec(dllexport) __declspec(naked) void NMCO_CallNMFunc() {
	__asm jmp dwNMCOCallFunc
}

extern "C" __declspec(dllexport) __declspec(naked) void NMCO_CallNMFunc2() {
	__asm jmp dwNMCOCallFunc2
}

extern "C" __declspec(dllexport) __declspec(naked) void NMCO_MemoryFree() {
	__asm jmp dwNMCOMemoryFree
}