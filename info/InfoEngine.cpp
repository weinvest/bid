#include "StdAfx.h"
#include  <cctype>
#include <string>
#include <sstream>
#include <functional>
#include <chrono>
#include <fstream>
#include "InfoEngine.h"

bool operator < (const BidTime& lhs, const BidTime& rhs)
{
	if (lhs.hour < rhs.hour)
	{
		return true;
	}

	if (lhs.minute < rhs.minute)
	{
		return true;
	}

	if (lhs.second < rhs.second)
	{
		return true;
	}

	if (lhs.milliseconds < rhs.milliseconds)
	{
		return true;
	}

	return false;
}

void BidTime::Parse(const std::string& value)
{
	if (value.length() >= 8)
	{
		int startPos = 0;
		hour = InfoEngine::Convert2Int(value, startPos, startPos + 2);
		minute = InfoEngine::Convert2Int(value, startPos + 3, startPos + 5);
		second = InfoEngine::Convert2Int(value, startPos + 6, startPos + 8);

		if (value.length() > 9)
		{
			milliseconds = InfoEngine::Convert2Int(value, startPos + 9, startPos + 12);
		}
	}
}

InfoEngine* InfoEngine::GetInstance()
{
	static InfoEngine gInstance;
	return &gInstance;
}

void InfoEngine::Data::ToString(std::string& out, size_t idx) const
{
	switch (type)
	{
	case PRICE_RANGE:
		{
			if (0 == idx)
			{
				out = std::to_string(priceRange.GetLowest());
			}
			else
			{
				out = std::to_string(priceRange.GetHighest());
			}
		}
		break;
	case PRICE:
		out = std::to_string( price);
		break;
	case TIME:
	    {
		    out.resize(12);
		    snprintf((char*)out.c_str(), out.length(), "%02d:%02d:%02d.%03d", time.hour, time.minute, time.second, time.milliseconds);
	    }
		break;
	case QUANTITY:
		out = std::to_string(quantity);
		break;
	default:
		break;
	}
}

void InfoEngine::Load(const CString& path)
{
	std::ifstream inFile((LPCTSTR)path, std::ios_base::binary | std::ios_base::in);
	if (inFile.is_open())
	{
		std::string line;
		std::getline(inFile, line);
		std::stringstream s(line);
		s >> mReferencePoint.x >> mReferencePoint.y;

		while (std::getline(inFile, line))
		{
			std::stringstream s(line);
			size_t index = 0, type(0);
			s >> index >> type;

			auto& info = mInfoRects[index];
			info.type = (DataType)type;
			s >> info.rect.top >> info.rect.left >> info.rect.bottom >> info.rect.right;
		}
	}
}

void InfoEngine::Save(const CString& path)
{
	std::ofstream file((LPCTSTR)path, std::ios_base::binary | std::ios_base::out);
	file << mReferencePoint.x << " " << mReferencePoint.y << '\n';
	for (size_t idx = 0; idx < COUNT_OF_INFOS; ++idx)
	{
		auto& info = mInfoRects[idx];
		file << idx << " " << info.type << " " << info.rect.top << " " << info.rect.left << " " << info.rect.bottom << " " << info.rect.right << "\n";
	}
}

void InfoEngine::SetReferencePoint(size_t x, size_t y)
{
	if (-1 != mReferencePoint.x && -1 != mReferencePoint.y)
	{
		auto diff = CPoint(x, y) - mReferencePoint;

		for (size_t idx = 0; idx < COUNT_OF_INFOS; ++idx)
		{
			auto& info = mInfoRects[idx];
			info.rect += diff;
		}
	}
	else
	{
		mReferencePoint.x = x;
		mReferencePoint.y = y;
	}
}

InfoEngine::InfoEngine()
	:mReferencePoint(-1, -1)
	,mIsRuning(true)
{}

void InfoEngine::Registe(IInfoListener* pListener)
{
	mListeners.insert(pListener);
}

void InfoEngine::UnRegiste(IInfoListener* pListener)
{
	mListeners.erase(pListener);
}

void InfoEngine::Pause( void )
{
	mIsRuning = false;
}

void InfoEngine::Step(void)
{
	if (!mIsRuning)
	{
		return;
	}

	mUpdateFields = 0;
	for (size_t iThread = 0; iThread < COUNT_OF_INFOS; ++iThread)
	{
		CollectData(iThread);
	}

	if (0 == mUpdateFields)
	{
		return;
	}

	for (auto pListener : mListeners)
	{
		pListener->OnUpdate(mUpdateFields);
	}
}

void InfoEngine::Resume( void )
{
	mIsRuning = true;
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
	//while (mIsRuning)
	{
		if (!IsEmptyRect(info.rect))
		{
			auto oldValue = info.price;
			std::string value, outFeature;
			info.img.CaptureRect(info.rect);
			if (mRecognizer->RecognizeEx(value, outFeature, &info.img) && value.length() > 0)
			{
				switch (info.type)
				{
				case PRICE:
					info.price = std::stoi(value);
					break;
				case TIME:
				if(value.length() >= 8)
				{
					int startPos = value.length() - 8;
					int hour = Convert2Int(value, startPos, startPos + 2);
					int minute = Convert2Int(value, startPos + 3, startPos + 5);
					int second = Convert2Int(value, startPos + 6, startPos + 8);
					info.time.hour = hour;
					info.time.minute = minute;
					info.time.second = second;
				}
				break;
				case QUANTITY:
					info.quantity = std::stoi(value);
					break;
				case PRICE_RANGE:
				{
					int sep = value.find('|');
					info.priceRange.SetLowest(Convert2Int(value, 0, sep));
					info.priceRange.SetHighest(Convert2Int(value, sep + 1, value.length()));
				}
				break;
				}//switch

				if (oldValue != info.price)
				{
					mUpdateFields |= 1 << index;
				}
			}//if (mRecognizer->RecognizeEx(value, outFeature, &info.img))
		}//if (!IsEmptyRect(info.rect))

	}
}

int InfoEngine::Convert2Int(const std::string& str, int from, int to)
{
	int value = 0, sign = 1;
	if('-' == str.at(from))
	{
		sign = -1;
		++from;
	}
	else if('+' == str.at(from))
	{
		++from;
	}

	for(; from < to && std::isdigit(str.at(from)); ++from)
	{
		value *= 10;
		value += (str.at(from) - '0');
	}
	value *= sign;

	if(from == to)
	{
		return value;
	}

	return 0;
}