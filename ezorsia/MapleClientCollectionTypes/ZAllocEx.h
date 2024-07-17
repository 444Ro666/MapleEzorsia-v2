#pragma once
#include "ZXString.h"
#include "ZAllocAnonSelector.h"
#include "ZAllocBase.h"
#include "ZAllocStrSelector.h"
#include <intrin.h>
#include <mutex>
#include <thread>
#include <windows.h>
#include <heapapi.h>
#include "ZFatalSection.h"
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
// fix returnaddress func
// https://docs.microsoft.com/en-us/cpp/intrinsics/returnaddress?view=msvc-160
#pragma intrinsic(_ReturnAddress)

template <typename T> struct ZAllocEx : ZAllocBase, T { }; // dummy base

/// <summary>
/// Exact replica (to the best of my ability) of MapleStory's memory mangement template class.
/// ZAllocEx uses a combination of memory pool and slab allocation for objects up to a certain size (based on pool type). Objects in
/// excess of the pool max limit are allocated and deallocated freely, without the use of the memory caches.
/// When memory is requested from ZAllocEx, it first attempts to provide already allocated memory that is not in use. If none is available,
/// it allocates a chunk of memory that varies in size based on the requested amount and pool type, and then returns a fraction of 
/// the allocated memory to the calling function.
/// When memory is freed through ZAllocEx, it does not get deallocated but instead gets placed on top of an available memory stack to be 
/// used by the next call to the allocator.
/// ZAllocEx has 4 stacks of available memory of different sizes which differ based on the pool type.
/// </summary>
template <>
struct ZAllocEx<ZAllocAnonSelector> : ZAllocBase, ZAllocAnonSelector
{
private:
	BYTE gap0[1];
	ZFatalSection m_lock; // we dont use this but we keep it for proper maple struct alignment
	LPVOID m_apBuff[4];
	LPVOID m_apBlockHead[4];

	std::mutex* GetMutex()
	{
		static std::mutex mtx;

		return &mtx;
	}

	ZAllocEx()
	{
		gap0[0] = 0;

		for (int i = 0; i < 4; i++)
		{
			m_apBuff[i] = nullptr;
			m_apBlockHead[i] = nullptr;
		}
	}

	/* ZAlloc instantiation has to used malloc because it cant initialize itself */
	void* operator new(unsigned int uSize)
	{
		return malloc(uSize);
	}

	void operator delete(void* p)
	{
		free(p);
	}

public:
	static ZAllocEx<ZAllocAnonSelector>* GetInstance()
	{
		static ZAllocEx<ZAllocAnonSelector> _s_ZAllocEx = ZAllocEx();
		return &_s_ZAllocEx;
	}

	PVOID Alloc(size_t uSize)
	{
		BLOCK_SIZE nBlockSizeIndex;

		if (uSize <= 16)
		{
			nBlockSizeIndex = BLOCK16;
		}
		else if (uSize <= 32)
		{
			nBlockSizeIndex = BLOCK32;
		}
		else if (uSize <= 64)
		{
			nBlockSizeIndex = BLOCK64;
		}
		else if (uSize <= 128)
		{
			nBlockSizeIndex = BLOCK128;
		}
		else
		{
			/* get handle to process heap */
			HANDLE hHeap = GetProcessHeap();

			/* allocate an extra pointer to store the allocation size */
			PVOID* lpAlloc = reinterpret_cast<PVOID*>(HeapAlloc(hHeap, NULL, uSize + sizeof(PVOID)));

			/* set allocation size at head of allocation */
			*lpAlloc = reinterpret_cast<PVOID>(uSize);

			return lpAlloc + 1; // return next 32-bit address after the size imprint
		}

		INT nAllocBlocks;
		INT nBlockSize = this->GetBlockSize(nBlockSizeIndex, &nAllocBlocks);

		GetMutex()->lock();

		/* check if theres an available block of memory at the current buffer position */
		/* if not, we allocate another kb of memory blocks */
		if (!this->m_apBuff[nBlockSizeIndex])
		{

			/* returns pointer to the first memory block in the linked list allocation */
			PVOID* pBlockAllocHead = this->AllocRawBlocks(nBlockSize, nAllocBlocks);

			/* the preceding address holds the block size, the address before that holds a pointer to the next block in the linked list series */
			*(pBlockAllocHead - 2) = this->m_apBlockHead[nBlockSizeIndex];

			/* throw the newly allocated memory block list onto the top of the stack */
			this->m_apBlockHead[nBlockSizeIndex] = pBlockAllocHead;
			this->m_apBuff[nBlockSizeIndex] = pBlockAllocHead;
		}

		/* grab the top pointer off the stack */
		PVOID* lpAllocRet = reinterpret_cast<PVOID*>(this->m_apBuff[nBlockSizeIndex]);

//#if _DEBUG
//		if ((DWORD) * (lpAllocRet - 1) > 0x400) // 1kb
//		{
//			auto p = lpAllocRet;
//
//			Log("Memory Dump:");
//			Log("Address[-2]: %08X Value: %08X", p - 2, *(p - 2));
//			Log("Address[-1]: %08X Value: %08X", p - 1, *(p - 1));
//			Log("Address[0]:  %08X Value: %08X", p, *(p));
//			Log("Address[1]:  %08X Value: %08X", p + 1, *(p + 1));
//			Log("Address[2]:  %08X Value: %08X", p + 2, *(p + 2));
//	}
//#endif

		/* set the top of the stack to equal the previous pointer */
		this->m_apBuff[nBlockSizeIndex] = *lpAllocRet;

		GetMutex()->unlock();

		/* return memory */
		return lpAllocRet;
	}

	void Free(void** p)
	{
		BLOCK_SIZE nBlockSizeIndex;

		if (!p) return;

		DWORD uSize = *(DWORD*)(p - 1); // pointer before the mem address holds allocation size

		if (uSize & 0x80000000)
		{
			uSize = ~uSize;
		}

		if (uSize == 16)
		{
			nBlockSizeIndex = BLOCK16;
		}
		else if (uSize == 32)
		{
			nBlockSizeIndex = BLOCK32;
		}
		else if (uSize == 64)
		{
			nBlockSizeIndex = BLOCK64;
		}
		else if (uSize == 128)
		{
			nBlockSizeIndex = BLOCK128;
		}
		else
		{
			HANDLE hHeap = GetProcessHeap(); // get handle to process heap

			LPVOID pMem = p - 1;

			BOOL bRet = HeapFree(hHeap, NULL, pMem); // release memory starting at memory size pointer
//#if _DEBUG
//			if (!bRet)
//			{
//				Log("Error caught when freeing heap memory. Error code: %d", GetLastError());
//
//				Log("Memory Dump:");
//				Log("Address[-2]: %08X Value: %08X", p - 2, *(p - 2));
//				Log("Address[-1]: %08X Value: %08X", p - 1, *(p - 1));
//				Log("Address[0]:  %08X Value: %08X", p, *(p));
//				Log("Address[1]:  %08X Value: %08X", p + 1, *(p + 1));
//				Log("Address[2]:  %08X Value: %08X", p + 2, *(p + 2));
//		}
//#endif
			return;
		}

		GetMutex()->lock();

		/* assign the top block pointer to the head of the freed memory*/
		*p = this->m_apBuff[nBlockSizeIndex];

		/* put the freed memory on top of the available memory stack */
		this->m_apBuff[nBlockSizeIndex] = p;

		GetMutex()->unlock();
	}
};

/// <summary>
/// Exact replica (to the best of my ability) of MapleStory's memory mangement template class.
/// ZAllocEx uses a combination of memory pool and slab allocation for objects up to a certain size (based on pool type). Objects in
/// excess of the pool max limit are allocated and deallocated freely, without the use of the memory caches.
/// When memory is requested from ZAllocEx, it first attempts to provide already allocated memory that is not in use. If none is available,
/// it allocates a chunk of memory that varies in size based on the requested amount and pool type, and then returns a fraction of 
/// the allocated memory to the calling function.
/// When memory is freed through ZAllocEx, it does not get deallocated but instead gets placed on top of an available memory stack to be 
/// used by the next call to the allocator.
/// ZAllocEx has 4 stacks of available memory of different sizes which differ based on the pool type.
/// </summary>
template <typename T>
struct ZAllocEx<ZAllocStrSelector<T>> : ZAllocBase, ZAllocStrSelector<T>
{
private:
	BYTE gap0[1];
	ZFatalSection m_lock; // we dont use this but we keep it for proper maple struct alignment
	LPVOID m_apBuff[4];
	LPVOID m_apBlockHead[4];

	std::mutex* GetMutex()
	{
		static std::mutex mtx;
		return &mtx;
	}

	ZAllocEx()
	{
		gap0[0] = 0;

		for (int i = 0; i < 4; i++)
		{
			m_apBuff[i] = nullptr;
			m_apBlockHead[i] = nullptr;
		}
	}

	/* ZAlloc instantiation has to used malloc because it cant initialize itself */
	void* operator new(unsigned int uSize)
	{
		return malloc(uSize);
	}

	void operator delete(void* p)
	{
		free(p);
	}

public:
	static ZAllocEx<ZAllocStrSelector<T>>* GetInstance()
	{
		static ZAllocEx<ZAllocStrSelector<T>> _s_ZAllocEx = ZAllocEx();
		return &_s_ZAllocEx;
	}

	PVOID Alloc(size_t uSize)
	{
		BLOCK_SIZE nBlockSizeIndex;

		if (uSize <= (sizeof(T) * 16) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK16;
		}
		else if (uSize <= (sizeof(T) * 32) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK32;
		}
		else if (uSize <= (sizeof(T) * 64) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK64;
		}
		else if (uSize <= (sizeof(T) * 128) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK128;
		}
		else
		{
			/* get handle to process heap */
			HANDLE hHeap = GetProcessHeap();

			/* allocate an extra pointer to store the allocation size */
			PVOID* lpAlloc = reinterpret_cast<PVOID*>(HeapAlloc(hHeap, NULL, uSize + sizeof(PVOID)));

			/* set allocation size at head of allocation */
			*lpAlloc = reinterpret_cast<PVOID>(uSize);

			return lpAlloc + 1; // return next 32-bit address after the size imprint
		}

		INT nAllocBlocks;
		INT nBlockSize = this->GetBlockSize(nBlockSizeIndex, &nAllocBlocks);

		GetMutex()->lock();

		/* check if theres an available block of memory at the current buffer position */
		/* if not, we allocate another kb of memory blocks */
		if (!this->m_apBuff[nBlockSizeIndex])
		{
			/* returns pointer to the first memory block in the linked list allocation */
			PVOID* pBlockAllocHead = this->AllocRawBlocks(nBlockSize, nAllocBlocks);

			/* the preceding address holds the block size, the address before that holds a pointer to the next block in the linked list series */
			*(pBlockAllocHead - 2) = this->m_apBlockHead[nBlockSizeIndex];

			/* throw the newly allocated memory block list onto the top of the stack */
			this->m_apBlockHead[nBlockSizeIndex] = pBlockAllocHead;
			this->m_apBuff[nBlockSizeIndex] = pBlockAllocHead;
		}

		/* grab the top pointer off the stack */
		PVOID* lpAllocRet = reinterpret_cast<PVOID*>(this->m_apBuff[nBlockSizeIndex]);

//#if _DEBUG
//		if ((DWORD) * (lpAllocRet - 1) > 0x400) // 1kb
//		{
//			auto p = lpAllocRet;
//
//			Log("Memory Dump:");
//			Log("Address[-2]: %08X Value: %08X", p - 2, *(p - 2));
//			Log("Address[-1]: %08X Value: %08X", p - 1, *(p - 1));
//			Log("Address[0]:  %08X Value: %08X", p, *(p));
//			Log("Address[1]:  %08X Value: %08X", p + 1, *(p + 1));
//			Log("Address[2]:  %08X Value: %08X", p + 2, *(p + 2));
//	}
//#endif

		/* set the top of the stack to equal the previous pointer */
		this->m_apBuff[nBlockSizeIndex] = *lpAllocRet;

		GetMutex()->unlock();

		/* return memory */
		return lpAllocRet;
	}

	void Free(void** p)
	{
		BLOCK_SIZE nBlockSizeIndex;

		if (!p) return;

		DWORD uSize = *(DWORD*)(p - 1); // pointer before the mem address holds allocation size

		if (uSize & 0x80000000)
		{
			uSize = ~uSize;
		}

		if (uSize == (sizeof(T) * 16) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK16;
		}
		else if (uSize == (sizeof(T) * 32) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK32;
		}
		else if (uSize == (sizeof(T) * 64) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK64;
		}
		else if (uSize == (sizeof(T) * 128) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T))
		{
			nBlockSizeIndex = BLOCK128;
		}
		else
		{
			HANDLE hHeap = GetProcessHeap(); // get handle to process heap

			BOOL bRet = HeapFree(hHeap, NULL, p - 1); // release memory starting at memory size pointer
//#if _DEBUG
//			if (!bRet)
//			{
//				Log("Error caught when freeing heap memory. Error code: %d", GetLastError());
//
//				Log("Memory Dump:");
//				Log("Address[-2]: %08X Value: %08X", p - 2, *(p - 2));
//				Log("Address[-1]: %08X Value: %08X", p - 1, *(p - 1));
//				Log("Address[0]:  %08X Value: %08X", p, *(p));
//				Log("Address[1]:  %08X Value: %08X", p + 1, *(p + 1));
//				Log("Address[2]:  %08X Value: %08X", p + 2, *(p + 2));
//		}
//#endif
			return;
		}

		GetMutex()->lock();

		/* assign the top block pointer to the head of the freed memory*/
		*p = this->m_apBuff[nBlockSizeIndex];

		/* put the freed memory on top of the available memory stack */
		this->m_apBuff[nBlockSizeIndex] = p;

		GetMutex()->unlock();
	}
};

/* Global memory management overloading */

void* operator new(size_t uSize);
void* operator new[](size_t uSize);
void operator delete(void* p);
void operator delete[](void* p);

//assert_size(sizeof(ZAllocEx<ZAllocAnonSelector>), 0x2C)
//assert_size(sizeof(ZAllocEx<ZAllocStrSelector<char>>), 0x2C)
//assert_size(sizeof(ZAllocEx<ZAllocStrSelector<wchar_t>>), 0x2C)
