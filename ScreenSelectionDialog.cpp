// ScreenSelectionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenSelectionDialog.h"


// CScreenSelectionDialog dialog

IMPLEMENT_DYNAMIC(CScreenSelectionDialog, CDialog)

CScreenSelectionDialog::CScreenSelectionDialog(CRect& rect)
   : CDialog(IDD_SCREEN_SELECTION), m_rect(rect)
   , mDrawRect(false)
   , mRectBrush(RGB(0,0,0))
{
   rect.SetRectEmpty();
}

CScreenSelectionDialog::~CScreenSelectionDialog()
{
}


BEGIN_MESSAGE_MAP(CScreenSelectionDialog, CDialog)
   ON_WM_ERASEBKGND()
   ON_WM_PAINT()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CScreenSelectionDialog message handlers

BOOL CScreenSelectionDialog::OnInitDialog()
{
   CDialog::OnInitDialog();
   
   AfxGetMainWnd()->ShowWindow(SW_HIDE);
   ::Sleep(333);
   if(!m_image.CaptureScreen())
   {
      EndDialog(IDCANCEL);
   }
   const int cx = ::GetSystemMetrics(SM_CXSCREEN);
   const int cy = ::GetSystemMetrics(SM_CYSCREEN);
   SetWindowPos(&CWnd::wndTopMost, 0, 0, cx, cy, SWP_SHOWWINDOW);

   return TRUE;
}

BOOL CScreenSelectionDialog::OnEraseBkgnd(CDC* pDC)
{
   return TRUE;
}

void CScreenSelectionDialog::OnPaint()
{
   CPaintDC dc(this);
   m_image.Draw(dc.GetSafeHdc(), 0, 0);

   if(mDrawRect)
   {
	   CPen pen(PS_SOLID, 1, RGB(50, 50, 50));
	   CPen* oldPen = dc.SelectObject(&pen);

	   dc.MoveTo(m_rect.left - 1, m_rect.top - 1);
	   dc.LineTo(m_rect.right, m_rect.top - 1);
	   dc.LineTo(m_rect.right, m_rect.bottom);
	   dc.LineTo(m_rect.left - 1, m_rect.bottom);
	   dc.LineTo(m_rect.left - 1, m_rect.top - 1);

	   dc.SelectObject(oldPen);
   }
   else
   {
       m_tracker.Draw(&dc);
   }
}

void CScreenSelectionDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect oldRect = m_rect;
	mDrawRect = false;
    m_tracker.TrackRubberBand(this, point);
	
    m_tracker.GetTrueRect(m_rect);
	if(m_rect.top == m_rect.bottom || m_rect.left == m_rect.right)
	{
		m_rect = oldRect;
	}
	else
	{
		mDrawRect = true;
	    Invalidate();
	}
}


void CScreenSelectionDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	EndDialog(IDOK);
}


void CScreenSelectionDialog::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(m_rect.IsRectEmpty())
	{
		EndDialog(IDOK);
	}
	else
	{
        mDrawRect = false;
	    Invalidate();
	}
}
