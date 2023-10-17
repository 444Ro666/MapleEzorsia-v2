#pragma once
#include <Windows.h>
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
/*
	Original Credits: https://github.com/67-6f-64/Firefly/blob/master/Firefly%20Spy/TSecType.hpp
	Modifications Made By:
		- Rajan Grewal
		- Minimum Delta

	Additional Information From: https://en.cppreference.com/w/cpp/language/operators
*/

template <typename T>
class TSecData
{
public:
	T data;
	BYTE bKey;
	BYTE FakePtr1;
	BYTE FakePtr2;
	WORD wChecksum;
};

template <typename T>
class TSecType
{
private:
	DWORD FakePtr1;
	DWORD FakePtr2;
	TSecData<T>* m_secdata;

public:
	TSecType()
	{
		this->m_secdata = new TSecData<T>(); // uses proper ZAllocEx now (since global new operator overload)

		this->FakePtr1 = static_cast<DWORD>(rand());
		this->FakePtr2 = static_cast<DWORD>(rand());

		this->m_secdata->FakePtr1 = LOBYTE(this->FakePtr1);
		this->m_secdata->FakePtr2 = LOBYTE(this->FakePtr2);

		this->SetData(NULL);
	}

	TSecType(const T op)
	{
		this->m_secdata = new TSecData<T>(); // uses proper ZAllocEx now (since global new operator overload)

		this->FakePtr1 = static_cast<DWORD>(rand()); 
		this->FakePtr2 = static_cast<DWORD>(rand()); 

		this->m_secdata->FakePtr1 = LOBYTE(this->FakePtr1);
		this->m_secdata->FakePtr2 = LOBYTE(this->FakePtr2);

		this->SetData(op);
	}

	~TSecType()
	{
		if (this->m_secdata)
		{
			delete this->m_secdata;
		}
	}

	operator T()
	{
		return this->GetData();
	}

	BOOL operator ==(TSecType<T>* op)
	{
		return this->GetData() == op->GetData();
	}

	TSecType<T>* operator =(const T op)
	{
		this->SetData(op);
		return this;
	}

	TSecType<T>* operator =(TSecType<T>* op)
	{
		T data = op->GetData();
		this->SetData(data);
		return this;
	}

	T operator /=(const T op)
	{
		T tmp = this->GetData() / op;
		this->SetData(tmp);
		return tmp;
	}

	T operator *=(const T op)
	{
		T tmp = this->GetData() * op;
		this->SetData(tmp);
		return tmp;
	}

	T operator +=(const T op)
	{
		T tmp = this->GetData() + op;
		this->SetData(tmp);
		return tmp;
	}

	T operator -=(const T op)
	{
		T tmp = this->GetData() - op;
		this->SetData(tmp);
		return tmp;
	}

	T GetData()
	{
		T decrypted_data = this->m_secdata->data;
		WORD wChecksum = 0;

		for (BYTE i = 0, key = this->m_secdata->bKey; i < (sizeof(T) + 1); i++)
		{
			if (i > 0)
			{
				key = reinterpret_cast<BYTE*>(&this->m_secdata->data)[i - 1] + key + 42;;
				wChecksum = i > 1 ? ((8 * wChecksum) | (key + (wChecksum >> 13))) : ((key + 4) | 0xD328);
			}

			if (i < sizeof(T))
			{
				if (!key)
				{
					key = 42;
				}

				reinterpret_cast<BYTE*>(&decrypted_data)[i] = reinterpret_cast<BYTE*>(&this->m_secdata->data)[i] ^ key;
			}

		}

		if (this->m_secdata->wChecksum != wChecksum || LOBYTE(this->FakePtr1) != this->m_secdata->FakePtr1 || LOBYTE(this->FakePtr2) != this->m_secdata->FakePtr2)
		{
			return NULL; //TODO: CxxThrow
		}

		return decrypted_data;
	}

	VOID SetData(T data)
	{
		this->m_secdata->bKey = LOBYTE(rand());
		this->m_secdata->wChecksum = sizeof(T) > 1 ? static_cast<WORD>(39525) : static_cast<WORD>(-26011);

		for (BYTE i = 0, key = this->m_secdata->bKey; i < (sizeof(T) + 1); i++)
		{
			if (i > 0)
			{
				key = (key ^ reinterpret_cast<BYTE*>(&data)[i - 1]) + key + 42;
				this->m_secdata->wChecksum = (8 * this->m_secdata->wChecksum) | (key + (this->m_secdata->wChecksum >> 13));
			}

			if (i < sizeof(T))
			{
				if (!key)
				{
					key = 42;
				}

				reinterpret_cast<BYTE*>(&this->m_secdata->data)[i] = reinterpret_cast<BYTE*>(&data)[i] ^ key;
			}

		}
	}
};

class SECPOINT
{
public:
	TSecType<long> y;
	TSecType<long> x;

	SECPOINT() { }

	SECPOINT(long ptX, long ptY)
	{
		this->x = ptX;
		this->y = ptY;
	}

	SECPOINT(SECPOINT* ptSrc)
	{
		this->x.SetData(ptSrc->x.GetData());
		this->y.SetData(ptSrc->y.GetData());
	}

	SECPOINT(tagPOINT* ptSrc)
	{
		this->x.SetData(ptSrc->x);
		this->y.SetData(ptSrc->y);
	}

	~SECPOINT()
	{
		this->x.~TSecType();
		this->y.~TSecType();
	}

	SECPOINT* operator =(tagPOINT* ptSrc)
	{
		this->x.SetData(ptSrc->x);
		this->y.SetData(ptSrc->y);
		return this;
	}

	SECPOINT* operator =(SECPOINT* ptSrc)
	{
		this->x.SetData(ptSrc->x.GetData());
		this->y.SetData(ptSrc->y.GetData());
		return this;
	}

	BOOL operator !=(tagPOINT* ptSrc)
	{
		return this->x.GetData() != ptSrc->x || this->y.GetData() != ptSrc->y;
	}

	BOOL operator ==(tagPOINT* ptSrc)
	{
		return this->x.GetData() == ptSrc->x && this->y.GetData() == ptSrc->y;
	}

	BOOL operator !=(SECPOINT* ptSrc)
	{
		return this->x.GetData() != ptSrc->x.GetData() || this->y.GetData() != ptSrc->y.GetData();
	}

	BOOL operator ==(SECPOINT* ptSrc)
	{
		return this->x.GetData() == ptSrc->x.GetData() && this->y.GetData() == ptSrc->y.GetData();
	}

	operator tagPOINT()
	{
		return { this->x.GetData(), this->y.GetData() };
	}
};

//assert_size(sizeof(TSecData<long>), 0x0C)
//assert_size(sizeof(TSecType<long>), 0x0C)