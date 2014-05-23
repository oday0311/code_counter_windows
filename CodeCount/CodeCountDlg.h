// CodeCountDlg.h : ͷ�ļ�
//

#pragma once
#include"FolderDialog.h"
#include<vector>
#include "afxcmn.h"
#include <afxmt.h>

using namespace std;

// CCodeCountDlg �Ի���
class CCodeCountDlg : public CDialog
{
// ����
public:
	CCodeCountDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CODECOUNT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString				m_strFilename;			//������ļ���+Ŀ¼
	vector<CString>		vcFile;					//����ļ��ַ�����
	vector<CString>		vcFilePath;				
	//int					m_nTotalLines;			//������
	int					m_nCodeLines;			//��������
	int					m_nBlankLines;			//�հ�����
	CString				m_strFilepath;			//�ļ�Ŀ¼
	CListCtrl			m_lcResult;
	int					m_nStatMethod;			//1.1���¼�
	CString				m_strLogFile;			//1.1���¼�

	//HANDLE				hThread[10];
	//HANDLE				hMutex;					//�ں˶��󣬱���m_nFiles
public:
	int					nTotol;					//ÿ������
	int					nLength;				
	int					nCodeLines;
	int					nCommentLines;
	int					nBlankLines;
	int					m_nFiles;
//ͳ�Ƶ��ļ���	
	CString				strTotol;
	CString				strCodeLines;
	CString				strCommentLines;
	CString				strBlankLines;
	CMutex				m_lock;					//�̻߳������
public:
	static int			gAllCodeLines;			//�����ļ��ܴ�����
	static int			gAllBlankLines;			//
	static int			gAllCommentLines;		//
	static int			gAllTotol;				//�����ļ�����

public:

	void SearchFile(CString strPath, CString ExtensionName1, CString ExtensionName2, CString ExtensionName3);
	afx_msg void OnBnClickedBtnCount();
	afx_msg void OnBnClickedBtnOpendir();
	static DWORD WINAPI MyThreadProc(LPVOID pParam);	//�����̴߳�����
	int GetFileLines(LPCTSTR strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines);
	void EmptyCodeCount();
	afx_msg void OnBnClickedBtnCountall();
	afx_msg void OnBnClickedBtnCls();
};
