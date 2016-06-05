#include "StdAfx.h"
#include  <cctype>
#include <string>
#include <sstream>
#include <functional>
#include <chrono>
#include <fstream>
#include "InfoEngine.h"

InfoEngine* InfoEngine::GetInstance()
{
	static InfoEngine gInstance;
	return &gInstance;
}

void InfoEngine::Data::ToString(CString& out, size_t idx) const
{
	switch (type)
	{
	case PRICE_RANGE:
		{
			if (0 == idx)
			{
				out.Format("%d", priceRange.GetLowest());
			}
			else
			{
				out.Format("%d", priceRange.GetHighest());
			}
		}
		break;
	case PRICE:
		out.Format("%d", price);
		break;
	case TIME:
		out.Format("%02d:%02d:%02d.%03d", time.hour, time.minute, time.second, time.milliseconds);
		break;
	case QUANTITY:
		out.Format("%d", quantity);
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

	for (size_t idx = 0; idx < COUNT_OF_INFOS; ++idx)
	{
		auto& info = mInfoRects[idx];
		file << idx << " " << info.type << " " << info.rect.top << " " << info.rect.left << " " << info.rect.bottom << " " << info.rect.right << "\n";
	}
}

InfoEngine::InfoEngine()
	:mFininshCount(0)
{}

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
	//mIsRuning = true;
	//for (size_t iThread = 0; iThread < COUNT_OF_INFOS; ++iThread)
	//{
	//	mThreads[iThread] = std::make_shared<std::thread>(std::bind(&InfoEngine::CollectData, this, iThread));
	//}
}

void InfoEngine::Step(void)
{
	//mUpdateFields = 0;
	//mFininshCount.store(0);
	//mRunCondition.notify_all();
 //   
	//{
	//	std::unique_lock<std::mutex> lock(mFinishMutex);
	//	mFinishCondition.wait_for(lock, std::chrono::seconds(10), [this]() { return mFininshCount.load() == COUNT_OF_INFOS; });
	//}

	for (size_t iThread = 0; iThread < COUNT_OF_INFOS; ++iThread)
	{
		CollectData(iThread);
	}
	for (auto pListener : mListeners)
	{
		pListener->OnUpdate(mUpdateFields.load());
	}
}

void InfoEngine::Stop( void )
{
	//if (mIsRuning)
	//{
	//	mIsRuning = false;

	//	for (size_t iThread = 0; iThread < COUNT_OF_INFOS; ++iThread)
	//	{
	//		auto pThread = mThreads[iThread];
	//		if (nullptr != pThread && pThread->joinable())
	//		{
	//			pThread->join();
	//		}
	//	}
	//}
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
		//{
		//	std::unique_lock<std::mutex> lock(mRunMutex);
		//	mRunCondition.wait(lock);
		//}

		if (!IsEmptyRect(info.rect))
		{
			CString value, outFeature;
			info.img.CaptureRect(info.rect);
			if (mRecognizer->RecognizeEx(value, outFeature, &info.img) && value.GetLength() > 0)
			{
				switch (info.type)
				{
				case PRICE:
					info.price = _ttoi(value);
					break;
				case TIME:
				if(value.GetLength() >= 8)
				{
					int startPos = value.GetLength() - 8;
					int hour = Convert2Int(value, startPos, startPos + 2);
					int minute = Convert2Int(value, startPos + 3, startPos + 5);
					int second = Convert2Int(value, startPos + 6, startPos + 8);
					info.time.hour = hour;
					info.time.minute = minute;
					info.time.second = second;
				}
				break;
				case QUANTITY:
					info.quantity = _ttoi(value);
					break;
				case PRICE_RANGE:
				{
					int sep = value.Find('|');
					info.priceRange.SetLowest(Convert2Int(value, 0, sep));
					info.priceRange.SetHighest(Convert2Int(value, sep + 1, value.GetLength()));
				}
				break;
				}//switch

				mUpdateFields.fetch_or(1 << index);
			}//if (mRecognizer->RecognizeEx(value, outFeature, &info.img))
		}//if (!IsEmptyRect(info.rect))

		//mFininshCount.fetch_add(1);
		//{
		//	std::unique_lock<std::mutex> lock(mFinishMutex);
		//	mFinishCondition.notify_one();
		//}
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