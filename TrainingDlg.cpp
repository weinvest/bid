// TrainingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyBid.h"
#include "TrainingDlg.h"
#include "afxdialogex.h"
#include "FontTraining.h"
#include "OCREngine.h"
#include "ScreenSelectionDialog.h"
#include <sstream>
// CTrainingDlg dialog

IMPLEMENT_DYNAMIC(CTrainingDlg, CDialogEx)

CTrainingDlg::CTrainingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrainingDlg::IDD, pParent)
	, mCurrentTraining(nullptr)
{

	mName = _T("");
	mRepValue = _T("");

	mCurrentTraining = new CFontTraining();

	std::set<char> chars;
	chars.insert('0'); chars.insert('3'); chars.insert('6');chars.insert('9');
	chars.insert('1'); chars.insert('4'); chars.insert('7');chars.insert(':');
	chars.insert('2'); chars.insert('5'); chars.insert('8');chars.insert('-');
	chars.insert('|');
	mCurrentTraining->Initialize(chars);
	mSepCount = 1;
	mStrKnowledge = _T("");
}

CTrainingDlg::~CTrainingDlg()
{
	delete mCurrentTraining;
}

void CTrainingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRAINING_NAME, mName);
	DDV_MaxChars(pDX, mName, 16);
	DDX_Text(pDX, IDC_TRAINING_VALUE, mRepValue);
	DDV_MaxChars(pDX, mRepValue, 64);
	DDX_Text(pDX, IDC_TR_SEPCOUNT, mSepCount);
	DDX_Control(pDX, IDC_TRAINING_IMAGE, mImageCtrl);
	DDX_Text(pDX, IDC_TR_KNOWLEDGE, mStrKnowledge);
	DDV_MaxChars(pDX, mStrKnowledge, 100000);
}


BEGIN_MESSAGE_MAP(CTrainingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TRBUTTON_NEXT, &CTrainingDlg::OnBnClickedTrbuttonNext)
	ON_BN_CLICKED(IDC_TRBUTTON_DONE, &CTrainingDlg::OnBnClickedTrbuttonDone)
	ON_BN_CLICKED(IDC_TRBUTTON_CANNEL, &CTrainingDlg::OnBnClickedTrbuttonCannel)
	ON_STN_DBLCLK(IDC_TRAINING_IMAGE, &CTrainingDlg::OnStnDblclickTrainingImage)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTrainingDlg message handlers



void CTrainingDlg::OnBnClickedTrbuttonNext()
{
	UpdateData();
	mCurrentTraining->SetSepCount(mSepCount);

	std::string outFeature, repValue;
	if(!mCurrentTraining->Traning(outFeature, &mScreenImage, repValue))
	{
		GetDlgItem(IDC_TR_STATUS)->SetWindowText(L"Traing Failed");
	}
	else
	{
		UpdateKnowledge();
	}
	mRepValue.Format(L"%s", repValue.c_str());
	GetDlgItem(IDC_TR_FEATURES)->SetWindowText(CString(outFeature.c_str()));
}


void CTrainingDlg::UpdateKnowledge()
{
	std::stringstream knownledge;
	mCurrentTraining->Save(knownledge);
	
	CWnd* pKnowledgeCtl = GetDlgItem(IDC_TR_KNOWLEDGE);
	pKnowledgeCtl->SetWindowText( CString(knownledge.str().c_str()));
}

void CTrainingDlg::OnBnClickedTrbuttonDone()
{
	UpdateData();
	std::string name = CW2A(mName);
	if(COCREngine::GetInstance()->Has(name))
	{
		GetDlgItem(IDC_TR_STATUS)->SetWindowText(mName + " may be overwrite another font, please using another name");
	}
	else
	{
		std::string outFeature, repValue;
		mCurrentTraining->SetSepCount(mSepCount);
		if(mCurrentTraining->Traning(outFeature, &mScreenImage, repValue))
		{
		    COCREngine::GetInstance()->Add(name, mCurrentTraining, false);
		    UpdateKnowledge();
		}
		else
		{
			GetDlgItem(IDC_TR_STATUS)->SetWindowText(L"Traing Failed");
		}
		mRepValue.Format(L"%s", repValue.c_str());
		GetDlgItem(IDC_TR_FEATURES)->SetWindowText(CString(outFeature.c_str()));
	}
}


void CTrainingDlg::OnBnClickedTrbuttonCannel()
{
	mCurrentTraining->Clear();
}


void CTrainingDlg::OnStnDblclickTrainingImage()
{
   CRect rect;
   CScreenSelectionDialog dlg(rect);
   dlg.DoModal();

   mScreenImage.CaptureRect(rect);
   AfxGetMainWnd()->ShowWindow(SW_SHOW);
}

void CTrainingDlg::OnPaint()
{
   CDialogEx::OnPaint();
   if(!mScreenImage.IsNull())
   {
	   CWnd *pWnd = GetDlgItem(IDC_TRAINING_IMAGE);
	   CDC *pDC = pWnd->GetDC();

	   CRect rect;
	   pWnd->GetClientRect(&rect);
	   pDC->SetStretchBltMode(STRETCH_HALFTONE);
	   mScreenImage.Draw(pDC->m_hDC, rect);
   }
   //ReleaseDC(pDC);
}