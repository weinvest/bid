#include "stdafx.h"
#include <fstream>
#include <sstream>
#include "ActionEngine.h"
#include "inputSim\MouseSimulator.h"
#include "inputSim\KeyboardSimulator.h"

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
{}

void ActionEngine::SetRect(size_t idx, const CRect& rect)
{
	mAreas[idx].rect = rect;
	mAreas[idx].center = rect.CenterPoint();
}

bool ActionEngine::InputPrice(int price)
{	
	mKeyboardSimulator->KeyPress(VirtualKeyCode::BACK);
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(BID_PRICE_INPUT_AREA));
	mMouseSimulator->LeftButtonClick();

	CString strPrice;
	strPrice.Format("%d", price);
	mKeyboardSimulator->TextEntry(strPrice);

	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(BID_PRICE_SUBMIT_BUTTON_AREA));
	mMouseSimulator->LeftButtonClick();
	return true;
}

bool ActionEngine::InputSecurityCode(const CString& SecurityCode)
{
	mKeyboardSimulator->KeyPress(VirtualKeyCode::BACK);
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(SECURITY_CODE_INPUT_AREA));
	mMouseSimulator->LeftButtonClick();

	mKeyboardSimulator->TextEntry(SecurityCode);
	mMouseSimulator->MoveMouseTo(AREA_COORDINATE_X_Y(SECURITY_CODE_CONFIRM_BUTTON_AREA));
	mMouseSimulator->LeftButtonClick();

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

			auto& info = mAreas[index];
			s >> info.rect.top >> info.rect.left >> info.rect.bottom >> info.rect.right;
			info.center = info.rect.CenterPoint();
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
}