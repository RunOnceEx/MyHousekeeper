// CPEFile.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CPEFile.h"
#include "afxdialogex.h"
#include "CAnalysisPE.h"


// CPEFile �Ի���

IMPLEMENT_DYNAMIC(CPEFile, CDialogEx)

CPEFile::CPEFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
	, m_strPath(_T(""))
{

}

CPEFile::~CPEFile()
{
}

void CPEFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPath);
}


BEGIN_MESSAGE_MAP(CPEFile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPEFile::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPEFile::OnClickedButton2)
END_MESSAGE_MAP()


// CPEFile ��Ϣ�������


void CPEFile::OnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//����ģ̬���ڣ�ѡ���ļ�
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.GetFolderPath() + "\\" + dlg.GetFileName();
		UpdateData(FALSE);
	}
}


void CPEFile::OnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//����ģ̬���ڣ�����PE�ļ�
	CAnalysisPE aly(m_strPath);
	INT IS = aly.DoModal();
}
