#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <thread>
#include "ActionEngine.h"
#include "inputSim\MouseSimulator.h"
#include "inputSim\KeyboardSimulator.h"
#include "Log.h"
#define AREA_COORDINATE_X_Y(idx) X(idx),Y(idx)
ActionEngine* ActionEngine::GetInstance()
{
	static ActionEngine gInstance;

	return &gInstance;
}

ActionEngine::ActionEngine()
	:mReferencePoint{ -1, -1 }
	,mMouseSimulator(std::make_shared<CMouseSimulator>())
	,mKeyboardSimulator(std::make_shared<CKeyboardSimulator>())
	,mXFactor(1)
	,mYFactor(1)
{
	mXFactor = (65535.0 / GetSystemMetrics(SM_CXSCREEN));
	mYFactor = (65535.0 / GetSystemMetrics(SM_CYSCREEN));
}

void ActionEngine::SetRect(size_t idx, const CRect& rect)
{
	mAreas[idx].rect = rect;
	mAreas[idx].center = rect.CenterPoint();

	mAreas[idx].center.x = mAreas[idx].center.x * mXFactor + 0.5;
	mAreas[idx].center.y = mAreas[idx].center.y * mYFactor + 0.5;
}

void ActionEngine::CleanEditBox()
{
	mKeyboardSimulator->KeyDown(VirtualKeyCode::CONTROL);
	mKeyboardSimulator->KeyDown(VirtualKeyCode::SHIFT);
	mKeyboardSimulator->KeyPress(VirtualKeyCode::VK_A);
	//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	mKeyboardSimulator->KeyUp(VirtualKeyCode::SHIFT);
	mKeyboardSimulator->KeyUp(VirtualKeyCode::CONTROL);
	mKeyboardSimulator->KeyPress(VirtualKeyCode::BACK);
	//std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

bool ActionEngine::InputPrice(int price)
{	
	//LOG_INFO("input price at (%d,%d):%d", AREA_COORDINATE_X_Y(BID_PRICE_INPUT_AREA),price);
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(BID_PRICE_INPUT_AREA));
	mMouseSimulator->LeftButtonClick();
	CleanEditBox();

	CString strPrice;
	strPrice.Format(L"%d", price);
	mKeyboardSimulator->TextEntry((LPCTSTR)strPrice);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(BID_PRICE_SUBMIT_BUTTON_AREA));
	//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	mMouseSimulator->LeftButtonClick();
	return true;
}

bool ActionEngine::InputSecurityCode(const CString& SecurityCode)
{
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(SECURITY_CODE_INPUT_AREA));
	mMouseSimulator->LeftButtonClick();

	mKeyboardSimulator->TextEntry(SecurityCode);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(SECURITY_CODE_CONFIRM_BUTTON_AREA));
	//mMouseSimulator->LeftButtonClick();

	return true;
}

bool ActionEngine::RefreshSecurityCode()
{
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(SECURITY_CODE_PICTURE_AREA));
	mMouseSimulator->LeftButtonClick();

	return true;
}

void ActionEngine::CloseBidReslt()
{
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(BID_RESULT_CONFIRM_BUTTON_AREA));
	mMouseSimulator->LeftButtonClick();
	mKeyboardSimulator->KeyPress(VirtualKeyCode::BACK);
}

void ActionEngine::Load(const CString& path)
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
			size_t index = 0;
			s >> index;

			CRect rect;
			s >> rect.top >> rect.left >> rect.bottom >> rect.right;
			SetRect(index, rect);
		}
	}
}

void ActionEngine::Save(const CString& path)
{
	std::ofstream file((LPCTSTR)path, std::ios_base::binary | std::ios_base::out);

	file << mReferencePoint.x << " " << mReferencePoint.y << '\n';
	for (size_t idx = 0; idx < COUNT_OF_AREA; ++idx)
	{
		auto& info = mAreas[idx];
		file << idx << " " << info.rect.top << " " << info.rect.left << " " << info.rect.bottom << " " << info.rect.right << "\n";
	}
}

void ActionEngine::SetReferencePoint(size_t x, size_t y)
{
	if (-1 != mReferencePoint.x && -1 != mReferencePoint.y)
	{
		auto diff = CPoint(x, y) - mReferencePoint;

		for (size_t idx = 0; idx < COUNT_OF_AREA; ++idx)
		{
			auto& info = mAreas[idx];
			info.rect += diff;
			info.center = info.rect.CenterPoint();
		}
	}
	else
	{
		mReferencePoint.x = x;
		mReferencePoint.y = y;
	}
}