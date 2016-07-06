#ifndef _INFO_ENGINE_H
#define _INFO_ENGINE_H
#include <set>
#include <array>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <memory>
#include "ScreenImage.h"
#include "Recognizer.h"

class IInfoListener;

struct BidTime
{
	uint32_t hour:6;
	uint32_t minute:6;
	uint32_t second:6;
	uint32_t milliseconds:14;

	friend bool operator < (const BidTime& lhs, const BidTime& rhs);
	void Parse(const std::string& time);
};

struct PriceRange
{
	static const int PRICE_BASE = 80000;
	int lowest:16;
	int highest:16;

	void SetLowest(int low) { lowest = low - PRICE_BASE;}
	void SetHighest(int high) { highest = high - PRICE_BASE; }

	int GetLowest() const { return lowest + PRICE_BASE; }
	int GetHighest() const { return highest + PRICE_BASE;}
};

class InfoEngine
{
public: 
	enum DataType
	{
		PRICE,
		TIME,
		QUANTITY,
		PRICE_RANGE
	};

	struct Data
	{
		DataType type;
		CRect rect;
		union
		{
			int price;
			int quantity;
			BidTime time;
			PriceRange priceRange;
		};
		CScreenImage img;

		void ToString(std::string& out, size_t idx) const;
	};

	static const size_t CURRENT_TIME_RECT_INDEX = 0;
	static const size_t CURRENT_LOWEST_PRICE_INDEX = 1;
	static const size_t CURRENT_LOWEST_PRICE_TIME_INDEX = 2;
	static const size_t CURRENT_ACCEPTABLE_PRICE_RANGE = 3;
	static const size_t COUNT_OF_INFOS = 4;

	static InfoEngine* GetInstance();

	void Load(const CString& path);
	void Save(const CString& path);

	void SetReferencePoint(size_t x, size_t y);
	void SetRect(size_t idx, CRect& rect, DataType dtype);

	void SetRecognizer(CRecognizer* pRecognizer) { mRecognizer = pRecognizer; }
	void Registe(IInfoListener* pListener);
	void UnRegiste(IInfoListener* pListener);

	const Data& GetInfo(size_t idx) const { return mInfoRects[idx]; }

	void Step(void);

	void Pause(void);
	void Resume(void);

	static bool IsEmptyRect(CRect& rect);
	static int Convert2Int(const std::string& str, int from, int to);
private:
	InfoEngine();

	void CollectData(size_t index);

	std::array<Data, COUNT_OF_INFOS> mInfoRects;
	
	std::set<IInfoListener*> mListeners;
	CRecognizer* mRecognizer;
	bool mIsRuning;
	int mUpdateFields;

	CPoint mReferencePoint;
};

class IInfoListener
{
public:
	virtual ~IInfoListener() {}

	virtual void OnUpdate(size_t updateFields) = 0;
};
#endif