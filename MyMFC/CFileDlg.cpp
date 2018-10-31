// CFileDlg.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CFileDlg.h"
#include "afxdialogex.h"


// CFileDlg �Ի���

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_ComBox);
}

BOOL  CFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//���ڳ�ʼ��������
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();

	m_ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_ListCtrl.InsertColumn(0, _T("�ļ���"), 0, 150);
	m_ListCtrl.InsertColumn(1, _T("����ʱ��"), 0, 100);
	m_ListCtrl.InsertColumn(2, _T("�޸�ʱ��"), 0, 100);
	m_ListCtrl.InsertColumn(3, _T("�ļ���С"), 0, 100);
	
	//��ȡ�����ϵ������̷� ������ʾ��combox��
	TCHAR szName[MAX_PATH] = { 0 };

	GetLogicalDriveStrings(MAX_PATH, szName);

	for (int i = 0; szName[i] != '\0' && i < MAX_PATH; ++i)
	{
		TCHAR szTmp[12] = { 0 };
		wcscpy_s(szTmp, &szName[i]);
		CString str;
		str.Format(_T("%s"), szTmp);
		m_ComBox.AddString(str);
		i += wcslen(&szName[i]);
	}
	m_ComBox.SetCurSel(0);
	CString tmp;
	m_ComBox.GetLBText(0, tmp);
	ShowFile(tmp);

	return TRUE;
}

void CFileDlg::ShowFile(CString str)
{
	//���ݴ��������̷������ļ�
	WIN32_FIND_DATA fData;
	HANDLE  hFile = FindFirstFile(str + L"*", &fData);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			m_ListCtrl.InsertItem(0, fData.cFileName);
			//�ļ�����ʱ��
			CTime tm(fData.ftCreationTime);
			CString str = tm.Format("%Y-%m-%d");
			m_ListCtrl.SetItemText(0, 1, str);
			//�ļ��޸�ʱ��
			CTime p = (fData.ftLastWriteTime);
			str = p.Format("%Y-%m-%d");
			m_ListCtrl.SetItemText(0, 2, str);
			//�ļ���С
			LONGLONG s = (fData.nFileSizeHigh << 32) + fData.nFileSizeLow;
			s = s / 1024;
			str.Format(L"%lld KB", s);
			m_ListCtrl.SetItemText(0, 3, str);
		} while (FindNextFile(hFile, &fData));
	}
}

BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFileDlg::OnSelchangeCombo1)
END_MESSAGE_MAP()


// CFileDlg ��Ϣ�������


void CFileDlg::OnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ListCtrl.DeleteAllItems();
	int nNum = m_ComBox.GetCurSel();
	CString tmp;
	m_ComBox.GetLBText(nNum, tmp);
	ShowFile(tmp);
}
