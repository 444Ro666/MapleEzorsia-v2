#pragma once
//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
class ZFakeStatAvBuff { };

class ZRecyclableStatic
{
public:
	struct CallBack
	{
		CallBack* m_pNext;

	protected:
		CallBack()
		{
			m_pNext = nullptr;
		}

		virtual ~CallBack() = default;
	};

protected:
	CallBack* m_pHead;
};

//assert_size(sizeof(ZFakeStatAvBuff), 0x1)
//assert_size(sizeof(ZRecyclableStatic::CallBack), 0x8)
//assert_size(sizeof(ZRecyclableStatic), 0x4)
