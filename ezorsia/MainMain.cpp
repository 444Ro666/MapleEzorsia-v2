#include "stdafx.h"
#include "INIReader.h"
#include <filesystem>
#include "resource.h"

INIReader reader("config.ini");
MainMain* MainMain::_s_pInstance;
FILE* MainMain::stream;
bool MainMain::ownLoginFrame = false;
bool MainMain::CustomLoginFrame = false;
bool MainMain::bigLoginFrame = false;
bool MainMain::ownCashShopFrame = false;
bool MainMain::EzorsiaV2WzIncluded = false;
bool MainMain::useV62_ExpTable = false;
SOCKET MainMain::m_GameSock = INVALID_SOCKET;
WSPPROC_TABLE MainMain::m_ProcTable = { 0 };//to set your max level you have to go to the function rewrite
const char* MainMain::m_sRedirectIP = "127.0.0.1";//paste your custom exp table's array into the place of v83ArrayForCustomEXP //custom exp table and level numbers MUST MATCH server numbers
unsigned int MainMain::v83ArrayForCustomEXP[] = { 15, 15, 34, 57, 92, 135, 372, 560, 840, 1144, 1242, 1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144, 11120, 13477, 16268, 19320, 22880, 27008, 31477, 36600, 42444, 48720, 55813, 63800, 86784, 98208, 110932, 124432, 139372, 155865, 173280, 192400, 213345, 235372, 259392, 285532, 312928, 342624, 374760, 408336, 445544, 483532, 524160, 567772, 598886, 631704, 666321, 702836, 741351, 781976, 824828, 870028, 917625, 967995, 1021041, 1076994, 1136013, 1198266, 1263930, 1333194, 1406252, 1483314, 1564600, 1650340, 1740778, 1836173, 1936794, 2042930, 2154882, 2272970, 2397528, 2528912, 2667496, 2813674, 2967863, 3130502, 3302053, 3483005, 3673873, 3875201, 4087562, 4311559, 4547832, 4797053, 5059931, 5337215, 5629694, 5938202, 6263614, 6606860, 6968915, 7350811, 7753635, 8178534, 8626718, 9099462, 9598112, 10124088, 10678888, 11264090, 11881362, 12532461, 13219239, 13943653, 14707765, 15513750, 16363902, 17260644, 18206527, 19204245, 20256637, 21366700, 22537594, 23772654, 25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428273, 38424542, 40530206, 42751262, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344, 69100311, 72887008, 76881216, 81094306, 85594273, 90225770, 95170142, 100385466, 105886589, 111689174, 117809740, 124265714, 131075474, 138258410, 145834970, 153826726, 162256430, 171148082, 180526997, 190419876, 200854885, 211861732, 223471711, 223471711, 248635353, 262260570, 276632449, 291791906, 307782102, 324648562, 342439302, 361204976, 380999008, 401877754, 423900654, 447130410, 471633156, 497478653, 524740482, 553496261, 583827855, 615821622, 649568646, 685165008, 722712050, 762316670, 804091623, 848155844, 894634784, 943660770, 995373379, 1049919840, 1107455447, 1168144006, 1232158297, 1299680571, 1370903066, 1446028554, 1525246918, 1608855764, 1697021059 };
unsigned int MainMain::v62ArrayForCustomEXP[] = { 0, 15, 34, 57, 92, 135, 372, 560, 840, 1242, 1716, 2360, 3216, 4200, 5460, 7050, 8840, 11040, 13716, 16680, 20216, 24402, 28980, 34320, 40512, 47216, 54900, 63666, 73080, 83720, 95700, 108480, 122760, 138666, 155540, 174216, 194832, 216600, 240500, 266682, 294216, 324240, 356916, 391160, 428280, 468450, 510420, 555680, 604416, 655200, 709716, 748608, 789631, 832902, 878545, 926689, 977471, 1031036, 1087536, 1147032, 1209994, 1276301, 1346242, 1420016, 1497832, 1579913, 1666492, 1757815, 1854143, 1955750, 2062925, 2175973, 2295216, 2420993, 2553663, 2693603, 2841212, 2996910, 3161140, 3334370, 3517093, 3709829, 3913127, 4127566, 4353756, 4592341, 4844001, 5109452, 5389449, 5684790, 5996316, 6324914, 6671519, 7037118, 7422752, 7829518, 8258575, 8711144, 9188514, 9692044, 10223168, 10783397, 11374327, 11997640, 12655110, 13348610, 14080113, 14851703, 15665576, 16524049, 17429566, 18384706, 19392187, 20454878, 21575805, 22758159, 24005306, 25320796, 26708375, 28171993, 29715818, 31344244, 33061908, 34873700, 36784778, 38800583, 40926854, 43169645, 45535341, 48030677, 50662758, 53439077, 56367538, 59456479, 62714694, 66151459, 69776558, 73600313, 77633610, 81887931, 86375389, 91108760, 96101520, 101367883, 106992842, 112782213, 118962678, 125481832, 132358236, 139611467, 147262175, 155332142, 163844343, 172823012, 182293713, 192283408, 202820538, 213935103, 225658746, 238024845, 251068606, 264827165, 279339639, 294647508, 310794191, 327825712, 345790561, 364739883, 384727628, 405810702, 428049128, 451506220, 476248760, 502347192, 529875818, 558913012, 589541445, 621848316, 655925603, 691870326, 729784819, 769777027, 811960808, 856456260, 903390063, 952895838, 1005114529, 1060194805, 1118293480, 1179575962, 1244216724, 1312399800, 1384319309, 1460180007, 1540197871, 1624600714, 1713628833, 1807535693, 1906558648, 2011069705, 2121276324 };
unsigned int MainMain::expTableMemSize = 804;//memory size of array for expTable, it is # of elements in array times data structure size (4 for int)
const char* MainMain::use_custom_dll_1 = "CUSTOM.dll";
const char* MainMain::use_custom_dll_2 = "CUSTOM2.dll";
const char* MainMain::use_custom_dll_3 = "CUSTOM3.dll";
bool MainMain::usingEzorsiaV2Wz = false;
HANDLE MainMain::mainTHread;

MainMain::MainMain(std::function<void()> pPostMutexFunc)
{
	std::filesystem::path BfilePath("Base.wz"); std::filesystem::path BfilePath2("Data/zmap.img"); std::filesystem::path filePath("config.ini"); //check if needed stuff exists
	if (std::filesystem::exists(filePath) && reader.ParseError()) { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"your config.ini file cannot be properly read, go to troubleshooting section of Ezorsia v2 setup guide at https://github.com/444Ro666/MapleEzorsia-v2 for more details, or delete your config.ini to have a new one generated with default settings", L"bad config file", 0); ExitProcess(0); }
	else if (!std::filesystem::exists(filePath)) {
		HANDLE hOrg = CreateFileA("config.ini", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL); DWORD dw;
		if (hOrg) {
			HMODULE hModule = GetModuleHandle(L"dinput8.dll"); // Get handle to current DLL
			HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
			HGLOBAL hResourceData = LoadResource(hModule, hResource);
			DWORD resourceSize = SizeofResource(hModule, hResource);
			LPVOID resourceData = LockResource(hResourceData); Sleep(2); SuspendThread(MainMain::mainTHread);
			if (WriteFile(hOrg, resourceData, resourceSize, &dw, NULL) && dw == resourceSize) { 
				CloseHandle(hOrg);
				ResumeThread(MainMain::mainTHread);
			}
			else { 
				CloseHandle(hOrg);
				MessageBox(NULL, L"your config.ini file doesn't exist, please re-download config.ini from Ezorsia v2 releases at https://github.com/444Ro666/MapleEzorsia-v2", L"bad config file", 0);
				ExitProcess(0);
			}
		}
	}
	if (!std::filesystem::exists(BfilePath) && !std::filesystem::exists(BfilePath2)) { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"Either Base.wz is missing from your game directory OR you are loading from .img and zmap.img is not in your Data directory, please reinstall and make sure relevant file(s) exist", L"missing .wz/.img file", 0); ExitProcess(0); }
	MainMain::CustomLoginFrame = reader.GetBoolean("optional", "CustomLoginFrame", false);
	if (MainMain::CustomLoginFrame) { MainMain::ownLoginFrame = true; MainMain::bigLoginFrame = true; } //use own login if true
	std::filesystem::path EfilePath("EzorsiaV2_UI.wz");	//support for other non-big frame users (i.e. ezorsia-like, with login centered, but different frame, isnt currently supported)
	std::filesystem::path EfilePath2("Data/MapleEzorsiaV2wzfiles.img");
	if (std::filesystem::exists(EfilePath)) { 	//only check after "if false" on Client::CustomLoginFrame or things break
		MainMain::EzorsiaV2WzIncluded = true; MainMain::CustomLoginFrame = true; MainMain::usingEzorsiaV2Wz = true; }
	else if(std::filesystem::exists(EfilePath2)){ MainMain::EzorsiaV2WzIncluded = true; MainMain::CustomLoginFrame = true; }
	else {
		if (std::filesystem::exists(BfilePath)) {
			HANDLE hOrg = CreateFileA("EzorsiaV2_UI.wz", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL); DWORD dw;
			if (hOrg) {
				HMODULE hModule = GetModuleHandle(L"dinput8.dll"); // Get handle to current DLL
				HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_RCDATA2), RT_RCDATA);
				HGLOBAL hResourceData = LoadResource(hModule, hResource);
				DWORD resourceSize = SizeofResource(hModule, hResource);
				LPVOID resourceData = LockResource(hResourceData); Sleep(2); SuspendThread(MainMain::mainTHread);
				if (WriteFile(hOrg, resourceData, resourceSize, &dw, NULL) && dw == resourceSize) {
					CloseHandle(hOrg);
					ResumeThread(MainMain::mainTHread);
				}
				else {
					CloseHandle(hOrg);
					MessageBox(NULL, L"your EzorsiaV2_UI.wz file doesn't exist, please re-download EzorsiaV2_UI.wz from Ezorsia v2 releases at https://github.com/444Ro666/MapleEzorsia-v2", L"bad WZ file", 0);
					ExitProcess(0);
				}
			}
			MainMain::EzorsiaV2WzIncluded = true; MainMain::CustomLoginFrame = true; MainMain::usingEzorsiaV2Wz = true;
		}
		if (std::filesystem::exists(BfilePath2)) {
			HANDLE hOrg = CreateFileA("Data\\MapleEzorsiaV2wzfiles.img", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL); DWORD dw;
			if (hOrg) {
				HMODULE hModule = GetModuleHandle(L"dinput8.dll"); // Get handle to current DLL
				HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_RCDATA3), RT_RCDATA);
				HGLOBAL hResourceData = LoadResource(hModule, hResource);
				DWORD resourceSize = SizeofResource(hModule, hResource);
				LPVOID resourceData = LockResource(hResourceData); Sleep(2); SuspendThread(MainMain::mainTHread);
				if (WriteFile(hOrg, resourceData, resourceSize, &dw, NULL) && dw == resourceSize) {
					CloseHandle(hOrg);
					ResumeThread(MainMain::mainTHread);
				}
				else {
					CloseHandle(hOrg);
					MessageBox(NULL, L"Ezorsia V2 has detected you are loading from .img, but that your MapleEzorsiaV2wzfiles.img file doesn't exist in your Data folder, please re-download MapleEzorsiaV2wzfiles.img from Ezorsia v2 releases at https://github.com/444Ro666/MapleEzorsia-v2", L"bad IMG file", 0);
					ExitProcess(0);
				}
			}
			MainMain::EzorsiaV2WzIncluded = true; MainMain::CustomLoginFrame = true;
		}
	}
	//Memory::UseVirtuProtect = reader.GetBoolean("general", "UseVirtuProtect", true);//breaks without it so i removed the option, too many options anyway and this wasnt helping anyone
	Client::m_nGameWidth = reader.GetInteger("general", "width", 1280);
	Client::m_nGameHeight = reader.GetInteger("general", "height", 720);
	Client::MsgAmount = reader.GetInteger("general", "MsgAmount", 26);
	Client::WindowedMode = reader.GetBoolean("general", "WindowedMode", true);
	Client::RemoveLogos = reader.GetBoolean("general", "RemoveLogos", true);
	Client::setDamageCap = reader.GetReal("optional", "setDamageCap", 199999.0);
	Client::useTubi = reader.GetBoolean("optional", "useTubi", false);
	Client::speedMovementCap = reader.GetInteger("optional", "speedMovementCap", 140);
	Client::ServerIP_AddressFromINI = reader.Get("general", "ServerIP_Address", "127.0.0.1");
	MainMain::ownCashShopFrame = reader.GetBoolean("optional", "ownCashShopFrame", false);
	MainMain::useV62_ExpTable = reader.GetBoolean("optional", "useV62_ExpTable", false);
	const char* serverIP_Address = Client::ServerIP_AddressFromINI.c_str();
	MainMain::m_sRedirectIP = serverIP_Address; unsigned int sleepySleepy = reader.GetInteger("debug", "sleepTime", 0);
	std::string use_custom_dll_1s = reader.Get("optional", "use_custom_dll_1", "CUSTOM.dll");
	std::string use_custom_dll_2s = reader.Get("optional", "use_custom_dll_2", "CUSTOM2.dll");
	std::string use_custom_dll_3s = reader.Get("optional", "use_custom_dll_3", "CUSTOM3.dll");
	const char* use_custom_dll_1c = use_custom_dll_1s.c_str(); const char* use_custom_dll_2c = use_custom_dll_2s.c_str(); const char* use_custom_dll_3c = use_custom_dll_3s.c_str();
	MainMain::use_custom_dll_1 = use_custom_dll_1c; MainMain::use_custom_dll_2 = use_custom_dll_2c; MainMain::use_custom_dll_3 = use_custom_dll_3c;

	//Sleep is for the unpacked client run //themida needs a slight amount of time to unpack or exe memory will be corrupted and crash. from my 
//tests the amount of time to sleep for ranges from 80 to 300 miliseconds, but may differ depending on the machine; most modern pc are pretty 
//fast so favor lower numbers. if the number is too high the non-system hooks wont reach the main thread in time and the main thread will
	if (sleepySleepy != 0) { Sleep(sleepySleepy); }//run without them (causing hackshield load that is normally overwritten and hooked in time to block access)

	if (strcmp(MainMain::use_custom_dll_1, "CUSTOM.dll")) { //load auxilliary dll files
		HMODULE hModule = LoadLibraryA(MainMain::use_custom_dll_1);
		if (!hModule) { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"Failed to find the first custom dll file", L"Missing file", 0); ExitProcess(0); }
	}
	if (strcmp(MainMain::use_custom_dll_2, "CUSTOM2.dll")) {
		HMODULE hModule1 = LoadLibraryA(MainMain::use_custom_dll_2);
		if (!hModule1) { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"Failed to find the second custom dll file", L"Missing file", 0); ExitProcess(0); }
	}
	if (strcmp(MainMain::use_custom_dll_3, "CUSTOM3.dll")) {
		HMODULE hModule2 = LoadLibraryA(MainMain::use_custom_dll_3);
		if (!hModule2) { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"Failed to find the third custom dll file", L"Missing file", 0); ExitProcess(0); }
	}

	pPostMutexFunc(); //can't reach my functions from this class =(
}	//and if i try to #include "ReplacementFuncs.h" here it says a lot of things are defined twice

MainMain::~MainMain()
{
	if (MainMain::m_GameSock != INVALID_SOCKET)
	{//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		MainMain::m_ProcTable.lpWSPCloseSocket(m_GameSock, nullptr);
		MainMain::m_GameSock = INVALID_SOCKET;
	}
}
//old config stuff:
	//;Should VirtuProtect be enabled ? (true / false, default true) (this is necessary for clients without CRC bypass such as hendi's clients
	//	;that havent been editted further, can be turned off for clients with CRC bypass to save whatever processing power it uses)
	//	UseVirtuProtect = true