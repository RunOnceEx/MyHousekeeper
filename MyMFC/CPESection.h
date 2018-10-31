#pragma once


// CPESection �Ի���

class CPESection : public CDialogEx
{
	DECLARE_DYNAMIC(CPESection)

public:
	CPESection(IMAGE_NT_HEADERS *pNt, CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CPESection();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECTION_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	IMAGE_NT_HEADERS * m_pNt;
	CListCtrl m_ListCtrl;
	virtual BOOL OnInitDialog();
};
