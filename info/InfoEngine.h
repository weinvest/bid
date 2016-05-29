#ifndef _INFO_ENGINE_H
#define _INFO_ENGINE_H
#include <set>
#include <array>

class IInfoListener
{
public:
	virtual ~IInfoListener() {}

	virtual void OnUpdate(size_t updateFields) = 0;
};

class InfoEngine
{
public: = 
	enum DataType
	{
		PRICE,
		TIME,
		QUANTITY
	};

	struct Data
	{
		DataType type;
		CRect rect;
		union
		{
			int price;
			int quantity;
			int time;
		};
	};

	static const size_t CURRENT_TIME_RECT_INDEX = 0;
	static const size_t CURRENT_LOWEST_PRICE_INDEX = 1;
	static const size_t CURRENT_LOWEST_PRICE_TIME_INDEX = 2;
	static const size_t PRICE_RANGE_INDEX = 4;

	void SetRect(size_t idx, CRect& rect, DataType dtype);

	void Registe(IInfoListener* pListener);
	void UnRegiste(IInfoListener* pListener);

	void Start( void );

	void Stop( void );
private:
	void CollectData(size_t index);

	std::array<Data, 8> mInfoRects;

	std::set<IInfoListener*> mListeners;
};
#endif