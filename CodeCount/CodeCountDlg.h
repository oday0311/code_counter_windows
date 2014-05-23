// CodeCountDlg.h : 头文件
//

#pragma once
#include"FolderDialog.h"
#include<vector>
#include "afxcmn.h"
#include <afxmt.h>

using namespace std;

// CCodeCountDlg 对话框
class CCodeCountDlg : public CDialog
{
// 构造
public:
	CCodeCountDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CODECOUNT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString				m_strFilename;			//需查找文件名+目录
	vector<CString>		vcFile;					//存放文件字符串组
	vector<CString>		vcFilePath;				
	//int					m_nTotalLines;			//总行数
	int					m_nCodeLines;			//代码行数
	int					m_nBlankLines;			//空白行数
	CString				m_strFilepath;			//文件目录
	CListCtrl			m_lcResult;
	int					m_nStatMethod;			//1.1版新加
	CString				m_strLogFile;			//1.1版新加

	//HANDLE				hThread[10];
	//HANDLE				hMutex;					//内核对象，保护m_nFiles
public:
	int					nTotol;					//每行总数
	int					nLength;				
	int					nCodeLines;
	int					nCommentLines;
	int					nBlankLines;
	int					m_nFiles;
//统计的文件数	
	CString				strTotol;
	CString				strCodeLines;
	CString				strCommentLines;
	CString				strBlankLines;
	CMutex				m_lock;					//线程互斥对象
public:
	static int			gAllCodeLines;			//所有文件总代码行
	static int			gAllBlankLines;			//
	static int			gAllCommentLines;		//
	static int			gAllTotol;				//所有文件总行

public:

	void SearchFile(CString strPath, CString ExtensionName1, CString ExtensionName2, CString ExtensionName3);
	afx_msg void OnBnClickedBtnCount();
	afx_msg void OnBnClickedBtnOpendir();
	static DWORD WINAPI MyThreadProc(LPVOID pParam);	//工作线程处理函数
	int GetFileLines(LPCTSTR strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
	void EmptyCodeCount();
	afx_msg void OnBnClickedBtnCountall();
	afx_msg void OnBnClickedBtnCls();
};
