#pragma once
#include <Windows.h>
#include <condition_variable>
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
/*
	Credits: https://github.com/67-6f-64/Firefly/blob/master/Firefly%20Spy/ZtlSecure.hpp

	Modifications By:
		 - Rajan Grewal
		 - Minimum Delta
*/

#define ZTLSECURE_CHECKSUM 0xBAADF00D
#define ZTLSECURE_ROTATION 5

template <typename T> // uses fastcall because it passes args through registers instead of on stack -- faster execution
unsigned int __fastcall ZtlSecureTear(T* at, T t)
{
	/* storage type is 1 byte large if size < 4, otherwise its 4 bytes large */
	typedef std::conditional<(sizeof(T) < 4), unsigned char, unsigned int>::type PType;

	unsigned int checksum = ZTLSECURE_CHECKSUM;

	PType* key = reinterpret_cast<PType*>(&at[0]);
	PType* encrypted_data = reinterpret_cast<PType*>(&at[1]);

	PType* p = reinterpret_cast<PType*>(&t);

	for (int i = 0; i < sizeof(T) / sizeof(PType); i++)
	{
		int rotations = sizeof(T) < sizeof(unsigned int) ? NULL : ZTLSECURE_ROTATION;

		/* TODO use CRand32 like MapleStory does */
		key[i] = sizeof(T) < sizeof(unsigned int) ? LOBYTE(rand()) : rand();
		encrypted_data[i] = _rotr(p[i] ^ key[i], rotations);

		checksum = encrypted_data[i] + _rotr(key[i] ^ checksum, ZTLSECURE_ROTATION);
	}

	return checksum;
}

template <typename T>
T __fastcall ZtlSecureFuse(T* at, unsigned int uCS)
{
	/* storage type is 1 byte large if size < 4, otherwise its 4 bytes large */
	typedef std::conditional<(sizeof(T) < 4), unsigned char, unsigned int>::type PType;

	unsigned int checksum = ZTLSECURE_CHECKSUM;

	PType* key = reinterpret_cast<PType*>(&at[0]);
	PType* encrypted_data = reinterpret_cast<PType*>(&at[1]);

	PType value[sizeof(T) / sizeof(PType)] = { 0 };

	for (int i = 0; i < sizeof(T) / sizeof(PType); i++)
	{
		int rotations = sizeof(T) < sizeof(unsigned int) ? NULL : ZTLSECURE_ROTATION;

		value[i] = key[i] ^ _rotl(encrypted_data[i], rotations);
		checksum = encrypted_data[i] + _rotr(key[i] ^ checksum, ZTLSECURE_ROTATION);
	}

	if (checksum != uCS)
	{
		// TODO exception handling
		/*ZException zException = { 5 };
		CxxThrowException(&zException, (void*)ThrowInfo::ZException);*/
	}

	return  *reinterpret_cast<T*>(&value[0]);
}

class SECRECT
{
private:
	int _ZtlSecureTear_left[2];
	unsigned int _ZtlSecureTear_left_CS;
	int _ZtlSecureTear_top[2];
	unsigned int _ZtlSecureTear_top_CS;
	int _ZtlSecureTear_right[2];
	unsigned int _ZtlSecureTear_right_CS;
	int _ZtlSecureTear_bottom[2];
	unsigned int _ZtlSecureTear_bottom_CS;

public:
	SECRECT()
	{
		SetRect(0, 0, 0, 0);
	}

	SECRECT(int l, int t, int r, int b)
	{
		SetRect(l, t, r, b);
	}

	void SetRect(int l, int t, int r, int b)
	{
		this->_ZtlSecureTear_left_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_left, l);
		this->_ZtlSecureTear_top_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_top, t);
		this->_ZtlSecureTear_right_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_right, r);
		this->_ZtlSecureTear_bottom_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_bottom, b);
	}

	void SetRectEmpty()
	{
		this->_ZtlSecureTear_left_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_left, NULL);
		this->_ZtlSecureTear_top_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_top, NULL);
		this->_ZtlSecureTear_right_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_right, NULL);
		this->_ZtlSecureTear_bottom_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_bottom, NULL);
	}

	BOOL IsRectEmpty()
	{
		if (GetLeft() < GetRight() && GetTop() < GetBottom())
		{
			return FALSE;
		}

		return TRUE;
	}

	int GetRight() // original name: ZtlSecureGet_right
	{
		return ZtlSecureFuse<int>(this->_ZtlSecureTear_right, this->_ZtlSecureTear_right_CS);
	}

	int GetLeft() // original name: ZtlSecureGet_left
	{
		return ZtlSecureFuse<int>(this->_ZtlSecureTear_left, this->_ZtlSecureTear_left_CS);
	}

	int GetTop()
	{
		return ZtlSecureFuse<int>(this->_ZtlSecureTear_top, this->_ZtlSecureTear_top_CS);
	}

	int GetBottom()
	{
		return ZtlSecureFuse<int>(this->_ZtlSecureTear_bottom, this->_ZtlSecureTear_bottom_CS);
	}

	void PutRight(int r)
	{
		this->_ZtlSecureTear_right_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_right, r);
	}

	void PutLeft(int l)
	{
		this->_ZtlSecureTear_left_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_left, l);
	}

	void PutTop(int t)
	{
		this->_ZtlSecureTear_top_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_top, t);
	}

	void PutBottom(int b)
	{
		this->_ZtlSecureTear_bottom_CS = ZtlSecureTear<int>(this->_ZtlSecureTear_bottom, b);
	}
};