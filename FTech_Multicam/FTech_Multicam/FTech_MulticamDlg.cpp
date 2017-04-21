
// FTech_MulticamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FTech_Multicam.h"
#include "FTech_MulticamDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTech_MulticamDlg dialog



UINT DisplayThread(LPVOID param);
CFTech_MulticamDlg::CFTech_MulticamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFTech_MulticamDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bThWork = false;
	m_pDspThread = NULL;
	m_pBmpInfo = NULL;
	m_nWidth  = 0;
	m_nHeight = 0;
}

void CFTech_MulticamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFTech_MulticamDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CFTech_MulticamDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CFTech_MulticamDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_ACQ, &CFTech_MulticamDlg::OnBnClickedBtnAcq)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CFTech_MulticamDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CFTech_MulticamDlg message handlers

BOOL CFTech_MulticamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFTech_MulticamDlg::OnPaint()
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
HCURSOR CFTech_MulticamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFTech_MulticamDlg::OnBnClickedBtnRefresh()
{
	CListBox* pLB = (CListBox*)GetDlgItem(IDC_LTB_LIST);
	pLB->ResetContent();

	int cnt=0;
	CMulticamEx::GetNumberOfBoard(cnt);

	for (int i=0; i<cnt; i++)
	{
		CString list=_T(""), name=_T(""), type=_T("");
		CMulticamEx::GetBoardName(i, name);
		CMulticamEx::GetBoardType(i, type);

		list.Format(_T("%s (%s)"), type, name);
		pLB->AddString(list);
	}
}

void CFTech_MulticamDlg::OnBnClickedBtnOpen()
{
	CListBox* pLB = (CListBox*)GetDlgItem(IDC_LTB_LIST);
	int index = pLB->GetCurSel();

	if (index == -1) { AfxMessageBox(_T("Please select board on list.")); return; }

	CString strFilter = _T("Camera Files (*.cam)|*.cam||");

	CFileDialog FileDlg(TRUE, _T(".cam"), NULL, 0, strFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		bool ret=false;
		
		ret = m_Grabber.OnInitGrablinkBoard(index, Grablink::EM, Grablink::EMONO, FileDlg.GetPathName());
		if (ret == true)
		{
			GetDlgItem(IDC_BTN_ACQ)->EnableWindow(TRUE);
			int bpp=0;
			m_Grabber.GetWidth(m_nWidth);
			m_Grabber.GetHeight(m_nHeight);
			m_Grabber.GetBpp(bpp);

			SetDlgItemInt(IDC_LB_WIDTH, m_nWidth);
			SetDlgItemInt(IDC_LB_HEIGHT, m_nHeight);
			SetDlgItemInt(IDC_LB_BPP, bpp);

			OnCreateBmpInfo(m_nWidth,m_nHeight,bpp);
		}
	}
}

void CFTech_MulticamDlg::OnCreateBmpInfo(int nWidth, int nHeight, int nBpp)
{
	if (m_pBmpInfo != NULL)
	{
		delete []m_pBmpInfo;
		m_pBmpInfo = NULL;
	}

	if (nBpp == 8)
		m_pBmpInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
	else if (nBpp == 24)
		m_pBmpInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];
	
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = nBpp;
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (nBpp == 8)
	{
		for (int i = 0 ; i < 256 ; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = (BYTE)i;
			m_pBmpInfo->bmiColors[i].rgbGreen = (BYTE)i;
			m_pBmpInfo->bmiColors[i].rgbRed = (BYTE)i;
			m_pBmpInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	
	m_pBmpInfo->bmiHeader.biWidth = nWidth;
	m_pBmpInfo->bmiHeader.biHeight = -nHeight;
}

void CFTech_MulticamDlg::OnBnClickedBtnAcq()
{
	CString caption;
	GetDlgItemText(IDC_BTN_ACQ, caption);

	bool ret=false;
	if (caption == _T("START"))
	{
		ret = m_Grabber.OnStartAcquisition();
		if (ret == true)
		{
			SetDlgItemText(IDC_BTN_ACQ, _T("STOP"));
			m_bThWork = true;
			m_pDspThread = AfxBeginThread(DisplayThread, this);
			SetTimer(100,30,NULL);
		}
	}
	else
	{
		ret = m_Grabber.OnStopAcquisition();
		if (ret == true)
		{
			m_bThWork = false;
			SetDlgItemText(IDC_BTN_ACQ, _T("START"));
			KillTimer(100);
		}
	}

}

void CFTech_MulticamDlg::OnDisplay()
{
	CClientDC dc(GetDlgItem(IDC_VIEW_CAMERA));

	CRect rect;
	GetDlgItem(IDC_VIEW_CAMERA)->GetClientRect(&rect);

	BYTE* pBuffer = m_Grabber.GetImageBuffer();

	SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR); 
	StretchDIBits(dc.GetSafeHdc(), 0, 0, rect.Width(), rect.Height(), 0, 0, m_nWidth, m_nHeight, pBuffer, m_pBmpInfo, DIB_RGB_COLORS, SRCCOPY);
}

UINT DisplayThread(LPVOID param)
{
	CFTech_MulticamDlg* pMain = (CFTech_MulticamDlg*)param;

	DWORD dwResult=0;
	while(pMain->m_bThWork)
	{
		Sleep(30);

		dwResult = WaitForSingleObject(pMain->m_Grabber.GetHandleGrabDone(), 100);
		if (dwResult == WAIT_OBJECT_0)
		{
			pMain->OnDisplay();
			pMain->m_Grabber.ResetHandleGrabDone();
		}
	}

	return 0;
}

void CFTech_MulticamDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(100);

	if (m_pBmpInfo != NULL)
	{
		delete []m_pBmpInfo;
		m_pBmpInfo = NULL;
	}

	m_bThWork = false;

	DWORD dwResult=0;
	if (m_pDspThread != NULL)
		dwResult = WaitForSingleObject(m_pDspThread->m_hThread, INFINITE);
	
}

void CFTech_MulticamDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100)
	{
		double fps=0;
		m_Grabber.GetFrameRate(fps);

		CString tmp=_T("");
		tmp.Format(L"%.3f fps",fps);
		SetDlgItemText(IDC_LB_FPS, tmp);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CFTech_MulticamDlg::OnBnClickedBtnSave()
{
	CString	strFilter = _T("BMP Files (*.bmp)|*.bmp||");

	CFileDialog FileDlg(FALSE, _T(".bmp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);

	if( FileDlg.DoModal() == IDOK )
	{
		m_Grabber.OnSaveImage(FileDlg.GetPathName());
	}
}
