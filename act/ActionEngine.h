#ifndef _ACTION_ENGINE_H
#define _ACTION_ENGINE_H
#include <afxwin.h>
#include <memory>
class CMouseSimulator;
class CKeyboardSimulator;
class ActionEngine
{
public:
	void SetPriceArea(const CRect& rect) { mPricePoint = rect.CenterPoint(); }
	void SetBidBtnArea(const CRect& rect) { mBidBtnPoint = rect.CenterPoint(); }
	bool InputPrice(int price);

	void SetSecurityCodeArea(const CRect& rect) { mSecurityCodePoint = rect.CenterPoint(); }
	void SetSecurityCodeConfirmBtnArea(const CRect& rect) { mSecurityCodeConfirmBtnPoint = rect.CenterPoint();  }
	bool InputSecurityCode(const CString& SecurityCode);

	void SetSecurityCodePicArea(const CRect& rect) { mSecurityCodePicPoint = rect.CenterPoint();  }
	bool RefreshSecurityCode();

	void SetBidResultBtnArea(const CRect& rect) { mBidResultBtnPoint = rect.CenterPoint(); }
	void CloseBidReslt();

	static ActionEngine* GetInstance();

private:
	ActionEngine();
	ActionEngine(const ActionEngine&);

	CPoint mPricePoint;
	CPoint mSecurityCodePoint;
	CPoint mBidBtnPoint;
	CPoint mSecurityCodeConfirmBtnPoint;
	CPoint mSecurityCodePicPoint;
	CPoint mBidResultBtnPoint;

	std::shared_ptr<CMouseSimulator> mMouseSimulator;
	std::shared_ptr<CKeyboardSimulator> mKeyboardSimulator;
};
#endif // !_ACTION_ENGINE_H