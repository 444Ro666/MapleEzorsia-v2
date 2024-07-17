#pragma once
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
struct ZFatalSectionData
{
	void* _m_pTIB;
	int _m_nRef;
};

struct ZFatalSection : ZFatalSectionData
{
	/* TODO emulate this class */
};

//assert_size(sizeof(ZFatalSection), 0x8)
