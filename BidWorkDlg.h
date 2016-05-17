#pragma once


// CBidWorkDlg dialog

class CBidWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBidWorkDlg)

public:
	CBidWorkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBidWorkDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
