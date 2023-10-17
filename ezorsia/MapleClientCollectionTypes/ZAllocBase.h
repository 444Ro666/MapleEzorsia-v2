#pragma once
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
class ZAllocBase
{
public:
	enum BLOCK_SIZE
	{
		BLOCK16 = 0,
		BLOCK32 = 1,
		BLOCK64 = 2,
		BLOCK128 = 3,
	};

	static PVOID* AllocRawBlocks(UINT uBlockSize, UINT uNumberOfBlocks)
	{
		/* TODO make this more legible */

		unsigned int uEnlargedBlockSize = uBlockSize + 4;
		unsigned int uTotalAllocationSize = (uNumberOfBlocks * uEnlargedBlockSize) + 8;

		HANDLE hHeap = GetProcessHeap();
		PVOID* pAlloc = reinterpret_cast<PVOID*>(HeapAlloc(hHeap, NULL, uTotalAllocationSize));

		/* if we deallocate the entire block collection, this first address is where we start */
		*(pAlloc) = reinterpret_cast<PVOID>((uNumberOfBlocks * uEnlargedBlockSize) + 4);

		/* block collection header */
		*(pAlloc + 1) = 0;

		/* size of first block */
		*(pAlloc + 2) = reinterpret_cast<PVOID>(uBlockSize);

		PVOID* pRet = pAlloc + 3;
		DWORD* pdwRet = (DWORD*)(pAlloc + 3);

		for (UINT i = 0; i < uNumberOfBlocks - 1; i++)
		{
			/* initialize each block with a pointer to the next block */
			*pRet = reinterpret_cast<PCHAR>(pRet) + uEnlargedBlockSize;

			/* increase pointer by block size (we divide because the compiler tries to multiply) */
			pRet = reinterpret_cast<PVOID*>(reinterpret_cast<PCHAR>(pRet) + uEnlargedBlockSize);

			/* set the preceding address to equal the size of the block */
			*(pRet - 1) = reinterpret_cast<PVOID>(uBlockSize);
		}

		/* nullptr indicates last block in the linked list */
		*pRet = nullptr;

		/* return address of the first block in the linked list */
		return pAlloc + 3;
	}
};