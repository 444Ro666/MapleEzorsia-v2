#pragma once
const DWORD dwDInput8DLLInject = 0x00796357; 
const DWORD dwMovementFlushInterval = 0x0068A83F;
const DWORD dwStatWndOnStatChange = 0x00A20213;
const DWORD dwUserEmoteCoolTime = 0x00A244AE;
const DWORD dwUserGivePopularityCoolTime = 0x00A23F28;
const DWORD dwMessageChatDelay = 0x00490651;
const DWORD dwMessageChatSpam = 0x00490607;
const DWORD dwRemoteAddress = 0x00AFE084;
const DWORD dwIGCipherHash = 0x00A4A845; //related to packet encryption, not used, kept for reference
const DWORD dwIGCipherVirtual1 = 0x00A4A8DA;
const DWORD dwIGCipherVirtual2 = 0x00A4A9BF;
const DWORD dwIGCipherDecrypt = 0x00A4A942;
const DWORD dwIGCipherDecryptStr = 0x00A4A9F7;
const DWORD dwUnlimitedSoulRush = 0x0096BF09;
const DWORD dwUnlimitedFJ = 0x0096BEB5;
const DWORD dwUnlimitedFJYVector = 0x0096BF86;
//0x008DB181 + 1	position of the line breaks in the text displayed in this chat log //ty dream
//008DFB06 008D4B75 008D4C27 008D4CBF 008D6308 //related to vertical scrolling //ty rynyan
//005F456A version number edit //ty windy
//008DFBA0 character typing limit	//ty chris
//008C4EA2 stat window related?? //ty masterrulax
//skill tooltip extension x 008F26F3/008F26F8 y //ty rynyan
//item UI tooltip extension 008EEEAF //ty rain

// ===== Resolution Modifications =====
const DWORD dwByteAvatarMegaHPos = 0x0045B97E;
const DWORD dwAvatarMegaWidth = 0x0045A5CB;
const DWORD dwApplicationHeight = 0x009F7B1D;
const DWORD dwApplicationWidth = 0x009F7B23;
const DWORD dwCursorVectorVPos = 0x0059A15D;
const DWORD dwCursorVectorHPos = 0x0059A169;
const DWORD dwUpdateMouseLimitVPos = 0x0059AC22;
const DWORD dwUpdateMouseLimitHPos = 0x0059AC09;
const DWORD dwCursorPosLimitVPos = 0x0059A8B1;
const DWORD dwCursorPosLimitHPos = 0x0059A898;
const DWORD dwToolTipLimitVPos = 0x008F32DF;
const DWORD dwToolTipLimitHPos = 0x008F32CC;
const DWORD dwTempStatToolTipDraw = 0x007B2EA0;
const DWORD dwTempStatToolTipFind = 0x007B3087;
const DWORD dwTempStatIconVPos = 0x007B2C97;
const DWORD dwTempStatIconHpos = 0x007B2CB5;
const DWORD dwTempStatCoolTimeVPos = 0x007B2DA0;
const DWORD dwTempStatCoolTimeHPos = 0x007B2DBE;
const DWORD dwQuickSlotInitVPos = 0x008D1793;	//008D182C	layer for keys like ins. del. etc..
const DWORD dwQuickSlotInitHPos = 0x008D179A;	//008D155A, 008D1ABE	other related layer
const DWORD dwQuickSlotVPos = 0x008DF782;
const DWORD dwQuickSlotHPos = 0x008DF7F8;
const DWORD dwQuickSlotCWndVPos = 0x008DE8EE;
const DWORD dwQuickSlotCWndHPos = 0x008DE8E5;
const DWORD dwViewPortHeight = 0x009DFCF0;
const DWORD dwViewPortWidth = 0x009DFE68;
const DWORD dwBossBar = 0x00533B0E;
const DWORD dwBossBarRetn = 0x00533B15;
const DWORD dwBossBarRetn2 = 0x007E16A1;
const DWORD dwCashFix = 0x00469348;
const DWORD dwCashFixRtm = 0x0046934E;
const DWORD dwVersionNumberFix = 0x005F464D;
const DWORD dwVersionNumberFixRtm = 0x005F4657;
const int dwVersionNumberFixNOPs = 10;
const DWORD dwAlwaysViewRestoreFix = 0x00642105;
const DWORD dwAlwaysViewRestorerFixRtm = 0x0064210C;
const int dwAlwaysViewRestoreFixNOPs = 7;

//const DWORD dwloginFrameFix = 0x005F4C11;
//const DWORD dwloginFrameFixCall = 0x009DE4D2;
//const int loginFrameFixNOPs = 5;

const DWORD dwLoginBackCanvasFix = 0x0060E1BF;
const DWORD dwLoginBackCanvasFixRetn = 0x0060E1CA;
const int LoginBackCanvasFixNOPs = 11;

const DWORD dwLoginViewRecFix = 0x0062B334;
const DWORD dwLoginViewRecFixRetn = 0x0062B345;
const int LoginViewRecFixNOPs = 17;

const DWORD dwLoginBackBtnFix = 0x0060E69D;	//not used, kept for referrence
const DWORD dwLoginBackBtnFixRetn = 0x0060E6A4;
const int LoginBackBtnFixNOPs = 7;

const DWORD dwInitialLoginScreenBackXY = 0x005F4B48;	//not used, kept for referrence, resets on back step from next step

const DWORD dwLoginDescriptorFix = 0x0060D85B;
const DWORD dwLoginDescriptorFixRetn = 0x0060D88E;
const int LoginDescriptorFixNOPs = 51;

const DWORD dwMoreGainMsgs = 0x0089B185;
const DWORD dwMoreGainMsgsRetn = 0x0089B18B;
const int MoreGainMsgsNOPs = 6;

const DWORD dwMoreGainMsgsFade = 0x0089B563;
const DWORD dwMoreGainMsgsFadeRetn = 0x0089B56A;
const int MoreGainMsgsFadeNOPs = 7;

const DWORD dwMoreGainMsgsFade1 = 0x0089B4E6;
const DWORD dwMoreGainMsgsFade1Retn = 0x0089B4EB;
const int MoreGainMsgsFade1NOPs = 5;

const DWORD dwMuruengraidPlayer = 0x00554041;
const DWORD dwMuruengraidPlayerRetn = 0x00554049;
const int MuruengraidPlayerNOPs = 8;

const DWORD dwMuruengraidClock = 0x005540C9;
const DWORD dwMuruengraidClockRetn = 0x005540D1;
const int MuruengraidClockNOPs = 8;

const DWORD dwMuruengraidMonster = 0x0055414F;
const DWORD dwMuruengraidMonsterRetn = 0x00554157;
const int MuruengraidMonsterNOPs = 8;

const DWORD dwMuruengraidMonster1 = 0x005543F1;
const DWORD dwMuruengraidMonster1Retn = 0x005543F8;
const int MuruengraidMonster1NOPs = 7;

const DWORD dwMuruengraidMonster2 = 0x0055447C;
const DWORD dwMuruengraidMonster2Retn = 0x00554483;
const int MuruengraidMonster2NOPs = 7;

const DWORD dwMuruengraidEngBar = 0x00554507;
const DWORD dwMuruengraidEngBarRetn = 0x0055450C;
const int MuruengraidEngBarNOPs = 5;

const DWORD dwMuruengraidEngBar1 = 0x005541DA;
const DWORD dwMuruengraidEngBar1Retn = 0x005541E2;
const int MuruengraidEngBar1NOPs = 8;

const DWORD dwMuruengraidEngBar2 = 0x00554265;
const DWORD dwMuruengraidEngBar2Retn = 0x0055426A;
const int MuruengraidEngBar2NOPs = 5;

const DWORD dwMuruengraidClearRoundUI = 0x0053500A;	//; int __cdecl sub_43E86F(int, wchar_t *, int, int, int, int, int, int, int, int)
const DWORD dwMuruengraidClearRoundUIRetn = 0x00535014;	//may be used to fix other cases of pop-up UI misallignment
const int MuruengraidClearRoundUINOPs = 10;

const DWORD dwMuruengraidTimerCanvas = 0x00555314;
const DWORD dwMuruengraidTimerCanvasRetn = 0x00555319;
const int MuruengraidTimerCanvasNOPs = 5;

const DWORD dwMuruengraidTimerMinutes = 0x005556CC;
const DWORD dwMuruengraidTimerMinutesRetn = 0x005556D5;
const int MuruengraidTimerMinutesNOPs = 9;

const DWORD dwMuruengraidTimerSeconds = 0x005556E6;
const DWORD dwMuruengraidTimerSecondsRetn = 0x005556EC;
const int MuruengraidTimerSecondsNOPs = 6;

const DWORD dwMuruengraidTimerBar = 0x00555541;
const DWORD dwMuruengraidTimerBarRetn = 0x00555548;
const int MuruengraidTimerBarNOPs = 7;

const DWORD dwMuruengraidMonster1_2 = 0x00554BA3;
const DWORD dwMuruengraidMonster1_2Retn = 0x00554BA8;
const int MuruengraidMonster1_2NOPs = 5;

const DWORD dwStatsSubMov = 0x008C5112;
const DWORD dwStatsSubMovRetn = 0x008C5117;
const int StatsSubMovNOPs = 5;

//beginning of packed client run addies
const DWORD dwCLoginSendCheckPasswordPacket = 0x005F6994;
const DWORD dwCLoginSendCheckPasswordPacketRetn = 0x005F6B5D;
const int CLoginSendCheckPasswordPacketNops = 6;
const DWORD dw0x00A63FF3 = 0x00A63FF3;
const DWORD dw0x00A63FF3Retn = 0x00A64021;
const int dw0x00A63FF3Nops = 46;
//addresses hooked by CRCbypassed v83 version (taken from released semi-named v83 IDB) to redirect to their section. may be incomplete
//0x0044E550 //0x0044E5DB //0x0044E71D //0x0044E8B4 //0x0044EA6F //0x0044ECA1 //0x0044ED52 //0x00494D3B //0x00494EEC //0x009F4E84
//0x009F4F12 //0x009F503C //0x009F526F //0x009F6F36 //0x009F7CFA //0x009F84E9 //0x00A4BDFE //0x00A4BD99 //0x00A4BD05 //0x00A4BB39
//end of packed client run addies

const DWORD dwCUIStatusBarChatLogAddBypass = 0x008DB387;
const DWORD dwCUIStatusBarChatLogAddBypassRetn = 0x008DB38D;
const DWORD dwCUIStatusBarChatLogAddBypass2Retn = 0x008DB39A;
const int dwCUIStatusBarChatLogAddBypassNops = 6;

//my cash shop fix
const DWORD dwCashFix1 = 0x00469414;
const DWORD dwCashFix1Rtm = 0x00469420;
const int dwCashFix1NOPs = 12;
const DWORD dwCashFix2 = 0x0046942F;
const DWORD dwCashFix2Rtm = 0x0046943D;
const int dwCashFix2NOPs = 14;
const DWORD dwCashFix3 = 0x0046944C;
const DWORD dwCashFix3Rtm = 0x0046945C;
const int dwCashFix3NOPs = 16;
const DWORD dwCashFix4 = 0x0046946B;
const DWORD dwCashFix4Rtm = 0x00469479;
const int dwCashFix4NOPs = 14;
const DWORD dwCashFix5 = 0x00469488;
const DWORD dwCashFix5Rtm = 0x00469499;
const int dwCashFix5NOPs = 17;
const DWORD dwCashFix6 = 0x004694A8;
const DWORD dwCashFix6Rtm = 0x004694B4;
const int dwCashFix6NOPs = 12;
const DWORD dwCashFix7 = 0x004694C8;
const DWORD dwCashFix7Rtm = 0x004694D9;
const int dwCashFix7NOPs = 17;
const DWORD dwCashFix8 = 0x004694E8;
const DWORD dwCashFix8Rtm = 0x004694F3;
const int dwCashFix8NOPs = 11;
const DWORD dwCashFixOnOff = 0x00776B5F;
const DWORD dwCashFixOnOffRtm = 0x00776B64;
const DWORD dwCashFixOnOffCall = 0x004732D2;
const int dwCashFixOnOffNOPs = 5;
//const DWORD dwCashFixOnOffCall2 = 0x00BEC20C;
const DWORD dwCashFixPrev = 0x004AB10F;
const DWORD dwCashFixPrevRtm = 0x004AB11D;
const int dwCashFixPrevNOPs = 14;

const DWORD dwLoadUItwice = 0x009F753C;
const DWORD dwLoadUItwiceRetn = 0x009F7543;
const int dwLoadUItwiceNOPs = 7;

// ===== Unlimited Teleport =====
const DWORD dwTeleFieldLimit = 0x00957BB7;
const DWORD dwTeleUpdateTime = 0x00957BFE;
const DWORD dwTeleFootholdAbove = 0x00957EFC;
const DWORD dwTeleFootholdBelow = 0x00957ED9;
const DWORD dwTeleIsPortal = 0x00957C25;

// ===== Status Bar =====
const DWORD dwStatusBarVPos = 0x008CFD55;
const DWORD dwStatusBarPosRetn = 0x008CFD5A;

const DWORD dwStatusBarBackgroundVPos = 0x008D1F65;
const DWORD dwStatusBarBackgroundPosRetn = 0x008D1F6A;

const DWORD dwStatusBarInputVPos = 0x008D217C;
const DWORD dwStatusBarInputPosRetn = 0x008D2185;

// ===== Login Screen =====
const DWORD dwLoginCreateDlg = 0x006203E8;
const DWORD dwLoginCreateDlgRtn = 0x006203F6;

const DWORD dwLoginUsername = 0x006209A6;
const DWORD dwLoginUsernameRtn = 0x006209B1;

const DWORD dwLoginPassword = 0x00620A0D;
const DWORD dwLoginPasswordRtn = 0x00620A15;

const DWORD dwLoginInputBackgroundColor = 0x0062094F;
const DWORD dwLoginInputFontColor = 0x00620930;
const DWORD dwLoginLoginBtn = 0x00620644;
const DWORD dwLoginFindPasswordBtn = 0x006207AF;
const DWORD dwLoginQuitBtn = 0x00620917;
const DWORD dwLoginFindIDBtn = 0x00620735;
const DWORD dwLoginSaveIDBtn = 0x006206BE;
const DWORD dwLoginWebHomeBtn = 0x006208A0;
const DWORD dwLoginWebRegisterBtn = 0x00620829;