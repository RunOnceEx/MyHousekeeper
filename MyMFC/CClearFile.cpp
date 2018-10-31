// CClearFile.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CClearFile.h"
#include "afxdialogex.h"


// CClearFile �Ի���

IMPLEMENT_DYNAMIC(CClearFile, CDialogEx)

CClearFile::CClearFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
	, m_editStr(_T(""))
{

}

CClearFile::~CClearFile()
{
}

void CClearFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_editStr);
	
}


BEGIN_MESSAGE_MAP(CClearFile, CDialogEx)
	ON_COMMAND_RANGE(IDC_BUTTON1, IDC_BUTTON4, &CClearFile::OnClickBtn)
END_MESSAGE_MAP()


// CClearFile ��Ϣ�������
void CClearFile::OnClickBtn(UINT id)
{
	switch (id)
	{
	case IDC_BUTTON1:
	{
		CFolderPickerDialog fdlg;
		if (fdlg.DoModal() == IDOK)
		{
			m_editStr = fdlg.GetFolderPath();
			UpdateData(FALSE);
		}
	}
		break;
	case IDC_BUTTON2:
	{
		if (m_editStr == "")
		{
			MessageBox(L"·������Ϊ��");
			return;
		}
		EnumFoldAndDeleteFile(m_editStr);
		MessageBox(L"����ɹ�");
	}
		break;
	default:
		break;
	}
}

void CClearFile::EnumFoldAndDeleteFile(LPCWSTR szFoldPath)
{
	WIN32_FIND_DATA wfData = {};
	CString str;
	str.Format(L"%s%s", szFoldPath, L"\\*");
	HANDLE hFile = FindFirstFile(str, &wfData);
	do 
	{
		//ȥ����������Ŀ¼
		if (!wcscmp(wfData.cFileName, L".") || !wcscmp(wfData.cFileName, L".."))
		{
			continue;
		}
		WCHAR szSubFoldPath[MAX_PATH];
		wsprintf(szSubFoldPath, L"%s\\%s", szFoldPath, wfData.cFileName);
		//������ļ�������ɾ���ļ���������ݣ�ʹ�õݹ�
		if (wfData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EnumFoldAndDeleteFile(szSubFoldPath);
		}
		else
		{
			//ɾ�������ļ�
			LPWSTR fileSuffix = PathFindExtension(szSubFoldPath);
			if (!wcscmp(fileSuffix, L".tlog") || 
				!wcscmp(fileSuffix, L".obj") ||
				!wcscmp(fileSuffix, L".log") ||
				!wcscmp(fileSuffix, L".pch") ||
				!wcscmp(fileSuffix, L".ilk") ||
				!wcscmp(fileSuffix, L".pdb") ||
				!wcscmp(fileSuffix, L".tmp") ||
				!wcscmp(fileSuffix, L"._mp") ||
				!wcscmp(fileSuffix, L".gid") ||
				!wcscmp(fileSuffix, L".chk") ||
				!wcscmp(fileSuffix, L".old") ||
				!wcscmp(fileSuffix, L".bak"))
			{
				DeleteFile(szSubFoldPath);
			}
		}
	} while (FindNextFile(hFile, &wfData));
}
