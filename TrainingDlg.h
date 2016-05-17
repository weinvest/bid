#pragma once

#include "ScreenImage.h"
// CTrainingDlg dialog

class CFontTraining;
class CTrainingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTrainingDlg)

public:
	CTrainingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrainingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TRAINING };
	void UpdateKnowledge();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString mName;
	CString mRepValue;
	CFontTraining* mCurrentTraining;

	afx_msg void OnBnClickedTrbuttonNext();
	afx_msg void OnBnClickedTrbuttonDone();
	afx_msg void OnBnClickedTrbuttonCannel();
	int mSepCount;
	CScreenImage mScreenImage;
	afx_msg void OnStnDblclickTrainingImage();
	afx_msg void OnPaint();
	CStatic mImageCtrl;
	CString mStrKnowledge;
};
