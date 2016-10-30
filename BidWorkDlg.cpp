// BidWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyBid.h"
#include "BidWorkDlg.h"
#include "afxdialogex.h"
#include "ScreenSelectionDialog.h"
#include "OCREngine.h"
#include "InfoEngine.h"
#include "ActionEngine.h"
#include "StrategyManager.h"
#include "IStrategy.h"
#include "Log.h"
#include "PixelBitAnalyze.h"
// CBidWorkDlg dialog

IMPLEMENT_DYNAMIC(CBidWorkDlg, CDialogEx)

CBidWorkDlg::CBidWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBidWorkDlg::IDD, pParent)
	, mInfoConfPath("conf/info_rects")
	, mActionConfPath("conf/act_rects")
	, mStrategyConfPath("conf/strategies")
	, mRectChanged(false)
{

}
static bool isCollecting = false;


CBidWorkDlg::~CBidWorkDlg()
{
	if (mInfoThread)
	{
		mInfoThread->detach();
		isCollecting = false;
	}
}

void CBidWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BID_FONT_NAME, mFontNameCombox);
	DDX_Control(pDX, IDC_BID_DATA, mDataListCtrl);
	DDX_Control(pDX, IDC_COMBOX_STRATEGY, mStrategyCombox);
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
	ON_STN_DBLCLK(IDC_BIDPI_BID_RESULT_BUTTON, &CBidWorkDlg::OnDoubleClickBidResultButtonArea)
	ON_STN_DBLCLK(IDC_BIDPI_POSTION_REFERENCE, &CBidWorkDlg::OnDoubleClickPostionReferenceArea)
	ON_STN_DBLCLK(IDC_BIDPI_VERIFY_CODE_INPUT, &CBidWorkDlg::OnDoubleClickVerifyCodeInputArea)
	ON_STN_DBLCLK(IDC_BIDPI_VERIFYCODE_CONFIRM, &CBidWorkDlg::OnDoubleClickVerifyCodeConfirmArea)
	ON_STN_DBLCLK(IDC_BIDPI_VERIFYCODE_PICTURE, &CBidWorkDlg::OnDoubleClickVerifyCodePictureArea)
	ON_BN_CLICKED(IDC_START_COLLECT_DATA, &CBidWorkDlg::OnBnClickedStartCollectData)
	ON_BN_CLICKED(IDC_VCODE_CONFIRM_BTN, &CBidWorkDlg::OnBnClickedVcodeConfirmBtn)
	ON_BN_CLICKED(IDC_VCODE_REJECT_BTN, &CBidWorkDlg::OnBnClickedVcodeRejectBtn)
	ON_BN_CLICKED(IDC_RE_BID, &CBidWorkDlg::OnBnClickedReBid)
END_MESSAGE_MAP()


// CBidWorkDlg message handlers


void CBidWorkDlg::OnDoubleClickCaptureCurrentTime()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_TIME_RECT_INDEX, rect, InfoEngine::TIME);
	mCurrentTimeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mInfoConfPath);
}


void CBidWorkDlg::OnDoubleClickCaptureLowestPrice()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_LOWEST_PRICE_INDEX, rect, InfoEngine::PRICE);
	mLowestPriceImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mInfoConfPath);
}


void CBidWorkDlg::OnDoubleClickCaptureLowestPriceTime()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_LOWEST_PRICE_TIME_INDEX, rect, InfoEngine::TIME);
	mLowestPriceTimeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mInfoConfPath);
}


void CBidWorkDlg::OnDoubleClickCapturePriceRange()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	InfoEngine::GetInstance()->SetRect(InfoEngine::CURRENT_ACCEPTABLE_PRICE_RANGE, rect, InfoEngine::PRICE_RANGE);
	mPriceRangeImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	InfoEngine::GetInstance()->Save(mInfoConfPath);
}

void CBidWorkDlg::OnDoubleClickCapturePriceConfirm()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetRect(ActionEngine::BID_PRICE_SUBMIT_BUTTON_AREA, rect);
	mPriceConfirmImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
}


void CBidWorkDlg::OnDoubleClickCapturePriceInput()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetRect(ActionEngine::BID_PRICE_INPUT_AREA, rect);
	mPriceInputImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
}



void CBidWorkDlg::OnDoubleClickCaptureVerifybox()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetRect(ActionEngine::SECURITY_CODE_INPUT_AREA, rect);
	mVerifyCodeInputImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
}

void CBidWorkDlg::OnUpdate(size_t updateFields)
{
	std::string text("OK");
	for (size_t iInfo = 0; iInfo < InfoEngine::COUNT_OF_INFOS; ++iInfo)
	{
		if (updateFields & (1 << iInfo))
		{
			auto& info = InfoEngine::GetInstance()->GetInfo(iInfo);
			info.ToString(text, 0);
			mDataListCtrl.SetItemText(iInfo, 1, CString(text.c_str()));

			if (InfoEngine::PRICE_RANGE == info.type)
			{
				info.ToString(text, 1);
				mDataListCtrl.SetItemText(iInfo, 2, CString(text.c_str()));
			}
		}
	}
}

void CBidWorkDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
	if (mRectChanged)
	{
		DrawImage(IDC_BIDPI_LOWEST_PRICE, mLowestPriceImg);
		DrawImage(IDC_BIDPI_LOWEST_PRICE_TIME, mLowestPriceTimeImg);
		DrawImage(IDC_BIDPI_PRICE_RANGE, mPriceRangeImg);
		DrawImage(IDC_BIDPI_CURRENT_TIME, mCurrentTimeImg);

		DrawImage(IDC_BIDPI_PRICE_INPUT, mPriceInputImg);
		DrawImage(IDC_BIDPI_PRICE_CONFIRM, mPriceConfirmImg);

		DrawImage(IDC_BIDPI_VERIFY_CODE_INPUT, mVerifyCodeInputImg);
		DrawImage(IDC_BIDPI_VERIFYCODE_PICTURE, mVerifyCodePictureImg);
		DrawImage(IDC_BIDPI_VERIFYCODE_CONFIRM, mVerifyCodeConfirmImg);

		DrawImage(IDC_BIDPI_BID_RESULT_BUTTON, mBidResultImg);
		mRectChanged = false;
	}
	else
	{
		DrawImage(IDC_BIDPI_BIG_VERIFY_CODE_PICTURE, mVerifyCodePictureImg);
	}
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
			mFontNameCombox.AddString(CString(itRecognize->first.c_str()));
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
	wchar_t szColumnName[3][10] = { L"Name", L"Value1", L"Value2" };
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
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_TIME_RECT_INDEX, L"系统目前时间");
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_LOWEST_PRICE_INDEX, L"目前最低可成交价");
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_LOWEST_PRICE_TIME_INDEX, L"最低可成交价出价时间");
	mDataListCtrl.InsertItem(InfoEngine::CURRENT_ACCEPTABLE_PRICE_RANGE, L"目前数据库接收的价格区间");

	ActionEngine::GetInstance()->Load(mActionConfPath);
	StrategyManager::GetInstance()->Load(mStrategyConfPath);
	InfoEngine::GetInstance()->Registe(this);

	for (auto pStrategy : StrategyManager::GetInstance()->GetStrategies())
	{
		InfoEngine::GetInstance()->Registe(pStrategy);
		mStrategyCombox.AddString(CString(pStrategy->GetName().c_str()));
	}
	mStrategyCombox.SetCurSel(0);

	InfoEngine::GetInstance()->Load(mInfoConfPath);

	Recaptal::GetInstance()->SetListener(this);

	PixelAnyalizeAll("C:\\Users\\shgli\\Desktop\\verifycode");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBidWorkDlg::OnFontNameChanged()
{
	auto index = mFontNameCombox.GetCurSel();
	CString name;
	mFontNameCombox.GetLBText(index, name);

	std::string strName = CW2A(name);
	auto pRecognizer = COCREngine::GetInstance()->GetRecognizer(strName);
	InfoEngine::GetInstance()->SetRecognizer(pRecognizer);
}

void CBidWorkDlg::OnNewRecognizer(const std::string& name, CRecognizer* pRecognizer)
{
	mFontNameCombox.AddString(CString(name.c_str()));
}

void CBidWorkDlg::OnDelRecognizer(const std::string& name, CRecognizer* pRecognizer)
{}


void CBidWorkDlg::OnDoubleClickBidResultButtonArea()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetRect(ActionEngine::BID_RESULT_CONFIRM_BUTTON_AREA, rect);
	mBidResultImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
}


void CBidWorkDlg::OnDoubleClickPostionReferenceArea()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetReferencePoint(rect.left, rect.top);
	InfoEngine::GetInstance()->SetReferencePoint(rect.left, rect.top);
	mPositionRefImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
	InfoEngine::GetInstance()->Save(mInfoConfPath);
}


void CBidWorkDlg::OnDoubleClickVerifyCodeInputArea()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetRect(ActionEngine::SECURITY_CODE_INPUT_AREA, rect);
	mVerifyCodeInputImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
}


void CBidWorkDlg::OnDoubleClickVerifyCodeConfirmArea()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetRect(ActionEngine::SECURITY_CODE_CONFIRM_BUTTON_AREA, rect);
	mVerifyCodeConfirmImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
}


void CBidWorkDlg::OnDoubleClickVerifyCodePictureArea()
{
	CRect rect;
	CScreenSelectionDialog dlg(rect);
	dlg.DoModal();

	mRectChanged = true;
	ActionEngine::GetInstance()->SetRect(ActionEngine::SECURITY_CODE_PICTURE_AREA, rect);
	mVerifyCodePictureImg.CaptureRect(rect);
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	ActionEngine::GetInstance()->Save(mActionConfPath);
}


void CBidWorkDlg::OnBnClickedStartCollectData()
{
	if (isCollecting)
	{
		SetDlgItemText(IDC_START_COLLECT_DATA, L"开始收集数据");
		mInfoThread->detach();
		isCollecting = false;
	}
	else
	{
		SetDlgItemText(IDC_START_COLLECT_DATA, L"停止收集数据");
		isCollecting = true;
		mInfoThread = std::make_shared<std::thread>([this]()
		{
			auto pInfoEngine = InfoEngine::GetInstance();

			while (isCollecting)
			{
				pInfoEngine->Step();
			}
		});
	}
}

void CBidWorkDlg::OnRecaptal()
{
	auto& result = Recaptal::GetInstance()->GetResult(true);
	ActionEngine::GetInstance()->InputSecurityCode(CString(result.c_str()));
}

void CBidWorkDlg::OnReFailed()
{
	ActionEngine::GetInstance()->RefreshSecurityCode();
}

void CBidWorkDlg::OnBnClickedVcodeConfirmBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CBidWorkDlg::OnBnClickedVcodeRejectBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CBidWorkDlg::OnBnClickedReBid()
{
	// TODO: 在此添加控件通知处理程序代码
}


