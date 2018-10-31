// CService.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CService.h"
#include "afxdialogex.h"


// CService �Ի���

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVICE_DLG, pParent)
{

}

CService::~CService()
{
}

void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CService, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CService::OnRclickList2)
	ON_COMMAND_RANGE(ID_SUBMENU, ID_SUBMENU1, &CService::OnRClickMenu)
END_MESSAGE_MAP()


// CService ��Ϣ�������


BOOL CService::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("����"), 0, 150);
	m_ListCtrl.InsertColumn(1, _T("PID"), 0, 80);
	m_ListCtrl.InsertColumn(2, _T("����"), 0, 150);
	m_ListCtrl.InsertColumn(2, _T("״̬"), 0, 150);

	//��Զ�̼����������ƹ�����
	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;

	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);

	//������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)
		new char[dwSize];

	//�ڶ���ö��
	BOOL bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, (LPBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	CString str;
	//������Ϣ
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		m_ListCtrl.InsertItem(i, pEnumService[i].lpServiceName);
		str.Format(L"%d", pEnumService[i].ServiceStatusProcess.dwProcessId);
		m_ListCtrl.SetItemText(i, 1, str);
		m_ListCtrl.SetItemText(i, 2, GetStringByCurrentState(pEnumService[i].ServiceStatusProcess.dwCurrentState));
		m_ListCtrl.SetItemText(i, 3, pEnumService[i].lpDisplayName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

LPCTSTR CService::GetStringByCurrentState(DWORD state)
{
	switch (state)
	{
	case SERVICE_CONTINUE_PENDING:
		return L"��������";
	case SERVICE_PAUSE_PENDING:
		return L"��ͣ";
	case SERVICE_PAUSED:
		return L"����ͣ";
	case SERVICE_RUNNING:
		return L"��������";
	case SERVICE_START_PENDING:
		return L"��������";
	case SERVICE_STOP_PENDING:
		return L"����ֹͣ";
	case SERVICE_STOPPED:
		return L"��ֹͣ";
	default:
		break;
	}
	return L"";
}


void CService::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

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
		menu.AppendMenu(MF_STRING, ID_SUBMENU, L"��������");
		menu.AppendMenu(MF_STRING, ID_SUBMENU1, L"ֹͣ����");
		menu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

//�����Ӳ˵��¼�
void CService::OnRClickMenu(UINT id)
{
	//�򿪷����ȡ������
	CString serviceName = m_ListCtrl.GetItemText(m_nItem, 0);
	SC_HANDLE hService = OpenService(hSCM, serviceName, SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP);
	BOOL bRet = FALSE;
	switch (id)
	{
		//��������
	case ID_SUBMENU:
	{
		bRet = StartService(hService, NULL, NULL);
		if (bRet)
		{
			SERVICE_STATUS serviceStatus = { 0 };
			do {
				bRet = QueryServiceStatus(hService, &serviceStatus);
				//����(�����߳�),��λ����
				Sleep(300);
			} while (bRet && serviceStatus.dwCurrentState != SERVICE_RUNNING);
			m_ListCtrl.SetItemText(m_nItem, 2, GetStringByCurrentState(serviceStatus.dwCurrentState));
		}
	}
	break;
	//ֹͣ����
	case ID_SUBMENU1:
	{
		SERVICE_STATUS serviceStatus = { 0 };
		bRet = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
		if (bRet)
		{
			do {
				bRet = QueryServiceStatus(hService, &serviceStatus);
				//����(�����߳�),��λ����
				Sleep(300);
			} while (bRet && serviceStatus.dwCurrentState != SERVICE_STOPPED);
			m_ListCtrl.SetItemText(m_nItem, 2, GetStringByCurrentState(serviceStatus.dwCurrentState));
		}
	}
	break;
	default:
		break;
	}

	CloseServiceHandle(hService);
}

