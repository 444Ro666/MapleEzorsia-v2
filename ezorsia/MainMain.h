#pragma once
#include <functional>
#include <WinSock2.h>
#include <WS2spi.h>

class MainMain
{
private:
	static MainMain* _s_pInstance;

public:
	static FILE* stream;
	static bool ownLoginFrame;
	static bool ownCashShopFrame;
	static bool EzorsiaV2WzIncluded;
	static bool useV62_ExpTable;
	static const char* m_sRedirectIP;
	static SOCKET m_GameSock;
	static WSPPROC_TABLE m_ProcTable;
	static unsigned int v83ArrayForCustomEXP[];
	static unsigned int v62ArrayForCustomEXP[];
	static unsigned int expTableMemSize;
	static const char* use_custom_dll_1;
	static const char* use_custom_dll_2;
	static const char* use_custom_dll_3;
	static bool usingEzorsiaV2Wz;
	static bool CustomLoginFrame;
	static bool bigLoginFrame;
	static HANDLE mainTHread;

private: // forcing the class to only have one instance, created through CreateInstance
	MainMain(std::function<void()> pPostMutexFunc);
	MainMain() = delete;
	MainMain(const MainMain&) = delete;
	MainMain& operator = (const MainMain&) = delete;

public:
	~MainMain();
public:
	static void CreateInstance(std::function<void()> pMutexFunc)
	{
		if (_s_pInstance) return;
		_s_pInstance = new MainMain(pMutexFunc);
	}
	static MainMain* GetInstance()
	{
		return _s_pInstance;
	}
	static void CreateConsole(FILE* stream) {
		AllocConsole();
		freopen_s(&stream, "CONOUT$", "w", stdout); //CONOUT$
	}
	static void DeleteConsole(FILE* stream) {
		FreeConsole();
		fclose(stream);
	}
};