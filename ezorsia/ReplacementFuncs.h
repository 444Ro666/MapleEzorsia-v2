
//notes from my knowledge as i have not used these kinds of codes practically well
//function replacement is when you replace the original function in the client with your own fake function, usually to add some extra functionality
//for more complex applications you would also need to define the client's variables and reinterpret_cast those (no void this time)
//you need the right calling convention (match client's original or use _fastcall, i havent tried it much)
//it would help to know the benefits and drawbacks of "reinterpret_cast", as well as how it is hooking to prevent accidents
//hooking to the original function will replace it at all times when it is called by the client
//i "think" you can also hook to specific instances if they always initialize in the same point in memory and apply instructions to change it
//at just that instance, but i have not tried this yet
//i personally have not tried it more because it requires a very thorough understanding of how the client code works, re-making the parts here,
//and using it, all together, in a way that doesnt break anything
//it would be the best way to do it for very extensive client edits and if you need to replace entire functions in that context but
//code caving is generally easier for short term, one-time patchwork fixes

bool HookMyTestHook()	//a working example for testing when i was learning how to do this, it does nothing even if run
{		//thanks you teto for helping me on this learning journey
	typedef void(__fastcall* _CWndCreateWnd_t)(void* pThis, void* edx, int nLeft, int nTop, int nWidth, int nHeight, int z, int bScreenCoord, void* esi, int bSetFocus);
	static auto _CWndCreateWnd = reinterpret_cast<_CWndCreateWnd_t>(0x009DE4D2);

	static _CWndCreateWnd_t _CWndCreateWnd_Hook = [](void* eax, void* edx, int nLeft, int nTop, int nWidth, int nHeight, int z, int bScreenCoord, void* esi, int bSetFocus)
		-> void {_CWndCreateWnd(eax, edx, nLeft, nTop, nWidth, nHeight, z, bScreenCoord, esi, bSetFocus); //changing the values here will modify the result of the function
								//usually put changes here
	};
	return Memory::SetHook(true, reinterpret_cast<void**>(&_CWndCreateWnd), _CWndCreateWnd_Hook);
}

//notes: to replace a function only once you must target the runtime instance and insert the right values

bool HookDetectLogin()	//a working example for testing when i was learning how to do this, it does nothing even if run
{		//thanks you teto for helping me on this learning journey
	typedef void(__fastcall* _CLoginSendSelectCharPacket_t)(void* pThis, void* edx);
	static auto _CLoginSendSelectCharPacket = reinterpret_cast<_CLoginSendSelectCharPacket_t>(0x005F726D);

	static _CLoginSendSelectCharPacket_t _CLoginSendSelectCharPacket_Hook = [](void* eax, void* edx)
		-> void {_CLoginSendSelectCharPacket(eax, edx); //changing the values here will modify the result of the function
		//Client::loggedIn = 1;						//usually put changes here
		//Client::UpdateResolution();	//tried to get client to change to different res than login while in main game. failed. this is what's left
	};
	return Memory::SetHook(true, reinterpret_cast<void**>(&_CLoginSendSelectCharPacket), _CLoginSendSelectCharPacket_Hook);
}

//#define tagPOINT		int[]*
//bool HookLoginDescriptor()	
//{
//	typedef void(__fastcall* _CFadeWndSetOption_t)(void* esi, void* edx, int a0, int a, int a1, tagPOINT pt0, tagPOINT pt, tagPOINT pt1, int t0, int t, int t1);
//	static auto _CFadeWndSetOption = reinterpret_cast<_CFadeWndSetOption_t>(0x0051F93D);
//	tagPOINT pt0 = [-149, -63];
//	static _CFadeWndSetOption_t _CFadeWndSetOption_Hook = [](void* esi, void* edx, int a0, int a, int a1, tagPOINT pt0, tagPOINT pt, tagPOINT pt1, int t0, int t, int t1)
//		-> void {_CFadeWndSetOption(esi, edx, 255, 255, 0, pt0, [-63, 0], [0, -63], 100, 0, 300);
//	};
//	return Memory::SetHook(true, reinterpret_cast<void**>(&_CFadeWndSetOption), _CFadeWndSetOption_Hook);
//}