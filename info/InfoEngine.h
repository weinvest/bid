#ifndef _INFO_ENGINE_H
#define _INFO_ENGINE_H
#include <set>
#include <array>
#include "ScreenImage.h"
#include "Recognizer.h"

class IInfoListener
{
public:
	virtual ~IInfoListener() {}

	virtual void OnUpdate(size_t updateFields) = 0;
};

struct BidTime
{
    int hour:6;
	int minute:6;
	int second:6;
	int milliseconds:14;
};

struct PriceRange
{
	static const int PRICE_BASE = 50000;
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
	};

	static const size_t CURRENT_TIME_RECT_INDEX = 0;
	static const size_t CURRENT_LOWEST_PRICE_INDEX = 1;
	static const size_t CURRENT_LOWEST_PRICE_TIME_INDEX = 2;
	static const size_t PRICE_RANGE_INDEX = 4;

	static InfoEngine* GetInstance();

	void SetRect(size_t idx, CRect& rect, DataType dtype);

	void SetRecognizer(CRecognizer* pRecognizer) { mRecognizer = pRecognizer; }
	void Registe(IInfoListener* pListener);
	void UnRegiste(IInfoListener* pListener);

	void Start( void );

	void Stop( void );
private:
	static bool IsEmptyRect(CRect& rect);
	static int Convert2Int(CString& str, int from, int to);
	void CollectData(size_t index);


	std::array<Data, 8> mInfoRects;

	std::set<IInfoListener*> mListeners;
	CRecognizer* mRecognizer;
};
#endif