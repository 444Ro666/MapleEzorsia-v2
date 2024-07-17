#pragma once
#include "ZAllocBase.h"
#include "ZXString.h"
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
/* forward declare zxstring for compiler happiness */
template <typename T> class ZXString;

template <typename T>
struct ZAllocStrSelector
{
protected:
	unsigned int GetBlockSize(ZAllocBase::BLOCK_SIZE nIndex, int* nAllocBlocks)
	{
		switch (nIndex)
		{
		case ZAllocBase::BLOCK_SIZE::BLOCK16:
			*nAllocBlocks = 64;
			return (sizeof(T) * 16) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T);
		case ZAllocBase::BLOCK_SIZE::BLOCK32:
			*nAllocBlocks = 32;
			return (sizeof(T) * 32) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T);
		case ZAllocBase::BLOCK_SIZE::BLOCK64:
			*nAllocBlocks = 16;
			return (sizeof(T) * 64) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T);
		case ZAllocBase::BLOCK_SIZE::BLOCK128:
			*nAllocBlocks = 8;
			return (sizeof(T) * 128) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T);
		default:
			*nAllocBlocks = 0;
			return 0;
		}
	}
};
