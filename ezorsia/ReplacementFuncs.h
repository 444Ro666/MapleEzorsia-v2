#pragma once
#include "AutoTypes.h"

static bool ownLoginFrame;
static bool ownCashShopFrame;
static bool EzorsiaV2WzIncluded;
//notes from my knowledge as i have not used these kinds of codes practically well
//function replacement is when you replace the original function in the client with your own fake function, usually to add some extra functionality
//for more complex applications you would also need to define the client's variables and reinterpret_cast those (no void this time)
//you need the right calling convention (match client's original or use _fastcall, i havent tried it much)
//it would help to know the benefits and drawbacks of "reinterpret_cast", as well as how it is hooking to prevent accidents
//hooking to the original function will replace it at all times when it is called by the client
//i personally have not tried it more because it requires a very thorough understanding of how the client code works, re-making the parts here,
//and using it, all together, in a way that doesnt break anything
//it would be the best way to do it for very extensive client edits and if you need to replace entire functions in that context but
//code caving is generally easier for short term, one-time patchwork fixes	//thanks you teto for helping me on this learning journey

bool HookGetModuleFileName(bool bEnable) {
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

	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_GetModuleFileNameW), GetModuleFileNameW_Hook);
}

/// <summary>
/// Creates a detour for the User32.dll CreateWindowExA function applying the following changes:
/// 1. Enable the window minimize box
/// </summary>
inline void HookCreateWindowExA(bool bEnable) {
	static auto create_window_ex_a = decltype(&CreateWindowExA)(GetProcAddress(LoadLibraryA("USER32"), "CreateWindowExA"));
	static const decltype(&CreateWindowExA) hook = [](DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) -> HWND {
		dwStyle |= WS_MINIMIZEBOX; // enable minimize button
		return create_window_ex_a(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	};
	Memory::SetHook(bEnable, reinterpret_cast<void**>(&create_window_ex_a), hook);
}

DWORD GetFuncAddress(LPCSTR lpModule, LPCSTR lpFunc)	//ty alias!			//multiclient, not currently working, likely cannot hook early enough with nmconew.dll
{
	auto mod = LoadLibraryA(lpModule);

	if (!mod)
	{
		return 0;
	}

	auto address = (DWORD)GetProcAddress(mod, lpFunc);

	if (!address)
	{
		return 0;
	}

	//#ifdef _DEBUG
		//Log(__FUNCTION__ " [%s] %s @ %8X", lpModule, lpFunc, address);
	//#endif

	return address;
}

bool Hook_CreateMutexA(bool bEnable)	//ty darter	//ty angel!
{
	static auto _CreateMutexA = decltype(&CreateMutexA)(GetFuncAddress("KERNEL32", "CreateMutexA"));

	decltype(&CreateMutexA) Hook = [](LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) -> HANDLE
	{
		//Multi-Client Check Removal
		if (lpName && strstr(lpName, "WvsClientMtx"))
		{
			return (HANDLE)0x0BADF00D;
		}

		return _CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
	};

	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CreateMutexA), Hook);
}

//bool Hook_get_unknown(bool bEnable)
//{
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_get_unknown), _get_unknown_Hook);
//}
//bool Hook_get_resource_object(bool bEnable)
//{
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_get_resource_object), _get_resource_object_Hook);
//}
//bool Hook_com_ptr_t_IWzProperty__ctor(bool bEnable)
//{
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_com_ptr_t_IWzProperty__ctor), _com_ptr_t_IWzProperty__ctor_Hook);
//}
//bool Hook_com_ptr_t_IWzProperty__dtor(bool bEnable)
//{
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_com_ptr_t_IWzProperty__dtor), _com_ptr_t_IWzProperty__dtor_Hook);
//}
bool HookPcCreateObject_IWzResMan(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_PcCreateObject_IWzResMan), _PcCreateObject_IWzResMan_Hook);
}
bool HookPcCreateObject_IWzNameSpace(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_PcCreateObject_IWzNameSpace), _PcCreateObject_IWzNameSpace_Hook);
}
bool HookPcCreateObject_IWzFileSystem(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_PcCreateObject_IWzFileSystem), _PcCreateObject_IWzFileSystem_Hook);
}
bool HookCWvsApp__Dir_BackSlashToSlash(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CWvsApp__Dir_BackSlashToSlash), _CWvsApp__Dir_BackSlashToSlash_Hook);
}
bool HookCWvsApp__Dir_upDir(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CWvsApp__Dir_upDir), _CWvsApp__Dir_upDir_Hook);
}
bool Hookbstr_ctor(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_bstr_ctor), _bstr_ctor_Hook);
}
bool HookIWzFileSystem__Init(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_IWzFileSystem__Init), _IWzFileSystem__Init_Hook);
}
bool HookIWzNameSpace__Mount(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_IWzNameSpace__Mount), _IWzNameSpace__Mount_Hook);
}
//#pragma optimize("", off)
bool HookCWvsApp__InitializeResMan(bool bEnable)	//resman hook that does nothing, kept for analysis and referrence //not skilled enough to rewrite to load custom wz files
{
	static _CWvsApp__InitializeResMan_t _CWvsApp__InitializeResMan_Hook = [](void* pThis, void* edx) {
		//-> void {_CWvsApp__InitializeResMan(pThis, edx);
		_CWvsApp__InitializeResMan(pThis, edx);	//comment this out and uncomment below if testing, supposed to load from .img files in folders but i never got to test it
		//void* pData = nullptr;
		//void* pFileSystem = nullptr;
		//void* pUnkOuter = 0;
		//void* nPriority = 0;
		//void* sPath;

		//// Resman
		//_PcCreateObject_IWzResMan(L"ResMan", g_rm, pUnkOuter);	//?(void*) //?&g

		//void* pIWzResMan_Instance = *g_rm;	//?&g
		//auto IWzResMan__SetResManParam = *(void(__fastcall**)(void*, void*, void*, int, int, int))((*(int*)pIWzResMan_Instance) + 20); // Hard Coded
		//IWzResMan__SetResManParam(nullptr, nullptr, pIWzResMan_Instance, RC_AUTO_REPARSE | RC_AUTO_SERIALIZE, -1, -1);

		//// NameSpace
		//_PcCreateObject_IWzNameSpace(L"NameSpace", g_root, pUnkOuter);

		//void* pIWzNameSpace_Instance = g_root;
		//auto PcSetRootNameSpace = *(void(__cdecl*)(void*, int)) * (int*)pNameSpace; // Hard Coded
		//PcSetRootNameSpace(pIWzNameSpace_Instance, 1);

		//// Game FileSystem
		//_PcCreateObject_IWzFileSystem(L"NameSpace#FileSystem", &pFileSystem, pUnkOuter);

		//char sStartPath[MAX_PATH];
		//GetModuleFileNameA(NULL, sStartPath, MAX_PATH);
		//_CWvsApp__Dir_BackSlashToSlash(sStartPath);
		//_CWvsApp__Dir_upDir(sStartPath);

		//_bstr_ctor(&sPath, pData, sStartPath);

		//auto iGameFS = _IWzFileSystem__Init(pFileSystem, pData, sPath);

		//_bstr_ctor(&sPath, pData, "/");

		//auto mGameFS = _IWzNameSpace__Mount(*g_root, pData, sPath, pFileSystem, (int)nPriority);

		//// Data FileSystem
		//_PcCreateObject_IWzFileSystem(L"NameSpace#FileSystem", &pFileSystem, pUnkOuter);

		//_bstr_ctor(&sPath, pData, "./Data");

		//auto iDataFS = _IWzFileSystem__Init(pFileSystem, pData, sPath);

		//_bstr_ctor(&sPath, pData, "/");

		//auto mDataFS = _IWzNameSpace__Mount(*g_root, pData, sPath, pFileSystem, (int)nPriority);
	};
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CWvsApp__InitializeResMan), _CWvsApp__InitializeResMan_Hook);
}
//#pragma optimize("", on)
bool Hook_StringPool__GetString(bool bEnable)	//hook stringpool modification //ty !! popcorn //ty darter
{
	_StringPool__GetString_t _StringPool__GetString_Hook = [](void* pThis, void* edx, ZXString<char>* result, unsigned int nIdx, char formal) ->  ZXString<char>*
	{
		auto ret = _StringPool__GetString(pThis, edx, result, nIdx, formal);
		switch (nIdx)
		{
			case 1307:	//1307_UI_LOGINIMG_COMMON_FRAME = 51Bh
				if (EzorsiaV2WzIncluded && !ownLoginFrame) {
					switch (Client::m_nGameWidth)
					{
						case 1280:	//ty teto for the suggestion to use ZXString<char>::Assign and showing me available resources
							*ret = ("UI/MapleEzorsiaV2wzfiles.img/Common/frame1280"); break;
						case 1366:
							*ret = ("UI/MapleEzorsiaV2wzfiles.img/Common/frame1366"); break;
						case 1600:
							*ret = ("UI/MapleEzorsiaV2wzfiles.img/Common/frame1600"); break;
						case 1920:
							*ret = ("UI/MapleEzorsiaV2wzfiles.img/Common/frame1920"); break;
						case 1024:
							*ret = ("UI/MapleEzorsiaV2wzfiles.img/Common/frame1024"); break;
					}
					break;
				}
			case 1301:	//1301_UI_CASHSHOPIMG_BASE_BACKGRND  = 515h
				if (EzorsiaV2WzIncluded && !ownCashShopFrame) { *ret = ("UI/MapleEzorsiaV2wzfiles.img/Base/backgrnd"); } break;
			case 1302:	//1302_UI_CASHSHOPIMG_BASE_BACKGRND1 = 516h
				if (EzorsiaV2WzIncluded && !ownCashShopFrame) { *ret = ("UI/MapleEzorsiaV2wzfiles.img/Base/backgrnd1"); } break;
			case 5361:	//5361_UI_CASHSHOPIMG_BASE_BACKGRND2  = 14F1h			
				if (EzorsiaV2WzIncluded && !ownCashShopFrame) { *ret = ("UI/MapleEzorsiaV2wzfiles.img/Base/backgrnd2"); } break;
			//case 1302:	//BACKGRND??????
			//	if (EzorsiaV2WzIncluded && ownCashShopFrame) { *ret = ("UI/MapleEzorsiaV2wzfiles.img/Base/backgrnd1"); } break;
			//case 5361:	//SP_1937_UI_UIWINDOWIMG_STAT_BACKGRND2  = 791h	
			//	if (EzorsiaV2WzIncluded && ownCashShopFrame) { *ret = ("UI/MapleEzorsiaV2wzfiles.img/Base/backgrnd2"); } break;
		}
		return ret;
	};
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_StringPool__GetString), _StringPool__GetString_Hook);
}
bool HookMyTestHook(bool bEnable)
{ return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CWndCreateWnd), _CWndCreateWnd_Hook); }

bool HookDetectLogin(bool bEnable)
{ return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CLoginSendSelectCharPacket), _CLoginSendSelectCharPacket_Hook); }