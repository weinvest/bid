// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/

#pragma once

#include <atlimage.h>
#include <vector>

class CScreenImage : public CImage
{
public:
	CScreenImage();

    BOOL CaptureRect(const CRect& rect) throw();
    BOOL CaptureScreen() throw();
    BOOL CaptureWindow(HWND hWnd) throw();

	BOOL CaptureRectEx(const CRect& rect) throw();

	void ScanAndSplit(CString& allFeature, std::vector<CString>& outFeatures, int sepCount);

	void AppendHorizonFeature( CString &allFeature, int sepCount, int newLen, CString &currentFeature );

	int GetColorFeature( COLORREF c, int feature );

	void Output();
private:
	void HorizonScan(CString& horizon, int from, int to);
	HDC mScreenDC;
	HDC mDestDC;

	HBITMAP mOldBMP;
	HBITMAP mDestBMP;
};
