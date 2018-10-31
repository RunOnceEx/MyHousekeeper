#pragma once
#define ID_SUBMENU  200
#define ID_SUBMENU1 201
#define ID_SUBMENU2 202

// CMenuProcess �Ի���

class CMenuProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CMenuProcess)

public:
	CMenuProcess(DWORD id,CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CMenuProcess();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	void ShowHeap(DWORD id);
	void ShowThread(DWORD id);
	void ShowModule(DWORD id);
	DWORD m_dwMenuId;
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickMenu(UINT id);
	INT m_nItem;
};
