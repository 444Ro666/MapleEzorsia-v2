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
			return (HANDLE)0xBAADF00D;
		}

		return _CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
	};

	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_CreateMutexA), Hook);
}