// CCPUTime.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CCPUTime.h"
#include "afxdialogex.h"

#define  WM_UPDATEDATA WM_USER+0x222
// CCPUTime �Ի���

IMPLEMENT_DYNAMIC(CCPUTime, CDialogEx)

CCPUTime::CCPUTime(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
	, m_editStr(_T(""))
	, m_editMem(_T(""))
{

}

CCPUTime::~CCPUTime()
{
}

void CCPUTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_editStr);
	DDV_MaxChars(pDX, m_editStr, 256);
	DDX_Text(pDX, IDC_EDIT2, m_editMem);
}


BEGIN_MESSAGE_MAP(CCPUTime, CDialogEx)
	ON_MESSAGE(WM_UPDATEDATA, &CCPUTime::OnUpdatedata)
	ON_BN_CLICKED(IDC_BUTTON1, &CCPUTime::OnClickedButton1)
END_MESSAGE_MAP()

__int64 CCPUTime::CompareFileTime(FILETIME time1, FILETIME time2)
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;
	return   (b - a);
}

// CCPUTime ��Ϣ�������


BOOL CCPUTime::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HANDLE hThread;
	DWORD  threadId;
	// �����߳�
	hThread = CreateThread(NULL, 0, ThreadFunc, this, 0, &threadId);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

DWORD WINAPI ThreadFunc(LPVOID p)
{
	CCPUTime *pCpu = (CCPUTime*)p;

	HANDLE hEvent;

	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // ��ʼֵΪ nonsignaled ������ÿ�δ������Զ�����Ϊnonsignaled
	while (1) {

		WaitForSingleObject(hEvent, 1000); //�ȴ�1000����

		FILETIME idleTime;
		FILETIME kernelTime;
		FILETIME userTime;
		GetSystemTimes(&idleTime, &kernelTime, &userTime);

		int idle = pCpu->CompareFileTime(preidleTime, idleTime);
		int kernel = pCpu->CompareFileTime(prekernelTime, kernelTime);
		int user = pCpu->CompareFileTime(preuserTime, userTime);

		int rate = (kernel + user - idle) * 100 / (kernel + user);

		pCpu->m_editStr.Format(L"%d%%", rate);
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;

		//��ȡ�ڴ�ʹ����
		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof(memStatus);
		GlobalMemoryStatusEx(&memStatus);
		pCpu->m_editMem.Format(L"%d%%", memStatus.dwMemoryLoad);

		SendMessage(pCpu->m_hWnd, WM_UPDATEDATA, 0, 0);
	}
	return 0;
}


afx_msg LRESULT CCPUTime::OnUpdatedata(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);
	return 0;
}

//�����ڴ�
void CCPUTime::OnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	MEMORYSTATUSEX memStatus;
	memStatus.dwLength = sizeof(memStatus);
	GlobalMemoryStatusEx(&memStatus);

	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	for (DWORD i = 0; i < dwNeedSize/sizeof(DWORD); i++)
	{
		HANDLE hPro = OpenProcess(PROCESS_SET_QUOTA, FALSE, dwPIDList[i]);
		SetProcessWorkingSetSize(hPro, -1, -1);
	}

	GlobalMemoryStatusEx(&memStatus);
	m_editMem.Format(L"%d%%", memStatus.dwMemoryLoad);
	MessageBox(L"����ɹ�");
}
