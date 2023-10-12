// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "NMCO.h"
#include "INIReader.h"

void CreateConsole() {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
}

bool HookGetModuleFileName() {
	static decltype(&GetModuleFileNameW) _GetModuleFileNameW = &GetModuleFileNameW;

	const decltype(&GetModuleFileNameW) GetModuleFileNameW_Hook = [](HMODULE hModule, LPWSTR lpFileName, DWORD dwSize) -> DWORD {
		auto len = _GetModuleFileNameW(hModule, lpFileName, dwSize);
		// Check to see if the length is invalid (zero)
		if (!len) {
			// Try again without the provided module for a fixed result
			len = _GetModuleFileNameW(nullptr, lpFileName, dwSize);
		}
		return len;
	};

	return Memory::SetHook(true, reinterpret_cast<void**>(&_GetModuleFileNameW), GetModuleFileNameW_Hook);
}

/// <summary>
/// Creates a detour for the User32.dll CreateWindowExA function applying the following changes:
/// 1. Enable the window minimize box
/// </summary>
inline void HookCreateWindowExA() {
	static auto create_window_ex_a = decltype(&CreateWindowExA)(GetProcAddress(LoadLibraryA("USER32"), "CreateWindowExA"));
	static const decltype(&CreateWindowExA) hook = [](DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) -> HWND {
		dwStyle |= WS_MINIMIZEBOX; // enable minimize button
		return create_window_ex_a(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	};
	Memory::SetHook(true, reinterpret_cast<void**>(&create_window_ex_a), hook);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	{
		//CreateConsole();

		//std::string ServerIP_AddressFromINI;

		INIReader reader("config.ini");
		if (reader.ParseError() == 0) {
			Client::m_nGameWidth = reader.GetInteger("general", "width", 1280);
			Client::m_nGameHeight = reader.GetInteger("general", "height", 720);
			Client::MsgAmount = reader.GetInteger("general", "MsgAmount", 26);
			Client::CustomLoginFrame = reader.GetBoolean("general", "CustomLoginFrame", true);
			Client::WindowedMode = reader.GetBoolean("general", "WindowedMode", true);
			Client::RemoveLogos = reader.GetBoolean("general", "RemoveLogos", true);
			Memory::UseVirtuProtect = reader.GetBoolean("general", "UseVirtuProtect", true);
			Client::setDamageCap = reader.GetReal("optional", "setDamageCap", 199999.0);
			Client::useTubi = reader.GetBoolean("optional", "useTubi", false);
			//ServerIP_AddressFromINI = reader.Get("general", "ServerIPaddress", "255.255.255.255");
		}
		//const char* ServerIP_Address = ServerIP_AddressFromINI.c_str();

		HookCreateWindowExA();
		HookGetModuleFileName();
		std::cout << "GetModuleFileName hook created" << std::endl;
		NMCO::CreateHook();
		std::cout << "NMCO hook initialized" << std::endl;

		//Memory::FillBytes(0xAFE084, 0x00, 0x6FE0B2 - 0x6FE084);//remove the existing server IP address	//writing ip from dll doesnt work
		//Memory::WriteString(0x6FE084, ServerIP_Address, strlen(ServerIP_Address));//write the user-set IP address
		//Memory::WriteString(0x6FE084 + 16, ServerIP_Address, strlen(ServerIP_Address));//write the user-set IP address
		//Memory::WriteString(0x6FE084 + 32, ServerIP_Address, strlen(ServerIP_Address));//write the user-set IP address
		//Memory::WriteByteArrayFromString(0xAFE084, "255.255.255.255");//write the user-set IP address //function not working
		//Memory::WriteByteArrayFromString(0xAFE084 + 16, "255.255.255.255");//write the user-set IP address
		//Memory::WriteByteArrayFromString(0xAFE084 + 32, "255.255.255.255");//write the user-set IP address

		//unsigned char remove_elevation[] = { 0x22, *(unsigned char*)"a", *(unsigned char*)"s", *(unsigned char*)"I", *(unsigned char*)"n", *(unsigned char*)"v", *(unsigned char*)"o", *(unsigned char*)"k", *(unsigned char*)"e", *(unsigned char*)"r", 0x22 };//remove elevation requests	//thanks stelmo for showing me how to do this
		//Memory::WriteByteArray(0xC08459, remove_elevation, sizeof(remove_elevation));//remove elevation requests	//not working from dll
		//Memory::FillBytes(0xC08464, 0x20, 0xC0846e - 0xC08464);//remove elevation requests
		//Memory::WriteByte(0x49C2CE, 0x01);//remove elevation requests	//thanks stelmo for showing me how to do this
		//Memory::WriteByte(0x49CFE9, 0x01);//remove elevation requests
		//Memory::WriteByte(0x49D399, 0x01);//remove elevation requests

		//Client::FunctionReplace();

		std::cout << "Applying resolution " << Client::m_nGameWidth << "x" << Client::m_nGameHeight << std::endl;
		Client::UpdateResolution();
		break;
	}
	default: break;
	case DLL_PROCESS_DETACH:
		ExitProcess(0);
	}
	return TRUE;
}