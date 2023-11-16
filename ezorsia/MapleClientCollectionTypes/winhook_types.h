#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <WS2spi.h>
#include <intrin.h>
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
/// </summary>
typedef HANDLE(WINAPI* CreateFileA_t)(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	);
extern CreateFileA_t CreateFileA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winexec
/// </summary>
typedef UINT(WINAPI* WinExec_t)(LPCSTR lpCmdLine, UINT uCmdShow);
extern WinExec_t WinExec_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
/// </summary>
typedef FARPROC(WINAPI* GetProcAddress_t)(HMODULE hModule, LPCSTR lpProcName);
extern GetProcAddress_t GetProcAddress_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createmutexa
/// </summary>
typedef HANDLE(WINAPI* CreateMutexA_t)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);
extern CreateMutexA_t CreateMutexA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-openmutexw
/// Please note: there is no microsoft doc for OpenMutexA, but OpenMutexW is the same except for the type of string passed (LPCSTR vs LPCWSTR)
/// </summary>
typedef HANDLE(WINAPI* OpenMutexA_t)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
extern OpenMutexA_t OpenMutexA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/ws2spi/nf-ws2spi-wspstartup
/// </summary>
typedef int(WSPAPI* WSPStartup_t)(WORD wVersionRequested, LPWSPDATA lpWSPData, LPWSAPROTOCOL_INFOW lpProtocolInfo, WSPUPCALLTABLE UpcallTable, LPWSPPROC_TABLE lpProcTable);
extern WSPStartup_t WSPStartup_Original;
/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa
/// </summary>
typedef ATOM(WINAPI* RegisterClassExA_t)(const WNDCLASSEXA* lpWc);
extern RegisterClassExA_t RegisterClassExA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw
/// </summary>
typedef BOOL(WINAPI* CreateProcessW_t)(
	LPCWSTR               lpApplicationName,
	LPWSTR                lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL                  bInheritHandles,
	DWORD                 dwCreationFlags,
	LPVOID                lpEnvironment,
	LPCWSTR               lpCurrentDirectory,
	LPSTARTUPINFOW        lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation
	);
extern CreateProcessW_t CreateProcessW_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
/// </summary>
typedef BOOL(WINAPI* CreateProcessA_t)(
	LPCSTR                lpApplicationName,
	LPSTR                 lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL                  bInheritHandles,
	DWORD                 dwCreationFlags,
	LPVOID                lpEnvironment,
	LPCSTR                lpCurrentDirectory,
	LPSTARTUPINFOA        lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation
	);
extern CreateProcessA_t CreateProcessA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess
/// </summary>
typedef HANDLE(WINAPI* OpenProcess_t)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
extern OpenProcess_t OpenProcess_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
/// </summary>
typedef HANDLE(WINAPI* CreateThread_t)(
	LPSECURITY_ATTRIBUTES   lpThreadAttributes,
	SIZE_T                  dwStackSize,
	LPTHREAD_START_ROUTINE  lpStartAddress,
	__drv_aliasesMem LPVOID lpParameter,
	DWORD                   dwCreationFlags,
	LPDWORD                 lpThreadId
	);
extern CreateThread_t CreateThread_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getacp
/// </summary>
typedef UINT(WINAPI* GetACP_t)();
extern GetACP_t GetACP_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
/// </summary>
typedef HWND(WINAPI* CreateWindowExA_t)(
	DWORD     dwExStyle,
	LPCSTR    lpClassName,
	LPCSTR    lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam
	);
extern CreateWindowExA_t CreateWindowExA_Original;

/// <summary>
/// http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FNT%20Objects%2FProcess%2FNtTerminateProcess.html
/// </summary>
typedef LONG(NTAPI* NtTerminateProcess_t)(HANDLE hProcHandle, LONG ntExitStatus);
extern NtTerminateProcess_t NtTerminateProcess_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regcreatekeyexa
/// </summary>
/// <returns></returns>
typedef LSTATUS(WINAPI* RegCreateKeyExA_t)(
	HKEY                        hKey,
	LPCSTR                      lpSubKey,
	DWORD                       Reserved,
	LPSTR                       lpClass,
	DWORD                       dwOptions,
	REGSAM                      samDesired,
	const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY                       phkResult,
	LPDWORD                     lpdwDisposition
	);
extern RegCreateKeyExA_t RegCreateKeyExA_Original;