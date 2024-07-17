#pragma once
#include "AutoTypes.h"
//#pragma optimize("", off)
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
bool HookGetModuleFileName_initialized = true;
bool Hook_GetModuleFileNameW(bool bEnable)
{
	static decltype(&GetModuleFileNameW) _GetModuleFileNameW = &GetModuleFileNameW;

	const decltype(&GetModuleFileNameW) GetModuleFileNameW_Hook = [](HMODULE hModule, LPWSTR lpFileName, DWORD dwSize) -> DWORD {
		if (HookGetModuleFileName_initialized)
		{
			std::cout << "HookGetModuleFileName started" << std::endl;
			HookGetModuleFileName_initialized = false;
		}
		auto len = _GetModuleFileNameW(hModule, lpFileName, dwSize);
		// Check to see if the length is invalid (zero)
		if (!len)
		{
			// Try again without the provided module for a fixed result
			len = _GetModuleFileNameW(nullptr, lpFileName, dwSize);
			std::cout << "HookGetModuleFileName null file name" << std::endl;
		}
		return len;
	};

	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_GetModuleFileNameW), GetModuleFileNameW_Hook);
}

/// <summary>
/// Creates a detour for the User32.dll CreateWindowExA function applying the following changes:
/// 1. Enable the window minimize box
/// </summary>
CreateWindowExA_t CreateWindowExA_Original = (CreateWindowExA_t)GetFuncAddress("USER32", "CreateWindowExA");
bool HookCreateWindowExA_initialized = true;
HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	if (HookCreateWindowExA_initialized)//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	{
		std::cout << "HookCreateWindowExA started" << std::endl;
		HookCreateWindowExA_initialized = false;
	}
	if (strstr(lpClassName, "MapleStoryClass"))
	{
		dwStyle |= WS_MINIMIZEBOX; // enable minimize button
		HWND ret = CreateWindowExA_Original(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		return ret;
	}
	if (strstr(lpClassName, "StartUpDlgClass"))
	{
		return nullptr; //kill startup balloon
	}
	//if(Client::WindowedMode)
	//{ //unfortunately doesnt work, reverting to old window mode fix
	//	dwExStyle = 0;
	//}
	return CreateWindowExA_Original(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
};
bool Hook_CreateWindowExA(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&CreateWindowExA_Original), CreateWindowExA_Hook);
}
bool CreateMutexA_initialized = true; ////credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
CreateMutexA_t CreateMutexA_Original = (CreateMutexA_t)GetFuncAddress("KERNEL32", "CreateMutexA");
HANDLE WINAPI CreateMutexA_Hook(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName)
{
	if (CreateMutexA_initialized)
	{
		std::cout << "HookCreateMutexA started" << std::endl;
		CreateMutexA_initialized = false;
	}
	if (!CreateMutexA_Original)
	{
		std::cout << "Original CreateMutex pointer corrupted. Failed to return mutex value to calling function." << std::endl;
		return nullptr;
	}
	if (lpName && strstr(lpName, "WvsClientMtx"))
	{
		// from https://github.com/pokiuwu/AuthHook-v203.4/blob/AuthHook-v203.4/Client176/WinHook.cpp
		char szMutex[128];	//multiclient
		int nPID = GetCurrentProcessId();
		sprintf_s(szMutex, "%s-%d", lpName, nPID);
		lpName = szMutex;
		return CreateMutexA_Original(lpMutexAttributes, bInitialOwner, lpName);
	}
	return CreateMutexA_Original(lpMutexAttributes, bInitialOwner, lpName);
}
bool Hook_CreateMutexA(bool bEnable)	//ty darter	//ty angel!
{
	//static auto _CreateMutexA = decltype(&CreateMutexA)(GetFuncAddress("KERNEL32", "CreateMutexA"));
	//decltype(&CreateMutexA) Hook = [](LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName) -> HANDLE
	//{
	//	if (CreateMutexA_initialized)
	//	{
	//		std::cout << "HookCreateMutexA started" << std::endl;
	//		CreateMutexA_initialized = false;
	//	}
	//	//Multi-Client Check Removal
	//	if (lpName && strstr(lpName, "WvsClientMtx"))
	//	{
	//		return (HANDLE)0x0BADF00D;
	//		//char szMutex[128];
	//		//lpName = szMutex;
	//	}
	//	return _CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
	//};	//original
	//return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CreateMutexA), Hook);
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&CreateMutexA_Original), CreateMutexA_Hook);
}
FindFirstFileA_t FindFirstFileA_Original = (FindFirstFileA_t)GetFuncAddress("KERNEL32", "FindFirstFileA");
bool FindFirstFileA_initialized = true; //ty joo for advice with this, check out their releases: https://github.com/OpenRustMS
HANDLE WINAPI FindFirstFileA_Hook(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData)
{
	if (FindFirstFileA_initialized)
	{
		std::cout << "HookFindFirstFileA started" << std::endl;
	}
	if (!FindFirstFileA_Original)
	{
		std::cout << "Original FindFirstFileA pointer corrupted. Failed to return ??? value to calling function." << std::endl;
		return nullptr;
	}
	if (lpFileName && strstr(lpFileName, "*") && FindFirstFileA_initialized)
	{
		FindFirstFileA_initialized = false;
		//std::cout << "FindFirstFileA dinput8.dll spoofed" << std::endl;
		return FindFirstFileA_Original("*.wz", lpFindFileData);
	}
	//else if (FindFirstFileA_initialized)
	//{
	//	std::cout << "FindFirstFileA failed... trying again" << std::endl;
	//	Sleep(1); //keep trying to find the file instead of failing
	//	return FindFirstFileA_Hook;
	//}
	//std::cout << "FindFirstFileA failed... unable to try again" << lpFileName << std::endl;
	FindFirstFileA_initialized = false;
	return FindFirstFileA_Original(lpFileName, lpFindFileData);
}
bool Hook_FindFirstFileA(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&FindFirstFileA_Original), FindFirstFileA_Hook);
}
GetLastError_t GetLastError_Original = (GetLastError_t)GetFuncAddress("KERNEL32", "GetLastError");
bool GetLastError_initialized = true;
DWORD WINAPI GetLastError_Hook()
{
	if (GetLastError_initialized)
	{
		std::cout << "HookGetLastError started" << std::endl;
		GetLastError_initialized = false;
	}
	DWORD error = GetLastError_Original();
	std::cout << "GetLastError: " << error << std::endl;
	return error;
}
bool Hook_GetLastError(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&GetLastError_Original), GetLastError_Hook);
}
sockaddr_in default_nXXXON_if;
#define WSAAddressToString  WSAAddressToStringA
bool WSPStartup_initialized = true; ////credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
INT WSPAPI WSPConnect_Hook(SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS, LPINT lpErrno)
{
	char szAddr[50];
	DWORD dwLen = 50;
	WSAAddressToString((sockaddr*)name, namelen, nullptr, szAddr, &dwLen);

	sockaddr_in* service = (sockaddr_in*)name;
	//hardcoded NXXON IP addies in default client
	if (strstr(szAddr, "63.251.217.2") || strstr(szAddr, "63.251.217.3") || strstr(szAddr, "63.251.217.4"))
	{
		default_nXXXON_if = *service;
		service->sin_addr.S_un.S_addr = inet_addr(MainMain::m_sRedirectIP);
		//service->sin_port = htons(MainMain::porthere);
		MainMain::m_GameSock = s;
	}

	return MainMain::m_ProcTable.lpWSPConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, lpErrno);
}
INT WSPAPI WSPGetPeerName_Hook(SOCKET s, struct sockaddr* name, LPINT namelen, LPINT lpErrno)
{
	int nRet = MainMain::m_ProcTable.lpWSPGetPeerName(s, name, namelen, lpErrno);//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	if (nRet != SOCKET_ERROR && s == MainMain::m_GameSock)
	{
		sockaddr_in* service = (sockaddr_in*)name; //suspecting this is for checking rather than actually connecting
		service->sin_addr.S_un.S_addr = default_nXXXON_if.sin_addr.S_un.S_addr;//inet_addr(MainMain::m_sRedirectIP)
		//service->sin_port = htons(MainMain::porthere);
	}
	return nRet;
}
INT WSPAPI WSPCloseSocket_Hook(SOCKET s, LPINT lpErrno)
{//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	int nRet = MainMain::m_ProcTable.lpWSPCloseSocket(s, lpErrno);
	if (s == MainMain::m_GameSock)
	{
		MainMain::m_GameSock = INVALID_SOCKET;
	}
	return nRet;
}
WSPStartup_t WSPStartup_Original = (WSPStartup_t)GetFuncAddress("MSWSOCK", "WSPStartup"); /*in this function we'll call the WSP Functions*/
const wchar_t* v42;
INT WSPAPI WSPStartup_Hook(WORD wVersionRequested, LPWSPDATA lpWSPData, LPWSAPROTOCOL_INFOW lpProtocolInfo, WSPUPCALLTABLE UpcallTable, LPWSPPROC_TABLE lpProcTable)
{
	if (WSPStartup_initialized)//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	{
		std::cout << "HookWSPStartup started" << std::endl;
		WSPStartup_initialized = false;
	}
	int nRet = WSPStartup_Original(wVersionRequested, lpWSPData, lpProtocolInfo, UpcallTable, lpProcTable);

	if (nRet == NO_ERROR)
	{
		MainMain::m_GameSock = INVALID_SOCKET;
		MainMain::m_ProcTable = *lpProcTable;

		lpProcTable->lpWSPConnect = WSPConnect_Hook;
		lpProcTable->lpWSPGetPeerName = WSPGetPeerName_Hook;
		lpProcTable->lpWSPCloseSocket = WSPCloseSocket_Hook;
	}
	else
	{
		std::cout << "WSPStartup Error Code: " + nRet << std::endl;
	}
	return nRet;
}
bool Hook_WSPStartup(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&WSPStartup_Original), WSPStartup_Hook);
}
#define x86CMPEAX 0x3D
bool GetACP_initialized = true;
DWORD LocaleSpoofValue = 0;//choose value from https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
GetACP_t GetACP_Original = (GetACP_t)GetFuncAddress("KERNEL32", "GetACP");
UINT WINAPI GetACP_Hook()
{ // AOB: FF 15 ?? ?? ?? ?? 3D ?? ?? ?? 00 00 74 <- library call inside winmain func
	if (GetACP_initialized)
	{//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate	
		std::cout << "HookGetACP started" << std::endl;
		GetACP_initialized = false;	//NOTE: idk what this really does tbh, maybe it is custom locale value but more likely it is to skip a check
	}	//that some clients may have that restricts you based on locale; if it is not a check and instead logged by server feel free to feed bad data by disabling the part below
	UINT uiNewLocale = LocaleSpoofValue;
	if (uiNewLocale == 0) { return GetACP_Original(); } //we do hook in my version!//should not happen cuz we dont hook if value is zero
	DWORD dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());
	// return address should be a cmp eax instruction because ret value is stored in eax
	// and nothing else should happen before the cmp
	if (ReadValue<BYTE>(dwRetAddr) == x86CMPEAX)
	{	//disable this if and else if you wanna always use spoof value (i.e. if server logs it)
		uiNewLocale = ReadValue<DWORD>(dwRetAddr + 1); // check value is always 4 bytes
		std::cout << "[GetACP] Found desired locale: " << uiNewLocale << std::endl;
	}
	else { std::cout << "[GetACP] Unable to automatically determine locale, using stored locale: " << uiNewLocale << std::endl; }
	std::cout << "[GetACP] Locale spoofed to: " << uiNewLocale << " unhooking. Calling address: " << dwRetAddr << std::endl;
	if (Memory::SetHook(FALSE, reinterpret_cast<void**>(&GetACP_Original), GetACP_Hook))
	{
		std::cout << "Failed to unhook GetACP." << std::endl;
	}
	return uiNewLocale;
}
bool Hook_GetACP(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&GetACP_Original), GetACP_Hook);
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
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FAF55), _PcCreateObject_IWzResMan_Hook);
}
bool HookPcCreateObject_IWzNameSpace(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FAFBA), _PcCreateObject_IWzNameSpace_Hook);
}
bool HookPcCreateObject_IWzFileSystem(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FB01F), _PcCreateObject_IWzFileSystem_Hook);
}
bool HookCWvsApp__Dir_BackSlashToSlash(bool bEnable)
{
	BYTE firstval = 0x56;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F95FE;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F95FE), _CWvsApp__Dir_BackSlashToSlash_rewrite);
}
bool HookCWvsApp__Dir_upDir(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F9644), _CWvsApp__Dir_upDir_Hook);
}
bool Hookbstr_ctor(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_406301), _bstr_ctor_Hook);
}
bool HookIWzNameSpace__Mount(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F790A), _IWzNameSpace__Mount_Hook);
}
//void loadMyShA() {	//partial credits to blackwings v95
//	void* pDataFileSystem = nullptr;
//	void* pThePackage = nullptr; //9FB0E9
//	//@_com_ptr_t<_com_IIID<IWzNameSpace,&_GUID_2aeeeb36_a4e1_4e2b_8f6f_2e7bdec5c53d> > g_root
//	//_sub_9FAFBA(L"NameSpace", g_root, NULL);//void __cdecl PcCreateObject(const wchar_t* sUOL, _com_ptr_t<_com_IIID<IWzNameSpace, &_GUID_2aeeeb36_a4e1_4e2b_8f6f_2e7bdec5c53d> > *pObj, IUnknown * pUnkOuter)
//
//	void* pIWzNameSpace_Instance = g_root; //partial credits to https://github.com/MapleMyth/ClientImageLoader
//	//auto PcSetRootNameSpace = (void(__cdecl*)(void*, int)) * (int*)pNameSpace;//Hard Coded//HRESULT __cdecl PcSetRootNameSpace(IUnknown *pNameSpace)
//	//PcSetRootNameSpace(pIWzNameSpace_Instance, 1);
//
//	char sStartPath[MAX_PATH];
//	GetModuleFileNameA(NULL, sStartPath, MAX_PATH);
//	_CWvsApp__Dir_BackSlashToSlash_rewrite(sStartPath);	//_sub_9F95FE
//	_sub_9F9644(sStartPath);//_CWvsApp__Dir_upDir
//
//	strcat(sStartPath, "/Ezorsia_v2_files");//sStartPath += "./Ezorsia_v2_files";
//	//char sStartPath2[MAX_PATH]; strcpy(sStartPath2, sStartPath);
//	//strcat(sStartPath2, "/");//sStartPath += "./Ezorsia_v2_files";
//
//	Ztl_bstr_t BsStartPath = Ztl_bstr_t();
//	_sub_425ADD(&BsStartPath, nullptr, sStartPath);//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s) //Ztl_bstr_t ctor
//	//Ztl_bstr_t BsStartPath2 = Ztl_bstr_t();
//	//_sub_425ADD(&BsStartPath2, nullptr, "/");//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s) //Ztl_bstr_t ctor
//
//	_sub_9FB01F(L"NameSpace#FileSystem", &pDataFileSystem, NULL);//void __cdecl PcCreateObject(const wchar_t *sUOL, _com_ptr_t<_com_IIID<IWzFileSystem,&_GUID_352d8655_51e4_4668_8ce4_0866e2b6a5b5> > *pObj, IUnknown *pUnkOuter)
//	_sub_9FB084(L"NameSpace#Package", &pThePackage, NULL);//void __cdecl PcCreateObject_IWzPackage(const wchar_t *sUOL, ??? *pObj, IUnknown *pUnkOuter)
//	HRESULT v0 =_sub_9F7964(pDataFileSystem, nullptr, BsStartPath);//HRESULT __thiscall IWzFileSystem::Init(IWzFileSystem *this, Ztl_bstr_t sPath)
//	std::cout << v0 << " Hook_sub_9F7159 HRESULT 1: " << BsStartPath.m_Data << "   " << sStartPath << std::endl;
//	
//	const char* myWzPath = "EzorsiaV2_wz_file.wz";
//	Ztl_bstr_t BmyWzPath = Ztl_bstr_t();
//	_sub_425ADD(&BmyWzPath, nullptr, myWzPath);//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s) //Ztl_bstr_t ctor
//
//	Ztl_variant_t pBaseData = Ztl_variant_t();
//	_sub_5D995B(pDataFileSystem, nullptr, &pBaseData, BmyWzPath);//Ztl_variant_t *__thiscall IWzNameSpace::Getitem(IWzNameSpace *this, Ztl_variant_t *result, Ztl_bstr_t sPath)
//
//	const char* mysKey = "83";
//	Ztl_bstr_t BmysKey = Ztl_bstr_t();
//	_sub_425ADD(&BmysKey, nullptr, mysKey);//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s) //Ztl_bstr_t ctor
//	const char* mysBaseUOL = "/";
//	Ztl_bstr_t BmysBaseUOL = Ztl_bstr_t();
//	_sub_425ADD(&BmysBaseUOL, nullptr, mysBaseUOL);//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s) //Ztl_bstr_t ctor
//	_sub_9F79B8(pThePackage, nullptr, BmysKey, BmysBaseUOL, pCustomArchive);//HRESULT __thiscall IWzPackage::Init(IWzPackage *this, Ztl_bstr_t sKey, Ztl_bstr_t sBaseUOL, IWzSeekableArchive *pArchive)
//
//
//
//	_sub_425ADD(&BsStartPath, nullptr, "/");//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s) //Ztl_bstr_t ctor
//	HRESULT v1 = _sub_9F790A(pIWzNameSpace_Instance, nullptr, BsStartPath, pThePackage, 0); //HRESULT __thiscall IWzNameSpace::Mount(IWzNameSpace *this, Ztl_bstr_t sPath, IWzNameSpace *pDown, int nPriority)
//	std::cout << v1 << " Hook_sub_9F7159 HRESULT 2: " << BsStartPath.m_Data << "   " << sStartPath << std::endl;
//
//
//
//
//	//void __thiscall _com_ptr_t<_com_IIID<IWzSeekableArchive == v95 9C4830 v83 9F7367
//} 
//bool Hook_sub_9F7159_initialized = true;
bool resmanSTARTED = false;
static _CWvsApp__InitializeResMan_t _sub_9F7159_append = [](CWvsApp* pThis, void* edx) {
	//-> void {_CWvsApp__InitializeResMan(pThis, edx);
	//if (Hook_sub_9F7159_initialized)
	//{
	//	std::cout << "_sub_9F7159 started" << std::endl;
	//	Hook_sub_9F7159_initialized = false;
	//}
	resmanSTARTED = true;
	//loadMyShA();
	//void* pData = nullptr;
	//void* pFileSystem = nullptr;
	//void* pUnkOuter = 0;
	//void* nPriority = 0;
	//void* sPath;
	//edx = nullptr
	// 
	//// Resman
	//_PcCreateObject_IWzResMan(L"ResMan", &g_rm, pUnkOuter);	//?(void*) //?&g

	//void* pIWzResMan_Instance = *&g_rm;	//?&g	//custom added, find existing instance
	//!!auto IWzResMan__SetResManParam = *(void(__fastcall**)(void*, void*, void*, int, int, int))((*(int*)pIWzResMan_Instance) + 20); // Hard Coded
	//!!IWzResMan__SetResManParam(nullptr, nullptr, pIWzResMan_Instance, RC_AUTO_REPARSE | RC_AUTO_SERIALIZE, -1, -1);

	//// NameSpace
	//_PcCreateObject_IWzNameSpace(L"NameSpace", &g_root, pUnkOuter);

	//void* pIWzNameSpace_Instance = &g_root;
	//auto PcSetRootNameSpace = (void(__cdecl*)(void*, int)) * (int*)pNameSpace; // Hard Coded
	//PcSetRootNameSpace(pIWzNameSpace_Instance, 1);

	//// Game FileSystem
	//_PcCreateObject_IWzFileSystem(L"NameSpace#FileSystem", &pFileSystem, pUnkOuter);
	_sub_9F7159(pThis, nullptr);	//comment this out and uncomment below if testing, supposed to load from .img files in folders but i never got to test it
	resmanSTARTED = false;
	//char sStartPath[MAX_PATH];
	//GetModuleFileNameA(NULL, sStartPath, MAX_PATH);
	//_CWvsApp__Dir_BackSlashToSlash(sStartPath);
	//_CWvsApp__Dir_upDir(sStartPath);

	//_bstr_ctor(&sPath, pData, sStartPath);

	//auto iGameFS = _IWzFileSystem__Init(pFileSystem, pData, sPath);

	//_bstr_ctor(&sPath, pData, "/");

	//auto mGameFS = _IWzNameSpace__Mount(*&g_root, pData, sPath, pFileSystem, (int)nPriority);

	//// Data FileSystem
	//_PcCreateObject_IWzFileSystem(L"NameSpace#FileSystem", &pFileSystem, pUnkOuter);

	//_bstr_ctor(&sPath, pData, "./Data");

	//auto iDataFS = _IWzFileSystem__Init(pFileSystem, pData, sPath);

	//_bstr_ctor(&sPath, pData, "/");

	//auto mDataFS = _IWzNameSpace__Mount(*&g_root, pData, sPath, pFileSystem, (int)nPriority);
};
bool Hook_sub_9F7159(bool bEnable)	//resman hook that does nothing, kept for analysis and referrence //not skilled enough to rewrite to load custom wz files
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F7159;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F7159), _sub_9F7159_append);
}
bool Hook_StringPool__GetString_initialized = true;
_StringPool__GetString_t _StringPool__GetString_rewrite = [](void* pThis, void* edx, ZXString<char>* result, unsigned int nIdx, char formal) -> ZXString<char>* {
	if (Hook_StringPool__GetString_initialized)
	{
		std::cout << "Hook_StringPool__GetString started" << std::endl;
		Hook_StringPool__GetString_initialized = false;
	}
	auto ret = _sub_79E993(pThis, nullptr, result, nIdx, formal);//_StringPool__GetString_t
	switch (nIdx)
	{
	case 1307:	//1307_UI_LOGINIMG_COMMON_FRAME = 51Bh
		if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownLoginFrame)
		{
			switch (Client::m_nGameWidth)
			{
			case 1280:	//ty teto for the suggestion to use ZXString<char>::Assign and showing me available resources
				*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1280");
				break;
			case 1366:
				*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1366");
				break;
			case 1600:
				*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1600");
				break;
			case 1920:
				*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1920");
				break;
			case 1024:
				*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1024");
				break;
			}
			break;
		}
	case 1301:	//1301_UI_CASHSHOPIMG_BASE_BACKGRND  = 515h
		if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd"); }
		break;
	case 1302:	//1302_UI_CASHSHOPIMG_BASE_BACKGRND1 = 516h
		if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd1"); }
		break;
	case 5361:	//5361_UI_CASHSHOPIMG_BASE_BACKGRND2  = 14F1h			
		if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd2"); }
		break;
	//case 1302:	//BACKGRND??????
	//	if (EzorsiaV2WzIncluded && ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd1"); } break;
	//case 5361:	//SP_1937_UI_UIWINDOWIMG_STAT_BACKGRND2  = 791h	
	//	if (EzorsiaV2WzIncluded && ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd2"); } break;
	}
	return ret;
};
bool Hook_StringPool__GetString(bool bEnable)	//hook stringpool modification //ty !! popcorn //ty darter
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x0079E993;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_79E993), _StringPool__GetString_rewrite);//_StringPool__GetString_t
}
bool HookMyTestHook(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9DE4D2), _CWndCreateWnd_Hook);
}

bool HookDetectLogin(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_5F726D), _CLoginSendSelectCharPacket_Hook);
}
bool Hook_lpfn_NextLevel_initialized = true;//__thiscall NEXTLEVEL::NEXTLEVEL(NEXTLEVEL *this)
constexpr int maxLevel = 201;//determine the max level for characters in your setup here //your max level is the size of your array divded by operator size( currently int size)
static _sub_78C8A6_t _sub_78C8A6_rewrite = [](unsigned int NEXTLEVEL[maxLevel], void* edx) {
	if (Hook_lpfn_NextLevel_initialized)
	{
		std::cout << "Hook exptable@_sub_78C8A6 started" << std::endl;
		Hook_lpfn_NextLevel_initialized = false;
	}
	edx = nullptr;
	MainMain::useV62_ExpTable ? memcpy(NEXTLEVEL, MainMain::v62ArrayForCustomEXP, MainMain::expTableMemSize) : memcpy(NEXTLEVEL, MainMain::v83ArrayForCustomEXP, MainMain::expTableMemSize);
	NEXTLEVEL[maxLevel] = 0;//(pThis->n)[len / sizeof((pThis->n)[0])] = 0; //set the max level to 0
	return NEXTLEVEL;
	//if you want to use your own formula rewrite this function. generated numbers MUST MATCH server numbers
};
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
bool Hook_sub_78C8A6(bool bEnable)
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x0078C8A6;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_78C8A6), _sub_78C8A6_rewrite);
	//return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_lpfn_NextLevel), _lpfn_NextLevel_v62_Hook);
}
bool Hook_CUIStatusBar__ChatLogAdd(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_8DB070), _CUIStatusBar__ChatLogAdd_Hook);
}
bool Hook_sub_9F9808(bool bEnable)	//1
{
	static _sub_9F9808_t _sub_9F9808_Hook = [](void) {
		return _sub_9F9808();
		//int v1; // esi
		//DWORD* v2; // eax
		//int v3; // ST08_4
		//ZXString<char> v5; // [esp+4h] [ebp-10h]
		//int v6; // [esp+10h] [ebp-4h]

		//if (!_byte_BF1AD0[0])
		//{
		//	v1 = _dword_BF039C((DWORD)_byte_BF1AD0, 260, a1);
		//	_sub_9F9621(_byte_BF1AD0);
		//	if (v1)
		//	{
		//		if (_byte_BF1ACF[v1] != 92)
		//			*(_byte_BF1AD0 + v1++) = 92;
		//	}
		//	v2 = (DWORD*)_sub_79E805();//(DWORD*)StringPool::GetInstance();
		//	v3 = *(DWORD*)_sub_406455(v2, &v5, 2512);//*_StringPool__GetString(v2, &v5, 2512, 0);
		//	v6 = 0;
		//	_dword_BF03BC((DWORD)_byte_BF1AD0 + v1);
		//	v6 = -1;
		//	v5.~ZXString();
		//}
		//return _byte_BF1AD0;
	};	//2
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F9808), _sub_9F9808_Hook);	//2
}
bool Hook_sub_4959B8(bool bEnable)	//1
{
	static _sub_4959B8_t _sub_4959B8_Hook = [](void* pThis, void* edx, void* pData, unsigned int uSize) {
		return _sub_4959B8(pThis, nullptr, pData, uSize);
		//int v3; // esi
		//unsigned __int64 v4; // rax
		//size_t v5; // ebx
		//unsigned __int8 v6; // cf
		//size_t result; // eax
		//int v8; // edx
		//int v9; // [esp+0h] [ebp-10h]
		//int v10; // [esp+Ch] [ebp-4h]
//
		//v3 = a1;
		//if (*(DWORD*)(a1 + 24))
		//{
		//	DWORD v4hi = *(DWORD*)(a1 + 12);	
		//	DWORD v4lo = LODWORD(v4);
		//	v4 = MAKELONGLONG(v4lo, v4hi);	//HIDWORD(v4) = *(DWORD*)(a1 + 12);
//
		//	DWORD ahi = HIDWORD(a1);
		//	DWORD alo = *(DWORD*)(a1 + 36);
		//	a1 = MAKELONGLONG(alo, ahi);	//LODWORD(a1) = *(DWORD*)(a1 + 36);
		//	DWORD v4lo = *(DWORD*)(v3 + 8);
		//	
		//	DWORD v4hi1 = HIDWORD(v4);
		//	DWORD v4lo1 = *(DWORD*)(v3 + 8);
		//	v4 = MAKELONGLONG(v4lo1, v4hi1);	//LODWORD(v4) = *(DWORD*)(v3 + 8);
//
		//	if ((unsigned int)a1 <= HIDWORD(v4))
		//	{
		//		unsigned __int64 a1_2 = a1;
		//		DWORD a1_2hi = HIDWORD(a1_2);
		//		DWORD a1_2lo = *(DWORD*)(v3 + 32);	//if ((unsigned int)a1 < HIDWORD(v4) || (LODWORD(a1) = *(DWORD*)(v3 + 32), (unsigned int)a1 <= (unsigned int)v4))
		//		a1_2 = MAKELONGLONG(a1_2lo, a1_2hi);	//LODWORD(v4) = *(DWORD*)(v3 + 8);  //in if statement
		//		if ((unsigned int)a1 < HIDWORD(v4) || (unsigned int)a1_2 <= (unsigned int)v4)
		//		{
		//			a1 = *(QWORD*)(v3 + 40);
		//			if (v4 <= a1)
		//			{
		//				if (v4 + a3 <= *(QWORD*)(v3 + 40))
		//				{
		//					v5 = a3;
		//					memcpy(a2, (const void*)(*(DWORD*)(v3 + 24) + *(DWORD*)(v3 + 8)), a3);
		//					v6 = __CFADD__(v5, *(DWORD*)(v3 + 8));
		//					*(DWORD*)(v3 + 8) += v5;
		//					result = v5;
		//					*(DWORD*)(v3 + 12) += v6;
		//					return result;
		//				}
		//				*(DWORD*)(v3 + 8) = _sub_49583A(*(DWORD*)(v3 + 16), *(DWORD*)(v3 + 8), SHIDWORD(v4), 0);
		//				*(DWORD*)(v3 + 12) = v8;
		//			}
		//		}
		//	}
		//}
		//v10 = 0;
		//if (!_dword_BF0358(a1, *(DWORD*)(v3 + 16), (DWORD)a2, a3, &v10, 0) && ((int(__cdecl*)())_dword_BF03A4)() != 109)
		//{
		//	a3 = _dword_BF03A4(v9);
		//	_CxxThrowException(&a3, _TI1_AVZException__);
		//}
		//result = v10;
		//*(QWORD*)(v3 + 8) += (unsigned int)v10;
		//return result;
	};	//2
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_4959B8), _sub_4959B8_Hook);	//2
}//CClientSocket::Connect(CClientSocket *this, CClientSocket::CONNECTCONTEXT *ctx)??
//bool Hook_sub_44E546(bool bEnable)
//{
//	static _sub_44E546_t _sub_44E546_Hook = [](unsigned __int8* a1, int a2) {
//		signed int v2; // edx
//		int* v3; //int* v3; // ecx
//		unsigned int v4; // eax
//		signed int v5; // esi
//		unsigned __int8* v6; // ecx
//		unsigned int i; // eax
//		int v9[256]; // [esp+4h] [ebp-400h]
//
//		v2 = 0;
//		//cc0x0044E546get(v9);
//		v3 = v9;// [v2] ;//v3 = v9;
//		do
//		{
//			v4 = v2;
//			v5 = 8;
//			do
//			{
//				if (v4 & 1)
//					v4 = (v4 >> 1) ^ 0xED1883C7;
//				else
//					v4 >>= 1;
//				--v5;
//			} while (v5);
//			*v3 = v4;//v9[v2] = v4;//*v3 = v4;
//			++v2;
//			++v3;
//		} while (v2 < 256);
//		v6 = a1;
//		for (i = -1; v6 < &a1[a2]; ++v6)
//			i = v9[*v6 ^ (unsigned __int8)i] ^ (i >> 8);
//		return ~i;
//	};
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44E546), _sub_44E546_Hook);
//}
//bool Hook_sub_44E5D5(bool bEnable)	//1
//{
//	static _sub_44E5D5_t _sub_44E5D5_Hook = [](int a1, void* a2, size_t a3) {
//		unsigned int v3; // esi
//		unsigned __int8* v4; // eax
//		void* v5; // ebx
//		int v6; // edx
//		DWORD* v7; // eax
//		int v8; // edi
//		unsigned int v9; // esi
//		int v10; // ecx
//		unsigned int v12; // [esp+4h] [ebp-8h]
//		unsigned __int8* lpMem; // [esp+8h] [ebp-4h]
//		int i; // [esp+14h] [ebp+8h]
//		WORD* v15; // [esp+18h] [ebp+Ch]
//
//		v3 = 0;
//		v4 = (unsigned __int8*)malloc(a3);
//		lpMem = v4;
//		if (v4)
//		{
//			v5 = a2;
//			memcpy(v4, a2, a3);
//			v6 = a1;
//			v7 = (DWORD*)(*(DWORD*)(*(DWORD*)(a1 + 60) + a1 + 160) + a1);
//			for (i = *(DWORD*)(*(DWORD*)(a1 + 60) + a1 + 164); i; v7 = (DWORD*)((char*)v7 + v10))
//			{
//				v8 = v6 + *v7;
//				if ((unsigned int)(v7[1] - 8) >> 1)
//				{
//					v15 = (WORD*)v7 + 2;
//					v12 = (unsigned int)(v7[1] - 8) >> 1;
//					do
//					{
//						if ((*v15 & 0xF000) == 12288)
//						{
//							v9 = v8 + (*v15 & 0xFFF);
//							if (v9 >= (unsigned int)v5 && v9 < (unsigned int)v5 + a3)
//								*(DWORD*)&lpMem[v9 - (DWORD)v5] -= v6;
//						}
//						++v15;
//						--v12;
//					} while (v12);
//				}
//				v10 = v7[1];
//				i -= v10;
//			}
//			v3 = _sub_44E546(lpMem, a3);
//			_sub_A61DF2(lpMem);
//		}
//		return v3;
//	};	//2
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44E5D5), _sub_44E5D5_Hook);	//2
//}
//bool Hook_sub_44E716(bool bEnable)
//{
//	static _sub_44E716_t _sub_44E716_Hook = [](int a1, void* a2, size_t a3) {
//
//		unsigned int v3; // esi
//		unsigned __int8* v4; // eax
//		DWORD* v5; // edi
//		DWORD* v6; // esi
//		int v7; // eax
//		unsigned int v8; // ecx
//		int v9; // eax
//		unsigned int v11; // [esp+4h] [ebp-10h]
//		unsigned __int8* lpMem; // [esp+8h] [ebp-Ch]
//		int i; // [esp+Ch] [ebp-8h]
//		WORD* v14; // [esp+10h] [ebp-4h]
//
//		v3 = 0;
//		v4 = (unsigned __int8*)malloc(a3);
//		lpMem = v4;
//		if (v4)
//		{
//			memcpy(v4, a2, a3);
//			v5 = (DWORD*)(a1 + *(DWORD*)(a1 + 60));
//			v6 = (DWORD*)(a1 + _sub_44E6C3(a1, v5[40]));
//			for (i = v5[41]; i; v6 = (DWORD*)((char*)v6 + v9))
//			{
//				v7 = a1 + _sub_44E6C3(a1, *v6);
//				if ((unsigned int)(v6[1] - 8) >> 1)
//				{
//					v14 = (WORD*)v6 + 2;
//					v11 = (unsigned int)(v6[1] - 8) >> 1;
//					do
//					{
//						if ((*v14 & 0xF000) == 12288)
//						{
//							v8 = v7 + (*v14 & 0xFFF);
//							if (v8 >= (unsigned int)a2 && v8 < (unsigned int)a2 + a3)
//								*(DWORD*)&lpMem[v8 - (DWORD)a2] -= v5[13];
//						}
//						++v14;
//						--v11;
//					} while (v11);
//				}
//				v9 = v6[1];
//				i -= v9;
//			}
//			v3 = _sub_44E546(lpMem, a3);
//			_sub_A61DF2(lpMem);
//		}
//		return v3;
//	};
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44E716), _sub_44E716_Hook);
//}
//bool sub_44E88E_initialized = true; //int(__stdcall* __stdcall MyGetProcAddress(HINSTANCE__* hModule, const char* lpProcName))()
static _sub_44E88E_t _sub_44E88E_rewrite = [](HINSTANCE__* hModule, const char* lpProcName) {
	//if (sub_44E88E_initialized)
	//{
	//	std::cout << "sub_44E88E started" << std::endl;
	//	sub_44E88E_initialized = false;
	//}
	return _sub_44E88E(hModule, lpProcName);
	//int result; // eax
	//int v3; // edx
	//int v4; // ecx
	//int v5; // ebx
	//int v6; // ecx
	//int v7; // ecx
	//DWORD* v8; // ebx
	//unsigned int v9; // eax
	//unsigned int v10; // ecx
	//DWORD* v11; // esi
	//unsigned __int8* v12; // edi
	//char v13; // [esp+14h] [ebp-5Ch]
	//unsigned int i; // [esp+18h] [ebp-58h]
	//char v15; // [esp+1Ch] [ebp-54h]
	//unsigned int v16; // [esp+20h] [ebp-50h]
	//BYTE* v17; // [esp+2Ch] [ebp-44h]
	//char* v18; // [esp+2Ch] [ebp-44h]
	//unsigned int v19; // [esp+34h] [ebp-3Ch]

	//result = 0;
	//v3 = a1;
	//if (*(WORD*)a1 == 23117)
	//{
	//	v4 = a1 + *(DWORD*)(a1 + 60);
	//	if (*(DWORD*)v4 == 17744)
	//	{
	//		if (*(WORD*)(v4 + 24) == 523)
	//		{
	//			v5 = *(DWORD*)(v4 + 136);
	//			v6 = *(DWORD*)(v4 + 140);
	//		}
	//		else
	//		{
	//			v5 = *(DWORD*)(v4 + 120);
	//			v7 = *(DWORD*)(v4 + 124);
	//		}
	//		if (v5)
	//		{
	//			v8 = (DWORD*)(a1 + v5);
	//			v9 = a2;
	//			if (a2 >= 0x10000)
	//			{
	//				v16 = 0;
	//				v17 = (BYTE*)a2;
	//				while (*v17)
	//				{
	//					++v17;
	//					if (++v16 >= 0x100)
	//						return 0;
	//				}
	//				v11 = (DWORD*)(a1 + v8[8]);
	//				v15 = 0;
	//				v19 = 0;
	//				while (v19 < v8[6])
	//				{
	//					v12 = (unsigned __int8*)(v3 + *v11);
	//					v18 = (char*)v9;
	//					if (IsBadReadPtr((const void*)(v3 + *v11), 4u))
	//						return 0;
	//					v13 = 1;
	//					for (i = 0; i < v16; ++i)
	//					{
	//						if (*v12 != *v18)
	//						{
	//							v13 = 0;
	//							break;
	//						}
	//						++v12;
	//						++v18;
	//					}
	//					if (v13)
	//					{
	//						v15 = 1;
	//						v3 = a1;
	//						break;
	//					}
	//					++v11;
	//					++v19;
	//					v3 = a1;
	//					v9 = a2;
	//				}
	//				if (!v15)
	//					return 0;
	//				v10 = *(unsigned __int16*)(v3 + v8[9] + 2 * v19);
	//			}
	//			else
	//			{
	//				v10 = a2 - 1;
	//			}
	//			result = v3 + *(DWORD*)(v3 + v8[7] + 4 * v10);
	//		}
	//	}
	//}
	//return result;
};
bool Hook_sub_44E88E(bool bEnable)
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x0044E88E;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44E88E), _sub_44E88E_rewrite);
}
//bool Hook_sub_44EA64(bool bEnable)
//{
//	static _sub_44EA64_t _sub_44EA64_Hook = [](void* pThis, void* edx) {
//		edx = nullptr;
//		//sub_44EA64_get(v3, lDistanceToMove);
//		HANDLE v0; // edi
//		int v2; // [esp+8h] [ebp-44Ch]
//		unsigned int v3 = 0;// = DWORD PTR[ebp - 0x8]; // [esp+3Ch] [ebp-418h] !!
//		char const v4cpy[] = "ws2_32.dll";
//		const int v4cpysize = sizeof(v4cpy);
//		char v4[v4cpysize];//const char* v4 = "ws2_32.dll"; // [esp+100h] [ebp-354h]
//		CHAR Buffer; // [esp+204h] [ebp-250h]
//		CHAR PathName; // [esp+308h] [ebp-14Ch]
//		__int16 v7; // [esp+40Ch] [ebp-48h]
//		LONG lDistanceToMove = 0; // [esp+448h] [ebp-Ch] !!
//		DWORD NumberOfBytesRead; // [esp+44Ch] [ebp-8h]
//		HANDLE hFile; // [esp+450h] [ebp-4h]
//		//sub_44EA64_get_v3(v3);
//		//LPCSTR PrefixString;
//		//const DWORD PrefixStringSrc = 0x00AF13FC;
//		//memmove(&PrefixString, &PrefixStringSrc, sizeof(LPCSTR));
//
//		GetSystemDirectoryA(&Buffer, 0x104u);
//		strcpy(v4, v4cpy); //sub_44EA64_strcpy(v4); //
//		strcat(&Buffer, "\\");
//		strcat(&Buffer, v4);
//		GetTempPathA(0x104u, &PathName);
//		CHAR PrefixString[] = "nst";
//		GetTempFileNameA(&PathName, PrefixString, 0, &PathName); //sub_44EA64_get_PrefixString(&PathName, 0, &PathName); //
//		CopyFileA(&Buffer, &PathName, 0);
//		v0 = CreateFileA(&PathName, 0xC0000000, 3u, 0, 3u, 0x80u, 0);
//		hFile = v0;
//		if (v0 != (HANDLE)-1)
//		{
//			ReadFile(v0, &v7, 0x40u, &NumberOfBytesRead, 0);
//			if (v7 == 23117)
//			{
//				SetFilePointer(v0, lDistanceToMove, 0, 0);
//				ReadFile(hFile, &v2, 0xF8u, &NumberOfBytesRead, 0);
//				if (v2 == 17744 && v3 > 0x80000000)
//				{
//					v3 = 0x10000000;
//					SetFilePointer(hFile, lDistanceToMove, 0, 0);
//					WriteFile(hFile, &v2, 0xF8u, &NumberOfBytesRead, 0);
//				}
//			}
//			CloseHandle(hFile);
//		}
//		return LoadLibraryExA(&PathName, 0, 8u);
//	};
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44EA64), _sub_44EA64_Hook);
//}
//bool Hook_sub_44EC9C(bool bEnable)	//1
//{
//	static _sub_44EC9C_t _sub_44EC9C_Hook = [](int a1) {
//		DWORD* result; // eax
//		int v2; // ecx
//
//		for (result = *(DWORD**)(*(DWORD*)(*(DWORD*)(__readfsdword(0x18u) + 48) + 12) + 12); ; result = (DWORD*)*result)
//		{
//			v2 = result[6];
//			if (!v2 || v2 == a1)
//				break;
//		}
//		if (result[6])
//		{
//			*(DWORD*)result[1] = *result;
//			*(DWORD*)(*result + 4) = result[1];
//			*(DWORD*)result[3] = result[2];
//			*(DWORD*)(result[2] + 4) = result[3];
//			*(DWORD*)result[5] = result[4];
//			*(DWORD*)(result[4] + 4) = result[5];
//			*(DWORD*)result[16] = result[15];
//			*(DWORD*)(result[15] + 4) = result[16];
//			memset(result, 0, 0x48u);
//		}
//		return result;
//	};	//2
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44EC9C), _sub_44EC9C_Hook);	//2
//}
//void __cdecl ResetLSP(void)
static _sub_44ED47_t _sub_44ED47_rewrite = []() {
	return _sub_44ED47();
	//edx = nullptr;
//		DWORD result; // eax
//		CHAR Buffer; // [esp+8h] [ebp-184h]
//		struct _STARTUPINFOA StartupInfo; // [esp+10Ch] [ebp-80h]
//		struct _PROCESS_INFORMATION ProcessInformation; // [esp+150h] [ebp-3Ch]
//		DWORD cbData; // [esp+160h] [ebp-2Ch]
//		int v5; // [esp+164h] [ebp-28h]
//		char* v6; // [esp+168h] [ebp-24h]
//		char v7; // [esp+16Ch] [ebp-20h]
//		char v8; // [esp+16Dh] [ebp-1Fh]
//		char v9; // [esp+16Eh] [ebp-1Eh]
//		char v10; // [esp+16Fh] [ebp-1Dh]
//		char v11; // [esp+170h] [ebp-1Ch]
//		char v12; // [esp+171h] [ebp-1Bh]
//		char v13; // [esp+172h] [ebp-1Ah]
//		char v14; // [esp+173h] [ebp-19h]
//		char v15; // [esp+174h] [ebp-18h]
//		char v16; // [esp+175h] [ebp-17h]
//		char v17; // [esp+176h] [ebp-16h]
//		char v18; // [esp+177h] [ebp-15h]
//		char v19; // [esp+178h] [ebp-14h]
//		char v20; // [esp+179h] [ebp-13h]
//		char v21; // [esp+17Ah] [ebp-12h]
//		char v22; // [esp+17Bh] [ebp-11h]
//		char v23; // [esp+17Ch] [ebp-10h]
//		char v24; // [esp+17Dh] [ebp-Fh]
//		char v25; // [esp+17Eh] [ebp-Eh]
//		char v26; // [esp+17Fh] [ebp-Dh]
//		char v27; // [esp+180h] [ebp-Ch]
//		char v28; // [esp+181h] [ebp-Bh]
//		char v29; // [esp+182h] [ebp-Ah]
//		char v30; // [esp+183h] [ebp-9h]
//		char v31; // [esp+184h] [ebp-8h]
//		char v32; // [esp+185h] [ebp-7h]
//		HKEY phkResult; // [esp+188h] [ebp-4h]
//
//		v5 = 0;
//		result = RegOpenKeyExA(
//			HKEY_LOCAL_MACHINE,
//			"SYSTEM\\CurrentControlSet\\Services\\WinSock2\\Parameters\\Protocol_Catalog9\\Catalog_Entries\\000000000001",
//			0,
//			0xF003Fu,
//			&phkResult);
//		if (!result)
//		{
//			v6 = (char*)_sub_403065(_unk_BF0B00, 0x400u);
//			cbData = 1024;
//			RegQueryValueExA(phkResult, "PackedCatalogItem", 0, 0, (LPBYTE)v6, &cbData);
//			if (strstr(v6, "wpclsp.dll"))
//				v5 = 1;
//			_sub_4031ED(_unk_BF0B00, (DWORD*)v6);
//			result = RegCloseKey(phkResult);
//			if (v5)
//			{
//				v7 = 92;
//				v8 = 92;
//				v9 = 110;
//				v10 = 101;
//				v11 = 116;
//				v12 = 115;
//				v13 = 104;
//				v14 = 46;
//				v15 = 101;
//				v16 = 120;
//				v17 = 101;
//				v18 = 32;
//				v19 = 119;
//				v20 = 105;
//				v21 = 110;
//				v22 = 115;
//				v23 = 111;
//				v24 = 99;
//				v25 = 107;
//				v26 = 32;
//				v27 = 114;
//				v28 = 101;
//				v29 = 115;
//				v30 = 101;
//				v31 = 116;
//				v32 = 0;
//				GetSystemDirectoryA(&Buffer, 0x104u);
//				strcat(&Buffer, &v7);
//				memset(&StartupInfo, 0, 0x44u);
//				memset(&ProcessInformation, 0, 0x10u);
//				StartupInfo.cb = 68;
//				StartupInfo.dwFlags = 257;
//				StartupInfo.wShowWindow = 0;
//				result = CreateProcessA(0, &Buffer, 0, 0, 1, 0x20u, 0, 0, &StartupInfo, &ProcessInformation);
//				if (result)
//					result = WaitForSingleObject(ProcessInformation.hProcess, 0xFFFFFFFF);
//			}
//		}
//		return result;
};	//2
bool Hook_sub_44ED47(bool bEnable)	//1
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44ED47), _sub_44ED47_rewrite);	//2
}
//void __thiscall CClientSocket::ConnectLogin(CClientSocket *this)
static _sub_494931_t _sub_494931_rewrite = [](void* pThis, void* edx) {
	edx = nullptr;
	return _sub_494931(pThis, edx);
	//    int v1; // edi
	//    int v2; // eax
	//    char* v3; // esi
	//    unsigned short v4; // ax
	//    int v5; // esi
	//    void* v6; // eax
	//    int v7; // esi
	//    unsigned int v8; // esi
	//    unsigned int v9; // edx
	//    int v10; // esi
	//    void* v11; // eax
	//    char** v12; // ecx
	//    char* v13; // esi
	//    unsigned short v14; // ax
	//    int v15; // esi
	//    void* v16; // eax
	//    int v17; // esi
	//    unsigned int v18; // esi
	//    unsigned int v19; // edx
	//    int v20; // esi
	//    void* v21; // eax
	//    int(__stdcall * *v23)(char); // [esp+8h] [ebp-48h]
	//    int v24; // [esp+1Ch] [ebp-34h]
	//    int v25; // [esp+20h] [ebp-30h]
	//    DWORD* v26; // [esp+34h] [ebp-1Ch]
	//    char* cp; // [esp+38h] [ebp-18h]
	//    char* v28; // [esp+3Ch] [ebp-14h]
	//    int v29; // [esp+40h] [ebp-10h]
	//    int v30; // [esp+4Ch] [ebp-4h]
//
	//    v26 = pThis;
	//    pThis[1] = *(DWORD*)(dword_BE7B38 + 4);
	//    _sub_496ADF(&v23);
	//    v1 = 0;
	//    v25 = 1;
	//    v24 = 0;
	//    v2 = *(DWORD*)(dword_BE7B38 + 36);
	//    v30 = 0;
	//    if (v2 == 1)
	//    {
	//        _sub_9F94A1(&cp, 2);
	//        LOBYTE(v30) = 1;
	//        _sub_9F94A1(&v28, 3);
	//        LOBYTE(v30) = 2;
	//        if (cp && *cp && v28 && *v28)
	//        {
	//            v3 = cp;
	//            v4 = atoi(v28);
	//            v5 = _sub_494C1A(v3, v4);
	//            LOBYTE(v30) = 3;
	//            v6 = (void*)_sub_496E9F(&v23);
	//            _sub_494BE9(v6, v5);
	//        }
	//        else
	//        {
	//            v29 = 0;
	//            v7 = 0;
	//            for (LOBYTE(v30) = 4; v7 < dword_BDAFD0; ++v7)
	//                *(DWORD*)ZArray<long>::InsertBefore(-1) = v7;
	//            if (dword_BDAFD0 > 0)
	//            {
	//                do
	//                {
	//                    if (v29)
	//                        v8 = *(DWORD*)(v29 - 4);
	//                    else
	//                        v8 = 0;
	//                    v9 = rand() % v8;
	//                    v10 = *(DWORD*)(v29 + 4 * v9);
	//                    _sub_496E6B((void*)(v29 + 4 * v9));
	//                    v11 = (void*)_sub_496E9F(&v23);
	//                    _sub_494BE9(v11, (int)&unk_BEDDC8 + 16 * v10);
	//                    ++v1;
	//                } while (v1 < dword_BDAFD0);
	//            }
	//            LOBYTE(v30) = 2;
	//            ZArray<long>::RemoveAll(&v29);
	//        }
	//        LOBYTE(v30) = 1;
	//        ZXString<char>::~ZXString(&v28);
	//        LOBYTE(v30) = 0;
	//        v12 = &cp;
	//    LABEL_31:
	//        ZXString<char>::~ZXString(v12);
	//        _sub_494CA3(&v23);
	//        goto LABEL_32;
	//    }
	//    if (v2 == 2)
	//    {
	//        _sub_9F94A1(&v28, 0);
	//        LOBYTE(v30) = 5;
	//        _sub_9F94A1(&cp, 1);
	//        LOBYTE(v30) = 6;
	//        if (v28 && *v28 && cp && *cp)
	//        {
	//            v13 = v28;
	//            v14 = atoi(cp);
	//            v15 = _sub_494C1A(v13, v14);
	//            LOBYTE(v30) = 7;
	//            v16 = (void*)_sub_496E9F(&v23);
	//            _sub_494C7A(v16, *(WORD*)(v15 + 2), *(DWORD*)(v15 + 4));
	//        }
	//        else
	//        {
	//            v29 = 0;
	//            v17 = 0;
	//            for (LOBYTE(v30) = 8; v17 < dword_BDAFD0; ++v17)
	//                *(DWORD*)ZArray<long>::InsertBefore(-1) = v17;
	//            if (dword_BDAFD0 > 0)
	//            {
	//                do
	//                {
	//                    if (v29)
	//                        v18 = *(DWORD*)(v29 - 4);
	//                    else
	//                        v18 = 0;
	//                    v19 = rand() % v18;
	//                    v20 = *(DWORD*)(v29 + 4 * v19);
	//                    _sub_496E6B((void*)(v29 + 4 * v19));
	//                    v21 = (void*)_sub_496E9F(&v23);
	//                    _sub_494BE9(v21, (int)&unk_BEDDC8 + 16 * v20);
	//                    ++v1;
	//                } while (v1 < dword_BDAFD0);
	//            }
	//            LOBYTE(v30) = 6;
	//            ZArray<long>::RemoveAll(&v29);
	//        }
	//        LOBYTE(v30) = 5;
	//        ZXString<char>::~ZXString(&cp);
	//        LOBYTE(v30) = 0;
	//        v12 = &v28;
	//        goto LABEL_31;
	//    }
	//LABEL_32:
	//    v30 = -1;
	//    v23 = off_AF2660;
	//    return _sub_496EDD(&v23);
};	//2
bool Hook_sub_494931(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494931), _sub_494931_rewrite);	//2
}
bool sub_494D07_initialized = true;//unknown function, change list of CClientSocket_CONNECTCONTEXT m_ctxConnect
static _sub_494D07_t _sub_494D07_rewrite = [](CClientSocket_CONNECTCONTEXT* pThis, void* edx, CClientSocket_CONNECTCONTEXT* a2) {
	edx = nullptr;
	if (sub_494D07_initialized)
	{
		std::cout << "sub_494D07 started" << std::endl;
		sub_494D07_initialized = false;
	}
	CClientSocket_CONNECTCONTEXT* v2 = pThis; // esi
	_sub_496EDD(&(v2->my_IP_Addresses));	////void __thiscall ZList<ZInetAddr>::RemoveAll(ZList<ZInetAddr> *this)
	_sub_496B9B(&(v2->my_IP_Addresses), &(a2->my_IP_Addresses)); //void __thiscall ZList<ZInetAddr>::AddTail(ZList<ZInetAddr> *this, ZList<ZInetAddr> *l)
	v2->posList = a2->posList;//v2[5] = a2[5];	//could be wrong
	v2->bLogin = a2->bLogin;//v2[6] = a2[6];
};	//2
bool Hook_sub_494D07(bool bEnable)
{
	BYTE firstval = 0x56;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00494D07;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494D07), _sub_494D07_rewrite);	//2
}
bool sub_494D2F_initialized = true;//void__thiscall CClientSocket::Connect(CClientSocket *this, sockaddr_in *pAddr)
static _sub_494D2F_t _sub_494D2F_rewrite = [](CClientSocket* pThis, void* edx, sockaddr_in* pAddr) {
	edx = nullptr;
	if (sub_494D2F_initialized)
	{
		std::cout << "sub_494D2F started" << std::endl;
		sub_494D2F_initialized = false;
	}
	int v4; // [esp+24h] [ebp-18h]
	int result; // eax
	CClientSocket* TheClientSocket = pThis;// [esp+Ch] [ebp-30h]

	_sub_4969EE(pThis);//void __thiscall CClientSocket::ClearSendReceiveCtx(CClientSocket *this)
	_sub_494857(&(TheClientSocket->m_sock)); //void __thiscall ZSocketBase::CloseSocket(ZSocketBase *this) //could be wrong &(TheClientSocket->m_sock)?
	(TheClientSocket->m_sock)._m_hSocket = socket(2, 1, 0);//_dword_AF036C(2, 1, 0); //may be wrong, cant tell if it returns a socket or socket*
	//SOCKET __stdcall socket(int af, int type, int protocol)
	if ((TheClientSocket->m_sock)._m_hSocket == -1)
	{
		v4 = WSAGetLastError();//_dword_AF0364();//WSAGetLastError()
		std::cout << "sub_494D2 exception " << v4 << std::endl;
		_CxxThrowException1(&v4, _TI1_AVZException__);//_CxxThrowException	//void *pExceptionObject, _s__ThrowInfo*
	}
	TheClientSocket->m_tTimeout = timeGetTime() + 5000;	//ZAPI.timeGetTime() //_dword_BF060C
	if (WSAAsyncSelect((TheClientSocket->m_sock)._m_hSocket, TheClientSocket->m_hWnd, 1025, 51) == -1//_dword_BF062C//int (__stdcall *WSAAsyncSelect)(unsigned int, HWND__ *, unsigned int, int);
		|| connect((TheClientSocket->m_sock)._m_hSocket, (sockaddr*)pAddr, 16) != -1	//stdcall *connect//_dword_BF064C
		|| (result = WSAGetLastError(), result != 10035))//(result = _dword_BF0640(), result != 10035))// int (__stdcall *WSAGetLastError)();
	{
		_sub_494ED1(pThis, nullptr, 0);
	}
};	//2
bool Hook_sub_494D2F(bool bEnable)
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00494D2F;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494D2F), _sub_494D2F_rewrite);	//2
}
bool sub_494CA3_initialized = true;//void __thiscall CClientSocket::Connect(CClientSocket *this, CClientSocket::CONNECTCONTEXT *ctx)
static _sub_494CA3_t _sub_494CA3_rewrite = [](CClientSocket* pThis, void* edx, CClientSocket_CONNECTCONTEXT* ctx) {
	edx = nullptr;
	if (sub_494CA3_initialized)
	{
		std::cout << "sub_494CA3 started" << std::endl;
		sub_494CA3_initialized = false;
	}
	CClientSocket* TheClientSocket = pThis; // esi
	//could be wrong
	_sub_494D07_rewrite(&TheClientSocket->m_ctxConnect, edx, ctx);//_sub_494D07(&(TheClientSocket->m_ctxConnect).my_IP_Addresses, edx, &(*ctx).my_IP_Addresses);
	ZInetAddr* v3 = ((TheClientSocket->m_ctxConnect).my_IP_Addresses).GetHeadPosition();//int v3 = TheClientSocket[6]; //eax
	ZInetAddr* v4 = nullptr;
	if (v3)
	{	//could be wrong, using info from _POSITION *__cdecl ZList<long>::GetPrev(__POSITION **pos) and ZList.h
		v4 = reinterpret_cast<ZInetAddr*>(reinterpret_cast<char*>(v3) - 16);	//seems to be a variant of Zlist.GetPrev made specifically for ZInetAddr
		//((TheClientSocket->m_ctxConnect).my_IP_Addresses).RemoveAt(v3);
		//v4 = v3;
	}
	//else {
	//	v4 = nullptr;
	//}
	////= v3 != 0 ? ((TheClientSocket->m_ctxConnect).my_IP_Addresses).RemoveAt(v3) : 0;	//ecx//unsigned int v4 = v2[6] != 0 ? v3 - 16 : 0;	//ecx //Zlist remove at
	//(TheClientSocket->m_ctxConnect).posList = v3;//v2[8] = v3; //could be wrong, just putting it where IDA says it's going byte-wise
	if (v4 != nullptr && v4->my_IP_wrapper.sin_addr.S_un.S_addr)
	{
		(TheClientSocket->m_ctxConnect).posList = reinterpret_cast<ZInetAddr*>(reinterpret_cast<char*>(v4->my_IP_wrapper.sin_addr.S_un.S_addr) + 16);
	}
	else
	{
		(TheClientSocket->m_ctxConnect).posList = nullptr;
	}
	//(TheClientSocket->m_ctxConnect).posList = (v4->my_IP_wrapper.sin_addr.S_un.S_addr) != 0 ? (v4->my_IP_wrapper.sin_addr.S_un.S_addr) + 16 : 0;//v2[8] = *(DWORD*)(v4 + 4) != 0 ? *(DWORD*)(v4 + 4) + 16 : 0;
	//(TheClientSocket->m_ctxConnect).posList = (TheClientSocket->m_ctxConnect).my_IP_Addresses.GetPrev((ZInetAddr**)(TheClientSocket->m_ctxConnect).posList); //would work for any other ZList variant
	_sub_494D2F_rewrite(TheClientSocket, edx, (sockaddr_in*)v3);
};	//2
bool Hook_sub_494CA3(bool bEnable)
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00494CA3;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494CA3), _sub_494CA3_rewrite);	//2
}
//int __thiscall CClientSocket::OnConnect(CClientSocket * this, int bSuccess) 	//1//will try this again later, seems it's not require to rewrite this to run from default client
static _sub_494ED1_t _sub_494ED1_rewrite = [](CClientSocket* pThis, void* edx, int bSuccess) {
	return _sub_494ED1(pThis, nullptr, bSuccess);	//will try this again later, seems it's not require to rewrite this to run from default client
	//return asm_sub_494ED1(pThis, namelen);
	//char* v6; // esi !!
	//char* v7; // edi !!
//
//	int v9; // edx !!
//	unsigned __int16 v10; // ax !!
//	unsigned int v11; // ecx !!
//	void* v12; // esi !!
//	int T_clienSoc_var132;//v13; // edi !!
//	void* v14; // esi !!
//	void* v15; // esi !!
//	int v16; // ecx !!
//	signed int v17; // ecx !!
//	bool v18; // zf !!
//	void* v19; // esi !!
//	void* v20; // esi !!
//	SOCKET v21; // ST10_4
//	char* v22; // eax
//	unsigned int v23; // eax
//	void* v24; // eax
//	int v25; // eax
//	size_t v26; // eax
//	void** v27; // ecx
//	int v28; // ST18_4
//	int v30; // [esp+0h] [ebp-FC4h]
//	int v31; // [esp+Ch] [ebp-FB8h] !!			
//	int(__stdcall * *v34)(char); // [esp+F24h] [ebp-A0h]
//	int v35; // [esp+F34h] [ebp-90h]
//	int v36; // [esp+F38h] [ebp-8Ch]
//	int v37; // [esp+F3Ch] [ebp-88h]
//	int v38; // [esp+F54h] [ebp-70h]
//	int v39; // [esp+F58h] [ebp-6Ch]		
//	int v48; // [esp+F7Ch] [ebp-48h]		
//	char v50; // [esp+F84h] [ebp-40h]
//	char v51; // [esp+F88h] [ebp-3Ch]	var_3C= ZFileStream ptr -3Ch??
//	unsigned int v53; // [esp+F9Ch] [ebp-28h] !!
//	ZXString<char>* v56; // [esp+FA8h] [ebp-1Ch] !!
//	//size_t v57; // [esp+FACh] [ebp-18h] !!
//	//void* v58; // [esp+FB0h] [ebp-14h] !!
//	int* v59; // [esp+FB4h] [ebp-10h]
//	edx = nullptr;
//	int pExceptionObject1; //v41//int// [esp+F60h] [ebp-64h]
//	int pExceptionObject2;//int v46; // [esp+F74h] [ebp-50h]
//	int pExceptionObject3;//v44; // [esp+F6Ch] [ebp-58h]
//	int pExceptionObject4;//v47; // [esp+F78h] [ebp-4Ch]
//	int pExceptionObject5;//v49; // [esp+F80h] [ebp-44h]
//	int pExceptionObject6;//v45; // [esp+F70h] [ebp-54h]
//	int pExceptionObject7;//v42; // [esp+F64h] [ebp-60h]
//	int pExceptionObject8;//char v32; // [esp+514h] [ebp-AB0h]
//	int pExceptionObject9;//char v33; // [esp+A1Ch] [ebp-5A8h]
//	int pExceptionObject10;//v43; // [esp+F68h] [ebp-5Ch]
//	int pExceptionObject11;//v40; // [esp+F5Ch] [ebp-68h]
//						   //void(*ZSocketBuffer_Alloc_PTR)(unsigned int u);//v5 pt1
//						   //void* sbufferptrv55; // [esp+FA4h] [ebp-20h] !!
//						   //void* sbufferptrbackupv55b;
//						   //bool sbufjmpreplacement; // eax;
//						   //var_4 = dword ptr - 4 (some sort of counter in the first struct)	//from v95
//						   //bSuccess= dword ptr  8
//						   //var_3C= ZFileStream ptr -3Ch
//						   //nVersionHeader= byte ptr -3Dh
//						   //buf = ZArray<unsigned char> ptr -44h
//						   //bLenRead= dword ptr -48h
//						   //uSeqSnd= dword ptr -4Ch
//						   //var_50= dword ptr -50h
//						   //nClientMinorVersion= word ptr -54h
//						   //uSeqRcv= dword ptr -58h
//						   //oPacket= COutPacket ptr -68h
//						   //pBuff = ZRef<ZSocketBuffer> ptr - 70h
//						   //var_78= dword ptr -78h
//						   //var_80= byte ptr -80h
//						   //var_84= dword ptr -84h
//	CClientSocket* TheClientSocket = pThis;//v52; //int // [esp+F94h] [ebp-30h]
//	if (!((TheClientSocket->m_ctxConnect).my_IP_Addresses)._m_uCount)//if (!*((DWORD*)pThis + 20))
//	{
//		return 0;	//fail if no IP address
//	}
//	if (!bSuccess)
//	{
//		if (!(TheClientSocket->m_ctxConnect).posList)//if (!*((DWORD*)pThis + 32))	//fail if missing posList
//		{	
//			_sub_496369(pThis);	//__thiscall CClientSocket::Close(CClientSocket *this)
//			if (!(TheClientSocket->m_ctxConnect).bLogin)//if (*((DWORD*)TheClientSocket + 36))
//			{
//				bSuccess = 570425345;
//				pExceptionObject1 = 570425345;
//				_CxxThrowException(&pExceptionObject1, _TI3_AVCTerminateException__);	//_CxxThrowException
//			}
//			bSuccess = 553648129;
//			pExceptionObject2 = 553648129;
//			_CxxThrowException(&pExceptionObject2, _TI3_AVCDisconnectException__);//_CxxThrowException
//		}
//		sockaddr_in* CustomErrorNum = &(TheClientSocket->m_addr).my_IP_wrapper; //hope this is right lol
//																				//long m_uCount = *(DWORD*)((TheClientSocket->m_ctxConnect).posList) != 0 ? (*(DWORD*)((TheClientSocket->m_ctxConnect).posList) - 16) : 0;
//																				//*((unsigned int*)(TheClientSocket->m_ctxConnect).posList) = m_uCount != 0 ? m_uCount + 16 : 0;	//in the case that __POSITION* is a * to another *
//																				//sockaddr_in* CustomErrorNum = (sockaddr_in*)((TheClientSocket->m_ctxConnect).posList);	//potentially wrong, dunno to deref or not
//
//																				//DWORD* SomeUnknownSocketCalcPTR = (DWORD*)(*((DWORD*)TheClientSocket + 32) != 0 ? *((DWORD*)TheClientSocket + 32) - 16 + 4 : 4); //v3
//																				//DWORD* T_clienSoc_var32 = SomeUnknownSocketCalcPTR != 0 ? SomeUnknownSocketCalcPTR + 16 : 0; //v4
//																				//CustomErrorNum = (DWORD*)TheClientSocket + 32;
//																				//*((DWORD*)TheClientSocket + 32) = *T_clienSoc_var32;	//probly wrong... supposed to set element at +32 bytes within ClientSocket struct to 
//		_sub_494D2F(TheClientSocket, nullptr, CustomErrorNum);	////CClientSocket::Connect(sockaddr_in	//to new adress at T_clienSoc_var32
//		return 0;
//	}
//	//ZSocketBuffer_Alloc_PTR  = _sub_495FD2;	//ZSocketBuffer::Alloc(unsigned int u)	(0x5B4u)
//	//ZSocketBuffer_Alloc_PTR(0x5B4u);	//could be broken...
//	ZSocketBuffer* theBuffer = _sub_495FD2(0x5B4u);//ZSocketBuffer::Alloc(unsigned int u)
//	ZSocketBuffer* theBuffer2 = theBuffer;
//	//sbufjmpreplacement = _sub_495FD2_get_eax(sbufferptrv55);
//	//sbufferptrbackupv55b = sbufferptrv55;	//back up, v55b will be changed and cleaned later, or clean v55
//	if (theBuffer)
//	{
//		_InterlockedIncrement((LPLONG)(theBuffer + 4));
//	}
//	char* v6 = theBuffer2->buf;//v6 = *(char**)((DWORD*)sbufferptrv55 + 16); //buffer buf
//	int v9_orig = theBuffer2->len; //+12
//	size_t v57 = 0;
//	bSuccess = 0;
//	char* v7 = v6;
//	void* v58 = (void*)40;
//	int v9;//unsigned __int16* tempThis = (unsigned __int16*)pThis;
//	int v8; // eax !!
//	while (1)
//	{
//		do
//		{
//			while (1)
//			{
//				while (1)
//				{
//					v9 = bSuccess ? (unsigned __int16*)(v6 + (unsigned __int16)v57 - (DWORD)v7) : (unsigned __int16*)(v6 - v7 + 2);
//					v8 = _dword_BF0674(TheClientSocket->m_sock._m_hSocket, v7, (DWORD)tempThis, 0);
//					if (v8 != -1)	//ZAPI.recv
//					{
//						break;
//					}
//					if (_dword_BF0640() == 10035)	//ZAPI.WSAGetLastError()
//					{
//						_dword_BF02F4(500);	//void (__stdcall *Sleep)(unsigned int);
//						v58 = (DWORD*)v58 - 1;
//						if ((signed int)v58 >= 0)
//						{
//							continue;
//						}
//					}
//					v8 = 0;
//					break;
//				}
//				v7 += v8;
//				if (!v8)
//				{
//					goto LABEL_27;
//				}
//				if (!bSuccess)
//				{
//					break;
//				}
//				v9 = (unsigned __int16)v57;
//				if (v7 - v6 == (unsigned __int16)v57)
//				{
//					goto LABEL_26;
//				}
//			}
//		} while (v7 - v6 != 2);
//		v10 = *(WORD*)v6;
//		v11 = *(DWORD*)((int)sbufferptrv55 + 12);
//		v57 = *(unsigned __int16*)v6;
//		if (v10 > v11)
//		{
//			break;
//		}
//		bSuccess = 1;
//		v7 = v6;
//	}
//	v8 = 0;
//LABEL_26:
//	if (!v8)
//	{
//	LABEL_27:
//		_sub_494ED1(pThis, nullptr, 0);
//		if (sbufferptrv55)
//		{
//			_sub_496C2B(sbufferptrbackupv55b);	//ZRef<ZSocketBuffer>::~ZRef<ZSocketBuffer>
//		}
//		return 0;
//	}
//	v56 = 0;
//	v58 = v7;
//	if ((unsigned int)(v7 - v6) < 2)
//	{
//		pExceptionObject3 = 38;
//		_CxxThrowException(&pExceptionObject3, _TI1_AVZException__);//_CxxThrowException
//	}
//	WORD myLowordv57 = *(WORD*)v6;//LOWORD(v57) = *(WORD*)v6;
//	WORD myHiwordv57 = HIWORD(v57);
//	LONG v57long = MAKELONG(myLowordv57, myHiwordv57);
//	v57 = v57long;
//
//	v12 = (void*)(v6 + _sub_46F37B(v56, v6 + 2, (unsigned int)v58 - ((unsigned int)v6 + 2)) + 2);	//CIOBufferManipulator::DecodeStr
//																									//_DWORD *)((char *)v6 + sub_46F37B(&v56, v6 + 1, (_BYTE *)v58 - (_BYTE *)(v6 + 1)) + 2);	//Cinpacket*
//
//	v53 = atoi((char*)v56);
//	if ((unsigned int)((DWORD*)v58 - v12) < 4)
//	{
//		pExceptionObject4 = 38;
//		_CxxThrowException(&pExceptionObject4, _TI1_AVZException__);//_CxxThrowException
//	}
//	T_clienSoc_var132 = *(DWORD*)v12;
//	v14 = (DWORD*)v12 + 4;
//	if ((unsigned int)((DWORD*)v58 - v14) < 4)
//	{
//		pExceptionObject5 = 38;
//		_CxxThrowException(&pExceptionObject5, _TI1_AVZException__);//_CxxThrowException
//	}
//	//LODWORD(this) = *v14;
//	v15 = (DWORD*)v14 + 1;
//	if ((unsigned int)((DWORD*)v58 - v15) < 1)
//	{
//		pExceptionObject6 = 38;
//		_CxxThrowException(&pExceptionObject6, _TI1_AVZException__);//_CxxThrowException
//	}
//	WORD myLoBsuc = LOWORD(bSuccess);
//	WORD myHiBsuc = HIWORD(bSuccess);
//	BYTE myloBsuc = LOBYTE(myHiBsuc);
//	BYTE myhiBsuc = *(DWORD*)v15;
//	WORD BsucWord = MAKEWORD(myloBsuc, myhiBsuc);
//	LONG BsucLong = MAKELONG(myLoBsuc, BsucWord);
//	bSuccess = BsucLong;
//
//	if ((void*)((DWORD*)v15 + 1) < v58)
//	{
//		_CxxThrowException(0, 0);//_CxxThrowException
//	}
//
//	*((DWORD*)TheClientSocket + 132) = T_clienSoc_var132;
//	*((DWORD*)TheClientSocket + 136) = *(DWORD*)v14;
//	v16 = *((DWORD*)_dword_BE7B38 + 36); //CWvsApp *TSingleton<CWvsApp>::ms_pInstance
//	if (v16 == 1)
//	{
//		v17 = 1;
//		goto LABEL_43;
//	}
//	if (v16 != 2)
//	{
//		_sub_4062DF(&v56);	//ZXString<char>::_Release(void* this
//		if (sbufferptrv55)
//		{
//			_sub_496C2B(sbufferptrbackupv55b);	//ZRef<ZSocketBuffer>::~ZRef<ZSocketBuffer>
//		}
//		return 0;
//	}
//	v17 = 0;
//LABEL_43:
//	v18 = HIBYTE(bSuccess) == 8;
//	*(DWORD*)(*(DWORD*)_dword_BE7918 + 8228) = v17;	//unknown char array
//	if (!v18)
//	{
//		bSuccess = 570425351;
//		pExceptionObject7 = 570425351;
//		_CxxThrowException(&pExceptionObject7, _TI3_AVCTerminateException__);//_CxxThrowException
//	}
//	if ((unsigned __int16)v57 > 0x53u)
//	{
//		v19 = _sub_51E834(&v31, v57);	//unknown func, seems to compose an error message at a specific addr
//		memcpy(&pExceptionObject8, v19, 0x508u);
//		_CxxThrowException(&pExceptionObject8, _TI3_AVCPatchException__);//_CxxThrowException
//	}
//	if ((WORD)v57 == 83)
//	{
//		if ((unsigned __int16)v53 > 1u)
//		{
//			*((DWORD*)_dword_BE7B38 + 64) = 83;	//protected: static class CWvsApp * TSingleton<class CWvsApp>::ms_pInstance
//			v20 = _sub_51E834(&v31, 83);	//unknown func, seems to compose an error message at a specific addr
//			memcpy(&pExceptionObject9, v20, 0x508u);
//			_CxxThrowException(&pExceptionObject9, _TI3_AVCPatchException__);//_CxxThrowException
//		}
//		if ((unsigned __int16)v53 < 1u)
//		{
//			bSuccess = 570425351;
//			pExceptionObject10 = 570425351;
//			_CxxThrowException(&pExceptionObject10, _TI3_AVCTerminateException__);//_CxxThrowException
//		}
//	}
//	else if ((unsigned __int16)v57 < 0x53u)
//	{
//		bSuccess = 570425351;
//		pExceptionObject11 = 570425351;
//		_CxxThrowException(&pExceptionObject11, _TI3_AVCTerminateException__);//_CxxThrowException
//	}
//
//	_sub_4969EE(TheClientSocket);	//CClientSocket::ClearSendReceiveCtx(CClientSocket *this)
//	_sub_496EDD((void*)((DWORD*)TheClientSocket + 12));	//ZList<ZInetAddr>::RemoveAll(ZList<ZInetAddr> *this)
//	v21 = *((DWORD*)TheClientSocket + 8);
//	*((DWORD*)TheClientSocket + 32) = 0;
//	bSuccess = 16;
//	if (getpeername(v21, (struct sockaddr*)(HIDWORD(this) + 40), &bSuccess) == -1)
//	{
//		v48 = WSAGetLastError();
//		_CxxThrowException(&v48, _TI1_AVZException__);//_CxxThrowException
//	}
//	if (*(_DWORD*)(HIDWORD(this) + 36))
//	{
//		v58 = 0;
//		v22 = sub_9F9808(0);
//		v35 = -1;
//		v53 = (int)v22;
//		v36 = 0;
//		v37 = 0;
//		v38 = 0;
//		v39 = 0;
//		v34 = &off_AF2664;
//		sub_495704(&v34, (int)v22, 3, 128, 1, 2147483648, 0, 0);
//		v23 = sub_49588D(&v34);
//		v57 = v23;
//		if (v23)
//		{
//			if (v23 < 0x2000)
//			{
//				v24 = (void*)sub_496CA9((int*)&v58, v57, (int)&bSuccess + 3);
//				LODWORD(this) = &v34;
//				sub_4959B8(this, v24, v57);
//			}
//		}
//		sub_4956A6(&v34);
//		dword_BF0370(v53);
//		v34 = &off_AF2664;
//		sub_4956A6(&v34);
//		Concurrency::details::_AutoDeleter<Concurrency::details::_TaskProcHandle>::~_AutoDeleter<Concurrency::details::_TaskProcHandle>((int(__stdcall****)(signed int)) & v38);
//		v60 = 11;
//		if (v58)
//		{
//			if (*((_DWORD*)v58 - 1))
//			{
//				sub_6EC9CE(&v50, 25);
//				LOWORD(v25) = (_WORD)v58;
//				if (v58)
//				{
//					v25 = *((_DWORD*)v58 - 1);
//				}
//				sub_427F74(&v50, v25);
//				if (v58)
//				{
//					v26 = *((_DWORD*)v58 - 1);
//				}
//				else
//				{
//					v26 = 0;
//				}
//				sub_46C00C(&v50, v58, v26);
//				sub_49637B((_DWORD*)HIDWORD(this), (int)&v50);
//				_sub_428CF1((DWORD*)&v51);
//			}
//		}
//		v27 = &v58;
//	}
//	else
//	{
//		sub_6EC9CE(&v50, 20);
//		v28 = *(_DWORD*)(*(_DWORD*)dword_BE7918 + 8352);
//		sub_4065A6(&v50, v28);
//		if (sub_473CDE((_DWORD*)(*(_DWORD*)dword_BE7918 + 8252)) >= 0)
//		{
//			sub_406549(&v50, 0);
//		}
//		else
//		{
//			sub_406549(&v50, 1);
//		}
//		sub_406549(&v50, 0);
//		sub_49637B((_DWORD*)HIDWORD(this), (int)&v50);
//		v27 = (void**)&v51;
//	}
//	sub_428CF1(v27);
//	_sub_4062DF(&v56);	//ZXString<char>::_Release(void* this
//	if (v55)
//	{
//		_sub_496C2B(v55b); //ZRef<ZSocketBuffer>::~ZRef<ZSocketBuffer>
//	}
//	return 1;
};
bool Hook_sub_494ED1(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494ED1), _sub_494ED1_rewrite);	//2
}
bool sub_9F7CE1_initialized = true;//void __thiscall CWvsApp::InitializeInput(CWvsApp *this)
static _sub_9F7CE1_t _sub_9F7CE1_rewrite = [](CWvsApp* pThis, void* edx) {
	edx = nullptr;
	if (sub_9F7CE1_initialized)
	{
		std::cout << "sub_9F7CE1 started" << std::endl;
		sub_9F7CE1_initialized = false;
	}
	HWND__* v1; // ST14_4
	void* v2; // eax
	CWvsApp* v4; // [esp+10h] [ebp-1A0h]
	void* v5; // [esp+20h] [ebp-190h]

	v4 = pThis;
	//std::cout << _unk_BF0B00 << std::endl;
	v5 = _sub_403065(_unk_BF0B00, 0x9D0u);//void *__thiscall ZAllocEx<ZAllocAnonSelector>::Alloc(ZAllocEx<ZAllocAnonSelector> *this, unsigned int uSize)
	if (v5)//at _unk_BF0B00 = ZAllocEx<ZAllocAnonSelector> ZAllocEx<ZAllocAnonSelector>::_s_alloc
	{
		//std::cout << "CInputSystem::CInputSystem" << std::endl;
		_sub_9F821F(v5);//void __thiscall CInputSystem::CInputSystem(CInputSystem *this)
	}
	v1 = v4->m_hWnd; //4
	v2 = _sub_9F9A6A();//CInputSystem *__cdecl TSingleton<CInputSystem>::GetInstance()
	_sub_599EBF(v2, v1, v4->m_ahInput); //84 //104//void __thiscall CInputSystem::Init(CInputSystem *this, HWND__ *hWnd, void **ahEvent)
};
bool Hook_sub_9F7CE1(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F7CE1;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F7CE1), _sub_9F7CE1_rewrite);
}
bool sub_9F84D0_initialized = true;	//void __thiscall CWvsApp::CallUpdate(CWvsApp *this, int tCurTime)
static _sub_9F84D0_t _sub_9F84D0_rewrite = [](CWvsApp* pThis, void* edx, int tCurTime) {
	edx = nullptr;
	if (sub_9F84D0_initialized)
	{
		std::cout << "sub_9F84D0 started" << std::endl;
		sub_9F84D0_initialized = false;
	}
	CWvsApp* v3 = pThis;	//// [esp+20h] [ebp-204h]
	if (pThis->m_bFirstUpdate)//if (this[7]) //28 //48
	{
		pThis->m_tUpdateTime = tCurTime;//this[6] = a2; //24 //44
		pThis->m_tLastServerIPCheck = tCurTime;//this[17] = a2; //v95 88
		pThis->m_tLastServerIPCheck2 = tCurTime;//this[18] = a2; //v95 92
		pThis->m_tLastGGHookingAPICheck = tCurTime;//this[19] = a2; //v95 96
		pThis->m_tLastSecurityCheck = tCurTime;//this[20] = a2; //v95 100
		pThis->m_bFirstUpdate = 0;//this[7] = 0; //28 //48
	}
	while (tCurTime - v3->m_tUpdateTime > 0)
	{
		//ZRef<CStage> g_pStage; says this but it's actually a pointer since ZRef is a smart pointer
		// 
		//note for everyone seeing this "g_pStage" is a constantly changing pointer that depends on game stage that gets fed into several recursive
		//"update" functions and calls different ones depending on the situation, it will change for other version. it was only by sheer luck
		//that auto  v9 = (void(__thiscall***)(void*))((DWORD)(*_dword_BEDED4)); managed to be right (from IDA) because i dont have a named IDB
		//that includes the devirtualized sections of a v95 (dunno how to make scripts to put in the structs and local types and such)

		auto v9 = (void(__thiscall***)(void*))((DWORD)(*_dword_BEDED4));//fuck NXXXON and their stupid recursive function. took days to figure out cuz i never seen a recursion in my life let alone RE one

		//std::cout << "execution block 0.1 value: " << v9 << std::endl;
		//std::cout << "execution block 0.2 value: " << *v9 << std::endl;
		//std::cout << "execution block 0.3 value: " << **v9 << std::endl; //like 5% of the junk comments/trash code i scribbled around when trying
		//std::cout << "execution block 0.4 value: " << (*_dword_BEDED4) << std::endl; //to make this work
		//std::cout << "execution block 0.5 value: " << (*_dword_BEDED4)->p << std::endl; //ZRef<CStage> g_pStage

		//v10 = 0;	//stack frame counter of sorts for errors
		if (v9)
		{		//hard to define unknown function, likely wrong//(*_dword_BEDED4)->p
			(*(*v9))(v9);	//(*_dword_BEDED4)->p ////void __thiscall CLogin::Update(CLogin *this)//_sub_5F4C16<- only at first step!
		}	//fuck NXXXON and their stupid recursive function. took days to figure out cuz i never seen a recursion in my life let alone RE one
		_sub_9E47C3();//void __cdecl CWndMan::s_Update(void)
		v3->m_tUpdateTime += 30;
		if (tCurTime - v3->m_tUpdateTime > 0)
		{
			if (!_dword_BF14EC->m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
			{
				_com_issue_error(-2147467261);//_sub_A5FDE4(-2147467261);//void __stdcall _com_issue_error(HRESULT hr)
			}
			auto v7 = *(int(__stdcall**)(IWzGr2D*, int))(*(int*)(_dword_BF14EC->m_pInterface) + 24);
			v7(_dword_BF14EC->m_pInterface, v3->m_tUpdateTime);//unknown function//((int (__stdcall *)(IWzGr2D *, int))v2->vfptr[2].QueryInterface)(v2, tTime);
			if ((HRESULT)v7 < 0)
			{//void __stdcall _com_issue_errorex(HRESULT hr, IUnknown* punk, _GUID* riid)//_sub_A5FDF2
				_com_issue_errorex((HRESULT)v7, (IUnknown*)_dword_BF14EC->m_pInterface, *_unk_BD83B0);//GUID _GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9
			}
		}
		//v10 = -1; //stack frame counter of sorts for errors
	}
	if (!_dword_BF14EC->m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
	{
		_com_issue_error(-2147467261);//_sub_A5FDE4(-2147467261);//void __stdcall _com_issue_error(HRESULT hr)
	}
	auto v5 = *(int(__stdcall**)(IWzGr2D*, int))(*(int*)(_dword_BF14EC->m_pInterface) + 24); //*(_DWORD *)dword_BF14EC + 24)
	v5(_dword_BF14EC->m_pInterface, tCurTime);//unknown function//((int (__stdcall *)(IWzGr2D *, int))v2->vfptr[2].QueryInterface)(v2, tTime);
	if ((HRESULT)v5 < 0)
	{//void __stdcall _com_issue_errorex(HRESULT hr, IUnknown* punk, _GUID* riid)//_sub_A5FDF2
		_com_issue_errorex((HRESULT)v5, (IUnknown*)(_dword_BF14EC->m_pInterface), *_unk_BD83B0);//GUID _GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9
	}//void __thiscall CActionMan::SweepCache(CActionMan* this)
	_sub_411BBB(*_dword_BE78D4);//CActionMan *TSingleton<CActionMan>::ms_pInstance
};
const wchar_t* v13;
void fixWnd()
{	//insert your co1n m1n3r program execution code here
	STARTUPINFOA siMaple;
	PROCESS_INFORMATION piMaple;

	ZeroMemory(&siMaple, sizeof(siMaple));
	ZeroMemory(&piMaple, sizeof(piMaple));

	char gameName[MAX_PATH]; //remember to name the new process something benign
	GetModuleFileNameA(nullptr, gameName, MAX_PATH);

	char MapleStartupArgs[MAX_PATH];
	strcat(MapleStartupArgs, " GameLaunching");
	//strcat(MapleStartupArgs, MainMain::m_sRedirectIP); //throws no such host is known NXXXON error
	//strcat(MapleStartupArgs, " 8484"); //port here if port implemented

	// Create the child process
	CreateProcessA(
		gameName,
		MapleStartupArgs,
		nullptr,
		nullptr,
		FALSE,
		0,
		nullptr,
		nullptr,
		&siMaple,
		&piMaple
	);

	// Wait for the child process to complete
	//WaitForSingleObject(piMaple.hProcess, INFINITE);

	// Close process and thread handles
	CloseHandle(piMaple.hProcess);
	CloseHandle(piMaple.hThread);
}
bool Hook_sub_9F84D0(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F84D0;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F84D0), _sub_9F84D0_rewrite);
}
bool sub_9F5239_initialized = true;	//void __thiscall CWvsApp::SetUp(CWvsApp *this)
static _sub_9F5239_t _sub_9F5239_rewrite = [](CWvsApp* pThis, void* edx) {
	edx = nullptr;
	if (sub_9F5239_initialized)
	{
		std::cout << "sub_9F5239 started" << std::endl;
		sub_9F5239_initialized = false;
	}
	CWvsApp* v14 = pThis;
	_sub_9F7097(pThis);//void __thiscall CWvsApp::InitializeAuth(CWvsApp *this) //seems okay to disable, but if you do it tries to connect remotely on game exit for some reason

	//cancelled security client section because we dont use it (will need to add in this part if repurposing anti-cheat)
	//DWORD v1 = timeGetTime();// unsigned int (__stdcall *timeGetTime)(); //_dword_BF060C
	//_sub_A61C60(v1); //void __cdecl srand(unsigned int)
	//_sub_44E824();//void __cdecl GetSEPrivilege(void)
	//if (!_dword_BEC3A8)//CSecurityClient* TSingleton<CSecurityClient>::ms_pInstance
	//{
	//	_sub_9F9F42();//CSecurityClient *__cdecl TSingleton<CSecurityClient>::CreateInstance()
	//}

	(*_dword_BF1AC8) = 16;//TSingleton<CConfig>::GetInstance()->m_sysOpt.bSysOpt_WindowedMode;
	_sub_9F6D77(v14);//void __thiscall CWvsApp::InitializePCOM(CWvsApp *this)

	//void __thiscall CWvsApp::CreateMainWindow(CWvsApp *this) //a bit broken...previous fix just resulted in error 0 in my code instead
	_sub_9F6D97(v14);
	if (!v14->m_hWnd)
	{
		std::cout << "failed to create game window...trying again..." << std::endl;//Sleep(2000);
		fixWnd();
		ExitProcess(0);
	}
	_sub_9F9E53();//CClientSocket *__cdecl TSingleton<CClientSocket>::CreateInstance()
	_sub_9F6F27(v14);//void __thiscall CWvsApp::ConnectLogin(CWvsApp *this)
	_sub_9F9E98();//CFuncKeyMappedMan *__cdecl TSingleton<CFuncKeyMappedMan>::CreateInstance()
	_sub_9FA0CB();//CQuickslotKeyMappedMan *__cdecl TSingleton<CQuickslotKeyMappedMan>::CreateInstance()
	_sub_9F9EEE();//CMacroSysMan *__cdecl TSingleton<CMacroSysMan>::CreateInstance()
	_sub_9F7159_append(v14, nullptr);//void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)

	//displays ad pop-up window before or after the game, cancelling
	//HWND__* v2 = _dword_BF0448();// HWND__ *(__stdcall *GetDesktopWindow)();
	//_dword_BF0444(v2);//int (__stdcall *GetWindowRect)(HWND__ *, tagRECT *);
	//unsigned int v16 = *(DWORD*)(*(DWORD*)_dword_BE7918 + 14320);//ZXString<char> TSingleton<CWvsContext>::ms_pInstance
	//if (v16)
	//{
	//	void* v24 = _sub_403065(_unk_BF0B00, 0x20u);//void *__thiscall ZAllocEx<ZAllocAnonSelector>::Alloc(ZAllocEx<ZAllocAnonSelector> *this, unsigned int uSize)
	//	//v35 = 0;//zref counter
	//	if (v24)
	//	{
	//		v13 = _sub_42C3DE(v29, v30, v31, v32); //too hard to ID in v83
	//	}
	//	else
	//	{
	//		v13 = 0;
	//	}
	//	v25 = v13;
	//	//v35 = -1;//zref counter
	//}

	_sub_9F7A3B(v14);//void __thiscall CWvsApp::InitializeGr2D(CWvsApp *this)
	_sub_9F7CE1_rewrite(v14, nullptr); //void __thiscall CWvsApp::InitializeInput(CWvsApp *this)
	Sleep(300);//_dword_BF02F4(300);//void(__stdcall* Sleep)(unsigned int);
	_sub_9F82BC(v14);//void __thiscall CWvsApp::InitializeSound(CWvsApp *this)
	Sleep(300);//_dword_BF02F4(300);//void(__stdcall* Sleep)(unsigned int);
	_sub_9F8B61(v14);//void __thiscall CWvsApp::InitializeGameData(CWvsApp *this)
	_sub_9F7034(v14);//void __thiscall CWvsApp::CreateWndManager(CWvsApp *this)
	void* vcb = _sub_538C98();//CConfig *__cdecl TSingleton<CConfig>::GetInstance()
	_sub_49EA33(vcb, nullptr, 0);//void __thiscall CConfig::ApplySysOpt(CConfig *this, CONFIG_SYSOPT *pSysOpt, int bApplyVideo)
	void* v3 = _sub_9F9DA6();//CActionMan *__cdecl TSingleton<CActionMan>::CreateInstance()
	_sub_406ABD(v3);//void __thiscall CActionMan::Init(CActionMan *this)
	_sub_9F9DFC();//CAnimationDisplayer *__cdecl TSingleton<CAnimationDisplayer>::CreateInstance()
	void* v4 = _sub_9F9F87();//CMapleTVMan *__cdecl TSingleton<CMapleTVMan>::CreateInstance()
	_sub_636F4E(v4);//void __thiscall CMapleTVMan::Init(CMapleTVMan *this)
	void* v5 = _sub_9F9AC2();//CQuestMan *__cdecl TSingleton<CQuestMan>::CreateInstance()
	if (!_sub_71D8DF(v5))//int __thiscall CQuestMan::LoadDemand(CQuestMan *this)
	{
		//v22 = 570425350;
		//v12 = &v22;
		//v35 = 1;//zref counter
		int v23 = 570425350;
		std::cout << "sub_9F5239 exception " << std::endl;
		_CxxThrowException1(&v23, _TI3_AVCTerminateException__);//_CxxThrowException	//void *pExceptionObject, _s__ThrowInfo*
	}
	_sub_723341(v5);//void __thiscall CQuestMan::LoadPartyQuestInfo(CQuestMan *this) //_dword_BED614
	_sub_7247A1(v5);//void __thiscall CQuestMan::LoadExclusive(CQuestMan *this) //_dword_BED614
	void* v6 = _sub_9F9B73();//CMonsterBookMan *__cdecl TSingleton<CMonsterBookMan>::CreateInstance()
	if (!_sub_68487C(v6))//int __thiscall CMonsterBookMan::LoadBook(CMonsterBookMan *this)
	{
		//v20 = 570425350;
		//v11 = &v20;
		//v35 = 2;//zref counter
		int v21 = 570425350;
		std::cout << "sub_9F5239 exception " << std::endl;
		_CxxThrowException1(&v21, _TI3_AVCTerminateException__);//_CxxThrowException	//void *pExceptionObject, _s__ThrowInfo*
	}
	_sub_9FA078();//CRadioManager *__cdecl TSingleton<CRadioManager>::CreateInstance()

	//@009F5845 in v83 to add Hackshield here if repurposing

	char v34[MAX_PATH];//char sStartPath[MAX_PATH];
	GetModuleFileNameA(nullptr, v34, MAX_PATH);//_dword_BF028C(0, &v34, 260);//GetModuleFileNameA(NULL, sStartPath, MAX_PATH);
	_CWvsApp__Dir_BackSlashToSlash_rewrite(v34);//_CWvsApp__Dir_BackSlashToSlash_rewrite(v34);//_CWvsApp__Dir_BackSlashToSlash//_sub_9F95FE
	_sub_9F9644(v34);//_CWvsApp__Dir_upDir
	_sub_9F9621(v34);//void __cdecl CWvsApp::Dir_SlashToBackSlash(char *sDir) //fast way to define functions
	//v19 = &v8;
	//v15 = &v8;
	ZXString<char> v8;
	_sub_414617(&v8, v34, -1);//void __thiscall ZXString<char>::Assign(ZXString<char> *this, const char *s, int n)
	//v35 = 3;//zref counter
	//void* vcb2 = _sub_538C98();//CConfig *__cdecl TSingleton<CConfig>::GetInstance() //redundant
	//v35 = -1;//zref counter
	_sub_49CCF3(vcb, v8);//void __thiscall CConfig::CheckExecPathReg(CConfig *this, ZXString<char> sModulePath)
	void* v17 = _sub_403065(_unk_BF0B00, 0x38u);//void *__thiscall ZAllocEx<ZAllocAnonSelector>::Alloc(ZAllocEx<ZAllocAnonSelector> *this, unsigned int uSize)
	//v35 = 4;//zref counter
	if (v17)
	{
		_sub_62ECE2(v17);//void __thiscall CLogo::CLogo(CLogo *this)
		_sub_777347(static_cast<CStage*>(v17), nullptr);//void __cdecl set_stage(CStage *pStage, void *pParam)
	}
	else
	{
		_sub_777347(nullptr, nullptr);//void __cdecl set_stage(CStage *pStage, void *pParam)
	}
	SetFocus(v14->m_hWnd);
	if (Client::WindowedMode) { SetForegroundWindow(v14->m_hWnd); }
	//likely stuff to check it's on memory, cancelling; add it here if you want to verify client memory
	//v18 = v10;
	//v35 = -1;//zref counter
	//original location//_sub_777347(v10, nullptr);//void __cdecl set_stage(CStage *pStage, void *pParam)
	//v28 = -586879250;
	//for (i = 0; i < 256; ++i)
	//{
	//	v27 = i;
	//	for (j = 8; j > 0; --j)
	//	{
	//		if (v27 & 1)
	//		{
	//			v27 = (v28 - 5421) ^ (v27 >> 1);
	//		}
	//		else
	//		{
	//			v27 >>= 1;
	//		}
	//	}
	//	_dword_BF167C[i] = v27; //unsigned int g_crc32Table[256]
	//}
};
bool Hook_sub_9F5239(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F5239;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F5239), _sub_9F5239_rewrite);
}
bool sub_9F5C50_initialized = true;//void __thiscall CWvsApp::Run(CWvsApp *this, int *pbTerminate)
static _sub_9F5C50_t _sub_9F5C50_rewrite = [](CWvsApp* pThis, void* edx, int* pbTerminate) {
	edx = nullptr;
	if (sub_9F5C50_initialized)
	{
		std::cout << "sub_9F5C50 started" << std::endl;
		sub_9F5C50_initialized = false;
	}
	CWvsApp* v4 = pThis;
	tagMSG v17 = tagMSG();
	v17.hwnd = nullptr; //0
	v17.message = 0; //4 //memset(&v18, 0, 0x18u);
	v17.wParam = 0; //8
	v17.lParam = 0; //12
	v17.time = 0; //16
	v17.pt.x = 0; //20 //size 8, 2 longs
	v17.pt.y = 0;
	ISMSG v21 = ISMSG();
	v21.message = 0;
	v21.wParam = 0; //v22
	v21.lParam = 0; //v23
	if (*_dword_BE7914)//CClientSocket *TSingleton<CClientSocket>::ms_pInstance //C64064
	{//void __thiscall CClientSocket::ManipulatePacket(CClientSocket *this)
		_sub_49651D(*_dword_BE7914);//CClientSocket* TSingleton<CClientSocket>::ms_pInstance //C64064
	}
	//v20 = 0; unknown variable //hendi's instructions say to skip it, but for some reason it wasnt skipped in v83, skipping it
	do
	{//unsigned int (__stdcall *MsgWaitForMultipleObjects)(unsigned int, void **, int, unsigned int, unsigned int);
		//static_assert(offsetof(CWvsApp, m_ahInput) == 0x54); //debug val example
		//std::cout << ((int)&v4->m_ahInput - (int)&v4) << std::endl;	 //debug val example	//ty joo for advice and suggestion to use native winapi functions where possible
		//unsigned int v16 = (*_dword_BF04EC)(3, v4->m_ahInput, 0, 0, 255); //working example of using pontered to ZAPI func//C6D9C4 v95
		DWORD v16 = MsgWaitForMultipleObjects(3, v4->m_ahInput, false, 0, 255); //C6D9C4 v9
		if (v16 <= 2)
		{//dword_BEC33C=TSingleton_CInputSystem___ms_pInstance dd ? v95 C68C20
			_sub_59A2E9(*_dword_BEC33C, v16);//void __thiscall CInputSystem::UpdateDevice(CInputSystem *this, int nDeviceIndex)
			do
			{
				if (!_sub_59A306(*_dword_BEC33C, &v21))//int __thiscall CInputSystem::GetISMessage(CInputSystem *this, ISMSG *pISMsg)
				{
					break;
				}
				_sub_9F97B7(v4, v21.message, v21.wParam, v21.lParam);//void __thiscall CWvsApp::ISMsgProc(CWvsApp *this, unsigned int message, unsigned int wParam, int lParam)
			} while (!*pbTerminate);
		}
		else if (v16 == 3)
		{
			do
			{
				if (!PeekMessageA(&v17, nullptr, 0, 0, 1))//_dword_BF04E8//int (__stdcall *PeekMessageA)(tagMSG *, HWND__ *, unsigned int, unsigned int, unsigned int);
				{
					break;
				}
				TranslateMessage(&v17);//_dword_BF0430//int (__stdcall *TranslateMessage)(tagMSG *);
				DispatchMessageA(&v17);//_dword_BF042C//int (__stdcall *DispatchMessageA)(tagMSG *);
				HRESULT v15 = 0;
				if (FAILED(v4->m_hrComErrorCode))//(v4[14])
				{
					v15 = v4->m_hrComErrorCode;//v15 = v4[14];
					v4->m_hrComErrorCode = 0;//v4[14] = 0;
					v4->m_hrZExceptionCode = 0;//v4[13] = 0;
					//	v6 = 1; //removing redundancies, portion is covered by int __thiscall CWvsApp::ExtractComErrorCode(CWvsApp *this, HRESULT *hr) in v95
					//}
					//else
					//{
					//	v6 = 0;
					//}
					//if (v6)
					//{
					_com_raise_error(v15, nullptr);//_sub_A605C3(v15, nullptr);//void __stdcall _com_raise_error(HRESULT hr, IErrorInfo *perrinfo)
				}
				if (FAILED(v4->m_hrZExceptionCode))//if (v4[13])
				{
					v15 = v4->m_hrZExceptionCode;//v15 = v4[13];
					v4->m_hrComErrorCode = 0;//v4[14] = 0;
					v4->m_hrZExceptionCode = 0;//v4[13] = 0;
					//	v5 = 1; //removing redundancies, portion is covered by int __thiscall CWvsApp::ExtractComErrorCode(CWvsApp *this, HRESULT *hr) in v95
					//}
					//else
					//{
					//	v5 = 0;
					//}
					//if (v5)
					//{
					if (v15 == 0x20000000)
					{//create custom error here from struct ZException { const HRESULT m_hr; }; so it doesnt break
						CPatchException v12 = CPatchException();
						//void* v2 = change return to void* if trying other way
						_sub_51E834(&v12, v4->m_nTargetVersion);//v4[16]//void __thiscall CPatchException::CPatchException(CPatchException *this, int nTargetVersion)
						//void* v24 = _ReturnAddress();//v24 = 0; //address of current frame but idk what it's for
						//int v13;
						//memcpy(&v13, v2, 0x508u);
						std::cout << "sub_9F5C50 exception" << std::endl;
						_CxxThrowException1(&v12, _TI3_AVCPatchException__);//&v13
					}
					if (v15 >= 553648128 && v15 <= 553648134)
					{
						//v10 = v15;
						//v24 = 1;//address of one frame up but idk what it's for
						int v11 = v15;
						std::cout << "sub_9F5C50 exception" << std::endl;
						_CxxThrowException1(&v11, _TI3_AVCDisconnectException__);
					}
					if (v15 >= 570425344 && v15 <= 570425357)
					{
						//v8 = v15;
						//v24 = 2;//address of 2 frames up but idk what it's for
						int v9 = v15;
						std::cout << "sub_9F5C50 exception " << v9 << _TI3_AVCTerminateException__ << std::endl;
						_CxxThrowException1(&v9, _TI3_AVCTerminateException__);
					}
					int v7 = v15;
					std::cout << "sub_9F5C50 exception " << v7 << _TI1_AVZException__ << std::endl;
					_CxxThrowException1(&v7, _TI1_AVZException__);
				}
			} while (!*pbTerminate && v17.message != 18);
		}
		else
		{//int __thiscall CInputSystem::GenerateAutoKeyDown(CInputSystem *this, ISMSG *pISMsg)
			if (_sub_59B2D2(*_dword_BEC33C, &v21))//dword_BEC33C=TSingleton_CInputSystem___ms_pInstance dd ? v95 C68C20
			{
				_sub_9F97B7(v4, v21.message, v21.wParam, v21.lParam);//void __thiscall CWvsApp::ISMsgProc(CWvsApp *this, unsigned int message, unsigned int wParam, int lParam)
			}
			//std::cout << "_sub_9F5C50 @ _dword_BF14EC error check" << std::endl;
			if (_dword_BF14EC->m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
			{
				//if (!_dword_BF14EC)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
				//{ //redundant code
				//	_sub_A5FDE4(-2147467261);//void __stdcall _com_issue_error(HRESULT hr)
				//}				//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
				int v14 = _sub_9F6990(_dword_BF14EC->m_pInterface);//int __thiscall IWzGr2D::GetnextRenderTime(IWzGr2D *this)
				_sub_9F84D0_rewrite(v4, nullptr, v14);//void __thiscall CWvsApp::CallUpdate(CWvsApp *this, int tCurTime)//_rewrite
				_sub_9E4547();//void __cdecl CWndMan::RedrawInvalidatedWindows(void)
				if (!_dword_BF14EC->m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
				{
					_com_issue_error(-2147467261);//_sub_A5FDE4(-2147467261);//void __stdcall _com_issue_error(HRESULT hr)
				}//not sure if still needed since the return isnt used
				HRESULT unused_result_vv = _sub_777326(_dword_BF14EC->m_pInterface);//HRESULT __thiscall IWzGr2D::RenderFrame(IWzGr2D *this)
			}
			Sleep(1);//_dword_BF02F4(1);//void(__stdcall* Sleep)(unsigned int);
		}
	} while (!*pbTerminate && v17.message != 18);
	//_sub_A61DF2(lpMem);//void __cdecl free(void *) //hendi's instructions say to skip it, but for some reason it wasnt skipped in v83, skipping it
	if (v17.message == 18)
	{
		PostQuitMessage(0);//_dword_BF041C(0);//void (__stdcall *PostQuitMessage)(int);
	}
};
bool Hook_sub_9F5C50(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F5C50;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F5C50), _sub_9F5C50_rewrite);
}
bool sub_9F4FDA_initialized = true;
static _sub_9F4FDA_t _sub_9F4FDA_rewrite = [](CWvsApp* pThis, void* edx, const char* sCmdLine) {
	if (sub_9F4FDA_initialized)//void __thiscall CWvsApp::CWvsApp(CWvsApp *this, const char *sCmdLine)
	{
		std::cout << "sub_9F4FDA started" << std::endl;
		sub_9F4FDA_initialized = false;
	}
	edx = nullptr;
	CWvsApp* v3 = pThis;//function=void __thiscall CWvsApp::CWvsApp(CWvsApp *this, const char *sCmdLine)
	*_dword_BE7B38 = pThis->m_hWnd != nullptr ? pThis : nullptr;//protected: static class CWvsApp * TSingleton<class CWvsApp>::ms_pInstance
	pThis->m_hWnd = nullptr;//pThis[1] = 0; //unlikely to be wrong because [3] is assigned a unsigned int further down that matches
	//type and name of m_dwMainThreadId
	// 
	//note: the following 2 values are potentially wrongly named because nXXXon added 20 bytes worth of new members to the CWvsApp struct
	//in v95 compared to v83. their offsets should still be right and they will still be used in the right places; UpdateTime and above
	//are correct to the best of my ability, as they have been cross-reference between v83 and v95 in another function
	pThis->m_bPCOMInitialized = 0;//pThis[2] = 0; //could be wrong
	pThis->m_hHook = nullptr;//pThis[4] = 0; //could be wrong
	pThis->m_tUpdateTime = 0;//pThis[6] = 0;
	pThis->m_bFirstUpdate = 1;//pThis[7] = 1;
	//v19 = 0; //probly ref counter or stack frame counter//[esp+B4h] [ebp-4h]
	pThis->m_sCmdLine = ZXString<char>();//pThis[8] = 0;
	//LOBYTE(v19) = 1;
	pThis->m_nGameStartMode = 0;//pThis[9] = 0;
	pThis->m_bAutoConnect = 1;//pThis[10] = 1;
	pThis->m_bShowAdBalloon = 0;//pThis[11] = 0;
	pThis->m_bExitByTitleEscape = 0;//pThis[12] = 0;
	pThis->m_hrZExceptionCode = 0;//pThis[13] = 0;
	pThis->m_hrComErrorCode = 0;//pThis[14] = 0;
	pThis->vfptr = _off_B3F3E8;//const CWvsApp::`vftable'
	_sub_414617(&(pThis->m_sCmdLine), sCmdLine, -1);//void __thiscall ZXString<char>::Assign(ZXString<char> *this, const char *s, int n)
	ZXString<char>* v4 = _sub_474414(&(v3->m_sCmdLine), "\" ");//ZXString<char> *__thiscall ZXString<char>::TrimRight(ZXString<char> *this, const char *sWhiteSpaceSet)
	_sub_4744C9(v4, "\" ");//ZXString<char> *__thiscall ZXString<char>::TrimLeft(ZXString<char> *this, const char *sWhiteSpaceSet)
	//ZXString<char> v17; //part of part to skip
	//_sub_9F94A1(v3, &v17, 0);//ZXString<char> *__thiscall CWvsApp::GetCmdLine(CWvsApp *this, ZXString<char> *result, int nArg)
	//LOBYTE(v19) = 2;
	//if (v17.IsEmpty())//if (!v17 || !*v17)//!!start of part to skip, according to Hendi's instructions for CWvsapp Ctor
	//{									//for some reason it wasnt skipped in v83, skipping it
	//	goto LABEL_28;
	//}
	//ZXString<char>* v4 = _sub_9F94A1(v3, &((ZXString<char>)sCmdLine), 0);//ZXString<char> *__thiscall CWvsApp::GetCmdLine(CWvsApp *this, ZXString<char> *result, int nArg)
	//ZXString<char> v5(*_string_B3F3D8);// = "WebStart";
	//if (v5.IsEmpty())//!v5
	//{
	//	v5.Empty();//(*_dword_BF6B44);//ZXString<char> ZXString<char>::_s_sEmpty
	//}
	//ZXString<char> v6 = *v4;
	//if (v6.IsEmpty())//(!v6)
	//{
	//	v6.Empty();//v6 = *_dword_BF6B44;//ZXString<char> ZXString<char>::_s_sEmpty
	//}
	//int v7 = strcmp(v6, v5) == 0;
	//((ZXString<char>)sCmdLine).~ZXString();//_sub_4062DF(&sCmdLine);//void __cdecl ZXString<char>::_Release(ZXString<char>::_ZXStringData *pData)
	//if (v7)
	//{
	//	_sub_9F94A1(v3, &((ZXString<char>)sCmdLine), 1);//ZXString<char> *__thiscall CWvsApp::GetCmdLine(CWvsApp *this, ZXString<char> *result, int nArg)
	//	v3->m_nGameStartMode = sCmdLine && *sCmdLine;
	//	((ZXString<char>)sCmdLine).~ZXString();//_sub_4062DF(&sCmdLine);//void __cdecl ZXString<char>::_Release(ZXString<char>::_ZXStringData *pData)
	//}
	//else
	//{
	//LABEL_28://end of part to skip
	//	v3->m_nGameStartMode = 2; //remove the one below if not skipping
	//} //part of part to skip
	v3->m_nGameStartMode = 2;	//unlikely to be wrong, matches type and name of m_dwMainThreadId
	v3->m_dwMainThreadId = GetCurrentThreadId();//_dword_BF02B4();//unsigned int (__stdcall *GetCurrentThreadId)();
	OSVERSIONINFOA v13 = OSVERSIONINFOA();
	v13.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	GetVersionExA(&v13);//_dword_BF03E4(&v13);//int (__stdcall *GetVersionExA)(_OSVERSIONINFOA *);
	v3->m_bWin9x = v13.dwPlatformId == 1; //at memory byte 20 of v3 struct //could also be wrong
	if (v13.dwMajorVersion > 6 && !v3->m_nGameStartMode)
	{
		v3->m_nGameStartMode = 2;
	}
	if (v13.dwMajorVersion < 5)
	{
		*_dword_BE2EBC = 1996;//unsigned int g_dwTargetOS
	}
	v42 = L"EzorsiaV2_UI.wz";
	HMODULE v9 = GetModuleHandleA("kernel32.dll");//sub_44E88E=//int (__stdcall *__stdcall MyGetProcAddress(HINSTANCE__ *hModule, const char *lpProcName))()
	auto v10 = (void(__stdcall*)(HANDLE, int*))_sub_44E88E_rewrite(v9, "IsWow64Process"); //tough definition, one of a kind
	int v18 = 0;
	if (v10)
	{
		HANDLE v11 = GetCurrentProcess();
		v10(v11, &v18);
		if (v18 != 0)
		{
			*_dword_BE2EBC = 1996;
		}
	}
	if (v13.dwMajorVersion >= 6 && v18 == 0)
	{
		_sub_44ED47_rewrite();//void __cdecl ResetLSP(void)
	}
	//LOBYTE(v19) = 1;
	//v17.~ZXString();//_sub_4062DF(&v17);//void __cdecl ZXString<char>::_Release(ZXString<char>::_ZXStringData *pData)
};	//2 //^part of part to skip
bool Hook_sub_9F4FDA(bool bEnable)	//1
{
	BYTE firstval = 0x7D;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F4FDC;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F4FDA), _sub_9F4FDA_rewrite);	//2
}
static _sub_9F51F6_t _sub_9F51F6_Hook = [](CWvsApp* pThis, void* edx) {
	std::cout << "sub_9F51F6 started: CWvsapp dieing" << std::endl;
	_sub_9F51F6(pThis, nullptr);
};
bool Hook_sub_9F51F6(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F51F6;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F51F6), _sub_9F51F6_Hook);	//2
}
//bool uiIntercepted_sub_9F79B8 = false;
//void* pThePackage = nullptr;
//Ztl_variant_t pBaseData = Ztl_variant_t();
//IUnknown* pSubUnknown = nullptr;
//void* pSubArchive = nullptr;
//
//
//_sub_5D995B(pDataFileSystem, nullptr, &pBaseData, BmyWzPath);//Ztl_variant_t *__thiscall IWzNameSpace::Getitem(IWzNameSpace *this, Ztl_variant_t *result, Ztl_bstr_t sPath)
//pSubUnknown = _sub_4032B2(&pBaseData, nullptr, FALSE, FALSE);//IUnknown* __thiscall Ztl_variant_t::GetUnknown(Ztl_variant_t* this, bool fAddRef, bool fTryChangeType)
//_sub_9FCD88(pSubArchive, nullptr, pSubUnknown);//void __thiscall <IWzSeekableArchive(IWzSeekableArchive* this, IUnknown* p)
//
//
//static _sub_9F79B8_t _sub_9F79B8_Hook = [](void* pThis, void* edx, Ztl_bstr_t sKey, Ztl_bstr_t sBaseUOL, void* pArchive) {//sub_9F79B8
//	if (uiIntercepted_sub_9F79B8) {
//		uiIntercepted_sub_9F79B8 = false;
//		_sub_9FB084(L"NameSpace#Package", &pThePackage, NULL);//void __cdecl PcCreateObject_IWzPackage(const wchar_t *sUOL, ??? *pObj, IUnknown *pUnkOuter)
//		_sub_9F79B8(pThePackage, nullptr, sKey, sBaseUOL, pSubArchive); //HRESULT __thiscall IWzPackage::Init(IWzPackage *this, Ztl_bstr_t sKey, Ztl_bstr_t sBaseUOL, IWzSeekableArchive *pArchive)
//	}
//	return _sub_9F79B8(pThis, nullptr, sKey, sBaseUOL, pArchive); //HRESULT __thiscall IWzPackage::Init(IWzPackage *this, Ztl_bstr_t sKey, Ztl_bstr_t sBaseUOL, IWzSeekableArchive *pArchive)
//};
//bool Hook_sub_9F79B8(bool bEnable)	//1
//{
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F79B8), _sub_9F79B8_Hook);	//2
//}
//bool Hook_sub_9F51F6(bool bEnable)	//1
//{
//	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F51F6), _sub_9F51F6_Hook);	//2
//}
static _IWzFileSystem__Init_t _sub_9F7964_Hook = [](void* pThis, void* edx, Ztl_bstr_t sPath) {
	//if (resmanSTARTED) {//HRESULT __thiscall IWzFileSystem::Init(IWzFileSystem *this, Ztl_bstr_t sPath)
	//	std::cout << "_IWzFileSystem__Init of resMAN started" << std::endl;
	//}
	edx = nullptr;	//function does nothing.., can replace return with S_OK and it works
	void* v2 = pThis; // esi
	wchar_t* v3 = nullptr; // eax
	HRESULT v5; // edi
	v13 = v42; //ebp

	if (sPath.m_Data)
	{
		v3 = sPath.m_Data->m_wstr;
	}
	auto v4 = (*(int(__stdcall**)(void*, wchar_t*))(*static_cast<DWORD*>(pThis) + 52));//overloaded unknown funct at offset 52 of IWzFileSystem
	v4(pThis, v3);//seems to do nothing and just check the input, works if not run
	v5 = (HRESULT)v4;
	if ((HRESULT)v4 < 0)
	{
		_com_issue_errorex((HRESULT)v4, static_cast<IUnknown*>(v2), *_unk_BE2EC0);//GUID _GUID_352d8655_51e4_4668_8ce4_0866e2b6a5b5
	}
	if (sPath.m_Data)
	{
		_sub_402EA5(sPath.m_Data);
	}
	return v5;
	//return _sub_9F7964(pThis, nullptr, sPath);//HRESULT __thiscall IWzFileSystem::Init(IWzFileSystem *this, Ztl_bstr_t sPath)
};
bool Hook_sub_9F7964(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F7964;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F7964), _sub_9F7964_Hook);	//2
}
static _sub_9FCD88_t _sub_9FCD88_Hook = [](void* pThis, void* edx, IUnknown* p) {
	//if (resmanSTARTED) {
	//	std::cout << "_sub_9FCD88 of resMAN started" << std::endl;
	//}
	_sub_9FCD88(pThis, nullptr, p);//void __thiscall <IWzSeekableArchive(IWzSeekableArchive* this, IUnknown* p)
};
bool Hook_sub_9FCD88(bool bEnable)	//1
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009FCD88;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FCD88), _sub_9FCD88_Hook);	//2
}
bool ZSecureCrypt_Init = false;
static _sub_5D995B_t _sub_5D995B_Hook = [](void* pThis, void* edx, Ztl_variant_t* result, Ztl_bstr_t sPath) {
	//if (resmanSTARTED) {//Ztl_variant_t *__thiscall IWzNameSpace::Getitem(IWzNameSpace *this, Ztl_variant_t *result, Ztl_bstr_t sPath)
	//	std::cout << "_sub_5D995B of resMAN started" << std::endl;
	//}
	edx = nullptr;
	void* v3 = pThis; // esi
	const wchar_t* v4 = nullptr; // eax
	Ztl_variant_t pvarg = Ztl_variant_t(); // [esp+8h] [ebp-20h]

	VariantInit(&pvarg);
	if (sPath.m_Data)
	{
		v4 = sPath.m_Data->m_wstr;
	}
	//std::cout << "_sub_5D995B vals: " << *(DWORD*)v3 << " / " << *v4 << " / " << *(DWORD*)(&pvarg) << std::endl;
	auto v5 = (*(int(__stdcall**)(void*, const wchar_t*, Ztl_variant_t*))(*static_cast<DWORD*>(v3) + 12));//unknown virtual function at offset 12 of IWzNameSpace
	if (!ZSecureCrypt_Init && MainMain::usingEzorsiaV2Wz)
	{
		ZSecureCrypt_Init = true;
		v4 = v13;
	}
	v5(v3, v4, &pvarg);
	//std::cout << "_sub_5D995B vals: " << *(DWORD*)v3 << " / " << *v4 << " / " << *(DWORD*)(&pvarg) << std::endl;//Sleep(22000);
	if ((HRESULT)v5 < 0)
	{
		_com_issue_errorex((HRESULT)v5, static_cast<IUnknown*>(v3), *_unk_BD8F28); ///GUID _GUID_2aeeeb36_a4e1_4e2b_8f6f_2e7bdec5c53d
	}
	_sub_4039AC(result, &pvarg, 0);//non-existent func in v95//int __thiscall sub_4039AC(VARIANTARG *pvargDest, VARIANTARG *pvargSrc, char) //works with v95 overwrite//memcpy_s(result, 0x10u, &pvarg, 0x10u);//_sub_4039AC(result, &pvarg, 0); //works with v95 overwrite
	pvarg.vt = 0;
	if (sPath.m_Data)
	{
		_sub_402EA5(sPath.m_Data);//unsigned int __thiscall _bstr_t::Data_t::Release(_bstr_t::Data_t *this)
	}
	return result;
	//return _sub_5D995B(pThis, nullptr, result, sPath);
};
//bool sub_9F4E54_initialized = true; //unsigned int __cdecl Crc32_GetCrc32_VMTable(unsigned int* pmem, unsigned int size, unsigned int* pcheck, unsigned int *pCrc32) 
static _sub_9F4E54_t _sub_9F4E54_Hook = [](unsigned int* pmem, void* edx, unsigned int size, unsigned int* pcheck, unsigned int* pCrc32) {
	//if (sub_9F4E54_initialized)
	//{
	std::cout << "!!!WARNING!!! _sub_9F4E54 anonymously called !!!WARNING!!!" << std::endl;
	//sub_9F4E54_initialized = false;
	//}
	edx = nullptr;
	unsigned int result = *pCrc32;
	//v4 = pCrc32; /	/disabled this part just in case its anonymously called, rewrite if you want CrC
	//v6 = 0;
	//v7 = a2 >> 1;
	//v9 = a2 >> 1;
	//v8 = 0;
	//if (a2)
	//{
	//	while (1)
	//	{
	//		if (v6 == v7)
	//		{
	//			v9 = 0;
	//			*v4 = ((unsigned int)*v4 >> 8) ^ _dword_BF167C[*v4 & 0xFF ^ *(unsigned __int8*)(v6 + a1)]; //unsigned int g_crc32Table[256]
	//			*a3 = 811;
	//			result = *v4 + 1;
	//		}
	//		else
	//		{
	//			*v4 = ((unsigned int)*v4 >> 8) ^ _dword_BF167C[*v4 & 0xFF ^ *(unsigned __int8*)(v6 + a1)]; //unsigned int g_crc32Table[256]
	//			v6 = v8;
	//		}
	//		v8 = ++v6;
	//		if (v6 >= a2)
	//		{
	//			break;
	//		}
	//		v7 = v9;
	//	}
	//}
	return result;
};
bool Hook_sub_9F4E54(bool bEnable)	//1
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F4E54;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F4E54), _sub_9F4E54_Hook);	//2
}
bool Hook_sub_5D995B(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x005D995B;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_5D995B), _sub_5D995B_Hook);	//2
}
static _sub_4032B2_t _sub_4032B2_Hook = [](Ztl_variant_t* pThis, void* edx, bool fAddRef, bool fTryChangeType) {
	//if (resmanSTARTED) {
	//	std::cout << "_sub_4032B2 of resMAN started" << std::endl;
	//}
	return _sub_4032B2(pThis, nullptr, fAddRef, fTryChangeType);//IUnknown* __thiscall Ztl_variant_t::GetUnknown(Ztl_variant_t* this, bool fAddRef, bool fTryChangeType)
};
bool Hook_sub_4032B2(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x004032B2;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_4032B2), _sub_4032B2_Hook);	//2
}
static _sub_425ADD_t _sub_425ADD_Hook = [](Ztl_bstr_t* pThis, void* edx, const char* str) {
	//if (resmanSTARTED) {
	//	std::cout << "resman-bstr_t wchar val: " << str << std::endl;
	//}
	return _sub_425ADD(pThis, nullptr, str); //HRESULT __thiscall IWzPackage::Init(IWzPackage *this, Ztl_bstr_t sKey, Ztl_bstr_t sBaseUOL, IWzSeekableArchive *pArchive)
};
bool Hook_sub_425ADD(bool bEnable)	//1
{
	BYTE firstval = 0x56;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00425ADD;	//will crash if you hook to early, so you gotta check the byte to see
	while (true)
	{						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s)
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_425ADD), _sub_425ADD_Hook);	//2
}
//#pragma optimize("", on)
