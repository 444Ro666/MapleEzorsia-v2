#pragma once
class Client
{
public:
	static void UpdateGameStartup();
	static void EnableNewIGCipher();
	static void UpdateResolution();
	static void UpdateLogin();
	static constexpr int m_nIGCipherHash = 0XC65053F2;
	static int m_nGameHeight;
	static int m_nGameWidth;
	static int MsgAmount;
	static bool WindowedMode;
	static bool RemoveLogos;
	static double setDamageCap;
	static bool useTubi;
	static int speedMovementCap;
	static std::string ServerIP_AddressFromINI;
};
