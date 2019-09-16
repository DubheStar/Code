
// KeyBoardSimulatorDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "KeyBoardSimulator.h"
#include "KeyBoardSimulatorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:

// 实现
protected:
	//DECLARE_MESSAGE_MAP()
public:

};




CKeyBoardSimulatorDlg::CKeyBoardSimulatorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEYBOARDSIMULATOR_DIALOG, pParent)
	, WebCaption(_T(""))
	, Rate(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyBoardSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITWEBCAPTION, WebCaption);
	DDV_MaxChars(pDX, WebCaption, 1000);
	DDX_Text(pDX, IDC_EDITRATE, Rate);
	DDV_MinMaxInt(pDX, Rate, 1, 100);
	DDX_Control(pDX, IDC_BUTTONSTART, BtnStart);
	//  DDX_Control(pDX, IDC_BTNSTOP, BtnStop);
}

BEGIN_MESSAGE_MAP(CKeyBoardSimulatorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONSTART, &CKeyBoardSimulatorDlg::OnBnClickedButtonstart)
//	ON_WM_TIMER()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CKeyBoardSimulatorDlg 消息处理程序

BOOL CKeyBoardSimulatorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKeyBoardSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKeyBoardSimulatorDlg::OnPaint()
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
HCURSOR CKeyBoardSimulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKeyBoardSimulatorDlg::OnBnClickedButtonstart()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (WebCaption == "" || Rate == 0)
	{
		MessageBox(_T("警告: 输入的参数不合法"), _T("警告"), MB_OK);
		return;
	}
	CString strBtnStart;

	BtnStart.GetWindowText(strBtnStart);

	//判断按钮状态
	if (strBtnStart == _T("Start"))
	{
		//设置定时器
		SetTimer(1, Rate * 1000, NULL);
		BtnStart.SetWindowText(_T("Stop"));
		GetDlgItem(IDC_EDITWEBCAPTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITRATE)->EnableWindow(FALSE);
	}
	else
	{
		KillTimer(1);
		BtnStart.SetWindowTextW(_T("Start"));
		GetDlgItem(IDC_EDITWEBCAPTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDITRATE)->EnableWindow(TRUE);
	}
}


void CKeyBoardSimulatorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HWND hWnd = ::FindWindow(NULL, WebCaption.GetBuffer(0));
	::PostMessage(hWnd, WM_KEYDOWN, VK_F5, 1);
	Sleep(50);
	::PostMessage(hWnd, WM_KEYUP, VK_F5, 1);
	CDialogEx::OnTimer(nIDEvent);
}
