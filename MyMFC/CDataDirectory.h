#pragma once


// CDataDirectory �Ի���

class CDataDirectory : public CDialogEx
{
	DECLARE_DYNAMIC(CDataDirectory)

public:
	CDataDirectory(LPBYTE pBuff, IMAGE_NT_HEADERS *pNt, CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CDataDirectory();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTORY_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	IMAGE_NT_HEADERS * m_pNt;
	virtual BOOL OnInitDialog();
	DWORD RvaToFoa(DWORD dwRva);
	void ParseResources(LPBYTE pResRoot, IMAGE_RESOURCE_DIRECTORY* pRes, int nLevel = 1);
	CTreeCtrl m_treeCtrl;
	LPBYTE m_pBuff;
	HTREEITEM hRoot3;
	HTREEITEM hResChild1;
	HTREEITEM hResChild2;
};
