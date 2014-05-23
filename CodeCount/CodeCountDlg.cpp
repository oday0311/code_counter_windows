// CodeCountDlg.cpp : ʵ���ļ�
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
// CCodeCountDlg �Ի���

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


// CCodeCountDlg ��Ϣ�������

BOOL CCodeCountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_lcResult.InsertColumn(0,_T("�ļ���"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(1,_T("�ļ�·��"), LVCFMT_LEFT, 180, -1);
	m_lcResult.InsertColumn(2,_T("������"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(3,_T("������"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(4,_T("ע����"), LVCFMT_LEFT, 60, -1);
	m_lcResult.InsertColumn(5,_T("�հ���"), LVCFMT_LEFT, 60, -1);
	m_lcResult.SetExtendedStyle(m_lcResult.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CCodeCountDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CCodeCountDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//����Ŀ¼������Ŀ¼����
void CCodeCountDlg::SearchFile(CString strPath, CString ExtensionName1, CString ExtensionName2, CString ExtensionName3)
{

	if ( strPath.Right( 1 ) != _T( "\\" ) )
		strPath += _T( "\\" );
	strPath += _T( "*.*" );
	CFileFind file;
	BOOL bContinue = file.FindFile( strPath );

	while ( bContinue )     //������ѡĿ¼
	{
		bContinue = file.FindNextFile();
		if ( file.IsDirectory() && ! file.IsDots() )  //������ļ��У���ݹ���ñ�����
		{
			SearchFile(file.GetFilePath(), ExtensionName1, ExtensionName2, ExtensionName3);
		}
		else if ( ! file.IsDots() )       //������ļ������ж��Ƿ�������չ��Ҫ��
		{
			//if ( file.GetFileName().Find( ExtensionName.Right( 4 ) ) != -1 )
			CString s=file.GetFileName();
			int n = s.Find('.')+1;
			CString tmp = s.Right(s.GetLength()-n);
			int i = tmp.CompareNoCase( ExtensionName1 );
			int j = tmp.CompareNoCase( ExtensionName2 );
			int k = tmp.CompareNoCase( ExtensionName3 );
			if(  i == 0 || j == 0 || 0 == k )//�ж��Ƿ������չ��
			{
				vcFile.push_back(file.GetFileName());//���ļ������浽ȫ�ֱ���
				m_strFilename = file.GetFilePath();//���ļ�·�����浽ȫ�ֱ���
				vcFilePath.push_back(file.GetFilePath());
			}
		} 
	}
}

//�������������
void CCodeCountDlg::OnBnClickedBtnCount()
{
	GetDlgItemText(IDC_EDIT_FILEDIR,m_strFilepath);
	if(m_strFilepath==_T(""))
		AfxMessageBox(_T("��������򿪵��ļ���·��!"));
	m_nFiles = (int)vcFilePath.size()-1;
////////////////////////////////////////////////////////////////////
	//SearchFile(m_strFilepath,_T("c"), _T("h"), _T("cpp"));
	////�������ʾ���б����	
	//for (int i=0;i<(int)vcFile.size();i++)
	//{
	//	m_lcResult.InsertItem(i,vcFile[i]);
	//	m_lcResult.SetItemText(i,1,vcFilePath[i]);
	//}
////////////////////////////////////////////////////////////////////
	for (int i=0;i<5;i++)
	{
		::CloseHandle(::CreateThread(NULL, 0, MyThreadProc, (void*)this, 0, NULL));
		//�൱�� hThread = ::CreateThread();
		//::ClosedHandle(hThreade);
	}
}

//��Ŀ¼
void CCodeCountDlg::OnBnClickedBtnOpendir()
{
	CFolderDialog dlg(_T(""), BIF_RETURNONLYFSDIRS, this);
	if(dlg.DoModal()==IDOK)
	{
		m_strFilepath = dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_FILEDIR,m_strFilepath);
		SearchFile(m_strFilepath,_T("c"), _T("h"), _T("cpp"));
		
//�������ʾ���б����	
		for (int i=0;i<(int)vcFile.size();i++)
		{
			m_lcResult.InsertItem(i,vcFile[i]);
			m_lcResult.SetItemText(i,1,vcFilePath[i]);
		}
	}
}

DWORD WINAPI CCodeCountDlg::MyThreadProc(LPVOID pParam)
{
	//���̺߳���Ϊȫ�־�̬����������Ҫ*me����ֵ
	CCodeCountDlg *me = (CCodeCountDlg *)pParam;
	while (TRUE)//���̺߳�����������
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

	BOOL bCommentSet = FALSE; //ע����ͳ�Ʊ�ʶ ��"/*"ʱTRUE, "*/"ʱFALSE
	BOOL bQuatoSet = FALSE;   //�ַ���ͳ�Ʊ�ʶ �״�һ����������"ʱTRUE, ��һ����������"ʱFALSE

	int nLength = (int)file.GetLength();
	*pnLength = nLength;

	CString bufRead;

	int nLineCommentBegin = 0;
	while(file.ReadString(bufRead)!=FALSE)
	{
		BOOL bStatedComment = FALSE;//������Ϊע�����Ƿ���ͳ�ƹ�
		BOOL bStatedCode = FALSE;   //������Ϊ�������Ƿ���ͳ�ƹ�

		nLines++;

		bufRead.TrimLeft(); //�Ƚ��ļ�ͷ�Ŀո���Ʊ��ȥ��

		if(bufRead.GetLength()==0) //Ϊ�հ���
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
		{//������и�������ע�ͷ�����Ҫ����ע�ͷ���Ҫ���Ǵ�����
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
				//i++;//Ӧ��+1����Ҳû��ʲô�ô�
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
			//��������б�����bCommentSet�ر�֮ǰ
			else if(bufRead[i]!=' ' && bufRead[i]!='\t' && !bCommentSet)
			{
				if(!bStatedCode)
				{
					bStatedCode = TRUE;
					nCodeLines++;
				}
				if(bufRead[i]=='\\')
				{//\֮����ַ�Ҫ����
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
				{//"�����������ӣ���лltzhou
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
			if(m_nStatMethod==2 && bStatedCode) //���ͳ�Ʒ���Ϊ�����֣���ͬʱ�д�������ע���У���ֻ��ע����
			{
				nCodeLines--;
			}
			if(m_nStatMethod==0 && !bStatedCode)//���ͳ�Ʒ���Ϊ��һ�֣���δ��Ϊ������ͳ�ƹ�����ô��Ϊע����
			{
				nCommentLines++;
			}
			continue;
		}

		if(bufRead[bufRead.GetLength()-1]=='"'&&!bCommentSet)
		{//��ĳ�����һ����"����ض������ر�bQuatoSet���Ǵ�����һ�У����򱨴�
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
						strMsg.Format(_T("�ļ�\t��\t����\n"), strFileName, nLines);
						fileLog.WriteString(strMsg);
					}
					strMsg.Format(_T("%s\t%d\t�ַ�������δ��\\\n"), strFileName, nLines);
					fileLog.WriteString(strMsg);
					fileLog.Close();
				}
			}
			continue;
		}

		if(bufRead[bufRead.GetLength()-1]!=' ' && bufRead[bufRead.GetLength()-1]!='\t' && !bCommentSet
			&& bufRead[bufRead.GetLength()-2]!='*' && bufRead[bufRead.GetLength()-1]!='/')
		{//������һ���ַ��ǿո���Ʊ������ǰ����/*����������ַ�����*/����Ϊ������
			if(!bStatedCode)
			{
				bStatedCode = TRUE;
				nCodeLines++;
			}
		}

		if(bSplashStarFound)
		{
			if(m_nStatMethod==2 && bStatedCode) //���ͳ�Ʒ���Ϊ�����֣���ͬʱ�д�������ע���У���ֻ��ע����
			{
				nCodeLines--;
			}

			if(m_nStatMethod==0 && !bStatedCode && !bStatedComment)	//�������޴�����    /*abc*/ //222
				//����ͳ�Ʒ����ǵ�һ�֣�����Ҫ׷��ע���м���һ��
			{
				bStatedComment = TRUE;
				nCommentLines++;
			}
		}

		if(!bStatedComment && bCommentSet)//������ǰ����/*���ڵ�һ��ͳ�Ʒ����У�δ��Ϊ�����м��������ô���п϶���ע����
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
					strMsg.Format(_T("�ļ�\t��\t����\n"), strFileName, nLines);
					fileLog.WriteString(strMsg);
				}
				strMsg.Format(_T("%s\t%d\t�ַ�������δ��\\\n"), strFileName, nLines);
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