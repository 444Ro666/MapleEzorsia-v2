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

DWORD GetFuncAddress(LPCSTR lpModule, LPCSTR lpFunc)	//ty alias!			//multiclient
{
	HMODULE mod = LoadLibraryA(lpModule);

	if (!mod)
	{
		return 0;
	}

	DWORD address = (DWORD)GetProcAddress(mod, lpFunc);

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
			//char szMutex[128];
			//lpName = szMutex;
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

int myArrayForCustomEXP[] = { 15, 15, 34, 57, 92, 135, 372, 560, 840, 1144, 1242, 1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144, 11120, 13477, 16268, 19320, 22880, 27008, 31477, 36600, 42444, 48720, 55813, 63800, 86784, 98208, 110932, 124432, 139372, 155865, 173280, 192400, 213345, 235372, 259392, 285532, 312928, 342624, 374760, 408336, 445544, 483532, 524160, 567772, 598886, 631704, 666321, 702836, 741351, 781976, 824828, 870028, 917625, 967995, 1021041, 1076994, 1136013, 1198266, 1263930, 1333194, 1406252, 1483314, 1564600, 1650340, 1740778, 1836173, 1936794, 2042930, 2154882, 2272970, 2397528, 2528912, 2667496, 2813674, 2967863, 3130502, 3302053, 3483005, 3673873, 3875201, 4087562, 4311559, 4547832, 4797053, 5059931, 5337215, 5629694, 5938202, 6263614, 6606860, 6968915, 7350811, 7753635, 8178534, 8626718, 9099462, 9598112, 10124088, 10678888, 11264090, 11881362, 12532461, 13219239, 13943653, 14707765, 15513750, 16363902, 17260644, 18206527, 19204245, 20256637, 21366700, 22537594, 23772654, 25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428273, 38424542, 40530206, 42751262, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344, 69100311, 72887008, 76881216, 81094306, 85594273, 90225770, 95170142, 100385466, 105886589, 111689174, 117809740, 124265714, 131075474, 138258410, 145834970, 153826726, 162256430, 171148082, 180526997, 190419876, 200854885, 211861732, 223471711, 223471711, 248635353, 262260570, 276632449, 291791906, 307782102, 324648562, 342439302, 361204976, 380999008, 401877754, 423900654, 447130410, 471633156, 497478653, 524740482, 553496261, 583827855, 615821622, 649568646, 685165008, 722712050, 762316670, 804091623, 848155844, 894634784, 943660770, 995373379, 1049919840, 1107455447, 1168144006, 1232158297, 1299680571, 1370903066, 1446028554, 1525246918, 1608855764, 1697021059 };
const int maxLevelForCustomEXP = sizeof(myArrayForCustomEXP);//paste your custom exp table's array into the place of myArrayForCustomEXP[]
void* __fastcall _lpfn_NextLevel_Hook(int expTable[maxLevelForCustomEXP])	 //your max level is the size of your array 
{
	memcpy(expTable, myArrayForCustomEXP, maxLevelForCustomEXP);	//ty to creator of github.com/PurpleMadness/CustomExpTable
	expTable[maxLevelForCustomEXP] = 0;	//insert your own formula or predefined array into this part. MUST MATCH server numbers
	return expTable;					//currently using predefined array	
}
//void* __fastcall _lpfn_NextLevel_v62_Hook(int expTable[])	//formula for v62 exp table, kept for reference/example
//{															//if you want to use it remember to change the setting in Hook_lpfn_NextLevel
//	int level = 1;
//	while (level <= 5)
//	{
//		expTable[level] = level * (level * level / 2 + 15);
//		level++;
//	}
//	while (level <= 50)
//	{
//		expTable[level] = level * level / 3 * (level * level / 3 + 19);
//		level++;
//	}
//	while (level < 200)
//	{
//		expTable[level] = long(double(expTable[level - 1]) * 1.0548);
//		level++;
//	}
//	expTable[200] = 0;	//you need a MAX_INT checker for exp if you have levels over 200 and are not using a predefined array
//	return expTable;
//}
bool Hook_lpfn_NextLevel(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_lpfn_NextLevel), _lpfn_NextLevel_Hook);
	//return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_lpfn_NextLevel), _lpfn_NextLevel_v62_Hook);
}
bool Hook_CUIStatusBar__ChatLogAdd(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CUIStatusBar__ChatLogAdd), _CUIStatusBar__ChatLogAdd_Hook);
}