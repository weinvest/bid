// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/

#pragma once

#include <atlimage.h>
#include <vector>
#include "Common.h"
class CScreenImage : public CImage
{
public:
	CScreenImage();

    BOOL CaptureRect(const CRect& rect) throw();
    BOOL CaptureScreen() throw();
    BOOL CaptureWindow(HWND hWnd) throw();

	BOOL CaptureRectEx(const CRect& rect) throw();

	void ScanAndSplit(CString& allFeature, std::vector<Feature>& outFeatures, int sepCount);

	void AppendHorizonFeature( CString &allFeature, int sepCount, int newLen, Feature &currentFeature );

	int GetColorFeature( COLORREF c, int feature );

	void Output();
private:
	void HorizonScan(Feature& horizon, int from, int to);
	HDC mScreenDC;
	HDC mDestDC;

	HBITMAP mOldBMP;
	HBITMAP mDestBMP;
};
