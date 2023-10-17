#pragma once
#include "ZAllocEx.h"
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
template<typename T> struct ZAllocEx;
template<typename T> struct ZAllocStrSelector;

/*
	Credits: Minimum Delta - this was a pain in the ass to figure out, ya'll

	Stole a lot of documenation and ideas from:
	 * https://docs.microsoft.com/en-us/cpp/cpp/bstr-t-class
	 * https://docs.microsoft.com/en-us/cpp/cpp/smart-pointers-modern-cpp
	 * https://en.cppreference.com/w/cpp/language/operators
*/

/// <summary>
/// A ZXString object encapsulates a 1 or 2 byte binary string of type char or wchar_t.
/// The class manages resource allocation and deallocation through function calls to ZAllocEx::Alloc and ZAllocEx::Free.
/// This is intended to almost entirely mimick the functionality of the MapleStory ZXString template class.
/// </summary>
/// <remarks>
/// In order for ZXString objects created in your dll to be passed into MapleStory, they must be allocated using the ZAllocEx::Free function.
/// To make this less of a hassle, the ZAllocEx templates have been added to this project and the ZXString new and delete operators have been overloaded to seamlessly deal with this.
/// </remarks>
/// <typeparam name="T">Either char or wchar_t (2 byte type)</typeparam>
template <typename T>
class ZXString
{
public:
	/// <summary>
	/// The ZXString smart pointer data structure.
	/// This structure keeps track of the number of references to the string, the size allocated for the string, 
	/// and the byte length of the string itself (not including the null terminator) which is calculated by sizeof(T) * bytelen.
	/// </summary>
	struct _ZXStringData
	{
	public:
		/// <summary>
		/// The number of references pointing to this string.
		/// </summary>
		/// <remarks>Has to be volatile because of interlocked operations</remarks>
		volatile long nRef;

		/// <summary>
		/// The string buffer size limit (not including the string data struct)
		/// </summary>
		size_t nCap;

		/// <summary>
		/// The length, in bytes, of the string. Must be shifted right by sizeof(T) - 1 to get the string length.
		/// </summary>
		size_t nByteLen;
	};

private:
	T* m_pStr; // needs to be initialized to zero when hooking sometimes

public:
	// ------------------------------------------------------ Constructors

	/// <summary>
	/// Constructs an empty ZXString object.
	/// </summary>
	ZXString()
	{
		this->m_pStr = nullptr;
	}

	/// <summary>
	/// Constructs a ZXString object.
	/// </summary>
	/// <param name="val">String to pass to object.</param>
	/// <param name="n">Length of string. If -1, string length will be automatically calculated.</param>
	ZXString(const T* s, int n = -1)
	{
		this->m_pStr = nullptr;
		this->Assign(s, n);
	}

	/// <summary>
	/// Constructs a ZXString object.
	/// </summary>
	/// <param name="s">An existing ZXString object.</param>
	ZXString(ZXString<T>* s)
	{
		this->m_pStr = nullptr;
		this->Assign(s);
	}

	/// <summary>
	/// Destroys the ZXString object.
	/// Free's any memory used by the ZXString object if it's references are <= 1 (including this instance).
	/// </summary>
	~ZXString()
	{
		if (this->m_pStr)
		{
			this->Release();
		}
	}

	// ------------------------------------------------------ Operator Overloading

	/// <summary>
	/// Assigns a new value to an existing ZXString object.
	/// </summary>
	/// <param name="s">A ZXString object to be assigned to an existing ZXString object.</param>
	ZXString<T*> operator=(ZXString<T>* s)
	{
		this->Assign(s);
		return this;
	}

	/// <summary>
	/// Assigns a new value to an existing ZXString object.
	/// </summary>
	/// <param name="s">A T-sized char array to be assigned to an existing ZXString object.</param>
	ZXString<T>* operator=(const T* s)
	{
		this->Assign(s, -1);
		return this;
	}

	/// <summary>
	/// Appends characters to the end of the ZXString object.
	/// </summary>
	/// <param name="s">ZXString object containing the char array to append to the ZXString object.</param>
	ZXString<T>* operator+=(ZXString<T>* s)
	{
		return s->m_pStr ? this->Concat(s, s->GetData()->nByteLen) : this;
	}

	/// <summary>
	/// Appends characters to the end of the ZXString object.
	/// </summary>
	/// <param name="s">T-size char array to append to the ZXString object.</param>
	ZXString<T>* operator+=(const T* s)
	{
		return s ? this->Concat(s, this->TStrLen(s)) : this;
	}

	/// <summary>
	/// Compares a ZXString object to an existing ZXString object
	/// </summary>
	/// <param name="s">ZXString object to compare against.</param>
	/// <returns>
	/// True if the encapsulated string in the ZXString objects are lexicographically identical to each other, else false.
	/// </returns>
	BOOL operator==(ZXString<T>* s)
	{
		return this->Compare(s);
	}

	BOOL operator!=(ZXString<T>* s)
	{
		return !this->Compare(s);
	}

	/// <summary>
	/// Compares a ZXString object to a char array of equal size as the T type in the ZXString object.
	/// </summary>
	/// <param name="s">T-size char array to compare against.</param>
	/// <returns>
	/// True if the encapsulated string in the ZXString object is lexicographically identical to the char array s, else false.
	/// </returns>
	BOOL operator==(const T* s)
	{
		return this->Compare(s);
	}

	BOOL operator!=(const T* s)
	{
		return !this->Compare(s);
	}

	/// <summary>
	/// Checks if the encapsulated string is a NULL string.
	/// </summary>
	BOOL operator !()
	{
		return *this->m_pStr == NULL;
	}

	/// <summary>
	/// Extract the encapsulated unicode or multibyte char pointer.
	/// </summary>
	operator const T* ()
	{
		return this->m_pStr;
	}

	// ------------------------------------------------------ Public Member Functions

	/// <summary>
	/// Retrieves the number of characters in the char array, not including the terminating null.
	/// </summary>
	size_t Length()
	{
		return this->m_pStr ? this->GetData()->nByteLen >> (sizeof(T) - 1) : 0;
	}

	/// <summary>
	/// Releases the resources associated with the string and sets the encapsulated T pointer to null.
	/// Reduces the reference count to 
	/// </summary>
	void Empty()
	{
		if (this->m_pStr)
		{
			this->Release();
		}
		this->m_pStr = nullptr;
	}

	/// <summary>
	/// Determines if the ZXString object is empty.
	/// </summary>
	/// <returns>True if the encapsulated T pointer is null or if the value pointed to by the T pointer is null, else false.</returns>
	BOOL IsEmpty()
	{
		return this->m_pStr == nullptr || *this->m_pStr == NULL;
	}

	void Assign(ZXString<T>* s)
	{
		ZXString<T>::_ZXStringData* pNewData;

		if (this != s)
		{
			pNewData = s->GetData();

			if (s->m_pStr && pNewData)
			{
				if (pNewData->nRef == -1)
				{
					this->Assign(s->m_pStr);
				}
				else
				{
					InterlockedIncrement(&pNewData->nRef);

					if (this->m_pStr)
					{
						this->Release();
					}

					this->m_pStr = reinterpret_cast<T*>(&pNewData[1]);
				}
			}
			else if (this->m_pStr)
			{
				this->Release();
				this->m_pStr = 0;
			}
		}
	}

	void Assign(const T* s, int nLen = -1)
	{
		if (s)
		{
			if (nLen == -1)
			{
				nLen = this->TStrLen(s);
			}

			T* pBuff = this->GetBuffer(nLen, FALSE);
			int nByteLength = nLen * sizeof(T);

			memcpy(pBuff, s, nByteLength);
			this->ReleaseBuffer(nLen);
		}
		else if (this->m_pStr)
		{
			this->Release();
			this->m_pStr = 0;
		}
	}

	BOOL Compare(ZXString<T>* s)
	{
		int nStr1Len, nStr2Len;

		if (this->m_pStr == s->m_pStr) return TRUE;

		nStr1Len = this->Length();
		nStr2Len = s->Length();

		if (nStr1Len == nStr2Len && this->m_pStr && s->m_pStr)
		{
			for (int i = 0; i < nStr1Len; i++)
			{
				if (this->m_pStr[i] != s->m_pStr[i])
				{
					return FALSE;
				}
			}

			return TRUE;
		}

		return FALSE;
	}

	BOOL Compare(const char* s)
	{
		if (!this->m_pStr) return !s;

		return !strcmp(reinterpret_cast<const char*>(this->m_pStr), reinterpret_cast<const char*>(s));
	}

	BOOL Compare(const wchar_t* s)
	{
		if (!this->m_pStr) return !s;

		return !wcscmp(reinterpret_cast<const wchar_t*>(this->m_pStr), reinterpret_cast<const wchar_t*>(s));
	}

	BOOL CompareNoCase(const char* s)
	{
		if (!this->m_pStr) return !s;

		return !stricmp(reinterpret_cast<const char*>(this->m_pStr), reinterpret_cast<const char*>(s));
	}

	BOOL CompareNoCase(const wchar_t* s)
	{
		if (!this->m_pStr) return !s;

		return !wcsicmp(reinterpret_cast<const wchar_t*>(this->m_pStr), reinterpret_cast<const wchar_t*>(s));
	}

	ZXString<T>* Concat(const T* s, int nLen = -1) // _Cat
	{
		if (nLen == -1)
		{
			nLen = this->TStrLen(s);
		}

		if (!nLen) return this;

		if (this->IsEmpty()) // if existing string is null or empty, copy new string to existing string
		{
			T* pBuff = this->GetBuffer(nLen, FALSE);
			memcpy(pBuff, s, sizeof(T) * nLen);
			this->ReleaseBuffer(nLen);
		}
		else // else add new string to existing string
		{
			ZXString<T>::_ZXStringData* pData = this->GetData();

			int nCap = pData->nCap;
			int i = nLen + this->Length();

			// double the buffer size until its a multiple of two larger
			//	than the required length of the combined strings plus the size of the pointer data structure
			while (nCap < i)
			{
				nCap *= 2;
			}

			T* pBuff = this->GetBuffer(nCap, TRUE);

			int nByteLength = sizeof(T) * nLen;

			/* length is starting index, compiler automatically adjusts for size of T when getting buffer index */
			memcpy(&pBuff[this->Length()], s, nByteLength);

			this->ReleaseBuffer(i);
		}

		return this;
	}

	void Format(const T* sFormat, ...)
	{
		ZXString<T> s = ZXString<T>::ZXString();
		T* pBuff;

		int nBytesWritten = -1;

		va_list args;
		va_start(args, sFormat);

		for (int nBufferLen = 16; nBufferLen <= 1024; nBufferLen *= 2)
		{
			if (nBytesWritten >= 0) break;

			pBuff = s.GetBuffer(nBufferLen, FALSE);

			if (sizeof(T) == 1) // compiler should optimize away this conditional when in release mode
			{
				nBytesWritten = _vsnprintf_s((char*)pBuff, nBufferLen, _TRUNCATE, (const char*)sFormat, args);
			}
			else
			{
				nBytesWritten = _vsnwprintf_s((wchar_t*)pBuff, nBufferLen, _TRUNCATE, (const wchar_t*)sFormat, args);
			}

			s.ReleaseBuffer(nBufferLen < 0 ? 0 : nBufferLen);
		}

		this->Assign(&s);

		if (s.m_pStr)
		{
			s.Release();
		}

		va_end(args);
	}

	// ------------------------------------------------------ Private Member Functions
private:
	ZXString<T>::_ZXStringData* GetData()
	{
		if (this->m_pStr)
		{
			return reinterpret_cast<ZXString<T>::_ZXStringData*>(reinterpret_cast<unsigned char*>(this->m_pStr) - sizeof(ZXString<T>::_ZXStringData));
		}

		return nullptr;
	}

	T* GetBuffer(size_t nMinLength, BOOL bRetain)
	{
		ZXString<T>::_ZXStringData* pCurData;
		ZXString<T>::_ZXStringData* pNewData;
		size_t nStrLen;

		if (this->m_pStr)
		{
			pCurData = this->GetData();

			if (pCurData)
			{
				if (pCurData->nRef <= 1 && pCurData->nCap >= nMinLength)
				{
					pCurData->nRef = -1;
					return this->m_pStr;
				}

				nStrLen = pCurData->nByteLen >> (sizeof(T) - 1);
			}
			else
			{
				nStrLen = 0;
			}
		}
		else
		{
			nStrLen = 0;
			pCurData = nullptr;
		}

		if (nStrLen < nMinLength) nStrLen = nMinLength;

		pNewData = this->Alloc(nStrLen);

		pNewData->nRef = -1;
		this->m_pStr = reinterpret_cast<T*>(&pNewData[1]);

		if (bRetain && pCurData)
		{
			size_t nSize = sizeof(T) * (pCurData->nByteLen >> (sizeof(T) - 1)) + sizeof(T);

			memcpy(reinterpret_cast<T*>(&pNewData[1]), reinterpret_cast<T*>(&pCurData[1]), nSize);
			pNewData->nByteLen = pCurData->nByteLen;
		}
		else
		{
			pNewData->nByteLen = 0;
			*this->m_pStr = 0;
		}

		if (pCurData)
		{
			if (InterlockedDecrement(&pCurData->nRef) <= 0)
			{
				ZAllocEx<ZAllocStrSelector<T>>::GetInstance()->Free((void**)pCurData);
			}
		}

		return this->m_pStr;
	}

	void ReleaseBuffer(size_t nLength)
	{
		ZXString<T>::_ZXStringData* pData = this->GetData();

		pData->nRef = 1;

		if (nLength == -1)
		{
			pData->nByteLen = sizeof(T) * this->TStrLen(reinterpret_cast<T*>(&pData[1]));
		}
		else
		{
			this->m_pStr[nLength] = 0;
			pData->nByteLen = sizeof(T) * nLength;
		}
	}

	ZXString<T>::_ZXStringData* Alloc(size_t nCap)
	{
		size_t nTotalSize = (sizeof(T) * nCap) + sizeof(ZXString<T>::_ZXStringData) + sizeof(T);

		PVOID pAllocated = ZAllocEx<ZAllocStrSelector<T>>::GetInstance()->Alloc(nTotalSize);

		ZXString<T>::_ZXStringData* result = reinterpret_cast<ZXString<T>::_ZXStringData*>(pAllocated);
		result->nCap = nCap;
		return result;
	}

	void Release()
	{
		if (InterlockedDecrement(&this->GetData()->nRef) <= 0)
		{
			ZAllocEx<ZAllocStrSelector<T>>::GetInstance()->Free((void**)this->GetData());
		}
	}

	size_t TStrLen(const char* s)
	{
		return strlen(reinterpret_cast<const char*>(s));
	}

	size_t TStrLen(const wchar_t* s)
	{
		return wcslen(reinterpret_cast<const wchar_t*>(s));
	}
};

//assert_size(sizeof(ZXString<char>), 0x04)