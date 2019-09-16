
// UserMsgClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "UserMsgClient.h"
#include "UserMsgClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUserMsgClientDlg 对话框



CUserMsgClientDlg::CUserMsgClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USERMSGCLIENT_DIALOG, pParent)
	, nNum1(0)
	, nNum2(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUserMsgClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, nNum1);
	DDV_MinMaxInt(pDX, nNum1, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, nNum2);
	DDV_MinMaxInt(pDX, nNum2, 0, 100);
}

BEGIN_MESSAGE_MAP(CUserMsgClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUserMsgClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserMsgClientDlg 消息处理程序

BOOL CUserMsgClientDlg::OnInitDialog()
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

void CUserMsgClientDlg::OnPaint()
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
HCURSOR CUserMsgClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUserMsgClientDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	MessageBoxW(_T("发送成功"),_T("通知"),MB_OK);
	HWND hWnd = ::FindWindow(NULL, _T("测试"));

	::SendMessage(hWnd, WM_UMSG, (WPARAM)nNum1, (LPARAM)nNum2);
}



