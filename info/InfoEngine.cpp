#include "StdAfx.h"
#include  <cctype>
#include "InfoEngine.h"

InfoEngine* InfoEngine::GetInstance()
{
	static InfoEngine gInstance;
	return &gInstance;
}

void InfoEngine::Registe(IInfoListener* pListener)
{
	mListeners.insert(pListener);
}

void InfoEngine::UnRegiste(IInfoListener* pListener)
{
	mListeners.erase(pListener);
}

void InfoEngine::Start( void )
{

}

void InfoEngine::Stop( void )
{

}

void InfoEngine::SetRect(size_t idx, CRect& rect, DataType dtype)
{
	auto& info = mInfoRects[idx];
	info.type = dtype;
    info.rect = rect;
	info.price = 0;
}

bool InfoEngine::IsEmptyRect(CRect& rect) 
{ 
	return rect.left == rect.right|| rect.bottom == rect.top;
}

void InfoEngine::CollectData(size_t index)
{
	auto& info = mInfoRects[index];
	if(!IsEmptyRect(info.rect))
	{
		CString value, outFeature;
		info.img.CaptureRect(info.rect);
		if(mRecognizer->RecognizeEx(value, outFeature, &info.img))
		{
			switch (info.type)
			{
			case PRICE:
				info.price =  _ttoi(value);
				break;
			case TIME:
				{
					int hour = Convert2Int(value, 0, 2);
					int minute = Convert2Int(value, 3, 5);
					int second = Convert2Int(value, 6, 8);
					info.time.hour = hour;
					info.time.minute = minute;
					info.time.second = second;
				}
				break;
			case QUANTITY:
				info.quantity =  _ttoi(value);
				break;
			case PRICE_RANGE:
				{
					int sep = value.Find('|');
					info.priceRange.SetLowest(Convert2Int(value, 0, sep));
					info.priceRange.SetHighest(Convert2Int(value, sep + 1, value.GetLength()));
				}
				break;
			}
		}
	}
}

int InfoEngine::Convert2Int(CString& str, int from, int to)
{
	int value = 0, sign = 1;
	if('-' == str.GetAt(from))
	{
		sign = -1;
		++from;
	}
	else if('+' == str.GetAt(from))
	{
		++from;
	}

	for(; from < to && std::isdigit(str.GetAt(from)); ++from)
	{
		value *= 10;
		value += (str.GetAt(from) - '0');
	}
	value *= sign;

	if(from == to)
	{
		return value;
	}

	return 0;
}