﻿
// WMCPDATASDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WMCPDATAS.h"
#include "WMCPDATASDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWMCPDATASDlg 对话框



CWMCPDATASDlg::CWMCPDATASDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WMCPDATAS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWMCPDATASDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_strServerRecv);
}

BEGIN_MESSAGE_MAP(CWMCPDATASDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_DELALL, &CWMCPDATASDlg::OnBnClickedDelall)
END_MESSAGE_MAP()


// CWMCPDATASDlg 消息处理程序

BOOL CWMCPDATASDlg::OnInitDialog()
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

void CWMCPDATASDlg::OnPaint()
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
HCURSOR CWMCPDATASDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CWMCPDATASDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strRecvText;

	DWORD dwPid = 0;
	::GetWindowThreadProcessId(pWnd->m_hWnd, &dwPid);

	strRecvText.Format(_T("PID = [%d]的进程发来的消息：%s"), dwPid, pCopyDataStruct->lpData);
	m_strServerRecv.AddString(strRecvText);

	//获取本地时间
	SYSTEMTIME st;
	GetLocalTime(&st);
	
	CString strTime;
	strTime.Format(_T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
	
	COPYDATASTRUCT cds;
	cds.dwData = 0;
	cds.cbData = strTime.GetLength() + 1;
	cds.lpData = strTime.GetBuffer(cds.cbData);

	::SendMessage(pWnd->m_hWnd, WM_COPYDATA, NULL, (LPARAM)& cds);

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}




void CWMCPDATASDlg::OnBnClickedDelall()
{
	// TODO: 在此添加控件通知处理程序代码
	while (m_strServerRecv.GetCount())
	{
		m_strServerRecv.DeleteString(0);
	}
}
