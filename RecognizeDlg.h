#pragma once
#include "ScreenImage.h"
#include "IOCRListener.h"
// CRecognizeDlg dialog

class CRecognizer;
class CRecognizeDlg : public CDialogEx, public IOCRListener
{
	DECLARE_DYNAMIC(CRecognizeDlg)

public:
	CRecognizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRecognizeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RECOGNIZE };

protected:
	void ReBuildNameCombox();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void OnNewRecognizer(const CString& name, CRecognizer* pRecognizer) override;
	void OnDelRecognizer(const CString& name, CRecognizer* pRecognizer) override;

	afx_msg void OnStnDblclickReImage();
	afx_msg void OnCbnSelchangeReFontName();
private:
	CComboBox mFontNameCtrl;
	CRecognizer* mCurrentRecognizer;
	CScreenImage mScreenImage;
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRebtbRecognize();
};
