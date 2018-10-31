// CDIALOG1.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CDIALOG1.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include <Psapi.h>
#include "CMenuProcess.h"


// CDIALOG1 �Ի���

IMPLEMENT_DYNAMIC(CDIALOG1, CDialogEx)

CDIALOG1::CDIALOG1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDIALOG1::~CDIALOG1()
{
}

void CDIALOG1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDIALOG1, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDIALOG1::OnRclickList1)
	ON_COMMAND_RANGE(ID_32771, ID_32774, &CDIALOG1::OnRClickMenu)
END_MESSAGE_MAP()


// CDIALOG1 ��Ϣ�������

BOOL  CDIALOG1::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("�Ӵ���"));
	//���ڳ�ʼ��������
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("����"), 0, 100);
	m_ListCtrl.InsertColumn(1, _T("PID"), 0, 50);
	m_ListCtrl.InsertColumn(2, _T("·��"), 0, 300);

	ShowProcess();
	
	return TRUE;
}

//������ʾ���н���
void CDIALOG1::ShowProcess()
{
	//��ȡ���վ��
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
	//��ȡ��һ������
	BOOL bSucess = Process32First(hSnap, &pe32);
	USES_CONVERSION;
	if (bSucess)
	{
		do 
		{
			m_ListCtrl.InsertItem(0, pe32.szExeFile);
			CString str;
			str.Format(_T("%d"), pe32.th32ProcessID);
			m_ListCtrl.SetItemText(0, 1, str);
			//�򿪽��̻�ȡexe·��
			HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, 
				FALSE, pe32.th32ProcessID);
			TCHAR szPath[MAX_PATH] = { 0 };
			GetModuleFileNameEx(hPro, 0, szPath, MAX_PATH);
			m_ListCtrl.SetItemText(0, 2, szPath);
		} while (Process32Next(hSnap, &pe32));
	}
	//�رտ���
	CloseHandle(hSnap);
}

void CDIALOG1::OnRClickMenu(UINT id)
{
	//��ȡ����ID
	CString str;
	str = m_ListCtrl.GetItemText(m_RowNum, 1);
	//����ID
	DWORD pid = 0;
	swscanf_s(str, L"%d", &pid);
	//�رս���
	if(id == 32774)
	{
		if (id == GetCurrentProcessId())
		{
			//�ر��Լ�
			ExitProcess(0);
		}
		else
		{
			//�رձ���
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			BOOL bSuccess  = TerminateProcess(hProcess, 0);
			if (bSuccess)
			{
				m_ListCtrl.DeleteItem(m_RowNum);
				MessageBox(L"�رս��̳ɹ�");
			}
			
		}
		return;
	}
	//Ϊ��ģ̬���������ڴ�
	CMenuProcess* pDlg = new CMenuProcess(id);
 	pDlg->Create(IDD_DIALOG3);
 	pDlg->ShowWindow(SW_SHOW);
	
	//���ݲ�ͬid��ʾ��ͬ��Ϣ
	switch (id)
	{
	//����ģ��
	case 32771:
	{
		pDlg->ShowModule(pid);
	}
	break;
	//������
	case 32772:
	{
		pDlg->ShowHeap(pid);
	}
	break;
	//�����߳�
	case 32773:
	{
		pDlg->ShowThread(pid);
	}
	break;
	default:
		break;
	}
}

void CDIALOG1::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CPoint pos;
	GetCursorPos(&pos);

	m_ListCtrl.ScreenToClient(&pos);

	LVHITTESTINFO info;

	//��ȡ�������ǰ�е�����
	info.pt = pos;
	INT nItem = m_ListCtrl.HitTest(&info);
	//�����Ϊ-1  ����˵�
	if (nItem != -1)
	{
		m_RowNum = nItem;
		CMenu menu;
		menu.LoadMenu(IDR_MENU1);
		CPoint point;
		GetCursorPos(&point);
		
		CMenu *pSubMebu = menu.GetSubMenu(0);
		pSubMebu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}

}
