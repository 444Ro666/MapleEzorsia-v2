// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ReplacementFuncs.h"
#include "dinput8.h"

//NOTE: ideally order hooks by execution order in exe to best bypass themida but im lazy...

//executed after the client is unpacked
void MainFunc()
{
	//NOTE: rewritten functions will kill all direct memory edits and code caves within their memory range

	//Hook_sub_9F9808(true);//not rewritten//sub_9F9808 end 009F9892
	//Hook_sub_4959B8(true);//not rewritten//sub_4959B8 end 00495A86

	//all virtualized sections(v83, ordered top to bottom based on address):
	//Hook_sub_44E546(false);//unlikely conflict, section virtualized by default //sub_44E546	end 0044E5D4 //unsigned int __cdecl CheckSumForce(char *pbStart, unsigned int dwSize)
	//Hook_sub_44E5D5(false);//unlikely conflict, section virtualized by default //sub_44E5D5	end	0044E6C2 //unsigned int __cdecl GetGPNMemCrc32(unsigned int dwImgBase, unsigned int dwGPNStart, unsigned int dwGPNSize)
	//Hook_sub_44E716(false);//unlikely conflict, section virtualized by default //sub_44E716	end 0044E822 //unsigned int __cdecl GetGPNFileCrc32(unsigned int dwImgBase, unsigned int dwGPNStart, unsigned int dwGPNSize)
	Hook_sub_44E88E(true);//not re-written//unlikely conflict, section virtualized by default //sub_44E88E	end	0044EA61 //int (__stdcall *__stdcall MyGetProcAddress(HINSTANCE__ *hModule, const char *lpProcName))()
	//Hook_sub_44EA64(false);//unlikely conflict, section virtualized by default //sub_44EA64	end	0044EBEB //HINSTANCE__ *__cdecl InitSafeDll()
	//Hook_sub_44EC9C(false);//unlikely conflict, section virtualized by default //sub_44EC9C	end	0044ED46 //void __cdecl HideDll(HINSTANCE__ *hModule)
	//Hook_sub_44ED47(false);//unlikely conflict, section virtualized by default //sub_44ED47	end	0044EEE6 //void __cdecl ResetLSP(void)
	//Hook_sub_494931(false);//not rewritten//unlikely conflict, section virtualized by default //sub_494931  end 00494BE8 //void __thiscall CClientSocket::ConnectLogin(CClientSocket *this)
	Hook_sub_494CA3(true);//seems to work//unlikely conflict, section virtualized by default //sub_494CA3  end 00494D04 //CClientSocket::Connect(CClientSocket *this, CClientSocket::CONNECTCONTEXT *ctx)
	Hook_sub_494D07(true);//seems to work//unlikely conflict, section virtualized by default //sub_494D07	end	00494D2C //unnamed function, part of CClientSocket::Connect in v83, merged together with connect in v95
	Hook_sub_494D2F(true);//seems to work//unlikely conflict, section virtualized by default //sub_494D2F	end	00494ECE //CClientSocket::Connect(CClientSocket *this, sockaddr_in *pAddr)
	//Hook_sub_494ED1(false);//not rewritten//unlikely conflict, section virtualized by default //sub_494ED1  end 004954C4 //CClientSocket::OnConnect(CClientSocket * this, int bSuccess)
	Hook_sub_9F4E54(true);//rewritten friendly CrC non-checker//unlikely conflict, section virtualized by default //sub_9F4E54  end 009F4F08	//likely: unsigned int __cdecl Crc32_GetCrc32_VMTable(unsigned int* pmem, unsigned int size, unsigned int* pcheck, unsigned int *pCrc32) 
	//Hook_sub_9F4F09(false);//not rewritten//unlikely conflict, section virtualized by default //sub_9F4F09  end 009F4FD9 //unknown func, built into cwvsapp::run in v95 //not called by anything
	Hook_sub_9F4FDA(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F4FDA end 009F51D3 //void __thiscall CWvsApp::CWvsApp(CWvsApp *this, const char *sCmdLine)
	Hook_sub_9F5239(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F5239 end 009F5C4F //void __thiscall CWvsApp::SetUp(CWvsApp *this)
	Hook_sub_9F5C50(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F5C50  end 009F698D //void __thiscall CWvsApp::Run(CWvsApp *this, int *pbTerminate)
	//defined//sub_9F6F27 end 009F7033 //void __thiscall CWvsApp::ConnectLogin(CWvsApp *this)
	Hook_sub_9F7CE1(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F7CE1 end 009F821E //void __thiscall CWvsApp::InitializeInput(CWvsApp *this)
	Hook_sub_9F84D0(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F84D0 end 009F8B5E //void __thiscall CWvsApp::CallUpdate(CWvsApp *this, int tCurTime)
	//sub_A4BB2B end 00A4BC8D //void __thiscall CSecurityClient::InitModule(CSecurityClient *this) //may be useful for re-purposing hackshield
	//sub_A4BCFF end 00A4BD62 //void __thiscall CSecurityClient::ClearModule(CSecurityClient *this)
	//sub_A4BD91 end 00A4BDF7 //void __thiscall CSecurityClient::StartModule(CSecurityClient *this)?? may not be right, v95 had start section virtualized
	//sub_A4BDF8 end 00A4BE5B //void __thiscall CSecurityClient::StopModule(CSecurityClient *this)
	//other non-virtualized(in v83) that hendi mentioned:
	//sub_411BBB end 004123D1 //void __thiscall CActionMan::SweepCache(CActionMan *this)
	//sub_A03EBA end 00A04002 //void __thiscall CWvsContext::OnEnterField(CWvsContext *this)

	//!!fixes todo: 005F40A4 006395D3 00A03350 maybe:_sub_9F5239 _sub_9F84D0
	//! _sub_9F5C50 _sub_9F84D0

	//HookPcCreateObject_IWzResMan(true);//not rewritten //sub_9FAF55    end 009FAFB9
	//HookPcCreateObject_IWzNameSpace(true);//not rewritten	//sub_9FAFBA    end 009FB01E
	//HookPcCreateObject_IWzFileSystem(true);//not rewritten //sub_9FB01F    end 009FB083
	HookCWvsApp__Dir_BackSlashToSlash(true);//rewritten but minor utility //sub_9F95FE	end 009F9620
	//HookCWvsApp__Dir_upDir(true);//not rewritten //sub_9F9644	end 009F9679
	//Hookbstr_ctor(true);//not rewritten //sub_406301	end	00406356
	Hook_sub_9F7964(true);//re-written for testing//HRESULT __thiscall IWzFileSystem::Init(IWzFileSystem *this, Ztl_bstr_t sPath)
	//HookIWzNameSpace__Mount(true);//not rewritten
	Hook_sub_9F7159(true);//added on some stuff//void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)//sub_9F7159 end 009F7909 //experimental //ty to all the contributors of the ragezone release: Client load .img instead of .wz v62~v92
	Hook_StringPool__GetString(true);//no conflicts, only modifies return //hook stringpool modification //ty !! popcorn //ty darter
	Hook_sub_78C8A6(true);//potential conflicts //sub_78C8A6	end 0078D165 //custom exp table client side
	Hook_CUIStatusBar__ChatLogAdd(false);//@@still crashes	//potential conflicts //sub_8DB070	end 008DB45A //custom Set Any Chat Bar Limit (default is 64) //ty Spiderman
	Hook_sub_425ADD(true);//not re-written, just tracking//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s)
	Hook_sub_9F51F6(true);//not re-written, just tracking process exit for tests//void __thiscall CWvsApp::~CWvsApp(CWvsApp *this)
	Hook_sub_9FCD88(true);//not re-written, just tracking process exit for tests//void __thiscall <IWzSeekableArchive(IWzSeekableArchive* this, IUnknown* p)
	Hook_sub_5D995B(true);//re-written!!may conflict//Ztl_variant_t *__thiscall IWzNameSpace::Getitem(IWzNameSpace *this, Ztl_variant_t *result, Ztl_bstr_t sPath)
	Hook_sub_4032B2(true);//not re-written, just tracking process exit for tests//IUnknown* __thiscall Ztl_variant_t::GetUnknown(Ztl_variant_t* this, bool fAddRef, bool fTryChangeType)
	//Hook_get_unknown(true);
	//Hook_get_resource_object(true); //helper function hooks  //ty teto for helping me get started
	//Hook_com_ptr_t_IWzProperty__ctor(true);
	//Hook_com_ptr_t_IWzProperty__dtor(true);

	std::cout << " Applying updated startup routines" << std::endl;
	Client::UpdateGameStartup();

	std::cout << "Applying resolution " << Client::m_nGameWidth << "x" << Client::m_nGameHeight << std::endl;
	Client::UpdateResolution();

	dinput8::CreateHook();
	std::cout << "dinput8 hook initialized" << std::endl;
}

//gatekeeper thread
void MainProc() { MainMain::CreateInstance(MainFunc); }

//dll entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{	//MainMain::CreateConsole(MainMain::stream);//console for devs, use this to log stuff if you want
		MainMain::mainThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, GetCurrentThreadId());

		//windows API hooks(for ones that are called by the maplestory client)//there is more than this, but the default ones in the client template mostly do logging
		//note: these are likely not all the Windows API maple uses, for everything it uses you need to look at the dlls it imports on the system and kernel level
		//it's possible to make hooks for some of these that change how maplestory interacts with the operating system, for example possibly loading up modern-made
		//anti-cheat(that you bought from the anti-cheat company) into the exe. or could potentially be used to install miners or kill registry (dont do these last 2)
		Hook_CreateMutexA(true);//Sleep(42);//not in-game edit //multiclient //ty darter, angel, and alias! //new one: credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		Hook_WSPStartup(true);//allows to set IP for default installed client//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		Hook_CreateWindowExA(true);//not in-game edit//enables minimize button //default ezorsia
		Hook_FindFirstFileA(true);//kills check for .dll already in dir//ty joo for advice with this, check out their releases: https://github.com/OpenRustMS
		Hook_GetACP(true);//either kills locale checks or feeds the server custom data//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		Hook_GetModuleFileNameW(true);//not in-game edit//better call _GetModuleFileNameW more flexible//default ezorsia
		Hook_GetLastError(false);//for some reason crashes at hooking, still dunno why//mostly for deving, prints last error to console
		//INITWINHOOK("KERNEL32", "OpenProcess", OpenProcess_Original, OpenProcess_t, WinHooks::OpenProcess_Hook); //Used to track what processes Maple opens.
		//INITWINHOOK("KERNEL32", "CreateProcessW", CreateProcessW_Original, CreateProcessW_t, WinHooks::CreateProcessW_Hook); //Used to track what maple is trying to start (mainly for anticheat modules).
		//INITWINHOOK("KERNEL32", "CreateProcessA", CreateProcessA_Original, CreateProcessA_t, WinHooks::CreateProcessA_Hook); //Used same as above and also to kill/redirect some web requests
		//INITWINHOOK("KERNEL32", "OpenMutexA", OpenMutexA_Original, OpenMutexA_t, WinHooks::OpenMutexA_Hook);//In some versions, Maple calls this library function to check if the anticheat has started. We can spoof this and return a fake handle for it to close.
		//INITWINHOOK("KERNEL32", "RegCreateKeyExA", RegCreateKeyExA_Original, RegCreateKeyExA_t, WinHooks::RegCreateKeyExA_Hook); //Maplestory saves registry information (config stuff) for a number of things. This can be used to track that.
		//INITWINHOOK("KERNEL32", "GetProcAddress", GetProcAddress_Original, GetProcAddress_t, WinHooks::GetProcAddress_Hook); //Used to map out imports used by MapleStory
		//INITWINHOOK("NTDLL", "NtTerminateProcess", NtTerminateProcess_Original, NtTerminateProcess_t, WinHooks::NtTerminateProcess_Hook); //We use this function to track what memory addresses are killing the process,There are more ways that Maple kills itself, but this is one of them.

		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&MainProc, nullptr, 0, nullptr);
		break;
	}
	default: break;
	case DLL_PROCESS_DETACH:
	{
		MainMain::GetInstance()->~MainMain();
		break;
	}
	}
	return TRUE;
}
