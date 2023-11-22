#include "stdafx.h"
#include "dinput8.h"
//NOTE: this dll can also be used to remap the core functionality of keybinds, i.e. changing arrow keys in the game to WASD. but this would
FARPROC DirectInput8Create_Proc; //require reinterpreting the functions of the dll instead of just redirecting as is done here (to dinput8.dll)
FARPROC GetdfDIJoystick_Proc;

void dinput8::CreateHook() {
	char szPath[MAX_PATH];
	if (GetSystemDirectoryA(szPath, sizeof(szPath))) { strcat(szPath, "\\dinput8.dll"); }
	else { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"Failed to load original dinput8.dll from system location, make sure your directory path is not longer than " + MAX_PATH, L"systems directory inaccessible", 0); ExitProcess(0); }
	HMODULE hModule = LoadLibraryA(szPath);
	if (hModule) { 
		DirectInput8Create_Proc = GetProcAddress(hModule, "DirectInput8Create"); 
		GetdfDIJoystick_Proc = GetProcAddress(hModule, "GetdfDIJoystick");
	}
	else { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"Failed to find original dinput8.dll, verify that a non-Ezorsia v2 dinput8.dll exists in your system directory", L"Missing file", 0); ExitProcess(0); }
}
extern "C" __declspec(dllexport) __declspec(naked) void DirectInput8Create()
{
	__asm	jmp dword ptr[DirectInput8Create_Proc]
}
extern "C" __declspec(dllexport) __declspec(naked) void GetdfDIJoystick()
{
	__asm	jmp dword ptr[GetdfDIJoystick_Proc]
}