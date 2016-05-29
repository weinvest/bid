// BidWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyBid.h"
#include "BidWorkDlg.h"
#include "afxdialogex.h"
#include "ScreenSelectionDialog.h"
#include "OCREngine.h"
// CBidWorkDlg dialog

IMPLEMENT_DYNAMIC(CBidWorkDlg, CDialogEx)

CBidWorkDlg::CBidWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBidWorkDlg::IDD, pParent)
{

}

CBidWorkDlg::~CBidWorkDlg()
{
}

void CBidWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BID_FONT_NAME, mFontNameCombox);
}


BEGIN_MESSAGE_MAP(CBidWorkDlg, CDialogEx)
	ON_STN_DBLCLK(IDC_BIDPI_CURRENT_TIME, &CBidWorkDlg::OnDoubleClickCaptureCurrentTime)
	ON_STN_DBLCLK(IDC_BIDPI_LOWEST_PRICE, &CBidWorkDlg::OnDoubleClickCaptureLowestPrice)
	ON_STN_DBLCLK(IDC_BIDPI_LOWEST_PRICE_TIME, &CBidWorkDlg::OnDoubleClickCaptureLowestPriceTime)
	ON_STN_DBLCLK(IDC_BIDPI_PRICE_CONFIRM, &CBidWorkDlg::OnDoubleClickCapturePriceConfirm)
	ON_STN_DBLCLK(IDC_BIDPI_PRICE_INPUT, &CBidWorkDlg::OnDoubleClickCapturePriceInput)
	ON_STN_DBLCLK(IDC_BIDPI_PRICE_RANGE, &CBidWorkDlg::OnDoubleClickCapturePriceRange)
	ON_STN_DBLCLK(IDC_BIDPI_VERIFYBOX, &CBidWorkDlg::OnDoubleClickCaptureVerifybox)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_BID_FONT_NAME, &CBidWorkDlg::OnFontNameChanged)
END_MESSAGE_MAP()


// CBidWorkDlg message handlers


void CBidWorkDlg::OnDoubleClickCaptureCurrentTime()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mCurrentTimeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}


void CBidWorkDlg::OnDoubleClickCaptureLowestPrice()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mLowestPriceImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}


void CBidWorkDlg::OnDoubleClickCaptureLowestPriceTime()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mLowestPriceTimeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}


void CBidWorkDlg::OnDoubleClickCapturePriceConfirm()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mPriceConfirmImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}


void CBidWorkDlg::OnDoubleClickCapturePriceInput()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mPriceInputImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}


void CBidWorkDlg::OnDoubleClickCapturePriceRange()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mPriceRangeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}


void CBidWorkDlg::OnDoubleClickCaptureVerifybox()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mVerifyBoxImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
}


void CBidWorkDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	DrawImage(IDC_BIDPI_LOWEST_PRICE, mLowestPriceImg);
	DrawImage(IDC_BIDPI_LOWEST_PRICE_TIME, mLowestPriceTimeImg);
	DrawImage(IDC_BIDPI_PRICE_RANGE, mPriceRangeImg);
	DrawImage(IDC_BIDPI_CURRENT_TIME, mCurrentTimeImg);
	DrawImage(IDC_BIDPI_PRICE_INPUT, mPriceInputImg);
	DrawImage(IDC_BIDPI_PRICE_CONFIRM, mPriceConfirmImg);
	DrawImage(IDC_BIDPI_VERIFYBOX, mVerifyBoxImg);
	DrawImage(IDC_SCREEN_TEST, mScreenTest);
}


void CBidWorkDlg::DrawImage(int id, CScreenImage& img)
{
	if(!img.IsNull())
	{
		CWnd *pWnd = GetDlgItem(id);
		CDC *pDC = pWnd->GetDC();

		CRect rect;
		pWnd->GetClientRect(&rect);
		pDC->SetStretchBltMode(STRETCH_HALFTONE);
		img.Draw(pDC->m_hDC, rect);
	}
}

void CBidWorkDlg::ReBuildNameCombox()
{
	mFontNameCombox.Clear();
	const auto& allRecognizer = COCREngine::GetInstance()->GetAllRecognizer();
	if(0 != allRecognizer.size())
	{
		for(auto itRecognize = allRecognizer.begin(); itRecognize != allRecognizer.end(); ++itRecognize)
		{
			mFontNameCombox.AddString(itRecognize->first);
		}
		COCREngine::GetInstance()->Registe(this);

		mFontNameCombox.SetCurSel(0);
		OnFontNameChanged();
	}
}

BOOL CBidWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	ReBuildNameCombox();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBidWorkDlg::OnFontNameChanged()
{
	auto index = mFontNameCombox.GetCurSel();
	CString name;
	mFontNameCombox.GetLBText(index, name);

	//mCurrentRecognizer = COCREngine::GetInstance()->GetRecognizer(name);
}

void CBidWorkDlg::OnNewRecognizer(const CString& name, CRecognizer* pRecognizer)
{
	mFontNameCombox.AddString(name);
}

void CBidWorkDlg::OnDelRecognizer(const CString& name, CRecognizer* pRecognizer)
{}
