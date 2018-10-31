// CMenuProcess.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CMenuProcess.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// CMenuProcess �Ի���

IMPLEMENT_DYNAMIC(CMenuProcess, CDialogEx)

CMenuProcess::CMenuProcess(DWORD id, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent),
	m_dwMenuId(id)
{

}

CMenuProcess::~CMenuProcess()
{
}

BOOL  CMenuProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("�Ӵ���"));
	//���ڳ�ʼ��������
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//���ݲ˵�ѡ��Ĳ�ͬ ��ʼ��������Ҳ��ͬ
	switch (m_dwMenuId)
	{
	case 32771:
	{
		m_ListCtrl.InsertColumn(0, _T("ģ������"), 0, 150);
		m_ListCtrl.InsertColumn(1, _T("ģ��ID"), 0, 50);
		m_ListCtrl.InsertColumn(2, _T("ģ����������ID"), 0, 50);
		m_ListCtrl.InsertColumn(3, _T("ģ��·��"), 0, 200);
	}
	break;
	case 32772:
	{
		m_ListCtrl.InsertColumn(0, _T("��ID"), 0, 150);
		m_ListCtrl.InsertColumn(1, _T("����ID"), 0, 50);
		m_ListCtrl.InsertColumn(2, _T("�Ѵ�С"), 0, 50);
		m_ListCtrl.InsertColumn(3, _T("�ѵ�ַ"), 0, 200);
	}
	break;
	case 32773:
	{
		m_ListCtrl.InsertColumn(0, _T("�߳�ID"), 0, 100);
		m_ListCtrl.InsertColumn(1, _T("��������ID"), 0, 100);
	}
	break;
	default:
		break;
	}
	return TRUE;
}

void CMenuProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

void CMenuProcess::ShowHeap(DWORD id)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPHEAPLIST, id);

	HEAPLIST32 peList = { sizeof(HEAPLIST32) };

	BOOL bSuccess = Heap32ListFirst(hSnap, &peList);

	if (bSuccess)
	{
		HEAPENTRY32 pe = { sizeof(HEAPENTRY32) };
		do 
		{
			
			Heap32First(&pe, peList.th32ProcessID, peList.th32HeapID);
			do 
			{
				CString str;
				str.Format(_T("%d"), pe.th32HeapID);
				m_ListCtrl.InsertItem(0, str);
				str.Format(_T("%d"), pe.th32ProcessID);
				m_ListCtrl.SetItemText(0, 1, str);
				str.Format(_T("%d"), pe.dwBlockSize);
				m_ListCtrl.SetItemText(0, 2, str);
				str.Format(_T("0x%x"), pe.dwAddress);
				m_ListCtrl.SetItemText(0, 3, str);


			} while (FALSE);
			
		} while (Heap32ListNext(hSnap, &peList));
	}
	CloseHandle(hSnap);
}

void CMenuProcess::ShowThread(DWORD id)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, id);

	THREADENTRY32 pe = { sizeof(THREADENTRY32) };

	BOOL bSuccess = Thread32First(hSnap, &pe);

	if (bSuccess)
	{
		do 
		{
			CString str;
			str.Format(_T("%d"), pe.th32ThreadID);
			m_ListCtrl.InsertItem(0, str);
			str.Format(_T("%d"), pe.th32OwnerProcessID);
			m_ListCtrl.SetItemText(0, 1, str);

		} while (Thread32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);
}

void CMenuProcess::ShowModule(DWORD id)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	MODULEENTRY32 pe = { sizeof(MODULEENTRY32) };

	BOOL bSuccess = Module32First(hSnap, &pe);

	if (bSuccess)
	{
		do 
		{
			CString str;
			str.Format(_T("%d"), pe.th32ModuleID);
			m_ListCtrl.InsertItem(0, pe.szModule);
			m_ListCtrl.SetItemText(0, 1, str);
			str.Format(_T("%d"), pe.th32ProcessID);
			m_ListCtrl.SetItemText(0, 2, str);
			m_ListCtrl.SetItemText(0, 3, pe.szExePath);
		} while (Module32Next(hSnap, &pe));
	}
	CloseHandle(hSnap);
}

BEGIN_MESSAGE_MAP(CMenuProcess, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMenuProcess::OnRclickList1)
	ON_COMMAND_RANGE(ID_SUBMENU, ID_SUBMENU2, &CMenuProcess::OnRClickMenu)
END_MESSAGE_MAP()


// CMenuProcess ��Ϣ�������



void CMenuProcess::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��������߳̽��� ���� �����˵�
	if (m_dwMenuId != 32773)
	{
		return;
	}
	CPoint pt;
	GetCursorPos(&pt);
	m_ListCtrl.ScreenToClient(&pt);
	LVHITTESTINFO info;
	info.pt = pt;
	//��ȡ�������ǰ�е�����
	m_nItem = m_ListCtrl.HitTest(&info);
	if (m_nItem != -1)
	{
		CMenu menu;
		CPoint point;
		GetCursorPos(&point);
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_SUBMENU, L"�����߳�");
		menu.AppendMenu(MF_STRING, ID_SUBMENU1, L"�ָ��߳�");
		menu.AppendMenu(MF_STRING, ID_SUBMENU2, L"�����߳�");
		menu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}

void CMenuProcess::OnRClickMenu(UINT id)
{
	//��ȡ�߳�ID
	CString str;
	str = m_ListCtrl.GetItemText(m_nItem, 0);
	//�߳�ID
	DWORD pid = 0;
	swscanf_s(str, L"%d", &pid);
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, pid);

	if (hThread == INVALID_HANDLE_VALUE)
	{
		return;
	}
	//��Ӧ�˵��¼�
	switch (id)
	{
	case 200:
	{
		DWORD err = SuspendThread(hThread);
		if (err != 0)
		{
			MessageBox(L"�����̳߳ɹ�");
		}
	}
	break;
	case 201:
	{
		DWORD err = ResumeThread(hThread);
		if (err != 0)
		{
			MessageBox(L"�ָ��̳߳ɹ�");
		}
	}
	break;
	case 202:
	{
		BOOL bSuccess = TerminateThread(hThread, 0);
		if (bSuccess)
		{
			m_ListCtrl.DeleteItem(m_nItem);
			MessageBox(L"ɾ���̳߳ɹ�");
		}
	}
	break;
	default:
		break;
	}
}