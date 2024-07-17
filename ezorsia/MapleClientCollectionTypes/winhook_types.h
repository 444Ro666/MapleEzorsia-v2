#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <WS2spi.h>
#include <intrin.h>
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
/// </summary>
using CreateFileA_t = HANDLE(WINAPI*)(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);
extern CreateFileA_t CreateFileA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winexec
/// </summary>
using WinExec_t = UINT(WINAPI*)(LPCSTR lpCmdLine, UINT uCmdShow);
extern WinExec_t WinExec_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
/// </summary>
using GetProcAddress_t = FARPROC(WINAPI*)(HMODULE hModule, LPCSTR lpProcName);
extern GetProcAddress_t GetProcAddress_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createmutexa
/// </summary>
using CreateMutexA_t = HANDLE(WINAPI*)(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);
extern CreateMutexA_t CreateMutexA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-openmutexw
/// Please note: there is no microsoft doc for OpenMutexA, but OpenMutexW is the same except for the type of string passed (LPCSTR vs LPCWSTR)
/// </summary>
using OpenMutexA_t = HANDLE(WINAPI*)(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName);
extern OpenMutexA_t OpenMutexA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/ws2spi/nf-ws2spi-wspstartup
/// </summary>
using WSPStartup_t = int(WSPAPI*)(WORD wVersionRequested, LPWSPDATA lpWSPData, LPWSAPROTOCOL_INFOW lpProtocolInfo, WSPUPCALLTABLE UpcallTable, LPWSPPROC_TABLE lpProcTable);
extern WSPStartup_t WSPStartup_Original;
/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexa
/// </summary>
using RegisterClassExA_t = ATOM(WINAPI*)(const WNDCLASSEXA* lpWc);
extern RegisterClassExA_t RegisterClassExA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw
/// </summary>
using CreateProcessW_t = BOOL(WINAPI*)(
	LPCWSTR lpApplicationName,
	LPWSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCWSTR lpCurrentDirectory,
	LPSTARTUPINFOW lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation
	);
extern CreateProcessW_t CreateProcessW_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
/// </summary>
using CreateProcessA_t = BOOL(WINAPI*)(
	LPCSTR lpApplicationName,
	LPSTR lpCommandLine,
	LPSECURITY_ATTRIBUTES lpProcessAttributes,
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	BOOL bInheritHandles,
	DWORD dwCreationFlags,
	LPVOID lpEnvironment,
	LPCSTR lpCurrentDirectory,
	LPSTARTUPINFOA lpStartupInfo,
	LPPROCESS_INFORMATION lpProcessInformation
	);
extern CreateProcessA_t CreateProcessA_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocess
/// </summary>
using OpenProcess_t = HANDLE(WINAPI*)(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId);
extern OpenProcess_t OpenProcess_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
/// </summary>
using CreateThread_t = HANDLE(WINAPI*)(
	LPSECURITY_ATTRIBUTES lpThreadAttributes,
	SIZE_T dwStackSize,
	LPTHREAD_START_ROUTINE lpStartAddress,
	__drv_aliasesMem LPVOID lpParameter,
	DWORD dwCreationFlags,
	LPDWORD lpThreadId
	);
extern CreateThread_t CreateThread_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winnls/nf-winnls-getacp
/// </summary>
using GetACP_t = UINT(WINAPI*)();
extern GetACP_t GetACP_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
/// </summary>
using CreateWindowExA_t = HWND(WINAPI*)(
	DWORD dwExStyle,
	LPCSTR lpClassName,
	LPCSTR lpWindowName,
	DWORD dwStyle,
	int X,
	int Y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam
	);
extern CreateWindowExA_t CreateWindowExA_Original;

/// <summary>
/// http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FNT%20Objects%2FProcess%2FNtTerminateProcess.html
/// </summary>
using NtTerminateProcess_t = LONG(NTAPI*)(HANDLE hProcHandle, LONG ntExitStatus);
extern NtTerminateProcess_t NtTerminateProcess_Original;

/// <summary>
/// https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regcreatekeyexa
/// </summary>
/// <returns></returns>
using RegCreateKeyExA_t = LSTATUS(WINAPI*)(
	HKEY hKey,
	LPCSTR lpSubKey,
	DWORD Reserved,
	LPSTR lpClass,
	DWORD dwOptions,
	REGSAM samDesired,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	PHKEY phkResult,
	LPDWORD lpdwDisposition
	);
extern RegCreateKeyExA_t RegCreateKeyExA_Original;
