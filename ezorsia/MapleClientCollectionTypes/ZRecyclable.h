#pragma once
#include "ZRecyclableAvBuffer.h"
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
template <typename Type, size_t Size, typename Value>
class ZRecyclable : protected ZAllocBase
{
protected:
	virtual ~ZRecyclable() = default;

	/* uses ZRecycleAvBuffer for memory management */

	void* operator new(unsigned int uSize)
	{
		return ZRecyclableAvBuffer<Type>::GetInstance()->raw_new();
	}

	void* operator new[](unsigned int uSize)
	{
		return ZRecyclableAvBuffer<Type>::GetInstance()->raw_new();
	}

	void operator delete(void* p)
	{
		ZRecyclableAvBuffer<Type>::GetInstance()->raw_delete(p);
	}

	void operator delete[](void* p)
	{
		ZRecyclableAvBuffer<Type>::GetInstance()->raw_delete(p);
	}
};

//assert_size(sizeof(ZRecyclable<long, 16, long>), 0x4);