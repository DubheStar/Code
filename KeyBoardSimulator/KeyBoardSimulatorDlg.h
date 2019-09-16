
// KeyBoardSimulatorDlg.h: 头文件
//

#pragma once


// CKeyBoardSimulatorDlg 对话框
class CKeyBoardSimulatorDlg : public CDialogEx
{
// 构造
public:
	CKeyBoardSimulatorDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEYBOARDSIMULATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString WebCaption;// CString WebCaption;
	int Rate;
	afx_msg void OnBnClickedButtonstart();
	CButton BtnStart;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
