#ifndef _ACTION_ENGINE_H
#define _ACTION_ENGINE_H
#include <afxwin.h>
#include <array>
#include <memory>
class CMouseSimulator;
class CKeyboardSimulator;
class ActionEngine
{
public:
	static const size_t BID_PRICE_INPUT_AREA = 0;
	static const size_t BID_PRICE_SUBMIT_BUTTON_AREA = 1;
	static const size_t SECURITY_CODE_INPUT_AREA = 2;
	static const size_t SECURITY_CODE_CONFIRM_BUTTON_AREA = 3;
	static const size_t SECURITY_CODE_PICTURE_AREA = 4;
	static const size_t BID_RESULT_CONFIRM_BUTTON_AREA = 5;
	static const size_t COUNT_OF_AREA = 6;

	struct DataType
	{
		CRect rect;
		CPoint center;
	};

	void CleanEditBox();
	void SetRect(size_t idx, const CRect& rect);
	void SetReferencePoint(size_t x, size_t y);

	bool InputPrice(int price);

	bool InputSecurityCode(const CString& SecurityCode);

	bool RefreshSecurityCode();

	void CloseBidReslt();

	static ActionEngine* GetInstance();

	void Load(const CString& path);
	void Save(const CString& path);
private:
	ActionEngine();
	ActionEngine(const ActionEngine&);

	int X(size_t idx) const { return mAreas[idx].center.x; }
	int Y(size_t idx) const { return mAreas[idx].center.y; }

	CPoint mReferencePoint;
	std::array<DataType, COUNT_OF_AREA> mAreas;

	std::shared_ptr<CMouseSimulator> mMouseSimulator;
	std::shared_ptr<CKeyboardSimulator> mKeyboardSimulator;

	double mXFactor;
	double mYFactor;
};
#endif // !_ACTION_ENGINE_H