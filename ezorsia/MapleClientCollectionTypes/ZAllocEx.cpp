#include "stdafx.h"
#include "ZAllocEx.h"
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
void* operator new(size_t uSize)
{
	return ZAllocEx<ZAllocAnonSelector>::GetInstance()->Alloc(uSize);
}

void* operator new[](size_t uSize)
{
	return ZAllocEx<ZAllocAnonSelector>::GetInstance()->Alloc(uSize);
}

void operator delete(void* p)
{
	ZAllocEx<ZAllocAnonSelector>::GetInstance()->Free((void**)p);
}

void operator delete[](void* p)
{
	ZAllocEx<ZAllocAnonSelector>::GetInstance()->Free((void**)p);
}