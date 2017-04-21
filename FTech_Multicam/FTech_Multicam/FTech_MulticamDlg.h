
// FTech_MulticamDlg.h : header file
//

#pragma once
#include "MulticamEx.h"

// CFTech_MulticamDlg dialog
class CFTech_MulticamDlg : public CDialogEx
{
// Construction
public:
	CFTech_MulticamDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FTECH_MULTICAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public :
	CMulticamEx m_Grabber;
	bool m_bThWork;
	BITMAPINFO * m_pBmpInfo;
	CWinThread* m_pDspThread;
	int m_nWidth, m_nHeight;
	void OnDisplay();
	void OnCreateBmpInfo(int nWidth, int nHeight, int nBpp);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnAcq();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnSave();
};
