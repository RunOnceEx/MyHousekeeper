// CAnalysisPE.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CAnalysisPE.h"
#include "afxdialogex.h"
#include "CPESection.h"
#include "CDataDirectory.h"


// CAnalysisPE �Ի���

IMPLEMENT_DYNAMIC(CAnalysisPE, CDialogEx)

CAnalysisPE::CAnalysisPE(CString path, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ANALYSISPE_DLG, pParent),
	m_strPath(path)
	, m_pNt(NULL)
	, m_strData1(_T(""))
	, m_strData2(_T(""))
	, m_strData3(_T(""))
	, m_strData4(_T(""))
	, m_strData5(_T(""))
	, m_strData6(_T(""))
	, m_strData7(_T(""))
	, m_strData8(_T(""))
	, m_strData9(_T(""))
	, m_strData10(_T(""))
	, m_strData11(_T(""))
	, m_strData12(_T(""))
{

}

CAnalysisPE::~CAnalysisPE()
{
}

void CAnalysisPE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strData1);
	DDX_Text(pDX, IDC_EDIT2, m_strData2);
	DDX_Text(pDX, IDC_EDIT3, m_strData3);
	DDX_Text(pDX, IDC_EDIT4, m_strData4);
	DDX_Text(pDX, IDC_EDIT5, m_strData5);
	DDX_Text(pDX, IDC_EDIT6, m_strData6);
	DDX_Text(pDX, IDC_EDIT7, m_strData7);
	DDX_Text(pDX, IDC_EDIT8, m_strData8);
	DDX_Text(pDX, IDC_EDIT9, m_strData9);
	DDX_Text(pDX, IDC_EDIT10, m_strData10);
	DDX_Text(pDX, IDC_EDIT11, m_strData11);
	DDX_Text(pDX, IDC_EDIT12, m_strData12);
}


BEGIN_MESSAGE_MAP(CAnalysisPE, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnalysisPE::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnalysisPE::OnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAnalysisPE ��Ϣ�������


BOOL CAnalysisPE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//����DOSͷNTͷ
	//���ļ�
	m_hFile = CreateFile(m_strPath, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"�ļ�������");
		return 0;
	}

	DWORD size = GetFileSize(m_hFile, NULL);
	m_pBuff = new BYTE[size];
	DWORD dwRead = 0;
	ReadFile(m_hFile, m_pBuff, size, &dwRead, 0);

	//ʹ�ö�Ӧ�ṹ������ڴ��е�����
	IMAGE_DOS_HEADER *pDos = NULL;
	pDos = (IMAGE_DOS_HEADER *)m_pBuff;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(L"������Ч��PE��ʽ");
		return 0;
	}

	//�ҵ�NTͷ
	m_pNt = (IMAGE_NT_HEADERS*)(m_pBuff + pDos->e_lfanew);
	if (m_pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"������ЧPE��ʽ");
		return 0;
	}

	//�ҵ��ļ�ͷ
	IMAGE_FILE_HEADER *pFileHeader = NULL;
	pFileHeader = (IMAGE_FILE_HEADER*)&m_pNt->FileHeader;
	
	//�ҵ���չͷ
	IMAGE_OPTIONAL_HEADER *pOptionHeader = NULL;
	pOptionHeader = (IMAGE_OPTIONAL_HEADER*)&m_pNt->OptionalHeader;

	m_strData1.Format(L"0x%x", pFileHeader->NumberOfSections);
	m_strData2.Format(L"0x%x", pOptionHeader->Magic);
	m_strData3.Format(L"0x%x", pFileHeader->SizeOfOptionalHeader);
	m_strData4.Format(L"0x%x", pFileHeader->Characteristics);
	m_strData5.Format(L"0x%x", pOptionHeader->SizeOfCode);
	m_strData6.Format(L"0x%x", pOptionHeader->AddressOfEntryPoint);
	m_strData7.Format(L"0x%x", pOptionHeader->ImageBase);
	m_strData8.Format(L"0x%x", pOptionHeader->SectionAlignment);
	m_strData9.Format(L"0x%x", pOptionHeader->FileAlignment);
	m_strData10.Format(L"0x%x", pOptionHeader->SizeOfImage);
	m_strData11.Format(L"0x%x", pOptionHeader->SizeOfHeaders);
	m_strData12.Format(L"0x%x", pOptionHeader->NumberOfRvaAndSizes);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//��������Ϣ
void CAnalysisPE::OnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPESection sec(m_pNt);
	sec.DoModal();
}

//����Ŀ¼����Ϣ
void CAnalysisPE::OnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDataDirectory data(m_pBuff, m_pNt);
	data.DoModal();
}


void CAnalysisPE::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
	CloseHandle(m_hFile);
}
