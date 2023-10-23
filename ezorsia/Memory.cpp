#include "stdafx.h"
#include "Memory.h"
#include "detours.h"

bool Memory::UseVirtuProtect = true;

bool Memory::SetHook(bool attach, void** ptrTarget, void* ptrDetour)
{
    if (DetourTransactionBegin() != NO_ERROR)
    {
        return false;
    }

    HANDLE pCurThread = GetCurrentThread();

    if (DetourUpdateThread(pCurThread) == NO_ERROR)
    {
        auto pDetourFunc = attach ? DetourAttach : DetourDetach;

        if (pDetourFunc(ptrTarget, ptrDetour) == NO_ERROR)
        {
            if (DetourTransactionCommit() == NO_ERROR)
            {
                return true;
            }
        }
    }

    DetourTransactionAbort();
    return false;
}

void Memory::FillBytes(const DWORD dwOriginAddress, const unsigned char ucValue, const int nCount) {
    if (UseVirtuProtect) {
        DWORD dwOldProtect;
        VirtualProtect((LPVOID)dwOriginAddress, nCount, PAGE_EXECUTE_READWRITE, &dwOldProtect); //thanks colaMint, joo, and stelmo for informing me of using virtualprotect
        memset((void*)dwOriginAddress, ucValue, nCount);
        VirtualProtect((LPVOID)dwOriginAddress, nCount, dwOldProtect, &dwOldProtect);
    }
    else { memset((void*)dwOriginAddress, ucValue, nCount); }
}

void Memory::WriteString(const DWORD dwOriginAddress, const char* sContent) {
    const size_t nSize = strlen(sContent);
    if (UseVirtuProtect) {
        DWORD dwOldProtect;
        VirtualProtect((LPVOID)dwOriginAddress, nSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
        memcpy((void*)dwOriginAddress, sContent, nSize);
        VirtualProtect((LPVOID)dwOriginAddress, nSize, dwOldProtect, &dwOldProtect);
    }
    else { memcpy((void*)dwOriginAddress, sContent, nSize); }
}

void Memory::WriteByte(const DWORD dwOriginAddress, const unsigned char ucValue) {
    if (UseVirtuProtect) {
        DWORD dwOldProtect;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(unsigned char), PAGE_EXECUTE_READWRITE, &dwOldProtect);
        *(unsigned char*)dwOriginAddress = ucValue;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(unsigned char), dwOldProtect, &dwOldProtect);
    }
    else { *(unsigned char*)dwOriginAddress = ucValue; }
}

void Memory::WriteShort(const DWORD dwOriginAddress, const unsigned short usValue) {
    if (UseVirtuProtect) {
        DWORD dwOldProtect;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(unsigned short), PAGE_EXECUTE_READWRITE, &dwOldProtect);
        *(unsigned short*)dwOriginAddress = usValue;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(unsigned short), dwOldProtect, &dwOldProtect);
    }
    else { *(unsigned short*)dwOriginAddress = usValue; }
}

void Memory::WriteInt(const DWORD dwOriginAddress, const unsigned int dwValue) {
    if (UseVirtuProtect) {
        DWORD dwOldProtect;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(unsigned int), PAGE_EXECUTE_READWRITE, &dwOldProtect);
        *(unsigned int*)dwOriginAddress = dwValue;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(unsigned int), dwOldProtect, &dwOldProtect);
    }
    else { *(unsigned int*)dwOriginAddress = dwValue; }
}

void Memory::WriteDouble(const DWORD dwOriginAddress, const double dwValue) {
    if (UseVirtuProtect) {
        DWORD dwOldProtect;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(double), PAGE_EXECUTE_READWRITE, &dwOldProtect);
        *(double*)dwOriginAddress = dwValue;
        VirtualProtect((LPVOID)dwOriginAddress, sizeof(double), dwOldProtect, &dwOldProtect);
    }
    else { *(double*)dwOriginAddress = dwValue; }
}

void Memory::WriteByteArray(const DWORD dwOriginAddress, unsigned char* ucValue, const int ucValueSize) {
    if (UseVirtuProtect) {
        for (int i = 0; i < ucValueSize; i++) {
            const DWORD newAddr = dwOriginAddress + i;
            DWORD dwOldProtect;
            VirtualProtect((LPVOID)newAddr, sizeof(unsigned char), PAGE_EXECUTE_READWRITE, &dwOldProtect);
            *(unsigned char*)newAddr = ucValue[i];
            VirtualProtect((LPVOID)newAddr, sizeof(unsigned char), dwOldProtect, &dwOldProtect);
        }
    }
    else {
        for (int i = 0; i < ucValueSize; i++) { const DWORD newAddr = dwOriginAddress + i; *(unsigned char*)newAddr = ucValue[i]; }
    }
}

void Memory::CodeCave(void* ptrCodeCave, const DWORD dwOriginAddress, const int nNOPCount) { //tested and working
	__try {
		if (nNOPCount) FillBytes(dwOriginAddress, 0x90, nNOPCount); // create space for the jmp
		WriteByte(dwOriginAddress, 0xe9); // jmp instruction
		WriteInt(dwOriginAddress + 1, (int)(((int)ptrCodeCave - (int)dwOriginAddress) - 5)); // [jmp(1 byte)][address(4 bytes)] //this means you need to clear a space of at least 5 bytes (nNOPCount bytes)
	} __except (EXCEPTION_EXECUTE_HANDLER) {}
}
