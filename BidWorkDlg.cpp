// BidWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyBid.h"
#include "BidWorkDlg.h"
#include "afxdialogex.h"
#include "ScreenSelectionDialog.h"
#include "OCREngine.h"
#include "InfoEngine.h"
// CBidWorkDlg dialog

IMPLEMENT_DYNAMIC(CBidWorkDlg, CDialogEx)

CBidWorkDlg::CBidWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBidWorkDlg::IDD, pParent)
	, mConfPath("conf/rects")
{

}

CBidWorkDlg::~CBidWorkDlg()
{
}

void CBidWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BID_FONT_NAME, mFontNameCombox);
	DDX_Control(pDX, IDC_BID_DATA, mDataListCtrl);
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

	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_TIME_RECT_INDEX, rect, InfoEngine::TIME);
	mCurrentTimeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mConfPath);
}


void CBidWorkDlg::OnDoubleClickCaptureLowestPrice()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_LOWEST_PRICE_INDEX, rect, InfoEngine::PRICE);
	mLowestPriceImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mConfPath);
}


void CBidWorkDlg::OnDoubleClickCaptureLowestPriceTime()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_LOWEST_PRICE_TIME_INDEX, rect, InfoEngine::TIME);
	mLowestPriceTimeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mConfPath);
}


void CBidWorkDlg::OnDoubleClickCapturePriceConfirm()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mPriceConfirmImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mConfPath);
}


void CBidWorkDlg::OnDoubleClickCapturePriceInput()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mPriceInputImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mConfPath);
}


void CBidWorkDlg::OnDoubleClickCapturePriceRange()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_ACCEPTABLE_PRICE_RANGE , rect, InfoEngine::PRICE_RANGE);
	mPriceRangeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mConfPath);
}


void CBidWorkDlg::OnDoubleClickCaptureVerifybox()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mVerifyBoxImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mConfPath);
}

void CBidWorkDlg::OnUpdate(size_t updateFields)
{
	CString text("OK");
	for (size_t iInfo = 0; iInfo < InfoEngine::COUNT_OF_INFOS; ++iInfo)
	{
		if (updateFields & (1 << iInfo))
		{
			auto& info = InfoEngine::GetInstance()->GetInfo(iInfo);
			info.ToString(text, 0);
			mDataListCtrl.SetItemText(iInfo, 1, text);

			if (InfoEngine::PRICE_RANGE == info.type)
			{
				info.ToString(text, 1);
				mDataListCtrl.SetItemText(iInfo, 2, text);
			}
		}
	}
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
	
	ReBuildNameCombox();

	mDataListCtrl.DeleteAllItems();
	char szColumnName[3][10] = { "Name", "Value1", "Value2" };
	LV_COLUMN lvColumn;
	CRect listRect;
	mDataListCtrl.GetWindowRect(&listRect);
	for (size_t iColumn = 0; iColumn < 3; ++iColumn)
	{
		lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
		lvColumn.fmt = LVCFMT_LEFT;
		lvColumn.pszText = szColumnName[iColumn];
		lvColumn.iSubItem = iColumn;
		lvColumn.iOrder = iColumn;

		mDataListCtrl.InsertColumn(iColumn, &lvColumn);
		mDataListCtrl.SetColumnWidth(iColumn, listRect.Width() * 2/ 7);
	}
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_TIME_RECT_INDEX, "系统目前时间");
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_LOWEST_PRICE_INDEX, "目前最低可成交价");
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_LOWEST_PRICE_TIME_INDEX, "最低可成交价出价时间");
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_ACCEPTABLE_PRICE_RANGE, "目前数据库接收的价格区间");

	InfoEngine::GetInstance()->Registe(this);
	InfoEngine::GetInstance()->Load(mConfPath);
	mInfoThread = std::make_shared<std::thread>([this]()
	{
		auto pInfoEngine = InfoEngine::GetInstance();
		pInfoEngine->Start();

		while (true)
		{
			pInfoEngine->Step();
		}

		pInfoEngine->Stop();
	});
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBidWorkDlg::OnFontNameChanged()
{
	auto index = mFontNameCombox.GetCurSel();
	CString name;
	mFontNameCombox.GetLBText(index, name);

	auto pRecognizer = COCREngine::GetInstance()->GetRecognizer(name);
	InfoEngine::GetInstance()->SetRecognizer(pRecognizer);
}

void CBidWorkDlg::OnNewRecognizer(const CString& name, CRecognizer* pRecognizer)
{
	mFontNameCombox.AddString(name);
}

void CBidWorkDlg::OnDelRecognizer(const CString& name, CRecognizer* pRecognizer)
{}
