
// MyBidDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "TrainingDlg.h"
#include "RecognizeDlg.h"
#include "BidWorkDlg.h"

// CMyBidDlg dialog
class CMyBidDlg : public CDialogEx
{
// Construction
public:
	CMyBidDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MYBID_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CTabCtrl m_pTabCtrl;
	CTrainingDlg m_pTrainingDlg;
	CRecognizeDlg m_pRecognizeDlg;
	CBidWorkDlg m_pBidWorkDlg;
public:
	afx_msg void OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult);
};
