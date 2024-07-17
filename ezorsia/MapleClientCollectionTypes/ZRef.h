#pragma once
#include "ZRefCounted.h"
#include "ZRefCountedDummy.h"
#include "ZRefCountedAccessor.h"
#include <type_traits>
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
// ZRef is a smart pointer wrapper class that MapleStory uses to manage memory.
// If the object passed to the ZRef template is a ZRefCounted object, it will treat it normally,
//	otherwise it will add ZRefCountedDummy as an additional wrapper level to simulate a
//	ZRefCounted derived class.

template <class T>
class ZRef : protected ZRefCountedAccessor<T>, protected ZRefCountedAccessor<ZRefCountedDummy<T>>
{
	BYTE gap0[1];

public:
	T* p; // TODO uhh maybe reconsider exposing this as public

	ZRef()
	{
		this->gap0[0] = NULL;
		this->p = nullptr;
	}

	ZRef(ZRefCounted* pT, BOOL bAddRef = TRUE)
	{
		this->gap0[0] = NULL;

		if (!pT)
		{
			this->p = nullptr;
		}
		else
		{
			this->p = reinterpret_cast<T*>(pT);

			if (bAddRef)
			{
				InterlockedIncrement(&pT->m_nRef); // (this->p - 12)
			}
		}
	}

	ZRef(ZRef<T>* r)
	{
		ZRefCounted* pBase;
		this->gap0[0] = NULL;

		this->p = r->p;

		if (r->p)
		{
			pBase = r->GetBase();

			InterlockedIncrement(&pBase->m_nRef); // (this->p - 12)
		}
	}

	~ZRef()
	{
		this->ReleaseRaw();
	}

	/// <summary>
	/// Allocate resources for encapsulated pointer and initialize type.
	/// </summary>
	void Alloc()
	{
		this->ReleaseRaw();

		/* is_base_of was released in c++11, so maple did this some other way */
		if (std::is_base_of<ZRefCounted, T>())
		{
			ZRefCounted* pAlloc = reinterpret_cast<ZRefCounted*>(new T());

			pAlloc->m_nRef = 1;
			this->p = reinterpret_cast<T*>(pAlloc);
		}
		else
		{
			ZRefCountedDummy<T>* pAlloc = new ZRefCountedDummy<T>();// ZRefCounted_Alloc<ZRefCountedDummy<T>>();

			pAlloc->m_nRef = 1;
			this->p = &pAlloc->t;
		}
	}

	/// <summary>
	/// Set this ZRef pointer equal to the given pointer. Only works for ZRefCounted types.
	/// </summary>
	ZRef<T>* operator=(ZRefCounted* pT)
	{
		ZRef<ZRefCounted> r;
		if (pT)
		{
			InterlockedIncrement(&pT->m_nRef);
		}

		T* old = this->p;
		this->p = reinterpret_cast<T*>(pT);
		r.p = old; // resources are automatically freed by compiler-generated destructor

		return this;
	}

	/// <summary>
	/// Set this ZRef equal to the given ZRef
	/// </summary>
	ZRef<T>* operator=(ZRef<T>* r)
	{
		ZRefCounted* pBase;

		if (r->p)
		{
			pBase = r->GetBase();
			InterlockedIncrement(&pBase->m_nRef);
		}

		this->ReleaseRaw();

		this->p = r->p;

		return this;
	}

	/// <summary>
	/// Release pointer resources.
	/// </summary>
	ZRef<T>* operator=(int zero)
	{
		this->ReleaseRaw();
		return this;
	}

	/// <summary>
	/// Fetch pointer to encapsulated object.
	/// </summary>
	operator T*()
	{
		return this->p;
	}

	/// <summary>
	/// Fetch pointer to encapsulated object.
	/// </summary>
	T* operator->()
	{
		return this->p;
	}

	/// <summary>
	/// Determine if encapsulated pointer is null.
	/// </summary>
	BOOL operator!()
	{
		return this->p == nullptr;
	}

private:
	/// <summary>
	/// Decrement pointer reference count and release resources if references are zero.
	/// </summary>
	void ReleaseRaw()
	{
		if (!this->p) return;

		ZRefCounted* pBase = this->GetBase();

		if (InterlockedDecrement(&pBase->m_nRef) <= 0)
		{
			InterlockedIncrement(&pBase->m_nRef);

			delete pBase; // if (v3) (**v3)(v3, 1);
		}

		this->p = nullptr;
	}

	/// <summary>
	/// Returns the associated ZRefCounted object pointer.
	/// </summary>
	ZRefCounted* GetBase()
	{
		ZRefCounted* pBase;

		/* is_base_of was released in c++11, so maple did this some other way */
		if (std::is_base_of<ZRefCounted, T>() || typeid(ZRefCounted) == typeid(T))
		{
			pBase = reinterpret_cast<ZRefCounted*>(this->p);
		}
		else
		{
			pBase = reinterpret_cast<ZRefCounted*>(reinterpret_cast<char*>(this->p) - (sizeof(ZRefCountedDummy<T>) - sizeof(T)));

			static_assert(sizeof(ZRefCountedDummy<T>) - sizeof(T) == 16, "Size is not expected value");
		}

		return pBase;
	}
};

//assert_size(sizeof(ZRef<int>), 0x08);
