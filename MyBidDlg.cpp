
// MyBidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyBid.h"
#include "MyBidDlg.h"
#include "afxdialogex.h"
#include "OCREngine.h"
#include "Log.h"
#include "recaptal\Recaptal.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyBidDlg dialog




CMyBidDlg::CMyBidDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyBidDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyBidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_pTabCtrl);
}

BEGIN_MESSAGE_MAP(CMyBidDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CMyBidDlg::OnTcnSelchangeMainTab)
END_MESSAGE_MAP()


// CMyBidDlg message handlers

BOOL CMyBidDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	mHAccel = LoadAccelerators(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDR_ACCELERATOR1));

	COCREngine::GetInstance()->Load("font");

	m_pTrainingDlg.Create(IDD_DIALOG_TRAINING,GetDlgItem(IDC_MAIN_TAB)); 
    m_pRecognizeDlg.Create(IDD_DIALOG_RECOGNIZE,GetDlgItem(IDC_MAIN_TAB)); 
    m_pBidWorkDlg.Create(IDD_DIALOG_BID,GetDlgItem(IDC_MAIN_TAB)); 

	m_pTabCtrl.InsertItem(0, L"Training");
	m_pTabCtrl.InsertItem(1, L"Recognize");
	m_pTabCtrl.InsertItem(2, L"Bid");

    CRect rs; 
    m_pTabCtrl.GetClientRect(&rs); 
    //调整子对话框在父窗口中的位置 
    rs.top += 21; 
    rs.bottom -=1; 
    rs.left +=1; 
    rs.right -=2; 
     
    //设置子对话框尺寸并移动到指定位置 
    m_pTrainingDlg.MoveWindow(&rs); 
    m_pRecognizeDlg.MoveWindow(&rs); 
    m_pBidWorkDlg.MoveWindow(&rs); 
     
    //分别设置隐藏和显示 
    m_pTrainingDlg.ShowWindow(true); 
    m_pRecognizeDlg.ShowWindow(false); 
    m_pBidWorkDlg.ShowWindow(false); 

	m_pTabCtrl.SetCurSel(0);
	// TODO: Add extra initialization here
	Recaptal::GetInstance()->Start("51moni-sh.oss-cn-shanghai.aliyuncs.com", "80");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyBidDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyBidDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyBidDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyBidDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CMyBidDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch(m_pTabCtrl.GetCurSel())
	{
	case 0:
		{
			m_pTrainingDlg.ShowWindow(true);
			m_pRecognizeDlg.ShowWindow(false);
	        m_pBidWorkDlg.ShowWindow(false);
		}
		break;
	case 1:
		{
			m_pTrainingDlg.ShowWindow(false);
			m_pRecognizeDlg.ShowWindow(true);
	        m_pBidWorkDlg.ShowWindow(false);
		}
		break;
	case 2:
		{
			m_pTrainingDlg.ShowWindow(false);
			m_pRecognizeDlg.ShowWindow(false);
	        m_pBidWorkDlg.ShowWindow(true);
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}


BOOL CMyBidDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_pBidWorkDlg.IsWindowVisible() 
		&& mHAccel && ::TranslateAccelerator(m_hWnd, mHAccel, pMsg))
	{
		switch (pMsg->wParam)
		{
		case 'Y':
			m_pBidWorkDlg.OnBnClickedVcodeConfirmBtn();
			break;
		case 'N':
			m_pBidWorkDlg.OnBnClickedVcodeRejectBtn();
			break;
		case 'R':
			m_pBidWorkDlg.OnBnClickedReBid();
			break;
		}

		return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
