#pragma once
#include "ZRefCountedAccessor.h"
#include "ZRefCountedDummy.h"
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
/// <summary>
/// Emulation of MapleStory's implementation of a doubly linked list template.
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class ZList : ZRefCountedAccessor<T>, ZRefCountedAccessor<ZRefCountedDummy<T>>
{
#define ZLIST_INVALID_INDEX -1

private:
	char gap4[1];
	size_t m_uCount;
	T* m_pHead;
	T* m_pTail;
public:

	/***=========== CONSTRUCTORS ===========***/

	ZList()
	{
		this->gap4[0] = 0;
		this->m_uCount = 0;
		this->m_pHead = nullptr;
		this->m_pTail = nullptr;
	}

	ZList(ZList<T>* l)
	{
		this->gap4[0] = 0;
		this->m_uCount = 0;
		this->m_pHead = nullptr;
		this->m_pTail = nullptr;

		this->RemoveAll();
		this->AddTail(l);
	}

	virtual ~ZList()
	{
		this->RemoveAll();
	}

	/***=========== OPERATOR OVERLOAD ===========***/

	ZList<T>* operator=(ZList<T>* l)
	{
		this->RemoveAll();
		this->AddTail(l);

		return this;
	}

	/***=========== MEMBER VARIABLE ACCESSORS ===========***/

	T* GetHeadPosition()
	{
		return this->m_pHead;
	}

	T* GetTailPosition()
	{
		return this->m_pTail;
	}

	size_t GetCount()
	{
		return this->m_uCount;
	}

	/***=========== ADD HEAD ===========***/

	/* TODO fix: doesnt work hehe */

	//T* AddHead()
	//{
	//	T* pAlloc = this->New(nullptr, this->m_pHead);

	//	if (this->m_pTail)
	//	{
	//		ZRefCountedDummy<T>* pNode = pAlloc ? this->CastNode(pAlloc) : nullptr;
	//		ZRefCountedDummy<T>* pHeadNode = this->CastNode(this->m_pHead);

	//		pHeadNode->m_pPrev = pNode;
	//		this->m_pHead = pAlloc;
	//	}
	//	else
	//	{
	//		this->m_pHead = pAlloc;
	//		this->m_pTail = pAlloc;
	//	}

	//	return pAlloc;
	//}

	//T* AddHead(T* d)
	//{
	//	T* pNewHead = this->AddHead();
	//	*pNewHead = *d;
	//	return pNewHead;
	//}

	//T* AddHead(ZList<T>* l)
	//{
	//	return nullptr; // TODO
	//}

	/***=========== ADD TAIL ===========***/

	T* AddTail()
	{
		T* pAlloc = this->New(this->m_pTail, nullptr);

		if (this->m_pTail)
		{
			ZRefCountedDummy<T>* pNode = pAlloc ? this->CastNode(pAlloc) : nullptr;
			ZRefCountedDummy<T>* pTailNode = this->CastNode(this->m_pTail);

			pTailNode->m_pNext = pNode;
			this->m_pTail = pAlloc;
		}
		else
		{
			this->m_pHead = pAlloc;
			this->m_pTail = pAlloc;
		}

		return pAlloc;
	}

	T* AddTail(T* d)
	{
		T* pNewTail = this->AddTail();
		*pNewTail = *d;
		return pNewTail;
	}

	void AddTail(ZList<T>* l) // TODO test this, currently untested and prolly not working
	{
		T* pHead = l->m_pHead;

		while (pHead)
		{
			T* pNext = pHead;

			ZRefCountedDummy<T>* pNode = this->CastNode(pHead);
			ZRefCountedDummy<T>* pNodePrev = reinterpret_cast<ZRefCountedDummy<T>*>(pNode->m_pPrev);

			pHead = pNodePrev ? &pNodePrev->t : reinterpret_cast<T*>(nullptr);

			T* pNew = this->AddTail();
			*pNew = *pNext;
		}
	}

	/***=========== NODE REMOVAL ===========***/

	void RemoveAll()
	{
		T* pPosition = this->GetHeadPosition();

		while (pPosition)
		{
			T* pItem = this->GetNext(&pPosition);

			delete this->CastNode(pItem); // IMPORTANT: must delete the node, not the wrapped object (T)
		}

		this->m_pTail = nullptr;
		this->m_pHead = nullptr;
		this->m_uCount = 0;
	}

	void RemoveAt(T* pos)
	{
		ZRefCountedDummy<T>* pNodeDelete = pos ? this->CastNode(pos) : nullptr;

		if (pNodeDelete && pNodeDelete->m_pPrev)
		{
			ZRefCountedDummy<T>* pPrevNode = reinterpret_cast<ZRefCountedDummy<T>*>(pNodeDelete->m_pPrev);

			if (pNodeDelete->m_pNext)
			{
				ZRefCountedDummy<T>* pNextNode = reinterpret_cast<ZRefCountedDummy<T>*>(pNodeDelete->m_pNext);

				pPrevNode->m_pNext = pNodeDelete->m_pNext;
				pNextNode->m_pPrev = pNodeDelete->m_pPrev;
			}
			else // there is no node after the deleted node, meaning the deleted node is the tail node
			{
				// the node prior to the deleted node is the new tail
				pPrevNode->m_pNext = nullptr;
				this->m_pTail = &pPrevNode->t;
			}
		}
		else if (pNodeDelete && pNodeDelete->m_pNext)
		{
			ZRefCountedDummy<T>* pNextNode = reinterpret_cast<ZRefCountedDummy<T>*>(pNodeDelete->m_pNext);

			pNextNode->m_pPrev = nullptr;
			this->m_pHead = &pNextNode->t;
		}
		else // no next and no prev node
		{
			this->m_pTail = nullptr;
			this->m_pHead = nullptr;
		}

		this->m_uCount -= 1;

		delete pNodeDelete; // IMPORTANT: must delete the node, not the wrapped object (T)
	}

	/***=========== NODE SEARCH ===========***/

	T* FindIndex(const size_t uIndex) // TODO fix this
	{
		T* pRet;

		if (uIndex >= this->m_uCount) return nullptr;

		if (uIndex <= this->m_uCount / 2)
		{
			pRet = this->m_pHead;

			for (int i = 0; i < uIndex; i++)
			{
				if (!pRet) break;

				ZRefCounted* pNode = this->CastNode(pRet)->m_pNext;

				if (pNode) pRet = &reinterpret_cast<ZRefCountedDummy<T>*>(pNode)->t;
			}
		}
		else
		{
			pRet = this->m_pTail;

			for (int i = this->m_uCount - 1; i > uIndex; i--)
			{
				if (!pRet) break;

				ZRefCounted* pNode = this->CastNode(pRet)->m_pPrev;

				if (pNode) pRet = &reinterpret_cast<ZRefCountedDummy<T>*>(pNode)->t;
			}
		}

		return pRet;
	}

	int IndexOf(const T* pos)
	{
		T* pHead = this->m_pHead;
		int nIdx = 0;

		if (!pHead) return ZLIST_INVALID_INDEX;

		while (pHead != pos)
		{
			nIdx += 1;

			ZRefCountedDummy<T>* pNode = this->CastNode(pHead);

			if (!pNode->m_pNext) return ZLIST_INVALID_INDEX;

			pHead = &reinterpret_cast<ZRefCountedDummy<T>*>(pNode->m_pNext)->t;

			if (!pHead) return  ZLIST_INVALID_INDEX;
		}

		if (!pHead) return ZLIST_INVALID_INDEX;

		return nIdx;
	}

	/// <summary>
	/// Tries to find a node in the list with the same value as the given node d.
	/// If posAfter is defined, the function will only search for items after the given posAfter item.
	/// If posAfter is defined but is not a list node, undefined behavior will occur.
	/// </summary>
	T* Find(T* d, T* posAfter)
	{
		T* pRet;
		if (posAfter)
		{
			ZRefCountedDummy<T>* pNode = this->CastNode(posAfter);

			if (!pNode->m_pNext) return nullptr;

			pRet = &reinterpret_cast<ZRefCountedDummy<T>*>(pNode->m_pNext)->t;
		}
		else
		{
			pRet = this->m_pHead;
		}

		if (!pRet) return nullptr;

		while (*pRet != *d)
		{
			ZRefCountedDummy<T>* pNode = this->CastNode(pRet);

			if (pNode->m_pNext)
			{
				pRet = &reinterpret_cast<ZRefCountedDummy<T>*>(pNode->m_pNext)->t;

				if (pRet) continue;
			}

			return nullptr;
		}

		return pRet;
	}

	/***=========== INSERTION ===========***/

	T* Insert(T* d)
	{
		//T* result = this->AddTail(this);
		return nullptr; // TODO
	}

	T** InsertBefore(T* pos)
	{
		return nullptr; // TODO
	}

	/***=========== TRAVERSAL ===========***/

	T* GetNext(T** pos)
	{
		if (!pos) return nullptr;

		T* pRet = *pos;

		if (!pRet)
		{
			*pos = nullptr;
			return nullptr;
		}

		ZRefCountedDummy<T>* pNode = this->CastNode(pRet);

		*pos = pNode->m_pNext ? reinterpret_cast<T*>(&reinterpret_cast<ZRefCountedDummy<T>*>(pNode->m_pNext)->t) : nullptr;

		return pRet;
	}

	T* GetPrev(T** pos)
	{
		if (!pos) return nullptr;

		T* pRet = *pos;

		if (!pRet)
		{
			*pos = nullptr;
			return nullptr;
		}

		ZRefCountedDummy<T>* pNode = this->CastNode(pRet);

		*pos = pNode->m_pPrev ? &reinterpret_cast<ZRefCountedDummy<T>*>(pNode->m_pPrev)->t : nullptr;

		return pRet;
	}

private:

	/// <summary>
	/// If t is not a ZList member, then this will produce undefined results
	/// </summary>
	/// <param name="t"></param>
	/// <returns></returns>
	ZRefCountedDummy<T>* CastNode(T* t)
	{
		return reinterpret_cast<ZRefCountedDummy<T>*>(reinterpret_cast<char*>(t) - 16);
	}

	T* New(T* pPrev, T* pNext)
	{
		ZRefCountedDummy<T>* pAlloc = new ZRefCountedDummy<T>(); // IDA: ZRefCounted_Alloc<ZRefCountedDummy<T>>();

		pAlloc->m_pPrev = pPrev ? this->CastNode(pPrev) : nullptr;
		pAlloc->m_pNext = pNext ? this->CastNode(pNext) : nullptr;
		pAlloc->m_nRef = 0;

		this->m_uCount += 1;

		return &pAlloc->t;
	}
};

//assert_size(sizeof(ZList<int>), 0x14);