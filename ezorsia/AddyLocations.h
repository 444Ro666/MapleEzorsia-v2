#pragma once
constexpr DWORD dwDInput8DLLInject = 0x00796357;
constexpr DWORD dwMovementFlushInterval = 0x0068A83F;
constexpr DWORD dwStatWndOnStatChange = 0x00A20213;
constexpr DWORD dwUserEmoteCoolTime = 0x00A244AE;
constexpr DWORD dwUserGivePopularityCoolTime = 0x00A23F28;
constexpr DWORD dwMessageChatDelay = 0x00490651;
constexpr DWORD dwMessageChatSpam = 0x00490607;
constexpr DWORD dwRemoteAddress = 0x00AFE084;
constexpr DWORD dwIGCipherHash = 0x00A4A845; //related to packet encryption, not used, kept for reference
constexpr DWORD dwIGCipherVirtual1 = 0x00A4A8DA;
constexpr DWORD dwIGCipherVirtual2 = 0x00A4A9BF;
constexpr DWORD dwIGCipherDecrypt = 0x00A4A942;
constexpr DWORD dwIGCipherDecryptStr = 0x00A4A9F7;
constexpr DWORD dwUnlimitedSoulRush = 0x0096BF09;
constexpr DWORD dwUnlimitedFJ = 0x0096BEB5;
constexpr DWORD dwUnlimitedFJYVector = 0x0096BF86;
//0x008DB181 + 1	position of the line breaks in the text displayed in this chat log //ty dream
//008DFB06 008D4B75 008D4C27 008D4CBF 008D6308 //related to vertical scrolling //ty rynyan
//005F456A version number edit //ty windy
//008DFBA0 character typing limit	//ty chris
//008C4EA2 stat window related?? //ty masterrulax
//skill tooltip extension x 008F26F3/008F26F8 y //ty rynyan
//item UI tooltip extension 008EEEAF //ty rain

// ===== Resolution Modifications =====
constexpr DWORD dwByteAvatarMegaHPos = 0x0045B97E;
constexpr DWORD dwAvatarMegaWidth = 0x0045A5CB;
constexpr DWORD dwApplicationHeight = 0x009F7B1D;
constexpr DWORD dwApplicationWidth = 0x009F7B23;
constexpr DWORD dwCursorVectorVPos = 0x0059A15D;
constexpr DWORD dwCursorVectorHPos = 0x0059A169;
constexpr DWORD dwUpdateMouseLimitVPos = 0x0059AC22;
constexpr DWORD dwUpdateMouseLimitHPos = 0x0059AC09;
constexpr DWORD dwCursorPosLimitVPos = 0x0059A8B1;
constexpr DWORD dwCursorPosLimitHPos = 0x0059A898;
constexpr DWORD dwToolTipLimitVPos = 0x008F32DF;
constexpr DWORD dwToolTipLimitHPos = 0x008F32CC;
constexpr DWORD dwTempStatToolTipDraw = 0x007B2EA0;
constexpr DWORD dwTempStatToolTipFind = 0x007B3087;
constexpr DWORD dwTempStatIconVPos = 0x007B2C97;
constexpr DWORD dwTempStatIconHpos = 0x007B2CB5;
constexpr DWORD dwTempStatCoolTimeVPos = 0x007B2DA0;
constexpr DWORD dwTempStatCoolTimeHPos = 0x007B2DBE;
constexpr DWORD dwQuickSlotInitVPos = 0x008D1793;	//008D182C	layer for keys like ins. del. etc..
constexpr DWORD dwQuickSlotInitHPos = 0x008D179A;	//008D155A, 008D1ABE	other related layer
constexpr DWORD dwQuickSlotVPos = 0x008DF782;
constexpr DWORD dwQuickSlotHPos = 0x008DF7F8;
constexpr DWORD dwQuickSlotCWndVPos = 0x008DE8EE;
constexpr DWORD dwQuickSlotCWndHPos = 0x008DE8E5;
constexpr DWORD dwViewPortHeight = 0x009DFCF0;
constexpr DWORD dwViewPortWidth = 0x009DFE68;
constexpr DWORD dwBossBar = 0x00533B0E;
constexpr DWORD dwBossBarRetn = 0x00533B15;
constexpr DWORD dwBossBarRetn2 = 0x007E16A1;
constexpr DWORD dwCashFix = 0x00469348;
constexpr DWORD dwCashFixRtm = 0x0046934E;
constexpr DWORD dwVersionNumberFix = 0x005F464D;
constexpr DWORD dwVersionNumberFixRtm = 0x005F4657;
constexpr int dwVersionNumberFixNOPs = 10;
constexpr DWORD dwAlwaysViewRestoreFix = 0x00642105;
constexpr DWORD dwAlwaysViewRestorerFixRtm = 0x0064210C;
constexpr int dwAlwaysViewRestoreFixNOPs = 7;

//const DWORD dwloginFrameFix = 0x005F4C11;
//const DWORD dwloginFrameFixCall = 0x009DE4D2;
//const int loginFrameFixNOPs = 5;

constexpr DWORD dwLoginBackCanvasFix = 0x0060E1BF;
constexpr DWORD dwLoginBackCanvasFixRetn = 0x0060E1CA;
constexpr int LoginBackCanvasFixNOPs = 11;

constexpr DWORD dwLoginViewRecFix = 0x0062B334;
constexpr DWORD dwLoginViewRecFixRetn = 0x0062B345;
constexpr int LoginViewRecFixNOPs = 17;

constexpr DWORD dwLoginBackBtnFix = 0x0060E69D;	//not used, kept for referrence
constexpr DWORD dwLoginBackBtnFixRetn = 0x0060E6A4;
constexpr int LoginBackBtnFixNOPs = 7;

constexpr DWORD dwInitialLoginScreenBackXY = 0x005F4B48;	//not used, kept for referrence, resets on back step from next step

constexpr DWORD dwLoginDescriptorFix = 0x0060D85B;
constexpr DWORD dwLoginDescriptorFixRetn = 0x0060D88E;
constexpr int LoginDescriptorFixNOPs = 51;

constexpr DWORD dwMoreGainMsgs = 0x0089B185;
constexpr DWORD dwMoreGainMsgsRetn = 0x0089B18B;
constexpr int MoreGainMsgsNOPs = 6;

constexpr DWORD dwMoreGainMsgsFade = 0x0089B563;
constexpr DWORD dwMoreGainMsgsFadeRetn = 0x0089B56A;
constexpr int MoreGainMsgsFadeNOPs = 7;

constexpr DWORD dwMoreGainMsgsFade1 = 0x0089B4E6;
constexpr DWORD dwMoreGainMsgsFade1Retn = 0x0089B4EB;
constexpr int MoreGainMsgsFade1NOPs = 5;

constexpr DWORD dwMuruengraidPlayer = 0x00554041;
constexpr DWORD dwMuruengraidPlayerRetn = 0x00554049;
constexpr int MuruengraidPlayerNOPs = 8;

constexpr DWORD dwMuruengraidClock = 0x005540C9;
constexpr DWORD dwMuruengraidClockRetn = 0x005540D1;
constexpr int MuruengraidClockNOPs = 8;

constexpr DWORD dwMuruengraidMonster = 0x0055414F;
constexpr DWORD dwMuruengraidMonsterRetn = 0x00554157;
constexpr int MuruengraidMonsterNOPs = 8;

constexpr DWORD dwMuruengraidMonster1 = 0x005543F1;
constexpr DWORD dwMuruengraidMonster1Retn = 0x005543F8;
constexpr int MuruengraidMonster1NOPs = 7;

constexpr DWORD dwMuruengraidMonster2 = 0x0055447C;
constexpr DWORD dwMuruengraidMonster2Retn = 0x00554483;
constexpr int MuruengraidMonster2NOPs = 7;

constexpr DWORD dwMuruengraidEngBar = 0x00554507;
constexpr DWORD dwMuruengraidEngBarRetn = 0x0055450C;
constexpr int MuruengraidEngBarNOPs = 5;

constexpr DWORD dwMuruengraidEngBar1 = 0x005541DA;
constexpr DWORD dwMuruengraidEngBar1Retn = 0x005541E2;
constexpr int MuruengraidEngBar1NOPs = 8;

constexpr DWORD dwMuruengraidEngBar2 = 0x00554265;
constexpr DWORD dwMuruengraidEngBar2Retn = 0x0055426A;
constexpr int MuruengraidEngBar2NOPs = 5;

constexpr DWORD dwMuruengraidClearRoundUI = 0x0053500A;	//; int __cdecl sub_43E86F(int, wchar_t *, int, int, int, int, int, int, int, int)
constexpr DWORD dwMuruengraidClearRoundUIRetn = 0x00535014;	//may be used to fix other cases of pop-up UI misallignment
constexpr int MuruengraidClearRoundUINOPs = 10;

constexpr DWORD dwMuruengraidTimerCanvas = 0x00555314;
constexpr DWORD dwMuruengraidTimerCanvasRetn = 0x00555319;
constexpr int MuruengraidTimerCanvasNOPs = 5;

constexpr DWORD dwMuruengraidTimerMinutes = 0x005556CC;
constexpr DWORD dwMuruengraidTimerMinutesRetn = 0x005556D5;
constexpr int MuruengraidTimerMinutesNOPs = 9;

constexpr DWORD dwMuruengraidTimerSeconds = 0x005556E6;
constexpr DWORD dwMuruengraidTimerSecondsRetn = 0x005556EC;
constexpr int MuruengraidTimerSecondsNOPs = 6;

constexpr DWORD dwMuruengraidTimerBar = 0x00555541;
constexpr DWORD dwMuruengraidTimerBarRetn = 0x00555548;
constexpr int MuruengraidTimerBarNOPs = 7;

constexpr DWORD dwMuruengraidMonster1_2 = 0x00554BA3;
constexpr DWORD dwMuruengraidMonster1_2Retn = 0x00554BA8;
constexpr int MuruengraidMonster1_2NOPs = 5;

constexpr DWORD dwStatsSubMov = 0x008C5112;
constexpr DWORD dwStatsSubMovRetn = 0x008C5117;
constexpr int StatsSubMovNOPs = 5;

//beginning of packed client run addies
constexpr DWORD dwCLoginSendCheckPasswordPacket = 0x005F6994;
constexpr DWORD dwCLoginSendCheckPasswordPacketRetn = 0x005F6B5D;
constexpr int CLoginSendCheckPasswordPacketNops = 6;
constexpr DWORD dw0x00A63FF3 = 0x00A63FF3;
constexpr DWORD dw0x00A63FF3Retn = 0x00A64021;
constexpr int dw0x00A63FF3Nops = 46;
//addresses hooked by CRCbypassed v83 version (taken from released semi-named v83 IDB) to redirect to their section. may be incomplete
//0x0044E550 //0x0044E5DB //0x0044E71D //0x0044E8B4 //0x0044EA6F //0x0044ECA1 //0x0044ED52 //0x00494D3B //0x00494EEC //0x009F4E84
//0x009F4F12 //0x009F503C //0x009F526F //0x009F6F36 //0x009F7CFA //0x009F84E9 //0x00A4BDFE //0x00A4BD99 //0x00A4BD05 //0x00A4BB39
//end of packed client run addies

constexpr DWORD dwCUIStatusBarChatLogAddBypass = 0x008DB387;
constexpr DWORD dwCUIStatusBarChatLogAddBypassRetn = 0x008DB38D;
constexpr DWORD dwCUIStatusBarChatLogAddBypass2Retn = 0x008DB39A;
constexpr int dwCUIStatusBarChatLogAddBypassNops = 6;

//my cash shop fix
constexpr DWORD dwCashFix1 = 0x00469414;
constexpr DWORD dwCashFix1Rtm = 0x00469420;
constexpr int dwCashFix1NOPs = 12;
constexpr DWORD dwCashFix2 = 0x0046942F;
constexpr DWORD dwCashFix2Rtm = 0x0046943D;
constexpr int dwCashFix2NOPs = 14;
constexpr DWORD dwCashFix3 = 0x0046944C;
constexpr DWORD dwCashFix3Rtm = 0x0046945C;
constexpr int dwCashFix3NOPs = 16;
constexpr DWORD dwCashFix4 = 0x0046946B;
constexpr DWORD dwCashFix4Rtm = 0x00469479;
constexpr int dwCashFix4NOPs = 14;
constexpr DWORD dwCashFix5 = 0x00469488;
constexpr DWORD dwCashFix5Rtm = 0x00469499;
constexpr int dwCashFix5NOPs = 17;
constexpr DWORD dwCashFix6 = 0x004694A8;
constexpr DWORD dwCashFix6Rtm = 0x004694B4;
constexpr int dwCashFix6NOPs = 12;
constexpr DWORD dwCashFix7 = 0x004694C8;
constexpr DWORD dwCashFix7Rtm = 0x004694D9;
constexpr int dwCashFix7NOPs = 17;
constexpr DWORD dwCashFix8 = 0x004694E8;
constexpr DWORD dwCashFix8Rtm = 0x004694F3;
constexpr int dwCashFix8NOPs = 11;
constexpr DWORD dwCashFixOnOff = 0x00776B5F;
constexpr DWORD dwCashFixOnOffRtm = 0x00776B64;
constexpr DWORD dwCashFixOnOffCall = 0x004732D2;
constexpr int dwCashFixOnOffNOPs = 5;
//const DWORD dwCashFixOnOffCall2 = 0x00BEC20C;
constexpr DWORD dwCashFixPrev = 0x004AB10F;
constexpr DWORD dwCashFixPrevRtm = 0x004AB11D;
constexpr int dwCashFixPrevNOPs = 14;

constexpr DWORD dwLoadUItwice = 0x009F753C;
constexpr DWORD dwLoadUItwiceRetn = 0x009F7543;
constexpr int dwLoadUItwiceNOPs = 7;

// ===== Unlimited Teleport =====
constexpr DWORD dwTeleFieldLimit = 0x00957BB7;
constexpr DWORD dwTeleUpdateTime = 0x00957BFE;
constexpr DWORD dwTeleFootholdAbove = 0x00957EFC;
constexpr DWORD dwTeleFootholdBelow = 0x00957ED9;
constexpr DWORD dwTeleIsPortal = 0x00957C25;

// ===== Status Bar =====
constexpr DWORD dwStatusBarVPos = 0x008CFD55;
constexpr DWORD dwStatusBarPosRetn = 0x008CFD5A;

constexpr DWORD dwStatusBarBackgroundVPos = 0x008D1F65;
constexpr DWORD dwStatusBarBackgroundPosRetn = 0x008D1F6A;

constexpr DWORD dwStatusBarInputVPos = 0x008D217C;
constexpr DWORD dwStatusBarInputPosRetn = 0x008D2185;

// ===== Login Screen =====
constexpr DWORD dwLoginCreateDlg = 0x006203E8;
constexpr DWORD dwLoginCreateDlgRtn = 0x006203F6;

constexpr DWORD dwLoginUsername = 0x006209A6;
constexpr DWORD dwLoginUsernameRtn = 0x006209B1;

constexpr DWORD dwLoginPassword = 0x00620A0D;
constexpr DWORD dwLoginPasswordRtn = 0x00620A15;

constexpr DWORD dwLoginInputBackgroundColor = 0x0062094F;
constexpr DWORD dwLoginInputFontColor = 0x00620930;
constexpr DWORD dwLoginLoginBtn = 0x00620644;
constexpr DWORD dwLoginFindPasswordBtn = 0x006207AF;
constexpr DWORD dwLoginQuitBtn = 0x00620917;
constexpr DWORD dwLoginFindIDBtn = 0x00620735;
constexpr DWORD dwLoginSaveIDBtn = 0x006206BE;
constexpr DWORD dwLoginWebHomeBtn = 0x006208A0;
constexpr DWORD dwLoginWebRegisterBtn = 0x00620829;
