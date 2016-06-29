// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/

#include "StdAfx.h"
#include "ScreenImage.h"
#include "afx.h"
#include "Log.h"
/****************************************************************************************
 Function:   CScreenImage::CaptureRect
 Purpose:    captures a screen rectangle
 Parameters: rect: screen rectangle to be captured
 Return:     non-zero value if successful
****************************************************************************************/
CScreenImage::CScreenImage()
{
	mScreenDC = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	mDestDC = ::CreateCompatibleDC(mScreenDC);

	mDestBMP = mOldBMP = 0;

}

BOOL CScreenImage::CaptureRectEx(const CRect& rect)
{
   // detach and destroy the old bitmap if any attached
  // CImage::Destroy();
   // create a compatible bitmap and select it in the memory DC
   if(0 == mDestBMP)
   {
	   mDestBMP = ::CreateCompatibleBitmap(mScreenDC, rect.Width(), rect.Height());
   }
   else
   {
       Detach();
   }

   mOldBMP = (HBITMAP)::SelectObject(mDestDC, mDestBMP);
   // bit-blit from screen to memory device context
   // note: CAPTUREBLT flag is required to capture layered windows
   DWORD dwRop = SRCCOPY | CAPTUREBLT;
   BOOL bRet = ::BitBlt(mDestDC, 0, 0, rect.Width(), rect.Height(), 
                        mScreenDC, 
                        rect.left, rect.top, dwRop);
   // attach bitmap handle to this object
   Attach(mDestBMP);
   ::SelectObject(mDestDC, mOldBMP);


   return bRet;
}

void CScreenImage::Output()
{
   for(int i = 0; i < GetHeight(); ++i)
   {
	   for(int j = 0; j < GetWidth(); ++j)
	   {
		   COLORREF c = GetPixel(j, i);
		   BYTE r = GetRValue(c);
		   BYTE g = GetGValue(c);
		   BYTE b = GetBValue(c);
		   if(255 == r && 255 == g && 255 == b)
		   {
			   //TRACE(" ");
			   LOG_INFO0(" ");
		   }
		   else
		   {
			   //TRACE("1");
			   LOG_INFO0("1");
		   }
		   //TRACE("(%3d,%3d,%3d)", r,g,b);
	   }
	   LOG_INFO0("\n");
   }
   return;
   for(int w = 0; w < GetWidth(); ++w)
   {
	   int i = 0;
	   for(int h = 0; h < GetHeight(); ++h)
	   {
		   COLORREF c = GetPixel(w, h);
		   BYTE r = GetRValue(c);
		   BYTE g = GetGValue(c);
		   BYTE b = GetBValue(c);
		   if(255 == r && 255 == g && 255 == b)
		   {
		   }
		   else
		   {
			   i+=1;
		   }
	   }
	   TRACE("%d",i);
   }
}
#define OUTPUT_LOG
void CScreenImage::ScanAndSplit(CString& allFeature, std::vector<Feature>& outFeatures, int sepCount)
{
	outFeatures.emplace_back();
#ifdef OUTPUT_LOG
	Output();
#endif
	bool ignoreZero = true;
	int continueZero = 0;
	for(int w = 0; w < GetWidth(); ++w)
	{
		int feature = 0;
		for(int h = 0; h < GetHeight(); ++h)
		{
			COLORREF c = GetPixel(w, h);
			feature = GetColorFeature(c, feature);
		}

		allFeature.AppendChar('0' + feature);
		if(0 == feature)
		{
			++continueZero;
			if(ignoreZero)
			{
			    continue;
			}
		}
		else if(ignoreZero)
		{
			continueZero = 0;
			ignoreZero = false;
		}

		if(continueZero == sepCount && outFeatures.back().verticalLength >= sepCount)
		{
			auto& currentFeature = outFeatures.back();
			int newLen =  currentFeature.verticalLength - sepCount + 1;

			AppendHorizonFeature(allFeature, sepCount, newLen, currentFeature);


			outFeatures.emplace_back();
		}
		
		if(0 != feature || continueZero < sepCount)
		{
		    outFeatures.back().Append2Vertical(feature + '0');
			continueZero = 0 != feature ? 0 : continueZero;
		}
	}
	
	if(0 == outFeatures.back().verticalLength)
	{
		outFeatures.pop_back();
	}
	else
	{
		auto& currentFeature = outFeatures.back();
		AppendHorizonFeature(allFeature, continueZero, currentFeature.verticalLength, currentFeature);
	}

#ifdef OUTPUT_LOG
	LOG_INFO0(allFeature);
	LOG_INFO0("\n");
#endif
}

void CScreenImage::HorizonScan(Feature& horizon, int from, int to)
{
	for(int h = 0; h < GetHeight(); ++h)
	{
		int feature = 0;
		for(int w = from; w < to; ++w)
		{
			COLORREF c = GetPixel(w, h);
			feature = GetColorFeature(c, feature);
		}

		horizon.Append2Horizon('0' + feature);
	}
	
	horizon.Trim('0', horizon.horizon, horizon.horizonLength);
}

BOOL CScreenImage::CaptureRect(const CRect& rect)
{
   // detach and destroy the old bitmap if any attached
   CImage::Destroy();
   
   // create a screen and a memory device context
   HDC hDCScreen = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
   HDC hDCMem = ::CreateCompatibleDC(hDCScreen);
   // create a compatible bitmap and select it in the memory DC
   HBITMAP hBitmap = 
      ::CreateCompatibleBitmap(hDCScreen, rect.Width(), rect.Height());
   HBITMAP hBmpOld = (HBITMAP)::SelectObject(hDCMem, hBitmap);

   // bit-blit from screen to memory device context
   // note: CAPTUREBLT flag is required to capture layered windows
   DWORD dwRop = SRCCOPY | CAPTUREBLT;
   BOOL bRet = ::BitBlt(hDCMem, 0, 0, rect.Width(), rect.Height(), 
                        hDCScreen, 
                        rect.left, rect.top, dwRop);
   // attach bitmap handle to this object
   Attach(hBitmap);

   // restore the memory DC and perform cleanup
   ::SelectObject(hDCMem, hBmpOld);
   ::DeleteDC(hDCMem);
   ::DeleteDC(hDCScreen);
   return bRet;
}

/****************************************************************************************
 Function:   CScreenImage::CaptureScreen
 Purpose:    captures the whole screen
 Parameters: none
 Return:     non-zero value if successful
****************************************************************************************/
BOOL CScreenImage::CaptureScreen()
{
   CRect rect(0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
   return CaptureRect(rect);
}

/****************************************************************************************
 Function:   CScreenImage::CaptureWindow
 Purpose:    captures a window
 Parameters: hWnd - the window handle
 Return:     non-zero value if successful
****************************************************************************************/
BOOL CScreenImage::CaptureWindow(HWND hWnd)
{
   BOOL bRet = FALSE;
   if(::IsWindow(hWnd))
   {
      CRect rect;
      ::GetWindowRect(hWnd, rect);
      bRet = CaptureRect(rect);
   }
   return bRet;
}

int CScreenImage::GetColorFeature( COLORREF c, int feature )
{
	BYTE r = GetRValue(c);
	BYTE g = GetGValue(c);
	BYTE b = GetBValue(c);
	if(250 > r || 250 > g || 250 > b)
	{
		++feature;
	}	
	return feature;
}

void CScreenImage::AppendHorizonFeature( CString &allFeature, int sepCount, int newLen, Feature &currentFeature )
{
	int horizonTo = allFeature.GetLength() - sepCount;
	int horizonFrom = horizonTo - newLen;

	HorizonScan(currentFeature, horizonFrom, horizonTo);
}

