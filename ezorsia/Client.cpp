#include "stdafx.h"
#include "AddyLocations.h"
#include "codecaves.h"

int Client::m_nGameHeight = 720;
int Client::m_nGameWidth = 1280;
int Client::MsgAmount = 26;
bool Client::WindowedMode = true;
bool Client::RemoveLogos = true;
double Client::setDamageCap = 199999.0;
bool Client::useTubi = false;
int Client::speedMovementCap = 140;
std::string Client::ServerIP_AddressFromINI = "127.0.0.1";

void Client::UpdateGameStartup()
{

	Memory::CodeCave(cc0x00A63FF3, dw0x00A63FF3, dw0x00A63FF3Nops); //fix start @0x00A63FF3, may be unnecessary, but dump of vanilla client showed broken code here

	Memory::CodeCave(ccCLoginSendCheckPasswordPacket, dwCLoginSendCheckPasswordPacket, CLoginSendCheckPasswordPacketNops); //CLogin::SendCheckPasswordPacket: At the start of the sequence of pushes that contains 0C9h, place a long jmp to further down in the method to the SystemInfo basic block.Do auth patches for encoding the correct strings(user / pw)
	Memory::WriteByte(0x005F6B87 + 2, 0x08); //CLogin::SendCheckPasswordPacket/sub_5F6952		end 005F6C6F //??
	Memory::WriteByte(0x005F6BA0 + 2, 0xA0); //CLogin::SendCheckPasswordPacket //?? //not sure what these edits specifically do, but hendi's client has them in place for getting rid of checks at this part
	unsigned char CA_005F6BA4[] = { 0xFF, 0x75, 0x0C, 0x90, 0x90, 0x90, 0x90 }; //CLogin::SendCheckPasswordPacket //??
	Memory::WriteByteArray(0x005F6BA4, CA_005F6BA4, sizeof(CA_005F6BA4)); //CLogin::SendCheckPasswordPacket //??

	Memory::WriteByte(0x00496633 + 1, 0x47); //kill CSecurityClient::OnPacket//ty chronicle for idea for this

	Memory::FillBytes(0x009F1C04, 0x90, 5);//run from packed client //WinMain: nop ShowStartUpWndModal	//get rid of pop-up at start of game
	//^might not work for packed client since the window seems to be run before the edit can take place, keeping in case
	Memory::WriteByte(0x009F242F, 0xEB); //run from packed client //WinMain: jz->jmp for ShowADBalloon code (pretty much at the end of method, above push with small number)

	Memory::WriteByte(0x009F6EDC, 0xEB); //kill if statement in broken //void __thiscall CWvsApp::CreateMainWindow(CWvsApp *this)

	Memory::WriteByte(0x009F74EA + 3, resmanLoadAMNT); //replace their wz load list size with ours //void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)
	Memory::CodeCave(LoadUItwice, dwLoadUItwice, dwLoadUItwiceNOPs);//working after a check after CWvsApp::InitializeInput

	//Memory::WriteByte(0x008DB387 + 3, 0xFF); //set charbar limit
	//Memory::CodeCave(ccCUIStatusBarChatLogAddBypass, dwCUIStatusBarChatLogAddBypass, dwCUIStatusBarChatLogAddBypassNops); //set charbar limit

	Memory::FillBytes(0x00C08459, 0x20, 0x00C0846E - 0x00C08459);//remove elevation requests
	Memory::WriteByte(0x00C08459, 0x22);//remove elevation requests	//thanks stelmo for showing me how to do this
	Memory::WriteString(0x00C08459 + 1, "asInvoker");//remove elevation requests	//not working from dll
	Memory::WriteByte(0x00C08463, 0x22);//remove elevation requests	//thanks stelmo for showing me how to do this
	Memory::WriteByte(0x0049C2CD + 1, 0x01);//remove elevation requests	//still not working unfortunately	//still keeping this to checks for admin privilege
	Memory::WriteByte(0x0049CFE8 + 1, 0x01);//likely requires affecting WINAPI CreateProcess, which requires a launcher		//because a packed client cannot be directly edited for these offsets
	Memory::WriteByte(0x0049D398 + 1, 0x01);//remove elevation requests	//still not working unfortunately

	Memory::FillBytes(0x00AFE084, 0x00, 0x006FE0B2 - 0x006FE084);//remove the existing server IP address in client
	const char* serverIP_Address = ServerIP_AddressFromINI.c_str();//could be obselete, but keeping it in case of use by unpacked localhosts
	Memory::WriteString(0x00AFE084, serverIP_Address);//write the user-set IP address
	Memory::WriteString(0x00AFE084 + 16, serverIP_Address);//write the user-set IP address
	Memory::WriteString(0x00AFE084 + 32, serverIP_Address);//write the user-set IP address

	//optional non-resolution related stuff
	if (useTubi) { Memory::FillBytes(0x00485C32, 0x90, 2); }
	Memory::WriteDouble(0x00AFE8A0, setDamageCap);	//ty rain
	int setDamageCapInt = static_cast<int>(setDamageCap < 0 ? setDamageCap - 0.5 : setDamageCap + 0.5);
	Memory::WriteInt(0x008C3304 + 1, setDamageCapInt); //ty rain

	Memory::WriteInt(0x00780743 + 3, speedMovementCap); //set speed cap //ty ronan
	Memory::WriteInt(0x008C4286 + 1, speedMovementCap); //set speed cap //ty ronan
	Memory::WriteInt(0x0094D91E + 1, speedMovementCap); //set speed cap //ty ronan

	Memory::WriteByte(0x0040013E, 0x2F);  //4g edit, not sure if it still works after execution

	//other potential resolution edits/etc

	//0043D260 //0043D3E2 //0043D5C8	//CAnimationDisplayer::Effect_RewardRullet
	//0048B96A	//CChatBalloon::MakeScreenBalloon
	//0049D105 //0049D218 //CConfig::
	//004D584D //CCtrlMLEdit::CreateIMECandWnd
	//0053EFC3 //x and y //SP_4371_UI_UIWINDOWIMG_ARIANTMATCH_RESULT
	//0055BEE6 //0055BEEC //0055C07F //0055C086 //0055C1C5 //0055C1CD //CField_LimitedView::DrawViewRange !!!!!
	//005EB45A //005EB464 //CItemSpeakerDlg::CItemSpeakerDlg
	//00663079 //CMob::Init
	//007C252C //007C2531 //CTradingRoomDlg::OnCreate
	//007E15BE //CSlideNotice::CSlideNotice
	//007E19CA //CSlideNotice::SetMsg
	//007E9ABC //007E9ACD //sub_7E99BC
	//007EB409 //007EB41A //sub_7EB303
	//007F2007 //007F201B //sub_7F1F25
	//0085F341 //0085F361 //sub_85F303
	//008C7FEA //NOP at +2,+3,+4,+5 //sub_8C7FB6  related to CUIStat::Draw(tagRECT const *) //dunno what they were trying to do here, noping jumps
	//008D2C03 //008D2EED //008D2FB3 //008D305B //008D3124 //008D31EC //008D3273 //008D32FA //008D3381 //008D3408 //008D348F //008D358B //008D369B //CUIStatusBar::OnCreate
	//008D405E //008D40D4 //sub_8D3B2F
	//008D4B93 //008D4BBC //CUIStatusBar::SetChatType
	//00960581 //00960839 //00960C67 //00960DED //CUserLocal::DrawCombo
}

void Client::UpdateResolution()
{
	nStatusBarY = m_nGameHeight - 578;

	Memory::CodeCave(AdjustStatusBar, dwStatusBarVPos, 5);
	Memory::CodeCave(AdjustStatusBarBG, dwStatusBarBackgroundVPos, 5);
	Memory::CodeCave(AdjustStatusBarInput, dwStatusBarInputVPos, 9);

	Memory::WriteInt(dwApplicationHeight + 1, m_nGameHeight);//push 600
	Memory::WriteInt(dwApplicationWidth + 1, m_nGameWidth);	//push 800 ; CWvsApp::InitializeGr2D
	Memory::WriteInt(dwCursorVectorVPos + 2, static_cast<int>(floor(-m_nGameHeight / 2)));//push -300				!!moves all interactable UI elements!!
	Memory::WriteInt(dwCursorVectorHPos + 2, static_cast<int>(floor(-m_nGameWidth / 2)));	//push -400 ; CInputSystem::SetCursorVectorPos				!!moves all interactable UI elements!!
	Memory::WriteInt(dwUpdateMouseLimitVPos + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(dwUpdateMouseLimitHPos + 1, m_nGameWidth);	//mov ecx,800 ; CInputSystem::UpdateMouse
	Memory::WriteInt(dwCursorPosLimitVPos + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(dwCursorPosLimitHPos + 1, m_nGameWidth);	//mov eax,800 ; CInputSystem::SetCursorPos
	Memory::WriteInt(dwViewPortHeight + 3, m_nGameHeight);//lea eax,[esi+eax-600]
	Memory::WriteInt(dwViewPortWidth + 3, m_nGameWidth);	//lea eax,[ecx+eax-800]

	Memory::WriteInt(dwToolTipLimitVPos + 1, m_nGameWidth - 1); //mov eax,599 ; CUIToolTip::MakeLayer
	Memory::WriteInt(dwToolTipLimitHPos + 1, m_nGameWidth - 1); //mov eax,799 ; CUIToolTip::MakeLayer

	Memory::WriteInt(dwTempStatToolTipDraw + 3, -m_nGameWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::ShowToolTip
	Memory::WriteInt(dwTempStatToolTipFind + 3, -m_nGameWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::FindIcon
	Memory::WriteInt(dwTempStatIconVPos + 2, (m_nGameHeight / 2) - 23);	//sub ebx,277 ; Skill icon buff y-pos
	Memory::WriteInt(dwTempStatIconHpos + 3, (m_nGameWidth / 2) - 3);	//lea eax,[eax+esi+397] ; Skill icon buff x-pos
	Memory::WriteInt(dwTempStatCoolTimeVPos + 2, (m_nGameHeight / 2) - 23);	//sub ebx,277 ; Skill icon cooltime y-pos
	Memory::WriteInt(dwTempStatCoolTimeHPos + 3, (m_nGameWidth / 2) - 3);	//lea eax,[eax+esi+397] ; Skill icon cooltime x-pos

	Memory::WriteInt(dwQuickSlotInitVPos + 1, m_nGameHeight + 1);//add eax,533
	Memory::WriteInt(dwQuickSlotInitHPos + 1, 798); //push 647 //hd800
	Memory::WriteInt(dwQuickSlotVPos + 2, m_nGameHeight + 1);//add esi,533
	Memory::WriteInt(dwQuickSlotHPos + 1, 798); //push 647 //hd800
	Memory::WriteInt(dwQuickSlotCWndVPos + 2, (600 - m_nGameHeight) / 2 - 427); //lea edi,[eax-427]
	Memory::WriteInt(dwQuickSlotCWndHPos + 2, -798); //lea ebx,[eax-647]

	//Memory::WriteInt(dwByteAvatarMegaHPos + 1, m_nGameWidth + 100); //push 800 ; CAvatarMegaphone::ByeAvatarMegaphone ; IWzVector2D::RelMove ##BAK
	Memory::WriteInt(dwByteAvatarMegaHPos + 1, m_nGameWidth); //push 800 ; CAvatarMegaphone::ByeAvatarMegaphone ; IWzVector2D::RelMove
	Memory::WriteInt(dwAvatarMegaWidth + 1, m_nGameWidth); //push 800 ; CAvatarMegaphone ; CreateWnd

	Memory::WriteInt(0x0043717B + 1, m_nGameHeight);//mov edi,600
	Memory::WriteInt(0x00437181 + 1, m_nGameWidth);	//mov esi,800 ; CreateWnd
	Memory::WriteInt(0x0053808B + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x00538091 + 1, m_nGameWidth);	//push 800 ; RelMove?
	Memory::WriteInt(0x004CC160 + 3, m_nGameWidth);	//mov [ebp-16],800 ; CreateWnd
	Memory::WriteInt(0x004CC2C5 + 2, m_nGameHeight);//cmp ecx,600
	Memory::WriteInt(0x004CC2B0 + 1, m_nGameWidth);	//mov eax,800 ; CreateWnd
	Memory::WriteInt(0x004D59B2 + 1, m_nGameHeight);//mov eax,800
	Memory::WriteInt(0x004D599D + 1, m_nGameWidth);	//mov eax,800 ; CreateWnd
	Memory::WriteInt(0x0085F36C + 2, m_nGameWidth);	//cmp edx,800
	Memory::WriteInt(0x0085F374 + 1, m_nGameWidth - 80);	//mov ecx,720 ; CreateDlg
	Memory::WriteInt(0x008EBC58 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x008EBC3C + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x009966B5 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x009966CA + 2, m_nGameWidth);	//cmp edi,800
	Memory::WriteInt(0x009966D2 + 1, m_nGameWidth - 100);	//mov edx,700 ; CreateDlg
	Memory::WriteInt(0x009A3E7F + 1, m_nGameHeight);//mov edx,600
	Memory::WriteInt(0x009A3E72 + 1, m_nGameWidth);	//mov edx,800 ; CreateDlg
	//Memory::WriteInt(0x0045B898 + 1, m_nGameHeight - 25);	//push 575
	Memory::WriteInt(0x0045B898 + 1, m_nGameWidth - 225);	//push 575 ##ED  //smega x axis fade
	//Memory::WriteInt(0x0045B97E + 1, m_nGameWidth);	//push 800 ; RelMove? ##REDUN
	//Memory::WriteInt(0x004D9BD1 + 1, m_nGameWidth);	//push 800	; StringPool#1443 (BtMouseCilck)		//click ???related?? for tabs and numbers in cash shop
	//Memory::WriteInt(0x004D9C37 + 1, m_nGameWidth);	//push 800	; StringPool#1443 (BtMouseCilck)		//click ???related?? for tabs and numbers in cash shop
	//Memory::WriteInt(0x004D9C84 + 1, m_nGameWidth);	//push 800 ; StringPool#1443 (BtMouseCilck)		//click ???related?? for tabs and numbers in cash shop
	Memory::WriteInt(0x005386F0 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x005386F5 + 1, m_nGameWidth);	//push 800 ; CField::DrawFearEffect
	Memory::WriteInt(0x0055B808 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0055B80D + 1, m_nGameWidth);	//mov edi,800
	Memory::WriteInt(0x0055B884 + 1, m_nGameWidth);	//push 600 ; RelMove?
	Memory::WriteInt(0x007E15BE + 1, m_nGameWidth);	//push 800 ; CreateWnd
	Memory::WriteInt(0x007E16B9 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x007E16BE + 1, m_nGameWidth);	//push 800 ; CWnd::GetCanvas //!!length of server message at top
	Memory::WriteInt(0x008AA266 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x008AA26B + 1, m_nGameWidth);	//push 800 ; CreateWnd
	Memory::WriteInt(0x009F6E99 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x009F6EA0 + 1, m_nGameWidth);	//push 800 ; StringPool#1162 (MapleStoryClass)

	Memory::WriteInt(0x007CF48F + 1, m_nGameHeight);//mov eax,600 ; 
	Memory::WriteInt(0x007CF49D + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x008A12F4 + 1, m_nGameHeight);//mov eax,600 ; 
	Memory::WriteInt(0x008A1302 + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x007F257E + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x007F258F + 1, m_nGameWidth);	//push 800 ; CWnd::CreateWnd
	Memory::WriteInt(0x0046B85C + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x0046B86A + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x009994D8 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x009994ED + 2, m_nGameWidth);	//cmp edi,800
	Memory::WriteInt(0x009994F5 + 1, m_nGameWidth - 100);	//mov edx,700 ; CreateDlg

	Memory::WriteInt(0x0062FC4A + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062FC4F + 1, m_nGameWidth);	//push 800 ; IWzGr2DLayer::Getcanvas
	Memory::WriteInt(0x0062FE63 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062FE68 + 1, m_nGameWidth);	//push 800 ; IWzGr2DLayer::Getcanvas
	Memory::WriteInt(0x0062F9C6 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062F9CB + 1, m_nGameWidth);	//push 800; (UI/Logo/Wizet)
	Memory::WriteInt(0x0062F104 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x0062F109 + 1, m_nGameWidth);	//mov eax,800 ; Rectangle
	Memory::WriteInt(0x006406D5 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x006406C3 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064050A + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x006404F8 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x00640618 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x00640690 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x0064061D + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x0064064B + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x00640606 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064067E + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x00640639 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064043E + 1, static_cast<int>(floor(m_nGameWidth / 2)));	//mov edi,400
	Memory::WriteInt(0x00640443 + 1, static_cast<int>(floor(m_nGameHeight / 2)));	//mov esi,300
	Memory::WriteInt(0x00640626 + 1, static_cast<int>(floor(m_nGameWidth / 2)));	//add eax,400 ; bunch of modulus stuff

	Memory::WriteInt(0x00641038 + 2, m_nGameHeight);//??possibly related to player display
	Memory::WriteInt(0x0064103F + 2, m_nGameWidth);//??possibly related to player display
	Memory::WriteInt(0x00641048 + 1, static_cast<int>(floor(-m_nGameHeight / 2)));	//mov esi,-300
	Memory::WriteInt(0x00641050 + 1, static_cast<int>(floor(-m_nGameWidth / 2)));		//mov esi,-400 ;
	Memory::WriteInt(0x00641A19 + 3, m_nGameHeight);//mov [ebp+28],600
	Memory::WriteInt(0x00641A12 + 3, m_nGameWidth);	//mov [ebp+32],800 ; idk
	Memory::WriteInt(0x00641B38 + 3, m_nGameHeight);//mov [ebp-32],600
	Memory::WriteInt(0x00641B2E + 3, m_nGameWidth);	//mov [ebp-36],800 ; CAnimationDisplayer::SetCenterOrigin

	Memory::WriteInt(0x006CD842 + 1, static_cast<int>(floor(m_nGameWidth / 2)));	//push 400 ; RelMove?

	Memory::WriteInt(0x0059A0A2 + 6, static_cast<int>(floor(m_nGameHeight / 2)));	//mov [ebx+2364],300
	Memory::WriteInt(0x0059A09C + 2, static_cast<int>(floor(m_nGameWidth / 2)));	//mov [esi],400	; CInputSystem::LoadCursorState
	Memory::WriteInt(0x0080546C + 1, m_nGameHeight);//mov edi,600
	Memory::WriteInt(0x00805459 + 1, m_nGameWidth);	//mov edx,800 ; CUIEventAlarm::CreateEventAlarm
	Memory::WriteInt(0x008CFD4B + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008CFD50 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x0053836D + 1, static_cast<int>(floor(-m_nGameHeight / 2)));//push -300
	Memory::WriteInt(0x00538373 + 1, static_cast<int>(floor(-m_nGameWidth / 2)));	//push -400	; RelMove?
	Memory::WriteInt(0x0055BB2F + 1, static_cast<int>(floor(-m_nGameHeight / 2)));//push -300
	Memory::WriteInt(0x0055BB35 + 1, static_cast<int>(floor(-m_nGameWidth / 2)));	//push -400 ; RelMove?

	Memory::WriteInt(0x005A8B46 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005A8B56 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005A9B42 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005A9B52 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005AADAA + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005AADBA + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005ABC65 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005ABC75 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005ACB29 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005ACB39 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005C187E + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005C188E + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005C2D62 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005C2D72 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005E3FA0 + 1, m_nGameHeight);//push 600

	Memory::WriteInt(0x005F64DE + 1, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300 ;
	Memory::WriteInt(0x005F6627 + 1, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300 ;

	Memory::WriteInt(0x0060411C + 1, m_nGameHeight);//push 600
	//Memory::WriteInt(0x00604126 + 1, floor(-m_nGameWidth / 2));	//push -300 //moves characters side to side on char select //unnecessary atm
	Memory::WriteInt(0x0060F79B + 1, (m_nGameHeight / 2) - 201);//??possibly related to login utildlg
	Memory::WriteInt(0x0060F7A4 + 1, (m_nGameHeight / 2) - 181);//??possibly related to login utildlg
	Memory::WriteInt(0x0060F7AC + 1, (m_nGameWidth / 2) - 201);//??possibly related to login utildlg
	Memory::WriteInt(0x0060F7B5 + 1, (m_nGameWidth / 2) - 181);//??possibly related to login utildlg
	Memory::WriteInt(0x00613093 + 1, (m_nGameHeight / 2) - 200);//??likely related to login pop-up msg
	Memory::WriteInt(0x00613099 + 1, (m_nGameWidth / 2) - 45);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DAFF + 1, (m_nGameHeight / 2) - 150);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DB08 + 1, (m_nGameHeight / 2) - 130);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DB10 + 1, (m_nGameWidth / 2) - 201);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DB19 + 1, (m_nGameWidth / 2) - 181);//??likely related to login pop-up msg

	Memory::WriteInt(0x004372B1 + 1, static_cast<int>(floor(-m_nGameHeight / 2)));//push -300
	Memory::WriteInt(0x004372B6 + 1, static_cast<int>(floor(-m_nGameWidth / 2)));	//push -400 ; RelMove?
	Memory::WriteInt(0x006CE3AB + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x007E1CB7 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x008D82F5 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D82FA + 1, m_nGameWidth);	//push 800 ; CreateWnd?
	Memory::WriteInt(0x00935870 + 1, static_cast<int>(floor(m_nGameHeight / 2)));	//push 300
	Memory::WriteInt(0x0093586B + 1, m_nGameWidth);	// push 800 ; RelMove? (Skills)
	Memory::WriteInt(0x009DFD5C + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x009DFED2 + 1, m_nGameHeight);//mov ecx,600	; IWzVector2D::RelMove
	Memory::WriteInt(0x009F6ADD + 1, static_cast<int>(floor(m_nGameHeight / 2))); //push 300 ; MapleStoryClass
	Memory::WriteInt(0x006D50D8 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074BAA9 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B951 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B4A2 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B3B7 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x006421B3 + 1, m_nGameHeight);//push 600 ; CSoundMan::PlayBGM

	Memory::WriteInt(0x0059EB49 + 1, m_nGameHeight);//push 600 ; CSoundMan::PlayBGM
	Memory::WriteInt(0x008D247B + 1, m_nGameHeight - 33);	//push 567 ; IWzVector2D::RelMove
	Memory::WriteInt(0x008DEB93 + 1, m_nGameHeight - 20);	//push 580
	Memory::WriteInt(0x008DEE2F + 1, m_nGameHeight - 20);	//push 580
	Memory::WriteInt(0x008D2765 + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D29B4 + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D8BFE + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D937E + 1, m_nGameHeight - 19);	//push 581 //008D9373  move mana bar outline? //ty rynyan
	Memory::WriteInt(0x008D9AC9 + 1, m_nGameHeight - 19);	//push  
	Memory::WriteInt(0x008D1D50 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D1D55 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x008D1FF4 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D1FF9 + 1, m_nGameWidth);	//push 800 ; CUIStatusBar
	Memory::WriteInt(0x0062F5DF + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062F5E4 + 1, m_nGameWidth);	//push 800 ; (UI/Logo/NXXXon)
	Memory::WriteInt(0x004EDB89 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x004EDB78 + 1, m_nGameHeight);//mov ecx,600 ; CreateWnd
	Memory::WriteInt(0x004EDAD8 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x009F7079, m_nGameHeight);	// dd 600
	Memory::WriteInt(0x009F707E, m_nGameWidth);	// dd 800
	Memory::WriteInt(0x00BE2738, static_cast<int>(floor(m_nGameWidth / 2)));	// dd 400
	Memory::WriteInt(0x00BE2DF4, static_cast<int>(floor(m_nGameHeight / 2)));	// dd 300
	Memory::WriteInt(0x00BE2DF0, static_cast<int>(floor(m_nGameWidth / 2)));	// dd 400
	Memory::WriteInt(0x00640656 + 2, static_cast<int>(floor(-m_nGameWidth / 2)));		//add edi,-400 ;

	Memory::WriteInt(0x006CE4C6 + 1, static_cast<int>(floor(-m_nGameWidth / 2)));		//push -400 ;
	Memory::WriteInt(0x009E2E85 + 1, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300		overall screen visible UI scaling
	Memory::WriteInt(0x009E2E8B + 1, static_cast<int>(floor(-m_nGameWidth / 2)));		//push -400 ;	overall screen visible UI scaling

	Memory::WriteInt(0x0093519A + 1, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300 ;
	Memory::WriteInt(0x00954433 + 1, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300 ;
	Memory::WriteInt(0x00981555 + 1, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300 ;
	Memory::WriteInt(0x00981F7A + 2, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300 ;
	Memory::WriteInt(0x00A448B0 + 2, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300 ; CWvsPhysicalSpace2D::Load]

	Memory::WriteInt(0x0066BACE + 2, static_cast<int>(floor(-m_nGameWidth / 2)));		//and ecx,-400
	Memory::WriteInt(0x009B76BD + 3, static_cast<int>(floor(-m_nGameHeight / 2)));	//push -300
	Memory::WriteInt(0x009B76CB + 3, static_cast<int>(floor(m_nGameHeight / 2)));		//push 300

	Memory::WriteInt(0x009F7078 + 1, m_nGameHeight);//??related to application dimensions	//(ragezone release merge)//thanks mr mr of ragezone for these addresses
	Memory::WriteInt(0x009F707D + 1, m_nGameWidth);//??related to application dimensions

	Memory::WriteInt(0x0058C8A6 + 1, m_nGameWidth);//??

	Memory::WriteInt(0x004EDABF + 1, m_nGameHeight);//??

	Memory::WriteInt(0x00991854 + 1, m_nGameHeight);//??unknown cwnd function
	Memory::WriteInt(0x0099185F + 1, (m_nGameWidth / 2) - 134);//??unknown cwnd function
	Memory::WriteInt(0x00991867 + 1, (m_nGameWidth / 2) - 133);//??unknown cwnd function
	Memory::WriteInt(0x00992BA7 + 1, static_cast<int>(floor(m_nGameWidth / 2)));//??unknown cwnd function, possibly related to cutildlg
	Memory::WriteInt(0x00992BAC + 1, static_cast<int>(floor(m_nGameHeight / 2)));//??unknown cwnd function, possibly related to cutildlg

	Memory::WriteInt(0x007E1E07 + 2, m_nGameWidth);//??related to displaying server message at top of screen
	Memory::WriteInt(0x007E19CA + 2, m_nGameWidth);//??related to displaying server message at top of screen

	Memory::WriteInt(0x005362B2 + 1, (m_nGameWidth / 2) - 129);//??related to boss bar
	Memory::WriteInt(0x005364AA + 2, (m_nGameWidth / 2) - 128);//??related to boss bar

	Memory::WriteInt(0x00592A08 + 1, (m_nGameWidth / 2) - 125);//??likely related to mouse pos

	Memory::WriteInt(0x00621226 + 1, (m_nGameWidth / 2) - 216);//??possibly related to logo
	Memory::WriteByte(0x0062121E + 1, 0x01);//??possibly related to logo

	Memory::WriteInt(0x008C069F + 1, (m_nGameHeight / 2) - 14);//??related to status bar
	Memory::WriteInt(0x008C06A4 + 1, (m_nGameWidth / 2) - 158);//???related to status bar

	Memory::WriteInt(0x00A24D0B + 1, (m_nGameWidth / 2) - 129);//??

	Memory::WriteInt(0x00BE273C, 128);//??
	Memory::WriteByte(0x00A5FC2B, 0x05);//??
	//Memory::WriteByte(0x008D1790 + 2, 0x01); //related to quickslots area presence		 originally 1U but changed because unsigned int crashes it after char select
	Memory::WriteByte(0x0089B636 + 2, 0x01); //related to exp gain/item pick up msg, seems to affect msg height ! originally 1U but changed because unsigned int crashes it after char select
	Memory::WriteByte(0x00592A06 + 1, 0x01);//???likely related to mouse pos

	Memory::WriteInt(0x00744EB4 + 1, m_nGameWidth);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744EB9 + 1, m_nGameHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744E2A + 1, 3 * m_nGameWidth * m_nGameHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744E43 + 1, m_nGameWidth * m_nGameHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744DA6 + 1, 4 * m_nGameWidth * m_nGameHeight);//??related to in-game taking screenshot functionality

	Memory::WriteInt(0x00897BB4 + 1, (m_nGameWidth / 2) - 143);//??related to exp gain/item pick up msg

	if (WindowedMode)
	{
		unsigned char forced_window[] = { 0xb8, 0x00, 0x00, 0x00, 0x00 }; //force window mode	//thanks stelmo for showing me how to do this
		Memory::WriteByteArray(0x009F7A9B, forced_window, sizeof(forced_window));//force window mode
	} //*note:CreateWindowExA_Hook doesnt work yet//old forced window mode, is now handled in CreateWindowExA_Hook, keeping for reference
	if (RemoveLogos)
	{
		Memory::FillBytes(0x0062EE54, 0x90, 21);	//no Logo @launch //Thanks Denki!!
	}

	const int msgAmnt = MsgAmount;
	const int msgAmntOffset = msgAmnt * 14;

	Memory::WriteInt(0x0089B639 + 1, m_nGameHeight - 6 - msgAmntOffset);//inventory/exp gain y axis //####hd100 //90
	Memory::WriteInt(0x0089B6F7 + 1, m_nGameWidth - 405);//inventory/exp gain x axis //310 //####hd415 //405

	Memory::WriteInt(0x0089AF33 + 1, 400);//length of pick up and exp gain message canvas //found with help from Davi
	Memory::WriteInt(0x0089B2C6 + 1, 400);//address to move the message in the canvas adjusted above to the center of the new canvas  //thanks chris

	Memory::WriteInt(0x0089AEE2 + 3, msgAmnt);//moregainmsgs part 1
	MoreGainMsgsOffset = msgAmnt;	//param for ccmoregainmssgs
	Memory::CodeCave(ccMoreGainMsgs, dwMoreGainMsgs, MoreGainMsgsNOPs); //moregainmsgs part 2
	MoreGainMsgsFadeOffset = 15000;	//param for ccmoregainmssgsFade
	Memory::CodeCave(ccMoreGainMsgsFade, dwMoreGainMsgsFade, MoreGainMsgsFadeNOPs); //moregainmsgsFade
	MoreGainMsgsFade1Offset = 255 * 4 / 3;	//param for ccmoregainmssgsFade
	Memory::CodeCave(ccMoreGainMsgsFade1, dwMoreGainMsgsFade1, MoreGainMsgsFade1NOPs); //moregainmsgsFade1

	Memory::WriteInt(0x0045B337 + 1, m_nGameWidth);//related to smega display  //likely screen area where pop up starts for smega
	Memory::WriteInt(0x0045B417 + 1, m_nGameWidth - 225);//smega with avatar x axis for duration on screen

	Memory::WriteInt(0x007C2531 + 1, m_nGameHeight - 80);//??

	Memory::WriteInt(0x0089B796 + 1, m_nGameHeight - 18);//???related to exp gain/item pick up msg //??!!found in another diff also !!!!!!!!!!!!

	//Memory::WriteInt(0x0089BA03 + 1, m_nGameHeight - 96); //??related to exp gain/item pick up msg
	//Memory::WriteInt(0x008D3F73 + 1, m_nGameHeight - 93);//bottom frame, white area
	//Memory::WriteInt(0x008D3FE5 + 1, m_nGameHeight - 93);//bottom frame, grey area
	//Memory::WriteInt(0x008D8353 + 1, m_nGameHeight - 46); //bottom frame, character level
	//Memory::WriteInt(0x008D83D1 + 1, m_nGameHeight - 55); //role
	//Memory::WriteInt(0x008D8470 + 1, m_nGameHeight - 40); //name of character

	//Memory::WriteInt(0x008DE850 + 1, 580);//quickslotcheckX//interactivity of bottom buttoms	//test, could be wrong
	//Memory::WriteInt(0x008DE896 + 1, 647);//quickslotcheckX//interactivity of bottom buttoms	//test, could be wrong
	//Memory::WriteInt(0x008DE82B + 1, 507);///quickslotcheckY //interactivity of bottom buttoms //test, could be wrong
	//008DE8A9 //CUIStatusBar::HitTest //related to prev^

	//Memory::WriteInt(0x008DA11C + 1, m_nGameHeight - 19);//??likely various status bar UI components
	//008DA115 //sub_8D850B //related to prev^ 
	//Memory::WriteInt(0x008DA3D4 + 1, m_nGameHeight - 56); //exphpmp % labels
	//Memory::WriteInt(0x008DA463 + 1, m_nGameHeight - 51); //stat bar gradient or bracket
	//Memory::WriteInt(0x008DA4F2 + 1, m_nGameHeight - 51);//stat bar gradient or bracket
	//Memory::WriteInt(0x008DA61B + 1, m_nGameHeight - 56);//??likely various status bar UI components

	//Memory::WriteInt(0x008DA90F + 1, m_nGameHeight - 51);//brackets for stat numbers
	//Memory::WriteInt(0x008DA9C6 + 1, m_nGameHeight - 51);
	//Memory::WriteInt(0x008DAC3F + 1, m_nGameHeight - 51);
	//Memory::WriteInt(0x008DACF1 + 1, m_nGameHeight - 51);
	//Memory::WriteInt(0x008DAF64 + 1, m_nGameHeight - 51);

	//Memory::WriteInt(0x008DFA6F + 1, m_nGameHeight - 81);//chat box selection, dragging box size, minus plus sign, typing interac
	//Memory::WriteInt(0x008DFB01 + 1, m_nGameHeight - 81);
	//Memory::WriteInt(0x008DFBA5 + 1, m_nGameHeight - 80);
	//Memory::WriteInt(0x008DFC10 + 1, m_nGameHeight - 85);

	//Memory::WriteInt(0x008D4AFB + 1, m_nGameHeight - 91); //is for the little grab/resize bar on it (I think)??
	//Memory::WriteInt(0x008D4C1F + 1, m_nGameHeight - 90);//??likely various status bar UI components
	//Memory::WriteInt(0x008D4CDD + 1, m_nGameHeight - 20);//??likely various status bar UI components
	//Memory::WriteInt(0x008D4BBC + 6, m_nGameHeight - 114);//??likely various status bar UI components
	//Memory::WriteInt(0x008D4C47 + 1, m_nGameHeight - 87);//minimized chat box frame
	//Memory::WriteInt(0x008D628B + 1, m_nGameHeight - 91); //is for the background for the text area.??
	//Memory::WriteInt(0x008D6300 + 1, m_nGameHeight - 90); //is for the scroll bar on the chat text area.??
	//Memory::WriteInt(0x008D4B6D + 1, m_nGameHeight - 90);//scroll bar of chat
	//Memory::WriteInt(0x008D276A + 1, m_nGameHeight - 19);//??likely various status bar UI components

	//Memory::WriteInt(0x008D7778 + 3, m_nGameHeight - 42);//???likely various status bar UI components
	//Memory::WriteInt(0x008D7785 + 3, m_nGameHeight - 26);//??likely various status bar UI components
	//Memory::WriteInt(0x008D783A + 3, m_nGameHeight - 41);//??likely various status bar UI components
	//Memory::WriteInt(0x008D7847 + 3, m_nGameHeight - 26);//??likely various status bar UI components

	//Memory::WriteInt(0x008D2FAE + 1, m_nGameHeight - 57); //bottom 4 large buttons
	//Memory::WriteInt(0x008D3056 + 1, m_nGameHeight - 57);
	//Memory::WriteInt(0x008D311F + 1, m_nGameHeight - 57);
	//Memory::WriteInt(0x008D31E7 + 1, m_nGameHeight - 57);//bottom 4 large buttons

	Memory::WriteInt(0x00849E39 + 1, m_nGameHeight - 177); //system menu pop up
	//00849E3F related to previous^
	Memory::WriteInt(0x0084A5B7 + 1, m_nGameHeight - 281); //shortcuts pop up	//0x84A5BD -  System Options "X" Position. if needed
	//0084A5BD related to previous^

	Memory::WriteInt(0x00522C73 + 1, m_nGameHeight - 92);// ??various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00522E65 + 1, m_nGameHeight - 92); // ??various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052307E + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523359 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523595 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up //quest complete y axis
	Memory::WriteInt(0x0052378B + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052397D + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523BB1 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523DA5 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523FA3 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x005243DB + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523154 + 1, m_nGameHeight - 102);//?? various requests like party, guild, friend, family, invites that pop up
	//0052315C part of the previous^

	int reqPopOffset = 41;
	Memory::WriteInt(0x0052418C + 1, m_nGameHeight - 102);//party quest available pop-up y axis		my first address find own my own
	//005241A0 //SP_1299_UI_UIWINDOWIMG_FADEYESNO_BACKGRND5 //no idea what this is for, but if you notice broken things in pop up requests, this could be your addy
	Memory::WriteInt(0x00523092 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up	//trade		 //thank you Rain for the width addresses
	Memory::WriteInt(0x0052336D + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up //Party Invite
	Memory::WriteInt(0x00522E79 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up //friend request
	Memory::WriteInt(0x00522C87 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up	// Guild Invite
	//Memory::WriteInt(0x005235A9 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up	// Quest Complete, currently unneeded as working without it
	Memory::WriteInt(0x0052379F + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up//??
	Memory::WriteInt(0x00523991 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up/??
	Memory::WriteInt(0x00523BC5 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up/??
	Memory::WriteInt(0x00523DC5 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up// ??
	Memory::WriteInt(0x00523FB7 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up// ??
	Memory::WriteInt(0x005243EF + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up//??

	//Memory::WriteInt(0x008D326E + 1, m_nGameHeight - 85); //smol buttoms right of chat box (all - 85 ones)
	//Memory::WriteInt(0x008D32F5 + 1, m_nGameHeight - 85);
	//Memory::WriteInt(0x008D337C + 1, m_nGameHeight - 85);
	//Memory::WriteInt(0x008D3403 + 1, m_nGameHeight - 85);
	//Memory::WriteInt(0x008D348A + 1, m_nGameHeight - 85);
	//Memory::WriteInt(0x008D3586 + 1, m_nGameHeight - 85);
	//Memory::WriteInt(0x008D3696 + 1, m_nGameHeight - 85);
	//Memory::WriteInt(0x008D4058 + 1, m_nGameHeight - 85);
	//Memory::WriteInt(0x008DF903 + 1, m_nGameHeight - 85);	
	//008DF908 addr of related^ //CUIStatusBar::ToggleQuickSlot 
	//Memory::WriteInt(0x008DFFCF + 1, m_nGameHeight - 85);	//CUIStatusBar::SetButtonBlink
	//008DFFD4 //related to^
	//Memory::WriteInt(0x008D40CE + 1, m_nGameHeight - 81);//smol buttoms right of chat box (all - 85 ones)

	//Memory::CodeCave(PositionBossBarY2, 0x007E169B, 6);//boss bar, check for server msg, looking in wrong address...
	//if (serverMessageExists != 0) 
	//{
	//	Memory::CodeCave(PositionBossBarY, dwBossBar, 7);//boss bar normal position without server msg
	//}
	//else {
	//	Memory::CodeCave(PositionBossBarY1, dwBossBar, 7);//boss bar with server msg
	//}

	Memory::WriteByte(0x00533B03, 0xb8);	//boss bar extend to window
	Memory::WriteInt(0x00533B03 + 1, m_nGameWidth - 15);	//boss bar	extend to window
	Memory::WriteByte(0x00534370, 0xb9);	//boss bar	extend to window
	Memory::WriteInt(0x00534370 + 1, m_nGameWidth - 22);	//boss bar	extend to window

	//myHeight = -(Client::m_nGameHeight - 600) / 2;//cash shop fix for frame area	//recalc offsets
	//myWidth = -(Client::m_nGameWidth - 800) / 2;//cash shop fix for frame area		//recalc offsets
	//Memory::CodeCave(CashShopFix, dwCashFix, 6);//cash shop fix for frame area //moves frame to top left (not used rn)

	myHeight = (m_nGameHeight - 600) / 2;//cash shop fix for frame area	//recalc offsets
	myWidth = (m_nGameWidth - 800) / 2;//cash shop fix for frame area		//recalc offsets
	nHeightOfsetted1 = 316;
	nWidthOfsetted1 = 256;
	nTopOfsetted1 = 0 + myHeight;
	nLeftOfsetted1 = 0 + myWidth; //parameters for fix1
	Memory::CodeCave(CashShopFix1, dwCashFix1, dwCashFix1NOPs);
	nHeightOfsetted2 = 104;
	nWidthOfsetted2 = 256;
	nTopOfsetted2 = 318 + myHeight;
	nLeftOfsetted2 = -1 + myWidth; //parameters for fix2
	Memory::CodeCave(CashShopFix2, dwCashFix2, dwCashFix2NOPs);
	nHeightOfsetted3 = 163;
	nWidthOfsetted3 = 246;
	nTopOfsetted3 = 426 + myHeight;
	nLeftOfsetted3 = 0 + myWidth; //parameters for fix3
	Memory::CodeCave(CashShopFix3, dwCashFix3, dwCashFix3NOPs);
	nHeightOfsetted4 = 78;
	nWidthOfsetted4 = 508;
	nTopOfsetted4 = 17 + myHeight;
	nLeftOfsetted4 = 272 + myWidth; //parameters for fix4
	Memory::CodeCave(CashShopFix4, dwCashFix4, dwCashFix4NOPs);
	nHeightOfsetted5 = 430;
	nWidthOfsetted5 = 412;
	nTopOfsetted5 = 95 + myHeight;
	nLeftOfsetted5 = 275 + myWidth; //parameters for fix5
	Memory::CodeCave(CashShopFix5, dwCashFix5, dwCashFix5NOPs);	//main part of shop, item listings	//thanks angel for stuff that helped
	nHeightOfsetted6 = 358;
	nWidthOfsetted6 = 90;
	nTopOfsetted6 = 157 + myHeight; //parameters for fix6
	Memory::CodeCave(CashShopFix6, dwCashFix6, dwCashFix6NOPs);//code cave 6 //best sellers
	Memory::WriteInt(0x004694BA + 1, myWidth + 690);//nleft, actual drawn part	//best sellers
	nHeightOfsetted7 = 56;
	nWidthOfsetted7 = 545;
	nTopOfsetted7 = 530 + myHeight;
	nLeftOfsetted7 = 254 + myWidth; //parameters for fix7
	Memory::CodeCave(CashShopFix7, dwCashFix7, dwCashFix7NOPs);
	nHeightOfsetted8 = 22;
	nWidthOfsetted8 = 89;
	nTopOfsetted8 = 97 + myHeight;
	nLeftOfsetted8 = 690 + myWidth; //parameters for fix8
	Memory::CodeCave(CashShopFix8, dwCashFix8, dwCashFix8NOPs);
	Memory::CodeCave(CashShopFixOnOff, dwCashFixOnOff, dwCashFixOnOffNOPs);	//fix for preview On/Off button not being accurate on entering cash shop //thanks windyboy

	nHeightOfsettedPrev = 165 + myHeight;
	nWidthOfsettedPrev = 212 + myWidth;
	nTopOfsettedPrev = 40 + myHeight;
	nLeftOfsettedPrev = 24 + myWidth; //parameters for fix cash preview
	Memory::CodeCave(CashShopFixPrev, dwCashFixPrev, dwCashFixPrevNOPs); //cash shop preview fix

	Memory::WriteInt(0x00641F61 + 1, static_cast<int>(floor(m_nGameWidth / 2)));	//mov ebc,400 ;  VRleft		//camera movement
	Memory::WriteInt(0x00641FC8 + 1, static_cast<int>(floor(m_nGameHeight / 2)));	//add eax,300  ; VRTop //camera movement //not working for most maps
	//Memory::WriteInt(0x0064202F + 2, (int)floor(m_nGameWidth / 2));	//mov ebc,400 ;  VRright		//camera movement	//crashes
	Memory::WriteInt(0x0064208F + 1, static_cast<int>(floor(m_nGameHeight / 2)));	//add eax,300  ; VRbottom //camera movement //not working for most maps

	myAlwaysViewRestoreFixOffset = myHeight; //parameters for fix view restore all maps number ?????working????!!!
	Memory::CodeCave(AlwaysViewRestoreFix, dwAlwaysViewRestoreFix, dwAlwaysViewRestoreFixNOPs);	//fix view restora on all maps, currently does nothing; i likely looked in the wrong area

	if (MainMain::CustomLoginFrame)
	{
		Memory::WriteInt(0x005F481E + 1, static_cast<int>(floor(-m_nGameHeight / 2)));//push -300				!!game login frame!! turn this on if you edit UI.wz and use a frame that matches your res
		Memory::WriteInt(0x005F4824 + 1, static_cast<int>(floor(-m_nGameWidth / 2)));	//push -400 ; RelMove?				!!game login frame!! turn this on if you edit UI.wz and use a frame that matches your res
	}
	//nHeightOfsettedloginFrameFix = 0 + myHeight; nWidthOfsettedloginFrameFix = 0 + myWidth;
	//nTopOfsettedloginFrameFix = 0 + myHeight; nLeftOfsettedloginFrameFix = 0 + myWidth; //parameters for fix cash preview
	//Memory::CodeCave(loginFrameFix, dwloginFrameFix, loginFrameFixNOPs); //failed login frame fix =(

	if (MainMain::bigLoginFrame)
	{
		Memory::WriteInt(0x005F464D + 1, m_nGameWidth - 165);	//mov eax,800 ; RelMove?	//game version number for login frames that hug the side of the screen //you will still need to offset ntop, and that may require a code cave if your height resolution is too big
	}
	else
	{
		nTopOfsettedVerFix = 10 + myHeight;
		nLeftOfsettedVerFix = 645 + myWidth; //parameters for fix version number
		Memory::CodeCave(VersionNumberFix, dwVersionNumberFix, dwVersionNumberFixNOPs);	//game version number fix //use this if you use no frame or default client frame
	}

	if (!MainMain::bigLoginFrame)
	{
		nHeightOfsettedLoginBackCanvasFix = 352 + myHeight;
		nWidthOfsettedLoginBackCanvasFix = 125 + myWidth;//para for world select buttonsViewRec
		nTopOfsettedLoginBackCanvasFix = 125 + myHeight;
		nLeftOfsettedLoginBackCanvasFix = 0 + myWidth;
		Memory::CodeCave(ccLoginBackCanvasFix, dwLoginBackCanvasFix, LoginBackCanvasFixNOPs);	//world select buttons fix		//thank you teto for pointing out my error in finding the constructor

		//yOffsetOfLoginBackBtnFix = 300 + myHeight; xOffsetOfLoginBackBtnFix = 0 + myWidth;	//para for back button
		//Memory::CodeCave(ccLoginBackBtnFix, dwLoginBackBtnFix, LoginBackBtnFixNOPs); //back button on world select //unnecessary as buttons move with canvas

		nHeightOfsettedLoginViewRecFix = 167 + myHeight;
		nWidthOfsettedLoginViewRecFix = 540 + myWidth;//para for ViewRec fix
		nTopOfsettedLoginViewRecFix = 51 + myHeight;
		nLeftOfsettedLoginViewRecFix = 136 + myWidth;
		Memory::CodeCave(ccLoginViewRecFix, dwLoginViewRecFix, LoginViewRecFixNOPs);	//world ViewRec fix	

		a1x = 0 + myWidth;
		a2x = -149 + myWidth;
		a2y = 0 + myHeight;
		a3 = 25;
		a1y = -250; //a4 = 0;	//LoginDescriptor params
		Memory::WriteInt(0x0060D849 + 1, 300 + a1y); //speed 1	//temporary fix by increasing the speed of display until i get good enough at procedural programming 
		//and memory management and reverse engineering to use nXXXon's own functions to put a black layer with greater z value to cover the tabs being shown off screen at origin
		Memory::CodeCave(ccLoginDescriptorFix, dwLoginDescriptorFix, LoginDescriptorFixNOPs);	//world LoginDescriptor fix	
	}

	int customEngY = -62, customEngX = -22, dojangYoffset = 0;	//myHeight //-55-35 (myHeight*250/100)	-(myWidth*53/100) 140 -130
	yOffsetOfMuruengraidPlayer = 50 + dojangYoffset;
	xOffsetOfMuruengraidPlayer = 169 + myWidth; //params
	Memory::CodeCave(ccMuruengraidPlayer, dwMuruengraidPlayer, MuruengraidPlayerNOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidClock = 26 + dojangYoffset;
	xOffsetOfMuruengraidClock = 400 + myWidth; //params
	Memory::CodeCave(ccMuruengraidClock, dwMuruengraidClock, MuruengraidClockNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidMonster = 50 + dojangYoffset;
	xOffsetOfMuruengraidMonster = 631 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster, dwMuruengraidMonster, MuruengraidMonsterNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidMonster1 = 32 + dojangYoffset;
	xOffsetOfMuruengraidMonster1 = 317 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster1, dwMuruengraidMonster1, MuruengraidMonster1NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidMonster2 = 32 + dojangYoffset;
	xOffsetOfMuruengraidMonster2 = 482 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster2, dwMuruengraidMonster2, MuruengraidMonster2NOPs);	//muruengraid scaling
	yOffsetOfMuruengraidEngBar = 86 + dojangYoffset + customEngY;
	xOffsetOfMuruengraidEngBar = 17 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar, dwMuruengraidEngBar, MuruengraidEngBarNOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidEngBar1 = 130 + dojangYoffset + customEngY;
	xOffsetOfMuruengraidEngBar1 = 20 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar1, dwMuruengraidEngBar1, MuruengraidEngBar1NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidEngBar2 = 80 + dojangYoffset + customEngY;
	xOffsetOfMuruengraidEngBar2 = 9 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar2, dwMuruengraidEngBar2, MuruengraidEngBar2NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidClearRoundUI = 260 + myHeight;
	xOffsetOfMuruengraidClearRoundUI = 400 + myWidth; //params
	Memory::CodeCave(ccMuruengraidClearRoundUI, dwMuruengraidClearRoundUI, MuruengraidClearRoundUINOPs);	//muruengraid scaling
	//yOffsetOfMuruengraidTimerCanvas = 28 + dojangYoffset; xOffsetOfMuruengraidTimerCanvas = 112 + myWidth; //params
	//Memory::CodeCave(ccMuruengraidTimerCanvas, dwMuruengraidTimerCanvas, MuruengraidTimerCanvasNOPs);	//muruengraid scaling	
	//yOffsetOfMuruengraidTimerMinutes = 0 + dojangYoffset; xOffsetOfMuruengraidTimerMinutes = 0 + myWidth; //params	//not needed, bar moves all, kept for referrence or if change are needed
	//Memory::CodeCave(ccMuruengraidTimerMinutes, dwMuruengraidTimerMinutes, MuruengraidTimerMinutesNOPs);	//muruengraid scaling	
	//yOffsetOfMuruengraidTimerSeconds = 0 + dojangYoffset; xOffsetOfMuruengraidTimerSeconds = 68 + myWidth; //params
	//Memory::CodeCave(ccMuruengraidTimerSeconds, dwMuruengraidTimerSeconds, MuruengraidTimerSecondsNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidTimerBar = 16 + dojangYoffset;
	xOffsetOfMuruengraidTimerBar = 345 + myWidth; //params
	Memory::CodeCave(ccMuruengraidTimerBar, dwMuruengraidTimerBar, MuruengraidTimerBarNOPs);	//muruengraid scaling
	xOffsetOfMuruengraidMonster1_2 = 318 + myWidth; //params	//finally fixed this bugger
	Memory::CodeCave(ccMuruengraidMonster1_2, dwMuruengraidMonster1_2, MuruengraidMonster1_2NOPs);	//muruengraid scaling

	//testingOut("IWzProperty__GetItem _this: 0x%x, result: 0x%x, sPath: %s");//, _this, result, (char*)sPath);

	//int myStatsWindowOffsetVal = 4, myStatsWindowOffset = 176, myStatsWindowOffset1 = 177;
	//Memory::WriteInt(0x008C4AB3 + 1, myStatsWindowOffset); //stat window ty resinate
	//Memory::WriteInt(0x008C510A + 1, myStatsWindowOffset1); //stat window ty resinate

	//const char* testString = "RoSWzFile"; Memory::WriteString(0x00B3F434, testString);//testing
	//Memory::WriteInt(0x009F74EA + 3, 16); //testing
	//Memory::WriteInt(0x008C4286 + 1, 400); //testing
	//Memory::WriteInt(0x00780743 + 3, 400); //testing
	//Memory::WriteByte(0x004289C0 + 1, 99); //testing
	//Memory::FillBytes(0x00485C01, 0x90, 2);
	//Memory::FillBytes(0x00485C21, 0x90, 2);

	//Memory::CodeCave(testingCodeCave, dwTesting, TestingNOPs); //testing
	//Memory::CodeCave(testingCodeCave2, dwTesting2, Testing2NOPs); //testing
	//Memory::CodeCave(testingCodeCave3, dwTesting3, Testing3NOPs); //testing
	//Memory::CodeCave(testingCodeCave4, dwTesting4, Testing4NOPs); //testing
	//std::cout << "Client Value: " << MainMain::CustomLoginFrame << std::endl;
}

void Client::EnableNewIGCipher()
{//??not called //no idea what cipher is
	constexpr int nCipherHash = m_nIGCipherHash;
	Memory::WriteInt(dwIGCipherHash + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherVirtual1 + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherVirtual2 + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherDecrypt + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherDecryptStr + 3, nCipherHash);
}

void Client::UpdateLogin()
{	//un-used //may still contain some useful addresses for custom login
	Memory::CodeCave(PositionLoginDlg, dwLoginCreateDlg, 14);
	Memory::CodeCave(PositionLoginUsername, dwLoginUsername, 11);
	Memory::CodeCave(PositionLoginPassword, dwLoginPassword, 8);
	Memory::WriteInt(dwLoginInputBackgroundColor + 3, 0xFFF8FAFF); // ARGB value
	Memory::WriteByte(dwLoginInputFontColor + 3, 1); // bool: true=black, false=white
	Memory::WriteInt(dwLoginLoginBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginFindPasswordBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginQuitBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginFindIDBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginFindIDBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginWebHomeBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginWebRegisterBtn + 1, -127); // x-pos
}
