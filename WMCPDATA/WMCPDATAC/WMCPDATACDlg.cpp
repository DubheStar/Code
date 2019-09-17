
// WMCPDATACDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WMCPDATAC.h"
#include "WMCPDATACDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWMCPDATACDlg 对话框



CWMCPDATACDlg::CWMCPDATACDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WMCPDATAC_DIALOG, pParent)
	, m_strSendText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWMCPDATACDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strSendText);
}

BEGIN_MESSAGE_MAP(CWMCPDATACDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWMCPDATACDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWMCPDATACDlg 消息处理程序

BOOL CWMCPDATACDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWMCPDATACDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWMCPDATACDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWMCPDATACDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	HWND hWnd = ::FindWindow(NULL, _T("WMCPDATAC"));

	COPYDATASTRUCT cds;
	cds.dwData = 0;
	cds.cbData = m_strSendText.GetLength() + 1;
	cds.lpData = m_strSendText.GetBuffer(cds.cbData);

	::SendMessage(hWnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)& cds);
}



bool CWMCPDATACDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT *pCopyDataStruct)
{
	// TODO: 在此处添加实现代码.
	m_strRecv.Format(_T("服务端在[%d]接收到消息"), pCopyDataStruct->lpData);

	m_strRecv.AddString(m_strRecv);
	return false;
}
