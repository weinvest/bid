#pragma once

#include "ScreenImage.h"
#include "IOCRListener.h"
#include "InfoEngine.h"
#include "recaptal\Recaptal.h"
#include "afxcmn.h"
#include "afxwin.h"
// CBidWorkDlg dialog
class IStrategy;
class CBidWorkDlg : public CDialogEx, public IOCRListener, public IInfoListener, public IRecaptalListener
{
	DECLARE_DYNAMIC(CBidWorkDlg)

public:
	CBidWorkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBidWorkDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BID };

	void OnNewRecognizer(const std::string& name, CRecognizer* pRecognizer) override;
	void OnDelRecognizer(const std::string& name, CRecognizer* pRecognizer) override;
	void OnUpdate(size_t updateFields) override;
	void OnRecaptal() override;
	void OnReFailed() override;
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

	bool mRectChanged;
	CScreenImage mCurrentTimeImg;
	CScreenImage mLowestPriceImg;
	CScreenImage mPriceRangeImg;
	CScreenImage mLowestPriceTimeImg;

	CScreenImage mPriceInputImg;
	CScreenImage mPriceConfirmImg;

	CScreenImage mVerifyCodeInputImg;
	CScreenImage mVerifyCodePictureImg;
	CScreenImage mVerifyCodeConfirmImg;

	CScreenImage mBidResultImg;

	CScreenImage mPositionRefImg;
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnFontNameChanged();
	CListCtrl mDataListCtrl;

	std::shared_ptr<std::thread> mInfoThread;
	CString mInfoConfPath;
	CString mActionConfPath;
	CString mStrategyConfPath;
	afx_msg void OnDoubleClickBidResultButtonArea();
	afx_msg void OnDoubleClickPostionReferenceArea();
	afx_msg void OnDoubleClickVerifyCodeInputArea();
	afx_msg void OnDoubleClickVerifyCodeConfirmArea();
	afx_msg void OnDoubleClickVerifyCodePictureArea();
	CComboBox mStrategyCombox;
	afx_msg void OnBnClickedStartCollectData();
	afx_msg void OnBnClickedVcodeConfirmBtn();
	afx_msg void OnBnClickedVcodeRejectBtn();
	afx_msg void OnBnClickedReBid();

	std::string mLastSecurityCode;
	afx_msg void OnStrategyChanged();

	IStrategy* mCurrentStrategy;
	std::atomic<IStrategy*> mNewSelectedStrategy;
};
