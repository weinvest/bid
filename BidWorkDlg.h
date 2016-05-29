#pragma once

#include "ScreenImage.h"
#include "IOCRListener.h"
// CBidWorkDlg dialog

class CBidWorkDlg : public CDialogEx, public IOCRListener
{
	DECLARE_DYNAMIC(CBidWorkDlg)

public:
	CBidWorkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBidWorkDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BID };

	void OnNewRecognizer(const CString& name, CRecognizer* pRecognizer) override;
	void OnDelRecognizer(const CString& name, CRecognizer* pRecognizer) override;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox mFontNameCombox;
	afx_msg void OnDoubleClickCaptureCurrentTime();
	afx_msg void OnDoubleClickCaptureLowestPrice();
	afx_msg void OnDoubleClickCaptureLowestPriceTime();
	afx_msg void OnDoubleClickCapturePriceConfirm();
	afx_msg void OnDoubleClickCapturePriceInput();
	afx_msg void OnDoubleClickCapturePriceRange();
	afx_msg void OnDoubleClickCaptureVerifybox();

private:
	void DrawImage(int id, CScreenImage& img);
	void ReBuildNameCombox();

	CScreenImage mCurrentTimeImg;
	CScreenImage mLowestPriceImg;
	CScreenImage mPriceConfirmImg;
	CScreenImage mPriceRangeImg;
	CScreenImage mLowestPriceTimeImg;
	CScreenImage mPriceInputImg;
	CScreenImage mVerifyBoxImg;

	CScreenImage mScreenTest;
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnFontNameChanged();
};
