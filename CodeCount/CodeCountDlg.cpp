// CodeCountDlg.cpp : 实现文件
// Download by http://www.codefans.net

#include "stdafx.h"
#include "CodeCount.h"
#include "CodeCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int CCodeCountDlg::gAllTotol = 0;
int CCodeCountDlg::gAllCommentLines = 0;
int CCodeCountDlg::gAllBlankLines = 0;
int CCodeCountDlg::gAllCodeLines = 0;
// CCodeCountDlg 对话框

CCodeCountDlg::CCodeCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeCountDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strFilepath = _T("");
	m_nCodeLines = 0;
	m_nBlankLines = 0;
	m_nFiles = 0;
}

void CCodeCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX,IDC_EDIT_ALLCCOUNT,m_nTotalLines);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lcResult);
}

BEGIN_MESSAGE_MAP(CCodeCountDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_BTN_OPENFILEPATH, &CCodeCountDlg::OnBnClickedBtnOpenfilepath)
	ON_BN_CLICKED(IDC_BTN_COUNT, &CCodeCountDlg::OnBnClickedBtnCount)
	ON_BN_CLICKED(IDC_BTN_OPENDIR, &CCodeCountDlg::OnBnClickedBtnOpendir)
	ON_BN_CLICKED(IDC_BTN_COUNTALL, &CCodeCountDlg::OnBnClickedBtnCountall)
	ON_BN_CLICKED(IDC_BTN_CLS, &CCodeCountDlg::OnBnClickedBtnCls)
END_MESSAGE_MAP()


// CCodeCountDlg 消息处理程序

BOOL CCodeCountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_lcResult.InsertColumn(0,_T("文件名"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(1,_T("文件路径"), LVCFMT_LEFT, 180, -1);
	m_lcResult.InsertColumn(2,_T("总行数"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(3,_T("代码行"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(4,_T("注释行"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(5,_T("空白行"), LVCFMT_LEFT, 60, -1);
	m_lcResult.SetExtendedStyle(m_lcResult.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CCodeCountDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCodeCountDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//遍历目录及其子目录代码
void CCodeCountDlg::SearchFile(CString strPath, CString ExtensionName1, CString ExtensionName2, CString ExtensionName3)
{

	if ( strPath.Right( 1 ) != _T( "\\" ) )
		strPath += _T( "\\" );
	strPath += _T( "*.*" );
	CFileFind file;
	BOOL bContinue = file.FindFile( strPath );

	while ( bContinue )     //遍历所选目录
	{
		bContinue = file.FindNextFile();
		if ( file.IsDirectory() && ! file.IsDots() )  //如果是文件夹，则递归调用本函数
		{
			SearchFile(file.GetFilePath(), ExtensionName1, ExtensionName2, ExtensionName3);
		}
		else if ( ! file.IsDots() )       //如果是文件，则判断是否满足扩展名要求
		{
			//if ( file.GetFileName().Find( ExtensionName.Right( 4 ) ) != -1 )
			CString s=file.GetFileName();
			int n = s.Find('.')+1;
			CString tmp = s.Right(s.GetLength()-n);
			int i = tmp.CompareNoCase( ExtensionName1 );
			int j = tmp.CompareNoCase( ExtensionName2 );
			int k = tmp.CompareNoCase( ExtensionName3 );
			if(  i == 0 || j == 0 || 0 == k )//判断是否符合扩展名
			{
				vcFile.push_back(file.GetFileName());//将文件名保存到全局变量
				m_strFilename = file.GetFilePath();//将文件路径保存到全局变量
				vcFilePath.push_back(file.GetFilePath());
			}
		} 
	}
}

//计算代码总行数
void CCodeCountDlg::OnBnClickedBtnCount()
{
	GetDlgItemText(IDC_EDIT_FILEDIR,m_strFilepath);
	if(m_strFilepath==_T(""))
		AfxMessageBox(_T("请输入需打开的文件夹路径!"));
	m_nFiles = (int)vcFilePath.size()-1;
////////////////////////////////////////////////////////////////////
	//SearchFile(m_strFilepath,_T("c"), _T("h"), _T("cpp"));
	////将结果显示在列表框内	
	//for (int i=0;i<(int)vcFile.size();i++)
	//{
	//	m_lcResult.InsertItem(i,vcFile[i]);
	//	m_lcResult.SetItemText(i,1,vcFilePath[i]);
	//}
////////////////////////////////////////////////////////////////////
	for (int i=0;i<5;i++)
	{
		::CloseHandle(::CreateThread(NULL, 0, MyThreadProc, (void*)this, 0, NULL));
		//相当于 hThread = ::CreateThread();
		//::ClosedHandle(hThreade);
	}
}

//打开目录
void CCodeCountDlg::OnBnClickedBtnOpendir()
{
	CFolderDialog dlg(_T(""), BIF_RETURNONLYFSDIRS, this);
	if(dlg.DoModal()==IDOK)
	{
		m_strFilepath = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_FILEDIR,m_strFilepath);
		SearchFile(m_strFilepath,_T("c"), _T("h"), _T("cpp"));
		
//将结果显示在列表框内	
		for (int i=0;i<(int)vcFile.size();i++)
		{
			m_lcResult.InsertItem(i,vcFile[i]);
			m_lcResult.SetItemText(i,1,vcFilePath[i]);
		}
	}
}

DWORD WINAPI CCodeCountDlg::MyThreadProc(LPVOID pParam)
{
	//因线程函数为全局静态函数，故需要*me对象传值
	CCodeCountDlg *me = (CCodeCountDlg *)pParam;
	while (TRUE)//令线程函数不断运行
	{
		me->m_lock.Lock();
		if(me->m_nFiles > -1)
		{	
			CString str2;
			int iAll = me->GetFileLines(me->vcFilePath[me->m_nFiles],&me->nLength,&me->nCodeLines,&me->nCommentLines,&me->nBlankLines);	
			
			gAllTotol += iAll;
			gAllBlankLines += me->nBlankLines;
			gAllCodeLines += me->nCodeLines;
			gAllCommentLines += me->nCommentLines;
			
			str2.Format(_T("%d"),iAll);
			me->strCodeLines.Format(_T("%d"),me->nCodeLines);
			me->strCommentLines.Format(_T("%d"),me->nCommentLines);
			me->strBlankLines.Format(_T("%d"),me->nBlankLines);

			me->m_lcResult.SetItemText(me->m_nFiles, 2, str2);
			me->m_lcResult.SetItemText(me->m_nFiles, 3, me->strCodeLines);
			me->m_lcResult.SetItemText(me->m_nFiles, 4, me->strCommentLines);
			me->m_lcResult.SetItemText(me->m_nFiles, 5, me->strBlankLines);
			me->m_nFiles--;
		}
		else
			break;

		me->m_lock.Unlock();

	}
	
	return 0;
}

int CCodeCountDlg::GetFileLines(LPCTSTR strFileName, int* pnLength, int *pnCodeLines, int* pnCommentLines, int* pnBlankLines)
{
	*pnLength = 0;
	*pnCodeLines = 0;
	*pnCommentLines = 0;
	*pnBlankLines = 0;

	CStdioFile file;
	if(file.Open(strFileName, CFile::modeRead)==FALSE)
		return 0;

	int nLines = 0;
	int nCodeLines = 0;
	int nCommentLines = 0;
	int nBlankLines = 0;

	BOOL bCommentSet = FALSE; //注释行统计标识 有"/*"时TRUE, "*/"时FALSE
	BOOL bQuatoSet = FALSE;   //字符串统计标识 首次一行有奇数个"时TRUE, 下一行有奇数个"时FALSE

	int nLength = (int)file.GetLength();
	*pnLength = nLength;

	CString bufRead;

	int nLineCommentBegin = 0;
	while(file.ReadString(bufRead)!=FALSE)
	{
		BOOL bStatedComment = FALSE;//本行作为注释行是否已统计过
		BOOL bStatedCode = FALSE;   //本行作为代码行是否已统计过

		nLines++;

		bufRead.TrimLeft(); //先将文件头的空格或制表符去掉

		if(bufRead.GetLength()==0) //为空白行
		{
			nBlankLines++;
			continue;
		}

		if(bCommentSet && bufRead.Find(_T("*/"))==-1)
		{
			nCommentLines++;
			continue;
		}

		if(bufRead.Find(_T("//"))==-1 && bufRead.Find(_T("/*"))==-1 && bufRead.Find(_T("*/"))==-1)
		{//如果本行根本就无注释符，则要不是注释符，要不是代码行
			if(bCommentSet)
			{
				nCommentLines++; continue;
			}
			else
			{
				if(bufRead.Find('"')==-1)
				{
					nCodeLines++; continue;
				}
			}
		}

		if(bufRead.Find(_T("//"))==0 && !bCommentSet && !bQuatoSet)
		{
			nCommentLines++;
			continue;
		}

		BOOL bDoubleSplashFound = FALSE;
		BOOL bSplashStarFound = FALSE;
		for(int i=0; i<bufRead.GetLength()-1; i++)
		{
			//char cTemp = bufRead[i];
			wchar_t cTemp = bufRead[i];
			if(bufRead[i]=='/' && bufRead[i+1]=='/' && !bCommentSet && !bQuatoSet)
			{
				if(!bStatedComment && (m_nStatMethod==1 || m_nStatMethod ==2))
				{
					bStatedComment = TRUE;
					nCommentLines++;
				}
				bDoubleSplashFound = TRUE;
				//i++;//应该+1，但也没有什么用处
				break;
			}
			else if(bufRead[i]=='/' && bufRead[i+1]=='*' && !bCommentSet && !bQuatoSet)
			{
				if(!bStatedComment && (m_nStatMethod==1 || m_nStatMethod ==2))
				{
					bStatedComment = TRUE;
					nCommentLines++;
				}
				bCommentSet = TRUE;
				bSplashStarFound = TRUE;
				i++;
			}
			//计算代码行必须在bCommentSet关闭之前
			else if(bufRead[i]!=' ' && bufRead[i]!='\t' && !bCommentSet)
			{
				if(!bStatedCode)
				{
					bStatedCode = TRUE;
					nCodeLines++;
				}
				if(bufRead[i]=='\\')
				{//\之后的字符要跳过
					i++;
					continue;
				}
				if(bufRead[i]=='\'')
				{
					if(bufRead[i+1]=='\\')
						i+=2;
					else
						i+=1;
					continue;
				}
				if(bufRead[i]=='"')
				{//"必须引起重视，感谢ltzhou
					bQuatoSet = !bQuatoSet;
				}
			}
			else if(bufRead[i]=='*' && bufRead[i+1]=='/' && bCommentSet && !bQuatoSet)
			{
				if(!bStatedComment && (m_nStatMethod==1 || m_nStatMethod ==2))
				{
					bStatedComment = TRUE;
					nCommentLines++;
				}
				bCommentSet = FALSE;
				bSplashStarFound = TRUE;
				i++;
			}
		}

		if(bDoubleSplashFound)
		{
			if(m_nStatMethod==2 && bStatedCode) //如果统计方法为第三种，且同时有代码行与注释行，则只计注释行
			{
				nCodeLines--;
			}
			if(m_nStatMethod==0 && !bStatedCode)//如果统计方法为第一种，且未作为代码行统计过，那么必为注释行
			{
				nCommentLines++;
			}
			continue;
		}

		if(bufRead[bufRead.GetLength()-1]=='"'&&!bCommentSet)
		{//若某行最后一个是"，则必定用来关闭bQuatoSet，记代码行一行，否则报错
			bQuatoSet = !bQuatoSet;
			if(!bQuatoSet)
			{
				if(!bStatedCode)
				{
					bStatedCode = TRUE;
					nCodeLines++;
				}
			}
			else
			{
				CStdioFile fileLog;
				if(fileLog.Open(m_strLogFile, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate)==TRUE)
				{
					CString strMsg;
					if(fileLog.GetLength()==0)
					{
						strMsg.Format(_T("文件\t行\t问题\n"), strFileName, nLines);
						fileLog.WriteString(strMsg);
					}
					strMsg.Format(_T("%s\t%d\t字符串换行未用\\\n"), strFileName, nLines);
					fileLog.WriteString(strMsg);
					fileLog.Close();
				}
			}
			continue;
		}

		if(bufRead[bufRead.GetLength()-1]!=' ' && bufRead[bufRead.GetLength()-1]!='\t' && !bCommentSet
			&& bufRead[bufRead.GetLength()-2]!='*' && bufRead[bufRead.GetLength()-1]!='/')
		{//如果最后一个字符非空格或制表符，且前面无/*，最后两个字符不是*/，则为代码行
			if(!bStatedCode)
			{
				bStatedCode = TRUE;
				nCodeLines++;
			}
		}

		if(bSplashStarFound)
		{
			if(m_nStatMethod==2 && bStatedCode) //如果统计方法为第三种，且同时有代码行与注释行，则只计注释行
			{
				nCodeLines--;
			}

			if(m_nStatMethod==0 && !bStatedCode && !bStatedComment)	//若该行无代码如    /*abc*/ //222
				//但是统计方法是第一种，则需要追加注释行计数一次
			{
				bStatedComment = TRUE;
				nCommentLines++;
			}
		}

		if(!bStatedComment && bCommentSet)//可能是前面有/*，在第一种统计方法中，未作为代码行计算过，那么本行肯定是注释行
		{
			if(m_nStatMethod==0 && !bStatedCode)
			{
				bStatedComment = TRUE;
				nCommentLines++;
			}
		}

		if(bQuatoSet && bufRead[bufRead.GetLength()-1]!='\\')
		{
			CStdioFile fileLog;
			if(fileLog.Open(m_strLogFile, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate)==TRUE)
			{
				CString strMsg;
				if(fileLog.GetLength()==0)
				{
					strMsg.Format(_T("文件\t行\t问题\n"), strFileName, nLines);
					fileLog.WriteString(strMsg);
				}
				strMsg.Format(_T("%s\t%d\t字符串换行未用\\\n"), strFileName, nLines);
				fileLog.WriteString(strMsg);
				fileLog.Close();
			}
		}

	}

	*pnCodeLines = nCodeLines;
	*pnCommentLines = nCommentLines;
	*pnBlankLines = nBlankLines;

	file.Close();

	return nLines;
}


void CCodeCountDlg::OnBnClickedBtnCountall()
{
	SetDlgItemInt(IDC_EDIT_ALLTOTOL,gAllTotol,TRUE);
	SetDlgItemInt(IDC_EDIT_ALLCODELINES,gAllCodeLines,TRUE);
	SetDlgItemInt(IDC_EDIT_ALLCOMENTLINES,gAllCommentLines,TRUE);
	SetDlgItemInt(IDC_EDIT_ALLBALKLINES,gAllBlankLines,TRUE);
}

void CCodeCountDlg::OnBnClickedBtnCls()
{
	EmptyCodeCount();
}
void CCodeCountDlg::EmptyCodeCount()
{
	m_strFilepath = _T("");
	m_nCodeLines = 0;
	m_nBlankLines = 0;
	m_nFiles = 0;
	SetDlgItemText(IDC_EDIT_FILEDIR,m_strFilepath);

	gAllTotol = 0;
	gAllCommentLines = 0;
	gAllBlankLines = 0;
	gAllCodeLines = 0;
	SetDlgItemInt(IDC_EDIT_ALLTOTOL,gAllTotol,TRUE);
	SetDlgItemInt(IDC_EDIT_ALLCODELINES,gAllCodeLines,TRUE);
	SetDlgItemInt(IDC_EDIT_ALLCOMENTLINES,gAllCommentLines,TRUE);
	SetDlgItemInt(IDC_EDIT_ALLBALKLINES,gAllBlankLines,TRUE);

	m_lcResult.DeleteAllItems();
	vcFile.clear();
	vcFilePath.clear();
}