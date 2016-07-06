// RecognizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include <sstream>
#include "MyBid.h"
#include "RecognizeDlg.h"
#include "afxdialogex.h"
#include "OCREngine.h"
#include "Recognizer.h"
#include "ScreenSelectionDialog.h"
#include "CpuTime.h"
// CRecognizeDlg dialog

IMPLEMENT_DYNAMIC(CRecognizeDlg, CDialogEx)

CRecognizeDlg::CRecognizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecognizeDlg::IDD, pParent)
	, mCurrentRecognizer(nullptr)
{

}

CRecognizeDlg::~CRecognizeDlg()
{
}

void CRecognizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RE_FONT_NAME, mFontNameCtrl);
}


BEGIN_MESSAGE_MAP(CRecognizeDlg, CDialogEx)
	ON_STN_DBLCLK(IDC_RE_IMAGE, &CRecognizeDlg::OnStnDblclickReImage)
	ON_CBN_SELCHANGE(IDC_RE_FONT_NAME, &CRecognizeDlg::OnCbnSelchangeReFontName)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_REBTB_RECOGNIZE, &CRecognizeDlg::OnBnClickedRebtbRecognize)
END_MESSAGE_MAP()


// CRecognizeDlg message handlers


void CRecognizeDlg::OnStnDblclickReImage()
{
	if(nullptr == mCurrentRecognizer)
	{
		MessageBox(L" please select a font name");
	}
	else
	{
		CRect rect;
		CScreenSelectionDialog dlg(rect);
		dlg.DoModal();

		mScreenImage.CaptureRect(rect);

		//CString value;
		//CString outFeature;
		//mCurrentRecognizer->RecognizeEx(value, outFeature, &mScreenImage);

		//GetDlgItem(IDC_STATIC_RECOGNIZE_RESULT)->SetWindowTextA(value);
	    OnBnClickedRebtbRecognize();
		AfxGetMainWnd()->ShowWindow(SW_SHOW);
	}

}

void CRecognizeDlg::ReBuildNameCombox()
{
	mFontNameCtrl.Clear();
	const auto& allRecognizer = COCREngine::GetInstance()->GetAllRecognizer();
	if(0 != allRecognizer.size())
	{
		for(auto itRecognize = allRecognizer.begin(); itRecognize != allRecognizer.end(); ++itRecognize)
		{
			mFontNameCtrl.AddString(CString(itRecognize->first.c_str()));
		}
		COCREngine::GetInstance()->Registe(this);

		mFontNameCtrl.SetCurSel(0);
		OnCbnSelchangeReFontName();
	}
}

BOOL CRecognizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ReBuildNameCombox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRecognizeDlg::OnNewRecognizer(const std::string& name, CRecognizer* pRecognizer)
{
	mFontNameCtrl.AddString(CString(name.c_str()));
}

void CRecognizeDlg::OnDelRecognizer(const std::string& name, CRecognizer* pRecognizer)
{}

void CRecognizeDlg::OnCbnSelchangeReFontName()
{
	auto index = mFontNameCtrl.GetCurSel();
	CString name;
	mFontNameCtrl.GetLBText(index, name);

	std::string szName = CW2A(name);
	mCurrentRecognizer = COCREngine::GetInstance()->GetRecognizer(szName);
}





void CRecognizeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
   if(!mScreenImage.IsNull())
   {
	   CWnd *pWnd = GetDlgItem(IDC_RE_IMAGE);
	   CDC *pDC = pWnd->GetDC();

	   CRect rect;
	   pWnd->GetClientRect(&rect);
	   pDC->SetStretchBltMode(STRETCH_HALFTONE);
	   mScreenImage.Draw(pDC->m_hDC, rect);
   }
}

void CRecognizeDlg::OnBnClickedRebtbRecognize()
{
	if(nullptr != mCurrentRecognizer)
	{
		std::string outFeature;
		std::string value;

		CpuTime time;
		bool success = mCurrentRecognizer->RecognizeEx(value, outFeature, &mScreenImage);
		double elapse = time.GetElapse();

		std::stringstream timeStream;
		timeStream << elapse << " seconds";
		GetDlgItem(IDC_RE_TIME)->SetWindowText(CString(timeStream.str().c_str()));

		if(success)
		{
			GetDlgItem(IDC_STATIC_RECOGNIZE_RESULT)->SetWindowText(CString(value.c_str()));
            GetDlgItem(IDC_RE_STATUS)->SetWindowText(L"OK");
		}
		else
		{
			GetDlgItem(IDC_STATIC_RECOGNIZE_RESULT)->SetWindowText(L"");
			GetDlgItem(IDC_RE_STATUS)->SetWindowText(L"Failed");
		}
		GetDlgItem(IDC_RE_FEATURE)->SetWindowText(CString(outFeature.c_str()));
	}
}
