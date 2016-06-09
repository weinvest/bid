#include "stdafx.h"
#include "ActionEngine.h"
#include "inputSim\MouseSimulator.h"
#include "inputSim\KeyboardSimulator.h"
ActionEngine* ActionEngine::GetInstance()
{
	static ActionEngine gInstance;

	return &gInstance;
}

ActionEngine::ActionEngine()
	:mMouseSimulator(std::make_shared<CMouseSimulator>())
	,mKeyboardSimulator(std::make_shared<CKeyboardSimulator>())
{}

bool ActionEngine::InputPrice(int price)
{
	mKeyboardSimulator->KeyPress(VirtualKeyCode::BACK);
	mMouseSimulator->MoveMouseTo(mPricePoint.x, mPricePoint.y);
	mMouseSimulator->LeftButtonClick();

	CString strPrice;
	strPrice.Format("%d", price);
	mKeyboardSimulator->TextEntry(strPrice);

	mMouseSimulator->MoveMouseTo(mBidBtnPoint.x ,mBidBtnPoint.y);
	mMouseSimulator->LeftButtonClick();
	return true;
}

bool ActionEngine::InputSecurityCode(const CString& SecurityCode)
{
	mKeyboardSimulator->KeyPress(VirtualKeyCode::BACK);
	mMouseSimulator->MoveMouseTo(mSecurityCodePoint.x, mSecurityCodePoint.y);
	mMouseSimulator->LeftButtonClick();

	mKeyboardSimulator->TextEntry(SecurityCode);
	mMouseSimulator->MoveMouseTo(mSecurityCodeConfirmBtnPoint.x, mSecurityCodeConfirmBtnPoint.y);
	mMouseSimulator->LeftButtonClick();

	return true;
}

bool ActionEngine::RefreshSecurityCode()
{
	mMouseSimulator->MoveMouseTo(mSecurityCodePicPoint.x, mSecurityCodePicPoint.y);
	mMouseSimulator->LeftButtonClick();

	return true;
}

void ActionEngine::CloseBidReslt()
{
	mMouseSimulator->MoveMouseTo(mBidResultBtnPoint.x, mBidResultBtnPoint.y);
	mMouseSimulator->LeftButtonClick();
}