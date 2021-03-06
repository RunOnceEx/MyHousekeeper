
// MyMFCDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MyMFC.h"
#include "MyMFCDlg.h"
#include "afxdialogex.h"
#include "CDIALOG1.h"
#include "CDIALOG2.h"
#include "CFileDlg.h"
#include "CClearFile.h"
#include "CCPUTime.h"
#include "CPEFile.h"
#include "CService.h"
#include "MD5/MD5Checksum.h"
#include <algorithm>
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyMFCDlg 对话框



CMyMFCDlg::CMyMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYMFC_DIALOG, pParent)
	, m_strPath(_T(""))
	, m_IsWindowHide(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_strPath);
}

BEGIN_MESSAGE_MAP(CMyMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyMFCDlg::OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyMFCDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyMFCDlg::OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyMFCDlg::OnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyMFCDlg::OnClickedButton4)
	ON_MESSAGE(WM_ONMYSOCKET, &CMyMFCDlg::OnMySocket)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyMFCDlg::OnClickedButton5)
END_MESSAGE_MAP()


// CMyMFCDlg 消息处理程序

BOOL CMyMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	//提升权限
	HANDLE hToken = NULL;
	HANDLE hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	TOKEN_PRIVILEGES tp = { 0 };
	LookupPrivilegeValue(0, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

	//SetWindowText(_T("任务管理器"));
	SetWindowText(_T("安全卫士"));
	// TODO: 在此添加额外的初始化代码
	m_TabCtrl.InsertItem(0, _T("进程"));
	m_TabCtrl.InsertItem(1, _T("窗口"));
	m_TabCtrl.InsertItem(2, _T("文件"));
	m_TabCtrl.InsertItem(3, _T("清理"));
	m_TabCtrl.InsertItem(4, _T("CPU"));
	m_TabCtrl.InsertItem(5, _T("PE"));
	m_TabCtrl.InsertItem(6, _T("服务"));

	m_pDia[0] = new CDIALOG1();//进程窗口
	m_pDia[1] = new CDIALOG2();//窗口信息
	m_pDia[2] = new CFileDlg();//文件窗口
	m_pDia[3] = new CClearFile();//清理VS窗口
	m_pDia[4] = new CCPUTime();//CPU及内存使用率
	m_pDia[5] = new CPEFile();//解析PE文件
	m_pDia[6] = new CService();//服务

	m_pDia[0]->Create(IDD_DIALOG1, &m_TabCtrl);	
	m_pDia[1]->Create(IDD_DIALOG2, &m_TabCtrl);
	m_pDia[2]->Create(IDD_DIALOG4, &m_TabCtrl);
	m_pDia[3]->Create(IDD_DIALOG5, &m_TabCtrl);
	m_pDia[4]->Create(IDD_DIALOG6, &m_TabCtrl);
	m_pDia[5]->Create(IDD_DIALOG7, &m_TabCtrl);
	m_pDia[6]->Create(IDD_SERVICE_DLG, &m_TabCtrl);
	
	CRect rect;
	m_TabCtrl.GetClientRect(rect);
	rect.DeflateRect(2, 23, 2, 2);

	for (int i = 0; i < (sizeof(m_pDia) / sizeof(m_pDia[0])); ++i)
	{
		m_pDia[i]->MoveWindow(rect);
		if (i == 0)
		{
			m_pDia[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDia[i]->ShowWindow(SW_HIDE);
		}
	}
	//注册全局热键
	BOOL isReg = ::RegisterHotKey(this->GetSafeHwnd(), 0xa001, MOD_CONTROL, 'K');


	//云查杀，要建立socket链接
	//链接服务器
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"链接服务器失败", L"Error", MB_OK | MB_ICONWARNING);
		return TRUE;
	}

	//异步IO
	WSAAsyncSelect(m_client.m_socket, m_hWnd, WM_ONMYSOCKET, FD_READ | FD_CLOSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyMFCDlg::OnPaint()
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
HCURSOR CMyMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyMFCDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	for (int i = 0; i < (sizeof(m_pDia) / sizeof(m_pDia[0])); ++i)
	{
		if (i == m_TabCtrl.GetCurSel())
		{
			m_pDia[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pDia[i]->ShowWindow(SW_HIDE);
		}
	}
}

void CMyMFCDlg::GetFileString(char *pFileName)
{
	m_vStr.clear();
	//打fileMD5文件 将当中存的MD5读取到动态数组当中
	FILE *pFile;
	errno_t err;
	if ((err = fopen_s(&pFile, pFileName, "r")) != 0)
	{
		MessageBox(L"文件打开失败");
		return;
	}

	char buf[64] = { 0 };
	CString str;
	char *p = NULL;
	USES_CONVERSION;
	while (!feof(pFile))
	{
		//逐行读取字符串
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), pFile);
		p = NULL;
		if ((p = strchr(buf, '\n')) != NULL)
			*p = '\0';
		str.Format(L"%s", A2W(buf));
		m_vStr.push_back(str);
	}
	fclose(pFile);
}

//MD5查杀
//病毒的MD5信息保存在key/fileMD5
//删除AntiVirus_MD5文件夹下所有与key/fileMD5中相匹配的文件
void CMyMFCDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strPath == "")
	{
		MessageBox(L"路径不能为空");
		return;
	}

	GetFileString("key/fileMD5.txt");
	//通过MD5比对删除病毒文件
	FindMD5VirusAndDelete(m_strPath, 0);
	MessageBox(L"杀毒成功");
}
/*dwTag 标识参数，0代表MD5查杀，1代表全路径查杀, 2代表云查杀*/
void CMyMFCDlg::FindMD5VirusAndDelete(LPCWSTR szFoldPath, DWORD dwTag)
{
	WIN32_FIND_DATA wfData = {};
	CString str;
	str.Format(L"%s%s", szFoldPath, L"\\*");
	HANDLE hFile = FindFirstFile(str, &wfData);
	do
	{
		//去掉两个特殊目录
		if (!wcscmp(wfData.cFileName, L".") || !wcscmp(wfData.cFileName, L".."))
		{
			continue;
		}
		WCHAR szSubFoldPath[MAX_PATH];
		wsprintf(szSubFoldPath, L"%s\\%s", szFoldPath, wfData.cFileName);
		//如果是文件，继续删除文件里面的内容，使用递归
		if (wfData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			FindMD5VirusAndDelete(szSubFoldPath, dwTag);
		}
		else
		{
			//文件MD5和fileMD5中记录的匹配
			CString strMD5 = CMD5Checksum::GetMD5(szSubFoldPath);
			//全路径查杀和MD5查杀
			if (dwTag == 0)
			{
				if (find(m_vStr.begin(), m_vStr.end(), strMD5) != m_vStr.end())
				{
					//删除文件
					DeleteFile(szSubFoldPath);
				}
			}
			else if(dwTag == 1)
			{
				if (find(m_vStr.begin(), m_vStr.end(), strMD5) != m_vStr.end() &&
					find(m_vStr.begin(), m_vStr.end(), szSubFoldPath) != m_vStr.end())
				{
					//删除文件
					DeleteFile(szSubFoldPath);
				}
			}
			else if (dwTag == 2)
			{
				//给服务端发送文件的MD5
				MessageContent msg = { 0 };
				USES_CONVERSION;
				strcpy(msg.name, W2A(wfData.cFileName));
				strcpy(msg.szMD5, W2A(strMD5.GetBuffer()));
				strcpy(msg.szFoldPath, W2A(szSubFoldPath));
				send(m_client.m_socket, (char*)&msg, sizeof(msg), NULL);
			}
		}
	} while (FindNextFile(hFile, &wfData));
}

//全路径查杀
void CMyMFCDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strPath == "")
	{
		MessageBox(L"路径不能为空");
		return;
	}

	GetFileString("key/filePath.txt");
	//通过MD5比对与全路径比对删除病毒文件
	FindMD5VirusAndDelete(m_strPath, 1);
	MessageBox(L"杀毒成功");
}

//白名单查杀
void CMyMFCDlg::OnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取白名单
	GetFileString("key/fileWhite.txt");

	//获取快照句柄
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
	//获取第一个进程
	BOOL bSucess = Process32First(hSnap, &pe32);
	USES_CONVERSION;
	if (bSucess)
	{
		do
		{
			if (find(m_vStr.begin(), m_vStr.end(), pe32.szExeFile) == m_vStr.end())
			{
				//获取进程句柄
				HANDLE hPro = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
				//关闭进程
				TerminateProcess(hPro, 0);
			}
		} while (Process32Next(hSnap, &pe32));
	}
	//关闭快照
	CloseHandle(hSnap);
	MessageBox(L"杀毒成功");
}

//云查杀
void CMyMFCDlg::OnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_strPath == "")
	{
		MessageBox(L"路径不能为空");
		return;
	}

	FindMD5VirusAndDelete(m_strPath, 2);
}

//服务端接收的消息处理函数
afx_msg LRESULT CMyMFCDlg::OnMySocket(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;
	WORD  wEvent = WSAGETSELECTEVENT(lParam);
	WORD wErrorCode = WSAGETSELECTERROR(lParam);
	if (wErrorCode)
	{
		exit(0);
		MessageBox(L"网络错误");
		CDialogEx::OnClose();
	}
	switch (wEvent)
	{
	case FD_READ:
	{
		TCHAR *szRecv = m_client.Recv();
		if (szRecv != nullptr)
		{
			DeleteFile(szRecv);
			MessageBox(L"杀毒成功");
		}
	}
	break;
	default:
		break;
	}
	return 0;
}

//查杀路径
void CMyMFCDlg::OnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CFolderPickerDialog fdlg;
	if (fdlg.DoModal() == IDOK)
	{
		m_strPath = fdlg.GetFolderPath();
		UpdateData(FALSE);
	}
}


BOOL CMyMFCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0xa001))
	{
		if (m_IsWindowHide == TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		else
		{
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
