#pragma once
//tips:pushad and pushfd,  and, popad and popfd to push and pop all stack variables and flags to restore after

//my own load other wz file attempt //OMG working thanks to a check after CWvsApp::InitializeInput !!
const char* resmanLoadOrder[] = {  //suspecting resman load list is an array of const char* rather than a vector
"Character", //"Character"
"Mob", //"Mob"
"Skill", //"Skill"
"Reactor", //"Reactor"
"Npc", //"Npc"
"UI", //"UI"
"Quest", //"Quest"
"Item", //"Item"
"Effect", //"Effect"
"String", //"String"
"Etc", //"Etc"
"Morph", //"Morph"
"TamingMob", //"TamingMob"
"Sound", //"Sound"
"Map",  //Map"
"EzorsiaV2_UI"	//sneakily insert our wz file here and walk away
};
unsigned char resmanLoadAMNT = sizeof(resmanLoadOrder) / sizeof(resmanLoadOrder[0]) - 1;
__declspec(naked) void LoadUItwice()
{
	__asm {
	mov ebx, [eax * 4 + resmanLoadOrder]//mov     ebx, MainMain::resmanLoadOrder //mov     ebx, [ebp+eax*4+
	jmp dword ptr[dwLoadUItwiceRetn]
	}
}

int nStatusBarY = 0;
__declspec(naked) void AdjustStatusBar()
{
	__asm {
	push nStatusBarY
	push ebx // horizontal position; 0
	mov ecx, esi
	jmp dword ptr[dwStatusBarPosRetn]
	}
}

__declspec(naked) void AdjustStatusBarBG()
{
	__asm {
	push nStatusBarY
	movsd
	push 0
	jmp dword ptr[dwStatusBarBackgroundPosRetn]
	}
}

__declspec(naked) void AdjustStatusBarInput()
{
	__asm {
	push nStatusBarY
	push edi
	lea ecx, [esi + 0x0CD0]
	jmp dword ptr[dwStatusBarInputPosRetn]
	}
}

__declspec(naked) void PositionLoginDlg()
{
	__asm {
	push 0x000000B4
	push 400
	push - 48 // y
	push - 185 // x
	jmp dword ptr[dwLoginCreateDlgRtn]
	}
}
__declspec(naked) void PositionLoginUsername()
{
	__asm {
	push 0x0F
	push 0x00000084
	push 127 // y
	push 0 // x
	jmp dword ptr[dwLoginUsernameRtn]
	}
}
__declspec(naked) void PositionLoginPassword()
{
	__asm {
	push 0x0F
	push 0x78
	push 127 // y
	push 272 // x
	jmp dword ptr[dwLoginPasswordRtn]
	}
}
__declspec(naked) void PositionBossBarY()
{
	__asm { //finally working!, originally posted by Angxl
	//push 22	//modification
	push edi //part of original memory
	push dword ptr ss : [ebp - 68] //part of original memory
	lea eax, dword ptr ss : [ebp - 32] //part of original memory
	jmp dword ptr[dwBossBarRetn]
	}
}
__declspec(naked) void PositionBossBarY1()
{
	__asm { //finally working!, originally posted by Angxl
	push 22 //modification
	//push edi	//part of original memory
	push dword ptr ss : [ebp - 68] //part of original memory
	lea eax, dword ptr ss : [ebp - 32] //part of original memory
	jmp dword ptr[dwBossBarRetn]
	}
}

int serverMessageExists;
__declspec(naked) void PositionBossBarY2()
{
	__asm {
	mov esi, eax
	and dword ptr ss : [ebp - 4] , 0
	mov serverMessageExists, esi
	jmp dword ptr[dwBossBarRetn2]
	}
}

int myHeight = -(Client::m_nGameHeight - 600) / 2;
int myWidth = -(Client::m_nGameWidth - 800) / 2;

__declspec(naked) void CashShopFix()
{
	__asm {
	push eax //vCanvas //originally posted by shavitash		//fixed
	push ebx //nZ
	push ebx //uHeight
	push ebx //uWidth
	push myHeight//84//myHeight //nTop - do the math yourself, this wont compile obviously
	push myWidth//283//myWidth //nLeft - same as above
	jmp dword ptr[dwCashFixRtm]
	}
}

int nHeightOfsetted1 = 0;
int nWidthOfsetted1 = 0;
int nTopOfsetted1 = 0;
int nLeftOfsetted1 = 0;
int nHeightOfsetted2 = 0;
int nWidthOfsetted2 = 0;
int nTopOfsetted2 = 0;
int nLeftOfsetted2 = 0;
int nHeightOfsetted3 = 0;
int nWidthOfsetted3 = 0;
int nTopOfsetted3 = 0;
int nLeftOfsetted3 = 0;
int nHeightOfsetted4 = 0;
int nWidthOfsetted4 = 0;
int nTopOfsetted4 = 0;
int nLeftOfsetted4 = 0;
int nHeightOfsetted5 = 0;
int nWidthOfsetted5 = 0;
int nTopOfsetted5 = 0;
int nLeftOfsetted5 = 0;
int nHeightOfsetted6 = 0;
int nWidthOfsetted6 = 0;
int nTopOfsetted6 = 0;
int nHeightOfsetted7 = 0;
int nWidthOfsetted7 = 0;
int nTopOfsetted7 = 0;
int nLeftOfsetted7 = 0;
int nHeightOfsetted8 = 0;
int nWidthOfsetted8 = 0;
int nTopOfsetted8 = 0;
int nLeftOfsetted8 = 0;

__declspec(naked) void CashShopFix1()
{
	__asm {
	push nHeightOfsetted1
	push nWidthOfsetted1
	push nTopOfsetted1
	push nLeftOfsetted1
	jmp dword ptr[dwCashFix1Rtm]
	}
}

__declspec(naked) void CashShopFix2()
{
	__asm {
	push nHeightOfsetted2
	push nWidthOfsetted2
	push nTopOfsetted2
	push nLeftOfsetted2
	jmp dword ptr[dwCashFix2Rtm]
	}
}

__declspec(naked) void CashShopFix3()
{
	__asm {
	push nHeightOfsetted3
	push nWidthOfsetted3
	push nTopOfsetted3
	push nLeftOfsetted3
	jmp dword ptr[dwCashFix3Rtm]
	}
}

__declspec(naked) void CashShopFix4()
{
	__asm {
	push nHeightOfsetted4
	push nWidthOfsetted4
	push nTopOfsetted4
	push nLeftOfsetted4
	jmp dword ptr[dwCashFix4Rtm]
	}
}

__declspec(naked) void CashShopFix5()
{
	__asm {
	push nHeightOfsetted5
	push nWidthOfsetted5
	push nTopOfsetted5
	push nLeftOfsetted5
	jmp dword ptr[dwCashFix5Rtm]
	}
}

__declspec(naked) void CashShopFix6()
{
	__asm {
	push nHeightOfsetted6
	push nWidthOfsetted6
	push nTopOfsetted6
	jmp dword ptr[dwCashFix6Rtm]
	}
}

__declspec(naked) void CashShopFix7()
{
	__asm {
	push nHeightOfsetted7
	push nWidthOfsetted7
	push nTopOfsetted7
	push nLeftOfsetted7
	jmp dword ptr[dwCashFix7Rtm]
	}
}

__declspec(naked) void CashShopFix8()
{
	__asm {
	push nHeightOfsetted8
	push nWidthOfsetted8
	push nTopOfsetted8
	push nLeftOfsetted8
	jmp dword ptr[dwCashFix8Rtm]
	}
}

__declspec(naked) void CashShopFixOnOff()
{	//could be improved upon because idk if it's the right way to do it or if it might cause issues
	__asm {
	pop ebx
	push ecx
	mov ecx, dword ptr[dwCashFixOnOffCall]
	call ecx
	add esp, 4
	leave
	retn 4
	}
}

int nHeightOfsettedPrev = 0;
int nWidthOfsettedPrev = 0;
int nTopOfsettedPrev = 0;
int nLeftOfsettedPrev = 0;

__declspec(naked) void CashShopFixPrev()
{
	__asm {
	push nHeightOfsettedPrev
	push nWidthOfsettedPrev
	push nTopOfsettedPrev
	push nLeftOfsettedPrev
	jmp dword ptr[dwCashFixPrevRtm]
	}
}

int nTopOfsettedVerFix = 0;
int nLeftOfsettedVerFix = 0;

__declspec(naked) void VersionNumberFix()
{
	__asm {
	mov eax, nLeftOfsettedVerFix
	sub eax, DWORD PTR[ebp - 0x1c]
	push nTopOfsettedVerFix
	jmp dword ptr[dwVersionNumberFixRtm]
	}
}

int myAlwaysViewRestoreFixOffset = 0;

__declspec(naked) void AlwaysViewRestoreFix()
{
	__asm {
	test eax, eax
	jnz C_Dest
	mov ecx, myAlwaysViewRestoreFixOffset
	push myAlwaysViewRestoreFixOffset
	jmp dword ptr[dwAlwaysViewRestorerFixRtm]
	C_Dest:
	mov ecx, DWORD PTR[eax]
	push eax
	jmp dword ptr[dwAlwaysViewRestorerFixRtm]
	}
}

//int nHeightOfsettedloginFrameFix = 0; int nWidthOfsettedloginFrameFix = 0;
//int nTopOfsettedloginFrameFix = 0; int nLeftOfsettedloginFrameFix = 0;

//__declspec(naked) void loginFrameFix() {
//	__asm {
//		pop ebx
////		push 1
//		push 0
//		push 1
//		push	nHeightOfsettedloginFrameFix
//		push	nWidthOfsettedloginFrameFix
//		push	nTopOfsettedloginFrameFix
//		push	nLeftOfsettedloginFrameFix
//		push esi
//		call dword ptr[dwloginFrameFixCall]
//		add esp, 4
//		leave
//		retn 4
//	}
//}

int nHeightOfsettedLoginBackCanvasFix = 0;
int nWidthOfsettedLoginBackCanvasFix = 0;
int nTopOfsettedLoginBackCanvasFix = 0;
int nLeftOfsettedLoginBackCanvasFix = 0;

__declspec(naked) void ccLoginBackCanvasFix()
{
	__asm {
	push nHeightOfsettedLoginBackCanvasFix
	push nWidthOfsettedLoginBackCanvasFix
	push nTopOfsettedLoginBackCanvasFix
	push nLeftOfsettedLoginBackCanvasFix
	jmp dword ptr[dwLoginBackCanvasFixRetn]
	}
}

int nHeightOfsettedLoginViewRecFix = 0;
int nWidthOfsettedLoginViewRecFix = 0;
int nTopOfsettedLoginViewRecFix = 0;
int nLeftOfsettedLoginViewRecFix = 0;

__declspec(naked) void ccLoginViewRecFix()
{
	__asm {
	push nHeightOfsettedLoginViewRecFix
	push nWidthOfsettedLoginViewRecFix
	push nTopOfsettedLoginViewRecFix
	push nLeftOfsettedLoginViewRecFix
	jmp dword ptr[dwLoginViewRecFixRetn]
	}
}

int yOffsetOfLoginBackBtnFix = 0;
int xOffsetOfLoginBackBtnFix = 0;

__declspec(naked) void ccLoginBackBtnFix()
{	//un used
	__asm {
	mov esi, yOffsetOfLoginBackBtnFix
	push esi
	mov edi, xOffsetOfLoginBackBtnFix
	push edi
	push 0x3e8
	jmp dword ptr[dwLoginBackBtnFixRetn]
	}
}

int a1x = 0;
int a2x = 0;
int a2y = 0;
int a3 = 0;
int a1y = 0;

__declspec(naked) void ccLoginDescriptorFix()
{
	__asm {
	and edx, 0x3f
	add edx, 0x21
	add edx, a2y
	cmp ecx, edi
	setl bl
	mov ecx, esi
	mov DWORD PTR[esi + 0x4], 0xaf7084
	mov DWORD PTR[esi + 0x8], 0xaf7080
	neg ebx
	sbb ebx, ebx
	and ebx, a3 //and ebx, 0x64
	add ebx, eax
	push ebx
	push edx
	xor eax, eax
	add eax, a1x //a1x
	push eax
	push edx
	push eax
	push edx
	mov eax, a2x //mov    eax, 0xffffff6b
	push eax
	push edi
	jmp dword ptr[dwLoginDescriptorFixRetn]
	}
}

int MoreGainMsgsOffset = 6;

__declspec(naked) void ccMoreGainMsgs()
{
	__asm {
	mov eax, DWORD PTR[edi + 0x10]
	cmp eax, MoreGainMsgsOffset
	jmp dword ptr[dwMoreGainMsgsRetn]
	}
}

int MoreGainMsgsFadeOffset = 0;

__declspec(naked) void ccMoreGainMsgsFade()
{
	__asm {
	add eax,MoreGainMsgsFadeOffset
	push 3
	jmp dword ptr[dwMoreGainMsgsFadeRetn]
	}
}

int MoreGainMsgsFade1Offset = 0;

__declspec(naked) void ccMoreGainMsgsFade1()
{
	__asm {
	push MoreGainMsgsFade1Offset
	jmp dword ptr[dwMoreGainMsgsFade1Retn]
	}
}

int yOffsetOfMuruengraidPlayer = 50;
int xOffsetOfMuruengraidPlayer = 169;

__declspec(naked) void ccMuruengraidPlayer()
{
	__asm {
	push yOffsetOfMuruengraidPlayer
	push xOffsetOfMuruengraidPlayer
	push ecx
	jmp dword ptr[dwMuruengraidPlayerRetn]
	}
}

int yOffsetOfMuruengraidClock = 26;
int xOffsetOfMuruengraidClock = 400;

__declspec(naked) void ccMuruengraidClock()
{
	__asm {
	push yOffsetOfMuruengraidClock
	push xOffsetOfMuruengraidClock
	push ecx
	jmp dword ptr[dwMuruengraidClockRetn]
	}
}

int yOffsetOfMuruengraidMonster = 50;
int xOffsetOfMuruengraidMonster = 631;

__declspec(naked) void ccMuruengraidMonster()
{
	__asm {
	push yOffsetOfMuruengraidMonster
	push xOffsetOfMuruengraidMonster
	push ecx
	jmp dword ptr[dwMuruengraidMonsterRetn]
	}
}

int yOffsetOfMuruengraidMonster1 = 32;
int xOffsetOfMuruengraidMonster1 = 317;

__declspec(naked) void ccMuruengraidMonster1()
{
	__asm {
	push yOffsetOfMuruengraidMonster1
	push xOffsetOfMuruengraidMonster1
	jmp dword ptr[dwMuruengraidMonster1Retn]
	}
}

int yOffsetOfMuruengraidMonster2 = 32;
int xOffsetOfMuruengraidMonster2 = 482;

__declspec(naked) void ccMuruengraidMonster2()
{
	__asm {
	push yOffsetOfMuruengraidMonster2
	push xOffsetOfMuruengraidMonster2
	jmp dword ptr[dwMuruengraidMonster2Retn]
	}
}

int yOffsetOfMuruengraidEngBar = 86;
int xOffsetOfMuruengraidEngBar = 17;

__declspec(naked) void ccMuruengraidEngBar()
{
	__asm {
	push yOffsetOfMuruengraidEngBar
	push xOffsetOfMuruengraidEngBar
	push ecx
	jmp dword ptr[dwMuruengraidEngBarRetn]
	}
}

int yOffsetOfMuruengraidEngBar1 = 130;
int xOffsetOfMuruengraidEngBar1 = 20;

__declspec(naked) void ccMuruengraidEngBar1()
{
	__asm {
	push yOffsetOfMuruengraidEngBar1
	push xOffsetOfMuruengraidEngBar1
	push ecx
	jmp dword ptr[dwMuruengraidEngBar1Retn]
	}
}

int yOffsetOfMuruengraidEngBar2 = 80;
int xOffsetOfMuruengraidEngBar2 = 9;

__declspec(naked) void ccMuruengraidEngBar2()
{
	__asm {
	push yOffsetOfMuruengraidEngBar2
	push xOffsetOfMuruengraidEngBar2
	push ecx
	jmp dword ptr[dwMuruengraidEngBar2Retn]
	}
}

int yOffsetOfMuruengraidClearRoundUI = 260;
int xOffsetOfMuruengraidClearRoundUI = 400;

__declspec(naked) void ccMuruengraidClearRoundUI()
{
	__asm {
	mov ecx, esi
	push yOffsetOfMuruengraidClearRoundUI
	push xOffsetOfMuruengraidClearRoundUI
	jmp dword ptr[dwMuruengraidClearRoundUIRetn]
	}
}

int yOffsetOfMuruengraidTimerCanvas = 28;
int xOffsetOfMuruengraidTimerCanvas = 112;

__declspec(naked) void ccMuruengraidTimerCanvas()
{
	__asm {
	push yOffsetOfMuruengraidTimerCanvas
	movsd
	push xOffsetOfMuruengraidTimerCanvas
	jmp dword ptr[dwMuruengraidTimerCanvasRetn]
	}
}

int yOffsetOfMuruengraidTimerMinutes = 0;
int xOffsetOfMuruengraidTimerMinutes = 0;

__declspec(naked) void ccMuruengraidTimerMinutes()
{
	__asm {
	mov DWORD PTR[esi + 0x848], edi
	push eax
	push yOffsetOfMuruengraidTimerMinutes
	push xOffsetOfMuruengraidTimerMinutes
	jmp dword ptr[dwMuruengraidTimerMinutesRetn]
	}
}

int yOffsetOfMuruengraidTimerSeconds = 0;
int xOffsetOfMuruengraidTimerSeconds = 68;

__declspec(naked) void ccMuruengraidTimerSeconds()
{
	__asm {
	mov ecx, esi
	push edx
	push yOffsetOfMuruengraidTimerSeconds
	push xOffsetOfMuruengraidTimerSeconds
	jmp dword ptr[dwMuruengraidTimerSecondsRetn]
	}
}

int yOffsetOfMuruengraidTimerBar = 16;
int xOffsetOfMuruengraidTimerBar = 345;

__declspec(naked) void ccMuruengraidTimerBar()
{
	__asm {
	push yOffsetOfMuruengraidTimerBar
	push xOffsetOfMuruengraidTimerBar
	jmp dword ptr[dwMuruengraidTimerBarRetn]
	}
}

int xOffsetOfMuruengraidMonster1_2 = 318;

__declspec(naked) void ccMuruengraidMonster1_2()
{
	__asm {
	mov edx,xOffsetOfMuruengraidMonster1_2
	jmp dword ptr[dwMuruengraidMonster1_2Retn]
	}
}

__declspec(naked) void ccStatsSubMov()
{
	__asm {
	mov ecx, esi
	add DWORD PTR[ebp + 0x8], 84 //260-176
	push DWORD PTR[ebp + 0x8]
	jmp dword ptr[dwStatsSubMovRetn]
	}
}
//beginning of packed client run caves
__declspec(naked) void ccCLoginSendCheckPasswordPacket()
{
	__asm {
	jmp dword ptr[dwCLoginSendCheckPasswordPacketRetn]
	}
}

__declspec(naked) void cc0x00A63FF3()
{
	__asm {
	push ebp
	mov ebp, esp
	push 0xffffffff
	push 0xb412e0
	push 0xa62478
	mov eax, fs:0x0
	push eax
	mov DWORD PTR fs : 0x0, esp
	sub esp, 0x58
	push ebx
	push esi
	push edi
	mov DWORD PTR[ebp - 0x18], esp
	call DWORD PTR ds : 0xaf0074
	xor edx, edx
	jmp dword ptr[dw0x00A63FF3Retn]
	}
}
//end of packed client run caves

//char CUIStatusBarChatLogAddBypassLimit = 0xFF;
__declspec(naked) void ccCUIStatusBarChatLogAddBypass()
{
	__asm {
	cmp DWORD PTR[eax - 0x4], 0xFF
	jbe CUIStatusBarChatLogAddBypass_Dest
	jmp dword ptr[dwCUIStatusBarChatLogAddBypassRetn]
	CUIStatusBarChatLogAddBypass_Dest:
	jmp dword ptr[dwCUIStatusBarChatLogAddBypass2Retn]
	}
}

constexpr char myWzFile[] = "TamingMob";
const char* ptrmyWzFile = myWzFile;

int MINT = 51 + 1;
constexpr DWORD dwTesting = 0x009F753C;
constexpr DWORD dwTestingRetn = 0x009F7543;
constexpr int TestingNOPs = dwTestingRetn - dwTesting;
__declspec(naked) void testingCodeCave()
{
	__asm {
	mov ebx, [eax * 4 + resmanLoadOrder]//mov     ebx, MainMain::resmanLoadOrder //mov     ebx, [ebp+eax*4+
	jmp dword ptr[dwTestingRetn]
	}
}

constexpr DWORD dwTesting2 = 0x005549F8;
constexpr DWORD dwTesting2Retn = 0x005549FD;
constexpr int Testing2NOPs = 5;
__declspec(naked) void testingCodeCave2()
{
	__asm {
	//call dword ptr[custom_sub_4289B7]
	jmp dword ptr[dwTesting2Retn]
	}
}

constexpr DWORD dwTesting3 = 0x005556CC;
constexpr DWORD dwTesting3Retn = 0x005556D5;
constexpr int Testing3NOPs = 9;
__declspec(naked) void testingCodeCave3()
{
	__asm {
	mov DWORD PTR[esi + 0x848], edi
	push eax
	push 80//0
	push 250//0
	jmp dword ptr[dwTesting3Retn]
	}
}

constexpr DWORD dwTesting4 = 0x005556E6;
constexpr DWORD dwTesting4Retn = 0x005556EC;
constexpr int Testing4NOPs = 6;
__declspec(naked) void testingCodeCave4()
{
	__asm {
	mov ecx,esi
	push edx
	push 80//0
	push 318//68
	jmp dword ptr[dwTesting4Retn]
	}
}
