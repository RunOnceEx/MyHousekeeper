// CDataDirectory.cpp: ʵ���ļ�
//

#include "stdafx.h"
#include "MyMFC.h"
#include "CDataDirectory.h"
#include "afxdialogex.h"

typedef struct TypeOffset
{
	WORD offset : 12; // (1) ��СΪ12Bit���ض�λƫ��
	WORD type : 4;// (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
}TypeOffset;


// CDataDirectory �Ի���

IMPLEMENT_DYNAMIC(CDataDirectory, CDialogEx)

CDataDirectory::CDataDirectory(LPBYTE pBuff, IMAGE_NT_HEADERS *pNt, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECTORY_DLG, pParent)
	, m_pNt(pNt)
	, m_pBuff(pBuff)
{

}

CDataDirectory::~CDataDirectory()
{
}

void CDataDirectory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CDataDirectory, CDialogEx)
END_MESSAGE_MAP()


// CDataDirectory ��Ϣ�������


BOOL CDataDirectory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//��ȡ����Ŀ¼��
	IMAGE_DATA_DIRECTORY *pData = (IMAGE_DATA_DIRECTORY*)(m_pNt->OptionalHeader.DataDirectory);
	HTREEITEM hRoot = m_treeCtrl.InsertItem(L"����Ŀ¼��", TVI_ROOT, TVI_LAST);
	WCHAR *szName[16] = { L"������", L"�����" , L"��Դ��", L"�쳣�������", L"��ȫ��"
		L"�ض�λ��" , L"������Ϣ��" , L"��Ȩ" , L"ȫ��ָ��" , L"TLS��", L"���ñ�",
		L"����Ŀ¼��", L"�����ַ��" , L"�ӳ������" , L"COM��Ϣ" };
	CString str;
	//������ֱ��RVA����С
	for (DWORD i = 0; i < m_pNt->OptionalHeader.NumberOfRvaAndSizes; i++, pData++)
	{
		//���Ϊ0˵��û�������
		if (pData->VirtualAddress == 0 && pData->Size == 0)
		{
			continue;
		}
		HTREEITEM hChild = m_treeCtrl.InsertItem(szName[i], hRoot, TVI_LAST);
		HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"RVA", hChild, TVI_LAST);
		str.Format(L"0x%x", pData->VirtualAddress);
		m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);

		HTREEITEM hChild2 = m_treeCtrl.InsertItem(L"Size", hChild, TVI_LAST);
		str.Format(L"0x%x", pData->Size);
		m_treeCtrl.InsertItem(str, hChild2, TVI_LAST);
	}

	//������
	HTREEITEM hRoot1 = m_treeCtrl.InsertItem(L"������", TVI_ROOT, TVI_LAST);
	//�Ȼ�õ������RVA
	DWORD expRva = m_pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	//RVAת���ļ�ƫ��
	DWORD expFoa = RvaToFoa(expRva);
	if (expFoa != -1)
	{
		//��ȡ������
		IMAGE_EXPORT_DIRECTORY *pExport = NULL;
		pExport = (IMAGE_EXPORT_DIRECTORY*)(m_pBuff + expFoa);

		HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"��������", hRoot1, TVI_LAST);
		str.Format(L"0x%x", pExport->NumberOfFunctions);
		m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);

		HTREEITEM hChild2 = m_treeCtrl.InsertItem(L"������������", hRoot1, TVI_LAST);
		str.Format(L"0x%x", pExport->NumberOfNames);
		m_treeCtrl.InsertItem(str, hChild2, TVI_LAST);

		HTREEITEM hChild3 = m_treeCtrl.InsertItem(L"������ַ��", hRoot1, TVI_LAST);
		HTREEITEM hChild4 = m_treeCtrl.InsertItem(L"�������Ʊ�", hRoot1, TVI_LAST);
		HTREEITEM hChild5 = m_treeCtrl.InsertItem(L"������ű�", hRoot1, TVI_LAST);
		DWORD *pEAT = (DWORD*)(RvaToFoa(pExport->AddressOfFunctions) + m_pBuff);
		DWORD *pENT = (DWORD*)(RvaToFoa(pExport->AddressOfNames) + m_pBuff);
		WORD *pEOT = (WORD*)(RvaToFoa(pExport->AddressOfNameOrdinals) + m_pBuff);
		for (DWORD i = 0; i < pExport->NumberOfNames; i++)
		{
			str.Format(L"0x%x", pEAT[i]);
			m_treeCtrl.InsertItem(str, hChild3, TVI_LAST);

			CHAR *szCh= (CHAR*)(RvaToFoa(pENT[i]) + m_pBuff);
			USES_CONVERSION;
			m_treeCtrl.InsertItem(A2W(szCh), hChild4, TVI_LAST);

			str.Format(L"%d", pEOT[i]);
			m_treeCtrl.InsertItem(str, hChild5, TVI_LAST);
		}
	}

	//�����
	HTREEITEM hRoot2 = m_treeCtrl.InsertItem(L"�����", TVI_ROOT, TVI_LAST);
	//�Ȼ�õ�����RVA
	DWORD impRva = m_pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	//ת����FOA
	DWORD impFoa = RvaToFoa(impRva);
	if (impFoa != -1)
	{
		//��ȡ�������׵�ַ
		IMAGE_IMPORT_DESCRIPTOR *pImp = NULL;
		pImp = (IMAGE_IMPORT_DESCRIPTOR*)(m_pBuff + impFoa);

		//��0��Ϊ��β
		while (pImp->Name != 0)
		{
			//Name��һ��RVAתΪFOA
			CHAR* szName = (CHAR*)(RvaToFoa(pImp->Name) + m_pBuff);
			//�����ּ������ؼ�
			HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"DLL", hRoot2, TVI_LAST);
			USES_CONVERSION;
			m_treeCtrl.InsertItem(A2W(szName), hChild1, TVI_LAST);

			//��ȡINT���׵�ַ��Ҳ��һ��RVA��ҪתFOA
			IMAGE_THUNK_DATA *pInt = (IMAGE_THUNK_DATA*)(RvaToFoa(pImp->OriginalFirstThunk) + m_pBuff);
			while (pInt->u1.Function != 0)
			{
				//���жϵ���ķ�ʽ������Ż�������
				if (IMAGE_SNAP_BY_ORDINAL(pInt->u1.Ordinal))
				{
					str.Format(L"0x%04x", pInt->u1.Ordinal & 0xffff);
					m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);
				}
				else
				{
					IMAGE_IMPORT_BY_NAME *pIntName = (IMAGE_IMPORT_BY_NAME*)
						(RvaToFoa(pInt->u1.AddressOfData) + m_pBuff);
					USES_CONVERSION;
					m_treeCtrl.InsertItem(A2W(pIntName->Name), hChild1, TVI_LAST);
				}
				pInt++;
			}

			pImp++;
		}
	}

	//��Դ��
	hRoot3 = m_treeCtrl.InsertItem(L"��Դ��", TVI_ROOT, TVI_LAST);
	//�Ȼ�ȡ��Դ��RVA
	DWORD resRva = m_pNt->OptionalHeader.DataDirectory[2].VirtualAddress;
	//ת��FOA
	DWORD resFoa = RvaToFoa(resRva);
	if (resFoa != -1)
	{
		//��ȡ��Դ��
		IMAGE_RESOURCE_DIRECTORY *pRes = NULL;
		pRes = (IMAGE_RESOURCE_DIRECTORY*)(m_pBuff + resFoa);
		ParseResources((LPBYTE)pRes, pRes);

	}

	//�ض�λ��
	HTREEITEM hRoot4 = m_treeCtrl.InsertItem(L"�ض�λ��", TVI_ROOT, TVI_LAST);
	//��ȡ�ض�λ��RVA
	DWORD reloRva = m_pNt->OptionalHeader.DataDirectory[5].VirtualAddress;
	DWORD reloFoa = RvaToFoa(reloRva);
	if (reloFoa != -1)
	{
		IMAGE_BASE_RELOCATION* pRelo = (IMAGE_BASE_RELOCATION*)
			(m_pBuff + reloFoa);
		while (pRelo->VirtualAddress != 0)
		{
			//��ȡtypeoffset��ַ������
			TypeOffset *pTypeOffset = (TypeOffset *)(pRelo + 1);
			DWORD dwCount = (pRelo->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
			
			HTREEITEM hChild1 = m_treeCtrl.InsertItem(L"ҳ��RVA", hRoot4, TVI_LAST);
			str.Format(L"0x%x", pRelo->VirtualAddress);
			m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);

			for (DWORD i = 0; i < dwCount; ++i)
			{
				m_treeCtrl.InsertItem(L"ҳ��ƫ��", hChild1, TVI_LAST);
				str.Format(L"0x%x", pTypeOffset[i].offset);
				m_treeCtrl.InsertItem(str, hChild1, TVI_LAST);
			}
			pRelo = (IMAGE_BASE_RELOCATION*)((LPBYTE)pRelo + pRelo->SizeOfBlock);
		}
	}

	//�ӳټ��ر�
	 m_treeCtrl.InsertItem(L"�ӳټ��ر�", TVI_ROOT, TVI_LAST);

	//TLS��
	HTREEITEM hRoot5 = m_treeCtrl.InsertItem(L"TLS��", TVI_ROOT, TVI_LAST);
	//��ȡTLS��RVA
	DWORD tlsRva = m_pNt->OptionalHeader.DataDirectory[9].VirtualAddress;
	DWORD tlsFoa = RvaToFoa(tlsRva);
	if (tlsFoa != -1)
	{
		IMAGE_TLS_DIRECTORY *pTls = (IMAGE_TLS_DIRECTORY*)
			(m_pBuff + tlsFoa);
		m_treeCtrl.InsertItem(L"Դ������ʼλ��", hRoot5, TVI_LAST);
		str.Format(L"0x%x", pTls->StartAddressOfRawData);
		m_treeCtrl.InsertItem(str, hRoot5, TVI_LAST);

		m_treeCtrl.InsertItem(L"Դ������ֹλ��", hRoot5, TVI_LAST);
		str.Format(L"0x%x", pTls->EndAddressOfRawData);
		m_treeCtrl.InsertItem(str, hRoot5, TVI_LAST);

		m_treeCtrl.InsertItem(L"�ص�������ַ��λ��", hRoot5, TVI_LAST);
		str.Format(L"0x%x", pTls->AddressOfCallBacks);
		m_treeCtrl.InsertItem(str, hRoot5, TVI_LAST);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//��������ַת�ļ�ƫ��
DWORD CDataDirectory::RvaToFoa(DWORD dwRva)
{
	IMAGE_SECTION_HEADER *pSection = (IMAGE_SECTION_HEADER*)IMAGE_FIRST_SECTION(m_pNt);
	for (DWORD i = 0; i < m_pNt->FileHeader.NumberOfSections; i++)
	{
		if (dwRva >= pSection[i].VirtualAddress && dwRva <= pSection[i].VirtualAddress
			+ pSection[i].SizeOfRawData)
		{
			return dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return -1;
}

void CDataDirectory::ParseResources(LPBYTE pResRoot, IMAGE_RESOURCE_DIRECTORY* pRes, int nLevel)
{
	//�Ȼ�ȡ��һ����Դ���ܸ���
	DWORD dwEntryCount = pRes->NumberOfIdEntries + pRes->NumberOfNamedEntries;

	//��ȡ����һ��Ŀ¼��ڵ��׵�ַ
	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry = NULL;
	pEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRes + 1);
	CString str;
	if (nLevel == 1)
	{
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			//�ж�id�ǲ���һ���ַ��� Ϊ1���ַ���
			if (pEntry[i].NameIsString)
			{
				IMAGE_RESOURCE_DIR_STRING_U *pStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].OffsetToData + pResRoot);
				hResChild1 = m_treeCtrl.InsertItem(L"��Դ����", hRoot3, TVI_LAST);
				m_treeCtrl.InsertItem(pStr->NameString, hResChild1, TVI_LAST);
			}
			else/*id��һ������*/
			{
				char* pIdName[] =
				{
					"",
					"���ָ�루Cursor��",
					"λͼ��Bitmap��",
					"ͼ�꣨Icon��",
					"�˵���Menu��",
					"�Ի���Dialog��",
					"�ַ����б�String Table��",
					"����Ŀ¼��Font Directory��",
					"���壨Font��",
					"��ݼ���Accelerators��",
					"�Ǹ�ʽ����Դ��Unformatted��",
					"��Ϣ�б�Message Table��",
					"���ָ���飨Group Cursor��",
					"",
					"ͼ���飨Group Icon��",
					"",
					"�汾��Ϣ��Version Information��"
				};
				USES_CONVERSION;
				if (pEntry[i].Id >= _countof(pIdName))
				{
					str.Format(L"%d", pEntry[i].Id);
					hResChild1 = m_treeCtrl.InsertItem(L"��ԴID", hRoot3, TVI_LAST);
					m_treeCtrl.InsertItem(str, hResChild1, TVI_LAST);
				}
				else
				{
					hResChild1 = m_treeCtrl.InsertItem(L"��ԴID", hRoot3, TVI_LAST);
					m_treeCtrl.InsertItem(A2W(pIdName[pEntry[i].Id]), hResChild1, TVI_LAST);
				}
			}
			//����Դ��Ŀ¼Ϊƫ�ƣ��ҵ���һ��Ŀ¼
			IMAGE_RESOURCE_DIRECTORY *pRes2 = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResRoot);
			ParseResources(pResRoot, pRes2, 2);
		}
	}
	else if (nLevel == 2)
	{
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			if (pEntry[i].NameIsString)
			{
				IMAGE_RESOURCE_DIR_STRING_U* pIdStr = (IMAGE_RESOURCE_DIR_STRING_U*)
					(pEntry[i].NameOffset + pResRoot);
				hResChild2 = m_treeCtrl.InsertItem(L"��ԴID", hResChild1, TVI_LAST);
				m_treeCtrl.InsertItem(pIdStr->NameString, hResChild2, TVI_LAST);
			}
			else
			{
				str.Format(L"%d", pEntry[i].Id);
				hResChild2 = m_treeCtrl.InsertItem(L"��ԴID", hResChild1, TVI_LAST);
				m_treeCtrl.InsertItem(str, hResChild2, TVI_LAST);
			}
			// 2. ƫ��(����Դ��Ŀ¼ƫ��, ƫ�Ƶ�������)
			IMAGE_RESOURCE_DIRECTORY* pRes3 = (IMAGE_RESOURCE_DIRECTORY*)
				(pEntry[i].OffsetToDirectory + pResRoot);
			// �ҵ���3��Ŀ¼���׵�ַ
			ParseResources(pResRoot, pRes3, 3);
		}
	}
	else if (nLevel == 3)
	{
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			// ����������
			// 1. id(û��ʲô��)
			// 2. ƫ��(����Դ��Ŀ¼Ϊƫ��, ƫ�Ƶ��������)
			IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)
				(pEntry[i].OffsetToData + pResRoot);
			HTREEITEM hResChild3 = m_treeCtrl.InsertItem(L"��Դ��С��λ��", hResChild2, TVI_LAST);
			str.Format(L"%d", pDataEntry->Size);
			m_treeCtrl.InsertItem(str, hResChild3, TVI_LAST);
			str.Format(L"%d", pDataEntry->OffsetToData);
			m_treeCtrl.InsertItem(str, hResChild3, TVI_LAST);
		}
	}
}
